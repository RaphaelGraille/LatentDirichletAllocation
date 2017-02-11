#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <cassert>

/* Récupère le clustering expert*/
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

/* Recupere la classe associée a chaque doc. par CEM*/
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

/* Associe à chaque document dans l'espace des topics une classe*/
std::vector<unsigned int> treat_CEM_clustering(const std::string &str){
	const char* filename = str.c_str();
    std::ifstream file(filename);
    std::string line;
    std::vector<unsigned int> res;
	unsigned int classe;	
	while (getline(file, line)){
        std::istringstream iss(line);
		iss >> classe;
		res.push_back(classe);
	}
	return res;
}

void results(const std::string &cluster, const std::string &corpus, char* file_name) {
	unsigned int nbClasses = 20;
	std::vector<unsigned int> expert_classes = parsing_classes(corpus);
	//std::vector<unsigned int>predicted_classes = treat_clustering(cluster);
	std::vector<unsigned int>predicted_classes = treat_CEM_clustering(cluster);
	std::vector<unsigned int> classes_size(nbClasses);
	std::vector<unsigned int> cluster_size(nbClasses);
	std::vector<unsigned int> topic_assigned_to_cluster(nbClasses);
	unsigned int repartition[nbClasses][nbClasses];
	for (unsigned int i=0; i<nbClasses; i++){
		for(unsigned int j=0; j<nbClasses; j++) {
			repartition[i][j] =0;
		}
	}
	
	assert(expert_classes.size() == predicted_classes.size());

	for(unsigned int i =0;i<expert_classes.size();i++){
		repartition[expert_classes[i]-1][predicted_classes[i]-1] += 1;
		classes_size[expert_classes[i]-1] += 1;
		cluster_size[predicted_classes[i]-1] += 1;
	}
		
	// On assigne a chaque cluster une classe experte.	
	for(unsigned int i=0; i<nbClasses; i++){
		topic_assigned_to_cluster[i] = 0;
		for(unsigned int j=0; j<nbClasses; j++){
			if(repartition[j][i] > repartition[topic_assigned_to_cluster[i]][i]) {
				topic_assigned_to_cluster[i] = j; 
			}
		}
	}

	std::fstream outfile(file_name, std::ios::in|std::ios::out|std::ios::trunc);
	for(unsigned int i =0; i<nbClasses ; i++) {
		outfile<<"Classe experte : "<< i+1 <<std::endl;
		for(unsigned int j=0; j<nbClasses; j++){
			outfile<<repartition[i][j]<<" ";
		}
		outfile<<std::endl<<std::endl;
	}

	double F_mean =0.0;
	for(unsigned int i =0; i<nbClasses ; i++) {
		outfile<<"Cluster "<< i+1 << " : classe experte majoritaire : " << topic_assigned_to_cluster[i] + 1<<std::endl;
		F_mean += 2.0*(double)(repartition[topic_assigned_to_cluster[i]][i])/((double)cluster_size[i]+(double)classes_size[topic_assigned_to_cluster[i]]);
		outfile<<"F-mesure : "<< 2.0*(double)(repartition[topic_assigned_to_cluster[i]][i])/((double)cluster_size[i]+(double)classes_size[topic_assigned_to_cluster[i]]) <<std::endl;
		outfile<<std::endl<<std::endl;
	}
	F_mean = F_mean/(double)nbClasses;
	outfile<< "Macro-F : "<<F_mean;
}

int main(int argc, char* argv[]) {
	if(argc!=4){
		std::cout<<"Erreur : nombre d'arguments inexact.\n";
		std::cout<<"Rappel de la commande : ./treat_un_cluster <Corpus utilise> <fichier d'entré> <fichier de sortie>\n";
	return 0;
	}
	results(argv[2], argv[1], argv[3]);
}
