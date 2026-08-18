#include <iostream>
#include "point.h"
#include "kdtree.h"
int main() {
    //testing point.h
    std::vector<Point> points = loadPoints("data/cities.csv");
    for(const Point& city : points){
        std::string title = city.name + " " + std::to_string(city.x)+ " " + std::to_string(city.y);
        std::cout << title << std::endl; 
    }
    
    //testing buildTree
    KDNode* kdtree = buildTree(points,0);
    std::cout << kdtree->point.name << std::endl;
    

    //testing findNearest
    const Point query = {"query", 47.0, -120.0};
    Point nearest = nearestNeighbor(kdtree, query);
    std::cout << "Nearest: " << nearest.name << std::endl;
    
    return 0;
}