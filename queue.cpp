#include "queue.h"

Queue::Queue( unsigned int v, unsigned int h ): vertical(v), horizontal(h)
{

}

Queue::~Queue()
{
	
}

bool Queue::Is_In_Queue( const Car& car )
{
	for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
	{
		if ( *car_iterator == &car ) return true;
	}
	return false;
}

void Queue::Move_Cars()
{
	// Jos risteyksessä vain yksi auto
	if ( cars.size() == 1 )
	{
		Car* current_car = cars.front();
		current_car->Move();
		cars.clear();
	}
	// Risteyksessä useampi kuin yksi auto
	else
	{
		// Onko kyseinen suunta sallittu.
		// Kun suuntaan päästetään auto, asetetaan epätodeksi.
		bool right_allowed = true;
		bool left_allowed = true;
		bool up_allowed = true;
		bool down_allowed = true;
		// Tuleeko suunnasta autoa.
		bool car_coming_from_top = false;
		bool car_coming_from_bottom = false;
		bool car_coming_from_left = false;
		bool car_coming_from_right = false; 
		// Tuleeko suunnasta suoraan tai oikealle menevää autoa.
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
		// Tarkistetaan mistä kaikista suunnista tulee auto.
		for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
		{
			int old_vertical_direction = ( *car_iterator )->Get_Old_Vertical_Direction();
			int old_horizontal_direction = ( *car_iterator )->Get_Old_Horizontal_Direction();
			if ( old_vertical_direction == 1 ) car_coming_from_top = true;
			else if ( old_vertical_direction == -1 ) car_coming_from_bottom = true;
			else if ( old_horizontal_direction == -1 ) car_coming_from_right = true;
			else if ( old_horizontal_direction == 1 ) car_coming_from_left = true;
		}
		// Jos ylhäältä tulee auto, tuleeko suoraan tai oikealle menevää autoa.
		if ( car_coming_from_top )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_vertical_direction = ( *car_iterator )->Get_Old_Vertical_Direction();
				int vertical_direction = ( *car_iterator )->Get_Vertical_Direction();
				int horizontal_direction = ( *car_iterator )->Get_Horizontal_Direction();
				if ( old_vertical_direction == 1 )
				{
					if ( horizontal_direction == -1 || vertical_direction == 1 )
					{
						up = true;
						break;
					}
				}
			}
		}
		// Jos alhaalta tulee auto, tuleeko suoraan tai oikealle menevää autoa.
		if ( car_coming_from_bottom )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_vertical_direction = ( *car_iterator )->Get_Old_Vertical_Direction();
				int vertical_direction = ( *car_iterator )->Get_Vertical_Direction();
				int horizontal_direction = ( *car_iterator )->Get_Horizontal_Direction();
				if ( old_vertical_direction == -1 )
				{
					if ( horizontal_direction == 1 || vertical_direction == -1 )
					{
						down = true;
						break;
					}
				}
			}
		}
		// Jos vasemmalta tulee auto, tuleeko suoraan tai oikealle menevää autoa.
		if ( car_coming_from_left )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_horizontal_direction = ( *car_iterator )->Get_Old_Horizontal_Direction();
				int vertical_direction = ( *car_iterator )->Get_Vertical_Direction();
				int horizontal_direction = ( *car_iterator )->Get_Horizontal_Direction();
				if ( old_horizontal_direction == 1 )
				{
					if ( vertical_direction == 1 || horizontal_direction == 1 )
					{
						left = true;
						break;
					}
				}
			}
		}
		// Jos oikealta tulee auto, tuleeko suoraan tai oikealle menevää autoa.
		if ( car_coming_from_right )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_horizontal_direction = ( *car_iterator )->Get_Old_Horizontal_Direction();
				int vertical_direction = ( *car_iterator )->Get_Vertical_Direction();
				int horizontal_direction = ( *car_iterator )->Get_Horizontal_Direction();
				if ( old_horizontal_direction == -1 )
				{
					if ( vertical_direction == -1 || horizontal_direction == -1 )
					{
						right = true;
						break;
					}
				}
			}
		}
		// Oikealle kääntyvät
		for ( car_iterator = cars.begin(); car_iterator != cars.end(); )
		{
			int old_vertical_direction = ( *car_iterator )->Get_Old_Vertical_Direction();
			int old_horizontal_direction = ( *car_iterator )->Get_Old_Horizontal_Direction();
			int vertical_direction = ( *car_iterator )->Get_Vertical_Direction();
			int horizontal_direction = ( *car_iterator )->Get_Horizontal_Direction();
			if ( left_allowed && old_vertical_direction == 1 && horizontal_direction == -1 )
			{
				left_allowed = false;
				( *car_iterator )->Move();
				cars.erase( car_iterator );
			}
			else if ( right_allowed && old_vertical_direction == -1 && horizontal_direction == 1 )
			{
				right_allowed = false;
				( *car_iterator )->Move();
				cars.erase( car_iterator );
			}
			else if ( up_allowed && old_horizontal_direction == -1 && vertical_direction == -1 )
			{
				up_allowed = false;
				( *car_iterator )->Move();
				cars.erase( car_iterator );
			}
			else if ( down_allowed && old_horizontal_direction == 1 && vertical_direction == 1 )
			{
				down_allowed = false;
				( *car_iterator )->Move();
				cars.erase( car_iterator );
			}
			else
			{
				++car_iterator;
			}
		}
		// Suoraan menevät
		// Oikealle ei jo ole mennyt autoa ja alhaalta (auton oikealta) ei tule autoa.
		if ( right_allowed && !car_coming_from_bottom )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_horizontal_direction = ( *car_iterator )->Get_Old_Horizontal_Direction();
				int horizontal_direction = ( *car_iterator )->Get_Horizontal_Direction();
				if ( old_horizontal_direction == 1 && horizontal_direction == 1 )
				{
					right_allowed = false;
					( *car_iterator )->Move();
					cars.erase( car_iterator );
					break;
				}
			}
		}
		// Vasemmalle ei jo ole mennyt autoa ja ylhäältä (auton oikealta) ei tule autoa.
		if ( left_allowed && !car_coming_from_top )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_horizontal_direction = ( *car_iterator )->Get_Old_Horizontal_Direction();
				int horizontal_direction = ( *car_iterator )->Get_Horizontal_Direction();
				if ( old_horizontal_direction == -1 && horizontal_direction == -1 )
				{
					left_allowed = false;
					( *car_iterator )->Move();
					cars.erase( car_iterator );
					break;
				}
			}
		}
		// Ylös ei jo ole mennyt autoa ja oikealta (auton oikealta) ei tule autoa.
		if ( up_allowed && !car_coming_from_right )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_vertical_direction = ( *car_iterator )->Get_Old_Vertical_Direction();
				int vertical_direction = ( *car_iterator )->Get_Vertical_Direction();
				if ( old_vertical_direction == -1 &&  vertical_direction == -1 )
				{
					up_allowed = false;
					( *car_iterator )->Move();
					cars.erase( car_iterator );
					break;
				}
			}
		}
		// Alas ei jo ole mennyt autoa ja vasemmalta (auton oikealta) ei tule autoa.
		if ( down_allowed && !car_coming_from_left )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_vertical_direction = ( *car_iterator )->Get_Old_Vertical_Direction();
				int vertical_direction = ( *car_iterator )->Get_Vertical_Direction();
				if ( old_vertical_direction == 1 && vertical_direction == 1 )
				{
					down_allowed = false;
					( *car_iterator )->Move();
					cars.erase( car_iterator );
					break;
				}
			}
		}
		// Vasemmalle kääntyvät
		if ( left_allowed && !car_coming_from_right && !up )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_vertical_direction = ( *car_iterator )->Get_Old_Vertical_Direction();
				int horizontal_direction = ( *car_iterator )->Get_Horizontal_Direction();
				if ( old_vertical_direction == -1 && horizontal_direction == -1 )
				{
					left_allowed = false;
					( *car_iterator )->Move();
					cars.erase( car_iterator );
					break;
				}
			}
		}
		if ( right_allowed && !car_coming_from_left && !down )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_vertical_direction = ( *car_iterator )->Get_Old_Vertical_Direction();
				int horizontal_direction = ( *car_iterator )->Get_Horizontal_Direction();
				if ( old_vertical_direction == 1 && horizontal_direction == 1 )
				{
					right_allowed = false;
					( *car_iterator )->Move();
					cars.erase( car_iterator );
					break;
				}
			}
		}
		if ( up_allowed && !car_coming_from_bottom && !right )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_horizontal_direction = ( *car_iterator )->Get_Old_Horizontal_Direction();
				int vertical_direction = ( *car_iterator )->Get_Vertical_Direction();
				if ( old_horizontal_direction == 1 && vertical_direction == -1 )
				{
					up_allowed = false;
					( *car_iterator )->Move();
					cars.erase( car_iterator );
					break;
				}
			}
		}
		if ( down_allowed && !car_coming_from_top && !left )
		{
			for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
			{
				int old_horizontal_direction = ( *car_iterator )->Get_Old_Horizontal_Direction();
				int vertical_direction = ( *car_iterator )->Get_Vertical_Direction();
				if ( old_horizontal_direction == -1 && vertical_direction == 1 )
				{
					down_allowed = false;
					( *car_iterator )->Move();
					cars.erase( car_iterator );
					break;
				}
			}
		}
		// Jos kukaan ei saa vuoroa
		if ( left_allowed && right_allowed && up_allowed && down_allowed )
		{
			bool finished = false;
			unsigned int random = rand() % 4 + 1;
			while ( !finished )
			{
				switch ( random )
				{
				case 1:
					if ( !car_coming_from_top )
					{
						random++;
						continue;
					}
					else
					{
						for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
						{
							int old_vertical_direction = ( *car_iterator )->Get_Old_Vertical_Direction();
							if ( old_vertical_direction == 1 )
							{
								( *car_iterator )->Move();
								cars.erase( car_iterator );
								break;
							}
						}
						finished = true;
						break;
					}
				case 2:
					if ( !car_coming_from_bottom )
					{
						random++;
						continue;
					}
					else
					{
						for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
						{
							int old_vertical_direction = ( *car_iterator )->Get_Old_Vertical_Direction();
							if ( old_vertical_direction == -1 )
							{
								( *car_iterator )->Move();
								cars.erase( car_iterator );
								break;
							}
						}
						finished = true;
						break;
					}
				case 3:
					if ( !car_coming_from_left )
					{
						random++;
						continue;
					}
					else
					{
						for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
						{
							int old_horizontal_direction = ( *car_iterator )->Get_Old_Horizontal_Direction();
							if ( old_horizontal_direction == 1 )
							{
								( *car_iterator )->Move();
								cars.erase( car_iterator );
								break;
							}
						}
						finished = true;
						break;
					}
				case 4:
					if ( !car_coming_from_right )
					{
						random = 1;
						continue;
					}
					else
					{
						for ( car_iterator = cars.begin(); car_iterator != cars.end(); ++car_iterator )
						{
							int old_horizontal_direction = ( *car_iterator )->Get_Old_Horizontal_Direction();
							if ( old_horizontal_direction == -1 )
							{
								( *car_iterator )->Move();
								cars.erase( car_iterator );
								break;
							}
						}
						finished = true;
						break;
					}
				default:
					std::cout << "Error!\n";
					throw -1;
					break;
				}
			}
		}
	}
}