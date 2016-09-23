include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${GTEST_INCLUDE_DIR}
)

add_executable(
    UtilitiesTest
    BandMatrixTest.cpp
    main.cpp
)
    
target_link_libraries(
    UtilitiesTest
    ${GTEST_BOTH_LIBRARIES}
)

add_test(
    UtilitiesTest
    ${CMAKE_BINARY_DIR}/bin/UtilitiesTest
    --gtest_output=xml:${CMAKE_BINARY_DIR}/Testing/CoreGTest.xml
)
