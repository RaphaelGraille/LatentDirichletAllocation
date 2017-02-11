/*

   Modèle de Naïve Bayes

*/

#include <cstdio>
#include <math.h>
#include <string.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include "Corpus.h"

double** allocateTab(unsigned int l, unsigned int c){
	double ** res =(double**)calloc(l,sizeof(double*));
	for(unsigned int i =0; i<l; i++) {
		res[i]=(double*)calloc(c,sizeof(double));
	}
	return res;
}

void ChargeFic(char* FicBase,unsigned int N,unsigned int Attribut)
	//float **X;
{/*
	long int i, j, k, NumeroClasse, x, U;
	float a;
	char Temp, Saut[10];
	FILE *fd1, *fd2;

	if((fd1=fopen(FicBase,"r"))==NULL)
	{
		printf("Erreur d'ouverture du fichier Base%s\n", FicBase);
		exit(0);
	} 

	for(j=1; j<=N; j++)
		for(i=1; i<=Attribut; i++)
			fscanf(fd1,"%f",&X[i][j]);

	fclose(fd1);
*/}

int main(int argc, char *argv[])
{
	double	**Pjk;
   	double *Pi; 
	double Somme; 
	double tmp; 
	double Max; 
	double Lnew; 
	double Lold;
	unsigned int* D;
	int MaxEpoque=1000, Epoque=1, IndCls;
	unsigned long U, i, j, k, n, P, C;
	std::fstream outfile(argv[5], std::ios::in|std::ios::out|std::ios::trunc);

	std::srand(std::time(NULL));
	if(argc!=6)
		std::cout<<"Usage : ./CEM-NB-UnSL <FicName> <Card> <Attributs> <NumOfClusters> <Outfile>"<<std::endl;
	/*
	sscanf(argv[2],"%ld",&n);
	sscanf(argv[3],"%ld",&P);
	sscanf(argv[4],"%ld",&C);
	*/

	n=strtoul(argv[2],NULL,10);
	P=strtoul(argv[3],NULL,10);
	C=strtoul(argv[4],NULL,10);
	Corpus c=Corpus(argv[1],P);

	std::cout<<C;
	assert(n==c.getDocCount());
	assert(P==c.getVocabularySize());

	//X=(float **)matrix(1,P,1,n);
	D=(unsigned int*)calloc(n,sizeof(unsigned int));
	Pjk=allocateTab(P,C);
	Pi=(double *)calloc(C,sizeof(double));

	// Ne change pas
	// Associe à chaque doc une classe aléatoirement
	
	std::cout<< n<<"\n";
	std::cout<< P<<"\n";
	std::cout<< C<<"\n";
	std::cout<<"Initialisation de D \n";
	for(i=0; i<n; i++)
	{
		U=(unsigned int)((double)(std::rand())/(double)(RAND_MAX)*(double)C);
		D[i]=U;
	}

	//ChargeFic(argv[1], n , P, X);

	double SommeV[C];
	for(i=0; i<C; i++){
		SommeV[i]=0.0;
	}
	std::cout<<"Initialisation de Pjk \n";
	// Initialisation des Pjk
	for(unsigned int d=0; d<c.getDocCount(); d++)
	{
		// Si le doc est de classe i, on ajoute a Pjk le nombre d'occurence de mot dans le doc
		// Comme dans l'initialisation de beta
		for(unsigned int w=0; w<c.getDocument(d).getWords().size(); w++){
			// a modifier en ajoutant c.getDocument(n).getWord("ce qu'il faut").getWordIndex()
			Pjk[c.getDocument(d).getWord(w).getWordIndex()][D[d]] += (double)c.getDocument(d).getWord(w).getFreq();
			SommeV[D[d]]+= (double)c.getDocument(d).getWord(w).getFreq();
		}
	}

	// Normalisation
	for(j=0; j<c.getVocabularySize(); j++){
		for(k=0; k<C; k++){
			tmp=Pjk[j][k];
			Pjk[j][k]=(1.0+tmp)/((double)c.getVocabularySize()+SommeV[k]);
		}
	}
	
	std::cout<<"Initialisation de Pi \n";
	// Initialisation des Pi
	for(i=0; i<n; i++){
		Pi[D[i]]+=1;
	}
	for(unsigned int k =0; k<C; k++){
		// Normalisation
		Pi[k]=(1.0+Pi[k])/((double)C+(double)n);
	}

	for(k=0; k<C; k++){
		// Changer pour le c++
		std::cout<<"Pi["<< k+1 <<"]="<<Pi[k]<<std::endl<<std::endl;
	}

	for(k=0; k<C; k++){
		for(j=0; j<c.getVocabularySize(); j++){
			std::cout<<"k : "<<k<<" j : "<<j<<std::endl;
			std::cout<<Pjk[j][k]<<std::endl;
		}
	}
	std::cout<<"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n";

	Lnew=1.0;
	Lold=0.0;
	MaxEpoque=1000;
	Epoque=1;
	while((fabs(Lnew-Lold)>=0.001) && (Epoque<=MaxEpoque))
	{
		Lold=Lnew;

		/* ----------------------------------- Classification ------------------------------------------*/
		std::cout<<"Debut classification\n";
		for(i=0; i<n; i++) {
			Max=-10e10;
			for(k=0; k<C; k++) {
				Somme=log(Pi[k]);
				for(j=0; j<c.getDocument(i).getWords().size(); j++){
					//Somme+=X[j][i]*log(Pjk[j][k]);
					Somme+=c.getDocument(i).getWord(j).getFreq()*log(Pjk[c.getDocument(i).getWord(j).getWordIndex()][k]);
				}
				if(Somme>Max)
				{
					Max=Somme;
					IndCls=k;
				}
			}

			D[i]=IndCls;
		}
		std::cout<<"Fin classification\nDebut EM\n";
		/* ----------------------------------------------- EM --------------------------------------- */

		for(unsigned int i=0; i<C; i++){
			SommeV[i]=0.0;
		}
		// Initialisation des Pjk
		for(unsigned int d=0; d<n; d++)
		{
			// Si le doc est de classe i, on ajoute a Pjk le nombre d'occurence de mot dans le doc
			// Comme dans l'initialisation de beta
			for(unsigned int w=0; w<c.getDocument(d).getWords().size(); w++){
				// a modifier en ajoutant c.getDocument(n).getWord("ce qu'il faut").getWordIndex()
				Pjk[c.getDocument(d).getWord(w).getWordIndex()][D[d]] += c.getDocument(d).getWord(w).getFreq();
				SommeV[D[d]]+= c.getDocument(d).getWord(w).getFreq();
			}
		}

		// Normalisation
		for(j=0; j<P; j++){
			for(k=0; k<C; k++){
				tmp=Pjk[j][k];
				Pjk[j][k]=(1.0+tmp)/((double)P+SommeV[k]);
			}
		}

		// Misa à jour de Pi
		for(i=0; i<n; i++){
			Pi[D[i]]+=1;
		}
		for(unsigned int k =0; k<C; k++){
			// Normalisation
			Pi[k]=(1.0+Pi[k])/((double)C+(double)n);
		}

		std::cout<<"Fin EM\n";
		/* ------------------------------------ Vraisemblance classifiante ------------------------------ */

		Lnew=0.0;
		for(i=0; i<n; i++){
			for(k=0; k<C; k++)
			{
				Somme=log(Pi[k]);
				for(j=0; j<c.getDocument(i).getWords().size(); j++){
					Somme+=c.getDocument(i).getWord(j).getFreq()*log(Pjk[c.getDocument(i).getWord(j).getWordIndex()][k]);
				}

				if(D[i]==k)
					Lnew+=Somme;
			}
		}

		std::cout<<"Epoque="<<Epoque << "Lnew=" << Lnew <<"Lold="<<Lold<<"\n";
		Epoque++;
	}

	//Proba=fopen("ResuTest.NAIVE","w");

	for(i=0; i<n; i++)
	{
		outfile<<D[i]+1<<std::endl;
	}

	/*
	free_matrix(X,1,P,1,n);
	free_matrix(D,1,C,1,n);
	free_matrix(Pjk,1,P,1,C);
	free_vector(Pi,1,C);
	*/
}


