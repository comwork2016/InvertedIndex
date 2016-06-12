#ifndef DOCUMENTDAO_H
#define DOCUMENTDAO_H

#include "mongo/client/dbclient.h"
#include "../content/Document.h"
#include "../content/WordIndexRecord.h"

class DocumentDao
{
    public:
        DocumentDao();
        virtual ~DocumentDao();

        int InsertDocument(const Document* doc);
        int DeleteAll();
        std::string QuerySIMSimilarity(const Document* doc);
        void GetSentenceSimilarDocument(const Document* doc);

    protected:
        std::map<DOC_ID,WordIndexRecord*> QueryIndexOfWord(const std::string& str_Word);
        bool isWordIndexExists(const std::string& str_Word);
        int InsertIndexOfDocument(const Document* doc);
    private:
        mongo::DBClientConnection m_Conn;
        std::string m_Host;
        std::string m_DocDBName;
        std::string m_IndexDBName;
};

#endif // DOCUMENTDAO_H
