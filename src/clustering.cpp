#include "clustering.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <cassert>

/* Constructeur de la classe cluster*/
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
			tabRes[current_doc][max] ++;
		}
	}
	cluster c;
	c.tab=tabRes;
	c.nbLine=nb_doc;
	c.nbColumn=line_length;
	
	return c;
}

void transform_cluster(const std::string &str1 ,const std::string &str2 ){
	const char* filename1 = str1.c_str();
	const char* filename2 = str2.c_str();
	std::ifstream file(filename1);
	std::string line;
	std::string word;
	std::fstream outfile(filename2, std::ios::in|std::ios::out|std::ios::trunc);
	
	while(getline(file, line)){
		int count =1;
		outfile<<"1 ";
		std::istringstream iss(line);
		while(iss >> word){
			std::string prefix;
			std::stringstream out;
			out << count;
			prefix = out.str();
			prefix = prefix+":";
			word = prefix + word +".0";
			outfile <<word<<" ";
			count += 1;
		}
		outfile<<std::endl;
	}
}

/* Transforme le fichier de sortie de gibbs ou varEM en un fichier exploitable par CEM ou treat_clustering*/
int main(int argc, char *argv[]){

	if(argc!=4){
		std::cout<<"erreur : nombre d'arguments inexact.\n Rappel de la commande : ./clustering <fichier résultat à parser> <nom fichier résultat intermédiaire> <nom du fichier de sortie utilisé en entrée de l'algo CEM>"<<std::endl;
		return 0;
	}
	std::fstream outfile(argv[2], std::ios::in|std::ios::out|std::ios::trunc);
	cluster c = parsing(argv[1]);
	for (unsigned int i=0;i<c.nbLine;i++){
		for (unsigned int j=0;j<c.nbColumn;j++){
			outfile<<c.tab[i][j]<<" ";
		}
        outfile<<std::endl;
	}
    outfile.close();
    transform_cluster(argv[2],argv[3]);
    
}
