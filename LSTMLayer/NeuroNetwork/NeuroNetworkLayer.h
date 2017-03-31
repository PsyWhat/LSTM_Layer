#pragma once

#include "NeuroNetworkLayerState.h"
#include "BackPropogateResult.h"
#include "NeuroNetworkError.h"


class NeuroNetworkLayer
{
public:

	virtual NeuroNetworkLayerState ForwardPass( const MatrixD &input ) = 0;

	virtual BackPropagateResult BackPropagation( const NeuroNetworkError &error , const NeuroNetworkLayerState &state ) = 0;

	virtual void ApplyBackPropagation( const BackPropagateResult &result) = 0;

};