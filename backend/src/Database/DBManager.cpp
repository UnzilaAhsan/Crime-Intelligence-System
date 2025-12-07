#include "DBManager.h"
#include <iostream>
#include <fstream>

using namespace std;

DBManager::DBManager(const string& h, const string& u, 
                     const string& p, const string& db, int pt) 
    : host(h), user(u), password(p), database(db), port(pt), connection(nullptr) {
    connect();
}

DBManager::~DBManager() {
    if (connection) {
        mysql_close(connection);
    }
}

bool DBManager::connect() {
    connection = mysql_init(nullptr);
    if (!connection) {
        cerr << "MySQL initialization failed!" << endl;
        return false;
    }
    
    if (!mysql_real_connect(connection, host.c_str(), user.c_str(), 
                           password.c_str(), database.c_str(), port, nullptr, 0)) {
        cerr << "Connection failed: " << mysql_error(connection) << endl;
        return false;
    }
    
    cout << "✅ Connected to database: " << database << endl;
    return true;
}

bool DBManager::isConnected() const {
    return connection != nullptr;
}

Json::Value DBManager::getCriminalByCNIC(const string& cnic) {
    Json::Value result;
    
    if (!connection) return result;
    
    string query = "SELECT * FROM criminals WHERE cnic = '" + cnic + "'";
    
    if (mysql_query(connection, query.c_str())) {
        cerr << "Query failed: " << mysql_error(connection) << endl;
        return result;
    }
    
    MYSQL_RES* res = mysql_store_result(connection);
    if (res) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row) {
            MYSQL_FIELD* fields = mysql_fetch_fields(res);
            int num_fields = mysql_num_fields(res);
            
            for(int i = 0; i < num_fields; i++) {
                result[fields[i].name] = row[i] ? row[i] : "";
            }
        }
        mysql_free_result(res);
    }
    
    return result;
}

Json::Value DBManager::getAllCriminals(int limit) {
    Json::Value result(Json::arrayValue);
    
    if (!connection) return result;
    
    string query = "SELECT * FROM criminals ORDER BY criminal_id DESC LIMIT " + to_string(limit);
    
    if (mysql_query(connection, query.c_str())) {
        cerr << "Query failed: " << mysql_error(connection) << endl;
        return result;
    }
    
    MYSQL_RES* res = mysql_store_result(connection);
    if (res) {
        MYSQL_ROW row;
        MYSQL_FIELD* fields = mysql_fetch_fields(res);
        int num_fields = mysql_num_fields(res);
        
        while ((row = mysql_fetch_row(res))) {
            Json::Value criminal;
            for(int i = 0; i < num_fields; i++) {
                criminal[fields[i].name] = row[i] ? row[i] : "";
            }
            result.append(criminal);
        }
        mysql_free_result(res);
    }
    
    return result;
}

Json::Value DBManager::getPendingReports() {
    Json::Value result(Json::arrayValue);
    
    if (!connection) return result;
    
    string query = "SELECT * FROM crime_reports WHERE status = 'Pending' ORDER BY created_at ASC";
    
    if (mysql_query(connection, query.c_str())) {
        cerr << "Query failed: " << mysql_error(connection) << endl;
        return result;
    }
    
    MYSQL_RES* res = mysql_store_result(connection);
    if (res) {
        MYSQL_ROW row;
        MYSQL_FIELD* fields = mysql_fetch_fields(res);
        int num_fields = mysql_num_fields(res);
        
        while ((row = mysql_fetch_row(res))) {
            Json::Value report;
            for(int i = 0; i < num_fields; i++) {
                report[fields[i].name] = row[i] ? row[i] : "";
            }
            result.append(report);
        }
        mysql_free_result(res);
    }
    
    return result;
}

bool DBManager::addCriminal(const Json::Value& criminalData) {
    if (!connection) return false;
    
    string query = "INSERT INTO criminals (cnic, name, age, gender, address, crime_type, status) "
                   "VALUES ('" + criminalData["cnic"].asString() + "', " +
                   "'" + criminalData["name"].asString() + "', " +
                   criminalData["age"].asString() + ", " +
                   "'" + criminalData["gender"].asString() + "', " +
                   "'" + criminalData["address"].asString() + "', " +
                   "'" + criminalData["crime_type"].asString() + "', " +
                   "'" + criminalData["status"].asString() + "')";
    
    if (mysql_query(connection, query.c_str())) {
        cerr << "Insert failed: " << mysql_error(connection) << endl;
        return false;
    }
    
    return true;
}

Json::Value DBManager::getSystemStats() {
    Json::Value stats;
    
    if (!connection) return stats;
    
    // Get counts
    string queries[] = {
        "SELECT COUNT(*) as total FROM criminals",
        "SELECT COUNT(*) as wanted FROM criminals WHERE status = 'Wanted'",
        "SELECT COUNT(*) as pending FROM crime_reports WHERE status = 'Pending'",
        "SELECT COUNT(*) as processed FROM crime_reports WHERE status = 'Resolved'",
        "SELECT COUNT(*) as weapons FROM weapons"
    };
    
    string names[] = {"total_criminals", "wanted_criminals", "pending_reports", 
                     "processed_reports", "total_weapons"};
    
    for (int i = 0; i < 5; i++) {
        if (mysql_query(connection, queries[i].c_str())) {
            cerr << "Query failed: " << mysql_error(connection) << endl;
            continue;
        }
        
        MYSQL_RES* res = mysql_store_result(connection);
        if (res) {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row) {
                stats[names[i]] = atoi(row[0]);
            }
            mysql_free_result(res);
        }
    }
    
    return stats;
}
