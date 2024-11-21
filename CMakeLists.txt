cmake_minimum_required(VERSION 3.10)
project(HttpLib)

set(HTTP_LIB_VERSION_MAJOR 1)
set(HTTP_LIB_VERSION_MINOR 0)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(include)

add_library(HttpLite STATIC src/HttpLite.cpp)

find_package(OpenSSL REQUIRED)
target_link_libraries(HttpLite PRIVATE OpenSSL::SSL OpenSSL::Crypto)

if(WIN32)
    target_link_libraries(HttpLite PRIVATE ws2_32)
endif()

install(TARGETS HttpLite
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

install(FILES include/HttpLite.h DESTINATION include)
