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


int main()
{
    unsigned long n;
    unsigned long i;
    struct timespec begin, current;
    long long start, elapsed, microseconds;
    bool* isprime ;
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
    CHUNKSIZE = (int) ( (n - (int)sqrt(n) ) / NUM_THREADS) + 1 ;
    cout << "chunksize : " << CHUNKSIZE << endl;
    memset(isprime, true, n);
    isprime[0] = isprime[1] = false;
    
    if (clock_gettime(USED_CLOCK, &begin)) exit(EXIT_FAILURE);
    start = begin.tv_sec*NANOS + begin.tv_nsec;

    for(i=2 ; i<sqrt(n)+1; i++)
    {
	if(isprime[i])
	{

	  for(int j = i*i ; j < sqrt(n); j+=i)
	  {
	    isprime[j] = false;
	  }
	  
	}
    }
    
    cout << "seeds: ";
    for (int i = 2 ; i < sqrt(n) ; i++)
      if (isprime[i] == true) { seeds[number_seeds++] = i ; cout << i << " ";}
    cout << endl;
    
    number_seeds = number_seeds ;
    int* chunk_max_index = new int[NUM_THREADS] ;
    int* chunk_min_index = new int[NUM_THREADS] ;
    
    cout << "got " << number_seeds << " seeds" << endl;
    
    for(int i = 0; i < NUM_THREADS; i++) chunk_max_index[i] = 0 ;
    #pragma omp parallel num_threads(4)
    {
	  #pragma omp for schedule(static,CHUNKSIZE) private(i)
	  for(i=sqrt(n) ; i<n ; i++)
	  {
		if (i <= sqrt(chunk_min_index[omp_get_thread_num()]))
		{
		    // UPDATE MIN AND MAX INDEXES OF CHUNK
		    if (chunk_max_index[omp_get_thread_num()] == 0)
		    {
			chunk_min_index[omp_get_thread_num()] = i ;
			if (i + CHUNKSIZE < n) chunk_max_index[omp_get_thread_num()] = i + CHUNKSIZE;
			else chunk_max_index[omp_get_thread_num()] = n ;
		    }
		    
		    
		    
		    //#pragma omp critical
		    //cout << "Thread : " << omp_get_thread_num() << " index : " << i << " max_index : " << chunk_max_index[omp_get_thread_num()] << endl;
		    
		    //PARSE CHUNK
		    if (isprime[i])
		    {
			for(int l = 0 ; l < number_seeds ; l++ )
			{
			    int temp_seed = seeds[l];
			    if (temp_seed > i) break;
			    if (i % temp_seed == 0) // encontrou um para marcar
			    {
				for(int j = i*i ; j < chunk_max_index[omp_get_thread_num()]; j+=temp_seed)
				{
				  isprime[j] = false;
				}
			    }
			}
		    }
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