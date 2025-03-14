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
    };

    vector<vector<Point>> map;
    unordered_set<Point> bombs;    
    unordered_set<Point> walls;   
    unordered_set<Point> waters;   
    unordered_set<string> visited;
    queue<Node> exploration;
    
public:
    Map(std::istream& stream);

    std::string route(Point src, Point dst);

    bool isValid(Node cur, Point dst);
};

#endif
