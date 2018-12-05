#include <iostream>
#include <vector>
#include <set>

int main(){
    std::vector<int> freqs;
    char sign;    

    while(std::cin >> sign){
        int freq;
        std::cin >> freq;
        
        freqs.push_back(sign == '-' ? -freq : freq);
    }

    int total_freq = 0;
    for(auto i : freqs)
        total_freq += i;
    std::cout << "Total: " << total_freq << std::endl;


    std::set<int> seen_frequencies;
    total_freq = 0;
    
    bool found = false;
    while(!found){
        for(auto i : freqs){
            if(seen_frequencies.find(total_freq) != seen_frequencies.end()){
                std::cout << "Duplicate " << total_freq << std::endl;
                found = true;
                break;
            }else{
                seen_frequencies.insert(total_freq);
                total_freq += i;
            }
        }
    }

    return 0;
}