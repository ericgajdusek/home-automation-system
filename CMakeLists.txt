cmake_minimum_required(VERSION 3.16)
project(ap_tests C)

include(CTest)

add_library(unity
    external/Unity/src/unity.c
)

target_include_directories(unity PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/Unity/src
)

add_subdirectory(shared/ap_core)
add_subdirectory(shared/ap_codec_json)

if(BUILD_TESTING)
    add_executable(test_ap_codec_json
        tests/test_ap_codec_json.c
    )

    target_link_libraries(test_ap_codec_json PRIVATE
        unity
        ap_codec_json
    )

    add_test(NAME test_ap_codec_json COMMAND test_ap_codec_json)
endif()