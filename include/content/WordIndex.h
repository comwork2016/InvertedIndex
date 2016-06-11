#ifndef WORDINDEX_H
#define WORDINDEX_H

#include <string>
#include <map>

#include "WordIndexRecord.h"
#include "../DataStructure.h"

// to delete
#include <iostream>

class WordIndex
{
    public:
        WordIndex(const std::string& str_Word, const int n_WordLength, const std::string& str_POS);
        virtual ~WordIndex();

        std::string GetstrWord(){ return m_strWord; }
        int GetnWordLength() { return m_nWordLength; }
        std::string GetstrPOS() { return m_strPOS; }
        std::map<DOC_ID,WordIndexRecord*> GetMapDocWordIndex() { return m_mapDocWordIndex; }

        int AddDocPosInfo(const DOC_ID docID, const int pos);

        void Display();
    protected:
    private:
        //WORD_ID m_WordID;
        std::string m_strWord;
        int m_nWordLength;
        std::string m_strPOS;//词性
        std::map<DOC_ID,WordIndexRecord*> m_mapDocWordIndex;//<doc_id,posInfo>
};

#endif // WORDINDEX_H
