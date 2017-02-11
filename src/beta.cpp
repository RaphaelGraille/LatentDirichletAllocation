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
#include <string>
#include <fstream>
#include <fstream>
#include "VarEM.h"
#include "Corpus.h"
#include <cassert>
#include <ctime>

int main(int argc, char* argv[]){
	//Nombre de classes
	unsigned int k =20;
	//Nombre de docs par classe dans le sous-corpus
	unsigned int nbDoc =100;
	
	std::srand(std::time(NULL));
	std::cout<<"Parsing du corpus" << argv[1] <<"\n";
	Corpus fullCorp(argv[1], "beta0", k);
	std::cout<<"Creation du sous-corpus\n";
	// Creation du "sous corpus" sur lequel sera initialisé beta.
	Corpus lightCorp(fullCorp, k, nbDoc);
	// Initialisation de beta sur le "sous corpus"
	lightCorp.NGInitBeta(k, nbDoc);
	std::cout<<"Beta initialisé avec succès\nEcriture dans fichier de sortie.\n";
	std::fstream outfile(argv[2], std::ios::in|std::ios::out|std::ios::trunc);
	// Ecriture des resultats dans la fichier de sortie
	for(unsigned int i=0; i<k; i++){
		for(unsigned int n=0; n<fullCorp.getVocabularySize();n++){
			outfile<<lightCorp.getBeta(i,n)<<" " ;
			//Décommenter pour initialiser beta aleatoirement
			//outfile<<1+std::rand()/(double)(RAND_MAX)<<" ";
		}
		outfile<<std::endl;
	}
	outfile.close();
	std::cout<<"Ecriture terminée avec succès !\n";
	std::cout<<"Fin du programme\n";
}
