add_executable(test test.c)
target_link_libraries(test TeglaMixer)
target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_custom_command(TARGET test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/examples/aryx.s3m
        $<TARGET_FILE_DIR:test>/aryx.s3m
)