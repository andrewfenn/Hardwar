
set(menustate_source
    "src/MenuState.cpp"
)

source_files("menustate_source")
set(client_source ${client_source} ${menustate_source} PARENT_SCOPE)

include_directories("include")

