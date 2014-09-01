include_directories(
    ../BoostLib
    ../GenomeLib
    ../UtilitiesLib
)

add_executable(
    DatabaseBuilder
    DatabaseBuilder.cpp
)
    
target_link_libraries(
    DatabaseBuilder
    GenomeLib
    UtilitiesLib
)

add_executable(
    FASTAExtractor
    FASTAExtractor.cpp
)
    
target_link_libraries(
    FASTAExtractor
    GenomeLib
    UtilitiesLib
)
