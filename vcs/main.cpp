#include <iostream>
#include <string>
#include "gitclasses.cpp"
#include <vector>




int main(int argc, char* argv[]) {
    Gitclasses gitclassobj;
    std::cout << "this is a simple vcs system" << std::endl;
    std::cout << "choose ur arguments" << std::endl;
    std::cout << " your arguement==" << std::endl;


    if (argc >= 2) {

        std::string arguement;// we take the arguement as a string so we can check what the arguement wants the code to do.
        arguement = argv[1];
        if (arguement == "initialization")// checks if the arguement first element is == to initialization
        {
            gitclassobj.gitInitialization();// calls to the .gitInitalization in the other cpp file
            std::cout << "git has been initialized";// upon completion of the initialization this message will appear

        }
        else if (arguement == "add") {// checks the first arguement if that arguement is == add then we continue
            if (argc >= 3)//depending on the argument count we will determine what is to be added
            {
                if (argc == 3)// checks to see if one file is specified
                {
                    std::string checker;
                    checker = argv[2]; // gets the file arguement 

                    if (checker == ".")// if the arguement is a '.' then it means to add all files in the current directory
                    {
                        gitclassobj.gitAdd();// calls gitAdd() with no arugement to add all files in this directory

                    }

                    else {// this means a specific file is named to be added in this directory
                        std::string v;//a string v to hold the file name
                        v = argv[2];//v is equal to the 2nd arguemtn in argv which in turn assigns the file name
                        std::string files[1] = { v };//array of string called files sized 1 and the only "variable" inside this array is the v
                        gitclassobj.gitAdd(files, 1);// calling of the gitadd
                    }


                }
                else// if more than one file is selected to be added
                {
                    std::vector<std::string> files(argc - 2);// a vector of strings is created to hold the file names
                    for (int i = 2;i < argc;i++)// loops through each commandline arguement that isnt in the first 2 arguements to add the file names to the vector
                    {
                        std::string v;
                        v = argv[i];
                        files[i] = v;
                    }
                    gitclassobj.gitAdd(files);// adds the files starting from the 3rd arguement in the argument that is entered


                }
            }
            else
            {
                std::cout << "Error: Unrecognized command or incorrect number" << std::endl;

            }
        }

            else if (arguement == "commit")
            {
                if (argc == 4)//git,commit,3rd argument should indicate where a mssg follows or not.
                {
                    std::string s2, s3;
                    s2 = argv[2];// indicates a message that followes
                    s3 = argv[3];// the message itself
                    if (s2 == "-m")// check if m which means a message is provided
                    {
                        gitclassobj.gitcommit(s3);// performs the commit with the added message
                        std::cout << "files commited" << std::endl;
                    }
                    else
                    {
                        std::cout << "missing arguements" << std::endl;// error if the message isnt found
                    }


                }
            }
            else if (arguement == "revert")// checks if the argv[1]== "revert"
            {
                if (argc == 3) {// checks for the correct number of arguments for a revert
                    std::string s2;
                    std::string argv2 = argv[2];
                    s2 = argv2;
                    if (s2 == "head")//checks if it wants to be reverted to the head commit
                    {
                        gitclassobj.gitrevert(s2);//performs the reversion
                        std::cout << "now we are at head" << std::endl;
                    }
                    else
                    {
                        gitclassobj.gitrevert(s2);// reverts to a the state before the one it currently is
                        std::cout << "reverted " << std::endl;

                    }
                }
                else
                {
                    std::cout << "invalid arguements" << std::endl;
                }
            }


        }
        else {
            std::cout << "invalid arguments" << std::endl;
        }

    }
   