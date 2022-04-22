cmake_minimum_required(VERSION 3.14.4)

project(poco_wl_client_example)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(WL_SRCS
    main.cpp
    WLApi.cpp

    WLApi.h
    )

include(FetchContent)
FetchContent_Declare(
  Poco
  URL https://github.com/pocoproject/poco/archive/refs/tags/poco-1.10.1-release.zip
)
FetchContent_MakeAvailable(Poco)

add_executable(poco_wl_client ${WL_SRCS})

target_link_libraries(poco_wl_client PUBLIC Poco::Foundation Poco::Net Poco::Util)
