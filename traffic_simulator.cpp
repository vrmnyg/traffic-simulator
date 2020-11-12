#include "traffic_simulator.h"

Traffic_Simulator::Traffic_Simulator( std::istream& in ): width(0), height(0), size(0), map(0)
{
	width = Read_32bit_Uint( in );
	height = Read_32bit_Uint( in );
	size = width * height;
	map = new unsigned char[size];
	Read_Map_Data( in );
	Init_Nodes();
	Add_Queues();
}

Traffic_Simulator::~Traffic_Simulator()
{
	delete[] map;
}

// Kopiokonstruktori sekä sijoitusoperaattori.
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

// Lue 32-bittinen etumerkitön kokonaisluku.
unsigned int Traffic_Simulator::Read_32bit_Uint( std::istream& in )
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

// Lue karttatiedosto ja tallenna se taulukkoon.
// Jos löytyy jokin muu merkki kuin building tai road,
// heitä poikkeus.
void Traffic_Simulator::Read_Map_Data( std::istream& in )
{
	try
	{
		unsigned int current_index = 0;
		while ( current_index < size )
		{
			char sequence_length = in.get();
			if ( sequence_length < 1 )
			{
				char character = in.get();
				if ( ( character == building ) || ( character == road ) )
				{
					for ( int i = 0; i < -sequence_length; ++i )
					{
						map[current_index++] = character;
					}
				}
				else
				{
					throw Exception( current_index, character );
				}
			}
			for ( int i = 0; i < sequence_length; ++i )
			{
				char character = in.get();
				if ( ( character == building ) || ( character == road ) )
				{
					map[current_index++] = character;
				}
				else
				{
					throw Exception( current_index, character );
				}
			}
		}
	}
	catch ( Exception& e )
	{
		e.Handle();
	}
}

// Luodaan jokaista kartan road-charia kohden oma node tyyppinen olionsa sectors-containeriin.
// Koordinaatteihin lisätään yksi, jotteivat ne alkaisi nollasta.
// Alustetaan node-olioiden väliset liittymät vastaamaan karttatiedostoa.
// Ne nodet, joista ei pääse mihinkään alustetaan nollaksi noden konstruktorissa.
void Traffic_Simulator::Init_Nodes()
{
	for ( unsigned int h = 0; h < height; ++h )
	{
		for ( unsigned int w = 0; w < width; ++w )
		{
			if ( map[h * width + w] == road )
			{

				sectors.push_back( Node( h + 1, w + 1 ) );
			}
		}
	}
	for ( unsigned int i = 0; i < sectors.size(); ++i )
	{
		Node* node = &sectors[i];
		for ( unsigned int j = 0; j < sectors.size(); ++j )
		{
			Node* node2 = &sectors[j];
			if ( node->Get_Vertical_Position() == node2->Get_Vertical_Position() && ( node->Get_Horizontal_Position() + 1 ) == node2->Get_Horizontal_Position() )
			{
				node->Set_Right_Node( node2 );
			}
			if ( node->Get_Vertical_Position() == node2->Get_Vertical_Position() && ( node->Get_Horizontal_Position() - 1 ) == node2->Get_Horizontal_Position() )
			{
				node->Set_Left_Node( node2 );
			}
			if ( ( node->Get_Vertical_Position() + 1 ) == node2->Get_Vertical_Position() && node->Get_Horizontal_Position() == node2->Get_Horizontal_Position() )
			{
				node->Set_Lower_Node( node2 );
			}
			if ( ( node->Get_Vertical_Position() - 1 ) == node2->Get_Vertical_Position() && node->Get_Horizontal_Position() == node2->Get_Horizontal_Position() )
			{
				node->Set_Upper_Node( node2 );
			}
		}
	}
}

// Kaikkiin niihin node-olioihin liittyy jono, jotka ovat risteyksiä.
// Tällaiset node-osoitin/jono parit tallennetaan mappiin.
void Traffic_Simulator::Add_Queues()
{
	for ( unsigned int i = 0; i < sectors.size(); ++i )
	{
		unsigned int count = 0;
		if ( sectors[i].Get_Left_Node() == 0 ) ++count;
		if ( sectors[i].Get_Right_Node() == 0 ) ++count;
		if ( sectors[i].Get_Upper_Node() == 0 ) ++count;
		if ( sectors[i].Get_Lower_Node() == 0 ) ++count;
		if ( count < 2 )
		{
			intersections.insert( std::pair< Node*, Queue >( &sectors[i], Queue( sectors[i].Get_Vertical_Position(), sectors[i].Get_Horizontal_Position() ) ) );
		}
	}
}

// Startup_sectors-vectoriin lisätään osoittimet kaikkiin niihin node-olioihin,
// jotka voidaan antaa autolle aloitusruuduksi.
void Traffic_Simulator::Init_Startup_Sectors( Startup_Sector_Container& startup_sectors )
{
	for ( unsigned int i = 0; i < sectors.size(); ++i )
	{
		unsigned int count = 0;
		if ( sectors[i].Get_Left_Node() == 0 ) ++count;
		if ( sectors[i].Get_Right_Node() == 0 ) ++count;
		if ( sectors[i].Get_Upper_Node() == 0 ) ++count;
		if ( sectors[i].Get_Lower_Node() == 0 ) ++count;
		if ( count == 3 )
		{
			startup_sectors.push_back( &sectors[i] );
		}
	}
}

// Laske autojen maksimimäärä.
unsigned int Traffic_Simulator::Count_Max_Number_Of_Cars() const
{
	unsigned int number_of_roadblocks = 0;
	for ( unsigned int i = 0; i < size; ++i )
	{
		if ( map[i] == road )
		{
			++number_of_roadblocks;
		}
	}
	return number_of_roadblocks/2;
}

// Luodaan uusi auto.
// Suunnaksi alustetaan ainoa mahdollinen suunta.
Car Traffic_Simulator::Create_Car( const Node& road, unsigned int& id, unsigned int current )
{
	int vertical_direction = 0;
	int horizontal_direction = 0;
	if ( road.Get_Left_Node() != 0 )
	{
		horizontal_direction = -1;
	}
	else if ( road.Get_Right_Node() != 0 )
	{
		horizontal_direction = 1;
	}
	else if ( road.Get_Upper_Node() != 0 )
	{
		vertical_direction = -1;
	}
	else if ( road.Get_Lower_Node() != 0 )
	{
		vertical_direction = 1;
	}
	else
	{
		std::cout << "Error!\n";
		std::cout << "Traffic_Simulator::Create_Car()!\n";
		throw -1;
	}
	// Aloituspaikka, id, luontikierros, suunta
	return Car( &road, id++, current, vertical_direction, horizontal_direction );
}

// Lasketaan auton liike nykyisellä kierroksella.
// Jos saavutaan risteykseen lisätään jonoon.
// Jos ei risteys, siirrytään seuraavaan pisteeseen.
// Palautetaan true, jos poistumassa kartalta, muuten false.
bool Traffic_Simulator::Calculate_Movement( Car& car )
{
	const Node* position = car.Get_Position();
	int vertical_direction = car.Get_Vertical_Direction();
	int horizontal_direction = car.Get_Horizontal_Direction();
	const Node* up = position->Get_Upper_Node();
	const Node* down = position->Get_Lower_Node();
	const Node* left = position->Get_Left_Node();
	const Node* right = position->Get_Right_Node();
	// Lasketaan mahdollisten suuntien määrä pisteessä.
	int count = 0;
	if ( up != 0 ) ++count;
	if ( down != 0 ) ++count;
	if ( left != 0 ) ++count;
	if ( right != 0 ) ++count;
	// Jos yli kaksi tällöin ollaan risteyksessä
	if ( count > 2 )
	{
		// Arvotaan uusi suunta ja lisätään jonoon vain jos ei jo ole jonossa
		// Risteykseen saavuttaessa lisätään yksi risteyskerta tilastointia varten.
		// Jokaisella jonotuskierroksella lisätään yksi jonotuskierros.
		Queue* queue = Get_Intersection( *position );
		if ( !( queue->Is_In_Queue( car ) ) )
		{
			// Tarkistetaan että suunnassa on tie ja ettei palata suuntaan josta tultiin.
			bool left_allowed = ( left != 0 && !( horizontal_direction == 1 ) );
			bool right_allowed = ( right != 0 && !( horizontal_direction == -1 ) );
			bool up_allowed = ( up != 0 && !( vertical_direction == 1 ) );
			bool down_allowed = ( down != 0 && !( vertical_direction == -1 ) );
			bool finished = false;
			// Arvotaan uusi suunta ja lisätään sen jälkeen jonoon.
			unsigned int random = rand() % 4 + 1;
			while ( !finished )
			{
				switch ( random )
				{
				case 1:
					if ( !left_allowed )
					{
						++random;
						continue;
					}
					car.Set_Direction_LEFT();
					finished = true;
					break;
				case 2:
					if ( !right_allowed )
					{
						++random;
						continue;
					}
					car.Set_Direction_RIGHT();
					finished = true;
					break;
				case 3:
					if ( !up_allowed )
					{
						++random;
						continue;
					}
					car.Set_Direction_UP();
					finished = true;
					break;
				case 4:
					if ( !down_allowed )
					{
						random = 1;
						continue;
					}
					car.Set_Direction_DOWN();
					finished = true;
					break;
				default:
					std::cout << "Error!\n";
					throw -1;
					break;
				}
			}
			car.Add_Intersection_Time();
			queue->Add_Car_To_Queue( car );
		}
		else
		{
			car.Add_Intersection_Turn();
		}
	}
	// Normaali tienpätkä tai aloitus/lopetuspaikka.
	else
	{
		// Nykyinen suunta oikealle. Suunnaksi ei voi asettaa vasen.
		if ( horizontal_direction == 1 )
		{
			// Suora. Säilytä suunta.
			if ( right != 0 ) car.Move();
			// Mutka. Muuta suuntaa.
			else if ( up != 0 )
			{
				car.Set_Direction_UP();
				car.Move();
			}
			// Mutka. Muuta suuntaa.
			else if ( down != 0 )
			{
				car.Set_Direction_DOWN();
				car.Move();
			}
			// Poistumassa. Palauta tosi.
			else if ( left != 0 )
			{
				return true;
			}
		}
		else if ( horizontal_direction == -1 )
		{
			if ( left != 0 ) car.Move();
			else if ( up != 0 )
			{
				car.Set_Direction_UP();
				car.Move();
			}
			else if ( down != 0 )
			{
				car.Set_Direction_DOWN();
				car.Move();
			}
			else if ( right != 0 )
			{
				return true;
			}
		}
		else if ( vertical_direction == 1 )
		{
			if ( down != 0 ) car.Move();
			else if ( right != 0 )
			{
				car.Set_Direction_RIGHT();
				car.Move();
			}
			else if ( left != 0 )
			{
				car.Set_Direction_LEFT();
				car.Move();
			}
			else if ( up != 0 )
			{
				return true;
			}
		}
		else if ( vertical_direction == -1 )
		{
			if ( up != 0 ) car.Move();
			else if ( right != 0 )
			{
				car.Set_Direction_RIGHT();
				car.Move();
			}
			else if ( left != 0 )
			{
				car.Set_Direction_LEFT();
				car.Move();
			}
			else if ( down != 0 )
			{
				return true;
			}
		}
		else
		{
			std::cout << "Error!\n";
			std::cout << "Traffic_Simulator::Calculate_Movement()!\n";
			throw -1;
		}
	}
	return false;
}

// Hae parametrina annettavaan nodeen liittyvä jono.
Queue* Traffic_Simulator::Get_Intersection( const Node& road )
{
	return &intersections.find( &road )->second;
}

// Kartan tulostus. Siirretään tilastot uuteen mappiin niin että jonotusten määrä on nyt avaimena.
// Tällöin multimap on automaattisesti käänteisessä järjestyksessä tulostamista varten.
// Lisätään viiden viimeisenä mapissa olevan risteyksen sijaintien kohdalle oikea numero kartalle,
// ja tulostetaan.
void Traffic_Simulator::Print_Map( const Queue_Statistics_Container& intersection_statistics )
{
	unsigned char numbers[] = { '1', '2', '3', '4', '5' };
	Statistics_Multimap_Container statistics;
	Statistics_Multimap_Iterator multimap_iterator;
	Queue_Statistics_Iterator statistics_iterator;
	unsigned int s = 0;
	for ( statistics_iterator = intersection_statistics.begin(); statistics_iterator != intersection_statistics.end(); ++statistics_iterator )
	{
		statistics.insert( std::make_pair( statistics_iterator->second, statistics_iterator->first ) );
	}
	multimap_iterator = statistics.end(); --multimap_iterator;
	while ( s < 5 )
	{
		unsigned int v = multimap_iterator->second->Get_Vertical_Position();
		unsigned int h = multimap_iterator->second->Get_Horizontal_Position();
		map[( v - 1 ) * width + ( h - 1 )] = numbers[s];
		--multimap_iterator; ++s;
	}
	for ( unsigned int h = 0; h < height; ++h )
	{
		for ( unsigned int w = 0; w < width; ++w )
		{
			std::cout << map[h * width + w];
		}
		std::cout << "\n";
	}
}

// Simulaattorin suoritus.
void Traffic_Simulator::Execute()
{
	unsigned int current_turn = 1;
	unsigned int id = 1;
	unsigned int number_of_turns = 1000000;
	unsigned int max_number_of_cars = Count_Max_Number_Of_Cars();
	unsigned int average_time_in_town = 0;
	double average_time_in_intersection = 0;
	Car_Container cars;
	Car_Iterator car_iterator;
	Queue_Statistics_Container intersection_statistics;
	Startup_Sector_Container startup_sectors;
	Init_Startup_Sectors( startup_sectors );

	std::cout << "running..." << std::endl;

	while ( current_turn <= number_of_turns )
	{
		// Luodaan uusi auto tarvittaessa.
		// Metodin ensimmäiseksi parametriksi annetaan sattumanvarainen aloitusruutu.
		// startup_sectors sisältää osoittimet kaikkiin niihin node-olioihin, jotka voidaan
		// antaa autolle aloituspisteeksi.
		// Muut parametrit ovat id, sekä luontikierros tilastointia varten.
		if ( cars.size() < max_number_of_cars )
		{
			cars.push_back( Create_Car( *( startup_sectors[rand() % startup_sectors.size()] ), id, current_turn ) );
		}
		// Poistetaan kaupungista poistuvat autot listasta, tai lasketaan ei
		// poistumassa oleville uudet koordinaatit.
		// Kutsutaan ei risteyksessä olevien autojen move()-metodia.
		// Risteykseen saapuville autoille arvotaan uusi suunta ja ne lisätään
		// kyseiseen risteykseen liittyvään jonoon.
		// Risteyksessä jo jonottaville autoille lisätään yksi jonotus.
		for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
		{
			if ( Calculate_Movement( *car_iterator ) )
			{
				static unsigned int turns_in_town = 0;
				static double average = 0;
				static unsigned int cars_deleted = 0;
				average = average + ( *car_iterator ).Get_Average_Intersection_Time();
				turns_in_town = turns_in_town + ( current_turn - ( *car_iterator ).Get_Turn_Created() ) + 1;
				car_iterator = cars.erase( car_iterator );
				++cars_deleted;
				average_time_in_town = turns_in_town/cars_deleted;
				average_time_in_intersection = average/cars_deleted;
			}
		}
		// Kutsutaan niiden risteysten move_cars()-metodia, joissa on odottavia autoja.
		// Vuorossa olevat autot pääsevät liikkeelle risteyksistä.
		// Katsotaan jäikö risteykseen jonottavia autoja ja päivitetään tilastot jos näin on.
		for ( intersections_iterator = intersections.begin(); intersections_iterator != intersections.end(); ++intersections_iterator )
		{
			if ( !intersections_iterator->second.Is_Empty() )
			{
				intersections_iterator->second.Move_Cars();
				if ( !intersections_iterator->second.Is_Empty() )
				{
					int size = intersections_iterator->second.Get_Queue_Size();
					unsigned int& s = intersection_statistics[&intersections_iterator->second];
					s = s + size;
				}
			}
		}
		++current_turn;
	}
	std::cout << "Autojen keskimaarainen kaupingissa vietetty aika: " << average_time_in_town << " kierrosta\n";
	std::cout << "Autojen keskimaarainen risteyksissa vietetty aika: " << average_time_in_intersection << " kierrosta\n";
	std::cout << "-------------------" << "\n";
	Print_Map( intersection_statistics );
	std::cout << "-------------------" << "\n";
}
