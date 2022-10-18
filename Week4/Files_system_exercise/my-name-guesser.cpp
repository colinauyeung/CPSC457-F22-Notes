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
// The program runs like so: "guess-my-name <name1> <name2>-<name3>"
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

    
}