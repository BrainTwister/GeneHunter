include_directories(
    ../UtilitiesLib
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

ADD_TEST(UtilitiesTest ${CMAKE_BINARY_DIR}/bin/UtilitiesTest -i ${CMAKE_SOURCE_DIR}/TestSuiteInput -o /home/bernd/scratch)
