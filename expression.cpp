#include "expression.h"

#include <string>
#include <vector>

#include "options.h"
#include "utils.h"
#include "stringutils.h"
#include "operation.h"
#include "scope.h"
#include "value.h"
#include "userfunction.h"
#include "systemfunction.h"
#include "logger.h"
#include "system.h"

/**
 * https://philcurnow.wordpress.com/2015/01/24/conversion-of-expressions-from-infix-to-postfix-notation-in-c-part-2-unary-operators/
 * https://www.wikizero.pro/index.php?q=aHR0cHM6Ly9lbi53aWtpcGVkaWEub3JnL3dpa2kvU2h1bnRpbmcteWFyZF9hbGdvcml0aG0
 * https://www.wikizero.pro/index.php?q=aHR0cHM6Ly9lbi53aWtpcGVkaWEub3JnL3dpa2kvUmV2ZXJzZV9Qb2xpc2hfbm90YXRpb24
 * */

Expression::Expression(Scope* scope, std::string content) {
    this->mScope = scope;
    this->mContent = content;

    this->mOutputStack = new std::vector<std::string>;
    this->mOperatorStack = new std::vector<std::string>;
}

Expression::~Expression() {

}

int Expression::getOperatorAssociative(char& c) {
    switch (c) {
        case '-': return 1;
        case '+': return 1;
        case '%': return 1;
        case '/': return 1;
        case '*': return 1;
        case '^': return -1;
        case ',': return 1;
        default: return 1; // for functions
    }
}

int Expression::getOperatorAssociative(std::string& s) {
    return Expression::getOperatorAssociative(s.at(0));
}

int Expression::getOperatorPrecedence(char& c) {
    switch (c) {
        case '-': return 2;
        case '+': return 2;
        case '%': return 2;
        case '/': return 3;
        case '*': return 3;
        case '^': return 4;
        case ',': return 1;

        default: return 5; // means operator is a function
    }
}

int Expression::getOperatorPrecedence(std::string& s) {
    return Expression::getOperatorPrecedence(s.at(0));
}

bool Expression::isOperator(char& c) {
    switch (c) {
        case '-': return true;
        case '+': return true;
        case '%': return true;
        case '/': return true;
        case '*': return true;
        case '^': return true;
        case ',': return true;

        default: return false;
    }
}

bool Expression::isOperator(std::string& s) {
    return Expression::isOperator(s.at(0));
}

// Shunting-yard algorithm
void Expression::runOnToken(std::string& token) {
    if (Expression::isOperator(token)) {
        while(!this->mOperatorStack->empty() &&
                this->mOperatorStack->at(0) != "(" &&
                (
                    !Expression::isOperator(this->mOperatorStack->at(0)) ||
                            Expression::getOperatorPrecedence(this->mOperatorStack->at(0)) > Expression::getOperatorPrecedence(token) ||
                    (Expression::getOperatorPrecedence(this->mOperatorStack->at(0)) == Expression::getOperatorPrecedence(token) && Expression::getOperatorAssociative(token) > 0)
                )
            ) {

            this->mOutputStack->push_back(this->mOperatorStack->at(0));
            this->mOperatorStack->erase(this->mOperatorStack->begin());
        }

        this->mOperatorStack->insert(this->mOperatorStack->begin(), token);
    }
    else if (this->mScope->hasFunction(token)) {
        this->mOperatorStack->insert(this->mOperatorStack->begin(), token);
    }
    else if (token == "(") {
        this->mOperatorStack->insert(this->mOperatorStack->begin(), token);
        // this->mOutputStack->push_back(")"); // TODO
    }
    else if (token == ")") {
        while(!this->mOperatorStack->empty() && this->mOperatorStack->at(0) != "(") {
            this->mOutputStack->push_back(this->mOperatorStack->at(0));
            this->mOperatorStack->erase(this->mOperatorStack->begin());
        }
        this->mOperatorStack->erase(this->mOperatorStack->begin()); // TODO
    }
    else {
        this->mOutputStack->push_back(token);
    }
}

Value* Expression::run() {
    std::string token = "";

    // Tokenize algorithm
    for (char c : this->mContent) {
        if (c == ' ') {
            if (!token.empty()) {
                this->runOnToken(token);
                token = "";
            }
        }
        else if (c == ',') { // TODO
            if (!token.empty()) {
                this->runOnToken(token);
                token = "";
            }

            token = ",";
            this->runOnToken(token);
            token = "";
        }
        else if (isalnum(c) || c == '.' || c == '~' || c == '_')  { // TODO
            token += c;
        }
        else if (isOperator(c)) {
            if (!token.empty()) {
                this->runOnToken(token);
                token = "";
            }
            token = c;
            this->runOnToken(token);
            token = "";
        }
        else if (c == '(' || c == ')') {
            if (!token.empty()) {
                this->runOnToken(token);
            }

            token = c;
            this->runOnToken(token);
            token = "";
        }
    }

    if (!token.empty()) {
        this->runOnToken(token);
        token = "";
    }

    while(!this->mOperatorStack->empty()) {
        this->mOutputStack->push_back(this->mOperatorStack->at(0));
        this->mOperatorStack->erase(this->mOperatorStack->begin());
    }

    delete this->mOperatorStack;

    // Reverse Polish Notation
    auto stack = new std::vector<Value*>;
    for(std::string s : *this->mOutputStack) {
        if (isOperator(s)) {
            auto value2 = stack->at(stack->size() - 1);
            stack->erase(stack->begin() + stack->size() - 1);

            auto value1 = stack->at(stack->size() - 1);
            stack->erase(stack->begin() + stack->size() - 1);

            Operation* operation = new Operation(this->mScope, value1, value2, s);
            stack->push_back(operation->run());
            delete operation;
        }
        else if (this->mScope->hasFunction(s)) {
            auto function = this->mScope->getFunction(s);
            auto parameters = stack->at(stack->size() - 1)->getAsValueList();

            auto result = function->run(parameters);

            stack->erase(stack->begin() + stack->size() - 1);
            stack->push_back(result);
        }
        else {
            stack->push_back(this->mScope->parseValue(s));
        }
    }

    delete this->mOutputStack;

    if (stack->empty()) {
        return new Value();
    }

    auto resultValue = stack->at(0);
    delete stack;

    return resultValue;
}