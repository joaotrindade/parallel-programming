#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fstream>
using namespace std;

#define USED_CLOCK CLOCK_MONOTONIC // CLOCK_MONOTONIC_RAW if available
#define NANOS 1000000000LL

int main()
{
    unsigned  long n;
    unsigned  long i;
    struct timespec begin, current;
    long long start, elapsed, microseconds;
    bool* isprime ;
    unsigned  long pCount = 0;
    ofstream outputFile;
    
    cout<<"Digite o numero:"<<endl;
    cin>>n;
    
    isprime = new bool[n];
    
    
    
    if (clock_gettime(USED_CLOCK, &begin)) exit(EXIT_FAILURE);
    
    
    
    memset(isprime, true, n);
    
    isprime[0] = isprime[1] = false;
    
    start = begin.tv_sec*NANOS + begin.tv_nsec;
    
    for(i=2 ; i<sqrt(n)+1; i++)
    {
	if(isprime[i])
	{

	  for(unsigned long j = i*i ; j < n; j+=i)
	  {
	    isprime[j] = false;
	  }
	}
    }
    
    if (clock_gettime(USED_CLOCK, &current)) exit(EXIT_FAILURE);
    elapsed = current.tv_sec*NANOS + current.tv_nsec - start;
    microseconds = elapsed / 1000 + (elapsed % 1000 >= 500);
    cout << "Elapsed time: " << microseconds << " microseconds (" << microseconds * 0.000001 << " s). " << endl;
    
    
    outputFile.open ("primes.txt");
    for(i = 2; i < n; i++)
    {
      if(isprime[i])
      {
	pCount++;
	//outputFile << (i) << endl;
      }
      
    }
	cout<<pCount<<endl; 
    cout<<endl;
}
