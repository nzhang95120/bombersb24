#include "Map.h"
#include "Errors.h"
#include <sstream>
#include <string>
// Member Functions


Map::Map(std::istream& stream){
    string line;
    int row = 0;
    int col = 0;
    while(getline(stream, line)){
        istringstream ss(line);
        vector<Point> newRow; 
        map.push_back(newRow);
        for(char c : line){
            Point newPoint = Point(row, col);
            if (c == '*'){
                bombs.insert(newPoint);
            } else if (c == '#'){
                walls.insert(newPoint);
            } else if (c == '~'){
                waters.insert(newPoint);
            }
            map[row].push_back(newPoint);
            col++;
        }
        col = 0;
        row++;
    }

}

vector<int> dx = {1, 0, -1, 0};
vector<int> dy = {0, 1, 0, -1};
vector<char> dir = {'e', 's', 'w', 'n'}; 

std::string Map::route(Point src, Point dst){

    if (src.lat < 0 || src.lng < 0 || src.lat >= static_cast<int>(map.size()) || src.lng >= static_cast<int>(map[0].size()) || walls.find(src) != walls.end() || waters.find(src) != waters.end()){
        throw PointError(src); 
    }
    if (dst.lat < 0 || dst.lng < 0 || dst.lat >= static_cast<int>(map.size()) || dst.lng >= static_cast<int>(map[0].size()) || waters.find(dst) != waters.end()) {
        throw PointError(dst);
    }

    Node cur = Node{src, 0, "", bombs, walls}; 
    visited.insert(cur);
    exploration.push(cur);

    while(!exploration.empty()){
        Node current = exploration.front();
        exploration.pop();
        
        if (current.position.lat == dst.lat && current.position.lng == dst.lng){
            
            while (!exploration.empty()){
                exploration.pop();
            }
            visited.clear();
            
            return current.path;
        }

        for (size_t i = 0; i < 4; i++){
            cout << current.path << endl;
            int newLat = current.position.lat + dy[i];
            int newLng = current.position.lng + dx[i];
            char direction = dir[i];
            size_t bomb_Count = current.bombCount;
            unordered_set<Point> curr_bombs = current.current_bombs;
            unordered_set<Point> curr_walls = current.current_walls;
            
            if (newLat < 0 || newLng < 0 || newLat >= static_cast<int>(map.size()) || newLng >= static_cast<int>(map[0].size())) {
                continue; 
            }
            
            Point newPoint = map[newLat][newLng];

            if (!isValid(current, newPoint)){
                continue;
            }

            if(curr_bombs.find(newPoint) != curr_bombs.end()){
                curr_bombs.erase(curr_bombs.find(newPoint));
                bomb_Count++;
            }
            if(curr_walls.find(newPoint) != curr_walls.end() && bomb_Count > 0){
                curr_walls.erase(curr_walls.find(newPoint));
                bomb_Count --;
            } 
            Node visiting =  Node{newPoint, bomb_Count, current.path + direction, curr_bombs, curr_walls};

            if(visited.find(visiting) == visited.end()){
                
                visited.insert(visiting);
                exploration.push(visiting);
            }

            
        }
    }

    while (!exploration.empty()){
        exploration.pop();
    }
    visited.clear();
    throw RouteError(src, dst);
}



bool Map::isValid(Node cur, Point dst){
    if(dst.lat < 0 || dst.lng < 0 || dst.lat >= static_cast<int>(map.size()) || dst.lng >= static_cast<int>(map[0].size())){
        return false;
    }
    if(cur.current_walls.find(dst) != cur.current_walls.end() && cur.bombCount == 0){
        return false;
    }
    if(waters.find(dst) != waters.end()){
        return false;
    }
    return true;
}
