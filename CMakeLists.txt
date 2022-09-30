set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(myMuduo)

set(BOOST_ROOT "/usr/local/include/boost")
#添加头文件搜索路径
include_directories(/usr/local/include, ../myMuduo)
#添加库文件搜索路径
link_directories(/usr/local/lib, ../myMuduo)

add_executable(../myMuduo main.cpp)
