#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "NLPIR.h"

#include "../DataStructure.h"
#include "../corpus/ReadCorpus.h"
#include "../util/StringUtil.h"
#include "../util/HashUtil.h"
#include "WordIndex.h"

// to delete
#include <iostream>

class Document
{
    public:
        Document(const std::string& str_DocPath,bool b_Split = false);
        virtual ~Document();

        DOC_ID GetDocID() const { return m_DocID; }
        std::string GetstrDocPath() const { return m_strDocPath; }
        std::string GetstrDocName() const { return m_strDocName; }
        std::string GetstrContents() const { return m_strContents; }

        SIMHASH_TYPE GetlSimHash() const { return m_lSimHash; }
        SIMHASH_TYPE GetlSimHash16_1() const { return m_lSimHash16_1; }
        SIMHASH_TYPE GetlSimHash16_2() const { return m_lSimHash16_2; }
        SIMHASH_TYPE GetlSimHash16_3() const { return m_lSimHash16_3; }
        SIMHASH_TYPE GetlSimHash16_4() const { return m_lSimHash16_4; }

        std::vector<Paragraph> GetvecParagraph() const { return m_vecParagraph; }

        std::map<std::string, WordIndex*> GetMapWordIndex() const { return m_mapWordIndex; }

        void BuildInvertedIndex();

        void Display() const;

    protected:
        int ReadDocumentContent();
        int ReadDocumentAndSplitToSentence();
        void SplitParaphToSentence(Paragraph& para);
        void SplitSentenceToWords();
        void SplitTermAndCalcTF(Sentence& sen);
        void CalcDocSimHash();
    private:

        DOC_ID m_DocID;
        std::string m_strDocPath;
        std::string m_strDocName;
        std::string m_strContents;

        bool m_bSplit;//是否分词了
        SIMHASH_TYPE m_lSimHash;
        SIMHASH_TYPE m_lSimHash16_1;//simhash0-15位
        SIMHASH_TYPE m_lSimHash16_2;//simhash16-31位
        SIMHASH_TYPE m_lSimHash16_3;//simhash32-47位
        SIMHASH_TYPE m_lSimHash16_4;//simhash48-63位

        std::vector<Paragraph> m_vecParagraph;
        //std::vector<std::string> m_vecTitleTerm;
        //int m_nWordCount; //文章中的有效词的总数
        //std::map<std::string, double> m_mapTF;//文档词频信息

        //当前文档中单词的倒排索引
        std::map<std::string,WordIndex*> m_mapWordIndex;
};

#endif // DOCUMENT_H
