#include <iostream>
#include <list>
#include <cstdlib>
#include <climits>

// Calculates size after reaction
// can also specify letter (uppercase) to exclude from reaction
int reaction(std::list<char> string, char c = 0){

    if(c != 0){
        string.remove(c);
        string.remove(c + 32);
    }

    for(auto it = string.begin(); it != string.end();){
        auto next = it;
        ++next;

        if(next == string.end())
            break;

        if(abs(*next - *it) == 32){
            string.erase(it);
            auto after = string.erase(next);
            if(after != string.begin())
                --after;
            it = after;
        }else{
            ++it;
        }
    }
    return string.size();
}




int main()
{
    std::list<char> string;
    char c;
    while(std::cin >> c){
        string.push_back(c);
    }

    std::cout << "Size: " << reaction(string) << std::endl;

    // part 2
    auto min = INT64_MAX;
    for(char c = 'A'; c <= 'Z'; c++){
        int current = reaction(string, c);
        if(current < min)
            min = current;
    }
    std::cout << "Best: " << min << std::endl;

    return 0;
}

