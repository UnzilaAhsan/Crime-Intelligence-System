#include "Queue.h"
#include <iostream>

using namespace std;

void CrimeReportQueue::pushReport(const string& report) {
    {
        lock_guard<mutex> lock(mtx);
        reports.push(report);
    }
    cv.notify_one();
    cout << "Report added to queue: " << report << endl;
}

string CrimeReportQueue::popReport() {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [this] { return !reports.empty(); });
    
    string report = reports.front();
    reports.pop();
    
    cout << "Report processed: " << report << endl;
    return report;
}

bool CrimeReportQueue::isEmpty() const {
    lock_guard<mutex> lock(mtx);
    return reports.empty();
}

size_t CrimeReportQueue::size() const {
    lock_guard<mutex> lock(mtx);
    return reports.size();
}

void CrimeReportQueue::clear() {
    lock_guard<mutex> lock(mtx);
    while (!reports.empty()) {
        reports.pop();
    }
}
