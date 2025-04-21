//
// Created by home on 20/04/25.
//

#ifndef STACK_MANAGER_H
#define STACK_MANAGER_H

#include <stack>
#include <utility>

class StackManager {
private:
    std::stack<int> stack;
public:
    void push(int value);
    int pop();
    [[nodiscard]] bool isEmpty() const;
    int top();
    void clear();
};

#endif //STACK_MANAGER_H
