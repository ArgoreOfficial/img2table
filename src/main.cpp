#include <stdio.h>
#include <stdint.h>
#include <fstream>

#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool compact  = true;
bool compress = false;

std::vector<uint32_t> compression_stack{};

union color
{
	struct
	{ uint8_t r, g, b, a;
	};
	uint32_t uint32;
};

size_t encode( std::ofstream& _ofstream , uint8_t* _pBuffer, size_t _index, uint32_t _numChannels )
{
	compression_stack.clear();

	uint32_t val = 0;
	do
	{
		if ( compact )
		{
			color c;
			c.r = ( _numChannels >= 1 ) ? _pBuffer[ _index ]     : 0;
			c.g = ( _numChannels >= 1 ) ? _pBuffer[ _index + 1 ] : 0;
			c.b = ( _numChannels >= 1 ) ? _pBuffer[ _index + 2 ] : 0;
			c.a = ( _numChannels >= 1 ) ? _pBuffer[ _index + 3 ] : 255;
			
			val = c.uint32;
			_index += _numChannels;
		}
		else
		{
			val = static_cast<uint32_t>( _pBuffer[ _index ] );
			_index++;
		}
		
		compression_stack.push_back( val );
	} while ( _pBuffer[ _index ] == val );

	
	if( compression_stack.size() == 1 )
		_ofstream << val << ",";
	else
	{
		if( compress )
			_ofstream << "{" << val << "," << compression_stack.size() << "},";
		else
			for ( uint32_t v : compression_stack  )
				_ofstream << v << ",";
	}

	return _index;
}

int main( int argc, char* argv[] )
{
	int width, height, channels;
	unsigned char* img;
	char* path = (char*)"./pop.png";

	if ( argc > 1 )
		path = argv[ 1 ];
	img = stbi_load( path, &width, &height, &channels, 0 );

	if ( img == NULL )
		return 1;
	
	char* name = 0; ( name = strrchr( path, '/' ) ) ? ++name : ( name = path );

	char outfilename[ 128 ];
	strcpy( outfilename, name );
	strcat( outfilename, ".lua" );

	std::ofstream outFile( outfilename );
	std::ifstream funcFile( "../funcs/decode.lua" );

	outFile << "local img = {}\n";
	outFile << "img.width = " << width << "\n";
	outFile << "img.height = " << height << "\n";
	outFile << "img.channels = " << channels << "\n";
	outFile << "img.vals_per_px = " << ( compact ? "1" : "4" ) << "\n";

	outFile << "img.data = {\n  ";

	for ( size_t i = 0; i < width * height * channels; )
		i = encode( outFile, img, i, channels );

	outFile << "\n}\n\n";
	outFile << funcFile.rdbuf();
	outFile << "\nreturn img\n";

	stbi_image_free( img );

	return 0;
}