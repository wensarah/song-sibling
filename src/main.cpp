#include <iostream>
#include <vector>
#include <string>
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

    //testing clusters
    double r = 15.0; // tweak this — try a few values
    std::vector<std::vector<Point>> clusters = clusterPoints(points, kdtree, r);

    std::cout << "Found " << clusters.size() << " clusters:" << std::endl;
    for (size_t i = 0; i < clusters.size(); i++) {
        std::cout << "Cluster " << i << ": ";
        for (const Point& p : clusters[i]) {
            std::cout << p.name << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}