#ifndef _Document_
#define _Document_

#include "Word.h"
#include <string>
#include <ostream>
#include <vector>

class Document {
    public :
        Document();
        Document(unsigned int wordCount);
        Document(std::string line, unsigned int k);
	//Document(Document& doc);
	~Document();
	
        /* mise a jour d'un mot du document */
        void setWord(unsigned int i, unsigned int j);
        
        /* renvoie le iè mot d'un document */
        Word getWord(unsigned int i) const;
        
        /* renvoie la liste de mots */
        std::vector<Word> getWords() const;

        /* renvoie gamma */
        std::vector<double> & getGamma();
       
        /* renvoie phi */
        std::vector<std::vector<double>> & getPhi();

        /* renvoie iè terme de gamma */
        double & getGamma(const unsigned int &i);

        /* renovie le vecteur phi(n) de taille k */
        std::vector<double> getPhi(unsigned int n);

        /* renvoie terme (n,i) de phi */
        double & getPhi(const unsigned int &n, const unsigned int &i);

        void setPhi(unsigned int n, unsigned int i, double d);

        void setGamma(unsigned int i, double d);
	
	unsigned int getTotalLength() const;
	
	void setTotalLength(unsigned int l);
		
	/* affichage sous forme vectorielle du document */
	void display(std::ostream &str) const;
 private :
	/* vector contenant tous les mots du document */
	std::vector<Word> words;
	/* tableau gam */
	std::vector<double> gamma;
	/* tableau phi */
	std::vector<std::vector<double>> phi;
	/* nomre de mots dans le document*/
	double totalLength;
};

#endif
