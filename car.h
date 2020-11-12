#ifndef CAR_H
#define CAR_H

#include <iostream>
#include "node.h"

class Car
{
public:
	Car( const Node*, unsigned int, unsigned int, int, int );
	~Car();
	const Node* Get_Position();
	void Add_Intersection_Time();
	void Add_Intersection_Turn();
	double Get_Average_Intersection_Time() const;
	unsigned int Get_Id() const;
	unsigned int Get_Turn_Created() const;
	int Get_Vertical_Direction() const;
	int Get_Horizontal_Direction() const;
	int Get_Old_Vertical_Direction() const;
	int Get_Old_Horizontal_Direction() const;
	void Set_Direction_UP();
	void Set_Direction_DOWN();
	void Set_Direction_LEFT();
	void Set_Direction_RIGHT();
	void Move();
private:
	const Node* position;
	unsigned int id;
	unsigned int turn_created;
	unsigned int intersection_count;
	unsigned int intersection_turns;
	int old_vertical_direction;
	int old_horizontal_direction;
	int vertical_direction;
	int horizontal_direction;
};

inline const Node* Car::Get_Position()
{
	return position;
}

inline void Car::Add_Intersection_Time()
{
	++intersection_count;
	++intersection_turns;
}
	
inline void Car::Add_Intersection_Turn()
{
	++intersection_turns;
}
	
inline double Car::Get_Average_Intersection_Time() const
{
	return (double)intersection_turns/intersection_count;
}

inline unsigned int Car::Get_Id() const
{
	return id;
}

inline unsigned int Car::Get_Turn_Created() const
{
	return turn_created;
}

inline int Car::Get_Vertical_Direction() const
{	
	return vertical_direction;
}

inline int Car::Get_Horizontal_Direction() const
{	
	return horizontal_direction;
}

inline int Car::Get_Old_Vertical_Direction() const
{	
	return old_vertical_direction;
}

inline int Car::Get_Old_Horizontal_Direction() const
{	
	return old_horizontal_direction;
}
#endif