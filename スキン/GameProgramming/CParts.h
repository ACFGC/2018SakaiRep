#ifndef CPARTS_H
#define CPARTS_H
#include"C3DBox.h"

class CParts : public C3DBox{
public:
	CParts *mpParent;
	CParts()
		: mpParent(0)
	{}
	void Update();
};
#endif