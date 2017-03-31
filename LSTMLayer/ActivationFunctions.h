#pragma once

#include "stdafx.h"
#include "MatrixD.h"


inline MFloatType TanhActivationFunction( MFloatType x )
{
	return tanh( x );
}

inline MFloatType TanhActivationFunctionDiff( MFloatType x )
{
	MFloatType th = tanh( x );
	return 1.0 - (th * th);
}

inline MFloatType SigmoidActivationFunction( MFloatType x )
{
	return 1.0 / (1.0 + exp( -x ));
}

inline MFloatType SigmoidActivationFunctionDiff( MFloatType x )
{
	MFloatType th = SigmoidActivationFunction( x );
	return th * (1.0 - th);
}

