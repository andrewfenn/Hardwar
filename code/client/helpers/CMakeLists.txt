
set(helper_source "/src/Fader.cpp")

source_files("helper_source")
set(client_source ${client_source} ${helper_source} PARENT_SCOPE)

include_directories("include")
