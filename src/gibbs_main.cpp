#include "Gibbs_sampling.h"
#include <iostream>
#include <fstream>
#include <string>


void write_theta(Corpus c, Gibbs_sampling model, const std::string &str) {
	std::fstream outfile(str.c_str(), std::ios::in|std::ios::out|std::ios::trunc);
    const unsigned int nbDoc =c.getDocCount();
    const unsigned int K = model.getK();
    for(unsigned int m=0; m<nbDoc; m++) {
	    outfile<<"Theta pour le document : "<<m<<"\n";
            for (unsigned int k=0; k<K; k++) {
		    outfile<<c.getDocument(m).getGamma(k)<<" ";
            }
            outfile<<std::endl<<std::endl;        
    }
    outfile.close();
}



void write_beta(Corpus c, Gibbs_sampling model, const std::string &str) {
	std::fstream outfile(str.c_str(), std::ios::in|std::ios::out|std::ios::trunc);

    const unsigned int K = model.getK();
    const unsigned int V = c.getVocabularySize();
    for(unsigned int k=0; k<K; k++) {
        for(unsigned int v=0; v<V; v++) {
            outfile<<c.getBeta(k,v)<<" ";
        }
        outfile<<std::endl;
    }
    outfile.close();
}

void write_z_clustering(Corpus c, Gibbs_sampling model, const std::string &str) {
	std::fstream outfile(str.c_str(), std::ios::in|std::ios::out|std::ios::trunc);
    const unsigned int M = c.getDocCount();
    const unsigned int K = model.getK();
	unsigned int cluster[M][K];
	for(unsigned int m=0; m<M; m++){
		for(unsigned int k=0; k<K; k++){
			cluster[m][k] = 0;
		}
	}
    for(unsigned int m=0; m<M; m++) {
        unsigned int N = c.getDocument(m).getWords().size();
        for (unsigned int n=0; n<N; n++) {
			cluster[m][model.getZ(m,n)] += 1;
        }
		for(unsigned int i=0; i<K; i++){
			outfile<<cluster[m][i]<<" ";
		}
		outfile<<std::endl;
	}
}

int main(int argc, char* argv[]) {

	unsigned int nbIter=1000;
	std::cout<<"Début du programme\n";
	unsigned int K = 40;
	std::cout<<"Initialisation du corpus\n";
	Corpus c(argv[1], "beta0", K);
	std::cout<<"Construction du model\n";
	Gibbs_sampling model(c,K);
	std::cout<<"Début du sampling ...\n";
	model.iter_sampling(c,nbIter);
	std::cout<<"Samnplig terminé.\n";
	std::cout<<"Calcul de beta\n";
	model.computeBeta(c);
	std::cout<<"Calcul de theta\n";
	model.computeTheta(c);
	std::cout<<"Eciture de beta\n";
	write_beta(c,model,argv[2]);
	std::cout<<"Ecriture de theta\n";
	write_theta(c, model, argv[3]);
	std::cout<<"Ecriture du clustering\n";
	write_z_clustering(c,model,argv[4]);
}
