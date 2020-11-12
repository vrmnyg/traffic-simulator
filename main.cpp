#include <fstream>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "traffic_simulator.h"

// Harjoitustyö kurssille Järjestelmäohjelmointi C++-kielellä
// Tekijä: Ville Nygård 70229

int main( int argc, const char** argv )
{
	if ( argc < 2 )
	{
		std::cout << "virhe: anna karttatiedosto parametrina.\n";
		return -1;
	}
	std::ifstream map_file( argv[1] );
	if ( map_file.is_open() )
	{
		srand( time( NULL ) );
		Traffic_Simulator ts( map_file );
		ts.Execute();
	}
	else
	{
		std::cout << "virhe: karttatiedostoa '" << argv[1] << "' ei loytynyt.\n";
		return -1;
	}
}