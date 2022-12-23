#include <stdio.h>
#include <iostream>
#include "df_set.h"
#include <filesystem>
#include "TSystem.h"

int main()
{
    //if data folder is found
    if(!gSystem->AccessPathName("./Data/"))
    std::filesystem::remove_all("./Data"); //delete data folder is found
    try
    {
        try
        {
            //call the function df_set (it handles the creation of the directory and the downloading of the dataset)
            df_set();
            // if data directory does not exists after the call of df_Set we have a problem
            if (!std::filesystem::is_directory("./Data") || !std::filesystem::exists("./Data")) 
            {
                throw("./Data"); // data directory does not exists
            }
            if (gSystem->AccessPathName("./Data/data.root")) // data.root does not exists
            {
                throw(std::runtime_error("Problem reading data file (it might not exist or might be corrupted)\n")); /// throw exception to handle
            }
        }
        catch (const char *pathToData)
        {
            std::cerr << "Directory not recreated" << std::endl;
            exit(1);
        }
    }
    catch (std::exception &ex) // hadles exception of not found file
    {
        std::cerr << ex.what() << std::endl;
        exit(1);
    }
    //now the directory is created, let's check if it works fine while finding the directory but not the data
    int status=std::remove("./Data/data.root");
    if (status==0)
    {
        std::cout << "Data file Deleted Successfully!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Error Occurred: cannot delete data file!\n"
                  << std::endl;
        std::cout << "Error Code: " << std::strerror(errno) << std::endl;
        exit(1);
    }
    try
    {
        try
        {
            ROOT::RDF::RNode df = df_set();
            if (!std::filesystem::is_directory("./Data") || !std::filesystem::exists("../../Data")) // if data directory does not exists
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
    }
    catch (std::exception &ex) // hadles exception of not found file
    {
        std::cerr << ex.what() << std::endl;
        exit(1);
    }

    std::filesystem::remove_all("./Data");

    return 0;
}