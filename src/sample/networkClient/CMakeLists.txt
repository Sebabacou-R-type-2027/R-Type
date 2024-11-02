file(GLOB sources *.cpp *.hpp)

target_sources(sample PRIVATE ${sources})
target_link_libraries(sample PRIVATE packet)
