#include "clustering.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>


cluster parsing(const std::string &str){
	const char* filename = str.c_str();
	std::ifstream file(filename);
	std::string line;
	unsigned int current_doc=0;
	unsigned int line_length=0;
	unsigned int nb_doc=0;
    double p;
	
	while(getline(file, line)){
		if (line.empty()) {
			nb_doc++;
		}
	}	

    file.clear();
	file.seekg(0, std::ios::beg);
	getline(file, line);
    std::istringstream iss(line);
	while (iss >> p){
		line_length++;
	}
	std::cout<<"fin du premier parsing, "<<"nombre de documents : "<<nb_doc<<" nombre de classes : "<<line_length<<std::endl;
	/* on retourne au début du fichier*/
	file.seekg(0, std::ios::beg);
    unsigned int** tabRes;
	tabRes=(unsigned int**)calloc(nb_doc,sizeof(unsigned int*));
	for (unsigned int k=0; k<nb_doc;k++){
		tabRes[k]=(unsigned int*)calloc(line_length,sizeof(unsigned int));
	}
	

	while (getline(file, line)){
		if (line.empty()) {
			current_doc++;
		}else {
            std::istringstream iss_2(line);
			/* stockage des valeurs de la ligne lu couramment */
			std::vector<double> word;
			while (iss_2 >> p){
				word.push_back(p);
			}

            
			/* recherche de l'indice de la composante de valeur maximale de la ligne*/
            std::vector<double>::iterator res = std::max_element(word.begin(), word.end());
			unsigned int max = std::distance(word.begin(), res);
            /*std::cout<<"val du max (ce qu'il y a dans *res : "<< *res << std::endl;
            std::cout<<"indice du max (ce que renvoie la fonction distance : "<< std::distance(word.begin(), res) << std::endl;
            std::cout<<"indice du max : ce qu'on a stocké dans max : "<< max <<std::endl;*/ 
			tabRes[current_doc][max] ++;
        }
	}
	cluster c;
	c.tab=tabRes;
	c.nbLine=nb_doc;
	c.nbColumn=line_length;
	
	return c;
}

int main(){
    std::fstream outfile("Clustering_nd1_nm10_UnS_24it", std::ios::in|std::ios::out|std::ios::trunc);
	cluster c = parsing("Cluster_nd1_nm10_UnS_24it");
	for (unsigned int i=0;i<c.nbLine;i++){
		for (unsigned int j=0;j<c.nbColumn;j++){
			outfile<<c.tab[i][j]<<" ";
		}
        outfile<<std::endl;
	}
    outfile.close();
}
		
		
		
	
	

