# compile everything except the main source file into a statis lib to be linked
# to both the main executable and the tests
add_library(${LIBRARY_NAME} STATIC 
  HQ_XY.cpp
)

set_target_properties(${LIBRARY_NAME} PROPERTIES PREFIX "")

# compile the actual executable
set(MAIN ${PROJECT_NAME}.cpp)
add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_NAME} ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME})
