//
// Created by H460351 on 3/8/2024.
//

#include <iostream>
#include <tuple>
#include <map>
#include <stack>
#include <string>
#include <sstream>
#include "attribute_parser.h"
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
    bool hasAttributes = token.find('>') == std::string::npos;
    if(hasAttributes) return token.substr(1);
    return token.substr(1, token.length() - 2);
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
                if(token.find('>') != std::string::npos) token = token.substr(0, token.length() - 1);
                value = token.substr(1, token.length() - 2);
                string fullKey = currentTag;
                fullKey.append("~").append(key);
                attributes.emplace(fullKey, value);
                equalAppeared = false;
            }
        }
    }
    return attributes;
}

void executeQueries(int lines, const map<string, string>& attributes){
    for(int i = 0 ; i < lines ; i++){
        string line;
        getline(cin, line);
        auto const& iterator = attributes.find(line);
        if(iterator == attributes.end()) cout << "Not Found!";
        else cout << iterator->second;
        cout << "\n";
    }
}


int attribute_parser::run(){
    auto lines = getLines();
    int codeLines = get<0>(lines);
    int queryLines = get<1>(lines);
    map<string, string> attributes = getHRMLAttributes(codeLines);
    executeQueries(queryLines, attributes);
    return 0;
}
