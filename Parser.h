//
// Created by root on 29/11/2016.
//

#ifndef LEXICALPARSER_PARSER_H
#define LEXICALPARSER_PARSER_H

#include<iostream>
#include<fstream>
#include<vector>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

class CParser {

public:
    CParser();
    ~CParser();

private:
    bool GetProduction(const char * szFileName);
    void split(const std::string &s, char delim, std::vector<std::string> &elems);
    vector<string> split(const string &s, char delim);
    bool AvoidLeftRecursion(vector<string> &vListProduction);
    bool DetectLeftRecursion(string strProduction);
    bool IsValidProduction(string strProduction);
    bool ParserProduction(string strProduction, string & strLeft, vector<string> &vListRight);
    bool ReplaceProduction(string &strProduction, string strToken, string strStringReplate);
    bool SplitProduction(string strProduction, string & strLeft, vector<string> &vListRight);
    bool ReplaceProByPro(string strSource, string &strDest);
    bool SplitProToPro(string strProduction, vector<string> &vListPro);
    bool ReplaceSingleProBySinglePro(string strSouce, string &strDest);
    bool AnalyzeProduction(vector<string> vListProduction, vector<string> &vTerminal, vector<string> &vNonTerminal);
    bool GetRight(vector<string> vProduction, string strLeft, vector<string> &vRight);
    bool GetFirstSet(vector<string> vProduction, vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, vector<string>> &mFirstSet);
    bool GetFirstSetNew(vector<string> vProduction, vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, vector<string>> &mFirstSet);

    void PrintProduction(vector<string> vProduction);
    void PrintUnorderedmap(unordered_map<string, vector<string>> umMap);
    bool IndexOf(string strToken, vector<string> vTerminal);
    bool InserMap(string strKey, string strValue, unordered_map<string, vector<string>> &mMap);
    vector<string> * GetListInMap(unordered_map<string, vector<string>> &mMap, string strKey);

private:
    string m_strStart;
    vector<string> m_vListProduction;
    vector<string> m_vTerminal;
    vector<string> m_vNonTerminal;
};


#endif //LEXICALPARSER_PARSER_H
