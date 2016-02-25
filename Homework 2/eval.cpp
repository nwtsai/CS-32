// #include lines you need, including "Map.h"
#include "Map.h"
#include <string>
#include <iostream>
#include <stack>
#include <cctype>
#include <cassert>

using namespace std;

// declarations of any additional functions you might have written
// to help you evaluate an expression
bool isInfix(string infix);
bool allLettersExistInMap(string infix, const Map& values);
int calculatePostFix(string postfix, const Map& values);
string infixToPostfix(string infix);
bool greaterOrEqualPrecedence(char c1, char c2);
bool triesToDivideByZero(string postfix, const Map& values);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	// if infix is not a valid infix expression, return 1
	if (!isInfix(infix))
		return 1;
	// postfix is set to the postfix version of the expression
	postfix = infixToPostfix(infix);
	// if at least one letter does not map to an existing value, return 2
	if (!allLettersExistInMap(infix, values))
		return 2;
	// if expression tries to divide by zero, return 3
	if (triesToDivideByZero(postfix, values))
		return 3;
	// set result equal to the evaluation of postfix
	result = calculatePostFix(postfix, values);
	return 0;
}

// implementations of any functions you might have written
// to help you evaluate an expression
bool isInfix(string infix)
{
	// if parameter is an empty string
	if (infix == "")
		return false;

	// checks each character in infix; if it is not an acceptable operator, parentheses, or lowercase letter, returns false
	for (int i = 0; i < infix.size(); i++)
	{
		if ((isalpha(infix[i]) || islower(infix[i]) || infix[i] == '(' || infix[i] == ')' || infix[i] == '+' 
			|| infix[i] == '-' || infix[i] == '/' || infix[i] == '*' || infix[i] == ' ') == false)
		{
			return false;
		}
	}

	// checks to see if each pair of parentheses have a left and right parentheses
	// checks if the number of lower case letters is one greater than the number of operators
	int numLetters = 0;
	int numOperators = 0;
	int LParenth = 0;
	int RParenth = 0;

	for (int i = 0; i < infix.size(); i++)
	{
		if (isalpha(infix[i]) && islower(infix[i]))
			numLetters++;
		else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '/' || infix[i] == '*')
			numOperators++;
		else if (infix[i] == '(')
			LParenth++;
		else if (infix[i] == ')')
			RParenth++;
	}

	if (numLetters - numOperators != 1)
		return false;
	if (LParenth != RParenth)
		return false;

	return true;
}

bool allLettersExistInMap(string infix, const Map& values)
{
	for (int i = 0; i < infix.size(); i++)
	{
		if (isalpha(infix[i]) && islower(infix[i]) && !values.contains(infix[i]))
			return false; 
	}
	return true;
}

int calculatePostFix(string postfix, const Map& values)
{
	if (postfix.size() == 1 && isalpha(postfix[0]) && islower(postfix[0]))
	{
		int value;
		values.get(postfix[0], value);
		return value;
	}

	stack<int> eqn;

	for (int i = 0; i < postfix.size(); i++)
	{
		// current character in the string that is being analyzed
		char curr = postfix[i]; 

		// if the current char is a variable
		if (isalpha(curr) && islower(curr))
		{
			int value;
			values.get(postfix[i], value);
			eqn.push(value);
		}
		else if (curr != ' ') // if current char is an operator and not a space
		{
			int op2 = eqn.top();
			eqn.pop();
			int op1 = eqn.top();
			eqn.pop();

			int result = 0;

			if (curr == '*')
			{
				result = op1 * op2;
			}
			else if (curr == '/')
			{
				result = op1 / op2;
			}
			else if (curr == '-')
			{
				result = op1 - op2;
			}
			else if (curr == '+')
			{
				result = op1 + op2;
			}
			eqn.push(result);
		}
	}
	return int(eqn.top());
}

// if c1 is greater than or equal to c2 in precedence, return true
bool greaterOrEqualPrecedence(char c1, char c2)
{
	if (c2 == '*' || c2 == '/')
	{
		if (c1 == '+' || c1 == '-')
		{
			return false; // only returns false if c1 is less in precedence than c2
		}
	}
	return true;
}

string infixToPostfix(string infix)
{
	string postfix = "";
	stack<char> op;

	for (int i = 0; i < infix.size(); i++)
	{
		char curr = infix[i];
		if (isalpha(curr) && islower(curr))
		{
			postfix += curr;
		}
		switch (curr)
		{
			case '(':
				op.push(curr);
				break;
			case ')':
				while (op.top() != '(')
				{
					postfix += op.top();
					op.pop();
				}
				op.pop();
				break;
			case '+':
			case '-':
			case '/':
			case '*':
				while (!op.empty() && op.top() != '(' && greaterOrEqualPrecedence(op.top(), curr))
				{
					postfix += op.top();
					op.pop();
				}
				op.push(curr);
				break;
		}
	}
	while (!op.empty())
	{
		postfix += op.top();
		op.pop();
	}
	return postfix;
}

bool triesToDivideByZero(string postfix, const Map& values)
{
	if (postfix.size() == 1)
	{
		return false;
	}

	stack<int> eqn;

	for (int i = 0; i < postfix.size(); i++)
	{
		// current character in the string that is being analyzed
		char curr = postfix[i];

		// if the current char is a variable
		if (isalpha(curr) && islower(curr))
		{
			int value;
			values.get(postfix[i], value);
			eqn.push(value);
		}
		else if (curr != ' ') // if current char is an operator and not a space
		{
			int op2 = eqn.top();
			eqn.pop();
			int op1 = eqn.top();
			eqn.pop();

			int result = 0;

			if (curr == '*')
			{
				result = op1 * op2;
			}
			else if (curr == '/')
			{
				if (op2 == 0)
				{
					return true;
				}
				result = op1 / op2;
			}
			else if (curr == '-')
			{
				result = op1 - op2;
			}
			else if (curr == '+')
			{
				result = op1 + op2;
			}
			eqn.push(result);
		}
	}
	return false;
}

// a main routine to test your function
int main()
{
	/*assert(greaterOrEqualPrecedence('*', '+'));
	assert(greaterOrEqualPrecedence('+', '+'));
	assert(!greaterOrEqualPrecedence('+', '/'));
	cerr << "All tests passed" << endl;
	string infix = "u + y * e + r";
	cout << infix << endl;
	cout << infixToPostfix(infix) << endl;*/

	char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = { 3,  -9,   6,   2,   4,   1 };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	string pf;
	int answer;
	assert(evaluate("a+ e", m, pf, answer) == 0 && pf == "ae+"  &&  answer == -6);
	answer = 999;
	assert(evaluate("", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
	// unary operators not allowed:
	assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a*b", m, pf, answer) == 2 &&
		pf == "ab*"  &&  answer == 999);
	assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
		pf == "yoau-*+"  &&  answer == -1);
	
	answer = 999;
	assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
		pf == "oyy-/"  &&  answer == 999);
	assert(evaluate(" a  ", m, pf, answer) == 0 &&
		pf == "a"  &&  answer == 3);
	assert(evaluate("((a))", m, pf, answer) == 0 &&
		pf == "a"  &&  answer == 3);
	cout << "Passed all tests" << endl;
}