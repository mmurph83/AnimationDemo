//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#include "MathEngine.h"
#include "MathApp.h"
#include <math.h>

//----------------------------------------------------------------------------- 
// Mutates result to be a slerp between the source and target by the
// specified factor.
// For a factor of 0.0, result == source.
// For a factor of 1.0, result == target.
//----------------------------------------------------------------------------- 

void QuatApp::Slerp( Quat &result, const Quat &src, const Quat &tar, const float slerpFactor )
{
	// Do your magic here
	MATH_UNUSED(result);
	MATH_UNUSED(src);
	MATH_UNUSED(tar);
	MATH_UNUSED(slerpFactor);
	Quat quatTar(tar);
	float angle = src.dot(tar);
	if (angle < 0.0f)
	{
		quatTar = -quatTar;
		angle = -angle;
	}
	angle = acosf(angle);
	
	if (fabsf(angle) > MATH_TOLERANCE)
	{
		result = src*(sinf((1.0f - slerpFactor)*angle)) / sinf(angle) + quatTar*(sinf((slerpFactor)*angle)) / sinf(angle);
	}
	else
	{
		result = src;
	}
};

void QuatApp::SlerpArray( Quat *out, const Quat *source, const Quat *target, const float slerpFactor, const int numQuats )
{
	// Do your magic here
	MATH_UNUSED(out);
	MATH_UNUSED(source);
	MATH_UNUSED(target);
	MATH_UNUSED(slerpFactor);
	MATH_UNUSED(numQuats);
	int i = 0;
	while (i < numQuats)
	{
		QuatApp::Slerp(out[i], source[i], target[i], slerpFactor);
		i++;
	}
} ;

// ---  End of File ---------------
