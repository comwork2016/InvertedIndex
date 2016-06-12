#ifndef SEMEME_H
#define SEMEME_H

#include <string>

class Sememe
{
    public:
        Sememe();
        Sememe(const int i, const std::string sem, const int p);
        virtual ~Sememe();

        int GetnIndex() const { return n_index; }
        std::string GetstrSememe() const { return str_sememe; }
        int GetnParent() const { return n_parent; }
        bool operator==(const Sememe & sem) const;
        bool operator>(const Sememe & sem) const;
        bool operator<(const Sememe & sem) const;

    protected:
    private:
        int n_index;
        std::string str_sememe;
        int n_parent;
};

#endif // SEMEME_H
