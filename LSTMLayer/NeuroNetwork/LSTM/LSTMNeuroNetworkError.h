#pragma once
#include "NeuroNetwork/NeuroNetworkError.h"

class LSTMNeuroNetworkError : public NeuroNetworkError
{
public:
	MatrixD cError;

	LSTMNeuroNetworkError( const MatrixD &inputError ) :NeuroNetworkError( inputError )
	{
		cError = MatrixD( inputError.Rows() , inputError.Columns(), 0.0 );
	}

	LSTMNeuroNetworkError( const MatrixD &inputError , const MatrixD &cellError ) : NeuroNetworkError(inputError)
	{
		cError = cellError;
	}

};