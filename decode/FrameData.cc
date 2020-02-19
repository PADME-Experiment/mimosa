#include"FrameData.h"
#include"mimosa.h"
#include"utlMessageBus.h"
#include <bitset>
#include <iomanip>
#include <utility>
void FrameData::DebugDump(){
    std::cout
    <<"\e[1;32m    Trigger number "<<fHeader.triggerNumber<<std::endl
    <<"\e[1;32m    Time Stamp     "<<fHeader.timeStamp<<std::endl
    <<"\e[1;32m    Frame Count    "<<fHeader.frameCount<<std::endl
    <<"\e[1;32m    Data Length    "<<fHeader.dataLength1<<"  "<<fHeader.dataLength2<<std::endl;
    for(auto mapit=fMap.begin();mapit!=fMap.end();++mapit){
      std::cout<<(int)mapit->first<<"  ";
      for(auto listit=mapit->second.begin();listit!=mapit->second.end();++listit){
        std::cout<<"   "<<(int)*listit;
      }
      std::cout<<std::endl;
    }
    //int i=0;
    //for(int ii=0;ii<fStatus.size();++ii){
    //  std::cout
    //  << std::setw(4) << std::hex << fStatus[ii].nStates<<"   "
    //  << std::setw(4) << std::hex << fStatus[ii].line   <<"   "
    //  << std::setw(4) << std::hex << fStatus[ii].zero   <<"   "
    //  << std::setw(4) << std::hex << fStatus[ii].ovf    <<"   "
    //  << std::setw(4) << std::hex << fState [ii].nHit   <<"   "
    //  << std::setw(4) << std::hex << fState [ii].column <<"   "
    //  << std::setw(4) << std::hex << fState [ii].zeros  <<"   "
    //  <<std::endl;
    //}
    //for(auto it=fData.begin();it!=fData.end();++it){
    //  std::cout
    //  <<i++<<"  "
    //  <<"      \e[0;31m"
    //  <<std::bitset<4>(it->stateNb)<<"  "
    //  <<std::bitset<10>(it->data)<<"  "
    //  <<it->zero1<<it->zero2<<"   "
    //  <<std::bitset<16>(it->row)<<" "<<(i%2?"-":" ")
    //  <<std::endl;;
    //}
    std::cout<<"\e[0m";
}
void FrameData::Pull(){
  //std::cout<<"\e[1;32mFrame pull begin"<<std::endl;
  for(int i=0;i<4;++i) fHeader.rows[i]=fInStream.Get32();
  if(fHeader.dataLength1!=fHeader.dataLength2){
    std::cerr
      <<"Repeated frame length error '"
      <<fHeader.dataLength1<<"!="<<fHeader.dataLength2
      <<"'"<<std::endl;
    throw FileFormatError();
  }
  //std::cout<<"number of elements  "<<(fHeader.dataLength1+fHeader.dataLength2)<<std::endl;


  int dataread=0;
  while (dataread<(fHeader.dataLength1+fHeader.dataLength2) ){
    //data_t tt;
    //fData.push_back(tt);
    status_line_t tmpstatus;
    state_t tmpstate;


    dataread++;
    tmpstate.row=fInStream.Get16();
    if(!(dataread<(fHeader.dataLength1+fHeader.dataLength2))){
      std::cout<<dataread<<"/"<<fHeader.dataLength1+fHeader.dataLength2<<std::endl;
      std::cerr<<"WARNING "<<__FILE__ <<":"<< __LINE__<<std::endl;break;}
    dataread++;
    tmpstatus.row=fInStream.Get16();


    //std::cout<<"Data read: "<<dataread<<"/"<<fHeader.dataLength1+fHeader.dataLength2<<std::endl;
    std::cout<< std::setfill('0')<<std::setw(4)<<std::hex<<dataread <<"        "
      <<"\e[0;42m"<< std::setfill('0')<< std::setw(1) << std::hex << tmpstatus.nStates<<"|"<<"\e[0;m"
      << std::setfill('0')<< std::setw(3) << std::hex << tmpstatus.line   <<"|"
      << std::setfill('0')<< std::setw(1) << std::hex << tmpstatus.zero   <<"|"
      << std::setfill('0')<< std::setw(1) << std::hex << tmpstatus.ovf    <<"    ";//<<std::endl;

    if(tmpstatus.nStates==0){std::cout<<std::endl;continue;}
    if(tmpstatus.nStates>9){
      WARNING("nStates="+std::to_string(tmpstatus.nStates)+" not throwing exception");
      //WARNING("nStates="+std::to_string(tmpstatus.nStates));
      //throw FileFormatError();
    }
    auto datamapit=fMap.emplace((uint16_t)tmpstatus.line,std::set<uint16_t>());
    std::set<uint16_t>& linelist=datamapit.first->second;

    auto it=fDatamap.emplace((uint16_t)tmpstatus.row,std::vector<state_t>());
    std::vector<state_t>& states=it->second;
    states.resize(tmpstatus.nStates);

    if(tmpstatus.nStates==0)continue;

    for(uint16_t col=tmpstate.column;col<=tmpstate.column+tmpstate.nHit;++col){
      linelist.insert(col);
    }

    states[0]=tmpstate;
    //std::cout<< std::setfill('0')<<std::setw(4)<<std::hex<<dataread   <<"                                "
    //  << std::setfill('0')<< std::setw(1) << std::hex << tmpstate.nHit   <<"   "
    //  << std::setfill('0')<< std::setw(3) << std::hex << tmpstate.column <<"   "
    //  << std::setfill('0')<< std::setw(1) << std::bitset<4>(tmpstate.zeros)  <<"   "
    //  <<std::endl;
    for(int state_i=1;state_i<tmpstatus.nStates;++state_i){
      dataread++;
      tmpstate.row=fInStream.Get16();
      states[state_i]=tmpstate;
      linelist.insert((uint16_t)tmpstate.column);
      //std::cout<<"Data read: "<<dataread<<"/"<<fHeader.dataLength1+fHeader.dataLength2<<std::endl;
      //std::cout<< std::setfill('0')<<std::setw(4)<<std::hex<<dataread <<"                                "
      //  << std::setfill('0')<< std::setw(1) << std::hex << tmpstate.nHit   <<"   "
      //  << std::setfill('0')<< std::setw(3) << std::hex << tmpstate.column <<"   "
      //  << std::setfill('0')<< std::setw(1) << std::bitset<4>(tmpstate.zeros)  <<"   "
      //  <<std::endl
      //  <<std::endl
      //  <<std::endl;
      if(!(dataread<(fHeader.dataLength1+fHeader.dataLength2))){
        std::stringstream aa;
        WARNING("Data exceed the frame buffer");
        break;}
    }
    //fDatamap[tmpstatus].resize(tmpstatus.nStates);

    for(int state_i=0;state_i<states.size();++state_i){
      std::cout
        << std::setfill('0')<< std::setw(1) << std::hex << states[state_i].nHit   <<"|"
        << std::setfill('0')<< std::setw(3) << std::hex << states[state_i].column <<"|"
        << std::setfill('0')<< std::setw(1) << std::bitset<4>(states[state_i].zeros)  <<"   ";
    }
    std::cout<<std::endl;


    //fStatus.push_back(tmpstatus);
    //state_t tmpstate;
    //tmpstate.row=fInStream.Get16();
    //fState.push_back(tmpstate);
  }
  //std::cout<<"Frame pull end\e[0m"<<std::endl;
}
