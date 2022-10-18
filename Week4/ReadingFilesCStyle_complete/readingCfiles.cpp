#include <cstdio>
#include <string> 
#include <deque>

/*
    In this example, we'll discuss a few different things in terms of an example piece of code
    1. We'll at one way to deal with strings, particularly how to extract useful information from them
    2. We'll look at C style file streams, particularly fopen, fread, fgets and fclose

    So in terms of the example itself:
    I have a list for a Magic the Gathering deck stored in the file "deck.txt". This file in in the format:
        1. The first line is the name of the deck
        2. Each subsequent line represents a card in the format:
            <name of card>:<numbers of copies in deck>:<cost of card>

    From this file, I want to extract this decklist and transform into a data type that's useful for playing around in code with.
    ie. I want to have the deck list in a useful data structure
*/

//The very first thing we need to do is to create some data structures to hold the deck list in

//I'm going to have a struct for both individuals cards...
struct Card{
    std::string name;
    int numofcopies;
    int cost;
};

//... and the deck as a whole. These will be useful for store our information once we extract it from the text file
struct Deck{
    std::deque<Card> cards;
    std::string name;
};


//Next we have a function for breaking up the lines in our file which hold the card information
//While I could make this specific to this use case, I'm actually just going to write a generic string split method.
//Note: This should look really similar to the one you saw in Assignment 1!

//What the function will do is that it will break up the string based on the delimiter character and return the array of substrings after the string has been broken up
//Analogous to split() method in Python

//First off, we're going to return an array of strings which will be the parts of the string after being broken up by the delimiter
//Next we'll take in as arguments: the string to be broken up and a delimter character
std::deque<std::string> split (std::string line, char delimter){

    //Let's set up the results structure
    std::deque<std::string> res;

    //Next we'll use a string to contain each part we'll be outputting
    std::string current_word = "";

    //For character in the string
    for(char c:line){

        //If we have a delimter
        if(c == delimter){

            //Then we're at the end of this part of the string
            //So I'm going to check that we actually have a part (ie we're not encountering )
            if(current_word.length() > 0){

                //
                res.push_back(current_word);
            }
            current_word.clear();
        }
        else{
            current_word.push_back(c);
        }
    }
    if(current_word.length() > 0){
        res.push_back(current_word);
    }
    return res;
}

int main(int argc, const char* argv[]){
    if(argc != 2){
        printf("Error input needs to be file\n");
        return 0;
    }

    Deck deck;

    FILE * file = fopen(argv[1], "r+");

    char c;
    deck.name = "";
    while(fread(&c, sizeof(char), 1, file) != 0){
        if(c != '\n'){
            deck.name.push_back(c);
        }
        else{
            break;
        }
    }

    char buffer[1024];

    std::string line = "";
    while(fgets(buffer, 1024, file) != NULL){
        line.append(buffer);
            auto broken = split(line, ':');
            if(broken.size() != 3){
                printf("File Malformed");
                return 0;
            }
            Card card;
            card.name.assign(broken[0]);
            card.numofcopies = std::stoi(broken[1]);
            card.cost = std::stoi(broken[2]);
            deck.cards.push_back(card);

            line.clear();
    }

    fclose(file);

    int totalname = 0;
    int totalcost = 0;
    for(auto card:deck.cards){
        totalname = totalname + card.name.length();
        totalcost = totalcost + card.cost;
    }
    int avgname = totalname / deck.cards.size();
    int avgcost = totalcost / deck.cards.size();

    FILE * wfile = fopen("res.txt", "w+");
    fprintf(wfile, "Statstics for Deck: %s\n", deck.name.c_str());
    fprintf(wfile, "Average name length: %d\n", avgname);
    fprintf(wfile, "Average cost: %d\n", avgcost);
    fclose(wfile);


    printf("%s\n", deck.name.c_str());
    for(int i = 0; i<deck.cards.size(); i++){
        printf("%s: %d %d\n", deck.cards[i].name.c_str(),deck.cards[i].numofcopies, deck.cards[i].cost);
    }

}