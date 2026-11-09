
#include "ExpressionConverter.h"

#include <cctype>
#include <stdexcept>
#include <stack>

bool ExpressionConverter::isOpeningBracket(char ch) const {
    return ch == '(' || ch == '[' || ch == '{';
}

bool ExpressionConverter::isClosingBracket(char ch) const {
    return ch == ')' || ch == ']' || ch == '}';
}

bool ExpressionConverter::bracketsMatch(
    char opening, char closing
) const {
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}');
}

bool ExpressionConverter::isOperator(char ch) const {
    return ch == '+' || ch == '-' ||
           ch == '*' || ch == '/';
}

int ExpressionConverter::precedence(char op) const {
    if (op == '*' || op == '/') {
        return 2;
    }

    if (op == '+' || op == '-') {
        return 1;
    }

    return 0;
}

std::vector<std::string>
ExpressionConverter::infixToPostfix(
    const std::string& expression
) {
    std::vector<std::string> postfix;
    std::stack<char> operators;

    // True means an operand or opening bracket is expected.
    bool expectOperand = true;
    bool foundToken = false;

    for (std::size_t i = 0; i < expression.size();) {
        char ch = expression[i];

        // Ignore spaces, tabs, and other whitespace.
        if (std::isspace(static_cast<unsigned char>(ch))) {
            ++i;
            continue;
        }

        foundToken = true;

        // Read a complete nonnegative integer.
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            if (!expectOperand) {
                throw std::runtime_error(
                    "Missing operator or implicit multiplication."
                );
            }

            std::size_t start = i;

            while (i < expression.size() &&
                   std::isdigit(
                       static_cast<unsigned char>(expression[i])
                   )) {
                ++i;
            }

            postfix.push_back(
                expression.substr(start, i - start)
            );

            expectOperand = false;
            continue;
        }

        // Opening grouping symbols.
        if (isOpeningBracket(ch)) {
            if (!expectOperand) {
                throw std::runtime_error(
                    "Missing operator before opening bracket."
                );
            }

            operators.push(ch);
            expectOperand = true;
            ++i;
            continue;
        }

        // Closing grouping symbols.
        if (isClosingBracket(ch)) {
            if (expectOperand) {
                throw std::runtime_error(
                    "Empty group or missing operand before closing bracket."
                );
            }

            bool matched = false;

            while (!operators.empty()) {
                char top = operators.top();

                if (isOpeningBracket(top)) {
                    if (!bracketsMatch(top, ch)) {
                        throw std::runtime_error(
                            "Mismatched grouping symbols."
                        );
                    }

                    operators.pop();
                    matched = true;
                    break;
                }

                postfix.push_back(std::string(1, top));
                operators.pop();
            }

            if (!matched) {
                throw std::runtime_error(
                    "Closing bracket has no matching opening bracket."
                );
            }

            expectOperand = false;
            ++i;
            continue;
        }

        // Arithmetic operators.
        if (isOperator(ch)) {
            if (expectOperand) {
                throw std::runtime_error(
                    "Unexpected operator or unsupported unary sign."
                );
            }

            // Pop operators with greater or equal precedence.
            // This preserves normal precedence and left associativity.
            while (!operators.empty() &&
                   !isOpeningBracket(operators.top()) &&
                   precedence(operators.top()) >= precedence(ch)) {
                postfix.push_back(
                    std::string(1, operators.top())
                );
                operators.pop();
            }

            operators.push(ch);
            expectOperand = true;
            ++i;
            continue;
        }

        // Reject letters, decimal points, and unsupported characters.
        throw std::runtime_error(
            "Invalid character: '" + std::string(1, ch) + "'."
        );
    }

    if (!foundToken) {
        throw std::runtime_error(
            "Expression cannot be empty."
        );
    }

    if (expectOperand) {
        throw std::runtime_error(
            "Expression ends with an operator or is incomplete."
        );
    }

    // Pop remaining operators and reject unclosed brackets.
    while (!operators.empty()) {
        char top = operators.top();
        operators.pop();

        if (isOpeningBracket(top)) {
            throw std::runtime_error(
                "Unclosed opening bracket."
            );
        }

        postfix.push_back(std::string(1, top));
    }

    return postfix;
}
