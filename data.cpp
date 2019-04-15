#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>








int main()
{
 
  int i=0;
  for(i=0;i<100000;++i)
  {
     std::ostringstream ito;
    ito<<i;
    std::string tmp=ito.str();
    std::cout<<tmp<<std::endl;
    tmp=tmp+"revoke.pem";
    auto fd= open(tmp.c_str(),O_WRONLY | O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    std::fstream fs(tmp);
    fs<<tmp;
    fs.close();
    close(fd);
  }
  return 0;
}






