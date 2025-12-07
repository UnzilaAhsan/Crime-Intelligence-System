#include "Stack.h"
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

void ActionStack::pushAction(const string& actionType, 
                            const string& tableName, 
                            int recordId,
                            const Json::Value& oldData,
                            const Json::Value& newData) {
    
    Action action;
    action.actionType = actionType;
    action.tableName = tableName;
    action.recordId = recordId;
    action.oldData = oldData;
    action.newData = newData;
    
    // Get current timestamp
    auto now = chrono::system_clock::now();
    time_t time = chrono::system_clock::to_time_t(now);
    action.timestamp = ctime(&time);
    
    undoStack.push(action);
    cout << "Action pushed to stack: " << actionType 
         << " on " << tableName << " (ID: " << recordId << ")" << endl;
}

bool ActionStack::undoLastAction() {
    if (undoStack.empty()) {
        cout << "No actions to undo!" << endl;
        return false;
    }
    
    Action lastAction = undoStack.top();
    undoStack.pop();
    
    // Here you would implement the actual undo logic
    cout << "Undoing action: " << lastAction.actionType 
         << " on " << lastAction.tableName 
         << " (ID: " << lastAction.recordId << ")" << endl;
    
    return true;
}

bool ActionStack::isEmpty() const {
    return undoStack.empty();
}

size_t ActionStack::size() const {
    return undoStack.size();
}

void ActionStack::clear() {
    while (!undoStack.empty()) {
        undoStack.pop();
    }
}
