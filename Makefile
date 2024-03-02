CC = g++
CFLAGS = -Wall -O3
ob = zad4.o GeneticAlgorithm.o graph.o PermutationAlgorithms.o

zad4: $(ob) 
	$(CC) $(CFLAGS) -o zad4 zad4.o GeneticAlgorithm.o graph.o PermutationAlgorithms.o

zad4.o: zad4.cpp Ilands.hpp graph.hpp PermutationAlgorithms.hpp Random.hpp
	$(CC) $(CFLAGS) -c zad4.cpp

GeneticAlgorithm.o: GeneticAlgorithm.cpp GeneticAlgorithm.hpp graph.hpp PermutationAlgorithms.hpp Random.hpp LocalSearch.hpp
	$(CC) $(CFLAGS) -c GeneticAlgorithm.cpp

graph.o: graph.cpp graph.hpp
	$(CC) $(CFLAGS) -c graph.cpp

PermutationAlgorithms.o: PermutationAlgorithms.cpp PermutationAlgorithms.hpp 
	$(CC) $(CFLAGS) -c PermutationAlgorithms.cpp

clean:
	rm $(ob) zad4
