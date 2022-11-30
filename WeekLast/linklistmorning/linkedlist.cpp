#include<iostream>
#include<cstring>
#include<list>
#include<vector>

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
            printf("----Line----\n");
            for(auto i: line){
                printf("id: %d, items: %d\n", i.id, i.items);
            }
            printf("----End----\n");
        }

        void printc(){
            printf("----Line----\n");
            for(int i = 0; i<customers.size(); i++){
                printf("%d: ", i);
                for(auto i: customers[i]){
                    printf("%d ", (*i).items);
                }
                printf("\n");
            }
            printf("----End----\n");
        }

        void add(int id, int items){
            if(line.begin()!=line.end()){
                auto back = line.end();
                back = std::prev(back);
                customer &a = *back;
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

            while(id >= customers.size()){
                std::list<customerref> vec;
                customers.push_back(vec); 
            }

            if(id < customers.size()){
                customers[id].push_back(nback);
            }


        }

        void remove(customerref ref){
            if(ref != line.begin() && std::next(ref) != line.end()){
                auto front = std::prev(ref);
                auto back = std::next(ref);

                customer &f = *front;
                customer &b = *back;
                if(f.id == b.id){
                    f.items = f.items + b.items;
                    remove(back);
                } 
            }

            auto &a = *ref;
            for(auto i = customers[a.id].begin(); i!=customers[a.id].end(); i = std::next(i)){
                if(ref == *i){
                    customers[a.id].erase(i);
                    break;
                }
            }

            line.erase(ref);
        }

        void removecustomer(int id){
            if(id<customers.size()){
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
    myline.add(1,5);
    myline.add(2, 6);
    myline.add(10, 4);
    myline.add(2, 6);
    myline.add(2,6);
    myline.print();
    myline.printc();
    myline.removecustomer(10);
    myline.print();
    myline.printc();

}