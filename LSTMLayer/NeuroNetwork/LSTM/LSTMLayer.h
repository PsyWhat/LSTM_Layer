#pragma once

#include "stdafx.h"
#include "MatrixD.h"
#include "LSTMLayerState.h"
#include "NeuroNetwork/ActivationFunctions.h"
#include "NeuroNetwork/NeuroNetworkLayer.h"
#include "LSTMNeuroNetworkError.h"
#include "LSTMBackPropogateResult.h"


class LSTMLayer : public NeuroNetworkLayer
{
protected:
	MatrixD Wc;
	MatrixD Wi;
	MatrixD Wo;
	MatrixD Wf;

	MatrixD Uc;
	MatrixD Ui;
	MatrixD Uo;
	MatrixD Uf;

	MatrixD ctmo;
	MatrixD htmo;

	int numInputs;
	int numOutputs;

	LSTMLayer( const int &inputs , const int &outputs )
	{
		numInputs = inputs;
		numOutputs = outputs;

		Wc = MatrixD( outputs , inputs );
		Uc = MatrixD( outputs , outputs );

		Wi = MatrixD( outputs , inputs );
		Ui = MatrixD( outputs , outputs );

		Wo = MatrixD( outputs , inputs );
		Uo = MatrixD( outputs , outputs );

		Wf = MatrixD( outputs , inputs );
		Uf = MatrixD( outputs , outputs );

		ctmo = MatrixD( numOutputs , 1 );
		htmo = MatrixD( numOutputs , 1 );
	}

	virtual NeuroNetworkLayerState ForwardPass( const MatrixD &data ) override
	{


		MatrixD ats = (Wc * data + Uc * htmo);
		MatrixD its = (Wi * data + Ui * htmo);
		MatrixD fts = (Wf * data + Uf * htmo);
		MatrixD ots = (Wo * data + Uo * htmo);

		MatrixD at = ats.ApliedFunction( TanhActivationFunction );
		MatrixD it = ats.ApliedFunction( SigmoidActivationFunction );
		MatrixD ft = ats.ApliedFunction( SigmoidActivationFunction );
		MatrixD ot = ats.ApliedFunction( SigmoidActivationFunction );

		MatrixD ct = it % at + ft % ctmo;
		MatrixD ht = ot % ct.ApliedFunction( TanhActivationFunction );


		LSTMLayerState state( ats, its, fts,
			at,it,ft,ot,
			data,ctmo,ct,ht,htmo
		);



		htmo = ht;
		ctmo = ct;
		return state;
	}




	virtual BackPropagateResult BackPropagation( const NeuroNetworkError &error , const NeuroNetworkLayerState &state ) override
	{
		const LSTMNeuroNetworkError &lstmError = static_cast<const LSTMNeuroNetworkError&>(error);
		const LSTMLayerState &lstmState = static_cast<const LSTMLayerState&>(state);
		MatrixD deltaOt = lstmError.outputError % lstmState.ct.ApliedFunction(TanhActivationFunction);
		MatrixD deltaCt = lstmError.outputError % deltaOt % (lstmState.ct.ApliedFunction( TanhActivationFunctionDiff )) + lstmError.cError;

		MatrixD deltaIt = deltaCt % lstmState.at;
		MatrixD deltaFt = deltaCt % lstmState.ctmo;
		MatrixD deltaAt = deltaCt % lstmState.it;
		MatrixD deltaCtmo = deltaCt % lstmState.ft;

		MatrixD deltaAts = deltaAt % lstmState.ats.ApliedFunction( TanhActivationFunctionDiff );
		MatrixD deltaIts = deltaAt % lstmState.it.ApliedFunction( SigmoidActivationFunctionDiff );
		MatrixD deltaFts = deltaAt % lstmState.ft.ApliedFunction( SigmoidActivationFunctionDiff );
		MatrixD deltaOts = deltaAt % lstmState.ot.ApliedFunction( SigmoidActivationFunctionDiff );

		MatrixD deltaXt = Wc * deltaAts + Wf * deltaFts + Wi * deltaIts + Wo * deltaOts;
		MatrixD deltaHtmo = Uc * deltaAts + Uf * deltaFts + Ui * deltaIts + Uo * deltaOts;

		MatrixD xtTrans = lstmState.xt.Transposed();

		MatrixD deltaWc = deltaAts * xtTrans;
		MatrixD deltaWi = deltaIts * xtTrans;
		MatrixD deltaWf = deltaFts * xtTrans;
		MatrixD deltaWo = deltaOts * xtTrans;

		MatrixD htmoTrans = lstmState.htmo.Transposed();

		MatrixD deltaUc = deltaAts * htmoTrans;
		MatrixD deltaUi = deltaIts * htmoTrans;
		MatrixD deltaUf = deltaFts * htmoTrans;
		MatrixD deltaUo = deltaOts * htmoTrans;


		LSTMBackPropogateResult res( deltaHtmo , deltaCt ,
			deltaWc , deltaWi , deltaWf , deltaWo ,
			deltaUc , deltaUi , deltaUf , deltaUo );
		return res;
	}



	virtual void ApplyBackPropagation( const BackPropagateResult &result ) override
	{
		const LSTMBackPropogateResult &lstmRes = static_cast<const LSTMBackPropogateResult &>(result);

		Wc += lstmRes.deltaWc;
		Wi += lstmRes.deltaWi;
		Wf += lstmRes.deltaWf;
		Wo += lstmRes.deltaWo;

		Uc += lstmRes.deltaUc;
		Ui += lstmRes.deltaUi;
		Uf += lstmRes.deltaUf;
		Uo += lstmRes.deltaUo;

	}

};