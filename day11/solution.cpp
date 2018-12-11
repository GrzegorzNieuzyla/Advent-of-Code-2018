#include <iostream>

// unique input
constexpr int INPUT = 9221;

// calculate level of a cell
int level(int x, int y){
    int power = (x+ 10)*y + INPUT;
    power *= (x+10);
    return ((power / 100) % 10) - 5;
}


// get grid level for given left-top coordinate and square size
int gridPower(int x, int y, int size=3){
    int res = 0;
    for(int xx = 0; xx < size; xx++){
        for(int yy = 0; yy < size; yy++){
            res += level(x + xx, y + yy);
        }
    }
    return res;

}


// get maximum grid level and corresponding size for given left-top coordinate
std::pair<int,int> gridPowerMax(int x, int y){
    int last = 0;
    int max_size = 0;
    int max = last;
    for(int s = 0; s < (301 - std::max(x,y)); s++){
        for(int dx = 0; dx <= s; dx++){
            last += level(x + dx, y + s);
        }
        for(int dy = 0; dy < s; dy++){
            last += level(x + s, y + dy);
        }
        if(last > max){
            max = last;
            max_size = s + 1;
        }
    }
    return {max, max_size};
}



int main()
{
    int max_power = 0;
    std::pair<int,int> max_coord;
    for(int x = 1; x <= 298; x++){
        for(int y = 1; y <= 298; y++){
            int val = gridPower(x, y);
            if (val > max_power){
                max_power = val;
                max_coord = {x, y};
            }
        }
    }
    std::cout << max_coord.first << "," << max_coord.second << std::endl;
    


    max_power = 0;
    int max_size = 0;
    for(int x = 1; x <= 300; x++){
        for(int y = 1; y <= 300; y++){
            auto r = gridPowerMax(x,y);
            if (r.first > max_power){
                max_power = r.first;
                max_coord = {x,y};
                max_size = r.second;
            }
        }
    }

    std::cout << max_coord.first << "," << max_coord.second;
    std::cout << "," << max_size << std::endl;
    
    
    return 0;
}
