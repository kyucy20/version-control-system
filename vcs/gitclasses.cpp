#include <iostream>
#include <filesystem>
#include <string>
#include "commits.cpp"
#include <vector>


class Gitclasses
{

public:
    Node_Llist list;// to store commit data

    void gitInitialization() // initilizes a new repository
    {
        std::filesystem::create_directory(".git");// main directory
        std::filesystem::create_directory(".git/staging_area");// acts as a temp storage where files are held before they are commited to the repository's history
        std::filesystem::create_directory(".git/commits");// stores detailed information about each commit
    }

    void gitAdd()
    {
        const auto copyOptions = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive;// updates all existing files and goes through them recursively to ensure all files are included then assigns them to a variable named copyoptions so that when we wanna do both we can just call this variable.
        for (const auto& dirEntry : std::filesystem::directory_iterator(std::filesystem::current_path()))// for dirEntry(represents one of the entries) we iterate throughout every file in the directory
        {
            auto fileName = dirEntry.path().filename();//retrieves file name of the current directory entry in the variable fileName and then we used it to compare if it is a git file
            if (fileName != "git" && fileName != ".git")//used to exclude our own directory
            {
                if (std::filesystem::is_directory(fileName))
                {
                    std::filesystem::copy(dirEntry, std::filesystem::current_path() / ".git" / "staging_area" / fileName, copyOptions);// so we basically copy the contents of dirEntry  to the current path's staging area
                }
                else
                {
                    std::filesystem::copy(dirEntry, std::filesystem::current_path() / ".git" / "staging_area" / "", copyOptions);// if it's a file not a directory so we don't copy its path (cuz there isnt) and we just place it at the root of the staging_area
                }
            }
        }
    }




    void gitAdd(std::string files[], int arrsize)// to work on specified files
    {
        const auto copyOptions = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive;// updates all existing files and goes through them recursively to ensure all files are included then assigns them to a variable named copyoptions so that when we wanna do both we can just call this variable.
        for (int i = 0;i < arrsize;i++) {
            std::filesystem::path filePath = std::filesystem::current_path() / files[i];
            if (std::filesystem::exists(filePath))
            {
                std::filesystem::path targetDir = std::filesystem::current_path() / ".git" / "staging_area" / filePath.parent_path().filename();// determines the targeted directory


                if (!std::filesystem::exists(targetDir)) {
                    std::filesystem::create_directories(targetDir);// creates that directory if it doesnt exist
                    std::filesystem::copy(filePath, targetDir, copyOptions);
                }


            }
            else {
                std::cout << "file doesnt exist" << std::endl;
            }
        }
    }

    void gitAdd(std::vector<std::string>&files)// to work on specified files
    {
        const auto copyOptions = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive;// updates all existing files and goes through them recursively to ensure all files are included then assigns them to a variable named copyoptions so that when we wanna do both we can just call this variable.
        for (int i = 0;i < files.size();i++) {
            std::filesystem::path filePath = std::filesystem::current_path() / files[i];
            if (std::filesystem::exists(filePath)) { // Check if the file actually exists
                std::filesystem::path targetDir = std::filesystem::current_path() / ".git" / "staging_area" / filePath.parent_path().filename();// Ensure the directory exists where files need to be copied


                std::filesystem::create_directories(targetDir); // create_directories safely does nothing if the directory already exists

                // Copy the file or directory
                std::filesystem::copy(filePath, targetDir, copyOptions);
            }
            else {
                std::cout << "File does not exist: " << files[i] << std::endl;
            }
        }
    }

    
    void gitcommit(std::string msg)
    {
        list.add_on_tail(msg);

    }

    void gitrevert(std::string hash)
    {
        list.revertcommit(hash);
    }

    void gitlog()
    {
        list.printcommitlist();
    }

};