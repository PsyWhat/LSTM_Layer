#pragma once

#include "NeuroNetwork/BackPropogateResult.h"
#include "MatrixD.h"

class LSTMBackPropogateResult : public BackPropagateResult
{
public:

	MatrixD deltaCt;

	MatrixD deltaWc;
	MatrixD deltaWi;
	MatrixD deltaWf;
	MatrixD deltaWo;

	MatrixD deltaUc;
	MatrixD deltaUi;
	MatrixD deltaUf;
	MatrixD deltaUo;

	LSTMBackPropogateResult( const MatrixD & deltaHTMO, const MatrixD & deltaCt, 
		const MatrixD & deltaWc, const MatrixD & deltaWi, const MatrixD & deltaWf, const MatrixD & deltaWo,
		const MatrixD & deltaUc, const MatrixD & deltaUi, const MatrixD & deltaUf, const MatrixD & deltaUo )
		:BackPropagateResult(deltaHTMO), deltaCt(deltaCt),
		deltaWc( deltaWc ) , deltaWi( deltaWi ) , deltaWf( deltaWf ) , deltaWo( deltaWo ) ,
		deltaUc( deltaUc ) , deltaUi( deltaUi ) , deltaUf( deltaUf ) , deltaUo( deltaUo )

	{

	}

};

