#ifndef NETWORK_H
#define NETWORK_H

#include "Router.h"

#include <sstream>
#include <string>
#include <vector> 
#include <iostream>
//#include <unordered_map>
#include <map>


class Network {
    public:
        // Map of routers, where the key is the name
        std::map<std::string, Router*> routers;
        // Link state database
        std::map<std::pair<std::string, std::string>, int> lsdb;
        void updateTables();
        // Update link between routers
        void updateLink(const std::string& r1, const std::string& r2, int cost);
        // Add new router to topology
        void addRouter(const std::string& name);
        // Add new link 
        void addLink(const std::string& r1, const std::string& r2, int cost);
        // Remove an existing link
        void rmLink(const std::string& r1, const std::string& r2);
        // Process the user input and call necessary functions
        void processIn(const std::vector<std::string>& inputLines);
        // Output the link state database
        void printLSDB(const std::string& routerName) const;
        // Basically a helper function, that calls calcRTable() from Router for every router,
        void calculateRoutingTables();
        // Calls printNTable() and printRTable()
        void printOut(const std::vector<std::string>& routerNames) const;
};

#endif