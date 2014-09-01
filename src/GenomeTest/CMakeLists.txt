include_directories(
    ../BoostLib
    ../GenomeLib
    ../UtilitiesLib
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

ADD_TEST(GenomeTest ${CMAKE_BINARY_DIR}/bin/GenomeTest -i ${CMAKE_SOURCE_DIR}/TestSuiteInput -o /home/bernd/scratch)
