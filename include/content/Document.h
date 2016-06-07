#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include "../DataStructure.h"
#include "../util/StringUtil.h"
#include "../util/SplitUtil.h"

// to delete
#include <iostream>

class Document
{
    public:
        Document(const std::string& str_DocPath,bool b_SplitToSentence = false, bool b_SplitToWords = false);
        virtual ~Document();

        void Dispaly();

    protected:
        int ReadDocumentContent();
        int ReadDocumentAndSplitToSentence();
        void SplitParaphToSentence(Paragraph& para,const std::string& str);
        void SplitSentenceToWords();
    private:

        const static int ERROR_OPENFILE = 1;
        const static int OK_READFILE = 0;

        std::string m_strDocPath;
        std::string m_strDocName;
        std::string m_strContents;
        SIMHASH_TYPE m_lSimHash;
        std::vector<Paragraph> m_vecParagraph;
        std::vector<std::string> m_vecTitleTerm;
        int m_nWordCount; //文章中的词的总数
        std::map<std::string, double> m_MapTF;//文档词频信息
};

#endif // DOCUMENT_H
