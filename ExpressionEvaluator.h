
#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H

#include <string>
#include <vector>

class ExpressionEvaluator {
public:
    // Evaluate postfix tokens and return a double result.
    double evaluatePostfix(
        const std::vector<std::string>& postfix
    );

private:
    bool isOperator(const std::string& token) const;
    bool isInteger(const std::string& token) const;
};

#endif
