#ifndef DOCUMENTOPERATION_H
#define DOCUMENTOPERATION_H

#include "../content/Document.h"

class DocumentOperation
{
    public:
        DocumentOperation();
        virtual ~DocumentOperation();
        static int AddDocument(const std::string& str_DocPath);
    protected:
    private:
};

#endif // DOCUMENTOPERATION_H
