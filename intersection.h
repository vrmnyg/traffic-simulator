#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <vector>
#include "car.h"

//#define PRINT

class Intersection
{
public:
	inline Intersection( unsigned int v, unsigned int h ): vertical(v), horizontal(h) {}
	inline ~Intersection() { /**std::cout << "Intersectionin destructoria kutsuttiin\n\n";**/ }
	unsigned int Get_Vertical_Position();
	unsigned int Get_Horizontal_Position();
	unsigned int Get_Queue_Size();
	void Add_Car_To_Queue( Car* );
	bool Is_In_Queue( Car* );
	void Move_Cars();
private:
	Intersection( const Intersection& );
	Intersection& operator=( const Intersection& );
	unsigned int vertical;
	unsigned int horizontal;
	std::vector< Car* > cars;
};
#endif
