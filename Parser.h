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
#include "GlobalConst.h"
#include <stack>
#include <map>
#include "ReleaseEvent.h"

using namespace std;

class CParser {

public:
    CParser();
    ~CParser();

private:
    //==========================================================================
    /**
     * Ham doc luat sinh tu file
     * @param
     *          Input : szFileName : File chua tap luat sinh
     *          Output :
     * @return  : true neu khong co loi
     *            false neu xay ra loi
     */
    bool GetProduction(const char * szFileName);
    //==========================================================================



    //==========================================================================
    /**
     * Ham tach xau boi 1 ky tu
     * @param
     *          Input : s       : Xau can tach
     *                  delim   :   Ky tu tach xau
     *          Output : elems : Tap cac xau sau khi tach
     * @return  : true neu khong co loi
     *            false neu xay ra loi
     * @example split("a:b:c", ':', elems)
     *          => elems = <a, b, c>
     */

    void split(const std::string &s, char delim, std::vector<std::string> &elems);
    //==========================================================================



    //==========================================================================
    /**
     *
     * @param s
     * @param delim
     * @return
     */
    vector<string> split(const string &s, char delim);
    //==========================================================================



    //==========================================================================
    /**
     * Bo de quy trai
     * @param
     *          Input_Output : vListProduction : Tap luat sinh can huy de quy trai
     * @return  true neu khong loi
     *          false neu co loi
     */
    bool AvoidLeftRecursion(vector<string> &vListProduction);
    //==========================================================================



    //==========================================================================
    /**
     * Phat hien 1 luat sinh co de quy trai hay khong
     * @param
     *          Input : strProduction : Luat sinh can xet
     * @return  true : neu luat sinh la de quy trai
     *          false : neu luat sinh khong de quy trai (hoac co loi xay ra)
     */
    bool DetectLeftRecursion(string strProduction);
    //==========================================================================



    //==========================================================================
    /**
     * Ham kiem tra tinh hop le cua 1 luat sinh
     * @param strProduction : Luat sinh can kiem tra
     * @return  true neu luat sinh hop le
     *          false neu luat sinh khong hop le
     */
    bool IsValidProduction(string strProduction);
    //==========================================================================


    //==========================================================================
    /**
     * Ham tach luat sinh
     * @param
     *          Input : strProduction   :   Luat sinh can tach
     *          Output: strLeft         :   Luu tru ve trai
     *                  vListRight      :   Luu tru tap cac ky tu ve phai
     * @return  true : khong xay ra loi
     *          false : co loi xay ra
     * @example  S : a b c | d e
     *           => strLeft : S
     *           => vListRight : [a, b, c, d, e]
     */
    bool ParserProduction(string strProduction, string & strLeft, vector<string> &vListRight);
    //==========================================================================


    //==========================================================================
    /**
     * Thay the ky tu trong ve phai boi 1 ky tu khac
     * @param
     *          Input_Output : strProduction : Luat sinh can xu ly
     *          Input : strToken    :   Ky tu bi thay the
     *                  strStringReplate    :   Ky tu duoc thay the
     * @return  true neu khong loi
     *          false neu co loi
     */
    bool ReplaceProduction(string &strProduction, string strToken, string strStringReplate);
    //==========================================================================



    //==========================================================================
    /**
    * Tach luat sinh
    * VD : SplitProduction(S->a b | c) se tach thanh
    *      strLeft : S
    *      vListRight : <a b, c>
    * @param
     *          Input  : strProduction : luat sinh
    *           Output :  strLeft       : phan ben trai
    * @param vListRight    : danh sach phan ben phai
    * @return
    */
    bool SplitProduction(string strProduction, string & strLeft, vector<string> &vListRight);
    //==========================================================================



    //==========================================================================
    /**
    * @function : thay the luat sinh dich boi luat sinh nguon
    *             VD : thay A->S c boi S->A b se duoc ket qua A->A b c
    * @param strSouce : luat sinh nguon
    * @param strDest  : luat sinh dich
    * @return  true if not error
    *          false if error
    */
    bool ReplaceProByPro(string strSource, string &strDest);
    //==========================================================================



    //==========================================================================
    /**
    * @funcion :   Tach luat sinh thanh nhieu luat sinh con
    *              VD S: a | b tach thanh S : a va S : b
    * @param strProduction : Luat sinh can tach
    * @param vListPro      : luu tru danh sach ket qua
    * @return  true if not error
    *          else return false
    */
    bool SplitProToPro(string strProduction, vector<string> &vListPro);
    //==========================================================================


    //==========================================================================
    /**
    * @function : thay the luat sinh dich boi luat sinh nguon
    *             VD : thay A->S c boi S->A b se duoc ket qua A->A b c
    * @param strSouce : luat sinh nguon
    * @param strDest  : luat sinh dich
    * @return  true if not error
    *          false if error
    */
    bool ReplaceSingleProBySinglePro(string strSouce, string &strDest);
    //==========================================================================



    //==========================================================================
    /**
    * Tach tap luat sinh thanh 2 tap terminal va nonterminal
    * @param vListProduction   : Danh sach luat sinh
    * @param vTerminal         : Dau ra luu tru tap ky tu ket thuc
    * @param vNonTerminal      : Dau ra luu tru tap ky tu chua ket thuc
    * @return                  : true neu khong loi
    *                            false neu co loi
    */
    bool AnalyzeProduction(vector<string> vListProduction, vector<string> &vTerminal, vector<string> &vNonTerminal);
    //==========================================================================



    //==========================================================================
    /**
    * tim tap ben phai trong tap luat sinh
    * @param vProduction       : tap cac luat sinh
    * @param strLeft           : phan tu ben trai
    * @param vRight            : danh sach luu tru phan tu ben phai
    * @return
    */
    bool GetRight(vector<string> vProduction, string strLeft, vector<string> &vRight);
    //==========================================================================


    //==========================================================================
    /**
    * Tim tap first cua tap cac luat sinh
    * @param vProduction       : Tap cac luat sinh
    * @param vTerminal         : Tap cac ky tu ket thuc
    * @param vNonTerminal      : Tap cac ky tu chua ket thuc
    * @param mFirstSet         : map, luu tru ket qua tap first
    * @return                  : true neu khong loi
    *                            false neu co loi
    */
    bool GetFirstSetNew(vector<string> vProduction, vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, vector<string>> &mFirstSet);
    //==========================================================================



    //==========================================================================
    /**
     * Tim tap follow cua tap luat sinh
     * @param vProduction       : Tap cac luat sinh
     * @param strStart          : Ky tu bat dau
     * @param vTerminal         : Tap cac ky tu ket thuc
     * @param vNonTerminal      : Tap cac ky tu chua ket thuc
     * @param mFirstSet         : Tap First
     * @param mFollowSet        : Luu tru ket qua tap follow
     * @return                  : true neu khong loi
     *                            false neu co loi
     */
    bool GetFollowSet(vector<string> vProduction, string strStart, vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, vector<string>> mFirstSet, unordered_map<string, vector<string>> &mFollowSet);
    //==========================================================================



    //==========================================================================
    /**
     * Tim tap First cua tap cac danh ky tu khi biet tap First cho truoc
     * @param vList         : Tap cac
     * @param mFirstSet     : Tap First
     * @param vTerminal     : Tap cac ky tu ket thuc
     * @param vListResult   : Luu tru tap first thu duoc
     * @return              : true neu khong loi
     *                        false neu loi
     * VD : S-> A B C D
     * Tim tap First(ABCD)
     */
    bool GetFirstSetInList(vector<string> vList, unordered_map<string, vector<string>> mFirstSet, vector<string> vTerminal, vector<string> &vListResult);
    //==========================================================================



    //==========================================================================
    /**
     * In tap luat sinh
     * @param vProduction : Tap luat sin
     */
    void PrintProduction(vector<string> vProduction);
    //==========================================================================



    //==========================================================================
    /**
     * In map ra console
     * @param umMap : map can in
     */
    void PrintUnorderedmap(unordered_map<string, vector<string>> umMap);
    //==========================================================================



    //==========================================================================
    /**
     * Kiem tra xem 1 xau co nam trong 1 mang hay khong
     * @param strToken      : Xau can kiem tra
     * @param vTerminal     : Mang can kiem tra
     * @return              : true neu xau nam trong mang
     *                        false neu xau khong nam trong mang
     */
    bool IndexOf(string strToken, vector<string> vTerminal);
    //==========================================================================



    //==========================================================================
    /**
     * Insert mot phan tu vao map
     * @param strKey        : key
     * @param strValue      : gia tri can insert
     * @param mMap          : map can insert
     * @return              : true neu co 1 gia tri moi duoc insert
     *                        false neu khong co su thay doi nao
     */
    bool InserMap(string strKey, string strValue, unordered_map<string, vector<string>> &mMap);
    //==========================================================================



    //==========================================================================
    /**
     * Get danh sach xau tu 1 key trong map
     * @param mMap          : Map dau vao
     * @param strKey        : Key
     * @return              : tra ve con tro toi danh sach xau
     */
    vector<string> * GetListInMap(unordered_map<string, vector<string>> &mMap, string strKey);
    //==========================================================================



    //==========================================================================
    /**
     * Tien xay dung bang phan tich (ham tong hop, lay tap first, follow...)
     * @param vProduction           : Tap luat sinh
     * @param strStart              : Ky tu bat dau
     * @param vTerminal             : Tap ky tu ket thuc
     * @param vNonTerminal          : Tap ky tu chua ket thuc
     * @param mFirstSet             : Tap First
     * @param mFollowSet            : Tap Follow
     * @return                      : true neu khong loi
     *                                false neu co loi
     */
    bool PreConstructParseTable(vector<string> vProduction, string strStart, vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, vector<string>> &mFirstSet, unordered_map<string, vector<string>> &mFollowSet);
    //==========================================================================



    //==========================================================================
    /**
     * Xay dung bang phan tich
     * @return
     */
    bool ConstructParseTable();
    //==========================================================================



    //==========================================================================
    /**
     * In bang phan tich ra console
     * @param vTerminal         : Tap ky tu ket thuc
     * @param vNonTerminal      : Tap ky tu chua ket thuc
     * @param mParseTable       : Bang phan tich
     */
    void PrintParseTable(vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, unordered_map<string, string>> mParseTable);
    //==========================================================================


    bool StackToString(stack<string> sStack, string &strStack);
    bool VectorToString(vector<string> vList, string &strList);


    //==========================================================================
    /**
     * Xoa ky tu dau tien ben phai cua 1 luat sinh don
     * @param strProduction : Luat sinh can xoa
     * @return              : true neu thanh cong
     *                        false neu bi loi
     */
    bool DeleteFirstSymbol(string &strProduction);
    //==========================================================================


    //==========================================================================
    /**
     * Tao ra yeu to trai cua tap luat sinh
     * @param vProduction  : Tap luat sinh
     * @return              : true neu thanh cong
     *                        false neu co loi
     */
    bool LeftFactoring(vector<string> &vProduction);
    //==========================================================================


    //==========================================================================
    /**
     * Tao ra yeu to trai cua 1 luat sinh
     * @param strProduction : Luat sinh
     * @param vNewPro       : Tap luat sinh moi duoc tao ra
     * @return              : true neu thanh cong
     *                        false neu co loi
     */
    bool LeftFactoringSinglePro(string strProduction, vector<string> &vNewPro);
    //==========================================================================



    //==========================================================================
    /**
     * Chuan hoa xau, xoa bo khoang trang o dau va cuoi xau
     * @param strString
     * @return
     */
    bool StandardString(string &strString);
public:
    bool CheckGrammar(string strToken);


private:
    string m_strStart;
    vector<string> m_vListProduction;
    vector<string> m_vTerminal;
    vector<string> m_vNonTerminal;
    unordered_map<string, vector<string>> m_mFirstSet;
    unordered_map<string, vector<string>> m_mFollowSet;
    unordered_map<string, unordered_map<string, string>> m_mParseTable;
};


#endif //LEXICALPARSER_PARSER_H
