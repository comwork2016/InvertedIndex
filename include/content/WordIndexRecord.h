#ifndef WORDINDEXRECORD_H
#define WORDINDEXRECORD_H

#include <string>
#include <vector>

#include "../DataStructure.h"

// to delete
#include <iostream>

class WordIndexRecord
{
    public:
        WordIndexRecord(const DOC_ID docId);
        virtual ~WordIndexRecord();

        DOC_ID GetDocID() { return m_DocID; }

        int GetnTermFrequency() {return m_nTermFrequency; }

        std::vector<int> GetVecPos() { return m_vecPos; }


        int AddPosInfo(const int pos);

        void Display();
    protected:
    private:
        DOC_ID m_DocID;
        int m_nTermFrequency;
        std::vector<int> m_vecPos;//位置信息
};

#endif // WORDINDEXRECORD_H
