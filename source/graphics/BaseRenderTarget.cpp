#include "BaseRenderTarget.h"

std::vector<float> BaseRenderTarget::ComputeGaussianKernel( const int Size, const float StandardDeviation )
{	
	std::vector<float> Kernel;
	Kernel.resize( Size );
	
	const float PI = 3.14159f;
	const float e  = 2.71828f;

	const float center = (float) (Size/2);
	const float sigma_sqrt2PI  = StandardDeviation * sqrt( 2.0f * PI );
	const float TWOsigmaSquared = 2.0f * StandardDeviation * StandardDeviation;
	
	double sum = 0;
	for( int x = 0; x < Size; ++x )
	{
		const float X = (float) x;
		const float distFromCenterSquared = ( X - center ) * ( X - center );
		const float baseEexponential = pow( e, -distFromCenterSquared / TWOsigmaSquared );
		
		Kernel[x] = baseEexponential / sigma_sqrt2PI;
		sum += (double)Kernel[x];
	}

	//normalize all entries in the kernel
	for( int x = 0; x < Size; ++x )
		Kernel[x] /= (float)sum;

	return Kernel;
}