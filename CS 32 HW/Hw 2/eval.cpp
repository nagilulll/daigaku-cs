#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

int evaluate(string infix, const Map& values, string& postfix, int& result) {
    stack<char> operators;
    postfix = "";

    for (int i = 0; i < infix.size(); ++i) {
        char ch = infix[i];
        if (ch == ' ')
            continue;
        if (islower(ch))
        {
            postfix += ch;
        }
        else if (ch == '(')
        {
            char next = infix[i + 1];
            if (next == ')' || next == '*' || next == '/' || next == '+' || next == '-')
                return 1;
            operators.push(ch);
        } else if (ch == ')')
        {
            while (!operators.empty() && operators.top() != '(')
            {
                postfix += operators.top();
                operators.pop();
            }
            if (operators.empty())
                return 1;
            operators.pop();
        }
        else
        {
            while (!operators.empty() && ((ch == '(' && (operators.top() == '*' || operators.top() == '/' || operators.top() == '+' || operators.top() == '-' || operators.top() == '-')) || ((ch == '+' || ch == '-') && (operators.top() == '*' || operators.top() == '/' || operators.top() == '+' || operators.top() == '-')) || ((ch == '*' || ch == '/') && (operators.top() == '*' || operators.top() == '/'))))
            {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(ch);
        }
    }

    while (!operators.empty())
    {
        postfix += operators.top();
        operators.pop();
    }

    stack<int> operands;
    for (int i = 0; i < postfix.size(); ++i)
    {
        char ch = postfix[i];
        if (islower(ch))
        {
            ValueType value;
            if (!values.get(ch, value))
                return 2;
            operands.push(value);
        }
        else
        {
            if (operands.size() < 2)
                return 1;
            int v2 = operands.top();
            operands.pop();
            int v1 = operands.top();
            operands.pop();
            if (ch == '+')
            {
                operands.push(v1 + v2);
            }
            else if (ch == '-')
            {
                operands.push(v1 - v2);
            }
            else if (ch == '*')
            {
                operands.push(v1 * v2);
            }
            else if (ch == '/')
            {
                if (v2 == 0)
                    return 3;
                operands.push(v1 / v2);
            }
            else
            {
                return 1;
            }
        }
    }
    if (operands.size() != 1)
        return 1;
    result = operands.top();
        return 0;
}

// Evaluates an integer arithmetic expression
//   If infix is a syntactically valid infix integer expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values map), then postfix is set to the postfix
//   form of the expression; if not, postfix may or may not be
//   changed, result is unchanged, and the function returns 1.  If
//   infix is syntactically valid but contains at least one lower
//   case letter operand that does not appear in the values map, then
//   result is unchanged and the function returns 2.  If infix is
//   syntactically valid and all its lower case operand letters
//   appear in the values map, then if evaluating the expression
//   (using for each letter in the expression the value in the map
//   that corresponds to it) attempts to divide by zero, then result
//   is unchanged and the function returns 3; otherwise, result is
//   set to the value of the expression and the function returns 0.

int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
                            pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()o", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(*o)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
      // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
                            pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
                            pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
                            pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
                            pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
                            pf == "a"  &&  answer == 3);
    cout << "Passed all tests" << endl;
}
