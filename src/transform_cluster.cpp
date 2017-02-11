#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <cassert>

int main(int argc, char* argv[]){
	std::ifstream file(argv[1]);
	std::string line;
	std::string word;
	std::fstream outfile(argv[2], std::ios::in|std::ios::out|std::ios::trunc);
	
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
