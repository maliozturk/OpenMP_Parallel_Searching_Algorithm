cmake_minimum_required(VERSION 3.26)
project(OpenMP_Parallel_Searching_Algorithm)

set(CMAKE_CXX_STANDARD 17)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(OpenMP_Parallel_Searching_Algorithm main.cpp)

target_link_libraries(OpenMP_Parallel_Searching_Algorithm PUBLIC ${MPI_CXX_LIBRARIES})
target_link_libraries(OpenMP_Parallel_Searching_Algorithm PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(OpenMP_Parallel_Searching_Algorithm PUBLIC ${OpenCV_LIBS})