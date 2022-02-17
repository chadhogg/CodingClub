#include <iostream>
#include <cstdlib>

int main (int argc, char* argv[]) {
    srand (time (NULL));
    int N = rand () % 1000000;
    int K = rand () % 100000;
    std::cout << N << " " << K << "\n";
    for (int i = 0; i < K; i++) {
        if (rand () % 2 == 0) {
            int bit = rand () % N;
            std::cout << "F " << bit << "\n";
        }
        else {
            int start = rand () % N;
            int end = rand () % (N - start) + start;
            std::cout << "C " << start << " " << end << "\n";
        }
    }
    return 0;
}