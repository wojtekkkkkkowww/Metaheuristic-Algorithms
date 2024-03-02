#pragma once
#include <random>

class RandomGenerator {
    std::mt19937 gen;
    int n;
    int k;
    int generation_size;
    int iland_size;
    int parrents_gen_size;
    int inversions;

    std::uniform_int_distribution<> uniform_inv;
    std::uniform_int_distribution<> uniform_gen;
    std::uniform_int_distribution<> uniform_iland;
    std::uniform_int_distribution<> uniofrm_il;
    std::uniform_int_distribution<> uniform_n;
    std::uniform_real_distribution<> uniform_real;
    std::uniform_int_distribution<> uniform_k;
    std::uniform_int_distribution<> uniform_age;

    public:
        RandomGenerator(int n, int generation_size,int inversions){
            this -> n = n;
            this -> generation_size = generation_size;
            this -> parrents_gen_size = parrents_gen_size;
            this -> inversions = inversions;
            

            std::random_device rd;
            gen = std::mt19937(rd());


            uniform_inv = std::uniform_int_distribution<>(0, inversions-1);
            uniform_gen = std::uniform_int_distribution<>(0, generation_size-1);
            uniform_n = std::uniform_int_distribution<>(0, n-1);
            uniform_real = std::uniform_real_distribution<>(0, 1);
            uniform_age = std::uniform_int_distribution<>(0, 50);
            uniofrm_il = std::uniform_int_distribution<>(0, 3);
        }
        std::mt19937  getGen(){ return gen;}

        int getRandomInv()    { return uniform_inv(gen);}
        int getRandomPerson() { return uniform_gen(gen);}
        int getRandomIlPerson()  { return uniform_iland(gen);}
        int getRandomIland()  { return uniofrm_il(gen);}
        int getRandomPoint()  { return uniform_n(gen);}
        double getReal()      { return uniform_real(gen);}
        void setK(int k){ 
            this -> k = k;
            uniform_k = std::uniform_int_distribution<>(0, n-k);
        }
        void setIlandSize(int iland_size){
            this -> iland_size = iland_size;
            uniform_iland = std::uniform_int_distribution<>(0, iland_size-1);
        }

        int getRandomK()      { return uniform_k(gen);}
        int getRandomAge()    { return uniform_age(gen);}
};
