
#include "stdafx.h"
#include "Parallel.h"

//using std::thread;
//using std::queue;



//template<class _Fn>
//void ForIJ( int im , int jm , int maxThreadsI , int maxThreadsJ , _Fn &&func )
//{
	/*
	queue<thread*> ithreads;
	for ( int i = 0; i < im; ++i )
	{
		while ( ithreads.size() > maxThreadsI )
		{
			thread *&aw = ithreads.front();
			if ( aw->joinable() )
			{
				aw->join();
			}
			ithreads.pop();
			delete aw;
		}

		thread *it = new thread( [&jm,i,&maxThreadsJ, &func](){
			queue<thread*> jthreads;
			for ( int j = 0; j < jm; ++j )
			{
				while ( jthreads.size() > maxThreadsJ )
				{
					thread *&aw = jthreads.front();
					if ( aw->joinable() )
					{
						aw->join();
					}
					jthreads.pop();
					delete aw;
				}

				

				thread *jt = new thread( [i,j, &func](){
					//_Fn as = func;
					//as( i , j );
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
	*/

//}

