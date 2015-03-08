

set(task_source
    "src/InputTask.cpp"
    "src/NetworkTask.cpp"
    "src/GuiTask.cpp"
    "src/rocket/RenderInterfaceOgre3D.cpp"
    "src/rocket/SystemInterfaceOgre3D.cpp"
)

source_files("task_source")
set(client_source ${client_source} ${task_source} PARENT_SCOPE)

include_directories("include")