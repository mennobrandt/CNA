#include "Router.h"

Router::Router(std::string name) : name(name) {}

void Router::addNeighbour(const std::string& neighbour, int cost) {
    nTable[neighbour] = cost;
}

// Remove neighbour
void Router::rmNeighbour(const std::string& neighbour) {
    nTable.erase(neighbour);
}

// ADAPTED METHOD from https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/ 
// and also adapted from course/textbook pseudocode.
void Router::calcRTable(const std::map<std::string, Router*>& routers) {
    std::map<std::string, int> distances;
    std::map<std::string, std::string> previousRouters;
    for(const auto& router : routers) {
        distances[router.first] = std::numeric_limits<int>::max();
    }
    distances[name] = 0;

    
    auto compare = [&distances](const std::string& a, const std::string& b) {
        return distances[a] > distances[b];
    };
    std::priority_queue<std::string, std::vector<std::string>, decltype(compare)> pq(compare);
    pq.push(name);

    while(!pq.empty()) {
        std::string currentRouter = pq.top();
        pq.pop();
        for(const auto& neighbour : routers.at(currentRouter)->nTable) {
            int newDist = distances[currentRouter] + neighbour.second;
            if(newDist < distances[neighbour.first]) {
                distances[neighbour.first] = newDist;
                previousRouters[neighbour.first] = currentRouter;
                pq.push(neighbour.first);
            }
        }
    }

    routingTable.clear();
    for(const auto& router : routers) {
        if(router.first != name && distances[router.first] != std::numeric_limits<int>::max()) {
            std::string nextHop = router.first;
            while(previousRouters[nextHop] != name) {
                nextHop = previousRouters[nextHop];
            }
            routingTable[router.first] = {nextHop, distances[router.first]};
        }
    }
}

// Format the neighbour table and print it correctly
void Router::printNTable() const {
    std::cout << name << " Neighbour Table:\n";
    std::map<std::string, int> sortedNTable(nTable.begin(), nTable.end());
    for(const auto& entry : sortedNTable) {
        std::cout << entry.first << "|" << entry.second << "\n";
    }
    std::cout << "\n";
}

// Format the routing table and print it correctly. It's already been found. 
void Router::printRTable() const {
    std::cout << name << " Routing Table:\n";
    std::map<std::string, std::pair<std::string, int>> sortedRTable(routingTable.begin(), routingTable.end());
    for(const auto& entry : sortedRTable) {
        std::cout << entry.first << "|" << entry.second.first << "|" << entry.second.second << "\n";
    }
    std::cout << "\n";
}