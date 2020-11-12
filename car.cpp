#include "car.h"

Car::Car( unsigned int v_pos, unsigned int h_pos, unsigned int id, unsigned int tc, int v, int h ): vertical_position(v_pos), horizontal_position(h_pos), id(id), turn_created(tc),
intersection_turns(0), intersection_times(0), old_vertical_direction(0), old_horizontal_direction(0), vertical_direction(v), horizontal_direction(h)
{

}

Car::~Car() 
{
	#ifdef PRINT 
	std::cout << "Auton numero " << id << " destructoria kutsuttiin" << "\n\n";
	#endif
}

unsigned int Car::Get_Vertical_Position()
{
	return vertical_position;
}

unsigned int Car::Get_Horizontal_Position()
{
	return horizontal_position;
}

unsigned int Car::Get_Turn_Created()
{
	return turn_created;
}

unsigned int Car::Get_Intersection_Turns()
{
	return intersection_turns;
}

unsigned int Car::Get_Intersection_Times()
{
	return intersection_times;
}

void Car::Add_Intersection_Turn()
{
	intersection_turns++;
}

void Car::Add_Intersection_Time()
{
	intersection_times++;
}

void Car::Set_Direction_UP()
{
	old_vertical_direction = vertical_direction;
	old_horizontal_direction = horizontal_direction;
	vertical_direction = -1;
	horizontal_direction = 0;
}

void Car::Set_Direction_DOWN()
{
	old_vertical_direction = vertical_direction;
	old_horizontal_direction = horizontal_direction;
	vertical_direction = 1;
	horizontal_direction = 0;
}

void Car::Set_Direction_LEFT()
{
	old_vertical_direction = vertical_direction;
	old_horizontal_direction = horizontal_direction;
	vertical_direction = 0;
	horizontal_direction = -1;
}

void Car::Set_Direction_RIGHT()
{
	old_vertical_direction = vertical_direction;
	old_horizontal_direction = horizontal_direction;
	vertical_direction = 0;
	horizontal_direction = 1;
}

unsigned int Car::Get_Id()
{
	return id;
}

void Car::Print_Car()
{
	std::cout << "Auto numero " << id << " sijainti kartalla: "
	<< vertical_position << "," << horizontal_position << "\n";
}

int Car::Get_Old_Vertical_Direction()
{	
	return old_vertical_direction;
}

int Car::Get_Vertical_Direction()
{	
	return vertical_direction;
}

int Car::Get_Old_Horizontal_Direction()
{	
	return old_horizontal_direction;
}

int Car::Get_Horizontal_Direction()
{	
	return horizontal_direction;
}

void Car::Move()
{
	vertical_position = vertical_position + vertical_direction;
	horizontal_position = horizontal_position + horizontal_direction;
}