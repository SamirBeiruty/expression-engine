#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>

#include "ArrayStack.h"

using namespace std;

// Token

struct Token {
    string value;   // number, operator, or parenthesis
};

// Tokenizer

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

// Helpers

bool isOperator(const string& s) {
    return s == "+" || s == "-" || s == "*" || s == "/";
}

int precedence(const string& op) {
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

// Detection

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
    // "start", "number", "operator", "lparen", "rparen"
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
            // number
            if (prev == "number" || prev == "rparen") return false;
            prev = "number";
        }
    }

    if (parenDepth != 0) return false;
    if (prev != "number" && prev != "rparen") return false;
    return true;
}

// Conversion

vector<Token> infixToPostfix(const vector<Token>& tokens) {
    vector<Token> output;
    // TODO
    return output;
}

// Evaluation

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

// Main

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
        for (const auto& t : postfix) {
            cout << t.value << " ";
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
