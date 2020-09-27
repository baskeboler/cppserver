cmake_minimum_required(VERSION 3.5)

project(cppserver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing(true)
find_package(Boost COMPONENTS unit_test_framework system filesystem program_options log REQUIRED)
find_package(Threads REQUIRED)
set(SERVED_BUILD_TESTS OFF)
set(SERVED_BUILD_EXAMPLES OFF)
include_directories(libs/served/src)
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES (${served_INCLUDE_DIRS})
#add_subdirectory(libs/served)

#set(CATCH_BUILD_EXAMPLES true)
#add_subdirectory(libs/Catch2)
find_package(Catch2 REQUIRED)
INCLUDE_DIRECTORIES (${Catch2_INCLUDE_DIRS})

#add_executable(cppserver main.cpp)
#target_link_libraries(cppserver served ${Boost_LIBS} Threads::Threads)
#add_executable(fileserver fileserver.cpp)

#target_link_libraries(fileserver served ${Boost_LIBRARIES} Threads::Threads)
#add_test(tester tester)

set(beastserver_SRC session.cpp 
    handlers.cpp 
    listener.cpp 
    html_element.cpp)
set(beastserver_HEADERS 
    session.h 
    handlers.h 
    listener.h
    html_element.h)
add_library(beaststuff  ${beastserver_SRC} ${beastserver_HEADERS})

target_link_libraries(beaststuff ${Boost_LIBRARIES} Catch2::Catch2)

add_executable(beastserver
    beastserver.cpp 
    )

target_link_libraries(beastserver ${Boost_LIBRARIES} Catch2::Catch2 beaststuff)

add_executable(mytest catchtest.cpp test_handlers.cpp test_html.cpp ${beastserver_HEADERS})

target_link_libraries(mytest beaststuff Catch2::Catch2 ${Boost_LIBRARIES})
include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(mytest)