#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fstream>
#include <omp.h>
using namespace std;

#define USED_CLOCK CLOCK_MONOTONIC // CLOCK_MONOTONIC_RAW if available
#define NANOS 1000000000LL

bool* isprime ;

void computeChunk(unsigned long seeds[], int number_seeds, int startIndex, int endIndex, int CHUNKSIZE)
{
    int maxLookupIndex = ceil(sqrt(endIndex)) + startIndex ;
    //cout << " computeChunk start: " << startIndex << " maxLookupIndex: " << maxLookupIndex << " chunk end: " << endIndex <<  endl; 
    bool foundFirst ; 
    
    for(int l = 0 ; l < number_seeds ; l++ )
    {
	int temp_seed = seeds[l];
	if (temp_seed  > maxLookupIndex ) break ; 
	foundFirst = false;
	for(int i = startIndex ; i <= maxLookupIndex; i++)
	{
	    if (foundFirst) break; 
	    if (i % temp_seed == 0) // encontrou um para marcar
	    {
		//cout << "a semenete : " << temp_seed << " marcou o numero" << i << endl;
		isprime[i] = false; 
		foundFirst = true;
		for(int j = i ; j <= endIndex; j+= temp_seed)
		{
		    //cout << "---- a semenete : " << temp_seed << " marcou o numero" << j << endl;
		    isprime[j] = false;
		}
	    }
	}
    }
    
    
    //int stop ; cin >> stop ; 

  
}


int main()
{
    unsigned long n;
    unsigned long i;
    struct timespec begin, current;
    long long start, elapsed, microseconds;
    
    unsigned long pCount = 0;
    unsigned long* seeds;
    int CHUNKSIZE = 0;
    int NUM_THREADS = 4;
    int number_seeds  = 0; 
    ofstream outputFile;
    
    cout<<"Digite o numero:"<<endl;
    cin>>n;
    
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
	   
    #pragma omp parallel num_threads(NUM_THREADS)
    {
    
	  #pragma omp single nowait
	  {
	      for(int i = 1; i<=NUM_THREADS; i++)
	      {
		    //#pragma omp task
		    computeChunk(seeds, number_seeds, nextStartIndex[i], nextEndIndex[i], CHUNKSIZE);
	      }
	      
	  }
    }
	  
    
    
    if (clock_gettime(USED_CLOCK, &current)) exit(EXIT_FAILURE);
    elapsed = current.tv_sec*NANOS + current.tv_nsec - start;
    microseconds = elapsed / 1000 + (elapsed % 1000 >= 500);
    cout << "Elapsed time: " << microseconds << " microseconds (" << microseconds * 0.000001 << " s). " << endl;
    
    cout << "WRITING TO FILE - IT MIGHT TAKE A WHILE" << endl; 
    outputFile.open ("primes.txt");
    for(i = 2; i < n; i++)
    {
      if(isprime[i])
      {
	pCount++;
	//cout << i << " "; 
	outputFile << (i) << endl;
      }
      
    }
}