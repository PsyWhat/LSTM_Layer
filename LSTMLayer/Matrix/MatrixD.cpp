#include "stdafx.h"
#include "MatrixD.h"

using std::exception;
using std::queue;
using std::thread;

MatrixD::MatrixD()
{
	rows = columns = 0;
	values = nullptr;
	this->tmp = false;
	this->copied = false;
}


MatrixD::~MatrixD()
{
	if ( values != nullptr && ((tmp && (!copied)) || !tmp) )
	{
		delete[] values;
	}
}

MatrixD::MatrixD( const int & rows , const int & columns )
{
	if ( !(rows > 0 || columns > 0) )
	{
		throw new std::exception( "Trying to initialize null matrix." );
	}
	this->values = new MFloatType[rows * columns];
	this->rows = rows;
	this->columns = columns;
	this->tmp = false;
	this->copied = false;
}

MatrixD::MatrixD( const int & rows , const int & columns , const MFloatType & filler ) :MatrixD( rows , columns )
{
	for ( int j = 0; j < columns; ++j )
	{
		int rj = j * rows;
		for ( int i = 0; i < rows; ++i )
		{
			values[i + rj] = filler;
		}
	}
}

MatrixD::MatrixD( const MatrixD & copy )
{
	this->rows = copy.rows;
	this->columns = copy.columns;
	this->values = new MFloatType[rows* columns];
	memcpy( values , copy.values , rows*columns * sizeof( MFloatType ) );
	this->tmp = false;
	this->copied = false;
}

MatrixD & MatrixD::Transpose()
{
	MFloatType *nw = new MFloatType[rows*columns];
	for ( int i = 0; i < rows; ++i )
	{
		for ( int j = 0; j < columns; ++j )
		{
			nw[j + i * columns] = values[i + j * rows];
		}
	}
	delete[] this->values;
	values = nw;
	int tmp = rows;
	rows = columns;
	columns = tmp;
	return *this;
}

MatrixD & MatrixD::FillWithNulls()
{
	if ( rows > columns )
	{
		Parallel::ForI( rows , maxThreads , [this]( int i ){
			for ( int j = 0; j < this->columns; ++j )
			{
				this->values[i + this->rows * j] += 0.0;
			}
		} );
	} else
	{
		Parallel::ForI( columns , maxThreads , [this]( int j ){
			for ( int i = 0; i < this->rows; ++i )
			{
				for ( int n = 0; n < this->columns; ++n )
				{
					this->values[i + this->rows * j] = 0.0;
				}
			}
		} );
	}


	return *this;
}

MatrixD MatrixD::Transposed() const
{
	MatrixD n( columns , rows );
	for ( int i = 0; i < rows; ++i )
	{
		for ( int j = 0; j < columns; ++j )
		{
			n.values[j + i * n.rows] = values[i + j * rows];
		}
	}

	n.tmp = true;
	n.copied = false;

	return n;
}

MatrixD MatrixD::operator*( const MatrixD & a ) const
{
	if ( columns == a.rows )
	{
		MatrixD res( rows , a.columns );

		if ( res.rows > res.columns )
		{
			Parallel::ForI( res.rows , maxThreads , [&res , &a , this]( int i ){
				for ( int j = 0; j < res.columns; ++j )
				{
					res.values[i + res.rows * j] = 0;
					register int rj = res.rows * j;
					register int aj = a.rows * j;
					for ( int n = 0; n < this->columns; ++n )
					{
						res.values[i + rj] += this->values[i + this->rows * n] * a.values[n + aj];
					}
				}
			} );
		} else
		{
			Parallel::ForI( res.columns , maxThreads , [&res , &a , this]( int j ){
				register int rj = res.rows * j;
				register int aj = a.rows * j;
				for ( int i = 0; i < res.rows; ++i )
				{
					res.values[i + res.rows * j] = 0;
					for ( int n = 0; n < this->columns; ++n )
					{
						res.values[i + rj] += this->values[i + this->rows * n] * a.values[n + aj];
					}
				}
			} );
		}

		res.tmp = true;

		return res;
	} else
	{
		throw new exception( "Matrix size should be NxM and MxK" );
	}
}

MatrixD & MatrixD::operator=( MatrixD & a )
{
	if ( &a != this )
	{
		if ( this->values != nullptr )
		{
			delete[] values;
		}

		if ( !(a.tmp && (!a.copied)) )
		{
			int len = a.rows * a.columns;

			this->values = new MFloatType[len];

			memcpy( values , a.values , len * sizeof( MFloatType ) );
		} else
		{
			a.copied = true;
			values = a.values;
		}
		rows = a.rows;
		columns = a.columns;
	}
	return *this;
}


MatrixD & MatrixD::operator=( const  MatrixD & a )
{
	if ( &a != this )
	{
		if ( this->values != nullptr )
		{
			delete[] values;
		}
		int len = a.rows * a.columns;

		this->values = new MFloatType[len];

		memcpy( values , a.values , len * sizeof( MFloatType ) );
		rows = a.rows;
		columns = a.columns;
	}
	return *this;
}

MatrixD MatrixD::operator+( const MatrixD & a ) const
{

	if ( rows == a.rows && columns == a.columns )
	{
		MatrixD res( rows , columns );

		int len = rows * columns;
		for ( int i = 0; i < len; ++i )
		{
			res.values[i] = values[i] + a.values[i];
		}

		res.tmp = true;
		res.copied = false;

		return res;

	} else
	{
		throw new exception( "Matrices should be the same size." );
	}
}

MatrixD MatrixD::operator+( MatrixD & a )
{

	if ( rows == a.rows && columns == a.columns )
	{
		if ( !tmp && !a.tmp )
		{
			MatrixD res( rows , columns );

			int len = rows * columns;
			for ( int i = 0; i < len; ++i )
			{
				res.values[i] = values[i] + a.values[i];
			}

			res.tmp = true;
			res.copied = false;

			return res;
		} else
		{
			if ( tmp )
			{
				int len = rows * columns;
				for ( int i = 0; i < len; ++i )
				{
					values[i] = values[i] + a.values[i];
				}
				return *this;

			} else
			{
				int len = rows * columns;
				for ( int i = 0; i < len; ++i )
				{
					a.values[i] = values[i] + a.values[i];
				}
				return a;
			}
		}
	} else
	{
		throw new exception( "Matrices should be the same size." );
	}
}



MatrixD MatrixD::operator-( const MatrixD & a ) const
{
	if ( rows == a.rows && columns == a.columns )
	{
		MatrixD res( rows , columns );


		int len = rows * columns;
		for ( int i = 0; i < len; ++i )
		{
			res.values[i] = values[i] - a.values[i];
		}

		res.tmp = true;
		res.copied = false;

		return res;
	} else
	{
		throw new exception( "Matrices should be the same size." );
	}
}

MatrixD MatrixD::operator-( MatrixD & a )
{
	if ( rows == a.rows && columns == a.columns )
	{
		if ( !tmp && !a.tmp )
		{
			MatrixD res( rows , columns );

			int len = rows * columns;
			for ( int i = 0; i < len; ++i )
			{
				res.values[i] = values[i] - a.values[i];
			}

			res.tmp = true;
			res.copied = false;

			return res;
		} else
		{
			if ( tmp )
			{
				int len = rows * columns;
				for ( int i = 0; i < len; ++i )
				{
					values[i] = values[i] - a.values[i];
				}
				return *this;

			} else
			{
				int len = rows * columns;
				for ( int i = 0; i < len; ++i )
				{
					a.values[i] = values[i] - a.values[i];
				}
				return a;
			}
		}
	} else
	{
		throw new exception( "Matrices should be the same size." );
	}
}

MatrixD MatrixD::operator%( const MatrixD & a ) const
{
	if ( rows == a.rows && columns == a.columns )
	{
		MatrixD res( rows , columns );



		int len = rows * columns;
		for ( int i = 0; i < len; ++i )
		{
			res.values[i] = values[i] * a.values[i];
		}

		res.tmp = true;
		res.copied = false;

		return res;
	} else
	{
		throw new exception( "Matrices should be the same size." );
	}
}

MatrixD MatrixD::operator%( MatrixD & a )
{
	if ( rows == a.rows && columns == a.columns )
	{
		if ( !tmp && !a.tmp )
		{
			MatrixD res( rows , columns );

			int len = rows * columns;
			for ( int i = 0; i < len; ++i )
			{
				res.values[i] = values[i] * a.values[i];
			}

			res.tmp = true;
			res.copied = false;

			return res;
		} else
		{
			if ( tmp )
			{
				int len = rows * columns;
				for ( int i = 0; i < len; ++i )
				{
					values[i] = values[i] * a.values[i];
				}
				return *this;

			} else
			{
				int len = rows * columns;
				for ( int i = 0; i < len; ++i )
				{
					a.values[i] = values[i] * a.values[i];
				}
				return a;
			}
		}
	} else
	{
		throw new exception( "Matrices should be the same size." );
	}
}

MatrixD MatrixD::operator*( const MFloatType & d ) const
{


	MatrixD res( rows , columns );



	int len = rows * columns;
	for ( int i = 0; i < len; ++i )
	{
		res.values[i] = values[i] * d;
	}

	res.tmp = true;
	res.copied = false;

	return res;
}

MatrixD MatrixD::operator*( const MFloatType & d )
{

	if ( !tmp )
	{

		MatrixD res( rows , columns );

		int len = rows * columns;
		for ( int i = 0; i < len; ++i )
		{
			res.values[i] = values[i] * d;
		}

		res.tmp = true;
		res.copied = false;

		return res;
	} else
	{
		int len = rows * columns;
		for ( int i = 0; i < len; ++i )
		{
			values[i] = values[i] * d;
		}
		return *this;
	}
}

MatrixD & MatrixD::operator*=( const MFloatType & d )
{
	for ( int j = 0; j < columns; ++j )
	{
		int jr = j * rows;
		for ( int i = 0; i < rows; ++i )
		{
			this->values[i + jr] *= d;
		}
	}
	return *this;
}

MatrixD& MatrixD::operator*=( const MatrixD & a )
{
	if ( columns == a.rows )
	{
		int rrows = rows;
		int rcols = a.columns;
		MFloatType *arr = new MFloatType[rrows * rcols];


		if ( rrows > rcols )
		{
			Parallel::ForI( rrows , maxThreads , [&arr , &a , &rcols , &rrows , this]( int i ){
				for ( int j = 0; j < rcols; ++j )
				{
					arr[i + rrows * j] = 0;
					for ( int n = 0; n < this->columns; ++n )
					{
						arr[i + rrows * j] += this->values[i + this->rows * n] * a.values[n + a.rows * j];
					}
				}
			} );
		} else
		{
			Parallel::ForI( rcols , maxThreads , [&arr , &rcols , &rrows , &a , this]( int j ){
				for ( int i = 0; i < rrows; ++i )
				{
					arr[i + rrows * j] = 0;
					for ( int n = 0; n < this->columns; ++n )
					{
						arr[i + rrows * j] += this->values[i + this->rows * n] * a.values[n + a.rows * j];
					}
				}
			} );
		}

		delete[] values;

		rows = rrows;
		columns = rcols;
		values = arr;

		return *this;
	} else
	{
		throw new exception( "Matrix size should be NxN and NxN" );
	}
}

MatrixD & MatrixD::operator+=( const MatrixD & a )
{
	if ( rows == a.rows && columns == a.columns )
	{



		int len = rows * columns;
		for ( int i = 0; i < len; ++i )
		{
			values[i] += a.values[i];
		}


		return *this;
	} else
	{
		throw new exception( "Matrix size should be NxN and NxN" );
	}
}

MatrixD & MatrixD::operator-=( const MatrixD & a )
{
	if ( rows == a.rows && columns == a.columns )
	{



		int len = rows * columns;
		for ( int i = 0; i < len; ++i )
		{
			values[i] -= a.values[i];
		}


		return *this;
	} else
	{
		throw new exception( "Matrix size should be NxN and NxN" );
	}
}

MatrixD & MatrixD::operator%=( const MatrixD & a )
{
	if ( rows == a.rows && columns == a.columns )
	{




		int len = rows * columns;
		for ( int i = 0; i < len; ++i )
		{
			values[i] *= a.values[i];
		}


		return *this;
	} else
	{
		throw new exception( "Matrix size should be NxN and NxN" );
	}
}

