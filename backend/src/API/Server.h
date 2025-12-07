#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <json/json.h>
#include "../Database/DBManager.h"

class CrimeServer {
private:
    int server_fd;
    int port;
    bool running;
    DBManager* db;
    
    void handleClient(int client_socket);
    std::string processRequest(const std::string& request);
    
    // API Endpoints
    std::string handleGetCriminals();
    std::string handleGetCriminalByCNIC(const std::string& cnic);
    std::string handleAddCriminal(const Json::Value& data);
    std::string handleGetReports();
    std::string handleGetStats();
    
public:
    CrimeServer(int p = 8080);
    ~CrimeServer();
    
    bool start();
    void stop();
};

#endif
