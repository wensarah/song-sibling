#include <iostream>
#include "point.h"
int main() {
    std::vector<Point> points = loadPoints("data/cities.csv");
    for(const Point& city : points){
        std::string title = city.name + " " + std::to_string(city.x)+ " " + std::to_string(city.y);
        std::cout << title << std::endl; 
    }
    return 0;
}