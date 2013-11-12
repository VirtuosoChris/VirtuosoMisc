#ifndef VIRTUOSO_PRINTF_H_INCLUDED
#define VIRTUOSO_PRINTF_H_INCLUDED


#include <iostream>

///print is a printf function that goes to an ostream
///printLn is a printf that outputs a newline
///printDel is a printf with a delimiter
///printDelLn is a printf with a newline and a delimiter
///printEndl is a printf that puts std::endl at the end of output
///printDelEndl is a delimiter separated printf that puts std::endl at the end of output


inline void print(std::ostream& os){

}

inline void printDel(std::ostream& os,unsigned  char delimiter){

}


template<typename T>
inline void print(std::ostream& str ,const T& val ){
    str<<val;
}

template<typename T, typename... Args>
inline void print( std::ostream& str,const T& val, const Args&... args)
{
    str<<val;
    print(str,args...);

}



template<typename T>
inline void printDel(std::ostream& str,unsigned char delimiter, const T& val ){
    str<<val<<delimiter;
}

template<typename T, typename... Args>
inline void printDel( std::ostream& str,unsigned char delimiter, const T& val, const Args&... args)
{
    str<<val<<delimiter;
    printDel(str,delimiter, args...);

}



template<typename T, typename... Args>
inline void printLn( std::ostream& str,const T& val, const Args&... args)
{

    print(str,val,args..., '\n');

}
template<typename T, typename... Args>
inline void printDelLn( std::ostream& str,unsigned char delimiter, const T& val, const Args&... args)
{

    printDel(str,delimiter,val,args..., '\n');

}


template<typename T, typename... Args>
inline void printEndl( std::ostream& str,const T& val, const Args&... args)
{

    print(str,val,args... );
    str<<std::endl;

}
template<typename T, typename... Args>
inline void printDelEndl( std::ostream& str,unsigned char delimiter, const T& val, const Args&... args)
{

    printDel(str,delimiter,val,args...);
    str<<std::endl;
}


#endif // VIRTUOSO_PRINTF_H_INCLUDED
