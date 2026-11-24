Native Transformer-only CUDA / HIP / CUTLASS backend for lc0. Brief description of core files:

1. network_cuda.cc   -> Implementation of CudaNetwork, backend factory, and engine registration (cuda, cuda-fp16, cuda-bf16).
2. layers.cc         -> Neural network layer implementations (AttentionBody, EncoderBlock, AttentionPolicyHead, ValueHead, EmbeddingLayer, FCLayer, PolicyMapLayer).
3. layers.h          -> Header file for layer classes and base layer abstractions.
4. kernels.h         -> Function declarations for device CUDA kernels.
5. common_kernels.cu -> Core CUDA kernels (FP32/FP16/BF16 transforms, activations, LayerNorm, Softmax, embeddings, policy maps).
6. cutlass_kernels.cu-> NVIDIA CUTLASS accelerated fused multi-head attention (FMHA) kernels.
7. cuda_common.h     -> CUDA device error checking, memory management, and stream abstractions.
8. hip_compat.h      -> ROCm / HIP compatibility mappings for AMD GPU interoperability.

High level overview:
The network is composed of layer objects executed sequentially. GEMM operations are accelerated via cuBLAS and CUTLASS, with elementwise and custom operations running on optimized native CUDA kernels.

Pipeline:
lc0 search -> CudaNetwork -> layers -> cuBLAS / CUTLASS / custom CUDA kernels
