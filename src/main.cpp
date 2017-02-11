#include "Corpus.h"
#include "VarEM.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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

int main() {
    cout<<"Debut du programme\n";
    unsigned int K = 20;
    VarEM model(K);
    //Corpus c("BaseReuters-5", K); //Ligne à décommenter si on souhaite utiliser BaseReuters-5

    Corpus c("TF-20NG-Indexation", "Beta_nd100_nmAll", K); //Ligne à commenter si on souhaite utiliser BaseReuters-5

    cout<<"Démarrage E-step pour le corpus ...\n";
    //model.EM_classify(c, 5);
    model.E_step(c,K);
	cout<<"E-step terminée avec succès pour l'ensemble du corpus\n";
    cout<<endl;

    /*cout<<"Stockage externe du dernier beta calculé ...\n";
    write_beta(c, model, "Last_Beta_ter_nd5_nm10");
    cout<<"Stockage réalisé\n";
    cout<<endl;
    
    cout<<"Ecriture des résultats 'utilisateur' ...\n";
    write_res_EM(c, model, "User_Result_20NG_test_Last_Beta_bis_nd5_nm10");
    cout<<"Ecriture terminée avec succès\n";
    cout<<endl;*/

    cout<<"Ecriture de la matrice phi à parser lors du clustering ...\n";
    write_phi_to_cluster(c, model, "Cluster_nd100_nmAll");
    cout<<"Ecriture terminée avec succès\n";
    cout<<endl;
    cout<<"Fin du programme\n";
}

