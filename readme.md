Week 8 Laboratory: Infix-to-Postfix Converter and Evaluator Institution: National Teachers College Course: CC105 – Data Structure with Lab Project Title: Infix-to-Postfix Conversion and Postfix Evaluation Using Stack Programming Language: C++17

1. Project Description This project implements an infix-to-postfix converter and a postfix expression evaluator using the stack data structure.

The program accepts arithmetic expressions, converts infix expressions into postfix notation, and evaluates the result using double values. It also checks for invalid expressions and reports errors instead of performing invalid operations.

2. Project Members and Roles Replace the placeholders with the actual names of your group members and their assigned responsibilities.

3. Compiler and Development Environment Programming language: C++17 Compiler: GCC/MinGW-w64 or another C++17-compatible compiler IDE: Visual Studio Code or Code::Blocks Data structure used: Stack Standard library components: std::stack, std::vector, and std::string Record the actual compiler version used for testing before submission.

4. Project File Structure

text InfixPostfixProject/ ├── main.cpp ├── ExpressionConverter.h ├── ExpressionConverter.cpp ├── ExpressionEvaluator.h ├── ExpressionEvaluator.cpp ├── README.md ├── test\_report.md ├── traces.md └── screenshots/ ├── correct\_expression.png ├── bracket\_error.png ├── division\_by\_zero.png └── empty\_stack.png 5\. Build and Run Instructions Using Windows Command Prompt Open the project folder. Open Command Prompt in that folder. Compile the source files using:

cmd g++ \-std=c++17 \-Wall \-Wextra \-pedantic main.cpp ExpressionConverter.cpp ExpressionEvaluator.cpp \-o infix\_postfix.exe Run the program:

cmd infix\_postfix.exe Alternatively, open the project in Code::Blocks, add all source and header files, select the C++17 standard, and build and run the project.

6. Supported Syntax The program supports the following syntax:

Nonnegative integer operands, including multi-digit integers. Addition (+), subtraction (-), multiplication (\*), and division (/). Parentheses (), square brackets \[\], and curly braces {} as grouping symbols. Whitespace between tokens. Normal operator precedence: multiplication and division before addition and subtraction. Left-associative operators of equal precedence. Floating-point evaluation using double. Example Input infix expression:

text (12 \+ 3\) \* 2 Postfix expression:

text 12 3 \+ 2 \* Result:

text 30 Another example:

text Input: 7 / 2 Postfix: 7 2 / Result: 3.5 7\. Limitations and Error Handling The program intentionally rejects:

Negative-number and unary-sign expressions, such as \-5 \+ 2\. Decimal operands, such as 12.5 \+ 2\. Letters and unsupported characters. Implicit multiplication, such as 2(3 \+ 4). Empty expressions and empty groups. Mismatched or unclosed grouping symbols. Malformed expressions, such as 2 \+ \* 3\. Division by zero. Invalid postfix expressions, insufficient operands, and extra leftover operands. Exponents and other operators are not supported.

The evaluator uses double, so very large integers may not be represented exactly.

8. Algorithm Explanation A. Infix-to-Postfix Conversion The program uses the shunting-yard algorithm.

Read the expression from left to right. Ignore whitespace. Add integer operands directly to the postfix output. Push opening grouping symbols onto the operator stack. When an operator appears, pop operators with greater or equal precedence before pushing the current operator. When a closing grouping symbol appears, pop operators until the matching opening symbol is found. At the end, move the remaining operators into the output. Reject malformed expressions and unmatched grouping symbols. Popping operators of equal precedence ensures left associativity.

B. Postfix Evaluation The program uses a stack of double values.

Read each postfix token. Push each valid integer operand onto the stack. When an operator appears, check that at least two operands are available. Pop the right operand first, followed by the left operand. Perform the operation and push the result. Reject division by zero and invalid arithmetic results. At the end, require exactly one value to remain on the stack. 9\. Test Report The test report documents valid expressions, malformed expressions, grouping-symbol errors, division by zero, and postfix stack errors.

The report includes the following fields:

Test case number Input Expected output Actual output Pass or Fail status See test\_report.md for the detailed test cases and results.

10. Screenshots The project includes screenshots demonstrating:

Correct expression conversion and evaluation. An unclosed or mismatched grouping-symbol error. Division-by-zero handling. Postfix stack-underflow handling. The screenshots are stored in the screenshots/ folder.

11. Conversion and Evaluation Traces The project includes a conversion trace showing the input tokens, operator stack, and postfix output.

It also includes a postfix evaluation trace showing each token, the operation performed, and the stack contents after each step.

See traces.md for the detailed traces.

12. Reflection Questions Answers to reflection questions 19–23 are included in the project documentation. They explain nested-bracket handling, operand order during evaluation, the difference between balanced brackets and valid expressions, the relationship between stacks and function calls, and memory allocation and release.

13. Conclusion This project demonstrates how stacks can be used to convert infix arithmetic expressions into postfix notation and evaluate postfix expressions. It also demonstrates operator precedence, left associativity, grouping-symbol validation, error handling, and floating-point arithmetic using C++17.