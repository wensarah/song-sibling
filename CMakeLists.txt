cmake_minimum_required(VERSION 3.10)   # minimum CMake version needed
project(GeoNNEngine)                    # name of your project

set(CMAKE_CXX_STANDARD 17)              # use C++17
set(CMAKE_CXX_STANDARD_REQUIRED True)   # enforce it (fail if compiler can't do C++17)

add_executable(geo_nn src/main.cpp)     # build an executable called "geo_nn"
                                          # from src/main.cpp (add more .cpp files here later)
target_include_directories(geo_nn PRIVATE external)
