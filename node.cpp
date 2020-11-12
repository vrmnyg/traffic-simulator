#include "node.h"

Node::Node( unsigned int v, unsigned int h ): vertical_position(v), horizontal_position(h),
left_node(0), right_node(0), upper_node(0), lower_node(0)
{

}

Node::~Node()
{ 
	
}