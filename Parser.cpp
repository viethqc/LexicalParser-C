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

    cout << "Left Factoring :" << endl;
    vector<string> vList;
    LeftFactoringSinglePro(m_vListProduction.at(0), vList);
    cout << "=============================" << endl;

    cout << "Luat sinh sau khi khu de quy trai :" << endl;
    AvoidLeftRecursion(m_vListProduction);
    PrintProduction(m_vListProduction);
    cout << "=============================" << endl;

    if (AnalyzeProduction(m_vListProduction, m_vTerminal, m_vNonTerminal) == false)
    {
        return;
    }
    cout << "Tap ky hieu ket thuc :" << endl;
    PrintProduction(m_vTerminal);
    cout << "=============================" << endl;

    cout << "Tap ky hieu chua ket thuc :" << endl;
    PrintProduction(m_vNonTerminal);
    cout << "=============================" << endl;

    if (PreConstructParseTable(m_vListProduction, m_strStart, m_vTerminal, m_vNonTerminal, m_mFirstSet, m_mFollowSet) == false)
    {
        return;
    }
    cout << "Tap First :" << endl;
    PrintUnorderedmap(m_mFirstSet);
    cout << "=============================" << endl;
    cout << "Tap Follow :" << endl;
    PrintUnorderedmap(m_mFollowSet);
    cout << "=============================" << endl;

    if (ConstructParseTable() == false)
    {
        return;
    }
    int a = 0;
}

CParser::~CParser()
{

}

bool CParser::ConstructParseTable()
{
    vector<string> vListPro;
    vector<string> vProductionExtract;
    vector<string> vRight;
    vector<string> vFirstSet;
    vector<string> vFollowSet;

    string strLeft;

    if (m_mFirstSet.size() == 0)
    {
        return false;
    }

    if (m_mFollowSet.size() == 0)
    {
        return false;
    }

    if (m_mFirstSet.size() == 0)
    {
        return false;
    }

    if (m_strStart.empty())
    {
        return false;
    }

    if (m_vTerminal.size() == 0)
    {
        return false;
    }

    if (m_vNonTerminal.size() == 0)
    {
        return false;
    }

    m_mParseTable.clear();
    for (int i = 0; i < m_vListProduction.size(); i++)
    {
        if (SplitProToPro(m_vListProduction.at(i), vListPro) == false)
        {
            return false;
        }

        for (int j = 0; j < vListPro.size(); j++)
        {
            vProductionExtract.push_back(vListPro.at(j));
        }
    }

    for (int i = 0; i < vProductionExtract.size(); i++)
    {
        vFirstSet.clear();
        strLeft.clear();
        vRight.clear();

        if (ParserProduction(vProductionExtract.at(i), strLeft, vRight) == false)
        {
            return false;
        }

        //Tim tap first cua ve phai
        if (GetFirstSetInList(vRight, m_mFirstSet, m_vTerminal, vFirstSet) == false)
        {
            return false;
        }

        for (int j = 0; j < vFirstSet.size(); j++)
        {
            if (vFirstSet.at(j).compare("ε") == 0)
            {
                continue;
            }

            m_mParseTable[strLeft][vFirstSet.at(j)] = vProductionExtract.at(i);
        }

        if (IndexOf("ε", vFirstSet) == true)
        {
            vFollowSet = m_mFollowSet[strLeft];
            for (int j = 0; j < vFollowSet.size(); j++)
            {
                m_mParseTable[strLeft][vFollowSet.at(j)] = vProductionExtract.at(i);
            }
        }
    }

    PrintParseTable(m_vTerminal, m_vNonTerminal, m_mParseTable);

    return true;
}

bool CParser::CheckGrammar(string strToken)
{
    vector<string> vToken;
    stack<string> sStack;
    vector<string> vList;
    string strLeft;
    string strTop;
    string strStack;
    string strVector;
    string strAction;
    int i = 0;
    bool bResult = false;

    cout << "===================================";
    cout << "PHAN TICH CU PHAP" << endl;

    if (strToken.empty())
    {
        return false;
    }

    //Chuan hoa xau dau vao
    split(strToken, ' ', vToken);
    i = 0;
    while (i < vToken.size())
    {
        if (vToken.at(i).compare("") == 0)
        {
            vToken.erase(vToken.begin() + i);
            continue;
        }

        i++;
    }
    vToken.push_back("$");

    cout << "Input : ";
    for (int i = 0; i < vToken.size(); i++)
    {
        cout << vToken.at(i) << " ";
    }
    cout << endl;

    //Phan tich cu phap
    sStack.push("$");
    sStack.push(m_strStart);

    printf("%-30s%-30s%-30s\n", "Stack", "Input", "Action");
    while (sStack.empty() == false && vToken.size() != 0)
    {
        StackToString(sStack, strStack);
        VectorToString(vToken, strVector);
        strAction.clear();
        strTop = sStack.top();
        if (strTop.compare("$") == 0 && vToken.at(0).compare("$") == 0)
        {
            return true;
        }

        if (IndexOf(strTop, m_vTerminal) == true && strTop.compare(vList.at(0)) != 0)
        {
            return false;
        }

//        if (IndexOf(strTop, m_vTerminal) == true && strTop.compare(vToken.at(0)) != 0)
//        {
//            return false;
//        }

        if (strTop.compare(vToken.at(0)) == 0 && strTop.compare("$") != 0)
        {
            strAction = "pop " + sStack.top() + " and ip++";
            printf("%-30s%-30s%-30s\n", strStack.data(), strVector.data(), strAction.data());
            sStack.pop();
            vToken.erase(vToken.begin());
            continue;
        }

        if (IndexOf(strTop, m_vNonTerminal) == true)
        {
            if (m_mParseTable[strTop][vToken.at(0)].compare("") == 0)
            {
                return false;
            }

            strAction += "expand by " + m_mParseTable[strTop][vToken.at(0)];
            printf("%-30s%-30s%-30s\n", strStack.data(), strVector.data(), strAction.data());
            sStack.pop();

            if (ParserProduction(m_mParseTable[strTop][vToken.at(0)], strLeft, vList) == false)
            {
                return false;
            }

            if (vList.size() == 0)
            {
                return false;
            }
            else if (vList.size() == 1)
            {
                if (vList.at(0).compare("ε") != 0)
                {
                    sStack.push(vList.at(0));
                }
            }
            else
            {
                for (int i = vList.size() - 1; i >= 0; i--)
                {
                    if (vList.at(0).compare("ε") != 0)
                    {
                        sStack.push(vList.at(i));
                    }
                }
            }
        }
    }

    return false;
}

bool CParser::StackToString(stack<string> sStack, string &strStack)
{
    strStack.clear();
    while (sStack.empty() == false)
    {
        strStack.append(sStack.top() + " ");
        sStack.pop();
    }

    return true;
}

bool CParser::VectorToString(vector<string> vList, string &strList)
{

    strList.clear();
    for (int i = 0; i < vList.size(); i++)
    {
        strList.append(vList.at(i) + " ");
    }

    return true;
}

void CParser::PrintParseTable(vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, unordered_map<string, string>> mParseTable)
{
    vector<string> vList;

    for (int i = 0; i < vTerminal.size(); i++)
    {
        if (vTerminal.at(i).compare("ε") == 0)
        {
            continue;
        }
        vList.push_back(vTerminal.at(i));
    }
    vList.push_back("$");

    printf("%-15s", "");
    for (int i = 0; i < vList.size(); i++)
    {
        printf("%-15s", vList.at(i).data());
    }
    cout << endl;

    for (int i = 0; i < vNonTerminal.size(); i++)
    {
        printf("%-15s", vNonTerminal.at(i).data());
        for (int j = 0; j < vList.size(); j++)
        {
            printf("%-15s", mParseTable[vNonTerminal.at(i)][vList.at(j)].data());
        }
        cout << endl;
    }
}

bool CParser::PreConstructParseTable(vector<string> vProduction, string strStart, vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, vector<string>> &mFirstSet, unordered_map<string, vector<string>> &mFollowSet)
{
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

    if (GetFirstSetNew(vProduction, vTerminal, vNonTerminal, mFirstSet) == false)
    {
        return false;
    }

    if (GetFollowSet(vProduction, strStart, vTerminal, vNonTerminal, mFirstSet, mFollowSet) == false)
    {
        return false;
    }

    return true;
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

#ifdef _DEBUG
    cout << "Luat sinh mo rong : " << endl;
    PrintProduction(vProductionExtract);
#endif

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
#ifdef _DEBUG
            cout << "Xet luat sinh : " << vProductionExtract.at(i) << endl;
#endif
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

bool CParser::GetFollowSet(vector<string> vProduction, string strStart, vector<string> vTerminal, vector<string> vNonTerminal, unordered_map<string, vector<string>> mFirstSet, unordered_map<string, vector<string>> &mFollowSet)
{
    vector<string> vProductionExtract;
    vector<string> vListPro;
    vector<string> vRight;
    vector<string> vRightBeta;
    vector<string> vFirstSet;

    string strBeta;

    bool bHasChange = false;

    string strLeft;

#ifdef _DEBUG
    cout << "======================" << endl;
    cout << "Start find follow set" << endl;
#endif

    if (vProduction.size() == 0)
    {
        return false;
    }

    if (strStart.empty())
    {
        return false;
    }

    if (vTerminal.empty())
    {
        return false;
    }

    if (vNonTerminal.empty())
    {
        return false;
    }

    if (mFirstSet.size() == 0)
    {
        return false;
    }

    //Them $ vao tap follow cua ky tu bat dau
    InserMap(strStart, "$", mFollowSet);

    //Tach luat sinh thanh cac luat sinh don
    //VD : S -> a | b thi tach thanh S-> a va S->b;
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
#ifdef _DEBUG
    cout << "Luat sinh mo rong : " << endl;
    PrintProduction(vProductionExtract);
#endif

    do
    {
        bHasChange = false;
        //Duyet qua tung luat sinh, ap dung luat first de them vao tap follow
        for (int i = 0; i < vProductionExtract.size(); i++)
        {
            if (ParserProduction(vProductionExtract.at(i), strLeft, vRight) == false)
            {
                return false;
            }

            for (int j = 0; j < vRight.size(); j++)
            {
                strBeta.clear();
                vRightBeta.clear();

                strBeta = vRight.at(j);
                if (IndexOf(strBeta, vTerminal) == true)  //Bo qua khong xet cac ky tu ket thuc
                {
                    continue;
                }
                vRightBeta.insert(vRightBeta.begin(), vRight.begin() + j + 1, vRight.end());

                GetFirstSetInList(vRightBeta, mFirstSet, vTerminal, vFirstSet);

                //Ap dung luat 3
                if (vFirstSet.size() == 0)
                {
                    auto object = mFollowSet.find(strLeft);
                    if (object == mFollowSet.end())
                    {
                        continue;
                    }

                    for (int k = 0; k < object->second.size(); k++)
                    {
                        if (InserMap(strBeta, object->second.at(k), mFollowSet) == true)
                        {
                            bHasChange = true;
                        }
                    }
                }
                else
                {
                    //Ap dung luat 2
                    for (int k = 0; k < vFirstSet.size(); k++)
                    {
                        if (vFirstSet.at(k).compare("ε") == 0) {
                            continue;
                        }

                        if (InserMap(strBeta, vFirstSet.at(k), mFollowSet) == true)
                        {
                            bHasChange = true;
                        }
                    }

                    //Ap dung luat 4
                    if (IndexOf("ε", vFirstSet) == true)
                    {
                        auto object = mFollowSet.find(strLeft);
                        if (object == mFollowSet.end())
                        {
                            continue;
                        }
                        for (int k = 0; k < object->second.size(); k++)
                        {
                            if (InserMap(strBeta, object->second.at(k), mFollowSet) == true)
                            {
                                bHasChange = true;
                            }
                        }
                    }
                }
            }
        }
    }while (bHasChange == true);

#ifdef _DEBUG
    cout << endl << "Tap follow" << endl;
    PrintUnorderedmap(mFollowSet);
#endif

    return true;
}

/**
 * Tim tap first cua 1 tap cho truoc
 * @param vList
 * @param mFirstSet
 * @param vListResult
 * @return
 */
bool CParser::GetFirstSetInList(vector<string> vList, unordered_map<string, vector<string>> mFirstSet, vector<string> vTerminal, vector<string> &vListResult)
{
    int i = 0;

    vListResult.clear();
    if (vList.size() == 0)
    {
        return true;
    }

    if (mFirstSet.size() == 0)
    {
        return false;
    }

    i = 0;
    for (i = 0; i < vList.size(); i++)
    {
        if (IndexOf(vList.at(i), vTerminal) == true)
        {
            vListResult.push_back(vList.at(i));
            break;
        }

        auto object = mFirstSet.find(vList.at(i));
        if (object == mFirstSet.end())
        {
            return false;
        }

        for (int j = 0; j < object->second.size(); j++)
        {
            if (IndexOf(object->second.at(j), vListResult) == true)
            {
                continue;
            }

            //Bo qua khong them ε
            if (object->second.at(j).compare("ε") == 0)
            {
                continue;
            }

            vListResult.push_back(object->second.at(j));
        }

        //Duyet cho den khi tim thay tap first nao khong co ky tu ε
        if (IndexOf("ε", object->second) == false)
        {
            break;
        }
    }

    //Neu tat ca cac tap first cua cac phan tu deu chua ε thi them ε vao tap first can tim
    if (i == vList.size())
    {
        vListResult.push_back("ε");
    }

    return true;
}

/**
 * Lay danh sach tu key dau vao
 * @param mMap
 * @param strKey
 * @return
 */
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

/**
 * Chen key , value vao map
 * @param strKey        : key
 * @param strValue      : value
 * @param mMap          : map can chen
 * @return              : true neu khong loi
 *                      : false neu co loi
 */
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

bool CParser::LeftFactoring(vector<string> &vProduction)
{
    string strLeft;
    vector<string> vRight, *pvTmp;
    map<string, vector<int> *> mMap;
    map<int, vector<string> * > mMapIndex;
    vector<string> vSymbol;
    string strAddLeft;
    string strAddRight;
    string strNewRight;
    string strAddPro;
    string strNewPro;
    string strTmp1, strTmp2;
    bool bSame;
    int iIndex;

    if (vProduction.size() == 0)
    {
        return false;
    }

    try
    {
        for (int i = 0; i < vProduction.size(); i++)
        {
            if (SplitProduction(vProduction.at(i), strLeft, vRight) == false)
            {
                return false;
            }

            for (int j = 0; j < vRight.size(); j++)
            {
                split(vRight.at(j), ' ', vSymbol);
                if (vSymbol.size() == 0)
                {
                    return false;
                }
                if (mMapIndex[j] == nullptr)
                {
                    mMapIndex[j] = new vector<string>;
                }
                for (int k = 0; k < vSymbol.size(); k++)
                {
                    mMapIndex[j]->push_back(vSymbol.at(k));
                }

                if (mMap[vSymbol[0]] == nullptr)
                {
                    mMap[vSymbol[0]] = new vector<int>();
                }

                mMap[vSymbol[0]]->push_back(j);
            }

            strNewRight = "";
            strAddLeft = strLeft;
            for (std::map<string, vector<int> *>::iterator it = mMap.begin(); it != mMap.end(); ++it)
            {
                strAddLeft.append("*");
                //Tim phan tu chung trong 1 tap, vd ab, abc, abd thi can tim ab la phan tu chung
                bSame = true;                                  //bien nhan dien phan tu chung
                do
                {
                    bSame = true;
                    //Duyet cac ky tu dau tien cua tap
                    for (int j = 0; j < it->second->size() - 1; j++)
                    {
                        if (mMapIndex[it->second->at(j)]->size() == 0 || mMapIndex[it->second->at(j + 1)]->size() == 0)
                        {
                            bSame = false;
                            break;
                        }
                        strTmp1 = mMapIndex[it->second->at(j)]->at(0);
                        strTmp2 = mMapIndex[it->second->at(j + 1)]->at(0);
                        if (strTmp1.compare(strTmp2) != 0)
                        {
                            bSame = false;
                        }
                    }

                    //Neu cac ky tu dau tien cua tap deu giong nhau thi luu lai va xoa phan tu dau tien
                    //de xet phan tu ke tiep
                    if (bSame == true)
                    {
                        strNewRight += strTmp1;
                        for (int j = 0; j < it->second->size(); j++)
                        {
                            pvTmp = mMapIndex[it->second->at(j)];
                            pvTmp->erase(pvTmp->begin());
                        }
                    }
                }
                while (bSame == true);
                strNewRight += strAddLeft;

                //Them vao ve phai luat sinh moi
                for (int j = 0; j < it->second->size(); j++)
                {
                    iIndex = it->second->at(j);
                    if (mMapIndex[iIndex]->size() == 0)
                    {
                        strAddRight += "ε ";
                    }
                    else
                    {
                        for (int k = 0; k < mMapIndex[iIndex]->size(); k++)
                        {
                            strAddRight += mMapIndex[iIndex]->at(k) + " ";
                        }
                    }

                    if (j != it->second->size() - 1)
                    {
                        strAddRight += " | ";
                    }
                }
            }

            strNewPro = strLeft + strNewRight;
            strAddPro = strAddLeft + strAddRight;
        }
    } catch (CReleaseEvent e)
    {
        return e.GetReturn();
    }
}

bool CParser::LeftFactoringSinglePro(string strProduction, vector<string> &vNewPro)
{
    string strLeft;
    vector<string> vRight, *pvTmp;
    map<string, vector<int> *> mMap;
    map<int, vector<string> * > mMapIndex;
    vector<string> vSymbol;
    string strAddLeft;
    string strAddRight;
    string strNewRight;
    string strAddPro;
    string strNewPro;
    string strTmp1, strTmp2;
    bool bSame;
    int iIndex;

    vector<string> vAddPro;

    if (strProduction.empty())
    {
        return false;
    }

    if (IsValidProduction(strProduction) == false)
    {
        return false;
    }

    if (SplitProduction(strProduction, strLeft, vRight) == false)
    {
        return false;
    }

    for (int j = 0; j < vRight.size(); j++)
    {
        split(vRight.at(j), ' ', vSymbol);
        if (vSymbol.size() == 0)
        {
            return false;
        }
        if (mMapIndex[j] == nullptr)
        {
            mMapIndex[j] = new vector<string>;
        }
        for (int k = 0; k < vSymbol.size(); k++)
        {
            mMapIndex[j]->push_back(vSymbol.at(k));
        }

        if (mMap[vSymbol[0]] == nullptr)
        {
            mMap[vSymbol[0]] = new vector<int>();
        }

        mMap[vSymbol[0]]->push_back(j);
    }

    strNewRight = "";
    strAddLeft = strLeft;
    for (std::map<string, vector<int> *>::iterator it = mMap.begin(); it != mMap.end(); ++it)
    {
        //Neu chi co 1 phan tu thi
        if (it->second->size() == 1)
        {
            strNewRight += " " + it->first;
            if (next(it) != mMap.end())
            {
                strNewRight.append(" | ");
            }

            continue;
        }

        strAddLeft.append("*");
        strAddRight.clear();
        //Tim phan tu chung trong 1 tap, vd ab, abc, abd thi can tim ab la phan tu chung
        bSame = true;                                  //bien nhan dien phan tu chung
        do
        {
            bSame = true;
            //Duyet cac ky tu dau tien cua tap
            for (int j = 0; j < it->second->size() - 1; j++)
            {
                if (mMapIndex[it->second->at(j)]->size() == 0 || mMapIndex[it->second->at(j + 1)]->size() == 0)
                {
                    bSame = false;
                    break;
                }
                strTmp1 = mMapIndex[it->second->at(j)]->at(0);
                strTmp2 = mMapIndex[it->second->at(j + 1)]->at(0);
                if (strTmp1.compare(strTmp2) != 0)
                {
                    bSame = false;
                }
            }

            //Neu cac ky tu dau tien cua tap deu giong nhau thi luu lai va xoa phan tu dau tien
            //de xet phan tu ke tiep
            if (bSame == true)
            {
                strNewRight += strTmp1;
                for (int j = 0; j < it->second->size(); j++)
                {
                    pvTmp = mMapIndex[it->second->at(j)];
                    pvTmp->erase(pvTmp->begin());
                }
            }
        }
        while (bSame == true);

        strNewRight += " " + strAddLeft;
        if (next(it) != mMap.end())
        {
            strNewRight.append(" | ");
        }

        //Them vao ve phai luat sinh moi
        for (int j = 0; j < it->second->size(); j++)
        {
            iIndex = it->second->at(j);
            if (mMapIndex[iIndex]->size() == 0)
            {
                strAddRight += "ε ";
            }
            else
            {
                for (int k = 0; k < mMapIndex[iIndex]->size(); k++)
                {
                    strAddRight += mMapIndex[iIndex]->at(k) + " ";
                }
            }

            if (j != it->second->size() - 1)
            {
                strAddRight += " | ";
            }
        }

        strAddPro = strAddLeft + " : " + strAddRight;
        vAddPro.push_back(strAddPro);
    }

    strNewPro = strLeft + " : " + strNewRight;

    vNewPro.push_back(strNewPro);


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
                DeleteFirstSymbol(strTmpProduction);
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

bool CParser::DeleteFirstSymbol(string &strProduction)
{
    string strLeft;
    vector<string> vList;

    if (IsValidProduction(strProduction) == false)
    {
        return false;
    }

    if (ParserProduction(strProduction, strLeft, vList) == false)
    {
        return false;
    }

    if (vList.size() == 1)
    {
        return false;
    }

    strProduction = "";
    strProduction.append(strLeft + " : ");
    for (int i = 1; i < vList.size(); i++)
    {
        strProduction.append(vList.at(i) + " ");
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