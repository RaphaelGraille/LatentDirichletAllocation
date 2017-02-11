#include "Corpus.h"
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>

/* Constructeur artificiel de corpus, lorsqu'aucun fichier de données n'est à disposition */
Corpus::Corpus(unsigned int vocabularySize, unsigned int docCount) {
    this->vocabularySize = vocabularySize;
    this->docCount = docCount;
    alpha = 1;
    //beta = 1;
    unsigned int defaultWordCount = 1000;
    for (unsigned int k=1; k<=docCount; k++) {
        Document d(defaultWordCount);
        docs.push_back(d);
    }
}


/* Construction par parsing de fichier */
Corpus::Corpus(const std::string &str, const std::string &str2, unsigned int k) {
    const char* filename = str.c_str();
    std::ifstream file(filename);
    std::string line;
    int d=0;
    std::cout<< "Parsing corpus ...\n";
    while (getline(file, line)) {
        //std::cout<<"debut parsing document "<<d<<std::endl;
        Document doc(line, k);
        docs.push_back(doc);
        //std::cout<<"fin parsing document "<<d<<std::endl;
        d++;
    }
    std::cout<<"Parsing corpus ok\n";
    vocabularySize = 59809;
    docCount = d;
    alpha = 1;
	allocateBeta(k);
	const char* filename2 = str2.c_str();
	std::ifstream file2(filename2);
	unsigned int i =0;
	unsigned int n =0;
	double betaCour;
	std::cout<<"Remplissage de beta \n";
	while(getline(file2, line)){
	    std::istringstream iss(line);
		while(iss>>betaCour) {
			//std::cout<<betaCour<<"\n";
			beta[i][n] = betaCour;
			n++;
		}
		n=0;
		i++;
	}
	std::cout<<"Remplissage de beta terminé \n";
    //randomInitBeta(k);
}

/* Construceur utilisé dans CEM*/
Corpus::Corpus(const std::string &str,unsigned int vocSize) {
    const char* filename = str.c_str();
    std::ifstream file(filename);
    std::string line;
    int d=0;
    std::cout<< "Parsing corpus ...\n";
    while (getline(file, line)) {
        //std::cout<<"debut parsing document "<<d<<std::endl;
        Document doc(line, 20);
        docs.push_back(doc);
        //std::cout<<"fin parsing document "<<d<<std::endl;
        d++;
    }
    std::cout<<"Parsing corpus ok\n";
    vocabularySize = vocSize;
    docCount = d;
    alpha = 1;
	beta=NULL;
}

/* Constructeur d'un sous corpus du corpus c de taille k*nbDoc contenant nbDoc document par classe*/
Corpus::Corpus(Corpus &c, unsigned int k, unsigned int nbDoc){
	std::srand(std::time(NULL));
	this->vocabularySize = c.getVocabularySize();
	this->docCount = k*nbDoc;
	this->alpha = c.alpha;
	this->docs.reserve(k*nbDoc);
	/* Selection aléatoire de nbDoc doc. par classe*/
	for(unsigned int i=0; i<k; i++){
		for(unsigned int j=0; j<nbDoc; j++){
			// On choisit un document au hasard dans le corpus
			unsigned int d=(std::rand()%c.getDocCount());
			std::cout<<d<<std::endl;
			while(c.getDocument(d).getWords().size()==0){d=(std::rand()%c.getDocCount());} //Permet d'éliminer les documents vides
			Document doc = Document();
			std::cout<<"Document selectionnée : "<< d <<"\n";
			doc = c.getDocument(d);
			docs.push_back(doc);
		}
	}
	/* Beta est PARTAGEE avec le corpus initial ! */
	this->beta = c.beta;
	NGInitBeta(k,nbDoc);
}

Corpus::~Corpus() {
}

unsigned int Corpus::getDocCount() const {
    return docCount;
}

std::vector<Document> & Corpus::getDocs() {
    return docs;
}

Document & Corpus::getDocument(const unsigned int &m) {
    return docs[m];
}

Document Corpus::getDocument2(const unsigned &m){
	return docs[m];
}

double Corpus::getAlpha() {
    return alpha;
}

double** Corpus::getBeta() {
    return beta;
}

double& Corpus::getBeta(unsigned int i, unsigned int j) {
	return beta[i][j];
}

void Corpus::setBeta(unsigned int i, unsigned int j, double d){
	beta[i][j] = d;
}

unsigned int Corpus::getVocabularySize(){
	return vocabularySize;
}

// Initialise beta de manière aleatoire.
void Corpus::randomInitBeta(unsigned int k){
	std::srand(std::time(NULL));
	
	for(unsigned int i=0; i<k; i++){
		for(unsigned int j=0; j<vocabularySize; j++){
			beta[i][j]= 1.0/vocabularySize + std::rand()/double(RAND_MAX);
		}
	}
}

// Initialise beta sur nbDoc pour chaque classe.
void Corpus::NGInitBeta(unsigned int k,unsigned int nbDoc){
	std::cout<<"Début de l'initialisation de beta :\n";
	std::cout<<"Nombre de docs : "<< docCount <<" "<< nbDoc*k << " "<< docs.size() <<"\n";
	std::srand(std::time(NULL));
	unsigned int mot;
	double* sumBeta = (double*)calloc(k,sizeof(double));
	
	/* Allocation de beta */
	allocateBeta(k);
	for(unsigned int i=0; i<k; i++){
		std ::cout<<"Initialisation de beta sur la classe :"<< i <<"\n";
		for(unsigned int j=0; j<nbDoc; j++){

			Document& doc = this->getDocument(i*nbDoc+j);
			for(unsigned int n=0; n<doc.getWords().size(); n++){
				mot = n;
				std::cout<<"Mot : " << doc.getWord(mot).getWordIndex() << "\n";
				getBeta(i,doc.getWord(mot).getWordIndex()) += doc.getWord(mot).getFreq();
				sumBeta[i] += doc.getWord(mot).getFreq();
			}
		}
		for(unsigned int n=0; n<vocabularySize;n++){
			getBeta(i,n) = (getBeta(i,n) + 1.0) /(sumBeta[i] + vocabularySize);
		}

	}
}

/* Réalise l'allocation de Beta*/
void Corpus::allocateBeta(unsigned int k){
	this->beta =(double**)malloc(k*sizeof(double*));
	for(unsigned int i =0; i<k; i++) {
		this->beta[i]=(double*)malloc(vocabularySize*sizeof(double));
	}
}
