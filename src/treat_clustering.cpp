#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <cassert>

std::vector<unsigned int> parsing_classes(const std::string &str) {
    const char* filename = str.c_str();
    std::ifstream file(filename);
    std::string line;
    std::vector<unsigned int> res;
    unsigned int p;

    while (getline(file, line)) {
        std::istringstream iss(line);
        iss >> p;
        res.push_back(p);
    }


    return res;
}


std::vector<unsigned int> treat_clustering(const std::string &str) {
    const char* filename = str.c_str();
    std::ifstream file(filename);
    std::string line;
    unsigned int i = 0;
    unsigned int max_index = 0;
    std::vector<unsigned int>::iterator max;
    std::vector<unsigned int> res;

    while (getline(file, line)) {
        std::istringstream iss(line);
        std::vector<unsigned int> pars;
        while (iss >> i) {
            pars.push_back(i);
        }
        max = std::max_element(pars.begin(), pars.end());
        max_index = std::distance(pars.begin(), max)+1;
        res.push_back(max_index);
    }

    return res;
}



void results(const std::string &cluster, const std::string &corpus, char* file_name) {
	std::vector<unsigned int> expert_classes = parsing_classes(corpus);
	std::vector<unsigned int>predicted_classes = treat_clustering(cluster);
	std::vector<double> taux_classes(20);
	std::vector<unsigned int> classes_size(20);
	double taux_glob =0.0;
	
	assert(expert_classes.size() == predicted_classes.size());

	unsigned int i =0;
	for (unsigned int k=1; k<=20; k++) {
		while (expert_classes[i] == k && i<expert_classes.size()) {
			if(predicted_classes[i] == k){
				taux_glob += 1.0;
				taux_classes[k-1] += 1.0;
			}
			classes_size[k-1] += 1;
			i++;
		}
	}

	taux_glob /= 19997;
	for(unsigned int k=0; k<20; k++){
		taux_classes[k] /= classes_size[k];
	}

	std::fstream outfile(file_name, std::ios::in|std::ios::out|std::ios::trunc);
	for(unsigned int j =0; j<expert_classes.size(); j++) {
		outfile<<"Document "<< j+1 <<":" <<std::endl<<std::endl;
		outfile<<"Clustering expert : "<<expert_classes[j]<<std::endl;
		outfile<<"Clustering maison : "<<predicted_classes[j]<<std::endl<<std::endl<<std::endl;
	}
	
	outfile<<"Taux de bonne prediction pour la classe : \n";
	for(unsigned int k=0; k<20; k++){
		outfile<< k+1 <<" : "<<taux_classes[k]*100<<" % \n";
	}
	
	outfile<< std::endl <<"Taux de bonne prediction global : "<< taux_glob*100 <<" % \n";
    outfile.close();
}



int main(int argc, char* argv[]) {

  results(argv[2], argv[1], argv[3]);
  //std::vector<unsigned int> vect = parsing_classes("TF-20NG-Indexation");
  //Verif :
    /*std::fstream outfile_0("Verif_Theo", std::ios::out);
    for(unsigned int k=0; k<19997; k++) {
        outfile_0<<"classe doc "<<k+1<<" : "<<vect[k]<<"\n";
    }
    outfile_0.close();


    std::vector<unsigned int> res = treat_clustering("Clustering_fullCorp_nd5_nm10");
    //Verif :
    std::fstream outfile("Verif_Brams", std::ios::out);
    for(unsigned int k=0; k<19997; k++) {
        outfile<<"indice du max, ligne "<<k+1<<" : "<<res[k]<<"\n";
    }
    outfile.close();*/    
}
