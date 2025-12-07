#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

class CrimeReportQueue {
private:
    std::queue<std::string> reports;
    mutable std::mutex mtx;
    std::condition_variable cv;
    
public:
    void pushReport(const std::string& report);
    std::string popReport();
    bool isEmpty() const;
    size_t size() const;
    void clear();
    
    // Database operations
    void loadPendingReports();
    void saveReportToDB(const std::string& report);
};

#endif
