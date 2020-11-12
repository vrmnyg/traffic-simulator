#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <iostream>

class Exception: public std::runtime_error
{
public:
	Exception( unsigned int, unsigned char );
	void Handle();
private:
	unsigned int index;
	unsigned char invalid_char;
};

inline Exception::Exception( unsigned int index, unsigned char invalid ):
std::runtime_error( "Exception" ), index(index), invalid_char(invalid)
{

}

inline void Exception::Handle()
{
	std::cout << "virhe: virheellinen merkki '" << invalid_char << "' karttatiedoston indeksissa " << index << ".\n";
	exit(1);
}
#endif