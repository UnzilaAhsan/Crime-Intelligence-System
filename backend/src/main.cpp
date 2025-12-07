#include <iostream>
#include <thread>
#include <chrono>
#include "API/Server.h"
#include "BTree/BTree.h"
#include "HashTable/HashTable.h"
#include "Queue/Queue.h"
#include "Stack/Stack.h"
#include "Database/DBManager.h"

using namespace std;

// Global instances
BTree<string>* criminalTree = nullptr;
HashTable<string, string>* fingerprintTable = nullptr;
HashTable<string, string>* weaponTable = nullptr;
CrimeReportQueue* reportQueue = nullptr;
ActionStack* actionStack = nullptr;

void initializeDataStructures() {
    cout << "🔧 Initializing Data Structures..." << endl;
    
    // Initialize B-Tree for criminals (degree 3)
    criminalTree = new BTree<string>(3);
    cout << "  ✅ B-Tree initialized (degree: 3)" << endl;
    
    // Initialize Hash Tables
    fingerprintTable = new HashTable<string, string>(100);
    weaponTable = new HashTable<string, string>(100);
    cout << "  ✅ Hash Tables initialized (capacity: 100 each)" << endl;
    
    // Initialize Queue for reports
    reportQueue = new CrimeReportQueue();
    cout << "  ✅ Crime Report Queue initialized" << endl;
    
    // Initialize Stack for undo actions
    actionStack = new ActionStack();
    cout << "  ✅ Action Stack initialized" << endl;
    
    cout << "🎯 Data Structures ready!" << endl;
}

void cleanup() {
    cout << "\n🧹 Cleaning up resources..." << endl;
    
    if (criminalTree) delete criminalTree;
    if (fingerprintTable) delete fingerprintTable;
    if (weaponTable) delete weaponTable;
    if (reportQueue) delete reportQueue;
    if (actionStack) delete actionStack;
    
    cout << "✅ Cleanup complete!" << endl;
}

void demoDataStructures() {
    cout << "\n📊 Demonstrating Data Structure Operations:" << endl;
    
    // Demo B-Tree
    cout << "\n1. B-Tree Operations:" << endl;
    criminalTree->insert("C-1001");
    criminalTree->insert("C-1002");
    criminalTree->insert("C-1003");
    cout << "   Inserted 3 criminal IDs" << endl;
    
    bool found = criminalTree->search("C-1002");
    cout << "   Search for C-1002: " << (found ? "Found ✅" : "Not found ❌") << endl;
    
    // Demo Hash Table
    cout << "\n2. Hash Table Operations:" << endl;
    fingerprintTable->insert("FP-001", "C-1001");
    fingerprintTable->insert("FP-002", "C-1002");
    cout << "   Added 2 fingerprint mappings" << endl;
    
    string* criminalId = fingerprintTable->get("FP-001");
    cout << "   Lookup FP-001: " << (criminalId ? *criminalId : "Not found") << endl;
    
    // Demo Queue
    cout << "\n3. Queue Operations:" << endl;
    reportQueue->pushReport("FIR-2024-001: Bank Robbery");
    reportQueue->pushReport("FIR-2024-002: Car Theft");
    cout << "   Added 2 crime reports to queue" << endl;
    cout << "   Queue size: " << reportQueue->size() << endl;
    
    // Demo Stack
    cout << "\n4. Stack Operations:" << endl;
    actionStack->pushAction("INSERT", "criminals", 101, {}, {});
    actionStack->pushAction("UPDATE", "criminals", 101, {}, {});
    cout << "   Added 2 admin actions to stack" << endl;
    cout << "   Stack size: " << actionStack->size() << endl;
}

int main() {
    cout << "\n=========================================" << endl;
    cout << "   CRIME INTELLIGENCE SYSTEM BACKEND    " << endl;
    cout << "=========================================" << endl;
    
    // Initialize data structures
    initializeDataStructures();
    
    // Demo the data structures
    demoDataStructures();
    
    // Start HTTP server
    cout << "\n🌐 Starting HTTP API Server..." << endl;
    CrimeServer server(8080);
    
    // Run server in separate thread
    thread server_thread([&server]() {
        server.start();
    });
    
    cout << "\n✅ System is running!" << endl;
    cout << "🔗 API Server: http://localhost:8080" << endl;
    cout << "📋 Available endpoints:" << endl;
    cout << "   GET  /api/criminals" << endl;
    cout << "   GET  /api/criminals/{cnic}" << endl;
    cout << "   POST /api/criminals" << endl;
    cout << "   GET  /api/reports" << endl;
    cout << "   GET  /api/stats" << endl;
    cout << "\nPress Ctrl+C to stop the server..." << endl;
    
    // Keep main thread alive
    server_thread.join();
    
    cleanup();
    return 0;
}
