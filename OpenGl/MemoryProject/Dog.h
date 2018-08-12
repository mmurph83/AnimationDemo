//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef DOG_H
#define DOG_H

class Dog
{
public: 
	//constructors
	Dog();
	Dog(int inA, int inB, int inC);
	Dog( const Dog &inDog );

	// assignment operator
	const Dog &operator = ( const Dog &inDog );

	// destructor
	~Dog();

private:
	int a;
	int b;
	int c;
};


#endif

// ---  End of File ---------------
