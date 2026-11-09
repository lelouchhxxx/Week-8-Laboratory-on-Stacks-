
#include "ExpressionEvaluator.h"

#include <cctype>
#include <cmath>
#include <stack>
#include <stdexcept>
#include <string>

bool ExpressionEvaluator::isOperator(
    const std::string& token
) const {
    return token == "+" || token == "-" ||
           token == "*" || token == "/";
}

bool ExpressionEvaluator::isInteger(
    const std::string& token
) const {
    if (token.empty()) {
        return false;
    }

    for (unsigned char ch : token) {
        if (!std::isdigit(ch)) {
            return false;
        }
    }

    return true;
}

double ExpressionEvaluator::evaluatePostfix(
    const std::vector<std::string>& postfix
) {
    std::stack<double> values;

    if (postfix.empty()) {
        throw std::runtime_error(
            "Postfix expression cannot be empty."
        );
    }

    for (const std::string& token : postfix) {
        // Push integer operands onto the stack.
        if (isInteger(token)) {
            double value;

            try {
                std::size_t parsed = 0;
                value = std::stod(token, &parsed);

                if (parsed != token.size() || !std::isfinite(value)) {
                    throw std::runtime_error(
                        "Integer is outside the supported range."
                    );
                }
            } catch (const std::out_of_range&) {
                throw std::runtime_error(
                    "Integer is outside the supported range."
                );
            }

            values.push(value);
            continue;
        }

        if (!isOperator(token)) {
            throw std::runtime_error(
                "Invalid postfix token: " + token
            );
        }

        // Each binary operator needs two operands.
        if (values.size() < 2) {
            throw std::runtime_error(
                "Stack underflow: operator needs two operands."
            );
        }

        // Pop the right operand before the left operand.
        double right = values.top();
        values.pop();

        double left = values.top();
        values.pop();

        double result = 0.0;

        if (token == "+") {
            result = left + right;
        } else if (token == "-") {
            result = left - right;
        } else if (token == "*") {
            result = left * right;
        } else if (token == "/") {
            if (right == 0.0) {
                throw std::runtime_error(
                    "Division by zero is not allowed."
                );
            }

            result = left / right;
        }

        if (!std::isfinite(result)) {
            throw std::runtime_error(
                "Arithmetic result is outside the supported range."
            );
        }

        values.push(result);
    }

    // A valid postfix expression must leave exactly one value.
    if (values.size() != 1) {
        throw std::runtime_error(
            "Malformed postfix expression: extra operands remain."
        );
    }

    return values.top();
}
