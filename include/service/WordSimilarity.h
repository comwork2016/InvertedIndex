#ifndef WORDSIMILARITY_H
#define WORDSIMILARITY_H

#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "../util/StringUtil.h"
#include "DataStructure.h"
#include "ReadCorpus.h"

//to delete
#include <iostream>

class WordSimilarity
{
    public:
        WordSimilarity();
        virtual ~WordSimilarity();
        static double CalcWordSimilarityByConcepts(const std::vector<std::string> vec1,const std::vector<std::string> vec2);
    protected:
        static double CalConceptSim(const std::string str_concept1,const std::string str_concept2);
        static double CalSimBase(const std::string str_sem1,const std::string str_sem2);
        static double CalSimReal(std::string str_sem1,std::string str_sem2);
        static double CalSim1(const std::string str_line1, const std::string str_line2);
        static double CalSim2(const std::string str_line1, const std::string str_line2);
        static double CalSim3(const std::string str_line1, const std::string str_line2);
        static double CalSim4(const std::string str_line1, const std::string str_line2);
    private:
};

#endif // WORDSIMILARITY_H
