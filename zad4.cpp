#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include "PermutationAlgorithms.hpp"
#include "graph.hpp"
#include "Random.hpp"
#include "Ilands.hpp"

using namespace std;


typedef struct {
    int x;
    int y;
} Point;

void processFile(const char* filePath) {
    mt19937 gen{std::random_device{}()};
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filePath);
        exit(1);
    }

    char line[256];
    int startLine = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "NODE_COORD_SECTION") != NULL) {
            startLine = 1;
            break;
        }
    }

    if (startLine == 0) {
        fprintf(stderr, "NODE_COORD_SECTION not found in file: %s\n", filePath);
        exit(1);
    }

    Point coordinates[3000];
    int coordinateIndex = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "EOF") != NULL) {
            break;
        }

        int x, y;
        sscanf(line, "%*d %d %d", &x, &y);
        coordinates[coordinateIndex].x = x;
        coordinates[coordinateIndex].y = y;
        coordinateIndex++;
    }

    int n = coordinateIndex;

    vector<int> distance(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int dist = round(sqrt(pow(coordinates[i].x - coordinates[j].x, 2) + pow(coordinates[i].y - coordinates[j].y, 2)));
            distance[i * n + j] = dist;
            distance[j * n + i] = dist;
        }
    }

    fclose(file);



    int it = 5;
    if(n > 1000){
        it = 2;
    }

    vector<pair<int,int>> inversions;
  
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if(i !=0 && j != n-1)
                inversions.push_back(make_pair(i,j));
        }
    }


    
    MST mst = MST(distance,n,0);
    
    double avg_weight2 = 0.0;
    int best_weight2 = 10000000;
    int generation_size = 80*5;
    int parrents_gen_size = 40*5;
    int itratrions = 100;
    double mutationRate = 0.1;
    double crossoverRate = 0.8;

    
    for (int i = 0; i < it; i++) {
        //vector<int> perm = randPerm(n);
        vector<int> perm;
        RandomGenerator RG = RandomGenerator(n,generation_size,inversions.size());
        vector<Person> generation = generateFirstGeneration(false,generation_size,mst,distance,n,RG);
        //geneticAlgorithm(perm,distance,n,generation,generation_size,parrents_gen_size,mutationRate,crossoverRate,itratrions,mst,RG);
        ilands(perm,distance,n,generation,generation_size,parrents_gen_size,mutationRate,crossoverRate,itratrions,mst,RG);
        int weight = calcWeight(perm, distance, n);
        cout << i <<" "<<weight <<endl;
        avg_weight2 += (double)weight;
        if (weight < best_weight2) {
            best_weight2 = weight;
        }
    }
    mst.deleteMST();
    
    printf("%s & & %d & %.3lf \\\\\\hline\n", filePath+6,  best_weight2, avg_weight2 / it);
}





int main(int argc, char *argv[]) {
    
    int option = atoi(argv[1]);
    if (option == 0) {
        processFile("testy/xqf131.tsp");
        processFile("testy/xqg237.tsp");
        processFile("testy/pma343.tsp");
        processFile("testy/pka379.tsp");
        processFile("testy/bcl380.tsp");
        processFile("testy/pbl395.tsp");
        processFile("testy/pbk411.tsp");
        processFile("testy/pbn423.tsp");
        processFile("testy/pbm436.tsp");
        processFile("testy/xql662.tsp");
    
    } else if (option == 1) {
       processFile("testy/xit1083.tsp");
    }else if (option == 4){ 
        processFile("testy/icw1483.tsp");
    }else if (option == 5){
        processFile("testy/djc1785.tsp");
    }else if (option == 2){
        processFile("testy/dcb2086.tsp");
    }else if (option == 3){
        processFile("testy/pds2566.tsp");
    
    }


    return 0;
}
