  #include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include "mpi.h"

#define USED_CLOCK CLOCK_MONOTONIC // CLOCK_MONOTONIC_RAW if available
#define NANOS 1000000000LL

using namespace std;

int computeSeeds(unsigned long n, int seeds[])
{
    int maxIndex = 2*(int)sqrt(n);
    bool* isprime = new bool[maxIndex];
    memset(isprime, true, maxIndex); //TODO: PODE SER REDUZIDO
    int number_seeds = 0;
    isprime[0] = isprime[1] = false;
    for(unsigned long i=2 ; i<=sqrt(n)+1; i++)
    {
	    if(isprime[i])
	    {

		    for(unsigned long j = i*i ; j <= sqrt(n); j+=i)
		    {
			    isprime[j] = false;
		    }
	    }
    }
    
    for (unsigned long i = 2 ; i < sqrt(n) ; i++)
      if (isprime[i] == true) { seeds[number_seeds++] = i ; /*cout << i << " ";*/}
    return number_seeds ;
    
}

void computeChunk(bool primeList[], int seeds[], int number_seeds, unsigned long startIndex, unsigned long endIndex, unsigned long CHUNKSIZE)
{
	unsigned long maxLookupIndex = ceil(sqrt(endIndex)) + startIndex ;
	bool foundFirst ;
	for(unsigned long l = 0 ; l < number_seeds ; l++ )
	{
		int temp_seed = seeds[l];
		if (temp_seed  > maxLookupIndex ) break ; 
		foundFirst = false;
		for(unsigned long i = 0 ; i <= CHUNKSIZE; i++)
		{
			if (foundFirst) break; 
			if ((i + startIndex)  % temp_seed == 0) // encontrou um para marcar
			{
				primeList[i] = false;
				foundFirst = true;
				for(unsigned long j = i ; j <= endIndex - startIndex; j+= temp_seed)
				{
					primeList[j] = false;
				}
			}
		}
	}
}


int main(int argc, char **argv)
{
	
	unsigned long i;
	struct timespec begin, current;
	long long start, elapsed, microseconds;
	bool* primeList; 
	int* seeds;
	int CHUNKSIZE = 0;
	int r_CHUNKSIZE = 0 ;
	int RANK = 0 ;
	int NUM_THREADS = 4;
	int number_seeds  = 0; 
	
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &NUM_THREADS );
	MPI_Comm_rank( MPI_COMM_WORLD, &RANK );

	if (RANK == 0)
	{
		cout << "Threads: " << NUM_THREADS << endl;
		int numberOfPrimes = 0 ;
		unsigned long n = 0, n2;
		MPI_Status status;
	
		cout<<"Digite o numero:"<<endl;
		cin>>n;
		
		
		n=  pow(2,n) ;
		cout << n << endl;
		
		CHUNKSIZE = ceil( ( (n - (int)sqrt(n) ) / (NUM_THREADS)) + 1 );
		primeList = new bool[CHUNKSIZE + 5];
		memset(primeList, true, CHUNKSIZE + 5);
		
		seeds = new int[(int)sqrt(n) + 5] ;
		
		if (clock_gettime(USED_CLOCK, &begin)) exit(EXIT_FAILURE);
		start = begin.tv_sec*NANOS + begin.tv_nsec;
		
		number_seeds = computeSeeds(n,seeds);
		
		unsigned long* nextStartIndex = new unsigned long[NUM_THREADS + 1 ];
		unsigned long* nextEndIndex = new unsigned long[NUM_THREADS + 1]; 

		nextStartIndex[0]  = ceil(sqrt(n));
		nextEndIndex[0] = nextStartIndex[0] + CHUNKSIZE ;
		//cout << "i: 1" << " start : " << nextStartIndex[1] << " -  end: " << nextEndIndex[1] << endl;
		for(int i = 1; i<NUM_THREADS ; i++)
		{
			nextStartIndex[i] = nextEndIndex[i - 1] + 1;
			nextEndIndex[i] = nextStartIndex[i] + CHUNKSIZE ;
			//cout << "i: " << i << " start : " << nextStartIndex[i] << " -  end: " << nextEndIndex[i] << endl;
		}
		
		if (nextEndIndex[NUM_THREADS-1] > n) nextEndIndex[NUM_THREADS-1] = n ;
		
		for(int i = 1; i < NUM_THREADS ; i++)
		{
			// DISTRIBUI TAREFAS void computeChunk(unsigned long seeds[], int number_seeds, int startIndex, int endIndex, int CHUNKSIZE)
			
			MPI_Send(&n,1,MPI_UNSIGNED_LONG,i,1,MPI_COMM_WORLD);

			MPI_Send(&nextStartIndex[i],1,MPI_UNSIGNED_LONG,i,3,MPI_COMM_WORLD);

			MPI_Send(&nextEndIndex[i],1,MPI_UNSIGNED_LONG,i,4,MPI_COMM_WORLD);
			
			MPI_Send(&CHUNKSIZE,1,MPI_INT,i,5,MPI_COMM_WORLD);
			
		}

		computeChunk(primeList,seeds,number_seeds,nextStartIndex[0],nextEndIndex[0],CHUNKSIZE);

		for(int k = 0 ; k <= CHUNKSIZE ; k++)
		{
		    if (primeList[k]){
		      numberOfPrimes++;
		      //cout <<  "PROCESSO: " <<  RANK << " PRIMO: " << k << endl ;
		    }
		}
		
		numberOfPrimes += number_seeds ; 
		
		for(int i = 1 ; i < NUM_THREADS ; i++)
		{
			int tempNumber = 0 ;
			//int mainIndex = nextEndIndex[i] ; 
			MPI_Recv(&tempNumber,1,MPI_UNSIGNED_LONG,-1,6,MPI_COMM_WORLD, &status);
			numberOfPrimes += tempNumber ; 
		}
		
		// ESPERA TODA A GENTE
		
		if (clock_gettime(USED_CLOCK, &current)) exit(EXIT_FAILURE);
		elapsed = current.tv_sec*NANOS + current.tv_nsec - start;
		microseconds = elapsed / 1000 + (elapsed % 1000 >= 500);
		cout << "Primes found: " << numberOfPrimes << endl;
		cout << "Elapsed time: " << microseconds << " microseconds (" << microseconds * 0.000001 << " s). " << endl;
				
	}
	else
	{
		//cout << "Here - RANK : " << RANK << endl ;
		unsigned long n_size,r_startIndex, r_endIndex; 
		string temp;
		int r_number_seeds;
		MPI_Status status; 
		
		n_size = 0 ;
		temp = "";
		
		MPI_Recv(&n_size,1,MPI_UNSIGNED_LONG,0,1,MPI_COMM_WORLD, &status);
		
		MPI_Recv(&r_startIndex,1,MPI_UNSIGNED_LONG,0,3,MPI_COMM_WORLD, &status);

		MPI_Recv(&r_endIndex,1,MPI_UNSIGNED_LONG,0,4,MPI_COMM_WORLD, &status);
		
		MPI_Recv(&r_CHUNKSIZE,1,MPI_INT,0,5,MPI_COMM_WORLD, &status);
		
		// COMPUTAR 
		
		seeds = new int[(int)sqrt(n_size) + 5] ;
		r_number_seeds = computeSeeds(n_size,seeds);

		primeList = new bool[r_CHUNKSIZE];
		memset(primeList, true, r_CHUNKSIZE);

		computeChunk(primeList,seeds,r_number_seeds,r_startIndex,r_endIndex,r_CHUNKSIZE);
		unsigned long count = 0 ; 
		for(int k = 0 ; k < r_CHUNKSIZE ; k++)
		{
		    if(k + r_startIndex > r_endIndex) break;
		    if (primeList[k])
		    {
			count++;
		    }
		}

		MPI_Send(&count,1,MPI_UNSIGNED_LONG,0,6,MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
}