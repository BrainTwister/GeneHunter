include_directories(
    ../BoostLib
    ../GenomeLib
    ../UtilitiesLib
    ${MYSQL_INCLUDE_DIR}
)

add_executable(
    GeneHunter
    GeneHunter.cpp
)
    
target_link_libraries(
    GeneHunter
    GenomeLib
    UtilitiesLib
    ${MYSQL_LIBRARIES}
)

install(TARGETS GeneHunter RUNTIME DESTINATION bin)
