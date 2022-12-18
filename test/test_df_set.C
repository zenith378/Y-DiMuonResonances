#include "stdio.h"
#include <iostream>
#include "df_set.h"
#include <filesystem>
#include "TSystem.h"

int main(){
    int status=1;
    ROOT::RDataFrame *df=nullptr;
    status = std::remove("./Data/data.root");
    if(status==0)
        std::cout<<"File Deleted Successfully!\n"<<std::endl;
    else
        {std::cout<<"Error Occurred: cannot delete file!\n"<<std::endl;
        exit(1);}
    try{
        *df=df_set();
        if(df==nullptr)
        throw(std::runtime_error("Dataframe not read correctly\n")); /// throw exception to handle
        if (!std::filesystem::is_directory("./Data") || !std::filesystem::exists("./Data")) // if data directory does not exists
      {
        throw("./Data"); // throw an exception to handle
      }
      if (gSystem->AccessPathName("./Data/data.root")) // if filesystem cannot access pathname
      {
        throw(std::runtime_error("Problem reading data file (it might not exist or might be corrupted)\n")); /// throw exception to handle
      }
    }
    catch (const char *pathToData)
  {
    std::cerr << "Directory does not exist" << std::endl;
    exit(1);
  }
    catch (std::exception &ex) // hadles exception of not found file
  {
    std::cerr << ex.what() << std::endl;
    exit(1);
  }
  
    status=1;
    ROOT::RDataFrame *df1=nullptr;

    status=std::filesystem::remove_all("./Data"); // Deletes one or more files recursively.
if(status==0)
        std::cout<<"\nFile Deleted Successfully!"<<std::endl;
    else{
        std::cout<<"\nError Occurred: cannot delete folder!"<<std::endl;
        exit(1);
    }
    try{
        *df1=df_set();
        if(df1==nullptr)
        throw(std::runtime_error("Dataframe not read correctly\n")); /// throw exception to handle
        if (!std::filesystem::is_directory("./Data") || !std::filesystem::exists("./Data")) // if data directory does not exists
      {
        throw("./Data"); // throw an exception to handle
      }
      if (gSystem->AccessPathName("./Data/data.root")) // if filesystem cannot access pathname
      {
        throw(std::runtime_error("Problem reading data file (it might not exist or might be corrupted)\n")); /// throw exception to handle
      }
    }
    catch (const char *pathToData)
  {
    std::cerr << "Directory not recreated" << std::endl;
    exit(1);
  }
    catch (std::exception &ex) // hadles exception of not found file
  {
    std::cerr << ex.what() << std::endl;
    exit(1);
  }
  return 0;

}