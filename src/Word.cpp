#include "Word.h"
#include <cstdlib>

/* Par défaut, mise à 0 de l'index à 0 */
Word::Word() {
    this->index = 0;
}

/* Constructeur allouant un indice non nul à un mot du vocabulaire;
 * Si condition non respectée, indice mis à 0 
 * */
Word::Word(unsigned int index, unsigned int freq) {
    if (index>0) {
        this->index = index;
    } else {
        this->index = 0;
    }
	this->freq = freq;
}

Word::~Word() {
}


void Word::setIndex(unsigned int i) {
    index = i;
}

unsigned int Word::getWordIndex() const {
    return index;
}

void Word::display(std::ostream &str) const {
    str<<index ;
}
		
unsigned int Word::getFreq() const{
	return freq;
}
		
void Word::setFreq(unsigned int freq){
	this->freq=freq;
}
