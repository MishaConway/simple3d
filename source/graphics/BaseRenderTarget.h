#pragma once

#include <vector>
#include "../interfaces/IValidatable.h"

//class Effect;
class BaseRenderTarget : public IValidatable
{
protected:
	std::vector<float> ComputeGaussianKernel( const int Size, const float StandardDeviation );
};