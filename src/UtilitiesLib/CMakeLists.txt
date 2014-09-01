include_directories(
)

add_library(
    UtilitiesLib
    ArgumentInterpreter.cpp
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
