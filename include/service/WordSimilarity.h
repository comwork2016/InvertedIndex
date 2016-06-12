#ifndef WORDSIMILARITY_H
#define WORDSIMILARITY_H

#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>

#include "../util/StringUtil.h"
#include "Sememe.h"
#include "GlossaryDao.h"
#include "DataStructure.h"

//to delete
#include <iostream>

class WordSimilarity
{
    public:
        WordSimilarity();
        virtual ~WordSimilarity();
        double CalcWordSimilarity(const std::string str_word1,const std::string str_word2);
    protected:
        void InitSemVec(const std::string str_filename);
        double CalConceptSim(const std::string str_concept1,const std::string str_concept2);
        double CalSimBase(const std::string str_sem1,const std::string str_sem2);
        double CalSimReal(std::string str_sem1,std::string str_sem2);
        double CalSim1(const std::string str_line1, const std::string str_line2);
        double CalSim2(const std::string str_line1, const std::string str_line2);
        double CalSim3(const std::string str_line1, const std::string str_line2);
        double CalSim4(const std::string str_line1, const std::string str_line2);
    private:
        GlossaryDao* dao;
        std::vector<Sememe> vec_Sememe;
};

#endif // WORDSIMILARITY_H
