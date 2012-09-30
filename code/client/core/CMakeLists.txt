
if (UNIX)
    set(client_source
        "src/GameRootLinux.cpp"
    )
endif()

if (WIN32)
    set(client_source
        "src/GameRootWin.cpp"
    )
endif()

set(client_source
    ${client_source}
    "src/GameRoot.cpp"
    "src/GameManager.cpp"
    "src/RootGameState.cpp"
    "src/GameState.cpp"
)

source_files("client_source")
set(client_source ${client_source} PARENT_SCOPE)

include_directories("include")

