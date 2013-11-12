#ifndef HANDY_INLINES_H
#define HANDY_INLINES_H

#include <sstream>
#include <string>


template <class T>
inline std::string operator+( std::string& st, T x)
{
    std::stringstream oss;
    oss<<st<<x;
    return oss.str();
}


template <class T>
inline std::string& operator+= (std::string& st, T x)
{
    std::stringstream oss;
    oss<<x;
    return st += oss.str();

}

#endif
