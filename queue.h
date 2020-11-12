#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include <cstdlib>
#include "car.h"

class Queue
{
public:
	Queue( unsigned int v, unsigned int h );
	~Queue();
	unsigned int Get_Vertical_Position() const;
	unsigned int Get_Horizontal_Position() const;
	unsigned int Get_Queue_Size() const;
	bool Is_Empty() const;
	void Add_Car_To_Queue( Car& );
	bool Is_In_Queue( const Car& );
	void Move_Cars();
private:
	typedef std::vector< Car* > Car_Container;
	typedef std::vector< Car* >::iterator Car_Vector_Iterator;
	Car_Container cars;
	Car_Vector_Iterator car_iterator;
	unsigned int vertical;
	unsigned int horizontal;
};

inline unsigned int Queue::Get_Vertical_Position() const
{
	return vertical;
}

inline unsigned int Queue::Get_Horizontal_Position() const
{
	return horizontal;
}

inline unsigned int Queue::Get_Queue_Size() const
{
	return cars.size();
}

inline bool Queue::Is_Empty() const
{
	return cars.empty();
}

inline void Queue::Add_Car_To_Queue( Car& car )
{
	cars.push_back( &car );
}
#endif