#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>

//to delete
#include <iostream>

class StringUtil
{
    public:
        StringUtil();
        virtual ~StringUtil();

        static std::string Trim(std::string& str);
        static bool isStringBlank(std::string str);
        static bool isStringBlank(std::wstring wstr);
        static std::wstring ConvertCharArraytoWString(const std::string& str);
        static std::vector<std::wstring> SplitWString(const std::wstring& wstr_Source, const std::wstring& wstr_Delims);
        static std::vector<std::string> SplitString(const std::string& str_Source,const std::string& str_pattern);

    protected:
    private:
};

#endif // STRINGUTIL_H
