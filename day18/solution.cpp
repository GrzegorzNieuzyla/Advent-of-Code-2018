#include <iostream>
#include <vector>
#include <algorithm>
#include <map>


std::vector<std::string> parseData(){
    std::vector<std::string> res;
    do{
        res.emplace_back();
    } while(std::getline(std::cin, res.back()));
    res.pop_back();
    
    return res;
} 

// transform a single cell based on rules
char transformSingle(std::vector<std::string>& map, int x, int y){
    std::vector<char> adjacent;
    if(x > 0)
        adjacent.push_back(map[y][x-1]);
    if(x < map[0].size() - 1)
        adjacent.push_back(map[y][x+1]);
    if(y > 0)
        adjacent.push_back(map[y-1][x]);
    if(y < map.size() - 1)
        adjacent.push_back(map[y+1][x]);
    if(y < map.size() - 1 && x < map[0].size() - 1)
        adjacent.push_back(map[y+1][x+1]);
    if(y < map.size() - 1 && x > 0)
        adjacent.push_back(map[y+1][x-1]);
    if(y > 0 && x < map[0].size() - 1)
        adjacent.push_back(map[y-1][x+1]);
    if(y > 0 && x > 0)
        adjacent.push_back(map[y-1][x-1]);

    switch(map[y][x]){
        case '.':
            if(std::count(adjacent.begin(), adjacent.end(), '|') >= 3)
                return '|';
            break;
        case '|':
            if(std::count(adjacent.begin(), adjacent.end(), '#') >= 3)
                return '#';
            break;
        case '#':
            if(std::count(adjacent.begin(), adjacent.end(), '#') == 0 ||
                std::count(adjacent.begin(), adjacent.end(), '|') == 0)
                return '.';
            break;
        default:;
    }

    return map[y][x];
}


int countValue(std::vector<std::string>& map){
    int l_count = 0;
    int w_count = 0;
    for(int x = 0; x < map[0].size(); x++){
        for(int y = 0; y < map.size(); y++){
            if(map[y][x] == '#')
                l_count++;
            else if(map[y][x] == '|')
                w_count++;
        }
    }
    return l_count * w_count;
}


int simulate(std::vector<std::string> map, size_t minutes){
    // make two copy buffers
    auto map2 = map;
    auto& current = map;
    auto& next = map2;
    
    for(size_t i = 0; i < minutes; i++){
        for(int x = 0; x < map[0].size(); x++){
            for(int y = 0; y < map.size(); y++){
                next[y][x] = transformSingle(current, x, y);
            }
        }
        // swap buffer references
        auto& tmp = current;
        current = next;
        next = tmp;
    }

    return countValue(map); 
}


int main(){
    auto map = parseData();

    std::cout << "Part 1: " << simulate(map, 10) << std::endl;
    
    size_t part2_len = 1000000000UL;
    std::map<int, int> scores;
    int cycle_length = 0;

    // search for cycle
    for(size_t i = 100; i <= part2_len; i+=100){
        int score = simulate(map, i);
        if(scores.find(score) == scores.end())
            scores[score] = i;
        else{
            cycle_length = i - scores[score];
            break;
        }
    }
    
    size_t remaining = part2_len % cycle_length;
    std::cout << "Part 2: " << simulate(map, remaining + cycle_length) << std::endl; 

}