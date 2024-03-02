#include "GeneticAlgorithm.hpp"
#include <thread>
#include "PermutationAlgorithms.hpp"





void data_exchange(std::vector<Person>& generation1,std::vector<Person>& generation2,std::vector<Person>& generation3,std::vector<Person>& generation4,std::vector<Person>& generation5,int iland_size,int n,RandomGenerator RG){

    for(int i = 0 ; i < n*n ; i++){
        int person1 = RG.getRandomIlPerson();
        int person2 = RG.getRandomIlPerson();
        int iland1 = RG.getRandomIland();
        int iland2 = RG.getRandomIland();
        
        if (iland1 == 0){
            if(iland2 == 1){
                std::swap(generation1[person1],generation2[person2]);
            }
            if(iland2 == 2){
                std::swap(generation1[person1],generation3[person2]);
            }
            if(iland2 == 3){
                std::swap(generation1[person1],generation4[person2]);
            }
            if(iland2 == 4){
                std::swap(generation1[person1],generation5[person2]);
            }
        }
        
        if (iland1 == 1){
            if(iland2 == 0){
                std::swap(generation2[person1],generation1[person2]);
            }
            if(iland2 == 2){
                std::swap(generation2[person1],generation3[person2]);
            }
            if(iland2 == 3){
                std::swap(generation2[person1],generation4[person2]);
            }
            if(iland2 == 4){
                std::swap(generation2[person1],generation5[person2]);
            }
           
           
        }
        
        if (iland1 == 2){
            if(iland2 == 0){
                std::swap(generation3[person1],generation1[person2]);
            }
            if(iland2 == 1){
                std::swap(generation3[person1],generation2[person2]);
            }
            if(iland2 == 3){
                std::swap(generation3[person1],generation4[person2]);
            }
            if(iland2 == 4){
                std::swap(generation3[person1],generation5[person2]);
            }
            
           
        }

        if (iland1 == 3){
            if(iland2 == 0){
                std::swap(generation4[person1],generation1[person2]);
            }
            if(iland2 == 1){
                std::swap(generation4[person1],generation2[person2]);
            }
            if(iland2 == 2){
                std::swap(generation4[person1],generation3[person2]);
            }
            if(iland2 == 4){
                std::swap(generation4[person1],generation5[person2]);
            }
            
            
        }

        if (iland1 == 4){
            if(iland2 == 0){
                std::swap(generation4[person1],generation1[person2]);
            }
            if(iland2 == 1){
                std::swap(generation4[person1],generation2[person2]);
            }
            if(iland2 == 2){
                std::swap(generation4[person1],generation3[person2]);
            }
            if(iland2 == 4){
                std::swap(generation4[person1],generation5[person2]);
            }     
        }

    }
}


void ilands(std::vector<int>& perm, const std::vector<int> distance, int n,std::vector<Person> generation,int generation_size,int parrents_gen_size ,double mutationRate, double crossoverRate,int itrerations,MST mst,RandomGenerator RG){
   //PRINT avaliable threads
   
    const int numIslands = 5;
    int exchange_it = 100;
    int best_weight = 100000000;

    //Prepare generations for islands
    int iland_size = generation_size/numIslands;
    int iland_parr = parrents_gen_size/numIslands;

    RG.setIlandSize(iland_size);

    std::vector<Person> generation1 = std::vector<Person>(generation.begin(),generation.begin()+iland_size);
    std::vector<Person> generation2 = std::vector<Person>(generation.begin()+iland_size,generation.begin()+2*iland_size);
    std::vector<Person> generation3 = std::vector<Person>(generation.begin()+2*iland_size,generation.begin()+3*iland_size);
    std::vector<Person> generation4 = std::vector<Person>(generation.begin()+3*iland_size,generation.begin()+4*iland_size);
    std::vector<Person> generation5 = std::vector<Person>(generation.begin()+4*iland_size,generation.end());

    //Przygotowanie permutacji
    std::vector<int> perm1;
    std::vector<int> perm2;
    std::vector<int> perm3;
    std::vector<int> perm4;
    std::vector<int> perm5;

    int it = 0;
    while(it<itrerations){
         cout << it << " " << endl;

        std::thread threads[numIslands];
        threads[0] = std::thread(geneticAlgorithm,std::ref(perm1),distance,n,std::ref(generation1),iland_size,iland_parr,mutationRate,crossoverRate,exchange_it,mst,RG);
        threads[1] = std::thread(geneticAlgorithm,std::ref(perm2),distance,n,std::ref(generation2),iland_size,iland_parr,mutationRate,crossoverRate,exchange_it,mst,RG);
        threads[2] = std::thread(geneticAlgorithm,std::ref(perm3),distance,n,std::ref(generation3),iland_size,iland_parr,mutationRate,crossoverRate,exchange_it,mst,RG);
        threads[3] = std::thread(geneticAlgorithm,std::ref(perm4),distance,n,std::ref(generation4),iland_size,iland_parr,mutationRate,crossoverRate,exchange_it,mst,RG);
        threads[4] = std::thread(geneticAlgorithm,std::ref(perm4),distance,n,std::ref(generation5),iland_size,iland_parr,mutationRate,crossoverRate,exchange_it,mst,RG);


        threads[0].join();
        threads[1].join();
        threads[2].join();
        threads[3].join();
        threads[4].join();

        
        //ocena permutacji
        int weight1 = calcWeight(perm1, distance, n);
        int weight2 = calcWeight(perm2, distance, n);
        int weight3 = calcWeight(perm3, distance, n);
        int weight4 = calcWeight(perm4, distance, n);
        int weight5 = calcWeight(perm4, distance, n);

       // int current_best_weight = std::min(std::min(weight1,weight2),std::min(weight3,weight4));
        int current_best_weight = std::min(std::min(std::min(weight1,weight2),std::min(weight2,weight3)),std::min(weight4,weight5));
        if (current_best_weight < best_weight) {
            it = 0;
            best_weight = current_best_weight;
            if(current_best_weight == weight1){
                perm = perm1;
            }
            if(current_best_weight == weight2){
                perm = perm2;
            }
            if(current_best_weight == weight3){
                perm = perm3;
            }
            if(current_best_weight == weight4){
                perm = perm4;
            }
            if(current_best_weight == weight5){
                perm = perm5;
            }
            
        }
        it++;

        //Wymiana informacji miedzy wyspami
        data_exchange(generation1,generation2,generation3,generation4,generation5,iland_size,n,RG);
    
    }

}
