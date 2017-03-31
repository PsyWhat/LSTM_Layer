#pragma once

#include "stdafx.h"
#include <thread>
#include <queue>


using std::thread;
using std::queue;


namespace Parallel
{


	template<class _Fn>
	void ForIJ( int im , int jm , int maxThreadsI , int maxThreadsJ , _Fn &&func )
	{
		queue<thread*> ithreads;
		for ( int i = 0; i < im; ++i )
		{
			while ( ithreads.size() >= maxThreadsI )
			{
				thread *&aw = ithreads.front();
				if ( aw->joinable() )
				{
					aw->join();
				}
				ithreads.pop();
				delete aw;
			}

			thread *it = new thread( [&jm , i , &maxThreadsJ , &func](){
				queue<thread*> jthreads;
				for ( int j = 0; j < jm; ++j )
				{
					while ( jthreads.size() >= maxThreadsJ )
					{
						thread *&aw = jthreads.front();
						if ( aw->joinable() )
						{
							aw->join();
						}
						jthreads.pop();
						delete aw;
					}



					thread *jt = new thread( [i , j , &func](){
						func( i , j );
					} );
					jthreads.push( jt );
				}

				while ( jthreads.size() > 0 )
				{
					thread *&aw = jthreads.front();
					if ( aw->joinable() )
					{
						aw->join();
					}
					jthreads.pop();
					delete aw;
				}

			} );
			ithreads.push( it );
		}

		while ( ithreads.size() > 0 )
		{
			thread *&aw = ithreads.front();
			if ( aw->joinable() )
			{
				aw->join();
			}
			ithreads.pop();
			delete aw;
		}
	}


	template<class _Fn>
	void ForI( int to , int threadsC , _Fn &&func )
	{
		queue<thread*> pool;
		if ( threadsC < to )
		{
			if ( to % threadsC == 0 )
			{
				int step = to / threadsC;
				for ( int i = 0; i < threadsC - 1; ++i )
				{
					int cfrom = i * step;
					int cto = i * step + step;
					thread *t = new thread( [cfrom , cto , &func](){
						for ( int ti = cfrom; ti < cto; ++ti )
						{
							func( ti );
						}
					} );
					pool.push( t );
				}
				int cfrom = ((threadsC - 1) * step);
				int cto = to;
				for ( int ti = cfrom; ti < cto; ++ti )
				{
					func( ti );
				}
				while ( pool.size() > 0 )
				{
					thread *&t = pool.front();
					if ( t->joinable() )
					{
						t->join();
					}
					pool.pop();
					delete t;
				}
			} else
			{
				int step = to / (threadsC - 1);
				for ( int i = 0; i < (threadsC - 1); ++i )
				{
					int cfrom = i * step;
					int cto = i * step + step;
					thread *t = new thread( [cfrom , cto , &func](){
						for ( int ti = cfrom; ti < cto; ++ti )
						{
							func( ti );
						}
					} );
					pool.push( t );
				}
				int cfrom = ((threadsC - 1) * step);
				int cto = to;
				for ( int ti = cfrom; ti < cto; ++ti )
				{
					func( ti );
				}
				while ( pool.size() > 0 )
				{
					thread *&t = pool.front();
					if ( t->joinable() )
					{
						t->join();
					}
					pool.pop();
					delete t;
				}
			}
		} else
		{
			for ( int i = 0; i < to-1; ++i )
			{
				thread *t = new thread( [i,&func](){
					func( i );
				} );
				pool.push( t );
			}
			func( to - 1 );
			while ( pool.size() > 0 )
			{
				thread *&t = pool.front();
				if ( t->joinable() )
				{
					t->join();
				}
				pool.pop();
				delete t;
			}
		}
	}

}