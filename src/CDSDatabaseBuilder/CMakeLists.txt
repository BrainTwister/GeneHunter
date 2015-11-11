include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/ArgumentParser/include
    ${MYSQL_INCLUDE_DIR}
)

add_executable(
    CDSDatabaseBuilder
    CDSDatabaseBuilder.cpp
)
    
target_link_libraries(
    CDSDatabaseBuilder
    ArgumentParser
    GenomeLib
    UtilitiesLib
    ${MYSQL_LIBRARIES}
    ${Boost_LIBRARIES}
)

install(TARGETS CDSDatabaseBuilder RUNTIME DESTINATION bin)
