#ifndef GLOSSARYDAO_H
#define GLOSSARYDAO_H

#include <string>
#include <fstream>
#include "mongo/client/dbclient.h"

class GlossaryDao
{
    public:
        GlossaryDao();
        virtual ~GlossaryDao();

        int ReadGlossaryToDB(const std::string str_path);
        int Insert(const std::string str_word, const std::string str_pos,const std::string str_sem);
        int RemoveAll();
        std::vector<std::string> SelectSememe(const std::string str);
    protected:
    private:
        mongo::DBClientConnection m_Conn;
        std::string m_Host;
        std::string m_DBName;
};

#endif // GLOSSARYDAO_H
