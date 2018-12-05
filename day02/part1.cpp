#include <iostream>
#include <string>
#include <algorithm>


// returns result of search as {two letters, three letters}
std::pair<bool, bool> checkId(std::string str){
    std::pair<bool, bool> res = {false, false};
    
    for(char c : str){
        size_t count = std::count(str.begin(), str.end(), c);
        if(count == 2)
            res.first = true;
        if(count == 3)
            res.second = true;

        if(res.first && res.second)
            break;
    }

    return res;
}


int main(){
    int two_count = 0, three_count = 0;
    
    std::string line;
    while(std::cin >> line){
        auto res = checkId(line);    
        if(res.first)
            two_count++;
        if(res.second)
            three_count++;
    }

    std::cout << "Checksum: " << two_count * three_count << std::endl;
    return 0;
}