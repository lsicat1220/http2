add_library(core_logic STATIC
	./receiving.c
	./services.c
	./parsing.c
	./map.c
	./utils.c
)

target_include_directories(core_logic PUBLIC ../include)

add_executable(http ./main.c)

target_link_libraries(http PRIVATE core_logic)
