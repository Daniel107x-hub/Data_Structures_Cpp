//
// Created by H460351 on 3/7/2024.
//
#include <iostream>
#include <tuple>
#include <map>
#include <stack>
#include <string>
#include <sstream>
using namespace std;

tuple<int, int> getLines(){
    int lines[2];
    string line;
    getline(cin, line);
    istringstream iss(line);
    int token;
    int position = -1;
    while(iss >> token){
        lines[++position] = (int) token;
    }
    return make_tuple(lines[0], lines[1]);
}

bool isClosingTag(const string& token){
    return token.find("</") != std::string::npos;
}

string getTagName(const string& token){
    bool hasAttributes = token.find('>') >= 0;
    if(hasAttributes) return token.substr(1);
    return token.substr(1, token.length());
}

map<string, string> getHRMLAttributes(int lines){
    map<string, string> attributes;
    stack<string> tags;
    tags.emplace("");
    for(int i = 0 ; i < lines ; i++){
        string line;
        getline(cin, line);
        istringstream  iss(line);
        string token;
        string currentTag = tags.top();
        string key;
        string value;
        bool equalAppeared = false;
        bool isFirstToken = true;
        while(iss >> token){
            if(isClosingTag(token)){
                tags.pop();
                break;
            }
            if(isFirstToken){
                string tagName = getTagName(token);
                if(!currentTag.empty()) currentTag = currentTag.append(".");
                currentTag = currentTag.append(tagName);
                tags.emplace(currentTag);
                isFirstToken = false;
                continue;
            }
            if(!equalAppeared && token != "="){
                key = token;
            }
            if(!equalAppeared && token == "="){
                equalAppeared = true;
            }
            if(equalAppeared && token != "="){
                value = token;
                if(token.find('>') >= 0) value = token.substr(0, token.length() - 1);
                string fullKey = currentTag.append("~").append(key);
                attributes.emplace(fullKey, value);
                equalAppeared = false;
            }
        }
    }
}

int main(){
    auto lines = getLines();
    int codeLines = get<0>(lines);
    int queryLines = get<1>(lines);
    map<string, string> attributes = getHRMLAttributes(codeLines);
    return 0;
}
