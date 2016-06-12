#include "StringUtil.h"

StringUtil::StringUtil()
{
    //ctor
}

/**
    去掉字符串首尾的空白符
*/
std::string StringUtil::Trim(std::string& str)
{
    //英文
    str.erase(0,str.find_first_not_of(" \t\r\n"));
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
    //中文
    str.erase(0,str.find_first_not_of("　"));
    str.erase(str.find_last_not_of("　") + 1);
    return str;
}

/**
    将字符串转换为wstring类型
*/
std::wstring StringUtil::ConvertCharArraytoWString(const std::string& str)
{
    const char* pch = str.c_str();
    int length = strlen(pch);
    wchar_t* wc = new wchar_t[length+1];
    mbstowcs(wc, pch, length+1);
    std::wstring wstr = wc;
    delete[] wc;
    return wstr;
}

/**
    检查是不是空白行
*/
bool StringUtil::isStringBlank(std::string str)
{
    str = Trim(str);
    for(int i=0; i<str.length(); i++)
    {
        if(str[i]!=' ')
        {
            return false;
        }
    }
    return true;
}

/**
    检查是不是空白行
*/
bool StringUtil::isStringBlank(std::wstring wstr)
{
    for(int i=0; i<wstr.length(); i++)
    {
        if(wstr[i] != L' ' && wstr[i] != L' ')
        {
            return false;
        }
    }
    return true;
}

/**
    分隔宽字节字符串
    str_Source: string to be splited
    str_Delims: split separator
    return splited vector
*/
std::vector<std::wstring> StringUtil::SplitWString(const std::wstring& wstr_Source, const std::wstring& wstr_Delims)
{
    std::wstring delims = wstr_Delims;
    std::wstring wstr;
    std::vector<std::wstring> vec_wstrSplited;
    if(delims.empty())
    {
        delims = L"/n/r";
    }
    std::wstring::size_type pos=0, length = wstr_Source.size();
    while(pos < length )
    {
        wstr=L"";
        //pos指到第一个不是delims中的字符的位置
        while( (delims.find(wstr_Source[pos]) != std::wstring::npos) && (pos < length) )
        {
            ++pos;
        }
        if(pos==length)
        {
            return vec_wstrSplited;
        }
        //pos指到下一个是delims中的字符的位置
        while( (delims.find(wstr_Source[pos]) == std::wstring::npos) && (pos < length) )
        {
            wstr += wstr_Source[pos++];
        }
        if(!wstr.empty() && !isStringBlank(wstr))
        {
            vec_wstrSplited.push_back(wstr);
        }
    }
    return vec_wstrSplited;
}

/**
    分隔字符串
*/
std::vector<std::string>  StringUtil::SplitString(const std::string& str_Source,const std::string& str_pattern)
{
    std::vector<std::string> result;
    std::string str = str_Source + str_pattern;
    int strsize=str.size();
    int patternSize = str_pattern.size();
    std::string::size_type pos;
    for(int i=0; i<strsize; i++)
    {
        pos =str.find(str_pattern,i);
        if(pos!=std::string::npos && pos<strsize)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+patternSize-1;
        }
    }
    return result;
}

StringUtil::~StringUtil()
{
    //dtor
}
