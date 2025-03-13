#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
struct variable{
    int value;
    char name[24];
};                                  // g++ test.cpp -pedantic -Wall -Werror && ./a.out
std::vector<variable> storage;
std::unordered_map<std::string, std::variant<void(*)(), void(*)(std::string), void(*)(std::string, int)>> Fns;
std::vector<std::string> tokenize(std::string input, const char* delimiter){
    size_t pos = 0;
    std::vector<std::string> output;
    while(pos != std::string::npos){
        pos = input.find(delimiter);
        output.emplace_back(input.substr(0, pos));
        input.erase(0, pos+strlen(delimiter));
    }
    return output;
}
void clear_(){
    system("clear");
}
void enter_(std::string roast){
    std::cout << "Entered: "<<roast << std::endl;
}
void delete_(std::string val){
    std::cout << "Deleted: "<< val << std::endl;
}
void exit_(){
    exit(0);
}
void help_(){
    std::cout << R"(
1.  help                - prints out help
2.  clear               - clears the screen
3.  enter(String)       - enters value into db
4.  delete(String)      - deletes value from db
5.  make(String)        - makes a variable set to 0
6.  print(String)       - prints given variable 
7.  interpret(String)   - reads a file
8.  exit                - exits the interface
9.  list                - lists every variable
10. dec(String)(int)    - decrements variable
11. pow(String)(int)    - exponent variable
12. modify(String)(int) - modifies variable
13. inc(String)(int)    - increments variable
14. add(String)(int)    - add variable
15. sub(String)(int)    - subtract variable
16. mul(String)(int)    - multiply variable
17. div(String)(int)    - divide variable
18. free(String)        - delete variable
19. loop(String)(int)   - loops a file N times

)";}
void make_(std::string name){
    for(variable& i : storage){
        if(!strcmp(i.name, name.c_str())){
            std::cout << "Variable already exists!" << std::endl;
            return;
        }
    }
    variable var;
    if(name.size() > 24){
        std::cout << "Variable name too long!!" << std::endl;
    }
    strcpy(var.name, name.c_str());
    var.value = 0;
    storage.push_back(var);
}
void print_(std::string name){
    for(variable& i : storage){
        if(!strcmp(i.name, name.c_str())){
            std::cout << i.name << " = " << i.value << std::endl;
            return;
        }
    }
    
    std::cout << "No variable found: " << name << std::endl;
}
void list_(){
    for(variable& i : storage){
        std::cout << i.name << " = " << i.value << std::endl;
    }
}
void pow_(std::string name, int exp){
    for(variable& i : storage){
        if(!strcmp(i.name, name.c_str())){
            for(short j = 0; j < exp; ++j){
                i.value *= i.value;
                return;
            }
        }
    }
    std::cout << "No variable found: " << name << std::endl;
}
void dec_(std::string name){
    for(variable& i : storage){
        if(!strcmp(i.name, name.c_str())){
            i.value--;
            return;
        }
    }
    std::cout << "No variable found: " << name << std::endl;
}
void inc_(std::string name){
    for(variable& i : storage){
        if(!strcmp(i.name, name.c_str())){
            i.value++;
            return;
        }
    }
    std::cout << "No variable found: " << name << std::endl;
}
void modify_(std::string name, int val){
    for(variable& i : storage){
        if(!strcmp(i.name, name.c_str())){
            i.value = val;
            return;
        }
    }
    std::cout << "No variable found: " << name << std::endl;
}
void add_(std::string name, int val){
    for(variable& i : storage){
        if(!strcmp(i.name, name.c_str())){
            i.value += val;
            return;
        }
    }
    std::cout << "No variable found: " << name << std::endl;
}
void sub_(std::string name, int val){
    for(variable& i : storage){
        if(!strcmp(i.name, name.c_str())){
            i.value -= val;
            return;
        }
    }
    std::cout << "No variable found: " << name << std::endl;
}
void mul_(std::string name, int val){
    for(variable& i : storage){
        if(!strcmp(i.name, name.c_str())){
            i.value *= val;
            return;
        }
    }
    std::cout << "No variable found: " << name << std::endl;
}
void div_(std::string name, int val){
    for(variable& i : storage){
        if(!strcmp(i.name, name.c_str())){
            i.value /= val;
            return;
        }
    }
    std::cout << "No variable found: " << name << std::endl;
}
void free_(std::string name)
{
    for(unsigned long i = 0; i < storage.size(); ++i){
        if(!strcmp(storage[i].name, name.c_str())){
            storage.erase(storage.begin()+i);
        }
    }
    storage.shrink_to_fit();
}
void interpret_(std::string filename){
    std::string temp;
    std::vector<std::string> total;
    std::ifstream file(filename, std::ios::binary);
    if(!file){
        std::cout << "File not found!" << std::endl;
        return;
    }
    while(!file.eof()){
        std::getline(file, temp);
        total.emplace_back(temp);
    }
    for(unsigned long i = 0; i < total.size(); ++i){
        if(!total[i].size()){
            total.erase(total.begin() + i);
        }
    }
    total.shrink_to_fit();
    std::vector<std::string> thing;
    for(unsigned long i = 0; i< total.size(); ++i){
        thing = (tokenize(total[i], " "));
        for(unsigned long i = 0; i < thing.size(); ++i){
            if(thing[i] == ""){
            thing.erase(total.begin() + i);}
        }
        thing.shrink_to_fit();
        int args = thing.size();
        if(thing[0].find("//") != std::string::npos){
            continue;
        }
        if(args == 3 && thing[1] == "="){
            modify_(thing[0], std::stoi(thing[2]));
            continue;
        }
        switch (args){
            case 1:{
                std::visit([](auto&& func){
                    if constexpr (std::is_invocable_v<decltype(func)>){
                        func();
                    } 
                    else{
                        std::cout << "Please try again1\n";
                    }
                }, Fns[thing[0]]);
                continue;
            }
            case 2:{
                std::visit([&thing](auto&& func){
                    if constexpr (std::is_invocable_v<decltype(func), std::string>){
                        func(thing[1]);
                    } 
                    else{
                        std::cout << "Please try again2\n";
                    }
                }, Fns[thing[0]]);
                continue;
            }
            case 3:{
                std::visit([&thing](auto&& func){
                    if constexpr (std::is_invocable_v<decltype(func), std::string, int>){
                        func(thing[1], std::stoi(thing[2]));
                    } 
                    else{
                        std::cout << "Please try again3\n";
                    }
                }, Fns[thing[0]]);
                continue;
            }
        }
    }
}
void loop_(std::string filename, int times){
    for(int i = 0; i < times; ++i){
        interpret_(filename);
    }
}
int main(){    
    std::string input;
    Fns["clear"] = clear_;
    Fns["enter"] = enter_;
    Fns["delete"] = delete_;
    Fns["help"] = help_;
    Fns["exit"] = exit_;
    Fns["make"] = make_;
    Fns["print"] = print_;
    Fns["list"] = list_;
    Fns["interpret"] = interpret_;
    Fns["pow"] = pow_;
    Fns["dec"] = dec_;
    Fns["modify"] = modify_;
    Fns["inc"] = inc_;
    Fns["add"] = add_;
    Fns["sub"] = sub_;
    Fns["div"] = div_;
    Fns["mul"] = mul_;
    Fns["free"] = free_;
    Fns["loop"] = loop_;
    while(1){
        std::cout << ">>> ";
        std::getline(std::cin, input);
        std::vector<std::string> parsed = tokenize(input, " ");
        if(Fns.find(parsed[0]) == Fns.end()){
            std::cout << "Invalid command!!\n";
            continue;
        }
        int args = parsed.size();
        switch (args){
            case 1:{
                std::visit([](auto&& func){
                    if constexpr (std::is_invocable_v<decltype(func)>){
                        func();
                    } 
                    else{
                        std::cout << "Please try again1\n";
                    }
                }, Fns[parsed[0]]);
                continue;
            }
            case 2:{
                std::visit([&parsed](auto&& func){
                    if constexpr (std::is_invocable_v<decltype(func), std::string>){
                        func(parsed[1]);
                    } 
                    else{
                        std::cout << "Please try again2\n";
                    }
                }, Fns[parsed[0]]);
                continue;
            }
                        case 3:{
                std::visit([&parsed](auto&& func){
                    if constexpr (std::is_invocable_v<decltype(func), std::string, int>){
                        func(parsed[1], std::stoi(parsed[2]));
                    } 
                    else{
                        std::cout << "Please try again3\n";
                    }
                }, Fns[parsed[0]]);
                continue;
            }
        }
    }
}
