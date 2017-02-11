#include "Corpus.h"
#include "VarEM.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/* Ecrit dans un fichier les estimations des paramètres gamma et phi pour être lus par l'utilisateur*/
void write_res_EM(Corpus c, VarEM model, const string &str) {
    fstream outfile(str.c_str(), ios::in|ios::out|ios::trunc);
    const unsigned int M = c.getDocs().size();
    const unsigned int K = model.getK();
    for(unsigned int m=0; m<M; m++) {
        /* Ecriture des Gamma */
        outfile<<"***************** Paramètres optimaux (gamma,phi) pour document "<<m+1<<" *********************\n";
        outfile<<"Gamma document "<<m+1<<" :\n";
        for (unsigned int k=0; k<K; k++) {
            outfile<<c.getDocument(m).getGamma(k)<<" ";
        }
        outfile<<endl;
        outfile<<endl;

        /* Ecriture des Phi */
        outfile<<"Phi document "<<m+1<<"\n";
        unsigned int N = c.getDocument(m).getWords().size();
        for (unsigned int n=0; n<N; n++) {
            for (unsigned int k=0; k<K; k++) {
                outfile<<c.getDocument(m).getPhi(n,k)<<" ";
            }
            outfile<<endl;
        }
        outfile<<endl;
        outfile<<endl;
    }
    outfile.close();
}

/* Ecrit les phi estimés dans un fichier qui sera utilisé pour faire le clustering*/
void write_phi_to_cluster(Corpus c, VarEM model, const string &str) {
    fstream outfile(str.c_str(), ios::in|ios::out|ios::trunc);
    const unsigned int M = c.getDocs().size();
    const unsigned int K = model.getK();
    for(unsigned int m=0; m<M; m++) {
        unsigned int N = c.getDocument(m).getWords().size();
        for (unsigned int n=0; n<N; n++) {
            for (unsigned int k=0; k<K; k++) {
                outfile<<c.getDocument(m).getPhi(n,k)<<" ";
            }
            outfile<<endl;
        }
        outfile<<endl;
    }
    outfile.close();
}

/* Ecrit les estimation de beta dans un fichier*/
void write_beta(Corpus c, VarEM model, const string &str) {
    fstream outfile(str.c_str(), ios::in|ios::out|ios::trunc);

    const unsigned int K = model.getK();
    const unsigned int V = c.getVocabularySize();
    for(unsigned int k=0; k<K; k++) {
        for(unsigned int v=0; v<V; v++) {
            outfile<<c.getBeta(k,v)<<" ";
        }
        outfile<<endl;
    }
    outfile.close();
}

int main(int argc, char* argv[]) {

	if (argc != 5) {
		std::cout<<"Commande incorrecte"<<std::endl;
		std::cout<<"rappel :"<<std::endl;
		std::cout<<"./varEM_estimate <nom du corpus> <beta initial> <nom de sortie du dernier beta estime> <nom de sortie du fichier de sortie des phi>"<<std::endl;
		return 0;
	}
    cout<<"Debut du programme\n";
    unsigned int K = 20;
	unsigned int nbIt = 1;
    VarEM model(K);

    Corpus c(argv[1], argv[2], K);

    cout<<"Démarrage E-step pour le corpus ...\n";
    model.EM_classify(c, nbIt);
	cout<<"E-step terminée avec succès pour l'ensemble du corpus\n";
    cout<<endl;

    cout<<"Stockage externe du dernier beta calculé ...\n";
    write_beta(c, model, argv[3]);
    cout<<"Stockage réalisé\n";
    cout<<endl;
    
	/* 
	 * Les fichiers de sortie utilisateur ont été utilisé pour débuger.
	 * Ils ne sont plus utiles à présent, ils ne sont donc plus générés.
	 *
    cout<<"Ecriture des résultats 'utilisateur' ...\n";
    write_res_EM(c, model, "User_Result_20NG_test_Last_Beta_bis_nd5_nm10");
    cout<<"Ecriture terminée avec succès\n";
    cout<<endl;
	*/
    
	cout<<"Ecriture de la matrice phi à parser lors du clustering ...\n";
    write_phi_to_cluster(c, model, argv[4]);
    cout<<"Ecriture terminée avec succès\n";
    cout<<endl;
    cout<<"Fin du programme\n";
}

