#ifndef TRAFFIC_SIMULATOR_H
#define TRAFFIC_SIMULATOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "startup_sector.h"
#include "car.h"
#include "intersection.h"

//#define PRINT

typedef std::map< std::pair< unsigned int, unsigned int >, Intersection* >::iterator intersections_map_iterator;
//typedef std::map< std::pair< unsigned int, unsigned int >, unsigned int >::iterator statistics_map_iterator;
typedef std::map< Intersection*, unsigned int >::iterator statistics_map_iterator;
typedef std::multimap< unsigned int, Intersection* >::iterator multimap_iterator;
//typedef std::multimap< unsigned int, std::pair< unsigned int, unsigned int > >::iterator multimap_iterator;

class Traffic_Simulator
{
public:
	Traffic_Simulator( std::istream& );
	~Traffic_Simulator();
	void Execute();
private:
	Traffic_Simulator( const Traffic_Simulator& );
 	Traffic_Simulator& operator=( const Traffic_Simulator& );
	bool Calculate_Movement( Car* );
	void Print_Map();
	Car* Create_Car( unsigned int&, unsigned int );
	void Delete_Car( int );
	bool Is_Leaving_Area( Car* );
	Intersection* Get_Intersection( unsigned int, unsigned int );
	Intersection* Get_Intersection( std::pair< unsigned int, unsigned int > );
	void Init_Intersections();
	Startup_Sector* Get_Random_Startup_Sector();
	void Init_Startup_Sectors();
	unsigned int Read_32bit_Uint( std::istream& );
	void Read_Map_Data( std::istream& );
	void Count_Number_Of_Roadblocks();
	void Calculate();
	void Update_Statistics();
	void Update_Intersections();
	void Print_Car_Statistics();
	static unsigned char const building = '#';
	static unsigned char const road  = ' ';
	unsigned int number_of_roadblocks;
	unsigned int width;
	unsigned int height;
	unsigned int size;
	std::map< std::pair< unsigned int, unsigned int >, Intersection* > intersections;
	//std::map< std::pair< unsigned int, unsigned int >, unsigned int > intersection_statistics;
	std::map< Intersection*, unsigned int > intersection_statistics;
	std::vector< Startup_Sector* > sectors;
	std::vector< Car* > cars;
	unsigned char* map;
};
#endif