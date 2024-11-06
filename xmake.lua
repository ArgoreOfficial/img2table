set_languages( "c17" )
set_symbols( "debug" );

add_rules("plugin.vsxmake.autoupdate")

set_targetdir "bin"
set_objectdir "build/obj"

target "img2table"
    set_kind "binary"
    set_default(true)

    add_files( "src/**.c" )
    add_headerfiles( "src/**.h" )
target_end()