#pragma once

class BackPropagateResult
{
public:
	MatrixD inputError;

	BackPropagateResult( const MatrixD &inputError ) :inputError( inputError )
	{

	}

};


