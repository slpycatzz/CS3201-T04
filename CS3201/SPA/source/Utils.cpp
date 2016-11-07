#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "Utils.h"

using std::istringstream;
using std::queue;
using std::set;
using std::stack;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

vector<vector<string>> Utils::Flatten(unordered_map<string, vector<string>> &map,
    vector<string> &list, unsigned int start, unsigned int end) {
    vector<vector<string>> result;
    vector<string> firstVList = map[list[start]];

    if (start == end) {
        for (string value : firstVList) {
            vector<string> vec({ value });
            result.push_back(vec);
        }

    } else {
        vector<vector<string>> recursiveList = Flatten(map, list, start + 1, end);

        for (string value : firstVList) {
            for (vector<string> vec : recursiveList) {
                vec.insert(vec.begin(), value);
                result.push_back(vec);
            }
        }
    }

    return result;
}

vector<vector<string>> Utils::Zip(vector<string> list1, vector<string> list2) {
    vector<vector<string>> result;

    for (string item1 : list1) {
        for (string item2 : list2) {
            vector<string> item;
            item.push_back(item1);
            item.push_back(item2);

            result.push_back(item);
        }
    }

    return result;
}

bool Utils::VectorContains(vector<unsigned int> vec, unsigned int value) {
    return (std::find(vec.begin(), vec.end(), value) != vec.end());
}

bool Utils::VectorContains(vector<string> vec, string value) {
    return (std::find(vec.begin(), vec.end(), value) != vec.end());
}

string Utils::VectorToString(vector<string> &vec) {
    stringstream ss;
    ss << "<";

    vector<string>::iterator iter(vec.begin());
    while (iter != vec.end()) {
        ss << *iter;

        iter++;
        if (iter != vec.end()) {
            ss << CHAR_SYMBOL_COMMA;
        }
    }

    ss << ">";
    return ss.str();
}

vector<string> Utils::VectorToStrings(vector<vector<string>> &vecs) {
    vector<string> result;

    for (vector<string> vec : vecs) {
        result.push_back(VectorToString(vec));
    }

    return result;
}

unordered_map<string, string> Utils::MergeMap(unordered_map<string, string> &map1, unordered_map<string, string> &map2) {
    unordered_map<string, string> map;

    map.insert(map1.begin(), map1.end());
    map.insert(map2.begin(), map2.end());

    return map;
}

unordered_map<string, unsigned> Utils::MergeMap(unordered_map<string, unsigned> &map1, unordered_map<string, unsigned> &map2) {
	unordered_map<string, unsigned> map;

	map.insert(map1.begin(), map1.end());
	map.insert(map2.begin(), map2.end());

	return map;
}

unordered_map<string, string> Utils::ReduceMap(unordered_map<string, string> &map, vector<string> &selections) {
    unordered_map<string, string> result;
    for (auto kv : map) {
        if (VectorContains(selections, kv.first)) {
            result.insert_or_assign(kv.first, kv.second);
        }
    }

    return result;
}

unordered_map<string, string> Utils::GetSubMap(unordered_map<string, string>& map, const vector<string>& keyList) {
    unordered_map<string, string> result;
    for (string key : keyList) {
        result.insert_or_assign(key, map.at(key));
    }

    return result;
}

unordered_map<string, unsigned> Utils::GetSubMap(unordered_map<string, unsigned> &map, const vector<string> &keyList) {
	unordered_map<string, unsigned> result;
	for (string key : keyList) {
		result.insert_or_assign(key, map.at(key));
	}

	return result;
}

string Utils::MapToString(unordered_map<string, string> &map) {
    stringstream ss;
    ss << "<";

    unordered_map<string, string>::iterator iter(map.begin());
    while (iter != map.end()) {
        ss << (*iter).first << ":" << (*iter).second;

        iter++;
        if (iter == map.end()) {
            break;
        }

        ss << CHAR_SYMBOL_COMMA;
    }

    ss << ">";
    return ss.str();
}

string Utils::MapToString(unordered_map<string, unsigned> &map) {
	stringstream ss;
	ss << "<";

	unordered_map<string, unsigned>::iterator iter(map.begin());
	while (iter != map.end()) {
		ss << (*iter).first << ":" << (*iter).second;

		iter++;
		if (iter == map.end()) {
			break;
		}

		ss << CHAR_SYMBOL_COMMA;
	}

	ss << ">";
	return ss.str();
}

string Utils::MapToValueString(unordered_map<string, string> &map) {
    stringstream ss;
    ss << "<";

    unordered_map<string, string>::iterator iter(map.begin());
    while (true) {
        ss << (*iter).second;

        iter++;
        if (iter == map.end()) {
            break;
        }

        ss << CHAR_SYMBOL_COMMA;
    }

    ss << ">";
    return ss.str();
}

string Utils::IntToString(unsigned int i) {
    return std::to_string(i);
}

vector<string> Utils::IntsToStrings(vector<unsigned int> vec) {
    vector<string> result;

    for (unsigned int i : vec) {
        result.push_back(IntToString(i));
    }

    return result;
}

unsigned int Utils::StringToInt(string str) {
    return std::stoi(str);
}

vector<unsigned int> Utils::StringsToInts(vector<string> vec) {
    vector<unsigned int> result;

    for (string str : vec) {
        result.push_back(StringToInt(str));
    }

    return result;
}

void Utils::Split(string str, char delimiter, vector<string> &target) {
    string temp;
    istringstream stringStream;

    stringStream.str(str);
    while (getline(stringStream, temp, delimiter)) {
        target.push_back(temp);
    }
}

vector<string> Utils::Split(string str, char delimiter) {
    vector<string> result;

    Utils::Split(str, delimiter, result);

    return result;
}

void Utils::SplitAndIgnoreEmpty(string str, string delimiters, vector<string> &target) {
    string current, temp;
    istringstream stringStream;

    stringStream.str(str);
    while (getline(stringStream, current)) {
        int previous = 0, position;
        while ((position = current.find_first_of(delimiters, previous)) != string::npos) {
            if (position > previous) {
                temp = current.substr(previous, position - previous);
                temp = TrimSpaces(temp);

                if (!temp.empty()) {
                    target.push_back(temp);
                }
            }

            previous = position + 1;
        }

        if (previous < current.length()) {
            temp = current.substr(previous, string::npos);
            temp = TrimSpaces(temp);

            if (!temp.empty()) {
                target.push_back(temp);
            }
        }
    }
}

void Utils::SplitAndIgnoreEmpty(string str, char delimiter, vector<string> &target) {
    string temp;
    istringstream stringStream;

    stringStream.str(str);
    while (getline(stringStream, temp, delimiter)) {
        temp = TrimSpaces(temp);

        if (!temp.empty()) {
            target.push_back(temp);
        }
    }
}

vector<string> Utils::SplitAndIgnoreEmpty(string str, char delimiter) {
    vector<string> result;

    Utils::SplitAndIgnoreEmpty(str, delimiter, result);

    return result;
}

string Utils::TrimSpaces(string str) {
    str = Utils::TrimLeadingSpaces(str);
    str = Utils::TrimTrailingSpaces(str);

    return str;
}

string Utils::TrimLeadingSpaces(string str) {
    int position = str.find_first_not_of(" \t");
    if (position == string::npos) {
        return (str.find_last_of(" \t") == (str.size() - 1)) ? "" : str;
    }

    return str.substr(position);
}

string Utils::TrimTrailingSpaces(string str) {
    int position = str.find_last_not_of(" \t");
    if (position == string::npos) {
        return (str.find_last_of(" \t") == (str.size() - 1)) ? "" : str;
    }

    return str.substr(0, position + 1);
}

bool Utils::IsValidNamingConvention(string str) {
    return StartsWithAnAlphabet(str) && IsAlphanumeric(str);
}

bool Utils::IsAlphanumeric(string str) {
    for (unsigned int i = 0; i < str.length(); i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }

    return true;
}

bool Utils::IsNonNegativeNumeric(string str) {
    for (unsigned int i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    return !str.empty();
}

bool Utils::StartsWith(string str, char c) {
    return (str[0] == c);
}

bool Utils::StartsWith(string str, string substr) {
    return (str.compare(0, substr.length(), substr) == 0);
}

bool Utils::StartsWithAnAlphabet(string str) {
    return (isalpha(str[0])) ? true : false;
}

bool Utils::EndsWith(string str, char c) {
    return (str[str.length() - 1] == c);
}

bool Utils::EndsWith(string str, string substr) {
    return (str.compare(str.length() - substr.length(), substr.length(), substr) == 0);
}

/* Returns a queue with the postfix expression. */
queue<string> Utils::GetPostfixExpression(vector<string> expression) {
    queue<string> operands;
    stack<string> operators;

    /* Uses shunting yard algorithm to get postfix expression. */
    for (string &token : expression) {
        if (IsOperator(token)) {
            while (!operators.empty()) {
                string str = operators.top();

                if (!IsOperator(str)) {
                    break;
                }

                if (GetOperatorPrecedence(token) > GetOperatorPrecedence(str)) {
                    break;
                }

                operands.push(str);
                operators.pop();
            }

            operators.push(token);

        } else if (token == string(1, CHAR_SYMBOL_OPENBRACKET)) {
            operators.push(token);

        } else if (token == string(1, CHAR_SYMBOL_CLOSEBRACKET)) {
            while (!operators.empty()) {
                string str = operators.top();
                operators.pop();

                if (str == string(1, CHAR_SYMBOL_OPENBRACKET)) {
                    break;
                }

                operands.push(str);
            }

        } else {
            operands.push(token);
        }
    }

    while (!operators.empty()) {
        operands.push(operators.top());
        operators.pop();
    }

    return operands;
}

string Utils::GetExactExpressionWithBrackets(queue<string> postfixExpression) {
    stack<string> result;

    /* Reads the postfix expression to format the expression with brackets. */
    while (!postfixExpression.empty()) {
        if (IsOperator(postfixExpression.front())) {
            string rightOperand = result.top();
            result.pop();

            string leftOperand = result.top();
            result.pop();

            /* Format with brackets. */
            string expression = string(1, CHAR_SYMBOL_OPENBRACKET);
            expression += leftOperand + postfixExpression.front() + rightOperand;
            expression += string(1, CHAR_SYMBOL_CLOSEBRACKET);

            postfixExpression.pop();
            result.push(expression);

        } else {
            result.push(postfixExpression.front());
            postfixExpression.pop();
        }
    }

    return (result.empty()) ? "" : result.top();
}

set<string> Utils::GetSubExpressionsWithBrackets(queue<string> postfixExpression) {
    stack<string> result;
    set<string> results;

    /* Reads the postfix expression to format the expression with brackets. */
    while (!postfixExpression.empty()) {
        if (IsOperator(postfixExpression.front())) {
            string rightOperand = result.top();
            result.pop();

            string leftOperand = result.top();
            result.pop();

            /* Format with brackets. */
            string expression = string(1, CHAR_SYMBOL_OPENBRACKET);
            expression += leftOperand + postfixExpression.front() + rightOperand;
            expression += string(1, CHAR_SYMBOL_CLOSEBRACKET);

            postfixExpression.pop();

            result.push(expression);
            results.insert(expression);

        } else {
            string operand = postfixExpression.front();
            postfixExpression.pop();

            result.push(operand);
            results.insert(operand);
        }
    }

    return results;
}

bool Utils::IsOperator(string operator_) {
    if (operator_ == string(1, CHAR_SYMBOL_MINUS) || operator_ == string(1, CHAR_SYMBOL_PLUS)) {
        return true;
    }

    if (operator_ == string(1, CHAR_SYMBOL_MULTIPLY)) {
        return true;
    }

    return false;
}

unsigned int Utils::GetOperatorPrecedence(string operator_) {
    if (operator_ == string(1, CHAR_SYMBOL_MINUS) || operator_ == string(1, CHAR_SYMBOL_PLUS)) {
        return 1;
    }

    if (operator_ == string(1, CHAR_SYMBOL_MULTIPLY)) {
        return 2;
    }

    return 0;
}
