include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

add_library(
    UtilitiesLib
    Environment.cpp
    EnvironmentVariables.cpp
    StringUtilities.cpp
)
    
target_link_libraries(
    UtilitiesLib
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)
