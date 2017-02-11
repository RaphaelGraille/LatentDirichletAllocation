#ifndef _Gibbs_sampling_
#define _Gibbs_sampling_

#include <iostream>
#include "Corpus.h"

class Gibbs_sampling{
 public :
	Gibbs_sampling(Corpus &c,unsigned int k);
	~Gibbs_sampling();
	
	 /* renvoie le nombre de classes K du modèle associé */
    unsigned int getK() const;
	unsigned int getZ(unsigned int m, unsigned int n) const;
	void sampling(Corpus &c);
	void computeBeta(Corpus &c);
	void computeTheta(Corpus &c);
	void iter_sampling(Corpus &c,unsigned int nbIter);
 private : 
	unsigned int K;	
	/* freq[i][j] représente le nombre de fois que le ième mot est associé au jème topic */
	unsigned int** freq;
	/* freq_total[i] représente le nombre de mots associés à la classe i*/
	unsigned int* freq_total;

	unsigned int** z;

	/* top[d][i] représente le nombre de mots dans le doc d assigné au ième topic*/
	unsigned int** top;
	unsigned int* sumtop;
	double alpha;
	double beta;
	unsigned int docCount;
	unsigned int vocabularySize;
	
};
#endif
