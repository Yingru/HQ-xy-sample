add_library(${LIBRARY_NAME} STATIC HQ_XY.cpp)
set(MAIN HQ_sample.cpp)
add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_NAME} ${Boost_LIBRARIES})

