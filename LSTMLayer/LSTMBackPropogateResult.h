#pragma once

#include "BackPropogateResult.h"
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

	LSTMBackPropogateResult operator +(const LSTMBackPropogateResult &a) const
	{
		return LSTMBackPropogateResult(inputError + a.inputError, deltaCt + a.deltaCt,
			deltaWc + a.deltaWc, deltaWi + a.deltaWi,
			deltaWf + a.deltaWf, deltaWo + a.deltaWo,
			deltaUc + a.deltaUc, deltaUi + a.deltaUi,
			deltaUf + a.deltaUf, deltaUo + a.deltaUo
		);
	}

	virtual BackPropagateResult operator +(const BackPropagateResult &a) const override
	{
		const LSTMBackPropogateResult &cs = static_cast<const LSTMBackPropogateResult &>(a);
		return LSTMBackPropogateResult(inputError + cs.inputError, deltaCt + cs.deltaCt,
			deltaWc + cs.deltaWc, deltaWi + cs.deltaWi,
			deltaWf + cs.deltaWf, deltaWo + cs.deltaWo,
			deltaUc + cs.deltaUc, deltaUi + cs.deltaUi,
			deltaUf + cs.deltaUf, deltaUo + cs.deltaUo
			);
	}

};

