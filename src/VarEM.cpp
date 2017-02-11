/**********************************************************************
 *          Projet Spécialité - Classification documentaire           *
 *              Algorithme LDA - Méthode EM variationnel              *
 *                         FRANCESIAZ Theo                            *
 *                         GRAILLE Raphael                            *
 *                         METAHRI Brahim                             *
 *********************************************************************/

#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include "VarEM.h"
#include "Corpus.h"
#include <cassert>

#define GAMMA 0.5772156649

VarEM::VarEM(unsigned int K) {
    this->K = K;
}

VarEM::~VarEM(){
}

unsigned int VarEM::getK() const {
    return K;
}

double VarEM::digamma(double x) {
    /*if (x<0) {
        throw std::invalid_argument("Erreur : digamma est définie pour x>(=)0\n");
    }
    double res = -GAMMA;
    for (double p=1.0; p<100.0; p++) {
        res += (x-1)/(p*(x+p-1));
    }
    return res;*/
    double p;
    x=x+6;
    p=1/(x*x);
    p=(((0.004166666666667*p-0.003968253986254)*p+
	0.008333333333333)*p-0.083333333333333)*p;
    p=p+log(x)-0.5/x-1/(x-1)-1/(x-2)-1/(x-3)-1/(x-4)-1/(x-5)-1/(x-6);
    return p;


}

void VarEM::E_step(Corpus &c, unsigned int k) {
   
			
    /* On itère sur tous les documents du corpus */
    for (unsigned int m=0; m<(c.getDocs().size());m++){
          
        /* initialisation gamma */
        unsigned int N = c.getDocument(m).getWords().size();
		double totalLength = c.getDocument(m).getTotalLength();
        for (unsigned int i=0; i<k; i++) {
            c.getDocument(m).getGamma(i) = c.getAlpha() + totalLength/double(k);
        }
        /* initialisation phi */
        std::vector<double> vect(k,(1.0/double(k)));
        for (unsigned int n=0; n<N; n++) {
            for (unsigned int i=0; i<k; i++) {
                c.getDocument(m).getPhi(n,i) = 1.0/double(k);
            }
        }

		/* initialisation de la somme des gamma utilisée pour MAJ des phi peut être intégré a l'init des gamma
		double gammaSum = 0;
		for(unsigned int i=0; i<k; i++){
			gammaSum += c.getDocument(m).getGamma(i);
		}
        */

        /* Début E-step */
        unsigned int count = 0;
		
		/* Changement ici : N remplacé par totalLength */
        /* Critère de cvergence -> count~totalLength (cf doc pour plus de détails) */ 
        while (count<totalLength) {

            double norm_phi = 0;
            /* Mise a jour phi :
            * Pour chaque mot, approche itérative de phi par méthode de Newton-Raphson 
            */

			double* psi_gamma = (double*)calloc(k,sizeof(double));
			for(unsigned int i=0; i<k; i++) {
				psi_gamma[i] = digamma(c.getDocument(m).getGamma(i));
			}
            for (unsigned int n=0; n<N; n++) {
                for(unsigned int i=0; i<k; i++) {
                    c.getDocument(m).getPhi(n,i) = c.getBeta(i,c.getDocument(m).getWord(n).getWordIndex())*exp(psi_gamma[i]);
                    norm_phi += c.getDocument(m).getPhi(n,i);
                }

                /* Normalisation du vecteur phi */
                for (unsigned int i=0; i<k; i++) {
                    c.getDocument(m).getPhi(n,i) = (c.getDocument(m).getPhi(n,i))/norm_phi;
					if(n>0) {
						c.getDocument(m).getGamma(i) += c.getDocument(m).getPhi(n,i)*c.getDocument(m).getWord(n).getFreq();
					} else {
						c.getDocument(m).getGamma(i) = c.getAlpha()+ c.getDocument(m).getPhi(n,i)*c.getDocument(m).getWord(n).getFreq();
					}
                }
				norm_phi = 0;
            }
        
            count ++;
        }
        std::cout<<"\t \tE_step document "<<m+1<<" réalisée\n";
    }
}

void VarEM::M_step(Corpus &c, unsigned int k, double eta){

	double* sumBeta = (double*)calloc(k,sizeof(double));

    std::cout<<"\t \tLancement de la M_step pour le corpus ...\n";
	/*Initialisation de Tous les beta à eta*/
	for(unsigned int i=0; i<k;i++){
		for(unsigned int j=0; j<c.getVocabularySize();j++){
			c.getBeta(i,j) = eta;
			sumBeta[i] += eta;
		}
	}
	/* Pour tous les mots de tous les dos, on ajoute pour chaque classe le z corrspondant*/
	for(unsigned int d=0; d<c.getDocs().size(); d++){
		Document& doc = c.getDocument(d);
		for(unsigned int n=0; n<doc.getWords().size(); n++){
			Word w = doc.getWord(n);
			for(unsigned int i=0; i<k; i++){
				c.getBeta(i,w.getWordIndex()) += doc.getPhi(n,i)*w.getFreq();
				sumBeta[i] += doc.getPhi(n,i)*w.getFreq();
			}
		}
	}
	
	std::cout<<"\t \tNormalisation de beta ...\n";
	/*Normalisation de beta*/
	for(unsigned int i=0; i<k; i++){
		for(unsigned int n=0; n<c.getVocabularySize(); n++){
			c.getBeta(i,n) = c.getBeta(i,n)/sumBeta[i];
		}
	}

    std::cout<<"\t \tM_step réalisée pour le corpus\n";
}

void VarEM::EM_classify(Corpus &c, unsigned int nbIterations) {
    for(unsigned int n=0; n<nbIterations; n++) {
        std::cout<<"\tItération "<<n+1<<" sur "<<nbIterations<<" ...\n";
        E_step(c, this->K);

        /* eta fixé à 1 pour l'instant; 
         * on suppose que les mots 'peu fréquents' apparaissent de façon ponctuelle ds le corpus, d'où ce choix*/
        M_step(c, this->K, 1.0);
    }

    /* On relance une E_step avec le dernier beta calculé pour avoir un 
     * résultat avec le meilleur beta estimé */
    std::cout<<"\tItération supplémentaire avec le dernier beta calculé ...\n"; 
    E_step(c, this->K);
}
