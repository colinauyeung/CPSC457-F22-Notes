#include<iostream>
#include<string>

int main(int argc, char * argv[]){
    if(argc != 3){
        std::cout << "USAGE: guess-my-name <name1> <name2>-<name3>" << std::endl;
        return 0;
    }
    std::string name1 (argv[1]);
    std::string name2 (argv[2]);
    if(name1.compare("Xyrjyre")==0 && name2.compare("Umilythe-Liaharice")==0){
        std::cout << "Congratulations! You have guess my name! Take your reward *Hands you 5 yen*" << std::endl;
        return 0;
    }
    std::cout << "Sadly that is not my name! Try again or face the wraith of ZUR!" << std::endl;

}
