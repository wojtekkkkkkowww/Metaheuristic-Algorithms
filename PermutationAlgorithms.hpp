#ifndef PERMUTATION_ALGORITHMS_H
#define PERMUTATION_ALGORITHMS_H

#include <vector>
using namespace std;

void reverse_section(vector<int>& perm, int i, int j);
int calcWeight(const vector<int>& perm, const vector<int>& distance, int n);
int calcInversedWeight(int w, int i, int j, const vector<int>& perm, int n, const vector<int>& d);
vector<int> randPerm(int n);

#endif
