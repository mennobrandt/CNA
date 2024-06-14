#include <iostream>
#include <string>
#include <vector>
//#include <sstream>
#include "Network.h"

/*
    X
    Y
    Z
    LINKSTATE
    X-Z 1 X,Y
    X-Y 5 
    Y-Z 3 X,Z
    UPDATE
    X-Z -1 X,Y
    Y-Z 9 Y,Z
    END
*/

std::vector<std::string> readInput() {
    std::vector<std::string> input;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line == "END") {
            input.push_back(line);
            break;
        }
        input.push_back(line);
    }

    return input;
}

int main() {
    std::vector<std::string> input = readInput();

    Network network;
    network.processIn(input);

    return 0;
}