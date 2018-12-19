#include <iostream>
#include <regex>
#include <unordered_map>
#include <climits>

struct Hash{
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &x) const
    {
        return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
    }
};

// parse input, populate map with clay and return y min and max coordinates
std::pair<int, int> parseData(std::unordered_map<std::pair<int, int>, char, Hash>& res){
    std::smatch matches;
    std::regex pattern("^([xy])=(.*), [xy]=(.*)\\.\\.(.*)$");
    std::string line;
    int max_y = 0;
    int min_y = INT32_MAX;
    while(std::getline(std::cin, line)){
        std::regex_search(line, matches, pattern);
        std::string axis1 = matches[1];
        int pos = std::stoi(matches[2]);
        int range_beg = std::stoi(matches[3]);
        int range_end = std::stoi(matches[4]);

        for(int i = range_beg; i <= range_end; i++){
            if (axis1 == "x")
                res[{pos, i}] = 1;
            else
                res[{i, pos}] = 1;
        }

        // set up limits
        if(axis1 == "y"){
            if(pos > max_y)
                max_y = pos;
            if(pos < min_y)
                min_y = pos;
        }
        else if(axis1 == "x"){
            if(range_end > max_y)
                max_y = range_end;
            if(range_beg < min_y)
                min_y = range_beg;
        }
    }
    return {min_y, max_y};
}

const int CLAY = 1;
const int STILL = 2;
const int RUNNING = 3;


bool isEmpty(std::unordered_map<std::pair<int, int>, char, Hash>& data, std::pair<int, int> pos){
    return data.find(pos) == data.end();
}

bool isClay(std::unordered_map<std::pair<int, int>, char, Hash>& data, std::pair<int, int> pos){
    if (data.find(pos) != data.end())
        return data[pos] == CLAY;
    return false;
}

bool isWater(std::unordered_map<std::pair<int, int>, char, Hash>& data, std::pair<int, int> pos){
    if (data.find(pos) != data.end())
        return data[pos] == STILL || data[pos] == RUNNING;
    return false;
}


bool flowToSides(std::unordered_map<std::pair<int, int>, char, Hash>& data,
    int max_y, std::pair<int, int> start);

// returns true if continues to flow to sides
bool flowDown(std::unordered_map<std::pair<int, int>, char, Hash>& data,
    int max_y, std::pair<int, int> start)
{
    auto pos = start;
    while(isEmpty(data, {pos.first, pos.second + 1})){
        pos.second++;
        data[pos] = RUNNING;
        if(pos.second >= max_y)
            return true;
    }
    if(isWater(data, {pos.first, pos.second + 1}))
        pos.second++;

    while(!flowToSides(data, max_y, pos)){
        data[pos] = STILL;
        pos.second--;
        if(pos.second == start.second)
            return false;
    }
    return true;
}

// returns true if continues to flow downwards
bool flowToSides(std::unordered_map<std::pair<int, int>, char, Hash>& data,
    int max_y, std::pair<int, int> start)
{
    // flow to left
    auto pos = start;
    bool left = false;
    while(!isClay(data, {pos.first - 1, pos.second})){
        if(isWater(data, {pos.first - 1, pos.second}) &&
            isEmpty(data, {pos.first - 2, pos.second + 1}))
        {
            left = true;
            break;
        }
        pos.first--;
        data[pos] = STILL;
        if(isEmpty(data, {pos.first, pos.second + 1})){
            left = flowDown(data, max_y, pos);
            if(left)
                break;
        }
    }
    int max_left = pos.first;

    // flow to right
    pos = start;
    bool right = false;
    while(!isClay(data, {pos.first + 1, pos.second})){
        if(isWater(data, {pos.first + 1, pos.second}) && 
            isEmpty(data, {pos.first + 2, pos.second + 1}))
        {
            right = true;
            break;
        }
    
        pos.first++;
        data[pos] = 2;
        if(isEmpty(data, {pos.first, pos.second + 1})){
            right = flowDown(data, max_y, pos);
            if(right)
                break;
        }
    }

    // change to running water if it continues to flow
    if(right || left){
        int max_right = pos.first;
        pos.first = max_left;
        while(pos.first <= max_right){
            data[pos] = RUNNING;
            pos.first++;
        }
    }

    return left || right;
}


// simulate flow from start point
void flow(std::unordered_map<std::pair<int, int>, char, Hash>& data,
    int max_y, std::pair<int, int> start)
{
    flowDown(data, max_y, start);
}


int main(){
    std::unordered_map<std::pair<int, int>, char, Hash> data;
    auto ylimit = parseData(data);
    flow(data, ylimit.second, {500, 0});

    int count_running = 0;
    int count_still = 0;
    for(auto& pos : data){
        if(pos.first.second >= ylimit.first && pos.first.second <= ylimit.second)
            if(pos.second == RUNNING)
                count_running++;
            if(pos.second == STILL)
                count_still++;
    }

    std::cout << "Part 1: " << count_still + count_running << std::endl;
    std::cout << "Part 2: " << count_still << std::endl;
    return 0;
}
