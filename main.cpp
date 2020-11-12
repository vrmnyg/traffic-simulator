#include <fstream>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "traffic_simulator.h"

//g++ -c -Wall -ansi main.cpp traffic_simulator.cpp intersection.cpp car.cpp
//g++ -o traffic.exe main.o traffic_simulator.o intersection.o car.o
//.\traffic.exe map.dat

//valgrind -v --leak-check=yes ./traffic map.dat

//g++ -c -pg -Wall -ansi main.cpp traffic_simulator.cpp intersection.cpp car.cpp
//g++ -pg -o traffic.exe main.o traffic_simulator.o intersection.o car.o
//.\traffic.exe map.dat
//gprof traffic.exe > stats

int main( int argc, const char** argv )
{
	if ( argc < 2 )
	{
		std::cout << "Anna skriptitiedosto parametrina.\n";
		return -1;
	}
	srand( time(NULL) );
	std::ifstream script_file( argv[1] );
	//std::ifstream script_file2( argv[1] );
	Traffic_Simulator ts( script_file );
	//Traffic_Simulator ts2( script_file2 );
	//Traffic_Simulator ts3( ts );
	//ts2 = ts3;
	ts.Execute();
}