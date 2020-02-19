#include"EventData.h"
#include"mimosa.h"
#include"utlMessageBus.h"
#include<fstream>
#include<sstream>


void EventData::NPixels(){
  unsigned b=0;
  for(auto boardit=boards.begin();boardit!=boards.end();++boardit){
    b++;
    unsigned f=0;
    for(auto frameit=boardit->fFrames.begin();frameit!=boardit->fFrames.end();++frameit){
      f++;
      unsigned npixels=0;
      for(auto rowit=frameit->fMap.begin();rowit!=frameit->fMap.end();++rowit){
        npixels+=rowit->second.size();
      }
      std::cout<<b<<":"<<f<<"  "<<npixels<<"    ";
    }
    //std::cout<<"     ";
  }
  std::cout<<" "<<std::endl;
}














void EventData::PBM(std::string basename){
  int n=0;
  for(auto boardit=boards.begin();boardit!=boards.end();++boardit){
    for(auto frameit=boardit->fFrames.begin();frameit!=boardit->fFrames.end();++frameit){
      if(frameit->fMap.size()<10)continue;
      std::ofstream outf;
      std::stringstream fname;
      fname<<basename<<"evt"<<boardit->fHeader.eventNumber<<"-board"<<boardit->fBoardID<<"-frame"<<frameit->fFrameID<<".pbm";
      outf.open(fname.str());
      INFO("open "+fname.str());
      outf<<"P1"<<std::endl<<"928 960"<<std::endl;
      for(uint16_t row=0;row<928;++row){
        auto rowit=frameit->fMap.find(row);
        if(rowit!=frameit->fMap.end()){
          for(uint16_t col=0;col<960;++col){
            if(rowit->second.count(col)){
              outf<<"0";
            }
            else{
              outf<<"1";
            }
          }
        }else{
          for(uint16_t col=0;col<960;++col){
            outf<<"1";
          }
        }
        outf<<std::endl;
      }
      outf.close();
    }
  }
}
void EventData::DebugDump(){
  int n=0;
  for(auto it=boards.begin();it!=boards.end();++it){
    //std::cout<<"\e[1;34mBoard "<<n++<<std::endl;
    it->DebugDump();
  }
}
void EventData::Pull(){
  //std::cout<<"\e[1;34mEvent pull begin"<<std::endl;
  uint32_t tmp;
  unsigned int wordsSkept=1;

  while(fInStream.Get32()!=0xfafafafa){
    // this algorithm stops after pattern
    // 0xabcdabcd -- BoardTrailer
    // 0xfafafafa -- FrameTrailer
    do{
      while(fInStream.Get16()!=0xabcd){
        wordsSkept++;
      }
    }while(fInStream.Get16()!=0xabcd);
  }
  if(wordsSkept>0){ WARNING(std::to_string(wordsSkept)+" words skept"); }
  for(unsigned i=0;i<1;++i){
    //  tmp=fInStream.Get32();
    //  if(tmp!=i*0x10001+0x80008000){ // board header
    //    std::cerr<<"Unexpected brd header '"<<tmp<<"'"<<std::endl;
    //    throw FileFormatError();
    //  }
    boards.emplace_back(i,fInStream);
    //  tmp=fInStream.Get32();
    //  if(tmp!=i*0x10001+0x8bb08bb0){ // board trailer
    //    std::cerr<<"Unexpected brd trailer '"<<tmp<<"'"<<std::endl;
    //    throw FileFormatError();
    //  }
  }
  tmp=fInStream.Get32();
  if(tmp!=0xabcdabcd){ // event trailer
    std::cerr<<"Unexpected event trailer '"<<tmp<<"'"<<std::endl;
    throw FileFormatError();
  }
  //std::cout<<"Event pull end\e[0m"<<std::endl;
}
