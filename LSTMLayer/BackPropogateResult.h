#pragma once

class BackPropagateResult
{
public:
	MatrixD inputError;

	BackPropagateResult( const MatrixD &inputError ) :inputError( inputError )
	{

	}


	virtual BackPropagateResult operator+(const BackPropagateResult &a) const
	{
		return BackPropagateResult(inputError + a.inputError);
	}

};


