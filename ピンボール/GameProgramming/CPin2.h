#ifndef CPIN2
#define CPIN2

#include "CCharacter.h"

class CPin2 : public CCharacter {
public:
	static CPin2 *mpPin2;
	CPin2();
	~CPin2();
	void Update();
};

#endif