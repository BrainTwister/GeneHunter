include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/ArgumentParser/include
    ${MYSQL_INCLUDE_DIR}
)

add_executable(
    TaxonomyDatabaseBuilder
    TaxonomyDatabaseBuilder.cpp
    TaxonomyWriter.cpp
)
    
target_link_libraries(
    TaxonomyDatabaseBuilder
    ArgumentParser
    GenomeLib
    UtilitiesLib
    ${MYSQL_LIBRARIES}
    ${Boost_LIBRARIES}
)

install(TARGETS TaxonomyDatabaseBuilder RUNTIME DESTINATION bin)
