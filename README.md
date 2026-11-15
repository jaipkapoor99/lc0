[![CircleCI](https://circleci.com/gh/LeelaChessZero/lc0.svg?style=shield)](https://circleci.com/gh/LeelaChessZero/lc0)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/3245b83otdee7oj7?svg=true)](https://ci.appveyor.com/project/leelachesszero/lc0)

# Lc0

Lc0 is a UCI-compliant chess engine designed to play chess via neural network, specifically those of the [LeelaChessZero project](https://lczero.org).

## Downloading source

Lc0 can be acquired either via a git clone or an archive download from GitHub.

For essentially all purposes, including selfplay game generation and match play, we highly recommend using the latest `release/version` branch (for example `release/0.32`), which is equivalent to using the latest version tag.

Versioning follows the Semantic Versioning guidelines, with major, minor and patch sections. The training server enforces game quality using the versions output by the client and engine.

Download using git:

```shell
git clone -b release/0.32 https://github.com/LeelaChessZero/lc0.git
```

If you have cloned already an old version, fetch, view and checkout a new branch:
```shell
git fetch --all
git branch --all
git checkout -t remotes/origin/release/0.32
```

If you prefer to download an archive:
 * Download the [.zip](https://api.github.com/repos/LeelaChessZero/lc0/zipball/release/0.32) file ([.tar.gz](https://api.github.com/repos/LeelaChessZero/lc0/tarball/release/0.32) archive is also available)
 * Extract

Having successfully acquired Lc0 via either of these methods, proceed to the build section below and follow the instructions for your OS.


## Building and running Lc0

Building Lc0 is straightforward using **CMake** (3.28+) and a **C++23** capable compiler (GCC 15+, Clang 18+, or MSVC from Visual Studio 2022 17.8+).

Dependencies (Abseil, Eigen, CUTLASS, ZLIB, and GoogleTest) are automatically managed via CMake's `FetchContent` or discovered from your system if installed.

### Quick Start with CMake Presets

Lc0 provides unified presets in `CMakePresets.json`:

```shell
# Linux (GCC 15, C++23, CUDA/CUTLASS, Native optimizations)
cmake --preset linux-gcc15-release
cmake --build --preset linux-release -j

# Windows (MSVC, C++23, CUDA, CUTLASS)
cmake --preset windows-msvc-cuda
cmake --build --preset windows-cuda -j

# Generic CPU Release (C++23, BLAS / Eigen fallback)
cmake --preset generic-release
cmake --build --preset generic -j
```

### Manual Configuration

#### Linux

1. Install dependencies:
   - C++23 compiler: GCC 15 (`gcc-15`, `g++-15`) or Clang 18+
   - CMake 3.28+ and Ninja: `apt-get install -y cmake ninja-build`
   - Optional: NVIDIA CUDA Toolkit (v12.x or v13.x) for GPU evaluation, `libopenblas-dev` for CPU BLAS
2. Configure and build:
   ```shell
   cmake -B build/release -G Ninja \
     -DCMAKE_BUILD_TYPE=Release \
     -DCMAKE_CXX_STANDARD=23 \
     -DENABLE_CUDA=ON \
     -DENABLE_CUTLASS=ON \
     -DENABLE_TESTS=ON
   cmake --build build/release -j $(nproc)
   ```
3. Run tests:
   ```shell
   ctest --test-dir build/release --output-on-failure
   ```
4. Download a [neural network weights file](https://lczero.org/play/networks/bestnets/) and place it in the same directory as the executable.

#### Windows

1. Install Visual Studio 2022 (version 17.8 or newer) with C++ Desktop Development and CMake tools.
2. Install [CUDA Toolkit](https://developer.nvidia.com/cuda-downloads) (v12.0 or newer).
3. Open a Developer PowerShell / Command Prompt and run:
   ```powershell
   cmake -B build -G "Visual Studio 17 2022" -A x64 `
     -DCMAKE_BUILD_TYPE=Release `
     -DCMAKE_CXX_STANDARD=23 `
     -DENABLE_CUDA=ON `
     -DENABLE_CUTLASS=ON `
     -DENABLE_TESTS=ON
   cmake --build build --config Release --parallel $env:NUMBER_OF_PROCESSORS
   ```
4. The executable will be at `build/Release/lc0.exe`.

#### macOS (Apple Silicon)

1. Install Xcode Command Line Tools, CMake, and Ninja (`brew install cmake ninja`).
2. Configure and build:
   ```shell
   cmake -B build -G Ninja \
     -DCMAKE_BUILD_TYPE=Release \
     -DCMAKE_CXX_STANDARD=23 \
     -DENABLE_CUDA=OFF \
     -DENABLE_TESTS=ON
   cmake --build build --parallel $(sysctl -n hw.ncpu)
   ``` 

Starting with v0.32.0, we are also offering a pre-compiled version that can be downloaded from the [release page](https://github.com/LeelaChessZero/lc0/releases).

### CUDA

CUDA can be downloaded and installed following the instructions in from <https://developer.nvidia.com/cuda-downloads>. The build in most cases will pick it up with no further action. However if the cuda compiler (`nvcc`) is not found you can call the build like this: `PATH=/usr/local/cuda/bin:$PATH ./build.sh`, replacing the path with the correct one for `nvcc`.

*Note* that CUDA uses the system compiler and stops if it doesn't recognize the version, even if newer. This is more of an issue with new Linux versions, but you can get around with the `nvcc_ccbin` build option to specify a different compiler just for cuda. As an example, adding `-Dnvcc_ccbin=g++-11` to the build command line will use g++-11 with cuda instead of the system compiler.

### ONNX

Lc0 offers several ONNX based backends, namely onnx-cpu, onnx-cuda, onnx-trt, onnx-rocm and on Windows onnx-dml, utilizing the execution providers offered by onnxruntime.

Some Linux systems are starting to offer onnxruntime packages, so after installing this there is a good chance the Lc0 build will pick it up with no further action required. Otherwise you can set the `onnx_libdir` and `onnx_include` build options to point to the onnxruntime libraries and include directories respectively. The same options are used if you unpack a package downloaded from <https://github.com/microsoft/onnxruntime/releases>.

For Windows, we offer pre-compiled packages for onnx-dml and onnx-trt, see the included README for installation instructions.

### SYCL

*Note* that SYCL support is new in v0.32.0 and as such is still considered experimental.

You will need the Intel "oneAPI DPC++/C++ Compiler", "DPC++ Compatibility Tool" and (for an Intel GPU) "oneAPI Math Kernel Library (oneMKL)" or (for an AMD GPU) hipBLAS.

The Intel tools can be found in either the "oneAPI Base Toolkit" or "C++ Essentials" packages that can be downloaded from
<https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html>, while hipBLAS can be downloaded from
<https://rocm.docs.amd.com/projects/hipBLAS/en/latest/>

The compiler for C code is icx and for C++ code is icx on Windows but icpx on Linux.

To build Lc0 with SYCL you need to set the `sycl` build option using `-Dsycl=l0` (that is el zero) for an Intel GPU or `-Dsycl=amd` for (you guessed it) an AMD GPU.

You may also have to set the `dpct_include` option to point to the DPC++ Compatibility Tool includes, the `onemkl_include` similarly for the oneMKL includes, or `hip_libdirs` and `hip_include` to the AMD HIP libraries and includes respectively.

On Linux, a typical session would go like this:
```shell
. /opt/intel/oneapi/setvars.sh --include-intel-llvm
CC=icx CXX=icpx AR=llvm-ar ./build.sh release -Dgtest=false -Dsycl=l0
```
The first line is to initialize the build environment and is only needed once per session, while the build line may need modification as described above.

On windows you will have to build using `ninja`, this is provided by Visual Studio if you install the CMake component. We provide a `build-sycl.cmd` script that should build just fine for an Intel GPU. This script has not yet been tested with and AMD GPU, some editing will be required.

You can also install the [oneAPI DPC++/C++ Compiler Runtime](https://www.intel.com/content/www/us/en/developer/articles/tool/compilers-redistributable-libraries-by-version.html) so you can run Lc0 without needing to initialize the build environment every time.

### HIP (ROCm)

The `hip`, `hip-fp16` and `hip-auto` backends run lc0's own CUDA NN-backend kernels (the cuBLAS + custom-kernel path) on AMD GPUs by compiling them with `hipcc`, so AMD users get a first-class ROCm path without the SYCL/DPC++ toolchain. This is distinct from the SYCL backend above (a different toolchain) and from `onnx-rocm` (which runs the network inside onnxruntime rather than lc0's own kernels). cuDNN/MIOpen and the optional CUTLASS fused-MHA are not used; the cuBLAS attention fallback runs in their place.

You will need a [ROCm](https://rocm.docs.amd.com/) installation providing `hipcc` and hipBLAS. Enable the backend with `-Dhip=true`. The target GPU architecture is taken from `-Damd_gfx` (e.g. `-Damd_gfx=gfx90a`); if it is omitted, the first architecture reported by `rocm_agent_enumerator` is used, which needs meson 1.2.0 or newer. There is no default architecture: when nothing can be detected the build stops and asks for an explicit `-Damd_gfx`, rather than guessing an architecture the machine may not have. Use `hip_libdirs` and `hip_include` if the ROCm libraries and headers are not in the default search path. lc0's HIP build links the kernels objects (built by `hipcc`) into a non-LTO host build, so pass `-Db_lto=false`.

A typical session on Linux:
```shell
meson setup build-hip -Dhip=true -Damd_gfx=gfx90a \
  -Dplain_cuda=false -Dcudnn=false -Dcutlass=false -Dnvcc=false \
  -Dgtest=true -Dblas=true -Db_lto=false \
  -Dhip_libdirs=/opt/rocm/lib -Dhip_include=/opt/rocm/include
ninja -C build-hip
```
Then select the backend at runtime with `--backend=hip` (or `hip-fp16` for the half-precision path). The HIP backends have been validated on Linux with ROCm on gfx90a (CDNA2) and gfx1100 (RDNA3).

### BLAS

Lc0 can also run (a bit slow) on CPU, using matrix multiplication functions from a BLAS library. By default OpenBLAS is used if available as it seems to offer good performance on a wide range of processors. If your system doesn't offer an OpenBLAS package (e.g. `libopenblas-dev`), or you have a recent processor you can get DNNL from [here](<https://github.com/uxlfoundation/oneDNN/releases/v2.2>). To use DNNL you have to pass `-Ddnnl=true` to the build and specify the directory where it was installed using the `-Ddnnl_dir=` option. For macs, the Accelerate library will be used.

If the "Intel Implicit SPMD Program Compiler" (`ispc`) is [installed](<https://ispc.github.io/downloads.html>), some performance critical functions will use vectorized code for faster execution. 

*Note* that Lc0 is not able to control the number of threads with all BLAS libraries. Some libraries try to exploit cores aggressively, in which case it may be best to leave the threads set to the default (i.e. automatic) setting.

## Getting help

If there is an issue or the above instructions were not clear, you can always ask for help. The fastest way is to ask in the help channel of our [discord chat](http://lc0.org/chat), but you can also open a [github issue](https://github.com/LeelaChessZero/lc0/issues) (after checking the issue hasn't already been reported).

## Python bindings

Python bindings can be built and installed as follows.

```shell
pip install --user git+https://github.com/LeelaChessZero/lc0.git
```

This will build the package `lczero-bindings` and install it to your Python user install directory.
All the `lc0` functionality related to position evaluation is now available in the module `lczero.backends`.
An example interactive session can be found [here](https://github.com/LeelaChessZero/lc0/pull/1261#issuecomment-622951248).

## License

Leela Chess is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Leela Chess is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Leela Chess.  If not, see <http://www.gnu.org/licenses/>.

### Additional permission under GNU GPL version 3 section 7

_The source files of Lc0 with the exception of the BLAS, OpenCL and SYCL
backends (all files in the `blas`, `opencl` and `sycl` sub-directories) have
the following additional permission, as allowed under GNU GPL version 3
section 7:_

If you modify this Program, or any covered work, by linking or
combining it with NVIDIA Corporation's libraries from the NVIDIA CUDA
Toolkit and the NVIDIA CUDA Deep Neural Network library (or a
modified version of those libraries), containing parts covered by the
terms of the respective license agreement, the licensors of this
Program grant you additional permission to convey the resulting work.

