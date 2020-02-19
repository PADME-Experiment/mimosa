#include"InputStream.h"
#include"EventData.h"
#include"mimosa.h"
#include"utlMessageBus.h"

#include<iostream>
#include<string>
#include<cstring>
#include<cstdlib> // strtol
#include<cstdio>
#include<deque>
#include<vector>
#include<exception>


int main(){
  utl::MessageBus::GetInstance().SetVerbosity(1);
  //InputStream is("../run_1010/8005_data_FPGA_Mouser993P0160_V1_ch5.ZS");
  InputStream is("192.168.62.130_data_FPGA_Mouser993P0160_V1_ch3.ZS");
  int n=0;
  while(is.HasMoreData()){
    if(n++>0x18d)break;
    try{
      EventData aa(is);
      aa.NPixels();
      aa.PBM();
      aa.DebugDump();
    }catch(const FileFormatError& t){
      std::cerr<<"FileFormatError caught. Ignore the event and continue"<<std::endl;
    }
  }

  return 0;
}














int decode_old(const std::string& filename){

  std::ifstream filep;
  filep.open(filename);
  if(! filep.is_open()){
    std::cerr<<"problem with file '"<<filename<<"'"<<std::endl;
    return 1;
  }
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
  filep.close();

  return 0;
}
