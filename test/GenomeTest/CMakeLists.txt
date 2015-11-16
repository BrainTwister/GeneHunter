include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${GTEST_INCLUDE_DIR}
    ${MYSQL_INCLUDE_DIR}
)

add_executable(
    GenomeTest
    main.cpp
    SequenceTest.cpp
)
    
target_link_libraries(
    GenomeTest
    GenomeLib
    UtilitiesLib
    ${GTEST_BOTH_LIBRARIES}
    ${MYSQL_LIBRARIES}
)

add_test(
    GenomeTest
    ${CMAKE_BINARY_DIR}/bin/GenomeTest
    --gtest_output=xml:${CMAKE_BINARY_DIR}/Testing/GenomeTest.xml    
)
