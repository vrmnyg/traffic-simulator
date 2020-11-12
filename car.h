#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <cstdlib>

//#define PRINT

class Car
{
public:
	Car( unsigned int, unsigned int, unsigned int, unsigned int, int, int );
	~Car();
	unsigned int Get_Vertical_Position();
	unsigned int Get_Horizontal_Position();
	int Get_Vertical_Direction();
	int Get_Horizontal_Direction();
	int Get_Old_Vertical_Direction();
	int Get_Old_Horizontal_Direction();
	unsigned int Get_Turn_Created();
	unsigned int Get_Intersection_Turns();
	unsigned int Get_Intersection_Times();
	void Add_Intersection_Turn();
	void Add_Intersection_Time();
	void Set_Direction_UP();
	void Set_Direction_DOWN();
	void Set_Direction_LEFT();
	void Set_Direction_RIGHT();
	unsigned int Get_Id();
	void Print_Car();
	void Move();
private:
	Car( const Car& );
	Car& operator=( const Car& );
	enum direction { LEFT, RIGHT, UP, DOWN };
	unsigned int vertical_position;
	unsigned int horizontal_position;
	unsigned int id;
	unsigned int turn_created;
	unsigned int intersection_turns;
	unsigned int intersection_times;
	int old_vertical_direction;
	int old_horizontal_direction;
	int vertical_direction;
	int horizontal_direction;
};
#endif