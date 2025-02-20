#include <iostream>
#include <cstdlib>

int
main (int argc, char* argv[])
{
    int numItems = atoi (argv[1]);
    int numBoxes = atoi (argv[2]);
    std::cout << numItems << " " << numBoxes << "\n";
    for (int i = 0; i < numItems; ++i) {
        int weight = rand() % 10000 + 1;
        std::cout << weight << " ";
    }
    std::cout << "\n";
    return 0;
}