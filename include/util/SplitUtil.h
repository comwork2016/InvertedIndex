#ifndef SPLITUTIL_H
#define SPLITUTIL_H

#include <string>
#include <map>

#include "NLPIR.h"

#include "../corpus/ReadCorpus.h"
#include "../DataStructure.h"
#include "../util/StringUtil.h"

// to delete
#include <iostream>

class SplitUtil
{
    public:
        SplitUtil();
        virtual ~SplitUtil();

        void SplitTermAndCalcTF(Sentence& sen,const std::string& str,std::map<std::string,double>& map_TF,int& n_WordCount);
        static std::vector<std::string> SplitString(const std::string& str_Source,const std::string& str_pattern);
    protected:
    private:
};

#endif // SPLITUTIL_H
