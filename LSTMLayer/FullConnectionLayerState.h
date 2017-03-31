#pragma once

#include "NeuroNetworkLayerState.h"
#include "MatrixD.h"

class FullConnectionLayerState : public NeuroNetworkLayerState
{

public:

	FullConnectionLayerState( const MatrixD &input , const MatrixD &output , const MatrixD &beforeActivation ) : input( input ) , output( output ) , beforeActivation( beforeActivation )
	{

	}

	MatrixD input;
	MatrixD output;
	MatrixD beforeActivation;

};