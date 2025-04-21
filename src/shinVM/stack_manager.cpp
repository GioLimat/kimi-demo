//
// Created by home on 20/04/25.
//

#include "stack_manager.h"

#include <stdexcept>

void StackManager::push(const int value) {
    stack.push(value);
}

int StackManager::pop() {
    const auto top = stack.top();
    if (!isEmpty()) {
        stack.pop();
    }
    return top;
}


void StackManager::clear() {
    stack = std::stack<int>();
}

int StackManager::top() {
    if (!isEmpty()) {
        return stack.top();
    }
    return INT32_MIN;
}

bool StackManager::isEmpty() const {
    return stack.empty();
}
