#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdint>

// load number of players and last marble value
std::pair<int, int> loadData(){
    std::vector<std::string> split;
    std::string s;
    while(std::cin >> s){
        split.push_back(s);
    }

    return {std::stoi(split[0]),std::stoi(split[6])};
}

// get iterator to next clockwise element in circle 
std::list<int>::iterator clockwise(std::list<int>& list, std::list<int>::iterator curr){
    curr++;
    return curr == list.end() ? list.begin() : curr;
}

// get iterator to next counter-clockwise element in circle 
std::list<int>::iterator c_clockwise(std::list<int>& list, std::list<int>::iterator curr){
    if (curr == list.begin())
        return std::prev(list.end());; 
    return std::prev(curr);
}

// simulate game
uint64_t getScore(int player_count, int marbles_count){
    std::list<int> marbles;
    std::vector<uint64_t> scores(player_count, 0);
    auto it = marbles.emplace(marbles.end(),0); 
    int player = 0;
    for(int i = 1; i < marbles_count; i++){
        if(i % 23 == 0){
            auto to_remove = it;
            for(int j = 0; j < 7;j++)
                to_remove = c_clockwise(marbles, to_remove);
            scores[player] += i + *to_remove;
            it = clockwise(marbles, to_remove);
            marbles.erase(to_remove);
        }
        else{
            it = clockwise(marbles, it);
            it = clockwise(marbles, it);
            it = marbles.emplace(it, i);
        }

        player = (player + 1) % player_count;
    }

    return *std::max_element(scores.begin(), scores.end());
}


int main(){
    auto data = loadData();
    std::cout << "Score: " << getScore(data.first, data.second) << std::endl;
    std::cout << "Score * 100: " << getScore(data.first, data.second * 100) << std::endl;
}