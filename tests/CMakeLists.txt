add_executable(rectest ./receivingtests.c)
add_executable(movsec ./movesection.c)
add_executable(fnv1a ./fnv1a.c)

target_link_libraries(http PRIVATE core_logic)
target_link_libraries(rectest PRIVATE core_logic "-Wl,--wrap,read")
target_link_libraries(movsec PRIVATE core_logic)
target_link_libraries(fnv1a PRIVATE core_logic m)
