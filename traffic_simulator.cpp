#include "traffic_simulator.h"

Traffic_Simulator::Traffic_Simulator( std::istream& in ): number_of_roadblocks(0), width(0), height(0), size(0), map(0)
{
	width = Read_32bit_Uint( in );
	height = Read_32bit_Uint( in );
	size = width * height;
	map = new unsigned char[size];
	Read_Map_Data( in );
	Init_Startup_Sectors();
	Init_Intersections();
	Count_Number_Of_Roadblocks();
}

Traffic_Simulator::~Traffic_Simulator()
{
	//std::cout << "Simulaattorin destructori kaynnistyi\n\n";
	delete[] map;
	//std::cout << "Tyhjennetaan sektorivektori\n";
	//std::cout << "-------------------" << std::endl;
	for ( unsigned int i = 0; i < sectors.size(); ++i )
	{
		delete sectors[i];
	}
	sectors.clear();
	//std::cout << "Tyhjennetaan autovektori\n";
	//std::cout << "-------------------" << std::endl;
	for ( unsigned int i = 0; i < cars.size(); ++i )
	{
		delete cars[i];
	}
	cars.clear();
	//std::cout << "Tyhjennetaan risteysvektori\n";
	//std::cout << "-------------------" << std::endl;
	intersections_map_iterator it;
	for ( it = intersections.begin(); it != intersections.end(); it++ )
	{
		delete it->second;
	}
	intersections.clear();
	//intersection_statistics.clear();
	//std::cout << "-------------------" << std::endl;
	//std::cout << "Tuhoaminen loppui" << std::endl;
}

/*Traffic_Simulator::Traffic_Simulator( const Traffic_Simulator& param ): width(param.width), height(param.height), size(param.size)
{
	map = new unsigned char[param.size];
	for( unsigned int i = 0; i < param.size; ++i )
	{
		*( map + i ) = *( param.map + i );
	}
}

Traffic_Simulator& Traffic_Simulator::operator=( const Traffic_Simulator& param )
{
	for( unsigned int i = 0; i < param.size; ++i )
	{
		*( map + i ) = *( param.map + i );
	}
	return *this;
}*/

void Traffic_Simulator::Print_Map()
{
	std::cout << "-------------------" << std::endl;
	unsigned char numbers[] = { '1', '2', '3', '4', '5' };
	std::multimap< unsigned int, Intersection* > statistics;
	//std::multimap< unsigned int, std::pair< unsigned int, unsigned int > > statistics;
	statistics_map_iterator it;
	for ( it = intersection_statistics.begin(); it != intersection_statistics.end(); it++ )
	{
		std::cout << "Risteys: " << it->first->Get_Vertical_Position() << "," << it->first->Get_Horizontal_Position() << " Jonotukset: " << it->second << "\n";
		statistics.insert( std::make_pair( it->second, it->first ) );
	}
	/*for ( it = intersection_statistics.begin(); it != intersection_statistics.end(); it++ )
	{
		std::cout << "Risteys: " << Get_Intersection( it->first )->Get_Vertical_Position() << "," << Get_Intersection( it->first )->Get_Horizontal_Position() << " Jonotukset: " << it->second << "\n";
		statistics.insert( std::make_pair( it->second, it->first ) );
	}*/
	std::cout << "-------------------" << std::endl;
	multimap_iterator it2 = statistics.end(); it2--;
	unsigned int s = 0;
	while ( s < 5 )
	{
		Intersection* inter = it2->second;
		//Intersection* inter = Get_Intersection( it2->second );
		unsigned int v = inter->Get_Vertical_Position();
		unsigned int h = inter->Get_Horizontal_Position();
		map[( v - 1 ) * width + ( h - 1 )] = numbers[s];
		it2--;
		s++;
	}
	for ( unsigned int h = 0; h < height; h++ )
	{
		for ( unsigned int w = 0; w < width; w++ )
		{
			std::cout << map[h * width + w];
		}
		std::cout << std::endl;
	}
}

void Traffic_Simulator::Execute()
{
	int current = 1;
	unsigned int id = 1;
	double average_intersection_times = 0;
	double turns_in_town = 0;
	unsigned int cars_deleted = 0;
	int no_cycles = 1000000;
	unsigned int max_number_of_cars = number_of_roadblocks/2;
	//unsigned int max_number_of_cars = 10;
	//cars.push_back( new Car( 11, 45, 1, current, Car::RIGHT ) );
	//cars.push_back( new Car( 11, 49, 2, current, Car::LEFT ) );
	//cars.push_back( new Car( 9, 47, 3, current, Car::DOWN ) );
	//cars.push_back( new Car( 13, 47, 4, current, Car::UP ) );

	std::cout << "executing..." << std::endl;

	while ( current <= no_cycles )
	{
		#ifdef PRINT
		std::cout << "\nKierros numero " << current << " alkaa.\n";
		std::cout << "-------------------" << std::endl;
		#endif
		unsigned int i = 0;
		if ( cars.size() < max_number_of_cars )
		{
			cars.push_back( Create_Car( id, current ) );
		}
		while ( i < cars.size() )
		{
			if ( Is_Leaving_Area( cars[i] ) )
			{
				#ifdef PRINT
				std::cout << "Auto numero " << cars[i]->Get_Id() << " poistui liikenteesta\n";
				#endif
				average_intersection_times = average_intersection_times + ( cars[i]->Get_Intersection_Turns()/cars[i]->Get_Intersection_Times() );
				turns_in_town = turns_in_town + ( current - cars[i]->Get_Turn_Created() );
				Delete_Car( i );
				cars_deleted++;
			}
			else
			{
				i++;
			}
		}
		Calculate();
		Update_Intersections();
		Update_Statistics();
		#ifdef PRINT
		Print_Car_Statistics();
		#endif
		current++;
	}
	std::cout << "Autojen keskimaarainen risteyksissa vietetty aika: " << ( (double)average_intersection_times/cars_deleted ) << " kierrosta." << "\n";
	std::cout << "Autojen keskimaarainen kaupungissa vietetty aika: " << ( (double)turns_in_town/cars_deleted ) << " kierrosta." << "\n";
	std::cout << "-------------------" << std::endl; std::cout << "Kartan tulostus:" << std::endl;
	Print_Map();
}

void Traffic_Simulator::Calculate()
{
	for ( unsigned int j = 0; j < cars.size(); ++j )
	{
		bool is_intersection = Calculate_Movement( cars[j] );
		if ( !is_intersection )
		{
			cars[j]->Move();
		}
		#ifdef PRINT
		cars[j]->Print_Car();
		#endif
	}
}

void Traffic_Simulator::Update_Statistics()
{
	statistics_map_iterator it2;
	for ( it2 = intersection_statistics.begin(); it2 != intersection_statistics.end(); it2++ )
	{
		Intersection* inter = it2->first;
		if ( inter->Get_Queue_Size() > 0 )
		{
			it2->second = it2->second + inter->Get_Queue_Size();
		}
	}
	/*statistics_map_iterator it2;
	for ( it2 = intersection_statistics.begin(); it2 != intersection_statistics.end(); it2++ )
	{
		Intersection* inter = Get_Intersection( it2->first );
		if ( inter->Get_Queue_Size() > 0 )
		{
			it2->second = it2->second + inter->Get_Queue_Size();
		}
	}*/
}

void Traffic_Simulator::Update_Intersections()
{
	intersections_map_iterator it;
	for ( it = intersections.begin(); it != intersections.end(); it++ )
	{
		it->second->Move_Cars();
	}
}

void Traffic_Simulator::Print_Car_Statistics()
{
	unsigned int x = 0;
	std::cout << "-------------------" << std::endl;
	while ( x < cars.size() )
	{
		std::cout << "Auton numero " << cars[x]->Get_Id() << " risteyksessa vietetyt kierrokset seka risteyskerrat: ";
		std::cout << cars[x]->Get_Intersection_Turns() << "," << cars[x]->Get_Intersection_Times() << "\n";
		x++;
	}
}

Car* Traffic_Simulator::Create_Car( unsigned int& id, unsigned int current )
{
	Startup_Sector* sector = Get_Random_Startup_Sector();
	unsigned int v = sector->Get_Vertical_Position();
	unsigned int h = sector->Get_Horizontal_Position();
	int vertical_direction = 0;
	int horizontal_direction = 0;
	if ( v == 1 ) vertical_direction = 1;
	else if ( h == 1 ) horizontal_direction = 1;
	else if ( v == height ) vertical_direction = -1;
	else if ( h == width ) horizontal_direction = -1;
	/*else
	{
		std::cout << "default!\n";
	}*/
	return new Car( v, h, id++, current, vertical_direction, horizontal_direction );
}

void Traffic_Simulator::Delete_Car( int i )
{
	delete cars[i];
	cars.erase( cars.begin() + i );
}

bool Traffic_Simulator::Is_Leaving_Area( Car* car )
{
	unsigned int vertical_position = car->Get_Vertical_Position();
	unsigned int horizontal_position = car->Get_Horizontal_Position();
	int vertical_direction = car->Get_Vertical_Direction();
	int horizontal_direction = car->Get_Horizontal_Direction();
	return ( ( vertical_position >= height && ( vertical_direction == 1 ) ) ||
	( vertical_position <= 1 && ( vertical_direction == -1 ) ) ||
	( horizontal_position >= width && ( horizontal_direction == 1 ) ) ||
	( horizontal_position <= 1 && ( horizontal_direction == -1 ) ) );
}

bool Traffic_Simulator::Calculate_Movement( Car* car )
{
	unsigned int vertical_position = car->Get_Vertical_Position();
	unsigned int horizontal_position = car->Get_Horizontal_Position();
	int vertical_direction = car->Get_Vertical_Direction();
	int horizontal_direction = car->Get_Horizontal_Direction();
	bool up_allowed = false;
	bool down_allowed = false;
	bool left_allowed = false;
	bool right_allowed = false;
	unsigned char up = building;
	unsigned char down = building;
	unsigned char left = map[( vertical_position - 1 ) * width + ( horizontal_position - 2 )];
	unsigned char right = map[( vertical_position - 1 ) * width + ( horizontal_position )];
	if ( vertical_position > 1 )
	{
		up = map[( vertical_position - 2 ) * width + ( horizontal_position - 1 )];
	}
	if ( vertical_position < height )
	{
		down = map[( vertical_position ) * width + ( horizontal_position - 1 )];
	}
	int count = 0;
	if ( up == road )
	{
		if ( !( vertical_direction == 1 ) ) up_allowed = true;
		count++;
	}
	if ( down == road )
	{
		if ( !( vertical_direction == -1 ) ) down_allowed = true;
		count++;
	}
	if ( left == road )
	{
		if ( !( horizontal_direction == 1 ) ) left_allowed = true;
		count++;
	}
	if ( right == road )
	{
		if ( !( horizontal_direction == -1 ) ) right_allowed = true;
		count++;
	}
	if ( count > 2 )
	{
		Intersection* inter = Get_Intersection( vertical_position, horizontal_position );
		if ( !( inter->Is_In_Queue( car ) ) )
		{
			#ifdef PRINT
			std::cout << "Auto numero " << car->Get_Id() << " saapui risteykseen\n\n";
			#endif
			bool finished = false;
			while ( !finished )
			{
				unsigned int random = rand() % 4 + 1;
				switch ( random )
				{
				case 1:
					if ( !left_allowed ) continue;
					car->Set_Direction_LEFT();
					finished = true;
					break;
				case 2:
					if ( !right_allowed ) continue;
					car->Set_Direction_RIGHT();
					finished = true;
					break;
				case 3:
					if ( !up_allowed ) continue;
					car->Set_Direction_UP();
					finished = true;
					break;
				case 4:
					if ( !down_allowed ) continue;
					car->Set_Direction_DOWN();
					finished = true;
					break;
				default:
					std::cout << "default!!!!" << std::endl;
					//finished = true;
					exit(1);
					break;
				}
			}
			inter->Add_Car_To_Queue( car );
		}
		return true;
	}
	else
	{
		if ( left_allowed ) car->Set_Direction_LEFT();
		else if ( right_allowed ) car->Set_Direction_RIGHT();
		else if ( up_allowed ) car->Set_Direction_UP();
		else if ( down_allowed ) car->Set_Direction_DOWN();
		/*if ( horizontal_direction == 1 || horizontal_direction == -1 )
		{
			if ( up == road )
			{
				car->Set_Direction_UP();
			}
			else if ( down == road )
			{
				car->Set_Direction_DOWN();
			}
		}
		else if ( vertical_direction == 1 || vertical_direction == -1 )
		{
			if ( right == road )
			{
				car->Set_Direction_RIGHT();
			}
			else if ( left == road )
			{
				car->Set_Direction_LEFT();
			}
		}*/
	}
	return false;
}

Intersection* Traffic_Simulator::Get_Intersection( std::pair< unsigned int, unsigned int > pair )
{
	return intersections[pair];
}

Intersection* Traffic_Simulator::Get_Intersection( unsigned int v, unsigned int h )
{
	return intersections[std::make_pair( v, h )];
}

void Traffic_Simulator::Init_Intersections()
{
	std::cout << "\nAlustetaan risteykset " << "\n";
	std::cout << "-------------------" << std::endl;
	for ( unsigned int h = 1; h < height - 1; h++ )
	{
		for ( unsigned int w = 1; w < width - 1; w++ )
		{
			if ( map[h * width + w] == road )
			{
				int count = 0;
				if ( map[( h - 1 ) * width + w] == road ) count++;
				if ( map[h * width + ( w - 1 )] == road ) count++;
				if ( map[( h + 1 ) * width + w] == road ) count++;
				if ( map[h * width + ( w + 1 )] == road ) count++;
				if ( count > 2 )
				{
					intersections.insert( std::pair< std::pair< unsigned int, unsigned int >, Intersection* >( std::make_pair( h + 1, w + 1 ), new Intersection( h + 1, w + 1 ) ) );
					//intersection_statistics.insert( std::pair< std::pair< unsigned int, unsigned int >, unsigned int >( std::make_pair( h + 1, w + 1 ), 0 ) );
					std::cout << "Risteys pisteessa " << h + 1 << "," << w + 1 << "\n";
				}
			}
		}
	}
	intersections_map_iterator it;
	for ( it = intersections.begin(); it != intersections.end(); it++ )
    {
		intersection_statistics.insert( std::pair< Intersection*, int >( it->second, 0 ) );
	}
	std::cout << "-------------------" << std::endl;
}

Startup_Sector* Traffic_Simulator::Get_Random_Startup_Sector()
{
	return sectors[rand() % sectors.size()];
}

void Traffic_Simulator::Init_Startup_Sectors()
{
	std::cout << "\nAlustetaan aloituspaikat " << "\n";
	std::cout << "-------------------" << std::endl;
	for ( unsigned int i = 0; i < height; ++i )
	{
		if ( map[i * width] == road )
		{
			sectors.push_back( new Startup_Sector( i + 1, 1 ) );
			std::cout << "Aloituspaikka pisteessa " << i + 1 << "," << 1 << "\n";
		}
		if ( map[i * width + width - 1] == road )
		{
			sectors.push_back( new Startup_Sector( i + 1, width ) );
			std::cout << "Aloituspaikka pisteessa " << i + 1 << "," << width << "\n";
		}
	}
	for ( unsigned int i = 0; i < width; ++i )
	{
		if ( map[0 * width + i] == road )
		{
			sectors.push_back( new Startup_Sector( 1, i + 1 ) );
			std::cout << "Aloituspaikka pisteessa " << 1 << "," << i + 1 << "\n";
		}
		if ( map[( height -1 ) * width + i] == road )
		{
			sectors.push_back( new Startup_Sector( height, i + 1 ) );
			std::cout << "Aloituspaikka pisteessa " << height << "," << i + 1 << "\n";
		}
	}
	std::cout << "-------------------" << std::endl;
}

inline unsigned int Traffic_Simulator::Read_32bit_Uint( std::istream& in )
{
	unsigned int i = 0;
	i += in.get();
	i <<= 8;
	i += in.get();
	i <<= 8;
	i += in.get();
	i <<= 8;
	i += in.get();
	return i;
}

void Traffic_Simulator::Read_Map_Data( std::istream& in )
{
	unsigned int current_index = 0;
	while ( current_index < size )
	{
		char sequence_length = in.get();
		if ( sequence_length < 1 )
		{
			char character = in.get();
			for ( int i = 0; i < -sequence_length; ++i )
			{
				map[current_index++] = character;
			}
		}
		for ( int i = 0; i < sequence_length; ++i )
		{
			map[current_index++] = in.get();
		}
	}
}

void Traffic_Simulator::Count_Number_Of_Roadblocks()
{
	for ( unsigned int i = 0; i < size; ++i )
	{
		if ( map[i] == road )
		{
			number_of_roadblocks++;
		}
	}
}
