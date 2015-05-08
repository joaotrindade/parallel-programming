#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main()
{
    unsigned long n;
    unsigned long i;
    
    cout<<"Digite o numero:"<<endl;
    cin>>n;
    
    bool* isprime = new bool[n];
    unsigned long* primes = new unsigned long[n];
    unsigned long pCount = 0;
    
    
    
    memset(isprime, true, n);
    
    isprime[0] = isprime[1] = false;
    
    for(i=2 ; i<sqrt(n)+1; i++)
    {
	if(isprime[i])
	{

	  for(int j = i*i ; j < n; j+=i)
	  {
	    isprime[j] = false;
	  }
	}
    }
    
    for(i = 2; i < n; i++)
    {
      if(isprime[i])
      {
	primes[pCount++] = i;
      }
      
    }
    cout<<endl;
}