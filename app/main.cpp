#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

// ---------------------------
// LEXER STATES
// ---------------------------
enum LexState { L_START, L_STRING, L_NUMBER };

// ---------------------------
// SYNTAX STATES
// ---------------------------
enum SynState { S_START, S_KEY, S_COLON, S_VALUE, S_COMMA };

// --------------------------------------------------------
//                  LEXICAL ANALYZER
// --------------------------------------------------------
vector<pair<string, string>> lexicalAnalyzer(const string &input) {
    vector<pair<string, string>> tokens;
    LexState state = L_START;
    string buffer = "";
    int i = 0;

    while (i < input.length()) {
        char c = input[i];

        switch (state) {
            case L_START:
                if (c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',') {
                    tokens.push_back({"STRUCTURAL", string(1, c)});
                    i++;
                }
                else if (c == '"') {
                    state = L_STRING;
                    buffer = "";
                    i++;
                }
                else if (isdigit(c) || c == '-') {
                    state = L_NUMBER;
                    buffer = c;
                    i++;
                }
                else if (isspace(c)) {
                    i++;
                }
                else {
                    tokens.push_back({"ERROR", string(1, c)});
                    i++;
                }
                break;

            case L_STRING:
                if (c == '"') {
                    tokens.push_back({"STRING", buffer});
                    buffer = "";
                    state = L_START;
                    i++;
                } else {
                    buffer += c;
                    i++;
                }
                break;

            case L_NUMBER:
                if (isdigit(c)) {
                    buffer += c;
                    i++;
                }
                else {
                    // End integer
                    tokens.push_back({"NUMBER", buffer});
                    buffer = "";
                    state = L_START;
                    // do NOT consume c — it will be processed next loop
                }
                break;

        }
    }

    if (state == L_NUMBER && !buffer.empty())
        tokens.push_back({"NUMBER", buffer});

    return tokens;
}

// --------------------------------------------------------
//                  SYNTAX ANALYZER
// --------------------------------------------------------
void syntaxAnalyzer(const vector<pair<string,string>> &tokens) {
    stack<char> stk;
    SynState state = S_START;

    auto toStateStr = [](SynState s) {
        switch(s) {
            case S_START: return "S_START";
            case S_KEY: return "S_KEY";
            case S_COLON: return "S_COLON";
            case S_VALUE: return "S_VALUE";
            case S_COMMA: return "S_COMMA";
        }
        return "UNKNOWN";
    };

    auto printTransition = [&](SynState from, const string &read, const string &action, SynState to) {
        cout << "{\"state\":\"" << toStateStr(from) << "\","
             << "\"read\":\"" << read << "\","
             << "\"action\":\"" << action << "\","
             << "\"next_state\":\"" << toStateStr(to) << "\"}\n";
    };

    auto printError = [&](int i, const string &expected) {
        string found = tokens[i].first + " ('" + tokens[i].second + "')";
        cout << "{\"error\":\"Expected " << expected << " but found " << found << " at token " << i << "\"}\n";
        exit(1);
    };

    for (int i = 0; i < (int)tokens.size(); i++) {
        string type = tokens[i].first;
        string value = tokens[i].second;

        switch (state) {
            case S_START:
                if (type == "STRUCTURAL" && value == "{") {
                    stk.push('{');
                    printTransition(state, "{", "push '{'", S_KEY);
                    state = S_KEY;
                }
                else if (type == "STRUCTURAL" && value == "[") {
                    stk.push('[');
                    printTransition(state, "[", "push '['", S_VALUE);
                    state = S_VALUE;
                }
                else
                    printError(i, "'{' or '['");
                break;

            case S_KEY:
                if (type == "STRING") {
                    printTransition(state, "STRING", "none", S_COLON);
                    state = S_COLON;
                }
                else if (type == "STRUCTURAL" && value == "}") {
                    if (stk.empty() || stk.top() != '{')
                        printError(i, "'}' (matching '{')");
                    stk.pop();
                    printTransition(state, "}", "pop '{'", S_COMMA);
                    state = S_COMMA;
                }
                else
                    printError(i, "STRING or '}'");
                break;

            case S_COLON:
                if (type == "STRUCTURAL" && value == ":") {
                    printTransition(state, ":", "none", S_VALUE);
                    state = S_VALUE;
                }
                else
                    printError(i, "':'");
                break;

            case S_VALUE:
                if (type == "STRING" || type == "NUMBER") {
                    printTransition(state, type, "none", S_COMMA);
                    state = S_COMMA;
                }
                else if (type == "STRUCTURAL" && value == "{") {
                    stk.push('{');
                    printTransition(state, "{", "push '{'", S_KEY);
                    state = S_KEY;
                }
                else if (type == "STRUCTURAL" && value == "[") {
                    stk.push('[');
                    printTransition(state, "[", "push '['", S_VALUE);
                    state = S_VALUE;
                }
                else
                    printError(i, "STRING, NUMBER, '{', or '['");
                break;

            case S_COMMA:
                if (type == "STRUCTURAL" && value == ",") {
                    if (!stk.empty() && stk.top() == '{') {
                        printTransition(state, ",", "none", S_KEY);
                        state = S_KEY;
                    }
                    else if (!stk.empty() && stk.top() == '[') {
                        printTransition(state, ",", "none", S_VALUE);
                        state = S_VALUE;
                    }
                    else {
                        cout << "{\"error\":\"Unexpected ',' at token " << i << "\"}\n";
                        exit(1);
                    }
                }
                else if (type == "STRUCTURAL" && value == "}") {
                    if (stk.empty() || stk.top() != '{')
                        printError(i, "'}' (matching '{')");
                    stk.pop();
                    printTransition(state, "}", "pop '{'", S_COMMA);
                }
                else if (type == "STRUCTURAL" && value == "]") {
                    if (stk.empty() || stk.top() != '[')
                        printError(i, "']' (matching '[')");
                    stk.pop();
                    printTransition(state, "]", "pop '['", S_COMMA);
                }
                else
                    printError(i, "',', '}', or ']'");
                break;
        }
    }

    if (!stk.empty()) {
        cout << "{\"error\":\"Unclosed opening bracket: '" << stk.top() << "'\"}\n";
        exit(1);
    }
}


// --------------------------------------------------------
//                       MAIN
// --------------------------------------------------------
int main() {
    // string input = R"({"id":"4","name":"BGMU!","fields":[{"floor":"1","id":0},{"floor":"2","id":1.2}]})";
    // string input = R"({"x":5,})";
    string input = R"({"id":{"hi":3.1.2},"name":"BGMU!","fields":[{"floor":"1","id":0},{"floor":"2","id":1.2}]})";

    // first tokenize the input
    auto tokens = lexicalAnalyzer(input);

    // print tokens for debugging
    cout << "\nTokens:\n";
    for (auto &t : tokens)
        cout << t.first << " : " << t.second << endl;

    // then perform syntax analysis
    syntaxAnalyzer(tokens);

    return 0;
}
