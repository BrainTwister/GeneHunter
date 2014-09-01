include_directories(
    ../BoostLib
    ../GenomeLib
    ../UtilitiesLib
    ${MYSQL_INCLUDE_DIR}
)

add_executable(
    GeneAssembler
    GeneAssembler.cpp
)
    
target_link_libraries(
    GeneAssembler
    GenomeLib
    UtilitiesLib
    ${MYSQL_LIBRARIES}
)
