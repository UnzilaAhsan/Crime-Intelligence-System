#include "Server.h"
#include <cstring>
#include <algorithm>

using namespace std;

CrimeServer::CrimeServer(int p) : port(p), running(false), db(nullptr) {
    db = new DBManager();
}

CrimeServer::~CrimeServer() {
    stop();
    if (db) delete db;
}

bool CrimeServer::start() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        cerr << "Socket creation failed" << endl;
        return false;
    }
    
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        cerr << "Setsockopt failed" << endl;
        return false;
    }
    
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Bind failed" << endl;
        return false;
    }
    
    if (listen(server_fd, 10) < 0) {
        cerr << "Listen failed" << endl;
        return false;
    }
    
    running = true;
    cout << "🚀 Crime Intelligence Server started on port " << port << endl;
    cout << "📡 API Endpoints:" << endl;
    cout << "  GET  /api/criminals" << endl;
    cout << "  GET  /api/criminals/:cnic" << endl;
    cout << "  POST /api/criminals" << endl;
    cout << "  GET  /api/reports" << endl;
    cout << "  GET  /api/stats" << endl;
    
    // Accept connections
    while (running) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            if (running) cerr << "Accept failed" << endl;
            continue;
        }
        
        thread client_thread(&CrimeServer::handleClient, this, client_socket);
        client_thread.detach();
    }
    
    return true;
}

void CrimeServer::stop() {
    running = false;
    close(server_fd);
}

void CrimeServer::handleClient(int client_socket) {
    char buffer[4096] = {0};
    read(client_socket, buffer, 4096);
    
    string request(buffer);
    string response = processRequest(request);
    
    // Add CORS headers
    string full_response = "HTTP/1.1 200 OK\r\n";
    full_response += "Content-Type: application/json\r\n";
    full_response += "Access-Control-Allow-Origin: *\r\n";
    full_response += "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
    full_response += "Access-Control-Allow-Headers: Content-Type\r\n";
    full_response += "Content-Length: " + to_string(response.length()) + "\r\n";
    full_response += "\r\n";
    full_response += response;
    
    send(client_socket, full_response.c_str(), full_response.length(), 0);
    close(client_socket);
}

string CrimeServer::processRequest(const string& request) {
    istringstream iss(request);
    string method, path, protocol;
    iss >> method >> path >> protocol;
    
    Json::Value response;
    response["success"] = false;
    response["message"] = "Unknown endpoint";
    
    // Parse query parameters
    size_t query_pos = path.find('?');
    string endpoint = (query_pos != string::npos) ? path.substr(0, query_pos) : path;
    
    if (endpoint == "/api/criminals" && method == "GET") {
        return handleGetCriminals();
    }
    else if (endpoint.find("/api/criminals/") == 0 && method == "GET") {
        string cnic = endpoint.substr(15); // Remove "/api/criminals/"
        return handleGetCriminalByCNIC(cnic);
    }
    else if (endpoint == "/api/reports" && method == "GET") {
        return handleGetReports();
    }
    else if (endpoint == "/api/stats" && method == "GET") {
        return handleGetStats();
    }
    else if (endpoint == "/api/criminals" && method == "POST") {
        // Extract JSON from request body
        size_t body_start = request.find("\r\n\r\n");
        if (body_start != string::npos) {
            string body = request.substr(body_start + 4);
            Json::Reader reader;
            Json::Value data;
            if (reader.parse(body, data)) {
                return handleAddCriminal(data);
            }
        }
    }
    else if (method == "OPTIONS") {
        // Handle preflight requests
        response["success"] = true;
        response["message"] = "CORS preflight";
    }
    
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, response);
}

string CrimeServer::handleGetCriminals() {
    Json::Value response;
    if (db && db->isConnected()) {
        Json::Value criminals = db->getAllCriminals(100);
        response["success"] = true;
        response["criminals"] = criminals;
    } else {
        response["success"] = false;
        response["message"] = "Database not connected";
    }
    
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, response);
}

string CrimeServer::handleGetCriminalByCNIC(const string& cnic) {
    Json::Value response;
    if (db && db->isConnected()) {
        Json::Value criminal = db->getCriminalByCNIC(cnic);
        if (!criminal.empty()) {
            response["success"] = true;
            response["criminal"] = criminal;
        } else {
            response["success"] = false;
            response["message"] = "Criminal not found";
        }
    } else {
        response["success"] = false;
        response["message"] = "Database not connected";
    }
    
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, response);
}

string CrimeServer::handleAddCriminal(const Json::Value& data) {
    Json::Value response;
    if (db && db->isConnected()) {
        if (db->addCriminal(data)) {
            response["success"] = true;
            response["message"] = "Criminal added successfully";
        } else {
            response["success"] = false;
            response["message"] = "Failed to add criminal";
        }
    } else {
        response["success"] = false;
        response["message"] = "Database not connected";
    }
    
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, response);
}

string CrimeServer::handleGetReports() {
    Json::Value response;
    if (db && db->isConnected()) {
        Json::Value reports = db->getPendingReports();
        response["success"] = true;
        response["reports"] = reports;
    } else {
        response["success"] = false;
        response["message"] = "Database not connected";
    }
    
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, response);
}

string CrimeServer::handleGetStats() {
    Json::Value response;
    if (db && db->isConnected()) {
        Json::Value stats = db->getSystemStats();
        response["success"] = true;
        response["stats"] = stats;
    } else {
        response["success"] = false;
        response["message"] = "Database not connected";
    }
    
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, response);
}
