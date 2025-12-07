#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <mysql/mysql.h>
#include <string>
#include <json/json.h>
#include <vector>

class DBManager {
private:
    MYSQL* connection;
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    int port;
    
    bool connect();
    
public:
    DBManager(const std::string& host = "localhost",
              const std::string& user = "crime_user",
              const std::string& password = "CrimePass123!",
              const std::string& database = "crime_db",
              int port = 3306);
    
    ~DBManager();
    
    bool isConnected() const;
    
    // Criminal operations
    Json::Value getCriminalByCNIC(const std::string& cnic);
    Json::Value getCriminalById(int id);
    Json::Value searchCriminals(const std::string& query);
    bool addCriminal(const Json::Value& criminalData);
    bool updateCriminal(int id, const Json::Value& criminalData);
    bool deleteCriminal(int id);
    Json::Value getAllCriminals(int limit = 100);
    
    // Report operations
    Json::Value getPendingReports();
    bool addReport(const Json::Value& reportData);
    bool markReportProcessed(int reportId);
    
    // Statistics
    Json::Value getSystemStats();
    
    // Backup/restore
    bool backupDatabase(const std::string& filepath);
    bool restoreDatabase(const std::string& filepath);
};

#endif
