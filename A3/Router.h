#ifndef ROUTER_H
#define ROUTER_H

#include <string>
//#include <unordered_map>
#include <map>
#include <vector>
#include <iostream>
#include <queue>
#include <limits>

class Router {
    private:
        //
    public:
        // Router name is a letter. But I'll store as a string regardless. 
        std::string name;
        // Neighbour table
        std::map<std::string, int> nTable; 
        // Link state database 
        std::map<std::string, int> lsdb;
        // Routing Table 
        std::map<std::string, std::pair<std::string, int>> routingTable;

        //Router(); 
        Router(std::string name);
        // Add a new neighbour for the node
        void addNeighbour(const std::string& neighbour, int cost);
        // Remove a neighbour
        void rmNeighbour(const std::string& neighbour);
        // Calculate the routing table, using Dijkstra
        void calcRTable(const std::map<std::string, Router*>& routers);
        // Print neighbour table
        void printNTable() const;
        // Print routing table (that was previously generated with calcRTable())
        void printRTable() const;
};

#endif