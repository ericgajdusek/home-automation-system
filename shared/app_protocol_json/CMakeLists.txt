set(APP_PROTOCOL_JSON_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/src/app_protocol_json.c
    ${CMAKE_CURRENT_LIST_DIR}/third_party/cJSON/cJSON.c
)

if(ESP_PLATFORM)
    idf_component_register(
        SRCS ${APP_PROTOCOL_JSON_SRCS}
        INCLUDE_DIRS
            include
            third_party/cJSON
        REQUIRES
            app_protocol
    )
else()
    add_library(app_protocol_json STATIC ${APP_PROTOCOL_JSON_SRCS})

    target_include_directories(app_protocol_json PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/third_party/cJSON
    )

    target_link_libraries(app_protocol_json PUBLIC
        app_protocol
    )
endif()