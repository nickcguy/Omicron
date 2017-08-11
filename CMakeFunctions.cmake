

MACRO(ADD_SOURCE_LIBRARY includePath srcPath)
    SET(tmp_Header )
    SET(tmp_Source )
    FILE(GLOB_RECURSE tmp_Header "${includePath}/*.h" "${includePath}/*.hpp")
    FILE(GLOB_RECURSE tmp_Source "${srcPath}/*.c" "${srcPath}/*.cpp")

    SET(SOURCE_FILES ${SOURCE_FILES} ${tmp_Source})
    SET(HEADER_FILES ${HEADER_FILES} ${tmp_Header})

    INCLUDE_DIRECTORIES(${includePath})

ENDMACRO()