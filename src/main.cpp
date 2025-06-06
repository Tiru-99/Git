#include "utils/zstr.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <openssl/sha.h>

int main(int argc, char *argv[])
{
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!\n";

    if (argc < 2)
    {
        std::cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }

    std::string command = argv[1];

    if (command == "init")
    {
        try
        {
            std::filesystem::create_directory(".git");
            std::filesystem::create_directory(".git/objects");
            std::filesystem::create_directory(".git/refs");

            std::ofstream headFile(".git/HEAD");
            if (headFile.is_open())
            {
                headFile << "ref: refs/heads/main\n";
                headFile.close();
            }
            else
            {
                std::cerr << "Failed to create .git/HEAD file.\n";
                return EXIT_FAILURE;
            }

            std::cout << "Initialized git directory\n";
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << e.what() << '\n';
            return EXIT_FAILURE;
        }
    }
    else if (command == "cat-file")
    {
        // check the length of command
        if (argc < 4)
        {
            std::cerr << "Command Less than 3 words!";
            return EXIT_FAILURE;
        }

        std::string option = argv[2];
        std::string sha = argv[3];

        // read for -p
        if (option != "-p")
        {
            std::cerr << "Unknown option for cat-file" << option << "\n";
        }

        //get the first two letters of the sha string
        std::string sha_dir = sha.substr(0, 2);
        std::string sha_filename = sha.substr(2); // gets the remaining 38 words

        //get the file path 
        std::string file_path = ".git/objects/" + sha_dir + "/" + sha_filename;

        // std::cout << "The file path is " << file_path << std::endl;
        // decompressing the file using zstr a open source tool for zlib algo
        zstr::ifstream input(file_path, std::ofstream::binary);
        // check
        if (!input.is_open())
        {
            std::cerr << "Failed to open the file" << std::endl;
            return EXIT_FAILURE;
        }

        // read the file stream into a string
        std::string object_str{std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
        input.close();

        //  blob 11\0hello world -> format
        const auto object_content = object_str.substr(object_str.find('\0') + 1);
        std::cout << object_content << std::flush;

    } else if(command == "hash-object"){
         if (argc < 4)
        {
            std::cerr << "Command Less than 3 words!";
            return EXIT_FAILURE;
        }

        std::string option = argv[2];
        std::string filename = argv[3];

        if(option != "-w"){
            std::cerr<<"No option provided"<<std::endl; 
            return EXIT_FAILURE; 
        }

        //read the file 
        std::ifstream input(filename , std::ios::in);
        if(!input.is_open()){
            std::cerr<<"File is not open"<<std::endl;
            return EXIT_FAILURE; 
        }

        //storing it as a string
        std::string object_content{std::istreambuf_iterator<char>(input) , std::istreambuf_iterator<char>()};
        std::cout<<object_content<<std::endl; 


    }
    else
    {
        std::cerr << "Unknown command " << command << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
