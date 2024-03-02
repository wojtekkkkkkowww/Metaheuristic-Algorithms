#include "PermutationAlgorithms.hpp"
#include "Random.hpp"

void RandomLocalSearch(vector<int>& perm,int n,vector<pair<int,int>> inversions,vector<int> distance,int iterations,RandomGenerator rg){
    int current_solution_weight = calcWeight(perm,distance,n);
    int i_c = 0;
    int j_c = 0;
    while (true){
        int best_neighbour_weight = current_solution_weight;

        for (int i = 0; i < n; i++) {
            int l = rg.getRandomInv();
            pair<int,int>inv = inversions[l];    
            int w_temp = calcInversedWeight(current_solution_weight, inv.first, inv.second, perm, n, distance);
            
            if(w_temp < best_neighbour_weight){
                best_neighbour_weight = w_temp;
                i_c = inv.first;
                j_c = inv.second;
            }
        
        }

        if (best_neighbour_weight < current_solution_weight) {
            reverse_section(perm, i_c, j_c);
            current_solution_weight = best_neighbour_weight;
        } else {
            break;
        }
    }

}