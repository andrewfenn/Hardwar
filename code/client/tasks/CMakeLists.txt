
set(task_source
    "src/InputTask.cpp"
    "src/GuiTask.cpp"
    "src/NetworkTask.cpp"
)

source_files("task_source")
set(client_source ${client_source} ${task_source} PARENT_SCOPE)

include_directories("include")

