#include <iostream>
#include <vector>

// returns position if strings differ by exactly one letter, otherwise -1
// expects that s1 and s2 are equal in length  
int letterDifference(std::string s1, std::string s2){
    int diff = -1;
    for(int i = 0; i < s1.size(); i++){
        if(s1[i] != s2[i]){
            if(diff >= 0)
                return -1;
            else
                diff = i;
        }
    }
    return diff;
}

int main(){
    std::vector<std::string> lines;

    std::string line;
    while(std::cin >> line)
        lines.push_back(line);

    for(auto& s1 : lines){
        for(auto& s2 : lines){
            int diff = letterDifference(s1,s2); 
            if(diff >= 0){
                std::cout << "Common letters: " << s1.erase(diff,1) << std::endl;    
                return 0;
            }
        }
    }
    return 1;
}