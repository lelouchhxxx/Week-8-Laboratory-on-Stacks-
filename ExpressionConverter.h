
#ifndef EXPRESSION_CONVERTER_H
#define EXPRESSION_CONVERTER_H

#include <string>
#include <vector>

class ExpressionConverter {
public:
    // Convert an infix expression to postfix notation.
    // Throws an exception if the expression is invalid.
    std::vector<std::string> infixToPostfix(
        const std::string& expression
    );

private:
    bool isOpeningBracket(char ch) const;
    bool isClosingBracket(char ch) const;
    bool bracketsMatch(char opening, char closing) const;
    bool isOperator(char ch) const;
    int precedence(char op) const;
};

#endif
