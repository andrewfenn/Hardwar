
# Nice little helper function for making sure the full path of the source file is
# tacked on when we come to including the files in the unit tests

macro(source_files name)
    set(_temp_source_files "")
    foreach(src_file ${${name}})
        string(STRIP ${src_file} src_file)
        set("_temp_source_files" "${_temp_source_files}${CMAKE_CURRENT_SOURCE_DIR}/${src_file};")
    endforeach()
    set(${name} ${_temp_source_files})
endmacro(source_files)

