#include "DocumentOperation.h"

DocumentOperation::DocumentOperation()
{
    //ctor
}

/**
    添加一个文档
*/
int DocumentOperation::AddDocument(const std::string& str_DocPath)
{
    Document* doc = new Document(str_DocPath,true);
    doc->BuildInvertedIndex();

    DocumentDao* dao = new DocumentDao();
    dao->InsertDocument(doc);

    delete dao;
    delete doc;
}

/**
    将整个目录中的文件添加到数据库中
*/
int DocumentOperation::AddDirectoryDocuments(const std::string& str_InputDir)
{
    DocumentDao* docDao = new DocumentDao();
    docDao->DeleteAll();
    delete docDao;

    std::vector<Document*> vec_Documents;
    //读取目录下所有的文件
    struct dirent *ptr;
    DIR *dir;
    dir=opendir((char *)str_InputDir.c_str());
    if(!dir)
    {
        std::cout<<"read input dir error"<<std::endl;
        return 1;
    }
    std::ofstream ofs_Log;
    ofs_Log.open("./test/log.txt",std::ios_base::out);
    int num=0;
    while((ptr=readdir(dir))!=NULL)
    {
        //跳过'.'和'..'两个目录
        if(ptr->d_name[0] == '.' || ptr->d_name[strlen(ptr->d_name)-1] == '~')
        {
            continue;
        }
        std::string str_DocPath = str_InputDir + ptr->d_name;
        Document* doc = new Document(str_DocPath,true);//分句但不分词
        vec_Documents.push_back(doc);
        num++;
        //一次
        if(num%1000==0)
        {
            num=0;
            //将文档集合添加到数据库中
            InsertDocuments(vec_Documents,ofs_Log);
            //释放所有文档资源
            for(int i=0; i<vec_Documents.size(); i++)
            {
                delete vec_Documents[i];
            }
            vec_Documents.clear();
        }
    }
    //将文档集合添加到数据库中
    InsertDocuments(vec_Documents,ofs_Log);
    //释放所有文档资源
    for(int i=0; i<vec_Documents.size(); i++)
    {
        delete vec_Documents[i];
    }
    ofs_Log.close();
    closedir(dir);
    return 0;
}
/**
    插入一个文档集合到数据库中
*/
int DocumentOperation::InsertDocuments(const std::vector<Document*>& vec_Doc,std::ofstream& ofs_Log)
{
    DocumentDao* docDao = new DocumentDao();
    for(int i=0; i<vec_Doc.size(); i++)
    {
        Document* doc = vec_Doc[i];
        const std::string str_SimilarDoc = docDao->QuerySIMSimilarity(doc);
        if(str_SimilarDoc=="")
        {
            doc->BuildInvertedIndex();
            docDao->InsertDocument(doc);
            std::cout<<doc->GetstrDocName() <<" inserted"<<std::endl;
            ofs_Log<<doc->GetstrDocName() <<" inserted"<<std::endl;
        }
        else
        {
            std::cout<<"DUPLICATE DOC TO INSERT: "<<doc->GetstrDocName()<<" is similar to "<<str_SimilarDoc<<std::endl;
            ofs_Log<<"DUPLICATE DOC TO INSERT: "<<doc->GetstrDocName()<<" is similar to "<<str_SimilarDoc<<std::endl;
        }
    }
    delete docDao;
}

DocumentOperation::~DocumentOperation()
{
    //dtor
}
