#include <iostream>
#include <vector>
#include <string>
#include <cctype>

#include "ArrayStack.h"

using namespace std;

struct Token {
    string value;
};

vector<Token> tokenize(const string& line) {
    vector<Token> tokens;
    int i = 0;
    while (i < (int)line.size()) {
        if (isspace(line[i])) {
            i++;
            continue;
        }
        if (isdigit(line[i])) {
            string num;
            while (i < (int)line.size() && isdigit(line[i])) {
                num += line[i++];
            }
            tokens.push_back({num});
        } else if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/') {
            tokens.push_back({string(1, line[i++])});
        } else if (line[i] == '(' || line[i] == ')') {
            tokens.push_back({string(1, line[i++])});
        } else {
            i++;
        }
    }
    return tokens;
}

bool isOperator(const string& s) {
    return s == "+" || s == "-" || s == "*" || s == "/";
}

int precedence(const string& op) {
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

bool isValidPostfix(const vector<Token>& tokens) {
    if (tokens.empty()) return false;
    int depth = 0;
    for (const auto& t : tokens) {
        if (t.value == "(" || t.value == ")") return false;
        if (isOperator(t.value)) {
            if (depth < 2) return false;
            depth--;
        } else {
            depth++;
        }
    }
    return depth == 1;
}

bool isValidInfix(const vector<Token>& tokens) {
    if (tokens.empty()) return false;

    int parenDepth = 0;
    string prev = "start";

    for (const auto& t : tokens) {
        const string& v = t.value;

        if (v == "(") {
            if (prev == "number" || prev == "rparen") return false;
            parenDepth++;
            prev = "lparen";
        } else if (v == ")") {
            if (prev == "operator" || prev == "lparen" || prev == "start") return false;
            parenDepth--;
            if (parenDepth < 0) return false;
            prev = "rparen";
        } else if (isOperator(v)) {
            if (prev == "operator" || prev == "lparen" || prev == "start") return false;
            prev = "operator";
        } else {
            if (prev == "number" || prev == "rparen") return false;
            prev = "number";
        }
    }

    if (parenDepth != 0) return false;
    if (prev != "number" && prev != "rparen") return false;
    return true;
}

vector<Token> infixToPostfix(const vector<Token>& tokens) {
    vector<Token> output;
    ArrayStack<Token> opStack;

    for (const auto& t : tokens) {
        const string& v = t.value;

        if (!isOperator(v) && v != "(" && v != ")") {
            output.push_back(t);
        } else if (v == "(") {
            opStack.push(t);
        } else if (v == ")") {
            while (!opStack.empty() && opStack.top().value != "(") {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.pop();
        } else {
            while (!opStack.empty() &&
                   isOperator(opStack.top().value) &&
                   precedence(opStack.top().value) >= precedence(v)) {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(t);
        }
    }

    while (!opStack.empty()) {
        output.push_back(opStack.top());
        opStack.pop();
    }

    return output;
}

double evalPostfix(const vector<Token>& tokens) {
    ArrayStack<double> stack;
    for (const auto& t : tokens) {
        if (isOperator(t.value)) {
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            if (t.value == "+") stack.push(a + b);
            else if (t.value == "-") stack.push(a - b);
            else if (t.value == "*") stack.push(a * b);
            else if (t.value == "/") stack.push(a / b);
        } else {
            stack.push(stod(t.value));
        }
    }
    return stack.top();
}

int main() {
    string line;
    getline(cin, line);

    vector<Token> tokens = tokenize(line);

    if (isValidPostfix(tokens)) {
        cout << "FORMAT: POSTFIX\n";
        cout << "RESULT: " << evalPostfix(tokens) << "\n";
    }
    else if (isValidInfix(tokens)) {
        vector<Token> postfix = infixToPostfix(tokens);
        cout << "FORMAT: INFIX\n";
        cout << "POSTFIX: ";
        for (int i = 0; i < (int)postfix.size(); i++) {
            if (i > 0) cout << " ";
            cout << postfix[i].value;
        }
        cout << "\n";
        cout << "RESULT: " << evalPostfix(postfix) << "\n";
    }
    else {
        cout << "FORMAT: NEITHER\n";
        cout << "ERROR: invalid expression\n";
    }

    return 0;
}
