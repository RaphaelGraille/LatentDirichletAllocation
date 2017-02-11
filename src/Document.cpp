#include "Document.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>


/* Document vide, crée dans les cas par défaut */
Document::Document() {
}

/* Constructeur "manuel" de documents, avec des données crées artificiellement */
Document::Document(unsigned int wordCount) {
    for (unsigned int k=1; k<=wordCount; k++) {
        Word w(k,1);
        words.push_back(w);
    }
}

/* Parser un document depuis un fichier de données */
Document::Document(std::string line, unsigned int k) {
    unsigned int v;
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream iss(line);
    iss >> v;
    unsigned int N=0;
	double totalLength =0.0;
    unsigned int wordIndex;
    double frequency;
    while (iss >> wordIndex && iss >> frequency) {
        Word w(wordIndex-1,(unsigned int)(frequency));
        words.push_back(w);
        //freqs[i].push_back(frequency);
        N++;
		totalLength += frequency;
    }
	this->setTotalLength(totalLength);
    for (unsigned int i=0; i<k; i++) {
    gamma.push_back(0.0);
    }
    std::vector<double> vect(k,0.0);
    for (unsigned int n=0; n<N; n++) {
        phi.push_back(vect);
    }
}


/*Document::Document(Document& doc){
	this->words = doc.getWords();
	this->gamma = doc.getGamma();
	this->phi = doc.getPhi();
	this->totalLength = doc.getTotalLength();
}*/

Document::~Document() {
}

void Document::setWord(unsigned int i, unsigned int j) {
    words[i].setIndex(j);
}

Word Document::getWord(unsigned int i) const {
    return words[i];
}

std::vector<Word> Document::getWords() const{
    return words;
}

std::vector<double> & Document::getGamma() {
    return gamma;
}

std::vector<std::vector<double>> & Document::getPhi() {
    return phi;
}

double & Document::getGamma(const unsigned int &i) {
    return gamma[i];
}

std::vector<double> Document::getPhi(unsigned int n) {
    return phi[n];
}

double & Document::getPhi(const unsigned int &n, const unsigned int &i) {
    return phi[n][i];
}

void Document::setPhi(unsigned int n, unsigned int i, double d) {
    phi[n][i] = d;
}

void Document::setGamma(unsigned int i, double d) {
    gamma[i] = d;
}

void Document::display(std::ostream &str) const {
    for (unsigned int i=0; i<words.size(); i++) {
        words[i].display(str);
        std::cout<<" ";
    }
    str<<std::endl;
}

void Document::setTotalLength(unsigned int l) {
	this->totalLength = l;
}	

unsigned int Document::getTotalLength() const{
	return this->totalLength;
}
