#include <iostream>
#include <vector>
#include <numeric>

// load data from stdin
std::vector<int> loadData(){
    std::vector<int> result;
    int x;
    while(std::cin >> x){
        result.emplace_back(x);
    }
    return result;
}


// traverse tree recursively and return {sum of metadata, size of node and its children} 
std::pair<int, int> traverseTree(std::vector<int>::iterator begin, std::vector<int>::iterator end){
    int child_count = *begin;
    int m_size = *(begin + 1);
    int metadata = 0;
    int offset = 2;
    
    for(int i = 0; i < child_count; i++){
        auto res = traverseTree(begin + offset, end);
        offset += res.second;
        metadata += res.first;
    }

    metadata += std::accumulate(begin + offset, begin + offset + m_size, 0);
    return {metadata, offset + m_size};
}


// traverse tree recursively and return {node value, size} 
std::pair<int, int> nodeValue(std::vector<int>::iterator begin, std::vector<int>::iterator end){
    int child_count = *begin;
    int m_size = *(begin + 1);
    int offset = 2;
    std::vector<int> child_values;

    for(int i = 0; i < child_count; i++){
        auto res = nodeValue(begin + offset, end);
        offset += res.second;
        child_values.push_back(res.first);
    }

    int metadata = 0;
    if(child_count == 0)
        metadata = std::accumulate(begin + offset, begin + offset + m_size, 0);
    else
        for(auto it = begin + offset; it != begin + offset + m_size; ++it){
            int i = *it - 1;
            if(i < child_values.size())
                metadata += child_values[i];
        }
    
    return {metadata, offset + m_size};
}


int main(){
    auto data = loadData();

    std::cout << "Size of metadata: ";
    std::cout << traverseTree(data.begin(), data.end()).first;
    std::cout << std::endl;

    std::cout << "Root value ";
    std::cout << nodeValue(data.begin(), data.end()).first;
    std::cout << std::endl;

}
