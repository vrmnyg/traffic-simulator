#ifndef TRAFFIC_SIMULATOR_H
#define TRAFFIC_SIMULATOR_H

#include <iostream>
#include <cstdlib>
#include <list>
#include <map>
#include <vector>
#include "exception.h"
#include "node.h"
#include "car.h"
#include "queue.h"

class Traffic_Simulator
{
public:
	explicit Traffic_Simulator( std::istream& );
	~Traffic_Simulator();
	void Execute();
private:
	Traffic_Simulator( const Traffic_Simulator& );
 	Traffic_Simulator& operator=( const Traffic_Simulator& );
	// Iteraattorit
	typedef std::map< const Node*, Queue >::iterator Queue_Iterator;
	typedef std::map< Queue*, unsigned int >::const_iterator Queue_Statistics_Iterator;
	typedef std::multimap< unsigned int, Queue* >::iterator Statistics_Multimap_Iterator;
	typedef std::list< Car >::iterator Car_Iterator;
	// Containerit
	typedef std::map< const Node*, Queue > Queue_Container;
	typedef std::map< Queue*, unsigned int > Queue_Statistics_Container;
	typedef std::multimap< unsigned int, Queue* > Statistics_Multimap_Container;
	typedef std::list< Car > Car_Container;
	typedef std::vector< Node > Node_Container;
	typedef std::vector< const Node* > Startup_Sector_Container;
	//
	unsigned int Read_32bit_Uint( std::istream& );
	void Read_Map_Data( std::istream& );
	void Init_Nodes();
	void Add_Queues();
	void Init_Startup_Sectors( Startup_Sector_Container& );
	unsigned int Count_Max_Number_Of_Cars() const;
	Car Create_Car( const Node&, unsigned int&, unsigned int );
	bool Calculate_Movement( Car& );
	Queue* Get_Intersection( const Node& );
	void Print_Map( const Queue_Statistics_Container& );
	static unsigned char const building = '#';
	static unsigned char const road  = ' ';
	unsigned int width;
	unsigned int height;
	unsigned int size;
	Node_Container sectors;
	Queue_Container intersections;
	Queue_Iterator intersections_iterator;
	unsigned char* map;
};
#endif