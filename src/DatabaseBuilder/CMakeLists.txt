include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/ArgumentParser/include
)

add_executable(
    DatabaseBuilder
    DatabaseBuilder.cpp
)
    
target_link_libraries(
    DatabaseBuilder
    ArgumentParser
    GenomeLib
    UtilitiesLib
)

add_executable(
    FASTAExtractor
    FASTAExtractor.cpp
)
    
target_link_libraries(
    FASTAExtractor
    ArgumentParser
    GenomeLib
    UtilitiesLib
)

install(TARGETS DatabaseBuilder RUNTIME DESTINATION bin)
