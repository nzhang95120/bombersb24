#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>

#include "Point.h"
using namespace std;
#include <vector>
#include <queue>
#include <unordered_set>




class Map {
    struct Node{
        Point position;
        size_t bombCount;
        string path; 
        unordered_set<Point> current_bombs;    
        unordered_set<Point> current_walls; 
        bool operator==(const Node& other) const {
        return position == other.position &&
               bombCount == other.bombCount &&
               current_bombs == other.current_bombs &&
               current_walls == other.current_walls;
    }
    };

    struct NodeHash {
    size_t operator()(const Node& s) const {
        size_t h1 = hash<Point>{}(s.position);
        size_t h2 = hash<size_t>{}(s.bombCount);
        size_t h3 = 0;
        for(const auto& bomb : s.current_bombs){
            h3 ^= hash<Point>{}(bomb);
        }
        size_t h4 = 0;
        for(const auto& wall : s.current_walls){
            h4 ^= hash<Point>{}(wall);
        }
        return h1 ^ h2 ^ h3 ^ h4;
    }
};

    vector<vector<Point>> map;
    unordered_set<Point> bombs;    
    unordered_set<Point> walls;   
    unordered_set<Point> waters;   
    unordered_set<Node, NodeHash> visited;
    queue<Node> exploration;
    
public:
    Map(std::istream& stream);

    std::string route(Point src, Point dst);

    bool isValid(Node cur, Point dst);
};

#endif