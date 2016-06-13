#ifndef READCORPUS_H
#define READCORPUS_H

#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../DataStructure.h"
#include "../util/StringUtil.h"
#include "Sememe.h"

class ReadCorpus
{
    public:
        static std::map<std::string,double> map_CorpusTF;
        static std::set<std::string> set_StopTerm;
        static std::vector<Sememe> vec_Sememe;

        ReadCorpus();
        virtual ~ReadCorpus();
        static void ReadCorpusTF(const std::string& str_CorpusPath);
        static void ReadStopTerm(const std::string& str_Path);
        static void ReadSememe(const std::string str_filename);
    protected:
    private:
};

#endif // READCORPUS_H
