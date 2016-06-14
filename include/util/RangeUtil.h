#ifndef RANGEUTIL_H
#define RANGEUTIL_H

#include "DataStructure.h"
#include <iostream>

class RangeUtil
{
    public:
        RangeUtil();
        virtual ~RangeUtil();

        static void MergeRangeToVector(std::vector<DOCRANGETIMES>& vec_DocRangeTimes,DOCRANGETIMES& docRangeTimes);

    protected:
    private:
};

#endif // RANGEUTIL_H
