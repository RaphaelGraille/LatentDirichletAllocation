#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]){
	unsigned int nbClasses =20;
	unsigned int nbTopWords =10;
	unsigned int vocSize = 59809;
	unsigned int numClass =1;
	unsigned int max_index;
	std::vector<double> beta_i;
	std::vector<std::string> vocabulary;
	std::vector<std::string> tmpvocabulary;
	std::string curWord;
	std::vector<double>::iterator max;
	std::ifstream beta(argv[2]);
	std::ifstream voc(argv[1]);
    std::string line;
	std::fstream outfile(argv[3], std::ios::in|std::ios::out|std::ios::trunc);

	while(getline(voc, line)){
		std::istringstream iss(line);
		iss>>curWord;
		vocabulary.push_back(curWord);
	}

	double betaCour;
	while(getline(beta, line)){
	    std::istringstream iss2(line);
		beta_i.clear();
		while(iss2>>betaCour) {
			beta_i.push_back(betaCour);
		}
		outfile<<"Top Words pour la classe : " << numClass <<std::endl;
		tmpvocabulary = vocabulary;
		for(unsigned int i=0; i<nbTopWords;i++){
			max = std::max_element(beta_i.begin(),beta_i.end());
			max_index = std::distance(beta_i.begin(), max);
			//std::cout<< max_index<<std::endl;
			outfile<< tmpvocabulary[max_index]<<std::endl;
			beta_i.erase(beta_i.begin()+max_index);
			tmpvocabulary.erase(tmpvocabulary.begin()+max_index);
		}
		outfile<<std::endl;
		numClass++;
	}
}
