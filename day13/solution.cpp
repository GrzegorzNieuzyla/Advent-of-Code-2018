#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

struct Cart{
    int x, y;
    char dir;
    int turn = 0;
    int id;
    Cart(int x, int y, int d, int id) : x(x), y(y), dir(d), id(id){}
};

// parse input
void parseData(std::vector<std::string>& map, std::vector<Cart>& carts){
    map.emplace_back();
    int id = 0;
    for(int y = 0; std::getline(std::cin, map.back()); y++){
        auto& line = map.back();
        for(int x = 0; x < line.size(); x++){
            if (line[x] == '<' || line[x] == '>'){
                carts.emplace_back(x, y, line[x], id++);
                line[x] = '-';
            }
            if(line[x] == '^' || line[x] == 'v'){
                carts.emplace_back(x, y, line[x], id++);
                line[x] = '|';
            }
        }
        map.emplace_back();
    }
    map.pop_back();
}

// function used when sorting carts in reading order
bool CartComp(Cart& c1, Cart& c2){
    if (c1.y == c2.y)
        return c1.x < c2.x;
    return c1.y < c2.y;
}

// move one unit
void moveCart(const std::vector<std::string>& map, Cart& c){
    switch(c.dir){
        case '>':{
            c.x++;
            switch(map[c.y][c.x]){
                case '+':{
                    if(c.turn == 0) c.dir = '^';
                    else if(c.turn == 2) c.dir = 'v';
                    c.turn = (c.turn + 1) % 3;
                    break;
                }
                case '\\': c.dir = 'v'; break;
                case '/': c.dir = '^'; break;
            }
            break;
        }
        case '<':{
            c.x--;
            switch(map[c.y][c.x]){
                case '+':{
                    if(c.turn == 0) c.dir = 'v';
                    else if(c.turn == 2) c.dir = '^';
                    c.turn = (c.turn + 1) % 3;
                    break;
                }
                case '\\': c.dir = '^'; break;
                case '/': c.dir = 'v'; break;
            }
            break;
        }
        case '^':{
            c.y--;
            switch(map[c.y][c.x]){
                case '+':{
                    if(c.turn == 0) c.dir = '<';
                    else if(c.turn == 2) c.dir = '>';
                    c.turn = (c.turn + 1) % 3;
                    break;
                }
                case '\\': c.dir = '<'; break;
                case '/': c.dir = '>'; break;
            }
            break;
        }
        case 'v':{
            c.y++;
            switch(map[c.y][c.x]){
                case '+':{
                    if(c.turn == 0) c.dir = '>';
                    else if(c.turn == 2) c.dir = '<';
                    c.turn = (c.turn + 1) % 3;
                    break;
                }
                case '\\': c.dir = '>'; break;
                case '/': c.dir = '<'; break;
            }
            break;
        }
    }
}

// run simulation until only one cart remains
Cart run(const std::vector<std::string>& map, std::vector<Cart>& carts){
    std::unordered_set<int> crashed;
    while(true){
        // define order of moving based on cart position
        std::sort(carts.begin(), carts.end(), CartComp);
        
        for(int i = 0; i < carts.size(); i++){
            auto& cart = carts[i];
            if(crashed.find(cart.id) != crashed.end())
                continue;
            
            moveCart(map, cart);
            for(int j = 0; j < carts.size(); j++){
                if(crashed.find(carts[j].id) == crashed.end() && j != i){
                    // check for crash
                    if(carts[j].x == cart.x && carts[j].y == cart.y){
                        std::cout << "Crash: (" << cart.x << "," << cart.y << ")" << std::endl;
                        // mark carts as crashed        
                        crashed.emplace(cart.id);
                        crashed.emplace(carts[j].id);
                        if(carts.size() - crashed.size() == 1){
                            carts.erase(std::remove_if(carts.begin(), carts.end(),
                                [&](auto &c){return crashed.find(c.id) != crashed.end();}),
                                carts.end());
                            return carts.back();
                        }
                        break;
                    }
                }   
            }
        }

        // remove crashed carts
        carts.erase(std::remove_if(carts.begin(), carts.end(),
            [&](auto &c){return crashed.find(c.id) != crashed.end();}), 
            carts.end());
        crashed.clear();
    }
}

int main(){
    std::vector<std::string> map;
    std::vector<Cart> carts;
    parseData(map, carts);
    Cart c = run(map, carts);
    std::cout << "Remaining: (" <<  c.x << "," << c.y  << ")" << std::endl;
}
