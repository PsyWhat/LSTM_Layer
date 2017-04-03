#pragma once

#include "BackPropogateResult.h"
#include "MatrixD.h"

class FullConnectionLayerBPResult : public BackPropagateResult
{
public:

	FullConnectionLayerBPResult( const MatrixD &InputError , const MatrixD &deltaWeights ) :BackPropagateResult( InputError ) , deltaWeights( deltaWeights )
	{

	}


	MatrixD deltaWeights;

	FullConnectionLayerBPResult operator +(const FullConnectionLayerBPResult &a) const 
	{
		return FullConnectionLayerBPResult(inputError + a.inputError, deltaWeights + a.deltaWeights);
	}

	virtual BackPropagateResult operator +(const BackPropagateResult &a) const override
	{
		const FullConnectionLayerBPResult &sc = static_cast<const FullConnectionLayerBPResult &>(a);
		return FullConnectionLayerBPResult(inputError + sc.inputError, deltaWeights + sc.deltaWeights);
	}

};
