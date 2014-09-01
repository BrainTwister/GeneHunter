include_directories(
    ../BoostLib
    ../GenomeLib
    ../UtilitiesLib
    ${MYSQL_INCLUDE_DIR}
)

add_executable(
    CDSDatabaseBuilder
    CDSDatabaseBuilder.cpp
)
    
target_link_libraries(
    CDSDatabaseBuilder
    GenomeLib
    UtilitiesLib
    ${MYSQL_LIBRARIES}
    ${Boost_LIBRARIES}
)
