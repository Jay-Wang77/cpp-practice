

include_directories(${CMAKE_SOURCE_DIR}/hw02)
add_hw_test(testhw02 hw02 test02.cpp)

target_link_libraries(testhw02 PRIVATE hw02)
