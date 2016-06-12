#include "Sememe.h"

Sememe::Sememe()
{
    //ctor
}

Sememe::Sememe(const int i, const std::string sem, const int p)
{
    n_index = i;
    str_sememe = sem;
    n_parent = p;
}

//重载关系运算符是为了使用STL中的find()函数
bool Sememe::operator==(const Sememe & sem) const
{
    return str_sememe.compare(sem.GetstrSememe())==0;
}

bool Sememe::operator>(const Sememe & sem) const
{
    return str_sememe.compare(sem.GetstrSememe())>0;
}

bool Sememe::operator<(const Sememe & sem) const
{
    return str_sememe.compare(sem.GetstrSememe())<0;
}

Sememe::~Sememe()
{
    //dtor
}
