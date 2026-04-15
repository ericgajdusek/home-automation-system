set(AP_CODEC_JSON_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/src/ap_codec_json.c
    ${CMAKE_CURRENT_LIST_DIR}/third_party/cJSON/cJSON.c
)

if(ESP_PLATFORM)
    idf_component_register(
        SRCS ${AP_CODEC_JSON_SRCS}
        INCLUDE_DIRS
            include
            third_party/cJSON
        REQUIRES
            ap_core
    )
else()
    add_library(ap_codec_json STATIC ${AP_CODEC_JSON_SRCS})

    target_include_directories(ap_codec_json PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/third_party/cJSON
    )

    target_link_libraries(ap_codec_json PUBLIC
        ap_core
    )
endif()