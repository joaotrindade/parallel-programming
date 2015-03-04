
#include "myClass.h"

// matriz de doubles
// l - primeira linha
// c - numero de colunas
// 
CdMatriz::CdMatriz(double *buffer, int l, int c)
{
	m_pt = buffer;
	m_c  = c;
	m_l  = l;
}


CdMatriz::~CdMatriz(void)
{
	return;
}

	
void CdMatriz::Update(double *buffer, int l, int c)
{
	m_pt = buffer;
	m_c  = c;
	m_l  = l;
}


double *CdMatriz::Buffer(void)
{
	return(m_pt);
}

double CdMatriz::operator() (int l, int c)
{	return(m_pt[l*m_c + c]);
}

void CdMatriz::operator() (int l, int c, double v)
{	m_pt[l*m_c + c] = v;
}

double* CdMatriz::Address(int l, int c)
{	return(&m_pt[l*m_c + c]);
}

double CdMatriz::SomaCol(int c)
{	int i;
	double soma;
	
	soma = 0.0;
	for(i=0; i<m_l; i++)
		soma += m_pt[i*m_c + c];

	return(soma);
}



double CdMatriz::SomaLin(int l)
{	int i;
	double soma;
	
	soma = 0.0;
	for(i=0; i<m_c; i++)
		soma += m_pt[l*m_c + i];

	return(soma);
}



double CdMatriz::SomaAll(void)
{	int i;
	double soma;
	
	soma = 0.0;
	for(i=0; i<m_l; i++)
		soma += SomaLin(i);

	return(soma);
}



