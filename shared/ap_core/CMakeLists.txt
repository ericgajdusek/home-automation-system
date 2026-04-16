set(AP_CORE_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/src/ap_message_ids.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ap_message_payloads.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ap_message_results.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ap_message_validate.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ap_message.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ap_version.c
)

if(ESP_PLATFORM)
    idf_component_register(
        SRCS ${AP_CORE_SRCS}
        INCLUDE_DIRS include
    )
else()
    add_library(ap_core STATIC ${AP_CORE_SRCS})

    target_include_directories(ap_core PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
    )
endif()

