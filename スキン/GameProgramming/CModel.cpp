#include "CModel.h"
#include <stdio.h>
#include <assert.h>
#include "CMatrix.h"
#include "glut.h"
#include <string.h>
#include<fcntl.h>
#include<sys\stat.h>
#include<io.h>
#include"windows.h"

/*
CModelXFrame
model:CModelX�C���X�^���X�ւ̃|�C���^
*/
CModelXFrame::CModelXFrame(CModel* model){
	//���݂̃t���[���z��̗v�f�����������ݒ肷��
	mIndex = model->mFrame.size();
	//CModelX�̃t���[���z��ɒǉ�����
	model->mFrame.push_back(this);
	//�ϊ��s���P�ʍs��ɂ���
	mTransformMatrix.Identity();
	//���̒P��i�t���[�����̗\��j����������
	model->GetToken();
	//�t���[�������G���A���m�ۂ���
	mpName = new char[strlen(model->mToken) + 1];
	//�t���[�������R�s�[����
	strcpy(mpName, model->mToken);
	//���̒P��({�̗\��j����������
	model->GetToken(); //{
	//�����������Ȃ�����I���
	while (*model->mpPointer != '\0'){
		//���̒P�ʏ���
		model->GetToken(); // Frame
		//}�������̏ꍇ�͏I��
		if (strchr(model->mToken, '}'))break;
		//�V���ȃt���[�����쐬���A�q�t���[���ɒǉ�
		if (strcmp(model->mToken, "Frame") == 0){
			//�t���[�����쐬���A�q�t���[���̔z��ɒǉ�
			mChild.push_back(
				new CModelXFrame(model));
		}
		else if (strcmp(model->mToken, "FrameTransformMatrix") == 0){
			model->GetToken(); // {
			for (int i = 0; i < ARRAY_SIZE(mTransformMatrix.f); i++){
				mTransformMatrix.f[i] = model->GetFloatToken();
			}
			model->GetToken(); // }
		}
		else if (strcmp(model->mToken, "Mesh") == 0){
			mMesh.Init(model);
		}
		else{
			//��L�ȊO�̗v�f�͓ǂݔ�΂�
			model->SkipNode();
		}
	}
	//�f�o�b�O�o�[�W�����̂ݗL��
#ifdef _DEBUG
	;//printf("%s\n", mpName);
	for (int i = 0; i < 16; i++){
		;//printf("%f ", mTransformMatrix.f[i]);
		if (i % 4 == 3)
			;//printf("\n");
	}


#endif
}
void CModel::GetToken(){
	char* p = mpPointer;
	char* q = mToken;

	while (*p != '\0' && strchr(" \t\r\n, ;\"", *p))p++;

	if (*p == '{' || *p == '}'){

		*q++ = *p++;
	}
	else{
		while (*p != '\0' && !strchr(" \t\r\n,;\"}", *p))
			*q++ = *p++;
	}
	*q = '\0';
	mpPointer = p;

	if (!strcmp("//", mToken)){
		while (*p != '\0' && !strchr("\r\n", *p))p++;

		mpPointer = p;

		GetToken();
	}
}

/*
SkipNode
�m�[�h��ǂݔ�΂�
*/
void CModel::SkipNode(){
	//�������I�������I��
	while (*mpPointer != '\0'){
		GetToken();
		//{�����������烋�[�v�I��
		if (strchr(mToken, '{'))break;
	}
	int count = 1;
	//�������I��邩�A�J�E���g���O�ɂȂ�����I��
	while (*mpPointer != '\0' && count > 0){
		GetToken();
		//{��������ƃJ�E���g�A�b�v
		if (strchr(mToken, '{'))count++;
		//}��������ƃJ�E���g�_�E��
		else if (strchr(mToken, '}'))count--;
	}
}
/*
GetFloatToken
�P��𕂓������_�̃f�[�^�ŕԂ�
*/
float CModel::GetFloatToken(){
	GetToken();
	//atof
	//�������float�^�֕ϊ�
	return atof(mToken);
}
int CModel::GetIntToken(){
	GetToken();
	return atoi(mToken);
}
void CMesh::Init(CModel *model){

	model->GetToken();
	if (!strchr(model->mToken, '{')){
		model->GetToken();
	}
	//���_���̏���
	mVertexNum = model->GetIntToken();
	//���_�����G���A�m��
	mpVertex = new CVector3[mVertexNum];
	mpAnimateVertex = new CVector3[mVertexNum];
	//���_�����f�[�^����荞��
	for (int i = 0; i < mVertexNum; i++){
		mpVertex[i].x = model->GetFloatToken();
		mpVertex[i].y = model->GetFloatToken();
		mpVertex[i].z = model->GetFloatToken();

	}
	mFaceNum = model->GetIntToken();
	//���_����1�ʂɂR���_
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3){
		model->GetToken(); //���_���ǂݔ�΂�
		mpVertexIndex[i] = model->GetIntToken();
		mpVertexIndex[i + 1] = model->GetIntToken();
		mpVertexIndex[i + 2] = model->GetIntToken();
	}
	while (model->mpPointer != '\0'){
		model->GetToken(); //MeshNormals
		//}�������̏ꍇ�͏I��
		if (strchr(model->mToken, '}'))
			break;
		if (strcmp(model->mToken, "MeshNormals") == 0){
			model->GetToken(); //{
			//�@���f�[�^��������
			mNormalNum = model->GetIntToken();
			//�@���̃f�[�^��z��Ɏ�荞��
			CVector3 *pNormal = new CVector3[mNormalNum];
			for (int i = 0; i < mNormalNum; i++){
				pNormal[i].x = model->GetFloatToken();
				pNormal[i].y = model->GetFloatToken();
				pNormal[i].z = model->GetFloatToken();
			}
			//�@�������ʐρ~�R
			mNormalNum = model->GetIntToken() * 3; //FaceNum
			int ni;
			//���_���ɖ@���f�[�^��ݒ肷��
			mpNormal = new CVector3[mNormalNum];
			mpAnimateNormal = new CVector3[mNormalNum];
			for (int i = 0; i < mNormalNum; i += 3){
				model->GetToken(); //3
				ni = model->GetIntToken();
				mpNormal[i] = pNormal[ni];

				ni = model->GetIntToken();
				mpNormal[i + 1] = pNormal[ni];

				ni = model->GetIntToken();
				mpNormal[i + 2] = pNormal[ni];
			}
			delete[]pNormal;
			model->GetToken(); //}
		}
		// MeshMaterialList�̂Ƃ�
		else if (strcmp(model->mToken, "MeshMaterialList") == 0){
			model->GetToken();//{
			//Material�̐�
			mMaterialNum = model->GetIntToken();
			//FaceNum
			mMaterialIndexNum = model->GetIntToken();
			//�}�e���A���C���f�b�N�X�̍쐬
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i < mMaterialIndexNum; i++){
				mpMaterialIndex[i] = model->GetIntToken();
			}
			//�}�e���A���f�[�^�̍쐬
			for (int i = 0; i < mMaterialNum; i++){
				model->GetToken();  //Material
				if (strcmp(model->mToken, "Material") == 0){
					mMaterial.push_back(new CMaterial(model));
				}
				else{
					// { ���o
					model->GetToken(); //MaterialName
					mMaterial.push_back(
						model->FindMaterial(model->mToken));
					model->GetToken(); // }
				}
			}
			model->GetToken(); //}//End of MeshMaterialList
		}
		else if (strcmp(model->mToken, "SkinWeights") == 0){
			//CSkinWeights�N���X�̃C���X�^���X���쐬���A�z��ɒǉ�
			mSkinWeights.push_back(new CSkinWeights(model));
		}
		//�e�N�X�`�����W�̎�
		else if (strcmp(model->mToken, "MeshTextureCoords") == 0){
			model->GetToken(); //{
			//�e�N�X�`�����W��������
			int textureCoordsNum = model->GetIntToken();
			//�e�N�X�`�����W�̃f�[�^��z��Ɏ�荞��
			mpTextureCoords = new CVector2[textureCoordsNum];
			for (int i = 0; i < textureCoordsNum; i++){
				mpTextureCoords[i].x = model->GetFloatToken();
				mpTextureCoords[i].y = model->GetFloatToken();
			}
			model->GetToken(); //}
		}
		else{
			//�ȊO�̃m�[�h�͓ǂݔ�΂�
			model->SkipNode();
		}
#ifdef _DEBUG
		;//printf("\n%d\n", mNormalNum);
		for (int i = 0; i < mNormalNum; i++){
			;//printf(" %f", mpNormal[i].x);
			;//printf(" %f", mpNormal[i].y);
			;//printf(" %f", mpNormal[i].z);

			;//printf("\n");
		}
		;//printf("\n%d\n", mFaceNum);
		for (int i = 0; i < mFaceNum * 3; i += 3){
			;//printf(" %d", mpVertexIndex[i]);
			;//printf(" %d", mpVertexIndex[i + 1]);
			;//printf(" %d", mpVertexIndex[i + 2]);

			;//printf("\n");
		}
		;//printf("\n%d\n", mVertexNum);
		for (int i = 0; i < mVertexNum; i++){
			;//printf(" %f", mpVertex[i].x);
			;//printf(" %f", mpVertex[i].y);
			;//printf(" %f", mpVertex[i].z);
			//			if (i % 3 == 3)
			;//printf("\n");
		}
		;//printf("\n%d\n", mMaterialNum);
		for (int i = 0; i < mMaterialNum; i++){
			;//printf(" %f", mMaterial[i]->mDiffuse[0]);
			;//printf(" %f", mMaterial[i]->mDiffuse[1]);
			;//printf(" %f", mMaterial[i]->mDiffuse[2]);
			;//printf(" %f", mMaterial[i]->mDiffuse[3]);
			;//printf("\n");
			;//printf(" %f", mMaterial[i]->mPower);
			;//printf("\n");
			;//printf(" %f", mMaterial[i]->mSpecular[0]);
			;//printf(" %f", mMaterial[i]->mSpecular[1]);
			;//printf(" %f", mMaterial[i]->mSpecular[2]);
			;//printf(" %f", mMaterial[i]->mSpecular[3]);
			;//printf("\n");
			;//printf(" %f", mMaterial[i]->mEmissive[0]);
			;//printf(" %f", mMaterial[i]->mEmissive[1]);
			;//printf(" %f", mMaterial[i]->mEmissive[2]);
			;//printf(" %f", mMaterial[i]->mEmissive[3]);

			//			if (i % 3 == 3)
			;//printf("\n");
		}
#endif
	}
}
/*
Render
��ʂɕ`�悷��
*/
void CMesh::Render(){
	/* ���_�f�[�^�A�@���f�[�^�̔z���L���ɂ���*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	/* ���_�f�[�^�A�@���f�[�^�̏ꏊ���w�肷��*/
	glVertexPointer(3, GL_FLOAT, 0, mpAnimateVertex);
	glNormalPointer(GL_FLOAT, 0, mpAnimateNormal);
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoords);

	/* ���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��*/
	//glDrawElements(GL_TRIANGLES, 3 * mFaceNum, GL_UNSIGNED_INT, mpVertexIndex);
	/* ���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��*/
	for (int i = 0; i < mFaceNum; i++){
		//�}�e���A����K�p����
		mMaterial[mpMaterialIndex[i]]->SetMaterial();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (mpVertexIndex + i * 3));
		//�}�e���A������������
		mMaterial[mpMaterialIndex[i]]->UnSetMaterial();
	}
}
CModel::CModel()
: mpVertex(0)
, mpNormal(0)
, mpTextureCoord(0)
, mpPointer(0)
{}

CModel::~CModel() {
	delete mFrame[0];
	for (int i = 0; i < mAnimationSet.size(); i++){
		delete mAnimationSet[i];
	}
	for (int i = 0; i < mMaterial.size(); i++){
		delete mMaterial[i];
	}
	//�e�N�X�`���̉��
	for (int i = 0; i < mTexture.size(); i++){
		delete mTexture[i];
	}
	for (int i = 0; i < mTriangles.size(); i++) {
		delete mTriangles[i];
	}
	for (int i = 0; i < mMaterials.size(); i++) {
		delete mMaterials[i];
	}
	if (mpVertex) {
		delete[] mpVertex;
	}
	if (mpNormal) {
		delete[] mpNormal;
	}
	if (mpTextureCoord) {
		delete[] mpTextureCoord;
	}
}
void CModel::Load(char *file){


	int fd = open(file, O_RDONLY);
	if (fd == -1){
		;//printf("open error\n");
		return;
	}
	struct stat statBuf;
	fstat(fd, &statBuf);
	close(fd);
	int size = statBuf.st_size;

	FILE *fp;
	fp = fopen(file, "rb");
	if (fp == NULL){
		;//printf("fopen error\n");
		return;
	}
	char *buf = mpPointer = new char[size + 1];
	fread(buf, size, 1, fp);
	buf[size] = '\0';
	fclose(fp);
	//	;//printf("%s" , buf);
	//������̍Ō�܂ŌJ��Ԃ�
	while (*mpPointer != '\0'){
		GetToken();//�P��̏���
		//template �ǂݔ�΂�
		if (strcmp(mToken, "template") == 0){
			SkipNode();
		}
		//Material �̎�
		else if (strcmp(mToken, "Material") == 0){
			new CMaterial(this);
		}
		//�P�ꂪFrame�̏ꍇ
		else if (strcmp(mToken, "Frame") == 0){
			//�t���[�����쐬����
			new CModelXFrame(this);

			//;//printf("%s", mToken);
			//GetToken();
			//;//printf("%s\n", mToken);
		}
		//if (strcmp(mToken, "FrameTransformMatrix") == 0){
		//	;//printf("&f", mTransformMatrix.f[0]);
		//	GetToken();
		//	;//printf("%f\n", mTransformMatrix.f[0]);
		//}
		//�P�ꂪAnimationSet�̏ꍇ
		else if (strcmp(mToken, "AnimationSet") == 0){
			new CAnimationSet(this);
		}
	}
	SAFE_DELETE_ARRAY(buf);
	//�X�L���E�F�C�g�̃t���[���ԍ��ݒ�
	SetSkinWeightFrameIndex();
}
void CModel::Load(char *obj, char *mtl) {
	FILE *fp;
	char buffer[100];
	char buf[4][50];

	fp = fopen(mtl, "r");
	if (fp == NULL) {
		printf("file not found:%s\n", mtl);
		return;
	}

	CMaterial *material;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		sscanf(buffer, "%s %s %s %s\n", buf[0], buf[1], buf[2], buf[3]);
		if (strcmp(buf[0], "newmtl") == 0) {
			material = new CMaterial();
			material->mpName = new char[strlen(buf[1]) + 1];
			strcpy(material->mpName, buf[1]);
			mMaterials.push_back(material);
		}
		else if (strcmp(buf[0], "Ns") == 0) {
			material->mShinness = atof(buf[1]);
		}
		else if (strcmp(buf[0], "Ka") == 0) {
			material->mAmbient[0] = atof(buf[1]);
			material->mAmbient[1] = atof(buf[2]);
			material->mAmbient[2] = atof(buf[3]);
		}
		else if (strcmp(buf[0], "Kd") == 0) {
			material->mDiffuse[0] = atof(buf[1]);
			material->mDiffuse[1] = atof(buf[2]);
			material->mDiffuse[2] = atof(buf[3]);
		}
		else if (strcmp(buf[0], "Ks") == 0) {
			material->mSpecular[0] = atof(buf[1]);
			material->mSpecular[1] = atof(buf[2]);
			material->mSpecular[2] = atof(buf[3]);
		}
		else if (strcmp(buf[0], "Ke") == 0) {
			material->mEmission[0] = atof(buf[1]);
			material->mEmission[1] = atof(buf[2]);
			material->mEmission[2] = atof(buf[3]);
		}
		else if (strcmp(buf[0], "d") == 0) {
			material->mAmbient[3] = atof(buf[1]);
			material->mDiffuse[3] = atof(buf[1]);
			material->mSpecular[3] = atof(buf[1]);
			material->mEmission[3] = atof(buf[1]);
		}
		else if (strcmp(buf[0], "map_Kd") == 0) {
			material->SetTexture(buf[1]);
		}
	}
	//�_�~�[�̃}�e���A���̒ǉ�
	material = new CMaterial();
	mMaterials.push_back(material);

	fclose(fp);

	fp = fopen(obj, "r");
	if (fp == NULL) {
		printf("file not found:%s\n", obj);
		return;
	}
	std::vector<CVector*> vertexes;
	std::vector<CVector*> normals;
	std::vector<CVector*> texcoords;

	int materiaId = 0;
	int vt = 0;
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		sscanf(buffer, "%s %s %s %s\n", buf[0], buf[1], buf[2], buf[3]);
		if (strcmp(buf[0], "v") == 0) {
			vertexes.push_back(new CVector(atof(buf[1]), atof(buf[2]), atof(buf[3])));
		}
		else if (strcmp(buf[0], "vn") == 0) {
			normals.push_back(new CVector(atof(buf[1]), atof(buf[2]), atof(buf[3])));
		}
		else if (strcmp(buf[0], "vt") == 0) {
			texcoords.push_back(new CVector(atof(buf[1]), atof(buf[2]), 0.0f));
		}
		else if (strcmp(buf[0], "f") == 0) {
			int v[3], n[3], t[3];
			//?
			//if (vt) {
			if (strstr(buf[1], "//") == 0) {
				sscanf(buf[1], "%d/%d/%d", &v[0], &t[0], &n[0]);
				sscanf(buf[2], "%d/%d/%d", &v[1], &t[1], &n[1]);
				sscanf(buf[3], "%d/%d/%d", &v[2], &t[2], &n[2]);
				CTriangle *triangle = new CTriangle(*vertexes[v[0] - 1], *vertexes[v[1] - 1], *vertexes[v[2] - 1]);
				triangle->mNormal[0].mX = normals[n[0] - 1]->mX;
				triangle->mNormal[0].mY = normals[n[0] - 1]->mY;
				triangle->mNormal[0].mZ = normals[n[0] - 1]->mZ;
				triangle->mNormal[1].mX = normals[n[1] - 1]->mX;
				triangle->mNormal[1].mY = normals[n[1] - 1]->mY;
				triangle->mNormal[1].mZ = normals[n[1] - 1]->mZ;
				triangle->mNormal[2].mX = normals[n[2] - 1]->mX;
				triangle->mNormal[2].mY = normals[n[2] - 1]->mY;
				triangle->mNormal[2].mZ = normals[n[2] - 1]->mZ;
				triangle->mMaterialId = materiaId;
				triangle->mUv[0].mX = texcoords[t[0] - 1]->mX;
				triangle->mUv[0].mY = texcoords[t[0] - 1]->mY;
				triangle->mUv[0].mZ = texcoords[t[0] - 1]->mZ;
				triangle->mUv[1].mX = texcoords[t[1] - 1]->mX;
				triangle->mUv[1].mY = texcoords[t[1] - 1]->mY;
				triangle->mUv[1].mZ = texcoords[t[1] - 1]->mZ;
				triangle->mUv[2].mX = texcoords[t[2] - 1]->mX;
				triangle->mUv[2].mY = texcoords[t[2] - 1]->mY;
				triangle->mUv[2].mZ = texcoords[t[2] - 1]->mZ;
				mTriangles.push_back(triangle);
			}
			else {
				sscanf(buf[1], "%d//%d", &v[0], &n[0]);
				sscanf(buf[2], "%d//%d", &v[1], &n[1]);
				sscanf(buf[3], "%d//%d", &v[2], &n[2]);
				CTriangle *triangle = new CTriangle(*vertexes[v[0] - 1], *vertexes[v[1] - 1], *vertexes[v[2] - 1]);
				triangle->mNormal[0].mX = normals[n[0] - 1]->mX;
				triangle->mNormal[0].mY = normals[n[0] - 1]->mY;
				triangle->mNormal[0].mZ = normals[n[0] - 1]->mZ;
				triangle->mNormal[1].mX = normals[n[1] - 1]->mX;
				triangle->mNormal[1].mY = normals[n[1] - 1]->mY;
				triangle->mNormal[1].mZ = normals[n[1] - 1]->mZ;
				triangle->mNormal[2].mX = normals[n[2] - 1]->mX;
				triangle->mNormal[2].mY = normals[n[2] - 1]->mY;
				triangle->mNormal[2].mZ = normals[n[2] - 1]->mZ;
				triangle->mMaterialId = materiaId;
				mTriangles.push_back(triangle);
			}
		}
		else if (strcmp(buf[0], "usemtl") == 0) {
			vt = 0;
			//�_�~�[�}�e���A���̑O�܂Ō���
			for (materiaId = 0; materiaId < mMaterials.size() - 1; materiaId++) {
				if (strcmp(mMaterials[materiaId]->mpName, buf[1]) == 0) {
					vt = mMaterials[materiaId]->mTexture.mId;
					break;
				}
			}
			assert(materiaId < mMaterials.size());
		}		
	}

	for (int i = 0; i < vertexes.size(); i++) {
		delete vertexes[i];
	}

	for (int i = 0; i < normals.size(); i++) {
		delete normals[i];
	}

	for (int i = 0; i < texcoords.size(); i++) {
		delete texcoords[i];
	}

	mpVertex = new float[mTriangles.size() * 9];
	mpNormal = new float[mTriangles.size() * 9];
	mpTextureCoord = new float[mTriangles.size() * 6];

	int v = 0, t = 0;
	for (int m = 0; m < mMaterials.size(); m++) {
		for (int i = 0; i < mTriangles.size(); i++) {
			if (mTriangles[i]->mMaterialId == m) {
				mpVertex[v++] = mTriangles[i]->mV[0].mX;
				mpVertex[v++] = mTriangles[i]->mV[0].mY;
				mpVertex[v++] = mTriangles[i]->mV[0].mZ;
				mpVertex[v++] = mTriangles[i]->mV[1].mX;
				mpVertex[v++] = mTriangles[i]->mV[1].mY;
				mpVertex[v++] = mTriangles[i]->mV[1].mZ;
				mpVertex[v++] = mTriangles[i]->mV[2].mX;
				mpVertex[v++] = mTriangles[i]->mV[2].mY;
				mpVertex[v++] = mTriangles[i]->mV[2].mZ;
				v -= 9;
				mpNormal[v++] = mTriangles[i]->mNormal[0].mX;
				mpNormal[v++] = mTriangles[i]->mNormal[0].mY;
				mpNormal[v++] = mTriangles[i]->mNormal[0].mZ;
				mpNormal[v++] = mTriangles[i]->mNormal[1].mX;
				mpNormal[v++] = mTriangles[i]->mNormal[1].mY;
				mpNormal[v++] = mTriangles[i]->mNormal[1].mZ;
				mpNormal[v++] = mTriangles[i]->mNormal[2].mX;
				mpNormal[v++] = mTriangles[i]->mNormal[2].mY;
				mpNormal[v++] = mTriangles[i]->mNormal[2].mZ;

				mpTextureCoord[t++] = mTriangles[i]->mUv[0].mX;
				mpTextureCoord[t++] = mTriangles[i]->mUv[0].mY;
				mpTextureCoord[t++] = mTriangles[i]->mUv[1].mX;
				mpTextureCoord[t++] = mTriangles[i]->mUv[1].mY;
				mpTextureCoord[t++] = mTriangles[i]->mUv[2].mX;
				mpTextureCoord[t++] = mTriangles[i]->mUv[2].mY;
			}
		}
		mMaterials[m]->mVertexNo = v / 3;
	}

	fclose(fp);

}
/*
Render
���b�V���̖ʐ���0�ȊO�Ȃ�`�悷��
*/
void CModelXFrame::Render(){
	if (mMesh.mFaceNum != 0)
		mMesh.Render();
}
/*
Render
�S�Ẵt���[���̕`�揈�����Ăяo��
*/
void CModel::Render(){
	for (int i = 0; i < mFrame.size(); i++){
		mFrame[i]->Render();
	}
}
void CModel::Render(const CMatrix &matrix) {
	/* ���_�f�[�^�C�@���f�[�^�̔z���L���ɂ��� */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//�e�N�X�`���}�b�s���O�̃f�[�^��L���ɂ���
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	/* ���_�f�[�^�C�@���f�[�^�̔z����w�肷�� */
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoord);

	glPushMatrix();
	glMultMatrixf(&matrix.mM[0][0]);

	int first = 0, count;
	for (int i = 0; i < mMaterials.size(); i++) {
		count = mMaterials[i]->mVertexNo - first;
		/* ���_�z��̐}�`��`�悷�� */
		mMaterials[i]->SetMaterial();
		glDrawArrays(GL_TRIANGLES, first, count);
		mMaterials[i]->UnSetMaterial();
		first += count;
	}

	glPopMatrix();

	/* ���_�f�[�^�C�@���f�[�^�̔z��𖳌��ɂ��� */
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//�e�N�X�`���}�b�s���O�̃f�[�^�𖳌��ɂ���
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}
/*
CSkinWeights
�X�L���E�F�C�g�̓ǂݍ���
*/
CSkinWeights::CSkinWeights(CModel *model)
: mpFrameName(0)
, mFrameIndex(0)
, mIndexNum(0)
, mpIndex(0)
, mpWeight(0)
{
	model->GetToken(); //{
	model->GetToken(); // FrameName
	//�t���[�����G���A�m�ہA�ݒ�
	mpFrameName = new char[strlen(model->mToken) + 1];
	strcpy(mpFrameName, model->mToken);
	//���_�ԍ�������
	mIndexNum = model->GetIntToken();
	//���_�ԍ�����0�𒴂���
	if (mIndexNum > 0){
		//���_�ԍ��ƒ��_�E�F�C�g�̃G���A�m��
		mpIndex = new int[mIndexNum];
		mpWeight = new float[mIndexNum];
		//���_�ԍ�����
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = model->GetIntToken();
		//���_�E�F�C�g����
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = model->GetFloatToken();
	}
	//�I�t�Z�b�g�s�񏊓�
	for (int i = 0; i < 16; i++){
		mOffset.f[i] = model->GetFloatToken();
	}
	model->GetToken(); //}
#ifdef _DEBUG
	;//printf("%s\n", mpFrameName);
	for (int i = 0; i < mIndexNum; i++){
		;//printf(" %d", mpIndex[i]);
		;//printf(" ");
		;//printf(" %f", mpWeight[i]);
		;//printf("\n");
	}
	for (int i = 0; i < 16; i++){
		;//printf(" %f", mOffset.f[i]);
		if (i % 4 == 3)
			;//printf("\n");

		//			if (i % 3 == 3)

	}


#endif
}
/*
CAnimationSet
*/
CAnimationSet::CAnimationSet(CModel *model)
: mpName(0)
, mTime(0)
, mWeight(0)
, mMaxTime(0)

{
	model->mAnimationSet.push_back(this);
	model->GetToken(); //Animation Name
	//�A�j���[�V�����̖��O��ޔ�
	mpName = new char[strlen(model->mToken) + 1];
	strcpy(mpName, model->mToken);
	model->GetToken(); //{
	while (*model->mpPointer != '\0'){
		model->GetToken(); // } or Animation
		if (strchr(model->mToken, '}'))break;
		if (strcmp(model->mToken, "Animation") == 0){
			////�Ƃ肠�����ǂݔ�΂�
			//model->SkipNode();
			//Animation�v�f�ǂݍ���
			mAnimation.push_back(new CAnimation(model));
		}
	}
	//�I�����Ԑݒ�
	mMaxTime = mAnimation[0]->mpKey[mAnimation[0]->mKeyNum - 1].mTime;
#ifdef _DEBUG
	;//printf("%s\n" , mpName);
#endif
}
CAnimation::CAnimation(CModel *model)
: mpFrameName(0)
, mFrameIndex(0)
, mKeyNum(0)
, mpKey(0)
{
	model->GetToken(); // { or Animation Name
	if (strchr(model->mToken, '{')){
		model->GetToken();//{
	}
	else{
		model->GetToken();//{
		model->GetToken();//{
	}

	model->GetToken(); //FrameName
	mpFrameName = new char[strlen(model->mToken) + 1];
	strcpy(mpFrameName, model->mToken);
	mFrameIndex =
		model->FindFrame(model->mToken)->mIndex;
	model->GetToken(); //}

	//�L�[�̔z���ۑ����Ă����z��
	CMatrix44 *key[4] = { 0, 0, 0, 0 };
	//���Ԃ̔z���ۑ����Ă����z��
	float *time[4] = { 0, 0, 0, 0 };
	while (*model->mpPointer != '\0'){
		model->GetToken(); // } or AnimationKey
		if (strchr(model->mToken, '}'))break;
		if (strcmp(model->mToken, "AnimationKey") == 0){
			model->GetToken();//{
			//�f�[�^�̃^�C�v����
			int type = model->GetIntToken();
			//���Ԑ�����
			mKeyNum = model->GetIntToken();
			switch (type){
			case 0: // Rotation Quaternion
				//�s��̔z������Ԑ����m��
				key[type] = new CMatrix44[mKeyNum];
				//���Ԃ̔z������Ԑ����m��
				time[type] = new float[mKeyNum];
				//���Ԑ����J��Ԃ�
				for (int i = 0; i < mKeyNum; i++){
					//���ԏ���
					time[type][i] = model->GetFloatToken();
					model->GetToken(); // 4��ǂݔ�΂�
					//w,x,y,z������
					float w = model->GetFloatToken();
					float x = model->GetFloatToken();
					float y = model->GetFloatToken();
					float z = model->GetFloatToken();
					//�N�H�[�^�j�I�������]�s��ɕϊ�
					key[type][i].SetQuaternion(x, y, z, w);
				}
				break;
			case 1: //�g��E�k���̍s��쐬
				key[type] = new CMatrix44[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++){
					time[type][i] = model->GetFloatToken();
					model->GetToken(); // 3
					float x = model->GetFloatToken();
					float y = model->GetFloatToken();
					float z = model->GetFloatToken();
					key[type][i].m00 = x;
					key[type][i].m11 = y;
					key[type][i].m22 = z;
				}
				break;
			case 2: //�ړ��̍s��쐬
				key[type] = new CMatrix44[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++){
					time[type][i] = model->GetFloatToken();
					model->GetToken(); // 3
					float x = model->GetFloatToken();
					float y = model->GetFloatToken();
					float z = model->GetFloatToken();
					CVector3 vec(x, y, z);
					key[type][i].translate(vec);
				}
				break;
			case 4: //�s��f�[�^������
				mpKey = new CAnimationKey[mKeyNum];
				for (int i = 0; i < mKeyNum; i++){
					mpKey[i].mTime = model->GetFloatToken(); // Time
					model->GetToken(); // 16
					for (int j = 0; j < 16; j++){
						mpKey[i].mMatrix.f[j] = model->GetFloatToken();
					}
				}
				break;
			}
			model->GetToken(); // }
		}
		else{
			model->SkipNode();
		}
	}
	//�s��f�[�^�ł͂Ȃ��Ƃ�
	if (mpKey == 0){
		//���Ԑ����L�[���쐬
		mpKey = new CAnimationKey[mKeyNum];
		for (int i = 0; i < mKeyNum; i++){
			//���Ԑݒ�
			mpKey[i].mTime = time[2][i]; // Time
			//�s��쐬 Position * Rotation * Size
			mpKey[i].mMatrix = key[2][i] * key[0][i] * key[1][i];
		}
	}
	//�m�ۂ����G���A�J��
	for (int i = 0; i < ARRAY_SIZE(key); i++){
		SAFE_DELETE_ARRAY(time[i]);
		SAFE_DELETE_ARRAY(key[i]);
	}
#ifdef _DEBUG
	;//printf("%s\n", mpFrameName);
	for (int i = 0; i < 16; i++){
		;//printf(" %f", mpKey[0].mMatrix.f[i]);
		//;//printf(" ");
		//;//printf(" %f", mpKey[i].mMatrix);
		if (i % 4 == 3)
			;//printf("\n");
	}
#endif
}
/*
AnimateFrame
�t���[���̕ϊ��s����A�j���[�V�����f�[�^�ōX�V����
*/
void CModel::AnimateFrame(){
	//�A�j���[�V�����œK�p�����t���[����
	//�ϊ��s����[���N���A����
	for (int i = 0; i < mAnimationSet.size(); i++){
		CAnimationSet* anim = mAnimationSet[i];
		//�d�݂��O�͔�΂�
		if (anim->mWeight == 0)continue;
		//�t���[����(Animation��)�J��Ԃ�
		for (int j = 0; j < anim->mAnimation.size(); j++){
			CAnimation* animation = anim->mAnimation[j];
			//�Y������t���[���̕ϊ��s����[���N���A����
			memset(&mFrame[animation->mFrameIndex]->mTransformMatrix, 0, sizeof(CMatrix44));
		}
	}
	//�A�j���[�V�����ɊY������t���[���̕ϊ��s���
	//�A�j���[�V�����̃f�[�^�Őݒ肷��
	for (int i = 0; i < mAnimationSet.size(); i++){
		CAnimationSet* anim = mAnimationSet[i];
		//�d�݂��O�͔�΂�
		if (anim->mWeight == 0)continue;
		//�t���[����(Animation��)�J��Ԃ�
		for (int j = 0; j < anim->mAnimation.size(); j++){
			//�t���[������������
			CAnimation* animation = anim->mAnimation[j];
			CModelXFrame* frame = mFrame[animation->mFrameIndex];
			//�L�[���Ȃ��ꍇ�͔�΂�
			if (animation->mpKey == 0)continue;
			//���Ԃ�����
			float time = anim->mTime;
			//�ŏ��̎��Ԃ�菬�����ꍇ
			if (time < animation->mpKey[0].mTime){
				//�ϊ��s����O�R�}�ڂ̍s��ōX�V
				frame->mTransformMatrix += animation->mpKey[0].mMatrix * anim->mWeight;
			}
			//�Ō�̎��Ԃ��傫���ꍇ
			else if (time >= animation->mpKey[animation->mKeyNum - 1].mTime){
				//�ϊ��s����Ō�̃R�}�̍s��ōX�V
				frame->mTransformMatrix += animation->mpKey[animation->mKeyNum - 1].mMatrix * anim->mWeight;
			}
			else{
				//���Ԃ̓r���̏ꍇ
				for (int k = 1; k < animation->mKeyNum; k++){
					//�ϊ��s����A���`��ԂɂčX�V
					if (time < animation->mpKey[k].mTime &&
						animation->mpKey[k - 1].mTime != animation->mpKey[k].mTime){
						float r = (animation->mpKey[k].mTime - time) /
							(animation->mpKey[k].mTime - animation->mpKey[k - 1].mTime);
						frame->mTransformMatrix +=
							(animation->mpKey[k - 1].mMatrix * r + animation->mpKey[k].mMatrix * (1 - r)) * anim->mWeight;
						break;
					}
				}
			}
		}
	}
#ifdef _DEBUG
	//	;//printf("%s\n", mFrame[6]);
	for (int i = 0; i < mFrame.size(); i++){
		//;//printf(" %f", mFrame[6]->mTransformMatrix.f[16]);
		;//printf("Frame : %s\n", mFrame[i]->mpName);
		for (int j = 0; j < 16; j++){
			;//printf("%10f", mFrame[i]->mTransformMatrix.f[j]);
			//;//printf(" ");
			//;//printf(" %f", mpKey[i].mMatrix);
			if (j % 4 == 3)
				;//printf("\n");
		}
	}
#endif
}
void CModelXFrame::AnimateCombined(CMatrix44* parent){
	//�e����̕ϊ��s��ɁA�����̕ϊ��s���������
	mCombinedMatrix = (*parent) * mTransformMatrix;
	//�q�t���[���̍����s����쐬����
	for (int i = 0; i < mChild.size(); i++){
		mChild[i]->AnimateCombined(&mCombinedMatrix);
	}
}
/*
SetSkinWeightFrameIndex
�X�L���E�F�C�g�Ƀt���[���ԍ���ݒ肷��
*/
void CModel::SetSkinWeightFrameIndex(){
	//�t���[�������J��Ԃ�
	for (int i = 0; i < mFrame.size(); i++){
		//���b�V���ɖʂ������
		if (mFrame[i]->mMesh.mFaceNum > 0){
			//�X�L���E�F�C�g���J��Ԃ�
			for (int j = 0; j < mFrame[i]->mMesh.mSkinWeights.size(); j++){
				//�t���[�����̃t���[������������
				CModelXFrame* frame = FindFrame(mFrame[i]->mMesh.mSkinWeights[j]->mpFrameName);
				//�t���[���ԍ���ݒ肷��
				mFrame[i]->mMesh.mSkinWeights[j]->mFrameIndex = frame->mIndex;
			}
		}
	}
}
/* AnimateVertex ���_�ɃA�j���[�V������K�p */
void CMesh::AnimateVertex(CModel *model){
	//�A�j���[�V�����p�̒��_�G���A�N���A
	memset(mpAnimateVertex, 0, sizeof(CVector3)* mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector3)* mNormalNum);
	//�X�L���E�F�C�g���J��Ԃ�
	for (int i = 0; i < mSkinWeights.size(); i++){
		//�t���[���ԍ�����
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//�t���[�������s��ɃI�t�Z�b�g�s�������
		CMatrix44 mSkinningMatrix = model->mFrame[frameIndex]->mCombinedMatrix * mSkinWeights[i]->mOffset;
		//���_�����J��Ԃ�
		for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++){
			//���_�ԍ�����
			int index = mSkinWeights[i]->mpIndex[j];
			//�d�ݏ���
			float weight = mSkinWeights[i]->mpWeight[j];
			//���_�Ɩ@�����X�V����
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}
	}
	//�@���𐳋K������
	for (int i = 0; i < mNormalNum; i++){
		mpAnimateNormal[i] = mpAnimateNormal[i].normalize();
	}
}
/*
AnimateVertex
���_�ɃA�j���[�V������K�p����
*/
void CModel::AnimateVertex(){
	//�t���[�������J��Ԃ�
	for (int i = 0; i < mFrame.size(); i++){
		//���b�V���ɖʂ������
		if (mFrame[i]->mMesh.mFaceNum > 0){
			//���_���A�j���[�V�����ōX�V����
			mFrame[i]->mMesh.AnimateVertex(this);
		}
	}
}
/*
FindMaterial
�}�e���A�����ɊY������}�e���A����ԋp����
*/
CMaterial* CModel::FindMaterial(char* name){
	//�}�e���A���z��̃C�e���[�^�쐬
	std::vector<CMaterial*>::iterator itr;
	//�}�e���A���z���擪���珇�Ɍ���
	for (itr = mMaterial.begin(); itr != mMaterial.end(); itr++){
		//���O����v����΃}�e���A���̃|�C���^��ԋp
		if (strcmp(name, (*itr)->mpName) == 0){
			return *itr;
		}
	}
	//��������NULL��ԋp
	return NULL;
}
/*
FindTexture
name�ɊY������t�@�C���������e�N�X�`������������
�ԋp����
*/
CTexture* CModel::FindTexture(char* name){
	//�e�N�X�`���z��̃C�e���[�^�쐬
	std::vector<CTexture*>::iterator itr;
	//�e�N�X�`���z��̐擪���珇�Ɍ���
	for (itr = mTexture.begin(); itr != mTexture.end(); itr++){
		//���O����v����΃e�N�X�`���̃|�C���^��ԋp
		if (strcmp(name, (*itr)->mpName) == 0){
			return *itr;
		}
	}
	//��������NULL��ԋp
	return NULL;
}

