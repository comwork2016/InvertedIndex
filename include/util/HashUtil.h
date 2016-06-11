#ifndef HASHUTIL_H
#define HASHUTIL_H

#include <string>

#include "StringUtil.h"
#include "../DataStructure.h"

class HashUtil
{
    public:
        HashUtil();
        virtual ~HashUtil();

        static SIMHASH_TYPE CalcStringHash(const std::string& str);
        static SIMHASH_TYPE CalcDocSimHash(const std::vector<SIMHASH_TYPE>& vec_SimHash);
        static bool IsSimHashSimilar(const SIMHASH_TYPE& l_num1, const SIMHASH_TYPE& l_num2);
    protected:
    private:
};

#endif // HASHUTIL_H
