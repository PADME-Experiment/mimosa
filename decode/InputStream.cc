#include"InputStream.h"
#include"mimosa.h"
#include<cstdlib> // strtol
#include<cstring>
#include<iostream>


#include"FrameData.h"
#include <bitset>
#include <iomanip>
#include <utility>


InputStream::InputStream(const std::string& filename){
  filep.open(filename);
  if(! filep.is_open()){
    std::cerr<<"problem with file '"<<filename<<"'"<<std::endl;
    throw FileCannotBeOpen();
  }
}

InputStream::~InputStream(){
  filep.close();
}

void InputStream::Dump(){
  char tmp[100];
  int l=0;
  while ( ! filep.eof()){
    filep.getline(tmp,100);
    long int lll=strtol(tmp,NULL,16);
    if(strlen(tmp)!=8){
      std::cerr<<"unexpected length "<<strlen(tmp)<<std::endl;
    }
    printf("%X  ", lll);
    std::cout<< l++<<"  "<<tmp<<"  "<<strlen(tmp)<<std::endl;
  }
}

uint32_t InputStream::Get32(){
  std::string tmp;
  char c;
  while (tmp.length()<8){
    if(!filep.get(c))throw FileFinishedUnexpectedly();
    if(c=='\n'||c=='\r')
      if(!filep.get(c))throw FileFinishedUnexpectedly();
    tmp+=c;
  }
  //std::cerr<<tmp<<std::endl;
  return strtol(tmp.c_str(),NULL,16);
}

uint16_t InputStream::Get16(){
  std::string tmp;
  char c;
  while (tmp.length()<4){
    if(!filep.get(c))throw FileFinishedUnexpectedly();
    if(c=='\n'||c=='\r')
      if(!filep.get(c))throw FileFinishedUnexpectedly();
    tmp+=c;
  }
  uint16_t ret=strtol(tmp.c_str(),NULL,16);
  //std::cerr<<tmp<<std::endl;

    //static unsigned nr=0;
    //std::cout<<"\e[1;33m"<< std::setfill('0')<< std::setw(4) << std::hex <<ret<<"        "
    //  << std::setfill('0')<< std::setw(1) << std::hex << ((FrameData::status_line_t*)&ret)->nStates<<"   "
    //  << std::setfill('0')<< std::setw(3) << std::hex << ((FrameData::status_line_t*)&ret)->line   <<"   "
    //  << std::setfill('0')<< std::setw(1) << std::hex << ((FrameData::status_line_t*)&ret)->zero   <<"   "
    //  << std::setfill('0')<< std::setw(1) << std::hex << ((FrameData::status_line_t*)&ret)->ovf    <<"         "
    //  << std::setfill('0')<< std::setw(1) << std::hex << ((FrameData::state_t*)&ret)->nHit   <<"   "
    //  << std::setfill('0')<< std::setw(3) << std::hex << ((FrameData::state_t*)&ret)->column <<"   "
    //  << std::setfill('0')<< std::setw(1) << std::bitset<4>(((FrameData::state_t*)&ret)->zeros)<<"  " <<(nr%2==0?"0":"1")<<"  "<<nr
    //  <<"\e[0m"<<std::endl;
    //  nr++;
  return ret;
}
