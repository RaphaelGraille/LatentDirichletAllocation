CXX= g++
CXXFLAGS= -O3 -std=c++0x -Wall -g 
OBJ= Word.o Document.o Corpus.o VarEM.o Gibbs_sampling.o

all : beta varEM clustering gibbs top_words treat_un_cluster CEM

gibbs : Word.o Document.o Corpus.o Gibbs_sampling.o gibbs_main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

top_words : topWords.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clustering : clustering.o
	$(CXX) $(CXXFLAGS) -o $@ $^

beta : Word.o Document.o Corpus.o beta.o
	$(CXX) $(CXXFLAGS) -o $@ $^

varEM : $(OBJ) Corpus.o Word.o Document.o varEM_estimate.o
	$(CXX) $(CXXFLAGS) -o $@ $^

treat_cluster : treat_clustering.o treat_unsupervised_clustering.o
	$(CXX) $(CXXFLAGS) -o $@ $^

treat_un_cluster : treat_unsupervised_clustering.o
	$(CXX) $(CXXFLAGS) -o $@ $^

CEM : Word.o Document.o Corpus.o CEM.o
	$(CXX) $(CXXFLAGS) -o $@ $^

check: all

	@echo 
	@echo Lancement des exécutables ...
	@echo ======================================
	@echo
	@echo beta :
	@echo
	@./beta TF-20NG-Indexation Beta_nd5_nm10
	@echo
	@echo main :
	@echo
	@./main
	@echo
	@echo clustering :
	@echo
	@./clustering

clean:
	rm -f top_words
	rm -f CEM
	rm -f treat_cluster
	rm -f beta
	rm -f clustering
	rm -f treat_un_cluster
	rm -f gibbs
	rm -f varEM
	rm -f *.o
	rm -f *~

