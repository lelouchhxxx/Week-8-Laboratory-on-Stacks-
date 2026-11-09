
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <iomanip>
#include <stdexcept>
#include <limits>
#include <cmath>

using namespace std;

// =====================================================
// CLASS: ExpressionConverter
// Converts infix expressions to postfix notation.
// =====================================================
class ExpressionConverter {
private:
    bool isOpeningBracket(char ch) {
        return ch == '(' || ch == '[' || ch == '{';
    }

    bool isClosingBracket(char ch) {
        return ch == ')' || ch == ']' || ch == '}';
    }

    bool bracketsMatch(char opening, char closing) {
        return (opening == '(' && closing == ')') ||
               (opening == '[' && closing == ']') ||
               (opening == '{' && closing == '}');
    }

    bool isOperator(char ch) {
        return ch == '+' || ch == '-' ||
               ch == '*' || ch == '/';
    }

    int precedence(char op) {
        if (op == '*' || op == '/') {
            return 2;
        }

        if (op == '+' || op == '-') {
            return 1;
        }

        return 0;
    }

public:
    vector<string> infixToPostfix(const string& expression) {
        vector<string> postfix;
        stack<char> operators;

        bool expectOperand = true;
        bool foundToken = false;

        for (size_t i = 0; i < expression.size();) {
            char ch = expression[i];

            // Ignore whitespace.
            if (isspace(static_cast<unsigned char>(ch))) {
                ++i;
                continue;
            }

            foundToken = true;

            // Read multi-digit integer operands.
            if (isdigit(static_cast<unsigned char>(ch))) {
                if (!expectOperand) {
                    throw runtime_error(
                        "Missing operator or implicit multiplication."
                    );
                }

                size_t start = i;

                while (i < expression.size() &&
                       isdigit(static_cast<unsigned char>(expression[i]))) {
                    ++i;
                }

                postfix.push_back(expression.substr(start, i - start));
                expectOperand = false;
                continue;
            }

            // Opening grouping symbols.
            if (isOpeningBracket(ch)) {
                if (!expectOperand) {
                    throw runtime_error(
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
                    throw runtime_error(
                        "Empty group or missing operand before closing bracket."
                    );
                }

                bool matched = false;

                while (!operators.empty()) {
                    char top = operators.top();

                    if (isOpeningBracket(top)) {
                        if (!bracketsMatch(top, ch)) {
                            throw runtime_error(
                                "Mismatched grouping symbols."
                            );
                        }

                        operators.pop();
                        matched = true;
                        break;
                    }

                    postfix.push_back(string(1, top));
                    operators.pop();
                }

                if (!matched) {
                    throw runtime_error(
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
                    throw runtime_error(
                        "Unexpected operator or unsupported unary sign."
                    );
                }

                // Pop operators with greater or equal precedence.
                // This ensures left associativity.
                while (!operators.empty() &&
                       !isOpeningBracket(operators.top()) &&
                       precedence(operators.top()) >= precedence(ch)) {
                    postfix.push_back(string(1, operators.top()));
                    operators.pop();
                }

                operators.push(ch);
                expectOperand = true;
                ++i;
                continue;
            }

            // Reject letters, decimal points, and other characters.
            throw runtime_error(
                "Invalid character: '" + string(1, ch) + "'."
            );
        }

        if (!foundToken) {
            throw runtime_error("Expression cannot be empty.");
        }

        if (expectOperand) {
            throw runtime_error(
                "Expression ends with an operator or is incomplete."
            );
        }

        // Move remaining operators into the postfix output.
        while (!operators.empty()) {
            char top = operators.top();
            operators.pop();

            if (isOpeningBracket(top)) {
                throw runtime_error("Unclosed opening bracket.");
            }

            postfix.push_back(string(1, top));
        }

        return postfix;
    }
};

// =====================================================
// CLASS: ExpressionEvaluator
// Evaluates postfix expressions using stack<double>.
// =====================================================
class ExpressionEvaluator {
private:
    bool isOperator(const string& token) {
        return token == "+" || token == "-" ||
               token == "*" || token == "/";
    }

    bool isInteger(const string& token) {
        if (token.empty()) {
            return false;
        }

        for (unsigned char ch : token) {
            if (!isdigit(ch)) {
                return false;
            }
        }

        return true;
    }

public:
    double evaluatePostfix(const vector<string>& postfix) {
        stack<double> values;

        if (postfix.empty()) {
            throw runtime_error("Postfix expression cannot be empty.");
        }

        for (const string& token : postfix) {
            // Push integer operands onto the stack.
            if (isInteger(token)) {
                try {
                    size_t parsed = 0;
                    double value = stod(token, &parsed);

                    if (parsed != token.size() || !isfinite(value)) {
                        throw runtime_error("Integer is outside the supported range.");
                    }

                    values.push(value);
                } catch (const out_of_range&) {
                    throw runtime_error(
                        "Integer is outside the supported range."
                    );
                }

                continue;
            }

            if (!isOperator(token)) {
                throw runtime_error("Invalid postfix token: " + token);
            }

            // A binary operator requires two operands.
            if (values.size() < 2) {
                throw runtime_error(
                    "Stack underflow: operator needs two operands."
                );
            }

            // Pop right operand first, then left operand.
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
                    throw runtime_error(
                        "Division by zero is not allowed."
                    );
                }

                result = left / right;
            }

            if (!isfinite(result)) {
                throw runtime_error(
                    "Arithmetic result is outside the supported range."
                );
            }

            values.push(result);
        }

        // Exactly one result must remain.
        if (values.size() != 1) {
            throw runtime_error(
                "Malformed postfix expression: extra operands remain."
            );
        }

        return values.top();
    }
};

// =====================================================
// MAIN PROGRAM
// =====================================================
int main() {
    ExpressionConverter converter;
    ExpressionEvaluator evaluator;

    int choice;

    cout << "========================================\n";
    cout << " INFIX-TO-POSTFIX CONVERTER AND EVALUATOR\n";
    cout << "             C++17 PROJECT\n";
    cout << "========================================\n";
    cout << "1. Convert and evaluate infix expression\n";
    cout << "2. Evaluate postfix expression directly\n";
    cout << "Choose an option: ";

    if (!(cin >> choice)) {
        cerr << "Error: Invalid menu choice.\n";
        return 1;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    try {
        if (choice == 1) {
            string expression;

            cout << "Enter infix expression: ";
            getline(cin, expression);

            vector<string> postfix =
                converter.infixToPostfix(expression);

            cout << "\nPostfix expression: ";

            for (size_t i = 0; i < postfix.size(); ++i) {
                if (i > 0) {
                    cout << ' ';
                }

                cout << postfix[i];
            }

            double result = evaluator.evaluatePostfix(postfix);

            cout << "\nResult: "
                 << setprecision(15) << result << '\n';
        } else if (choice == 2) {
            string line;
            vector<string> postfix;

            cout << "Enter postfix tokens separated by spaces: ";
            getline(cin, line);

            // Split postfix input into individual tokens.
            size_t i = 0;

            while (i < line.size()) {
                if (isspace(static_cast<unsigned char>(line[i]))) {
                    ++i;
                    continue;
                }

                size_t start = i;

                while (i < line.size() &&
                       !isspace(static_cast<unsigned char>(line[i]))) {
                    ++i;
                }

                postfix.push_back(line.substr(start, i - start));
            }

            double result = evaluator.evaluatePostfix(postfix);

            cout << "Result: "
                 << setprecision(15) << result << '\n';
        } else {
            cerr << "Error: Please choose 1 or 2.\n";
            return 1;
        }
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
