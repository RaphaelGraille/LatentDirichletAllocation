#ifndef _Word_
#define _Word_

#include <ostream>

class Word {
    public :
        Word();
        Word(unsigned int index, unsigned int freq);
        ~Word();
	
        /* renvoie le seul indice du tableau dont le contenu vaut 1, un mot étant un vecteur unitaire par définition  */
        unsigned int getWordIndex() const;
	
        /* mise a jour de l'indice d'un mot */
        void setIndex(unsigned int i);
	
	/* renvoie le nombre d'occurence du mot dans le doc*/
	unsigned int getFreq() const;

	/* mise à jour de a fréquence du mot*/
	void setFreq(unsigned int freq);
	
        /* affiche l'indice du mot au sein du vocabulaire associé */
        void display(std::ostream &str) const;
	
 private :
        /* index à la place de vector, pour des raisons de performance et de coût en mémoire */
        unsigned int index;
	
	/* nombre d'occurences du mot dans le doc*/
	unsigned int freq;
};

#endif
