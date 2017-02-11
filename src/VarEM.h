#ifndef _VarEM_
#define _VarEM_

#include <iostream>
#include "Corpus.h"

class VarEM {
    public :
        VarEM(unsigned int K);
        ~VarEM();
    
        /* renvoie le nombre de classes K du modèle associé */
        unsigned int getK() const;
	
        /* Réalise la E-step de l'EM variationnel; 
         * Calcul des estimateurs Gamma et Phi respectivement 
         * de Theta et z */
        void E_step(Corpus &c, unsigned int k);
	
	/* Realise la M-step pour mettre à jour beta*/
	void M_step(Corpus &c, unsigned int k, double eta);
	
        /* Réalise la classification EM dans son ensemble,
         * pour un nombre d'itérations donné, en mettant à jour 
         * la valeur de Beta à partir des Gamma et Phi calculés
         * dans la E_step*/
        void EM_classify(Corpus &c, unsigned int nbIterations);
        
	/* Fonction auxiliaire de calcul */
        double digamma(double x);
 private :
        unsigned int K;
};

#endif
