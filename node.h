#ifndef NODE_H
#define NODE_H

#include <iostream>

class Node
{
public:
	Node( unsigned int v,  unsigned int h );
	~Node();
	unsigned int Get_Vertical_Position() const;
	unsigned int Get_Horizontal_Position() const;
	const Node* Get_Left_Node() const;
	const Node* Get_Right_Node() const;
	const Node* Get_Upper_Node() const;
	const Node* Get_Lower_Node() const;
	void Set_Left_Node( const Node* node );
	void Set_Right_Node( const Node* node );
	void Set_Upper_Node( const Node* node );
	void Set_Lower_Node( const Node* node );
private:
	unsigned int vertical_position;
	unsigned int horizontal_position;
	const Node* left_node;
	const Node* right_node;
	const Node* upper_node;
	const Node* lower_node;
};

inline unsigned int Node::Get_Vertical_Position() const
{
	return vertical_position;
}

inline unsigned int Node::Get_Horizontal_Position() const
{
	return horizontal_position;
}

inline const Node* Node::Get_Left_Node() const
{
	return left_node;
}

inline const Node* Node::Get_Right_Node() const
{
	return right_node;
}

inline const Node* Node::Get_Upper_Node() const
{
	return upper_node;
}

inline const Node* Node::Get_Lower_Node() const
{
	return lower_node;
}

inline void Node::Set_Left_Node( const Node* node )
{
	left_node = node;
}

inline void Node::Set_Right_Node( const Node* node )
{
	right_node = node;
}

inline void Node::Set_Upper_Node( const Node* node )
{
	upper_node = node;
}

inline void Node::Set_Lower_Node( const Node* node )
{
	lower_node = node;
}
#endif