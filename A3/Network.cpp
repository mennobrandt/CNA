#include "Network.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <queue>
#include <sstream>

// Serves as a helper function to call calculateRoutingTables() and update routing tables
void Network::updateTables() {
    calculateRoutingTables();
}

void Network::updateLink(const std::string& r1, const std::string& r2, int cost) {
    //std::cout << "Updating link between " << r1 << " " << r2 << ". costs:" << cost << std::endl;
    if(lsdb.find({r1, r2}) != lsdb.end() || lsdb.find({r2, r1}) != lsdb.end()) {
        rmLink(r1, r2);
        if (cost != -1) {
            addLink(r1, r2, cost);
        }
    }
}

void Network::addRouter(const std::string& name) {
    if(routers.find(name) == routers.end()) {
        routers[name] = new Router(name);
    }
}

// Add link
void Network::addLink(const std::string& r1, const std::string& r2, int cost) {
    addRouter(r1);
    addRouter(r2);

    routers[r1]->addNeighbour(r2, cost);
    routers[r2]->addNeighbour(r1, cost);
    lsdb[{r1, r2}] = cost;
    lsdb[{r2, r1}] = cost;

    updateTables();
}

// Remove link. Of course, it has to exist first. 
void Network::rmLink(const std::string& r1, const std::string& r2) {
    //std::cout << "Rm link:" << r1 << " " << r2 << std::endl;
    if (routers.find(r1) != routers.end() && routers.find(r2) != routers.end()) {
        routers[r1]->rmNeighbour(r2);
        routers[r2]->rmNeighbour(r1);
        lsdb.erase({r1, r2});
        lsdb.erase({r2, r1});

        updateTables();
    }
}

// Process the input. Decide what kind of instruction it is, and act accordingly. 
void Network::processIn(const std::vector<std::string>& inputLines) {
    std::string instruction = "ADD_ROUTER";
    for(const auto& line : inputLines) {
        if(line == "LINKSTATE") {
            instruction = "ADD_LINK";
        } else if(line == "UPDATE") {
            instruction = "UPDATE_LINK";
        } else if(line == "END") {
            break;
        } else if(instruction == "ADD_ROUTER") {
            addRouter(line);
        } else if(instruction == "ADD_LINK" || instruction == "UPDATE_LINK") {
            if(line == "LINKSTATE" || line == "UPDATE") continue;

            std::istringstream iss(line);
            std::string routerPart, costPart, chosenRoutersPart;
            iss >> routerPart >> costPart >> chosenRoutersPart;

            auto dashPos = routerPart.find('-');
            std::string r1 = routerPart.substr(0, dashPos);
            std::string r2 = routerPart.substr(dashPos + 1);

            int cost = std::stoi(costPart);

            if(instruction == "ADD_LINK") {
                addLink(r1, r2, cost);
            } else if(instruction == "UPDATE_LINK") {
                if(cost != -1) {
                    addLink(r1, r2, cost);
                } else {
                    rmLink(r1, r2);
                }
            }

            if(!chosenRoutersPart.empty()) {
                std::vector<std::string> chosenRouters;
                std::istringstream criss(chosenRoutersPart);
                std::string router;
                while(std::getline(criss, router, ',')) {
                    chosenRouters.push_back(router);
                }
                // std::cout << "Printing output for chosen routers: ";
                // for(const auto& r : chosenRouters) {
                //     std::cout << r << " ";
                // }
                // std::cout << std::endl;
                printOut(chosenRouters);
            }
        }
    }
}

// BFS approach using queue 
void Network::printLSDB(const std::string& routerName) const {
    std::set<std::pair<std::string, std::string>> reachableLinks;
    std::set<std::string> visitedRouters;
    std::queue<std::string> queue;
    queue.push(routerName);

    while(!queue.empty()) {
        std::string currentRouter = queue.front();
        queue.pop();
        visitedRouters.insert(currentRouter);

        for(const auto& neighbour : routers.at(currentRouter)->nTable) {
            if(visitedRouters.find(neighbour.first) == visitedRouters.end()) {
                queue.push(neighbour.first);
                reachableLinks.insert({std::min(currentRouter, neighbour.first), std::max(currentRouter, neighbour.first)});
            }
        }
    }

    std::cout << routerName << " LSDB:\n";
    for(const auto& link : reachableLinks) {
        std::cout << link.first << "|" << link.second << "|" << lsdb.at(link) << "\n";
    }
    std::cout << "\n";
}

// Call Router's calcRTable() function with every router.
void Network::calculateRoutingTables() {
    for(const auto& router : routers) {
        router.second->calcRTable(routers);
    }
}

// Call necessary functions to print out Node information. 
void Network::printOut(const std::vector<std::string>& routerNames) const {
    for(const auto& routerName : routerNames) {
        if(routers.find(routerName) != routers.end()) {
            routers.at(routerName)->printNTable();
            printLSDB(routerName);
            routers.at(routerName)->printRTable();
        }
    }
}