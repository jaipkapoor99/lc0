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

CUDA can be downloaded and installed from <https://developer.nvidia.com/cuda-downloads>. CMake's `enable_language(CUDA)` and `FindCUDAToolkit` will automatically detect `nvcc` and cuBLAS. If `nvcc` is not on your `PATH`, pass `-DCMAKE_CUDA_COMPILER=/path/to/nvcc` to `cmake`.

- Target GPU architecture defaults to `native` (`CMAKE_CUDA_ARCHITECTURES=native`) for peak SASS performance. Override with e.g. `-DCMAKE_CUDA_ARCHITECTURES=89` or `120` when cross-compiling.
- CUTLASS fused multi-head attention (FMHA) is enabled by default (`-DENABLE_CUTLASS=ON`).
- Available runtime backends: `cuda-auto`, `cuda`, `cuda-fp16`, and `cuda-bf16` (for Ampere / Blackwell GPUs).

### BLAS / CPU

Lc0 can also run on CPU using OpenBLAS/Accelerate (`-DENABLE_BLAS=ON`) or the built-in Eigen and handcrafted `trivial` evaluators (`-DENABLE_BLAS=OFF`). On macOS, Apple's Accelerate framework is used automatically when `ENABLE_BLAS=ON`.

*Note*: Additional non-NVIDIA GPU/NPU backend sources (`onnx` for DirectML/CoreML/ROCm/MIGraphX, `metal` for Apple Silicon GPU, and `hip_compat.h` in `cuda` for AMD ROCm/HIP) remain in `src/neural/backends/` alongside the modern CMake `CUDA`/`CUTLASS` (Transformer-only) and `BLAS`/`Eigen`/CPU backends.

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

_The source files of Lc0 with the exception of the BLAS
backend (all files in the `blas` sub-directory) have
the following additional permission, as allowed under GNU GPL version 3
section 7:_

If you modify this Program, or any covered work, by linking or
combining it with NVIDIA Corporation's libraries from the NVIDIA CUDA
Toolkit and the NVIDIA CUDA Deep Neural Network library (or a
modified version of those libraries), containing parts covered by the
terms of the respective license agreement, the licensors of this
Program grant you additional permission to convey the resulting work.

