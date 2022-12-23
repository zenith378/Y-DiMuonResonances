#include <stdio.h>
#include <iostream>
#include "df_set.h"
#include <filesystem>
#include "TSystem.h"

int main()
{
    int status=std::remove("./Data/data.root");
    if (status==0)
    {
        std::cout << "File Deleted Successfully!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Error Occurred: cannot delete file!\n"
                  << std::endl;
        std::cout << "Error Code: " << std::strerror(errno) << std::endl;
        exit(1);
    }
    try
    {
        try
        {
            ROOT::RDF::RNode df = df_set();

            // if(df==nullptr)
            // throw(std::runtime_error("Dataframe not read correctly\n")); /// throw exception to handle
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

    // Deletes one or more files recursively.
    std::filesystem::remove_all("./Data");
    try
    {
        try
        {
            df_set();
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
    }
    catch (std::exception &ex) // hadles exception of not found file
    {
        std::cerr << ex.what() << std::endl;
        exit(1);
    }
    return 0;
}