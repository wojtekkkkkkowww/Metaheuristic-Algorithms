#include <iostream>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>

#include "GeneticAlgorithm.hpp"
#include "graph.hpp"
#include "PermutationAlgorithms.hpp"
#include "Random.hpp"
#include "LocalSearch.hpp"

enum SelectOption{
    Roulette,
    Tournament,
    Random
};

bool compareByFenotype(const Person& a, const Person& b) {
    return a.fenotype < b.fenotype;
}


struct ComparePerson {
    bool operator()(const Person& lhs, const Person& rhs) const {
        return lhs.fenotype < rhs.fenotype;
    }
};


//O(n)
void evaluateGeneration(std::vector<Person>& generation, const std::vector<int> distance, int n, vector<int>& bestPerm, int& bestW)
{
    //Zakłądam że wartości fenotypu są policzone
    for(int i = 0; i < generation.size(); i++){
        

        if(generation[i].fenotype < bestW){
            bestW = generation[i].fenotype;
            bestPerm = generation[i].genotype;
        }
    }

    //Prawdpopodobienstwo wyboru osobnika do krzyzowania
    for(int i = 0; i < generation.size(); i++){
        generation[i].propability =  bestW/(double)(generation[i].fenotype);
    }
}


void parentSelection(std::vector<Person>& generation, std::vector<pair<Person,Person>>& Parents, int numberOfParents, int n, SelectOption option,RandomGenerator RG){
 


    switch (option)
    {
        case Roulette:
        {   
            
            while(Parents.size() < numberOfParents){
                double r = RG.getReal();
                double sum = 0;
                int j = 0;

                while(sum < r && j < generation.size()){
                    sum += generation[j].propability;
                    j++;
                }
                int p = j-1;
                
                r = RG.getReal();
                sum = 0;
            
                while(sum < r && j < generation.size()){
                    sum += generation[j].propability;
                    j++;
                }
            
                int q = j-1;

                Person p1 = generation[p];
                Person p2 = generation[q];

                Parents.push_back(make_pair(p1,p2));
            }

            break;
        }
        case Tournament:
        {   int k = 20;
        
            while(Parents.size() < numberOfParents){
                vector<Person> tournament;
                while(tournament.size() < k){
                    int r = RG.getRandomPerson();
                    tournament.push_back(generation[r]);
                }
                sort(tournament.begin(),tournament.end(),compareByFenotype);

                //Wyniki turnieju 
                Person p1 = tournament[0];
                Person p2 = tournament[1];    
                
                Parents.push_back(make_pair(p1,p2));
            }
            break;  

        }
        case Random:
        {   //O(n* log(n)
            while(Parents.size() < numberOfParents){
                int r = RG.getRandomPerson();
                int p = RG.getRandomPerson();
                Person p1 = generation[r];
                Person p2 = generation[p];

                Parents.push_back(make_pair(p1,p2));
            }
        }
    }


}


pair<Person,Person> roulette(int numberOfParents,vector<Person> generation ,RandomGenerator RG){
    double r = RG.getReal();
    double sum = 0;
    int j = 0;

    while(sum < r && j < generation.size()){
        sum += generation[j].propability;
        j++;
    }
    int p = j-1;
                
    r = RG.getReal();
    sum = 0;
            
    while(sum < r && j < generation.size()){
        sum += generation[j].propability;
        j++;
    }
            
    int q = j-1;

    Person p1 = generation[p];
    Person p2 = generation[q];

    return make_pair(p1,p2);  
}



pair<Person,Person> tournament(int numberOfParents,vector<Person> generation ,RandomGenerator RG){
    int k = numberOfParents/2;
    vector<Person> tournament;
    while(tournament.size() < k){
            int r = RG.getRandomPerson();
            tournament.push_back(generation[r]);
    }
    sort(tournament.begin(),tournament.end(),compareByFenotype);

        //Wyniki turnieju 
        Person p1 = tournament[0];
        Person p2 = tournament[1];    
                
    return (make_pair(p1,p2));
}



void mutation(std::vector<Person>& generation, double mutationRate, int n, vector<pair<int,int>> inversions,const std::vector<int> distance,RandomGenerator RG)
{
    
    for(int i = 0; i < generation.size(); i++){
        double r = RG.getReal();
    
        if(r < mutationRate){
            auto inv = inversions[RG.getRandomInv()]; //losowanie inwersji 
           
        //    int inv_weight = calcInversedWeight(generation[i].fenotype,inv.first,inv.second,generation[i].genotype,n,distance);
            double q = RG.getReal();
            if (q < 0.5){
                reverse_section(generation[i].genotype, inv.first, inv.second);
            }else{
                swap(generation[i].genotype[inv.first],generation[i].genotype[inv.second]);
            }
            
        
            generation[i].fenotype = calcWeight(generation[i].genotype,distance,n);

        }



    }
}



Person Pointcross(Person parent1,Person parent2, int n, int k, const std::vector<int> distance){
    vector<bool> used(n,false); 
    Person child;
    vector<int> childGenotype;

    for(int i = 0; i < k; i++){
        //v to jest element permutacji  
        int v = parent1.genotype[i];
        used[v] = true;
        childGenotype.push_back(v);
    }

    for(int i = 0; i < n; i++){
        int v = parent2.genotype[i];
        if(!used[v]){
            childGenotype.push_back(v);
        }
    }    

    child.genotype = childGenotype;
    child.fenotype = calcWeight(child.genotype,distance,n);
    return child;
}



Person twoPointcross(Person parent1,Person parent2, int n, int k,int l, vector<int> distance){
    vector<bool> used(n,false); 
    Person child;
    vector<int> childGenotype(n,-1);

    for(int i = 0; i < k; i++){
        int v = parent1.genotype[i];
        used[v] = true;
        childGenotype[i] = v;
    }
    for(int i = l + 1; i < n; i++){
        int v = parent1.genotype[i];
        used[v] = true;
        childGenotype[i] = v;
    }
  
    unordered_set<int> Unused_ind;
    for(int i = 0; i < n; i++){
        if(childGenotype[i] == -1){
            Unused_ind.insert(i);
        }
    }
  
    for(int i = 0; i < n; i++){
        int v = parent2.genotype[i];
        if(!used[v]){
            int ind = *Unused_ind.begin();
            Unused_ind.erase(ind);
            childGenotype[ind] = v;
        }
    }    
    

    child.genotype = childGenotype;
    child.fenotype = calcWeight(child.genotype,distance,n);
    return child;
}



vector<Person> generateFirstGeneration(bool randomGen,int generation_size,MST mst,vector<int> distance, int n ,RandomGenerator RG){
    //Przygotowanie poczotkowej populacji
    //O(G*n)
    vector<Person> generation;
    if(!randomGen){
        //rozwiazania oparte na MST + Losowe
        int r = 0;
        while (generation.size() < generation_size){
            if (r < n){
                double q = RG.getReal();
                if(q < 0.7) {
                Person p;
                p.genotype = mst.DFS(r);
                p.fenotype = calcWeight(p.genotype, distance, n);
                p.age = RG.getRandomAge();
                generation.push_back(p);   
                }
            }
            Person p2;
            p2.genotype = randPerm(n);
            p2.fenotype = calcWeight(p2.genotype, distance, n);
            p2.age = RG.getRandomAge();
            generation.push_back(p2);
            r++;
        }
        
    }
    else{
        //rozwiazania losowe
        while(generation.size() < generation_size){
            Person p;
            p.genotype = randPerm(n);
            p.fenotype = calcWeight(p.genotype, distance, n);
            p.age = RG.getRandomAge();
            generation.push_back(p);
        }   
    }
    std::shuffle(generation.begin(), generation.end(), RG.getGen());
    return generation;

}
  



void geneticAlgorithm(std::vector<int>& perm, const std::vector<int> distance, int n,vector<Person>& generation,int generation_size,int parrents_gen_size ,double mutationRate, double crossoverRate,int itrerations,MST mst,RandomGenerator RG){
    
    vector<pair<int,int>> inversions;
  
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if(i !=0 && j != n-1)
                inversions.push_back(make_pair(i,j));
        }
    }


    //Przygotowanie poczotkowej populacji
    sort(generation.begin(),generation.end(),compareByFenotype); //O(GlogG) 
  
    //Pierwsza ewaluacja
    int best = generation[0].fenotype;
    vector<int> bestPerm = generation[0].genotype;
    evaluateGeneration(generation, distance, n, bestPerm, best); //O(G)


    int it = 0;

    while (it < itrerations){
    
       
        //Krzyżowanie
        vector<Person> children;
    
        for(int i = 0; i < parrents_gen_size; i++){
            double r = RG.getReal();
            if(r < crossoverRate){
               // pair<Person,Person> p = tournament(parrents_gen_size,generation,RG);       
                pair<Person,Person> p = roulette(parrents_gen_size,generation,RG);       
                
                int k =  RG.getRandomPoint(); 
                int l =  RG.getRandomPoint();
                while (k == l){
                    l =  RG.getRandomPoint();
                }
                if(k > l){
                    swap(k,l);
                } 

                Person p1 = p.first;
                Person p2 = p.second;

                
                
                Person child1 = Pointcross(p1,p2,n,k,distance);
                Person child2 = Pointcross(p2,p1,n,k,distance);
                

                /*
                Person child1 = twoPointcross(p1,p2,n,k,l,distance);
                Person child2 = twoPointcross(p2,p1,n,k,l,distance);
                */
                child1.age = RG.getRandomAge(); 
                child2.age = RG.getRandomAge(); 

                children.push_back(child1);
                children.push_back(child2);
            }
        }

        for(auto c : children){
            generation.push_back(c);
        }


        //Mutacja
        mutation(generation, mutationRate, n, inversions,distance,RG); //O(G)
              

        sort(generation.begin(),generation.end(),compareByFenotype); //O(GlogG)
        generation.erase(generation.begin()+generation_size,generation.end()); //O(G)
        
        
        
        for(auto p : generation){
            RandomLocalSearch(p.genotype,n,inversions,distance,p.age,RG);
        }
        
        int current_best = generation[0].fenotype;

        it++;
       
        

        evaluateGeneration(generation, distance, n, bestPerm, best); //O(G) 
        
        
        
 }

    perm = bestPerm;
}
