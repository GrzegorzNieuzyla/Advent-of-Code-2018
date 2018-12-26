#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <regex>
#include <algorithm>

using Point = std::array<int, 4>;

// parse data and return vector of points in 4-dimensional space
std::vector<Point> parseData(){
    std::vector<Point> res;
    std::smatch matches;
    std::regex pattern("^(.*),(.*),(.*),(.*)$");
    std::string line;
    while(std::getline(std::cin, line)){
        std::regex_search(line, matches, pattern);
        res.emplace_back();
        for (int i = 0; i < res.back().size(); i++) {
            res.back()[i] = std::stoi(matches[i+1]);
        }
    }
    return res;
}

// manhattan distance
int dist(Point& p1, Point& p2){
    int res = 0;
    for(int i = 0; i < p1.size(); i++)
        res += std::abs(p1[i]-p2[i]);
    return res;
}

// get count of clusters
int getCount(std::vector<Point>& data){
    std::list<std::vector<Point>> constellations;
    for(auto& point : data){
        std::vector<std::list<std::vector<Point>>::iterator> its;

        // get iterators of all constellations that should contain current point
        for(auto it = constellations.begin(); it != constellations.end(); it++){
            if(std::any_of(it->begin(), it->end(), [&](auto x){return dist(x, point) <= 3;}))
                its.push_back(it);
        }

        // add new constellation with current point
        constellations.emplace_back();
        constellations.back().push_back(point);

        // merge previously found constellations
        for(auto it : its){
            for(auto& point : *it)
                constellations.back().push_back(point);
            constellations.erase(it);
        }
    }
    return constellations.size();
}


int main() {
    auto points = parseData();

    std::cout << "Result: " << getCount(points) << std::endl;
    return 0;
}
