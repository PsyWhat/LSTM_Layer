#pragma once


#include "stdafx.h"
#include "MatrixD.h"
#include "NeuroNetworkLayer.h"
#include "ActivationFunctions.h"
#include "FullConnectionLayerState.h"
#include "FullConnectionLayerBPResult.h"

class FullConnectionLayer : public NeuroNetworkLayer
{
public:

	FullConnectionLayer( int inputs , int outputs ): weights(outputs, inputs )
	{

	}


public:

	virtual NeuroNetworkLayerState ForwardPass( const MatrixD &input ) override
	{
		MatrixD beforeActivation = weights * input;
		MatrixD output = beforeActivation.ApliedFunction( SigmoidActivationFunction );
		return FullConnectionLayerState( input , output , beforeActivation );
	}


	virtual BackPropagateResult BackPropagation( const NeuroNetworkError &error , const NeuroNetworkLayerState &state ) override
	{
		const FullConnectionLayerState &fcstate = static_cast<const FullConnectionLayerState&>(state);

		MatrixD activationError = error.outputError % fcstate.beforeActivation.ApliedFunction( SigmoidActivationFunctionDiff );

		MatrixD inputError = weights.Transposed() * activationError;

		MatrixD deltaWeight = activationError * fcstate.input;

		return FullConnectionLayerBPResult( inputError , deltaWeight );
	}


	virtual void ApplyBackPropagation( const BackPropagateResult &result ) override
	{
		const FullConnectionLayerBPResult &fcres = static_cast<const FullConnectionLayerBPResult &>(result);
		weights -= fcres.deltaWeights;
	}

protected:

	MatrixD weights;

};