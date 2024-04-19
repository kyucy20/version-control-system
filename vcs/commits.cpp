#include <fstream>
#include <filesystem>
#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS




class Commits
{
public:
    std::string commit_id;
    std::string commit_msg;
    std::string next_id;
    Commits* next;
    //commit_id = gethash();



    std::string  gethash()
    {
        std::string s;
        srand(time(NULL));//random number generator
        char array[] = "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";// a random array of alphanumerical characters

        for (int i = 0;i < 10;i++) {//loop to generate the numbers that will be added to a string of length 10
            s += array[rand() % (sizeof(array) - 1)];// this ensures that the value is inside the array and isnt out of bounds 
        }
        return s;


    }
    std::string get_time()
    {
        time_t t = std::time(0); // get time now
        tm* now = std::localtime(&t);
        std::string dateTime = std::to_string(now->tm_year + 1900) + "/" +
            std::to_string(now->tm_mon + 1) + "/" +
            std::to_string(now->tm_mday) + " " +
            std::to_string(now->tm_hour) + ":" +
            std::to_string(now->tm_min) + "\n";

        return dateTime;
    }

    void create_commit()
    {
        Commits newCommit;// automatically generates a commit id

        std::filesystem::create_directory(std::filesystem::current_path() / ".git" / "commits" / newCommit.commit_id);// creates a directory using commit_id
        auto path = std::filesystem::current_path() / ".git" / "commits" / newCommit.commit_id / "commits_info.txt";
        if (!std::filesystem::exists(path.parent_path())) {
            std::filesystem::create_directories(path.parent_path());//checks if directory exists and if it doesnt we create a new one
        }

        std::ofstream outputfile(path.string());//path.string converts the path to a string,outputfile is an object that represents the file stream.
        if (outputfile.is_open())
        {
            std::string t = get_time();
            outputfile << "1." + newCommit.commit_id + "\n" +
                "2." + newCommit.commit_msg + "\n" +
                "3." + get_time() + "\n";
            outputfile.close();// we use this to add specific details to this commit in the output file
        }
        else
        {
            std::cerr << "Error opening file: " << path << std::endl;
        }

        auto staging_area = std::filesystem::path(std::filesystem::current_path() / ".git" / "staging_area");// we create the staging area directory
        const auto copyOptions = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive;
        std::filesystem::path destination = (std::filesystem::current_path() / ".git" / "commits" / newCommit.commit_id / "data");// the destination path to which the staging area will be copied to
        if (std::filesystem::exists(staging_area) && std::filesystem::is_directory(staging_area))//checks if it exists and it is a directory
        {
            std::filesystem::copy(staging_area, destination, copyOptions);// if it exists then it is copied to the destination
        }
        else {
            std::cout << "isnt a directory";
        }

    }
public:
    Commits()
    {
        this->next = nullptr;
    }
    Commits(std::string commit_id, std::string commit_msg)
    {
        this->commit_id = commit_id;
        this->commit_msg = commit_msg;

    }
    Commits(std::string commit_id)
    {
        checknextcommit_id();
        this->commit_id = commit_id;
        this->next = nullptr;
    }
    void revertcommit(std::string hash)// checks if it the head of the list or the tail.
    {

        Commits newCommit;
        std::filesystem::create_directories(std::filesystem::current_path() / ".git" / "commits" / get_id() / "data");// we are making a directory 
        auto next_path = std::filesystem::current_path() / ".git" / "commits" / get_id() / "commitinfo.txt";//sets the path for the "commitinfo.txt" file
        auto copy_from = std::filesystem::current_path() / ".git" / "commits" / hash / "data";
        std::ofstream output(next_path.string());
        if (output.is_open())
        {
            output << "1." + newCommit.commit_id + "\n" +
                "2." + newCommit.commit_msg + "\n" +
                "3." + get_time() + "\n";
            output.close();
        }
        else
        {
            std::cerr << "cant be open";
        }

        const auto copy_options = std::filesystem::copy_options::recursive;
        std::filesystem::copy(copy_from, std::filesystem::current_path() / ".git" / "commits" / get_id() / "data", copy_options);


    }
    void read_git_info()
    {
        auto readfrom = std::filesystem::current_path() / ".git" / "commits" / get_id() / "commitinfo.txt";
        std::string info;
        std::ifstream output(readfrom.string());
        if (output.is_open())
        {
            while (getline(output, info))
            {
                if (info[0] == '1')
                {
                    this->set_id(info.substr(2));//gets the substring starting from the 2nd index to get the id

                }
                if (info[0] == '2')
                {
                    this->set_msg(info.substr(2));
                }

            }

        }

    }
    std::string get_id()
    {
        return this->commit_id;
    }
    void set_id(std::string commit_id)
    {
        this->commit_id = commit_id;
    }
    void set_msg(std::string commit_msg)
    {
        this->commit_msg = commit_msg;
    }
    std::string getcom_msg()
    {
        return this->commit_msg;
    }
    void set_next(Commits* next)
    {
        this->next = next;
    }
    Commits* get_next()
    {
        return next;
    }
    void set_next_id(std::string next_id)
    {
        this->next_id = next_id;
    }
    void writenextcommit_id(std::string nextcommit_id)
    {
        set_next_id(nextcommit_id);
        auto path = std::filesystem::current_path() / ".git" / "commits" / get_id() / "nextcommitinfo.txt";
        std::ofstream output(path.string());
        if (output.is_open())
        {
            output << nextcommit_id;
        }
    }

    std::string checknextcommit_id()
    {
        std::string info;
        auto path = std::filesystem::current_path() / ".git" / "commits" / get_id() / "nextcommitsinfo.txt";
        if (std::filesystem::exists(path))
        {
            std::ifstream output(path.string());
            if (output.is_open())
            {
                while (getline(output, info))
                {
                    this->next_id = info;
                    break;// breaks after reading the first line which is where the next commit id ios usually stored
                }

            }
            output.close();
            return info;// returns next commit id
        }
        return "";// empty if it doesnt open



    }

    std::string getnextcommmit_id()
    {
        return this->next_id;
    }
};
class Node_Llist :public Commits
{
private:
    Commits* head;// makes a pointer named head
    Commits* tails;// makes a pointer named tails

    bool checkif_headexists()
    {

        auto path = std::filesystem::current_path() / ".git" / "commits" / "0x0000";
        return std::filesystem::exists(path);
    }

public:
    Node_Llist()
    {
        this->head = NULL;
        this->tails = NULL;
        if (checkif_headexists())
        {
            set_head(new Commits("0x0000"));
        }
    }

    void set_head(Commits* head)
    {
        this->head = head;
    }

    Commits* get_tail(Commits* tails)
    {
        return this->tails;
    }
    void set_tails(Commits* tails)
    {
        this->tails = tails;
    }
    Commits* gethead()
    {
        return this->head;

    }

   void add_on_tail(std::string msg)
        {
            if (!checkif_headexists()) {
                Commits* newhead = new Commits("0x0000", msg);
                set_head(newhead);
            }
            else {
                std::string commit_id = gethash();
                Commits* current = gethead();
                while (current->get_next() != NULL) {
                    current = current->get_next();
                }
                Commits* newnode = new Commits(commit_id, msg);
                current->set_next(newnode);
                current->writenextcommit_id(newnode->get_id());
            }
        }

    


void revertcommit(std::string commithash)
{
    {
        if (!checkif_headexists())
        {
            std::cout << "This is the first iteration; nothing to revert to." << std::endl;
            return; // Early exit if no head exists.
        }

        Commits* currentnode = gethead();
        Commits* node_to_revert = nullptr;
        bool found = false;

        while (currentnode != NULL && !found)
        {
            if (commithash == currentnode->get_id()) // Check if the commit matches the current node
            {
                node_to_revert = currentnode;
                found = true; // Commit found, stop searching
            }
            currentnode = currentnode->get_next(); // Move to the next node
        }

        if (!found)
        {
            std::cout << "The hash does not match any commits." << std::endl;
            return; // No matching commit found, exit function
        }

        // Reverting to found commit
        node_to_revert->read_git_info(); // Reads info from the commit to be reverted
        Commits* newnode = new Commits(node_to_revert->get_id(), node_to_revert->getcom_msg());
        newnode->revertcommit(node_to_revert->get_id()); // Reverts the commit node, restoring it to the previous state

        if (node_to_revert->get_next() != NULL)
        {
            newnode->set_next(node_to_revert->get_next()); //Link the new node to the successor of the reverted node
        }
        node_to_revert->set_next(newnode); // Link the new node directly after the reverted node

        std::cout << "Commit reverted successfully." << std::endl;
    }


}
void printcommitlist()
{
    Commits* currnode = gethead();
    if (currnode == NULL) {
        std::cout << "No commits to display." << std::endl;
        return;
    }

    while (currnode != NULL)
    {
        
        std::filesystem::path commitpath(std::filesystem::current_path() / ".git" / "commits" / currnode->get_id() / "commitinfo.txt");// Build the path to the commit info file

        
        std::ifstream output(commitpath.string());// Open the commit info file and read its contents
        if (!output) {
            std::cout << "Failed to open commit file for commit ID " << currnode->get_id() << std::endl;
            currnode = currnode->get_next();  // Move to the next node
            continue;
        }

        std::string info;
        while (getline(output, info))
        {
            if (info[0] == '1') {
                std::cout << "Commit ID:    " << info.substr(2) << std::endl;
            }
            if (info[0] == '2') {
                std::cout << "Commit Msg:   " << info.substr(2) << std::endl;
            }
            if (info[0] == '3') {
                std::cout << "Date:         " << info.substr(2) << std::endl;
            }
        }
        output.close();  // Close the file after reading

        
        std::cout << "...........\n";// Print a separator between commits

        
        currnode = currnode->get_next();// Move to the next node
    }
}

};