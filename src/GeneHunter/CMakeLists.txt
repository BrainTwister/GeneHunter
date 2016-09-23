include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/ArgumentParser/include
    ${MYSQL_INCLUDE_DIR}
)

add_executable(
    GeneHunter
    GeneHunter.cpp
)
    
target_link_libraries(
    GeneHunter
    ArgumentParser
    GenomeLib
    UtilitiesLib
    ${MYSQL_LIBRARIES}
    ${Boost_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

install(TARGETS GeneHunter RUNTIME DESTINATION bin)
