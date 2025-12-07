#ifndef STACK_H
#define STACK_H

#include <stack>
#include <string>
#include <json/json.h>

class ActionStack {
private:
    struct Action {
        std::string actionType;  // "INSERT", "UPDATE", "DELETE"
        std::string tableName;
        int recordId;
        Json::Value oldData;
        Json::Value newData;
        std::string timestamp;
    };
    
    std::stack<Action> undoStack;
    
public:
    void pushAction(const std::string& actionType, 
                   const std::string& tableName, 
                   int recordId,
                   const Json::Value& oldData,
                   const Json::Value& newData);
    
    bool undoLastAction();
    bool isEmpty() const;
    size_t size() const;
    void clear();
};

#endif
