#include"BoardData.h"
#include"mimosa.h"

void BoardData::DebugDump(){
    std::cout
    <<"\e[1;31m  Event Number         "<<fHeader.eventNumber<<std::endl
    <<"\e[1;31m  Trigger number FAKE  "<<fHeader.triggerNumberFake<<std::endl
    <<"\e[1;31m  Time Stamp     FAKE  "<<fHeader.timeStampFake<<std::endl ;
    int n=0;
  for(auto it=fFrames.begin();it!=fFrames.end();++it){
    std::cout<<"  \e[1;31mFrame "<<n++<<std::endl;
    it->DebugDump();
  }
}
void BoardData::Pull(){
  //std::cout<<"\e[1;31mBoard pull begin"<<std::endl;
  for(int i=0;i<1;++i) fHeader.rows[i]=fInStream.Get32();
  uint32_t tmp;
  for(int i=0;i<3;++i){
    tmp=fInStream.Get32();
    if(tmp!=0x80088007){ // frame header
      std::cerr<<"Unexpected frame header '"<<tmp<<"'"<<std::endl;
      throw FileFormatError();
    }
    fFrames.emplace_back(i,fInStream);
    tmp=fInStream.Get32();
    if(tmp!=0xaaa8aaa7){ // frame trailer
      std::cerr<<"Unexpected frame trailer '"<<tmp<<"'"<<std::endl;
      throw FileFormatError();
    }
    //std::cout<<"Board pull end\e[0m"<<std::endl;
  }
}
