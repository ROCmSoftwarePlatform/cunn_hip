#include "THCUNN.h"
#include "common.h"

struct absupdateOutput_functor
{
  __host__ __device__
  absupdateOutput_functor() = default;

  __device__ 
  void operator()(float* output, const float* input) const
  {
    *output = abs(*input);
  }

  absupdateOutput_functor(const absupdateOutput_functor& fun) = default;

  // __host__ __device__
  // ~absupdateOutput_functor() {}
};

void THNN_CudaAbs_updateOutput(THCState *state, THCudaTensor *input, THCudaTensor *output)
{
  THCUNN_assertSameGPU(state, 2, input, output);
  THCudaTensor_resizeAs(state, output, input);
  stub_THC_pointwiseApply2(state, output, input, absupdateOutput_functor());
}

struct absupdateGradInput_functor
{
  __device__ void operator()(float* gradInput, const float* input, const float* gradOutput) const
  {
    *gradInput = *input < 0 ? - *gradOutput : *gradOutput;
  }
  __device__
  ~absupdateGradInput_functor() {}
};

void THNN_CudaAbs_updateGradInput(THCState *state, THCudaTensor *input, THCudaTensor *gradOutput, THCudaTensor *gradInput)
{
  THCUNN_assertSameGPU(state, 3, input, gradOutput, gradInput);
  THCudaTensor_resizeAs(state, gradInput, input);
  stub_THC_pointwiseApply3(state, gradInput, input, gradOutput, absupdateGradInput_functor());
}
