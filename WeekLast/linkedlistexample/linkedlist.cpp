#include<iostream>
#include<cstring>
#include<list>
#include<vector>

using namespace std;



struct customer{
    int id;
    int items;
};

using customerref = std::list<customer>::iterator;

class lineup{
    private:
        std::list<customer> line;
        std::vector<std::list<customerref>> customers;

    public:
        void print(){
            printf("-----Line is-----\n");
            for(auto i: line){
                printf("id: %d items: %d\n", i.id, i.items);
            }
            printf("-----Line End----\n");
        }

        void printc(){
            printf("-----Line is-----\n");
            for(int i = 0; (unsigned long)i<customers.size(); i++){
                printf("%d: ", i);
                for(auto ref:customers[i]){
                    printf("%d ", (*ref).items);
                }
                printf("\n");
            }
            printf("-----Line End----\n");
        }

        void add(int id, int items){

            if(line.begin() != line.end()){
                auto back = line.end();
                back = std::prev(back);

                customer & a = *back;

                if(a.id == id){
                    a.items = a.items + items;
                    return;
                }
            }


            customer myc;
            myc.id = id;
            myc.items = items;
            line.push_back(myc);

            auto nback = line.end();
            nback = std::prev(nback);

            if((unsigned long)id < customers.size()){
                customers[id].push_back(nback);
            }
            else{
                while((unsigned long)id >= customers.size()){
                    std::list<customerref> vec;
                    customers.push_back(vec);
                }

                customers[id].push_back(nback);
            }
        }

        void remove(customerref ref){
            if(ref != line.begin() && std::next(ref) != line.end()){
                auto before = std::prev(ref);
                auto after = std::next(ref);

                auto & beforec = *before;
                auto & afterc = *after;

                if(afterc.id == beforec.id){
                    beforec.items = beforec.items + afterc.items;
                     remove(after);
                } 

            }
            
            auto c = *ref;
            for(auto i = customers[c.id].begin(); i!=customers[c.id].end(); i++){
                if(*i == ref){
                    customers[c.id].erase(i);
                    break;
                }
            }
            line.erase(ref);
        }

        void removecustomer(int id){
            if((unsigned long) id < customers.size()){
                std::vector<customerref> toremove;
                for(auto i: customers[id]){
                    toremove.push_back(i);
                }
                for(auto i: toremove){
                    remove(i);
                }
            }
        }

};


int main(){

    lineup myline;

    myline.add(0, 4);
    myline.add(1, 5);
    myline.add(2,6);
    myline.add(2,6);
    myline.add(10,6);
    myline.add(2,6);
    myline.add(10,6);
    myline.print();
    myline.printc();

    myline.removecustomer(2);
    myline.print();
    myline.printc();


}