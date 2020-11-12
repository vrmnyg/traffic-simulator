#ifndef STARTUP_SECTOR_H
#define STARTUP_SECTOR_H

class Startup_Sector
{
public:
	inline Startup_Sector( unsigned int v, unsigned int h ): vertical(v), horizontal(h) {}
	inline ~Startup_Sector() { /**std::cout << "Startup_Sectorin destructoria kutsuttiin\n\n";**/ }
	inline unsigned int Get_Vertical_Position() const { return vertical; }
	inline unsigned int Get_Horizontal_Position() const { return horizontal; }
private:
	Startup_Sector( const Startup_Sector& );
 	Startup_Sector& operator=( const Startup_Sector& );
	unsigned int vertical;
	unsigned int horizontal;
};
#endif
