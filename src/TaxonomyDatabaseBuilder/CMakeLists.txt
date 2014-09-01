include_directories(
    ../BoostLib
    ../GenomeLib
    ../UtilitiesLib
    ${MYSQL_INCLUDE_DIR}
)

add_executable(
    TaxonomyDatabaseBuilder
    TaxonomyDatabaseBuilder.cpp
    TaxonomyWriter.cpp
)
    
target_link_libraries(
    TaxonomyDatabaseBuilder
    GenomeLib
    UtilitiesLib
    ${MYSQL_LIBRARIES}
    ${Boost_LIBRARIES}
)
