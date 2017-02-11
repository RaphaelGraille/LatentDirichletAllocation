#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <iostream>
#include <fstream>

void results(const std::string &cluster, const std::string &corpus, char* file_name) {
	std::vector<unsigned int> expert_classes = parsing_classes(corpus);
	std::vector<unsigned int>predicted_classes = treat_clustering(cluster);
	std::vector<double> taux_Classes(20);
	std::vector<unsigned int> classes_size(20);
	double taux_glob =0.0;
	
	assert(expert_classes.size() == predicted_classes.size());

	unsigned int i =0;
	for (unsigned int k=1; k<=20; k++) {
		while (expert_classes[i] == k) {
			if(predicted_classes[i] == k){
				taux_glob += 1.0;
				taux_classes[k-1] += 1.0;
			}
			classes_size[k-1] += 1;
			i++;
		}
	}

	taux_glob /= 20000;
	for(unsigned int k = 0; k<20; k++){
		taux_classes[k] /= classes_size[k];
	}

	std::fstream outfile(file_name, std::ios::in|std::ios::out|std::ios::trunc);
	for(unsigned int j =0; j<expert_classes.size()){
		outfile<<"Document "<< j+1 <<":" <<std::endl<<std::endl;
		outfile<<"Clustering expert : "<<expert_classes[j]<<std::endl;
		outfile<<"Clustering maison : "<<predicted_classes[j]<<std::endl<<std::endl<<std::endl;
	}
	
	outfile<<"Taux de bonne prediction pour la classe : ";
	for(unsigned int k =0; k<20; k++){
		outfile<< k <<" : "<<taux_classes[k]*100<<" % \n";
	}
	
	outfile<< std::endl <<"Taux de bonne prediction global : "<< taux_glob*100 <<" % \n";

}
