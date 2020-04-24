#ifndef CBAR
#define CBAR

#include "CCharacter.h"

class CBar : public CCharacter {
public:
	static CBar *mpBar;
	CBar();
	~CBar();
	void Update();
};

#endif