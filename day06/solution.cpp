#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <unordered_map>
#include <climits>
#include <unordered_set>

struct Hash{
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &x) const
    {
        return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
    }
};

// load and parse data from stdin
std::vector<std::pair<int,int>> loadData(){
    int x, y;
    char _; // for comma
    std::vector<std::pair<int,int>> result;
    while(std::cin >> x){
        std::cin >> _;
        std::cin >> y;
        result.emplace_back(x,y);
    }
    return result;
}

// manhattan distance
int manhanttan(std::pair<int,int> p1, std::pair<int,int> p2){
    return abs(p1.first - p2.first) + abs(p1.second - p2.second); 
}

//get x_min, x_max, y_min, y_max of bounding box
std::array<int, 4> getBounds(const std::vector<std::pair<int,int>>& data){
    std::array<int, 4> res {
        data[0].first, data[0].first,
        data[0].second, data[0].second
    };

    for(auto point : data){
        if(point.first < res[0])
            res[0] = point.first;
        if(point.first > res[1])
            res[1] = point.first;
        if(point.second < res[2])
            res[2] = point.second;
        if(point.second > res[3])
            res[3] = point.second;
    }
    return res;
}

// get closest point from data to specified one, 
// sets multiple to true if there is more points than one with min distance
std::pair<int,int>
getClosestPoint(std::pair<int,int> p, const std::vector<std::pair<int,int>>& data,
bool& multiple)
{
    int min = INT_MAX;
    auto min_p = data[0]; 
    for(auto& point : data){
        int curr = manhanttan(point, p);
        if(curr == min)
            multiple = true;
        else if(curr < min){
            min = curr;
            min_p = point;
            multiple = false;
        }
    }
    
    return min_p;
}

// get set of points to which infinite points is closest 
std::unordered_set<std::pair<int,int>, Hash>
getInfinite(const std::vector<std::pair<int,int>>& data){
    auto bb = getBounds(data);
    std::unordered_set<std::pair<int,int>, Hash> res;
    bool mul = false;
    std::pair<int,int> point;
    
    for(int x = bb[0] - 1; x <= bb[1] + 1; x++){
        point = getClosestPoint({x, bb[2] - 1}, data, mul);
        if(!mul)
            res.insert(point);
        point = getClosestPoint({x, bb[3] + 1}, data, mul);
        if(!mul)
            res.insert(point);
    }

    for(int y = bb[2]; y <= bb[3]; y++){
        point = getClosestPoint({bb[0] - 1, y}, data, mul);
        if(!mul)        
            res.insert(point);
        point = getClosestPoint({bb[1] + 1, y}, data, mul);
        if(!mul)
            res.insert(point);
    }
    return res;
}

int main(){
    const auto data = loadData();
    auto bb = getBounds(data);
    std::unordered_map<std::pair<int,int>, int, Hash> sizes;
    auto inf = getInfinite(data);

    for(int x = bb[0]; x < bb[1] + 1;  x++){
        for(int y = bb[2]; y < bb[3] + 1; y++){
            bool mul = false;
            auto min_p = getClosestPoint({x,y}, data, mul);
            if(!mul && inf.find(min_p) == inf.end())
                sizes[min_p]++;
        }
    }

    int max = 0;
    for(auto p : sizes){
        if(p.second > max)
            max = p.second;
    }
    std::cout << "Part 1: " << max << std::endl;

    int area = 0;
    for(int x = bb[0]; x < bb[1]+1;  x++){
        for(int y = bb[2]; y < bb[3]+1; y++){
            int total = 0;
            for(auto p : data){
                total += manhanttan(p, {x,y});
            }
            if(total < 10000)
                area++;
        }
    }

    std::cout << "Part 2: " << area << std::endl;
    return 0;
}