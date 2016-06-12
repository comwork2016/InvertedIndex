#ifndef DOCUMENTOPERATION_H
#define DOCUMENTOPERATION_H

#include <dirent.h>

#include "../content/Document.h"
#include "../dao/DocumentDao.h"

class DocumentOperation
{
    public:
        DocumentOperation();
        virtual ~DocumentOperation();
        static int AddDocument(const std::string& str_DocPath);
        static int AddDirectoryDocuments(const std::string& str_InputDir);
        static int InsertDocuments(const std::vector<Document*>& vec_Doc,std::ofstream& ofs_Log);
        static int SearchLeak(const std::string& str_DocPath);
    protected:
    private:
};

#endif // DOCUMENTOPERATION_H
