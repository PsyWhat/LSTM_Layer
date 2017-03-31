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

};
