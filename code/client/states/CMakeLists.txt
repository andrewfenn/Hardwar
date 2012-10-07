
add_subdirectory("menu/")

set(state_source
    "src/PlayState.cpp"
)

source_files("state_source")
set(client_source ${client_source} ${state_source} PARENT_SCOPE)

include_directories("include")

