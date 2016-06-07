#include "DocumentOperation.h"

DocumentOperation::DocumentOperation()
{
    //ctor
}

int DocumentOperation::AddDocument(const std::string& str_DocPath)
{
    Document* doc = new Document(str_DocPath,true,true);
    doc->Dispaly();
}

DocumentOperation::~DocumentOperation()
{
    //dtor
}
