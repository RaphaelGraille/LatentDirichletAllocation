#include "Document.h"
#include "Word.h"
#include <string>
#include <vector>
#include "Gibbs_sampling.h"
#include <iostream>
#include "Corpus.h"
#include <fstream>
#include <ctime>

Gibbs_sampling::Gibbs_sampling(Corpus &c,unsigned int k) {
	std::srand(std::time(NULL));
	freq=new unsigned int*[c.getVocabularySize()];
	for (unsigned int i=0;i<c.getVocabularySize();i++){
		freq[i]= new unsigned int[k];
	}
	freq_total=new unsigned int[k];	

	this->z = new unsigned int*[c.getDocCount()];
	for (unsigned int m=0; m < c.getDocCount(); m++){
		this->z[m] = new unsigned int[c.getDocument(m).getWords().size()];
	}

	top=new unsigned int*[c.getDocCount()];
	for (unsigned int i=0;i<c.getDocCount();i++){
		top[i]= new unsigned int[k];
	}
	sumtop=new unsigned int[c.getDocCount()];
	double alpha=50/k;
	double beta = 0.1;
	this->K = k;
	this->freq=freq;
	this->freq_total=freq_total;
	this->top=top;
	this->sumtop=sumtop;
	this->alpha=alpha;
	this->beta=beta;
	this->docCount=c.getDocCount();
	this->vocabularySize=c.getVocabularySize();


	for(unsigned int j = 0; j<K; j++) {
		this->freq_total[j]=0;
		for (unsigned int i=0;i<c.getVocabularySize();i++){
			this->freq[i][j] = 0;	
		}
	}
	for (unsigned int d=0;d<c.getDocCount();d++){
		sumtop[d]=0;
		for (unsigned int i=0;i<K;i++){
			top[d][i]=0;
		}
	}
	

	/* Initialisation des paramètres commun à tous les docs.*/
	for (unsigned int d=0;d<c.getDocCount();d++){
		for(unsigned int w=0; w<c.getDocument(d).getWords().size(); w++){
			unsigned int topic = (unsigned int)((double)(std::rand())/(double)(RAND_MAX)*(double)K);
			std::cout<<"topic : "<<topic<<std::endl;
			this->z[d][w] = topic;
			this->top[d][topic] += 1;
			this->freq[c.getDocument(d).getWord(w).getWordIndex()][topic] += 1;
			this->freq_total[topic] += 1;
		}
		this->sumtop[d] = c.getDocument(d).getWords().size();
	}

}

Gibbs_sampling::~Gibbs_sampling(){
	/*for (unsigned int i=0;i<vocabularySize;i++){
		delete [] freq[i];
	}
	delete [] freq;	
	for (unsigned int i=0;i<docCount;i++){
		delete [] top[i];
	}
	delete [] top;	
	delete [] freq_total;
	delete [] sumtop;*/
}

unsigned int Gibbs_sampling::getK() const {
	return K;
}

unsigned int Gibbs_sampling::getZ(unsigned int m, unsigned int n) const{
	return z[m][n];
}

void Gibbs_sampling::sampling(Corpus &c) {
	unsigned int nbClasses = this->K;
	std::srand(std::time(NULL));

	/* Pour chaque doc.*/
	for (unsigned int d=0;d<c.getDocCount();d++){
		//std::cout<<"Document "<<d<<"\n";
		/* le tableau z[i] contient le topic courant associé au ième mot du document d*/

		for (unsigned int i=0;i<c.getDocument(d).getWords().size();i++){
			for(unsigned int rr = 0; rr<c.getDocument(d).getWord(i).getFreq(); rr++){
				unsigned int w = c.getDocument(d).getWord(i).getWordIndex();
				unsigned int t = z[d][i];
				top[d][t] -= 1;
				freq[w][t] -= 1;
				freq_total[t] -= 1;
				sumtop[d] -= 1;
				double proba[nbClasses];
				proba[0]=(((double)top[d][0] +alpha)/(sumtop[d]+nbClasses*alpha))*((double)freq[w][0] + beta)/((double)freq_total[0]+beta*(double)c.getVocabularySize());
				for (unsigned int k=1; k<nbClasses; k++){					
					proba[k] = proba[k-1]+((top[d][k] +alpha)/(sumtop[d]+nbClasses*alpha))*(freq[w][k] +beta)/(freq_total[k]+beta*c.getVocabularySize());					
				}
				double r = ((double)std::rand()/RAND_MAX)*proba[nbClasses-1];
				unsigned int j;
				for (j=0; j<nbClasses; j++){
					if (proba[j]>r){
						break;
					}
				}
				z[d][i] = j;
				freq[w][j] += 1;
				top[d][j] += 1;
				freq_total[j] += 1;
				sumtop[d] += 1;
			}
		}		
	} 
}

/* correspond à phi dans le document 2011-darling ... */
void Gibbs_sampling::computeBeta(Corpus &c){
	for(unsigned int i=0;i<K;i++){
		for(unsigned int n=0;n<c.getVocabularySize();n++){
			c.getBeta(i,n)=((double)freq[n][i]+beta)/((double)freq_total[i]+beta*c.getVocabularySize());
		}
	}

}

void Gibbs_sampling::computeTheta(Corpus &c){
	for(unsigned int d=0;d<c.getDocCount();d++){	
		for(unsigned int i=0;i<K;i++){
			c.getDocument(d).getGamma(i)=((double)top[d][i]+alpha)/((double)sumtop[d]+alpha*K);
		}
	}
}		

/*
void Gibbs_sampling::loadZ(Corpus &c){
	for(unsigned int d=0; d<c.getDocCount(); d++){
		for(unsigned int w =0; w<c.getDocument(d).getWords().size(); w++){
			for(unsigned int i=0; i<K; i++){
				c.getDocument(d).getPhi(w,i) = 
			}
		}
	}
}
*/
void Gibbs_sampling::iter_sampling(Corpus &c,unsigned int nbIter){
	for (unsigned int i=0;i<nbIter;i++){
		std::cout<<"Iteration "<< i+1 <<" sur "<< nbIter <<"\n";
		sampling(c);
	}
}


