set_symbols( "debug" );

add_rules("plugin.vsxmake.autoupdate")

set_targetdir "bin"
set_objectdir "build/obj"

target "img2table"
    set_languages( "cxx20" )
    set_kind "binary"
    set_default(true)

    add_files( "src/**.cpp" )
    add_headerfiles( "src/**.h" )
target_end()