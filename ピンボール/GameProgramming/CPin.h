#ifndef CPIN
#define CPIN

#include "CCharacter.h"

class CPin : public CCharacter {
public:
	static CPin *mpPin;
	CPin();
	~CPin();
	void Update();
};

#endif