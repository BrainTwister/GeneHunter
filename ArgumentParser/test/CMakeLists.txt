set(TestName ArgumentParserTest)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIR}
)

add_executable(
    ${TestName}
    ArgumentParser.cpp
    Design.cpp
    main.cpp
)
    
target_link_libraries(
    ${TestName}
    ArgumentParser
    ${GTEST_BOTH_LIBRARIES}
    -pthread
)

add_test(
    NAME ${TestName}
    COMMAND ${TestName} --gtest_output=xml:${CMAKE_BINARY_DIR}/Testing/${TestName}.xml
)
