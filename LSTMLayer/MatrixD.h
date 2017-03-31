#pragma once

#include "stdafx.h"
#include "Parallel.h"

typedef float MFloatType;

class MatrixD
{
public:

	static const int maxThreads = 12;


	~MatrixD();

	MatrixD( const int& rows , const int& columns );

	MatrixD( const int& rows , const int& columns , const MFloatType &filler );

	MatrixD( const MatrixD& copy );

	MatrixD();


	//////////// Acesors /////////////////////

	inline int Rows() const
	{
		return rows;
	}

	inline int Columns() const
	{
		return columns;
	}

	//////////// #Acesors ////////////////////


	//////////// Self operations //////////////

	MatrixD& Transpose();

	MatrixD& FillWithNulls();

	template<class _Fn>
	inline MatrixD& AplyFunction( _Fn &&func );


	//////// #Self operations ////////////


	/////////// Return operations ///////////

	MatrixD Transposed() const;
	template<class _Fn>
	inline MatrixD ApliedFunction( _Fn &&func ) const;

	/////////// #Return operations //////////


	/////// Operators overloading //////////////

	MatrixD operator*( const MatrixD &a ) const;

	MatrixD& operator=( MatrixD &a );

	MatrixD& operator=( const MatrixD & a );

	MatrixD operator+( const MatrixD &a ) const;
	MatrixD operator+( MatrixD &a );

	MatrixD operator-( const MatrixD &a ) const;
	MatrixD operator-( MatrixD &a );

	MatrixD operator%( const MatrixD &a ) const;
	MatrixD operator%( MatrixD &a );

	MatrixD operator*( const MFloatType &d ) const;
	MatrixD operator*( const MFloatType &d );

	MatrixD& operator*=( const MFloatType &d );

	MatrixD& operator*=( const MatrixD &a );

	MatrixD& operator+=( const MatrixD &a );

	MatrixD& operator-=( const MatrixD &a );

	MatrixD& operator%=( const MatrixD &a );





	class Proxy
	{
	public:
		Proxy( MatrixD *m , const int const *row ) :m( m ) , row( row )
		{
		}



		inline MFloatType& operator[]( const int &column )
		{
			return m->values[*row + m->rows * column];
		}
	protected:


		Proxy()
		{
		}

		MatrixD *m;
		const int const *row;
	};

	class ConstProxy
	{
	public:
		ConstProxy( const MatrixD *m , const int const *row ) :m( m ) , row( row )
		{
		}



		inline const MFloatType& operator[]( const int &column ) const
		{
			return m->values[*row + m->rows * column];
		}
	protected:


		ConstProxy()
		{
		}

		const MatrixD *m;
		const int const *row;
	};

	inline Proxy operator[]( const int &row )
	{
		return Proxy( this , &row );
	}

	inline const ConstProxy operator[]( const int &row ) const
	{
		return ConstProxy( this , &row );
	}

	/////// #Operators overloading /////////////

protected:

	MFloatType *values;
	int rows;
	int columns;
	bool tmp;
	bool copied;

};


inline MatrixD operator*( const MFloatType &d , const MatrixD &m )
{
	return m * d;
}

template<class _Fn>
inline MatrixD & MatrixD::AplyFunction( _Fn && func )
{

	for ( int i = 0; i < rows; ++i )
	{
		for ( int j = 0; j < columns; ++j )
		{
			this->values[i + this->rows * j] = func( this->values[i + this->rows * j] );
		}
	}

	return *this;
}

template<class _Fn>
inline MatrixD MatrixD::ApliedFunction( _Fn && func ) const
{
	MatrixD res( rows , columns );

	for ( int i = 0; i < rows; ++i )
	{
		for ( int j = 0; j < columns; ++j )
		{
			res.values[i + this->rows * j] = func( this->values[i + this->rows * j] );
		}
	}

	res.tmp = true;
	res.copied = false;

	return res;
}
