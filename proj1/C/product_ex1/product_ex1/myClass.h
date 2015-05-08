// matriz de doubles
class CdMatriz
{
private:
	double *m_pt;
	int m_c;		// colunas
	int m_l;		// linhas

public:
	// l - primeira linha
	// c - numero de colunas
	CdMatriz(double *buffer, int l, int c);
	~CdMatriz(void);
	void Update(double *buffer, int l, int c);
	double *Buffer(void);
	double operator() (int l, int c);
	void operator() (int l, int c, double v);
	double *Address(int l, int c);
	double CdMatriz::SomaCol(int c);
	double CdMatriz::SomaLin(int l);
	double CdMatriz::SomaAll(void);

};
