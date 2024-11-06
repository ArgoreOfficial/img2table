#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char* toPixelFunc =
"function img:toPixelData() : PixelData\n"
"	local pd = PixelData.new( img.width, img.height, color.clear )\n"
"	local p : number, v : number, r : number, g : number, b : number\n"
"	\n"
"	for y = 1, img.height do\n"
"		for x = 1, img.width do\n"
"			p = ( y - 1 ) * img.width + ( x - 1 ) -- pixel index\n"
"			v = p * img.channels -- data offset\n"
"			\n"
"			r = img.data[ v + 1 ]\n"
"			g = img.data[ v + 2 ] or 0\n"
"			b = img.data[ v + 3 ] or 0\n"
"			pd:SetPixel( x, y, Color( r, g, b ) )\n"
"		end\n"
"	end\n"
"	return pd\n"
"end\n";

int main( int argc, char* argv[] )
{
	int width, height, channels;
	unsigned char* img;
	char* path = "./lua.png";

	if ( argc > 1 )
		path = argv[ 1 ];
	img = stbi_load( path, &width, &height, &channels, 0 );

	if ( img == NULL )
		return 1;
	
	char* name = 0; ( name = strrchr( path, '/' ) ) ? ++name : ( name = path );

	char outfilename[ 128 ];
	strcpy( outfilename, name );
	strcat( outfilename, ".lua" );

	FILE* pFile = fopen( outfilename, "w" );

	fprintf( pFile, "local img = {}\n" );
	fprintf( pFile, "img.width = %i\n", width );
	fprintf( pFile, "img.height = %i\n", height );
	fprintf( pFile, "img.channels = %i\n", channels );

	fprintf( pFile, "img.data = {\n  " );
	for ( size_t i = 0; i < width * height * channels; i++ )
		fprintf( pFile, "%i,", img[ i ] );

	fprintf( pFile, "\n}\n\n" );
	fprintf( pFile, "%s\n", toPixelFunc );

	fprintf( pFile, "return img\n" );

	stbi_image_free( img );

	return 0;
}