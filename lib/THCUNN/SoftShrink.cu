#include "THCUNN.h"
#include "common.h"

struct SoftShrinkUpdateOutput
{
  const float lambda_;

  __host__ __device__
  SoftShrinkUpdateOutput(float lambda)
    : lambda_(lambda)
  {}

  __device__ __forceinline__ void operator()(float *out, float *in)
  {
    float x = *in;
    if (x > lambda_) *out = x - lambda_;
    else if (x < -lambda_) *out = x + lambda_;
    else *out = 0;
  }

  __host__ __device__
  ~SoftShrinkUpdateOutput() {}
};

void THNN_CudaSoftShrink_updateOutput(THCState *state, THCudaTensor *input, THCudaTensor *output, double lambda)
{
  THCUNN_assertSameGPU(state, 2, input, output);
  THCudaTensor_resizeAs(state, output, input);
  stub_THC_pointwiseApply2(state, output, input, SoftShrinkUpdateOutput(lambda));
  THCudaCheck(hipGetLastError());
}

struct SoftShrinkUpdateGradInput
{
  const float lambda_;

  __host__ __device__
  SoftShrinkUpdateGradInput(float lambda)
    : lambda_(lambda)
  {}

  __device__ __forceinline__ void operator()(float *gradInput, float *input, float *gradOutput) const
  {
    float x = *input;
    if (x > lambda_ || x < -lambda_)
      *gradInput = *gradOutput;
    else
      *gradInput = 0;
  }

  __host__ __device__
  ~SoftShrinkUpdateGradInput() {}
};


void THNN_CudaSoftShrink_updateGradInput(THCState *state, THCudaTensor *input, THCudaTensor *gradOutput, THCudaTensor *gradInput, double lambda)
{
  THCUNN_assertSameGPU(state, 3, input, gradOutput, gradInput);
  THCudaTensor_resizeAs(state, gradInput, input);
  stub_THC_pointwiseApply3(state, gradInput, input, gradOutput, SoftShrinkUpdateGradInput(lambda));
  THCudaCheck(hipGetLastError());
}
