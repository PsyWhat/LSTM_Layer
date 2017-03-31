#pragma once
#include "MatrixD.h"


class NeuroNetworkError
{
public:
	MatrixD outputError;

	NeuroNetworkError( const MatrixD &error ):outputError(error)
	{

	}
};