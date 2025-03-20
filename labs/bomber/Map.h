#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>

#include "Point.h"
using namespace std;
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>




class Map {
    struct Node{
        Point position;
        size_t bombCount;
        string path; 
        unordered_set<Point> collected_bombs; 
        unordered_set<Point> destroyed_walls;  
        int priority;
        bool operator==(const Node& other) const {
        return position == other.position &&
               bombCount == other.bombCount;// &&
               //collected_bombs == other.collected_bombs;
    }
    };

    struct NodeHash { 
    size_t operator()(const Node& s) const {
        size_t h1 = hash<Point>{}(s.position);
        size_t h2 = hash<size_t>{}(s.bombCount);
        //for (const auto& b : s.collected_bombs) h1 ^= hash<Point>{}(b);
        //for (const auto& w : s.destroyed_walls) h2 ^= hash<Point>{}(w);
        return h1 ^ h2;

    }
};

    struct CompareNode {
        bool operator()(const Node& a, const Node& b) {
            return a.priority > b.priority; 
        }
    };

    vector<vector<Point>> map;
    unordered_set<Point> bombs;    
    unordered_set<Point> walls;   
    unordered_set<Point> waters;   
    unordered_map<Node, size_t, NodeHash> visited;
    priority_queue<Node, vector<Node>, CompareNode> exploration; 
    const vector<int> dx = {1, 0, -1, 0};
    const vector<int> dy = {0, 1, 0, -1};
    const vector<char> dir = {'e', 's', 'w', 'n'};



    
public:
    Map(std::istream& stream);

    std::string route(Point src, Point dst);

    bool isValid(Node cur, Point dst);
};

#endif