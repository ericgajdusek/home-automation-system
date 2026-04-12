set(APP_PROTOCOL_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/src/app_protocol_ids.c
    ${CMAKE_CURRENT_LIST_DIR}/src/app_protocol_message.c
    ${CMAKE_CURRENT_LIST_DIR}/src/app_protocol_validate.c
    ${CMAKE_CURRENT_LIST_DIR}/src/app_protocol_payloads.c
    ${CMAKE_CURRENT_LIST_DIR}/src/app_protocol_results.c
    ${CMAKE_CURRENT_LIST_DIR}/src/app_protocol_version.c
)

if(ESP_PLATFORM)
    idf_component_register(
        SRCS ${APP_PROTOCOL_SRCS}
        INCLUDE_DIRS include
    )
else()
    add_library(app_protocol STATIC ${APP_PROTOCOL_SRCS})

    target_include_directories(app_protocol PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
    )
endif()

