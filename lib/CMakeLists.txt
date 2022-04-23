add_library(${PROJECT_NAME}
  src/pool.cpp
  include/dkubiszewski/threading/pool.hpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
