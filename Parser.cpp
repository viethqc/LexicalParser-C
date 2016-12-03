//
// Created by root on 29/11/2016.
//

#include "Parser.h"

#define PRODUCTION_FILE "production.txt"

CParser::CParser()
{
    if (!GetProduction(PRODUCTION_FILE))
    {
        return;
    }
    cout << "Luat sinh goc :" << endl;
    PrintProduction(m_vListProduction);
    cout << "=============================" << endl;

    cout << "Luat sinh sau khi khu de quy trai :" << endl;
    AvoidLeftRecursion(m_vListProduction);
    PrintProduction(m_vListProduction);
    cout << "=============================" << endl;

    AnalyzeProduction(m_vListProduction, m_vTerminal, m_vNonTerminal);

    vector<string> vRight;
    GetRight(m_vListProduction, "S", vRight);

    cout << "Tap First :" << endl;
    unordered_map<string, vector<string>> mFirstSet;
    GetFirstSetNew(m_vListProduction, m_vTerminal, m_vNonTerminal, mFirstSet);
    PrintUnorderedmap(mFirstSet);
    cout << "=============================" << endl;

    int a = 0;
}

CParser::~CParser()
{

}

void CParser::PrintProduction(vector<string> vProduction)
{
    for (int i = 0; i < vProduction.size(); i++)
    {
        cout << vProduction.at(i) << endl;
    }
}

void CParser::PrintUnorderedmap(unordered_map<string, vector<string>> umMap)
{
    for ( auto it = umMap.begin(); it != umMap.end(); ++it )
    {
        std::cout << it->first << " : ";
        for (int j = 0; j < it->second.size(); j++)
        {
            cout << " " << it->second.at(j);
        }
        cout << endl;
    }
    std::cout << std::endl;
}

bool CParser::GetFirstSet(vector<string> vProduction, vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, vector<string>> &mFirstSet)
{
    vector<string> *vList;
    vector<string> vTmp;
    vector<string> vToken;
    string strLeft;
    int iCount = 0;
    bool bAllIsTerminal = false;

    if (vProduction.size() == 0)
    {
        return  false;
    }

    if (vTerminal.size() == 0)
    {
        return false;
    }

    if (vNonTerminal.size() == 0)
    {
        return false;
    }

    //Tim tap first cua cac ky tu chua ket thuc
    mFirstSet.clear();
    iCount = 0;
    while (iCount < vNonTerminal.size())
    {
        iCount = 0;
        for (int i = 0; i < vNonTerminal.size(); i++)
        {
            auto autoTmp = mFirstSet.find(vNonTerminal.at(i));
            if (autoTmp == mFirstSet.end())
            {
                vList = new vector<string>();
                GetRight(m_vListProduction, vNonTerminal.at(i), vTmp);
                for (int j = 0; j < vTmp.size(); j++)
                {
                    split(vTmp.at(j), ' ', vToken);
                    if (vList->size() == 0)
                    {
                        vList->push_back(vToken.at(0));
                        continue;
                    }

                    if (IndexOf(vToken.at(0), *vList) == true)
                    {
                        continue;
                    }
                    vList->push_back(vToken.at(0));
                }

                mFirstSet.insert({vNonTerminal.at(i), *vList});
            }
            else
            {
                int j = 0;
                bAllIsTerminal = true;
                while (j < autoTmp->second.size())
                {
                    if (IndexOf(autoTmp->second.at(j), vNonTerminal) == false)
                    {
                        j++;
                        continue;
                    }

                    auto autoListToken = mFirstSet.find(autoTmp->second.at(j));
                    for (int k = 0; k < autoListToken->second.size(); k++)
                    {
                        if (IndexOf(autoListToken->second.at(k), autoTmp->second) == false)
                        {
                            autoTmp->second.push_back(autoListToken->second.at(k));
                        }
                    }

                    autoTmp->second.erase(autoTmp->second.begin() + j);
                    bAllIsTerminal = false;
                }
            }
            if (bAllIsTerminal == true)
            {
                iCount++;
            }
        }
    }

    return true;
}

bool CParser::GetFirstSetNew(vector<string> vProduction, vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, vector<string>> &mFirstSet)
{
    vector<string> vProductionExtract;
    vector<string> vListPro;
    bool bHasChange = false;

    string strLeft;
    vector<string> vRight;
    vector<string> *vList;

    int i, j;

    if (vProduction.size() == 0)
    {
        return false;
    }

    if (vTerminal.size() == 0)
    {
        return false;
    }

    if (vNonTerminal.size() == 0)
    {
        return false;
    }

    mFirstSet.clear();

    //Them tap first cho tap terminal
    for (int i = 0; i < vTerminal.size(); i++)
    {
        InserMap(vTerminal.at(i), vTerminal.at(i), mFirstSet);
    }

    for (int i = 0; i < vProduction.size(); i++)
    {
        if (SplitProToPro(vProduction.at(i), vListPro) == false)
        {
            return false;
        }

        for (int j = 0; j < vListPro.size(); j++)
        {
            vProductionExtract.push_back(vListPro.at(j));
        }
    }
    cout << "Luat sinh mo rong : " << endl;
    PrintProduction(vProductionExtract);

    //Duyet tung luat sinh cho den khi tap first khong co su thay doi thi dung lai
    do
    {
        bHasChange = false;
        i = 0;
        while (i < vProductionExtract.size())
        {
            vRight.clear();
            strLeft.clear();
            //Phan tach luat sinh
            //Vd :  S->a b thi tach thanh
            //      strLeft : S     (string)
            //      vRight : a, b;  (vector)
            cout << "Xet luat sinh : " << vProductionExtract.at(i) << endl;
            if (ParserProduction(vProductionExtract.at(i), strLeft, vRight) == false)
            {
                return false;
            }

            //Kiem tra phan tu dau tien xem co phai ky tu ket thuc hay ε hay khong.
            //Neu la ky tu ket thuc hoac ε thi luat sinh da xac dinh duoc tap first
            //Luu tru va xoa luat sinh khoi danh sach luat sinh dang xet
            if (IndexOf(vRight.at(0), vTerminal) == true || vRight.at(0).compare("ε") == 0)
            {
                InserMap(strLeft, vRight.at(0), mFirstSet);
                bHasChange = true;
                vProductionExtract.erase(vProductionExtract.begin() + i);
                continue;
            }

            //Duyet qua tung token ben phai
            //Neu token nao co tap first chua ε thi bo qua khong xet
            j = 0;
            for (j = 0; j < vRight.size(); j++)
            {
                vList = GetListInMap(mFirstSet, vRight.at(j));
                if (vList == nullptr)
                {
                    return false;
                }

                //Neu tap First(S) rong thi tam thoi bo qua khong xet
                if (vList->size() == 0)
                {
                    break;
                }

                for (int k = 0; k < vList->size(); k++)
                {
                    if (vList->at(k).compare("ε") == 0)
                    {
                        continue;
                    }

                    if (InserMap(strLeft, vList->at(k), mFirstSet) == true)
                    {
                        bHasChange = true;
                    }
                }

                //Neu tap First(S) khong chua ky tu ε thi dung lai
                if (IndexOf("ε", *vList) == false)
                {
                    break;
                }

                //Neu tat ca cac phan tu ve phai deu co tap first chua ε thi them ε vao tap S
                if (j == vRight.size() - 1)
                {
                    if (InserMap(strLeft, "ε", mFirstSet) == true)
                    {
                        bHasChange = true;
                    }
                }
            }

            i++;
        }

        PrintUnorderedmap(mFirstSet);
    }
    while (bHasChange == true);

    return true;
}

vector<string>* CParser::GetListInMap(unordered_map<string, vector<string>> &mMap, string strKey)
{
    vector<string> *vListFirst;
    if (strKey.empty())
    {
        return nullptr;
    }

    auto object = mMap.find(strKey);
    if (object == mMap.end())
    {
        vListFirst = new vector<string>();
        mMap.insert({strKey, *vListFirst});
        return vListFirst;
    }

    return &object->second;
}

bool CParser::InserMap(string strKey, string strValue, unordered_map<string, vector<string>> &mMap)
{
    vector<string> *vList;
    auto object = mMap.find(strKey);
    if (object == mMap.end())
    {
        vList = new vector<string>();
        vList->push_back(strValue);
        mMap.insert({strKey, *vList});
    }
    else
    {
        for (int i = 0; i < object->second.size(); i++)
        {
            if (strValue.compare(object->second.at(i)) == 0)
            {
                return false;
            }
        }

        object->second.push_back(strValue);
    }

    return true;
}

bool CParser::IndexOf(string strToken, vector<string> vTerminal)
{
    if (strToken.empty())
    {
        return false;
    }

    for (int i = 0; i < vTerminal.size(); i++)
    {
        if (strToken.compare(vTerminal.at(i)) == 0)
        {
            return true;
        }
    }

    return false;
}

/**
 * tim tap ben trai cua luat sinh
 * @param vProduction       : tap cac luat sinh
 * @param strLeft           : phan tu ben trai
 * @param vRight            : danh sach luu tru phan tu ben phai
 * @return
 */
bool CParser::GetRight(vector<string> vProduction, string strLeft, vector<string> &vRight)
{
    string strTmp;
    vector<string> vList;

    vRight.clear();
    if (vProduction.size() == 0)
    {
        return false;
    }

    if (strLeft.empty())
    {
        return false;
    }

    for (int i = 0; i < vProduction.size(); i++)
    {
        if (SplitProduction(vProduction.at(i), strTmp, vList) == false)
        {
            return false;
        }

        if (strTmp.compare(strLeft) != 0)
        {
            continue;
        }

        for (int j = 0; j < vList.size(); j++)
        {
            vRight.push_back(vList.at(j));
        }
        break;
    }

    return true;
}

/**
 * Tach tap luat sinh thanh 2 tap terminal va nonterminal
 * @param vListProduction   : Danh sach luat sinh
 * @param vTerminal         : Dau ra luu tru tap ky tu ket thuc
 * @param vNonTerminal      : Dau ra luu tru tap ky tu chua ket thuc
 * @return                  : true neu khong loi
 *                            false neu co loi
 */
bool CParser::AnalyzeProduction(vector<string> vListProduction, vector<string> &vTerminal, vector<string> &vNonTerminal)
{
    vector<string> vLeft;
    vector<string> vRight;
    vector<string> vRightTmp;
    string strLeft;
    int i, j;

    bool bFound = false;

    for (int i = 0; i < vListProduction.size(); i++)
    {
        if (ParserProduction(vListProduction.at(i), strLeft, vRightTmp) == false)
        {
            return false;
        }

        //Them ve trai vao danh ve trai
        bFound = false;
        if (vLeft.size() == 0)
        {
            bFound = false;
        }
        else
        {
            for (int j = 0; j < vLeft.size(); j++)
            {
                if (vLeft.at(j).compare(strLeft) == 0)
                {
                    bFound = true;
                    break;
                }
            }
        }

        if (!bFound)
        {
            vLeft.push_back(strLeft);
        }

        //them ve phai vao danh sach ve phai
        for (int j = 0; j < vRightTmp.size(); j++)
        {
            bFound = false;
            if (vRight.size() == 0)
            {
                bFound = false;
            }
            else
            {
                for (int k = 0; k < vRight.size(); k++)
                {
                    if (vRight.at(k).compare(vRightTmp.at(j)) == 0)
                    {
                        bFound = true;
                        break;
                    }
                }
            }

            if (!bFound)
            {
                vRight.push_back(vRightTmp.at(j));
            }
        }
    }

    for (int i = 0; i < vLeft.size(); i++)
    {
        for (int j = 0; j < vRight.size(); j++)
        {
            if (vLeft.at(i).compare(vRight.at(j)) == 0)
            {
                vRight.erase(vRight.begin() + j);
            }
        }
    }

    vTerminal.clear();
    vNonTerminal.clear();
    for (int i = 0; i < vRight.size(); i++)
    {
        vTerminal.push_back(vRight.at(i));
    }

    for (int i = 0; i < vLeft.size(); i++)
    {
        vNonTerminal.push_back(vLeft.at(i));
    }

    return true;
}

/**
 *
 * @param szFileName : path to production file
 * @return  true if not error
 *          else return false
 */
bool CParser::GetProduction(const char * szFileName)
{
    ifstream ifsProductionFile;
    string strLine;
    vector<string> vList;
    string strStandardLine;
    int iPos = -1;

    if (szFileName == NULL)
    {
        return false;
    }

    ifsProductionFile.open(szFileName);
    if (!ifsProductionFile.is_open())
    {
        return false;
    }

    while(!ifsProductionFile.eof()) // To get you all the lines.
    {
        getline(ifsProductionFile, strLine); // Saves the line in STRING.

        //If line is emty, continue
        if (strLine.empty())
        {
            continue;
        }

        //production end with ";" charactor
        iPos = strLine.find(';');
        if (iPos == -1)
        {
            vList = split(strLine, ' ');
            if (vList.at(0).compare("start") != 0)
            {
                return false;
            }

            for (int i = 1; i < vList.size(); i++)
            {
                if (vList.at(i).compare("") == 0)
                {
                    continue;
                }

                m_strStart = vList.at(i);
                break;
            }

            continue;
        }
        else
        {
            strStandardLine = strLine.substr(0, iPos);
            m_vListProduction.push_back(strStandardLine);
        }
    }

    ifsProductionFile.close();
    return true;
}

/**
 * Khu de quy trai
 * @param vListProduction   : danh sach luat sinh
 * @param strStart          : Ky tu bat dau
 * @param vListProductionNew: danh sach luat sinh sau khi khu de quy trai
 * @return
 */
bool CParser::AvoidLeftRecursion(vector<string> &vListProduction)
{
    string strLeft;
    vector<string> vListRight;
    vector<string> vListProductionNew;
    string strTmpProduction;
    string strNewLeft;
    string strNewRight;
    string strAddedProduction;
    string strAddLeft;
    string strAddRight;
    string strNewProduction;
    string strTmp;

    vector<string> vList;

    vListProductionNew.clear();
    for (int i = 0; i < vListProduction.size(); i++)
    {
        strTmp = vListProduction.at(i);
        for (int j = 0; j < i; j++)
        {
            ReplaceProByPro(vListProduction.at(j), strTmp);
        }

        //Neu luat sinh dang xet khong phai de quy => luu lai, khong xu ly
        if (!DetectLeftRecursion(strTmp))
        {
            vListProductionNew.push_back(vListProduction.at(i));
            continue;
        }


        if (SplitProduction(strTmp, strLeft, vListRight) == false)
        {
            continue;
        }

        strNewLeft.clear();
        strNewRight.clear();
        strAddLeft = strLeft + "'";
        strAddRight = "";
        for (int j = 0; j < vListRight.size(); j++)
        {
            strTmpProduction = strLeft + " : " + vListRight.at(j);      //Tach thanh tung luat sinh le
            if (DetectLeftRecursion(strTmpProduction) == false)         //Kiem tra tung luat sinh le xem co de quy khong
            {
                if (!strNewRight.empty())
                {
                    strNewRight += " | ";
                }

                strNewRight += vListRight.at(j) + " " + strAddLeft;
            }
            else
            {
                ReplaceProduction(strTmpProduction, strLeft, "");
                strTmpProduction.append(" " + strAddLeft);
                if(!strAddRight.empty())
                {
                    strAddRight += " | ";
                }

                vList.clear();
                split(strTmpProduction, ':', vList);
                strAddRight += vList.at(1);
            }
        }

        strNewProduction = strLeft + " : " + strNewRight;
        strAddRight.append(" | ε");
        strAddedProduction = strAddLeft + " : " + strAddRight;

        vListProductionNew.push_back(strNewProduction);
        vListProductionNew.push_back(strAddedProduction);
    }

    vListProduction.clear();
    for (int i = 0; i < vListProductionNew.size(); i++)
    {
        vListProduction.push_back(vListProductionNew.at(i));
    }

    return true;
}

bool CParser::ReplaceProByPro(string strSource, string &strDest)
{
    string strSourceLeft;
    string strDestLeft;

    vector<string> vListSource;
    vector<string> vListDest;
    string strNewRight = "";

    string strTmpSource;
    string strTmpDest;

    vector<string> vList;
    vector<string> vListTmp;

    bool bFound = false;

    if (strSource.empty())
    {
        return false;
    }

    if (strDest.empty())
    {
        return false;
    }

    if (IsValidProduction(strSource) == false)
    {
        return false;
    }

    if (IsValidProduction(strDest) == false)
    {
        return false;
    }

    if (SplitProToPro(strSource, vListSource) == false)
    {
        return false;
    }

    if (SplitProToPro(strDest, vListDest) == false)
    {
        return false;
    }

    for (int i = 0; i < vListDest.size(); i++)
    {
        for (int j = 0; j < vListSource.size(); j++)
        {
            strTmpDest = vListDest.at(i);
            strTmpSource = vListSource.at(j);
            ReplaceSingleProBySinglePro(strTmpSource, strTmpDest);
            split(strTmpDest, ':', vListTmp);
            strDestLeft = vListTmp.at(0);
            bFound = false;
            if (vList.size() != 0)
            {
                for (int k = 0; k < vList.size(); k++)
                {
                    if (vListTmp.at(1).compare(vList.at(k)) == 0)
                    {
                        bFound = true;
                        break;
                    }
                }
            }
            else
            {
                bFound = false;
            }

            if (!bFound)
            {
                vList.push_back(vListTmp.at(1));
            }
        }
    }

    strDest = strDestLeft + " : ";
    for (int i = 0; i < vList.size(); i++)
    {
        strDest.append(vList.at(i));

        if (i != vList.size() - 1)
        {
            strDest.append(" | ");
        }
    }

    return true;
}

/**
 * @function : thay the luat sinh dich boi luat sinh nguon
 *             VD : thay A->S c boi S->A b se duoc ket qua A->A b c
 * @param strSouce : luat sinh nguon
 * @param strDest  : luat sinh dich
 * @return  true if not error
 *          false if error
 */
bool CParser::ReplaceSingleProBySinglePro(string strSouce, string &strDest)
{
    string strLeftSource;
    vector<string> vRightSource;

    string strLeftDest;
    vector<string> vRightDest;

    if (strSouce.empty())
    {
        return false;
    }

    if (strDest.empty())
    {
        return false;
    }

    if (SplitProduction(strSouce, strLeftSource, vRightSource) == false)
    {
        return false;
    }
    if (vRightSource.size() != 1)
    {
        return false;
    }

    if (SplitProduction(strDest, strLeftDest, vRightDest) == false)
    {
        return false;
    }
    if (vRightDest.size() != 1)
    {
        return false;
    }

    if (ReplaceProduction(strDest, strLeftSource, vRightSource.at(0)) == false)
    {
        return false;
    }

    return true;
}

/**
 *
 * @param strProduction
 * @param strToken
 * @param strStringReplate
 * @return
 */
bool CParser::ReplaceProduction(string &strProduction, string strToken, string strStringReplate)
{
    string strLeft;
    vector<string> vListTokenRight;
    string strNewProduction = "";

    if (strProduction.empty())
    {
        return false;
    }

    if (strToken.empty())
    {
        return false;
    }

    if (ParserProduction(strProduction, strLeft, vListTokenRight) == false)
    {
        return false;
    }

    for (int i = 0; i < vListTokenRight.size(); i++)
    {
        if (vListTokenRight.at(i).compare(strToken) == 0)
        {
            vListTokenRight.at(i) = strStringReplate;
        }
    }

    strNewProduction += strLeft + " : ";
    for (int i = 0; i < vListTokenRight.size(); i++)
    {
        strNewProduction += " " + vListTokenRight.at(i);
    }

    strProduction = strNewProduction;
    return true;
}

/**
 * Ham phat hien de quy
 * @param strProduction : Production to detect recursion
 * @return      true if production is left recursion
 *              else return false
 */
bool CParser::DetectLeftRecursion(string strProduction)
{
    string strLeft;
    vector<string> vListRight;
    vector<string> vProduction;

    if (!IsValidProduction(strProduction))
    {
        return false;
    }

    if (SplitProToPro(strProduction, vProduction) == false)
    {
        return false;
    }

    for (int i = 0; i < vProduction.size(); i++)
    {
        if (!ParserProduction(vProduction.at(i), strLeft, vListRight))
        {
            return false;
        }

        if (strLeft.compare(vListRight.at(0)) == 0)
        {
            return true;
        }
    }

    //find left in list right
    if (strLeft.compare(vListRight.at(0)))

    return false;
}


/**
 * @funcion :   Tach luat sinh thanh nhieu luat sinh con
 *              VD S: a | b tach thanh S : a va S : b
 * @param strProduction : Luat sinh can tach
 * @param vListPro      : luu tru danh sach ket qua
 * @return  true if not error
 *          else return false
 */
bool CParser::SplitProToPro(string strProduction, vector<string> &vListPro)
{
    string strLeft;
    string strPro;
    vector<string> vListRight;
    if (SplitProduction(strProduction, strLeft, vListRight) == false)
    {
        return false;
    }

    vListPro.clear();
    for (int i = 0; i < vListRight.size(); i++)
    {
        strPro.clear();
        strPro.append(strLeft + " : " + vListRight.at(i));
        vListPro.push_back(strPro);
    }

    return true;
}

/**
 * Tach luat sinh
 * VD : SplitProduction(S->a b | c) se tach thanh
 *      strLeft : S
 *      vListRight : a b, c
 * @param strProduction : luat sinh
 * @param strLeft       : phan ben trai
 * @param vListRight    : danh sach phan ben phai
 * @return
 */
bool CParser::SplitProduction(string strProduction, string &strLeft, vector<string> &vListRight)
{
    vector<string> vListLeft;
    vector<string> vParts;
    int j = 0;

    vListRight.clear();
    if (strProduction.empty())
    {
        return false;
    }

    if (!IsValidProduction(strProduction))
    {
        return false;
    }

    //chia luat sinh thanh 2 phan left, right boi dau ":"
    split(strProduction, ':', vParts);
    if (vParts.size() != 2)
    {
        return false;
    }

    //Doc ky tu left
    split(vParts[0], ' ', vListLeft);
    for (int i = 0; i < vListLeft.size(); i++)
    {
        if (vListLeft.at(i).empty())
        {
            vListLeft.erase(vListLeft.begin() + i);
        }
    }
    if (vListLeft.size() != 1)
    {
        return false;
    }
    strLeft = vListLeft.at(0);

    //Doc ky tu right
    split(vParts[1], '|', vListRight);
    for (int i = 0; i < vListRight.size(); i++)
    {
        while (vListRight.at(i)[0] == ' ')
        {
            vListRight.at(i).erase(vListRight.at(i).begin());
        }

        while (vListRight.at(i).back() == ' ')
        {
            vListRight.at(i).erase(vListRight.at(i).begin() + vListRight.at(i).size() - 1);
        }
    }

    return true;
}


/**
 * Kiem tra su hop le cua luat sinh
 * @param strProduction : production to check
 * @return  true if production is valid
 *          else return false
 */
bool CParser::IsValidProduction(string strProduction)
{
    if (strProduction.empty()) {
        return false;
    }

    if (split(strProduction, ':').size() != 2)
    {
        return false;
    }

    return true;
}

/**
 * Tach luat sinh thanh ve trai, va tap cac token ve phai
 * VD : S-> a b | c se tach thanh
 *      strLeft : S
 *      vListRight : a, b, c
 * @param strProduction     : Production to parse
 * @param strLeft           : variable to store left value
 * @param vListRight        : list string of right value
 * @return                  true if not error
 *                          else return false
 */
bool CParser::ParserProduction(string strProduction, string & strLeft, vector<string> &vListRight)
{
    vector<string> vList;
    int iPos = -1;
    int i = -1;

    vListRight.clear();
    if (!IsValidProduction(strProduction))
    {
        return false;
    }

    split(strProduction, ':', vList);
    if (vList.size() != 2)
    {
        return false;
    }

    strLeft = vList.at(0);
    do
    {
        iPos = strLeft.find(" ");
        if (iPos < 0)
        {
            continue;
        }

        strLeft.erase(iPos, 1);

    }while (iPos >= 0);

    //split right and get all token
    split(vList.at(1), ' ', vListRight);
    i = 0;
    while (i < vListRight.size())
    {
        if (vListRight.at(i).empty() || vListRight.at(i).compare("|") == 0)
        {
            vListRight.erase(vListRight.begin() + i);
        }
        else
        {
            i++;
        }

    }

    return true;
}

/**
 * Ham tach xau theo ky tu cho truoc
 * @param s
 * @param delim
 * @param elems
 */
void CParser::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;

    elems.clear();
    while (getline(ss, item, delim))
    {
        elems.push_back(item);
    }
}

vector<std::string> CParser::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}