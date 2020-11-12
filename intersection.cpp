#include "intersection.h"

unsigned int Intersection::Get_Vertical_Position()
{
	return vertical;
}

unsigned int Intersection::Get_Horizontal_Position()
{
	return horizontal;
}

unsigned int Intersection::Get_Queue_Size()
{
	return cars.size();
}

void Intersection::Add_Car_To_Queue( Car* car )
{
	cars.push_back( car );
}

bool Intersection::Is_In_Queue( Car* car )
{
	for ( unsigned int k = 0; k < cars.size(); ++k )
	{
		if ( cars[k] == car ) return true;
	}
	return false;
}

void Intersection::Move_Cars()
{
	bool right_allowed = true;
	bool left_allowed = true;
	bool up_allowed = true;
	bool down_allowed = true;
	// tuleeko suunnasta autoa
	bool car_coming_from_top = false;
	bool car_coming_from_bottom = false;
	bool car_coming_from_left = false;
	bool car_coming_from_right = false; 
	// tuleeko suunnasta suoraan tai oikealle menevää autoa
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	// yhden jonotuksen lisäys autoille tilastointia varten
	unsigned int y = 0;
	while ( y < cars.size() )
	{
		cars[y]->Add_Intersection_Turn();
		y++;
	}
	unsigned int k = 0;
	while ( k < cars.size() )
	{
		int old_vertical_direction = cars[k]->Get_Old_Vertical_Direction();
		int old_horizontal_direction = cars[k]->Get_Old_Horizontal_Direction();
		if ( old_vertical_direction == 1 ) car_coming_from_top = true;
		if ( old_vertical_direction == -1 ) car_coming_from_bottom = true;
		if ( old_horizontal_direction == -1 ) car_coming_from_right = true;
		if ( old_horizontal_direction == 1 ) car_coming_from_left = true;
		k++;
	}
	unsigned int l = 0;
	while ( l < cars.size() )
	{
		int old_vertical_direction = cars[l]->Get_Old_Vertical_Direction();
		int old_horizontal_direction = cars[l]->Get_Old_Horizontal_Direction();
		int vertical_direction = cars[l]->Get_Vertical_Direction();
		int horizontal_direction = cars[l]->Get_Horizontal_Direction();
		if ( old_vertical_direction == 1 && horizontal_direction == -1 ) up = true;
		else if ( old_vertical_direction == -1 && horizontal_direction == 1 ) down = true;
		else if ( old_horizontal_direction == 1 && vertical_direction == 1 ) left = true;
		else if ( old_horizontal_direction == -1 && vertical_direction == -1 ) right = true;
		else if ( ( old_vertical_direction == vertical_direction ) && ( old_horizontal_direction == horizontal_direction ) )
		{
			if ( vertical_direction == 1 ) up = true;
			if ( vertical_direction == -1 ) down = true;
			if ( horizontal_direction == -1 ) right = true;
			if ( horizontal_direction == 1 ) left = true;
		}
		l++;
	}
	/*if ( cars.size() > 0 )
	{
		std::cout << "Car from top: " << car_coming_from_top << " Car from bottom: " << car_coming_from_bottom << " Car from left: " << car_coming_from_left << " Car from right: " << car_coming_from_right << "\n";
		std::cout << "From top turning right or forward: " << up << " From down turning right or forward: " << down << "\n"
		<< "From left turning right or forward: " << left << " From right turning right or forward: " << right << "\n\n";
	}*/
	unsigned int i = 0;
	while ( i < cars.size() )
	{
		Car* current_car = cars[i];
		int old_vertical_direction = current_car->Get_Old_Vertical_Direction();
		int old_horizontal_direction = current_car->Get_Old_Horizontal_Direction();
		int vertical_direction = current_car->Get_Vertical_Direction();
		int horizontal_direction = current_car->Get_Horizontal_Direction();
		if ( left_allowed && old_vertical_direction == 1 && horizontal_direction == -1 )
		{
			cars.erase( cars.begin() + i );
			left_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else if ( right_allowed && old_vertical_direction == -1 && horizontal_direction == 1 )
		{
			cars.erase( cars.begin() + i );
			right_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else if ( up_allowed && old_horizontal_direction == -1 && vertical_direction == -1 )
		{
			cars.erase( cars.begin() + i );
			up_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else if ( down_allowed && old_horizontal_direction == 1 && vertical_direction == 1 )
		{
			cars.erase( cars.begin() + i );
			down_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else
		{
			i++;
		}
	}
	unsigned int a = 0;
	while ( a < cars.size() )
	{
		Car* current_car = cars[a];
		int old_vertical_direction = current_car->Get_Old_Vertical_Direction();
		int old_horizontal_direction = current_car->Get_Old_Horizontal_Direction();
		int vertical_direction = current_car->Get_Vertical_Direction();
		int horizontal_direction = current_car->Get_Horizontal_Direction();
		if ( right_allowed && !car_coming_from_bottom && old_horizontal_direction == 1 && horizontal_direction == 1 )
		{
			cars.erase( cars.begin() + a );
			car_coming_from_top = true;
			right_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else if ( left_allowed && !car_coming_from_top && old_horizontal_direction == -1 && horizontal_direction == -1 )
		{
			cars.erase( cars.begin() + a );
			car_coming_from_bottom = true;
			right_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else if ( up_allowed && !car_coming_from_right && old_vertical_direction == -1 &&  vertical_direction == -1 )
		{
			cars.erase( cars.begin() + a );
			car_coming_from_right = true;
			right_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else if ( down_allowed && !car_coming_from_left && old_vertical_direction == 1 && vertical_direction == 1 )
		{
			cars.erase( cars.begin() + a );
			car_coming_from_left = true;
			right_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else
		{
			a++;
		}
	}
	unsigned int f = 0;
	while ( f < cars.size() )
	{
		Car* current_car = cars[f];
		int old_vertical_direction = current_car->Get_Old_Vertical_Direction();
		int old_horizontal_direction = current_car->Get_Old_Horizontal_Direction();
		int vertical_direction = current_car->Get_Vertical_Direction();
		int horizontal_direction = current_car->Get_Horizontal_Direction();
		if ( left_allowed && !car_coming_from_right && !up && old_vertical_direction == -1 && horizontal_direction == -1 )
		{
			cars.erase( cars.begin() + f );
			up = true;
			car_coming_from_right = true;
			left_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else if ( right_allowed && !car_coming_from_left && !down && old_vertical_direction == 1 && horizontal_direction == 1 )
		{
			cars.erase( cars.begin() + f );
			up = true;
			car_coming_from_left = true;
			right_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else if ( up_allowed && !car_coming_from_bottom && !right && old_horizontal_direction == 1 && vertical_direction == -1 )
		{
			cars.erase( cars.begin() + f );
			right = true;
			car_coming_from_bottom = true;
			up_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else if ( down_allowed && !car_coming_from_top && !left && old_horizontal_direction == -1 && vertical_direction == 1 )
		{
			cars.erase( cars.begin() + f );
			left = true;
			car_coming_from_top = true;
			down_allowed = false;
			current_car->Add_Intersection_Time();
			current_car->Move();
			#ifdef PRINT
			current_car->Print_Car();
			#endif
		}
		else
		{
			f++;
		}
	}
	if ( left_allowed && right_allowed && up_allowed && down_allowed && ( cars.size() > 0 ) )
	{
		#ifdef PRINT
		std::cout << "-------------------" << std::endl;
		std::cout << "Jatkava auto arvottiin." << "\n";
		#endif
		bool finished = false;
		while ( !finished )
		{
			unsigned int random = rand() % 4 + 1;
			switch ( random )
			{
			case 1:
				if ( !car_coming_from_top ) continue;
				else
				{
					unsigned int f = 0;
					while ( f < cars.size() )
					{
						Car* current_car = cars[f];
						int old_vertical_direction = current_car->Get_Old_Vertical_Direction();
						if ( old_vertical_direction == 1 )
						{
							current_car->Add_Intersection_Time();
							current_car->Move();
							#ifdef PRINT
							current_car->Print_Car();
							#endif
							cars.erase( cars.begin() + f );
							break;
						}
						f++;
					}
					finished = true;
					break;
				}
			case 2:
				if ( !car_coming_from_bottom ) continue;
				else
				{
					unsigned int f = 0;
					while ( f < cars.size() )
					{
						Car* current_car = cars[f];
						int old_vertical_direction = current_car->Get_Old_Vertical_Direction();
						if ( old_vertical_direction == -1 )
						{
							current_car->Add_Intersection_Time();
							current_car->Move();
							#ifdef PRINT
							current_car->Print_Car();
							#endif
							cars.erase( cars.begin() + f );
							break;
						}
						f++;
					}
					finished = true;
					break;
				}
			case 3:
				if ( !car_coming_from_left ) continue;
				else
				{
					unsigned int f = 0;
					while ( f < cars.size() )
					{
						Car* current_car = cars[f];
						int old_horizontal_direction = current_car->Get_Old_Horizontal_Direction();
						if ( old_horizontal_direction == 1 )
						{
							current_car->Add_Intersection_Time();
							current_car->Move();
							#ifdef PRINT
							current_car->Print_Car();
							#endif
							cars.erase( cars.begin() + f );
							break;
						}
						f++;
					}
					finished = true;
					break;
				}
			case 4:
				if ( !car_coming_from_right ) continue;
				else
				{
					unsigned int f = 0;
					while ( f < cars.size() )
					{
						Car* current_car = cars[f];
						int old_horizontal_direction = current_car->Get_Old_Horizontal_Direction();
						if ( old_horizontal_direction == -1 )
						{
							current_car->Add_Intersection_Time();
							current_car->Move();
							#ifdef PRINT
							current_car->Print_Car();
							#endif
							cars.erase( cars.begin() + f );
							break;
						}
						f++;
					}
					finished = true;
					break;
				}
			}
		}
	}
	#ifdef PRINT
	unsigned int q = 0;
	while ( q < cars.size() )
	{
		std::cout << "-------------------" << std::endl;
		std::cout << "Seuraava auto odottaa jonossa:" << std::endl;
		cars[q]->Print_Car();
		q++;
	}
	#endif
}