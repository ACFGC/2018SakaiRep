#ifndef CMATERIAL
#define CMATERIAL
#define SAFE_DELETE_ARRAY(a){ if(a) delete[] a; a = 0;}
#include "CTexture.h"
class CModel;
class CMaterial {
public:
	char *mpName;
	float mPower;
	float mAmbient[4];
	float mDiffuse[4];
	float mSpecular[4];
	float mEmissive[3];
	float mEmission[4];
	float mShinness;
	int mVertexNo;

	CTexture mTexture;
	char *mpTextureFilename;
	//テクスチャID
	GLuint mTextureId;
	CMaterial();
	CMaterial(CModel *model);

	void SetMaterial();
	void UnSetMaterial();

	void SetTexture(char *file);
	~CMaterial(){
		if (mpName) {
			delete mpName;
		}
		SAFE_DELETE_ARRAY(mpName);
		SAFE_DELETE_ARRAY(mpTextureFilename);
	};
};

#endif
