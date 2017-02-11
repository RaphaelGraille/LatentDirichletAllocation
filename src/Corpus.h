#ifndef _Corpus_
#define _Corpus_

#include "Document.h"
#include <vector>


class Corpus {
    public :
        Corpus(unsigned int vocabularySize, unsigned int docCount);
        Corpus(const std::string &str, const std::string &str2, unsigned int k);
        Corpus(const std::string &str, unsigned int vocSize);
		Corpus(Corpus &c, unsigned int k, unsigned int nbDoc);
        ~Corpus();

        /* renvoie le nbe de document */
        unsigned int getDocCount() const;

        /* renvoie la liste de docs */
        std::vector<Document> & getDocs();

        /* renvoie le mième document du corpus */
        Document & getDocument(const unsigned int &m); 
		
		Document getDocument2(const unsigned &m);

        double getAlpha();

        double** getBeta();

		unsigned int getVocabularySize();

        /* affichage corpus sous forme vectorielle */
        void display(std::ostream &str);

		/* initialise beta de maniere aléatoire*/
		void randomInitBeta(unsigned int k);
		
		void NGInitBeta(unsigned int k,unsigned int nbDoc);
		
		void NGLightInitBeta(unsigned int k);

		/* renvoie la valeur de beta[i][j]*/
		double& getBeta(unsigned int i, unsigned int j);

		void setBeta(unsigned int i, unsigned int j, double d);

		void allocateBeta(unsigned int k);

    private :
        /* nbe de docs */
        unsigned int docCount;
        /* taille du vocabulaire associé au corpus */
        unsigned int vocabularySize;
        /* tableau contenant tous les documents du corpus */
        std::vector<Document> docs;
        /* Parametre alpha du modele (sans inférence)*/
        double alpha;
        /* Parametre beta du modele */
		double** beta;

};

#endif
