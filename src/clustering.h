#ifndef _clustering_
#define _clustering_

#include "Document.h"
#include <vector>
typedef struct {
	unsigned int** tab;
	unsigned int nbLine;
	unsigned int nbColumn;
}cluster ;
cluster parsing(const std::string &str);
void transform_cluster(const std::string &str1 ,const std::string &str2);

#endif
