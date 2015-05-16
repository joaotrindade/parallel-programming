#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fstream>
#include <omp.h>
#include "mpi.h"
using namespace std;

#define USED_CLOCK CLOCK_MONOTONIC // CLOCK_MONOTONIC_RAW if available
#define NANOS 1000000000LL

bool* isprime ;

void computeChunk(bool primeList[], unsigned long seeds[], int number_seeds, int startIndex, int endIndex, int CHUNKSIZE)
{
    int maxLookupIndex = ceil(sqrt(endIndex)) + startIndex ;
    bool foundFirst ;
    
    for(int l = 0 ; l < number_seeds ; l++ )
    {
	//cout << "a processar seed: " << seeds[l] << endl;
	int temp_seed = seeds[l];
	if (temp_seed  > maxLookupIndex ) break ; 
	foundFirst = false;
	for(int i = 0 ; i <= CHUNKSIZE; i++)
	{
	    if (foundFirst) break; 
	    if ((i + startIndex)  % temp_seed == 0) // encontrou um para marcar
	    {
		//cout << "a semenete : " << temp_seed << " marcou o numero" << i + startIndex<< endl;
		primeList[i] = false; 
		foundFirst = true;
		for(int j = i ; j <= endIndex - startIndex; j+= temp_seed)
		{
		    //cout << "---- a semenete : " << temp_seed << " marcou o numero" << j+startIndex << endl;
		    primeList[j] = false;
		}
	    }
	}
    } 
}


int main(int argc, char **argv)
{
    unsigned long n;
    unsigned long i;
    struct timespec begin, current;
    long long start, elapsed, microseconds;
    
    unsigned long pCount = 0;
    unsigned long* seeds;
    int CHUNKSIZE = 0;
    int RANK = 0 ;
    int NUM_THREADS = 4;
    int number_seeds  = 0; 
    ofstream outputFile;
    
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &NUM_THREADS );
    MPI_Comm_rank( MPI_COMM_WORLD, &RANK );
    
    cout << "Threads: " << NUM_THREADS << endl;
    cout << "RANK: " << RANK << endl;
    
    
    if (RANK == 0)
    {
	  CHUNKSIZE = 23;
	  cout<<"Digite o numero:"<<endl;
	  cin>>n;
	  
	  
	  
	  cout << "after mpi init" << NUM_THREADS << endl; 
	  
	  isprime = new bool[n];
	  seeds = new unsigned long[(int)sqrt(n) + 5] ;
	  CHUNKSIZE = ceil( ( (n - (int)sqrt(n) ) / NUM_THREADS) + 1 );
	  cout << "chunksize : " << CHUNKSIZE << endl;
	  memset(isprime, true, n);
	  isprime[0] = isprime[1] = false;
	  
	  if (clock_gettime(USED_CLOCK, &begin)) exit(EXIT_FAILURE);
	  start = begin.tv_sec*NANOS + begin.tv_nsec;

	  for(i=2 ; i<=sqrt(n)+1; i++)
	  {
	      if(isprime[i])
	      {

		for(int j = i*i ; j <= sqrt(n); j+=i)
		{
		  isprime[j] = false;
		}
		
	      }
	  }
	  
	  //cout << "seeds: ";
	  for (int i = 2 ; i < sqrt(n) ; i++)
	    if (isprime[i] == true) { seeds[number_seeds++] = i ; /*cout << i << " ";*/}
	  cout << endl;
	  
	  number_seeds = number_seeds ;
	  //cout << "got " << number_seeds << " seeds" << endl;

	  int* nextStartIndex = new int[NUM_THREADS + 1 ];
	  int* nextEndIndex = new int[NUM_THREADS + 1]; 
	  
	  nextStartIndex[1]  = ceil(sqrt(n));
	  nextEndIndex[1] = nextStartIndex[1] + CHUNKSIZE ;
	  //cout << "i: 1" << " start : " << nextStartIndex[1] << " -  end: " << nextEndIndex[1] << endl;
	  for(int i = 2; i<=NUM_THREADS; i++)
	  {
	      nextStartIndex[i] = nextEndIndex[i - 1];
	      nextEndIndex[i] = nextStartIndex[i] + CHUNKSIZE ;
	      //cout << "i: " << i << " start : " << nextStartIndex[i] << " -  end: " << nextEndIndex[i] << endl;
	  }
	  
	    
	  
	  
	  
	  
	  for(int i = 1; i < NUM_THREADS; i++)
	  {
		//#pragma omp task
		//computeChunk(seeds, number_seeds, nextStartIndex[i], nextEndIndex[i], CHUNKSIZE);
		
		// DISTRIBUI TAREFAS void computeChunk(unsigned long seeds[], int number_seeds, int startIndex, int endIndex, int CHUNKSIZE)
		
		// SEND startIndex - ID: 1
		MPI_Send(&number_seeds,1,MPI_INT,i,1,MPI_COMM_WORLD);
		
		// SEND seeds - ID: 2
		MPI_Send(seeds,number_seeds,MPI_UNSIGNED_LONG,i,2,MPI_COMM_WORLD);
		
		// SEND startIndex - ID: 3
		MPI_Send(&nextStartIndex[i],1,MPI_INT,i,3,MPI_COMM_WORLD);
	    
		// SEND endIndex - ID: 4
		MPI_Send(&nextEndIndex[i],1,MPI_INT,i,4,MPI_COMM_WORLD);
		
	  }
	  
	  // FAZ GATHER QUE ENVIA CHUNKSIZE = PARA TODOS e RECEBE RESPOSTAS
	  // SEND CHUNKSIZE - ID: 5 
	  //MPI_Send(&CHUNKSIZE,1,MPI_INT,1,5,MPI_COMM_WORLD);
	  MPI_Gather(&CHUNKSIZE, 1, MPI_INT, isprime, CHUNKSIZE, MPI_C_BOOL, 0, MPI_COMM_WORLD);
	  
	  for(int i = 0 ; i < n ; i++)
	  {
	      if (isprime[i]) cout << i << endl; 
	  }

	  
	  // ESPERA TODA A GENTE
	  
	  if (clock_gettime(USED_CLOCK, &current)) exit(EXIT_FAILURE);
	  elapsed = current.tv_sec*NANOS + current.tv_nsec - start;
	  microseconds = elapsed / 1000 + (elapsed % 1000 >= 500);
	  cout << "Elapsed time: " << microseconds << " microseconds (" << microseconds * 0.000001 << " s). " << endl;
	  
	  cout << "WRITING TO FILE - IT MIGHT TAKE A WHILE" << endl; 
	  outputFile.open ("primes.txt");
	  for(int i = 2; i < n; i++)
	  {
	    if(isprime[i])
	    {
	      pCount++;
	      //cout << i << " "; 
	      outputFile << (i) << endl;
	    }
	    
	  }
      }
      else
      {
	
	  
	  int retorno;
	  string temp = ""; 
	  int r_number_seeds, r_startIndex, r_endIndex, r_CHUNKSIZE;
	  
	  MPI_Status status; 
	  
	  MPI_Recv(&r_number_seeds,1,MPI_INT,0,1,MPI_COMM_WORLD, &status);
	  cout << "r_number_seeds: " << r_number_seeds << endl;
	  
	  unsigned long* r_seeds =  new unsigned long[r_number_seeds] ;
	  MPI_Recv(r_seeds,r_number_seeds,MPI_UNSIGNED_LONG,0,2,MPI_COMM_WORLD, &status);
	  
	  MPI_Recv(&r_startIndex,1,MPI_INT,0,3,MPI_COMM_WORLD, &status);
	  cout << "r_startIndex: " << r_startIndex << endl;
	  MPI_Recv(&r_endIndex,1,MPI_INT,0,4,MPI_COMM_WORLD, &status);
	  cout << "r_endIndex: " << r_endIndex << endl;
	  MPI_Recv(&r_CHUNKSIZE,1,MPI_INT,0,5,MPI_COMM_WORLD, &status);
	  
	  
	  
	  
	  cout << " ----> r_CHUNKSIZE: " << r_CHUNKSIZE << endl;
	  cout << "r_seeds" << endl; 
	  for(int i = 0 ; i < r_number_seeds ; i++)
	  {
	      cout << r_seeds[i] << endl; 
	  }
	  
	  
	  
	  
	  // COMPUTAR 
	  
	  bool* primeList = new bool[r_CHUNKSIZE];
	  memset(primeList, true, r_CHUNKSIZE);
	  computeChunk(primeList,r_seeds,r_number_seeds,r_startIndex,r_endIndex,r_CHUNKSIZE);
	  
	  
	  // SEND SOLUTION - ID: 6
	  MPI_Send(primeList,r_CHUNKSIZE,MPI_C_BOOL,0,6,MPI_COMM_WORLD);
	  
	  
	  
	  
	  
      }
      
      
    MPI_Finalize();
}