#include "PermutationAlgorithms.hpp"
#include <algorithm>
#include <random>

void reverse_section(vector<int>& perm, int i, int j) {
    reverse(perm.begin() + i, perm.begin() + j + 1);
}

int calcWeight(const vector<int>& perm, const vector<int>& distance, int n) {
    int w = 0;
    for (int i = 1; i < n; i++) {
        w += distance[perm[i] * n + perm[i - 1]];
    }
    w += distance[perm[n - 1] * n + perm[0]];
    return w;
}

vector<int> randPerm(int n) {
    vector<int> perm(n);
    mt19937 gen{std::random_device{}()};
    for (int i = 0; i < n; ++i) {
        perm[i] = i;
    }

    for (int i = n - 1; i > 0; --i) {
        uniform_int_distribution<> dist(0, i);
        int j = dist(gen);
        swap(perm[i], perm[j]);
    }

    return perm;
}


int calcInversedWeight(int w, int i, int j, const vector<int>& perm, int n, const vector<int>& d) {
    if (i == 0 && j == n - 1) {
        return w;
    }

    int i_prev = (i == 0) ? n - 1 : i - 1;
    int j_next = (j == n - 1) ? 0 : j + 1;

    int old = d[perm[i_prev] * n + perm[i]] + d[perm[j] * n + perm[j_next]];
    int new_ = d[perm[i_prev] * n + perm[j]] + d[perm[i] * n + perm[j_next]];

    return w - old + new_;
}
