#pragma once

#include "NeuroNetworkLayerState.h"
#include "MatrixD.h"



class LSTMLayerState :public NeuroNetworkLayerState
{

public:
	MatrixD ats;
	MatrixD its;
	MatrixD fts;

	MatrixD at;
	MatrixD it;
	MatrixD ft;
	MatrixD ot;

	MatrixD xt;
	MatrixD ctmo;
	MatrixD ct;
	MatrixD ht;
	MatrixD htmo;

	LSTMLayerState( const MatrixD &ats, const MatrixD &its, const MatrixD &fts,
		const MatrixD &at, const MatrixD &it, const MatrixD &ft, const MatrixD &ot,
		const MatrixD &xt, const MatrixD &ctmo, const MatrixD &ct, const MatrixD &ht, const MatrixD &htmo )
		:ats(ats), its(its), fts(fts), at(at), it(it)
		, ft(ft), ot(ot), xt(xt), ctmo(ctmo), ct(ct), ht(ht), htmo(htmo)
	{
	}


};