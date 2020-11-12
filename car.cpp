#include "car.h"

Car::Car( const Node* road, unsigned int id, unsigned int tc, int v, int h ): position(road), id(id), turn_created(tc),
intersection_count(0), intersection_turns(0), old_vertical_direction(0), old_horizontal_direction(0), vertical_direction(v), horizontal_direction(h)
{

}

Car::~Car() 
{

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

void Car::Move()
{
	if ( horizontal_direction == -1 )
	{
		position = position->Get_Left_Node();
	}
	else if ( horizontal_direction == 1 )
	{	
		position = position->Get_Right_Node();
	}	
	else if ( vertical_direction == -1 )
	{
		position = position->Get_Upper_Node();
	}	
	else if ( vertical_direction == 1 )
	{
		position = position->Get_Lower_Node();
	}
	else
	{
		std::cout << "Error!\n";
		std::cout << "Car::Move()!\n";
		throw -1;
	}
}