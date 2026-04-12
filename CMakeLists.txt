cmake_minimum_required(VERSION 3.16)
project(protocol_tests C)

include(CTest)

add_library(unity
    external/Unity/src/unity.c
)

target_include_directories(unity PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/Unity/src
)

add_subdirectory(shared/app_protocol)
add_subdirectory(shared/app_protocol_json)

if(BUILD_TESTING)
    add_executable(test_app_protocol_json
        tests/test_app_protocol_json.c
    )

    target_link_libraries(test_app_protocol_json PRIVATE
        unity
        app_protocol_json
    )

    add_test(NAME test_app_protocol_json COMMAND test_app_protocol_json)
endif()