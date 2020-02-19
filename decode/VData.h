#ifndef __MIMOSA_VData_h__
#define __MIMOSA_VData_h__ 0
#include"InputStream.h"
#include<iostream>
class VData{
  public:
    InputStream& fInStream;
    VData(InputStream& is): fInStream(is){
      //std::cout<<__func__<<" constructor"<<std::endl;
      //Pull();
    }
    ~VData(){
      //std::cout<<__func__<<" destructor"<<std::endl;
    }
};
#endif
