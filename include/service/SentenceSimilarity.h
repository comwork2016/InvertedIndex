#ifndef SENTENCESIMILARITY_H
#define SENTENCESIMILARITY_H

#include <string>
#include <vector>

#include "WordSimilarity.h"
#include "NLPIR.h"
#include "ReadCorpus.h"

class SentenceSimilarity
{
    public:
        SentenceSimilarity();
        virtual ~SentenceSimilarity();

        static double CalcSentenceSimilarity(const std::string str_Sen1,const std::string str_Sen2);

    protected:
        static void SplitSentenceToVectors(const std::string& str,std::vector<std::string>& vec_NWord,std::vector<std::string>& vec_VWord,std::vector<std::string>& vec_AWord,std::vector<std::string>& vec_MWord,std::vector<std::string>& vec_QWord,std::vector<std::string>& vec_TWord);
        static double CalcVectorSimilarity(std::vector<std::string>& vec1,std::vector<std::string>& vec2);
    private:
};

#endif // SENTENCESIMILARITY_H
