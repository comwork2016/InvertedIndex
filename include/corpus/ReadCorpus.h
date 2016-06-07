#ifndef READCORPUS_H
#define READCORPUS_H

#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../DataStructure.h"
#include "../util/SplitUtil.h"
#include "../util/StringUtil.h"

class ReadCorpus
{
    public:
        static std::map<std::string,double> map_CorpusTF;
        static std::set<std::string> set_StopTerm;

        ReadCorpus();
        virtual ~ReadCorpus();
        static void ReadCorpusTF(const std::string& str_CorpusPath);
        static void ReadStopTerm(const std::string& str_Path);
    protected:
    private:
};

#endif // READCORPUS_H
