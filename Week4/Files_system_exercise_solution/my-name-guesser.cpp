#include <fcntl.h>
#include <unistd.h>

#include<stdio.h>
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<unordered_map>
#include<list>

//Oh no, I've accidently left out a malicious sentinent program into the wild! It will  destroy all of Neptune unless you correctly guess it's name!
//...Enough roleplaying, so I've left you with a compiled file called "guess-my-name", you're goal as, the name of the file says, is to guess it's name
// The program runs like so: "./guess-my-name <name1> <name2>-<name3>"
// Now if I just left you with that there are a near infinite amount of possible names. Thankfully for you, I'lve left you with a text file full of the potential names that the program might accept

//You're tasks now are as follows:
//  1. Copy all of the names from the file "names.txt" into a data structure, the one you choose is up to you
//  2. Use popen() to run guess-my-name with each combination of names until you find the correct names. This will involve:
//      a. Looping through the names
//      b. Constructing string which correctly calls "guess-my-name"
//          Optimization Hint: It's name won't be have duplicates!
//      c. Calling guess-my-name using popen() with your string
//      d. Grabing the result and checking if you guess the right name
//          Hint: The program will print "Congradulations..." followed by some other stuff when you've correctly guessed the name
//                and it will print "Sadly..." if you didn't.
//  3. You should print out the correct names once you're done

void example_call(){
    system("./guess-my-name a b-c");
}

int main(){

    //Let's first set up a vector to store my potential names in 
    std::vector<std::string> names;

    //I'm just going to hard code the file name in for now
    char namefile[] = "names.txt";

    //This structure should look similar to what we did in files_processes
    std::string res_s = "";
    char buf[100];

    //Let's open the file and then read to the end
    int n_fd = open(namefile, O_RDONLY);
    int res = read(n_fd, buf, 100);
    while(res != 0){

        //Again let's only deal with characters that were actually added on this read
        for(int i = 0; i<res; i++){

            //If we have a new line, then we've reached the end of one name
            if(buf[i] == '\n'){

                //So let's add the name to the vector and clear our temporary string
                names.push_back(res_s);
                res_s.clear();
            }

            //Otherwise, we're in a name
            else{
                res_s.push_back(buf[i]);
            }
        }

        //Refresh our read
        res = read(n_fd, &buf, 100);
        
    }  
    //...close your files...  
    close(n_fd);

    //Let's take every permutation of our names...
    for(std::string n1: names){
        for(std::string n2: names){

            //get rid of duplicates
            if(n1.compare(n2) == 0){
                continue;
            }
            for(std::string n3: names){

                //more duplicates
                if(n1.compare(n3) == 0){
                    continue;
                }

                //even more duplicates
                if(n2.compare(n3) == 0){
                    continue;
                }

                //Okay, I'm just going to use a C++ string to build up a command from the names
                std::string command = "./guess-my-name";
                command = command + " " + n1 + " " + n2 + "-" + n3;

                //Let's print it for testing
                std::cout << command << std::endl;

                //And let's run the command and get it's results
                //This exactly the same idea as system.cpp in files_processes_complete
                //Also see: ReadingFilesCStyle in Week 4
                FILE * fp = popen(command.c_str(), "r");
                char path[1024];
                std::string str = "";
                while (fgets(path, 1024, fp) != NULL){
                    str.append(path);
                }

                pclose(fp);

                //We know that the prcess wil outpute "Congratulations..." on success, so we can check success, but looking for that C
                if((str.at(0) == 'C')){
                    printf("%s%s \n", str.c_str(), command.c_str());
                    return 0;
                }
                
            }
        }
    }

}