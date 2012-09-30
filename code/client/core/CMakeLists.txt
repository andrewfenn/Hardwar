
if (UNIX)
    set(core_source
        "src/GameRootLinux.cpp"
    )
endif()

if (WIN32)
    set(core_source
        "src/GameRootWin.cpp"
    )
endif()

set(core_source
    ${core_source}
    "src/GameRoot.cpp"
    "src/GameManager.cpp"
    "src/RootGameState.cpp"
    "src/GameState.cpp"
)

source_files("core_source")
set(client_source ${client_source} ${core_source} PARENT_SCOPE)

include_directories("include")

