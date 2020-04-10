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
model:CModelXインスタンスへのポインタ
*/
CModelXFrame::CModelXFrame(CModel* model){
	//現在のフレーム配列の要素数を所得し設定する
	mIndex = model->mFrame.size();
	//CModelXのフレーム配列に追加する
	model->mFrame.push_back(this);
	//変換行列を単位行列にする
	mTransformMatrix.Identity();
	//次の単語（フレーム名の予定）を所得する
	model->GetToken();
	//フレーム名分エリアを確保する
	mpName = new char[strlen(model->mToken) + 1];
	//フレーム名をコピーする
	strcpy(mpName, model->mToken);
	//次の単語({の予定）を所得する
	model->GetToken(); //{
	//文字が無くなったら終わり
	while (*model->mpPointer != '\0'){
		//次の単位所得
		model->GetToken(); // Frame
		//}かっこの場合は終了
		if (strchr(model->mToken, '}'))break;
		//新たなフレームを作成し、子フレームに追加
		if (strcmp(model->mToken, "Frame") == 0){
			//フレームを作成し、子フレームの配列に追加
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
			//上記以外の要素は読み飛ばす
			model->SkipNode();
		}
	}
	//デバッグバージョンのみ有効
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
ノードを読み飛ばす
*/
void CModel::SkipNode(){
	//文字が終わったら終了
	while (*mpPointer != '\0'){
		GetToken();
		//{が見つかったらループ終了
		if (strchr(mToken, '{'))break;
	}
	int count = 1;
	//文字が終わるか、カウントが０になったら終了
	while (*mpPointer != '\0' && count > 0){
		GetToken();
		//{を見つけるとカウントアップ
		if (strchr(mToken, '{'))count++;
		//}を見つけるとカウントダウン
		else if (strchr(mToken, '}'))count--;
	}
}
/*
GetFloatToken
単語を浮動小数点のデータで返す
*/
float CModel::GetFloatToken(){
	GetToken();
	//atof
	//文字列をfloat型へ変換
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
	//頂点数の所得
	mVertexNum = model->GetIntToken();
	//頂点数分エリア確保
	mpVertex = new CVector3[mVertexNum];
	mpAnimateVertex = new CVector3[mVertexNum];
	//頂点数分データを取り込む
	for (int i = 0; i < mVertexNum; i++){
		mpVertex[i].x = model->GetFloatToken();
		mpVertex[i].y = model->GetFloatToken();
		mpVertex[i].z = model->GetFloatToken();

	}
	mFaceNum = model->GetIntToken();
	//頂点数は1面に３頂点
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3){
		model->GetToken(); //頂点数読み飛ばし
		mpVertexIndex[i] = model->GetIntToken();
		mpVertexIndex[i + 1] = model->GetIntToken();
		mpVertexIndex[i + 2] = model->GetIntToken();
	}
	while (model->mpPointer != '\0'){
		model->GetToken(); //MeshNormals
		//}かっこの場合は終了
		if (strchr(model->mToken, '}'))
			break;
		if (strcmp(model->mToken, "MeshNormals") == 0){
			model->GetToken(); //{
			//法線データ数を所得
			mNormalNum = model->GetIntToken();
			//法線のデータを配列に取り込む
			CVector3 *pNormal = new CVector3[mNormalNum];
			for (int i = 0; i < mNormalNum; i++){
				pNormal[i].x = model->GetFloatToken();
				pNormal[i].y = model->GetFloatToken();
				pNormal[i].z = model->GetFloatToken();
			}
			//法線数＝面積×３
			mNormalNum = model->GetIntToken() * 3; //FaceNum
			int ni;
			//頂点毎に法線データを設定する
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
		// MeshMaterialListのとき
		else if (strcmp(model->mToken, "MeshMaterialList") == 0){
			model->GetToken();//{
			//Materialの数
			mMaterialNum = model->GetIntToken();
			//FaceNum
			mMaterialIndexNum = model->GetIntToken();
			//マテリアルインデックスの作成
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i < mMaterialIndexNum; i++){
				mpMaterialIndex[i] = model->GetIntToken();
			}
			//マテリアルデータの作成
			for (int i = 0; i < mMaterialNum; i++){
				model->GetToken();  //Material
				if (strcmp(model->mToken, "Material") == 0){
					mMaterial.push_back(new CMaterial(model));
				}
				else{
					// { 既出
					model->GetToken(); //MaterialName
					mMaterial.push_back(
						model->FindMaterial(model->mToken));
					model->GetToken(); // }
				}
			}
			model->GetToken(); //}//End of MeshMaterialList
		}
		else if (strcmp(model->mToken, "SkinWeights") == 0){
			//CSkinWeightsクラスのインスタンスを作成し、配列に追加
			mSkinWeights.push_back(new CSkinWeights(model));
		}
		//テクスチャ座標の時
		else if (strcmp(model->mToken, "MeshTextureCoords") == 0){
			model->GetToken(); //{
			//テクスチャ座標数を所得
			int textureCoordsNum = model->GetIntToken();
			//テクスチャ座標のデータを配列に取り込む
			mpTextureCoords = new CVector2[textureCoordsNum];
			for (int i = 0; i < textureCoordsNum; i++){
				mpTextureCoords[i].x = model->GetFloatToken();
				mpTextureCoords[i].y = model->GetFloatToken();
			}
			model->GetToken(); //}
		}
		else{
			//以外のノードは読み飛ばし
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
画面に描画する
*/
void CMesh::Render(){
	/* 頂点データ、法線データの配列を有効にする*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	/* 頂点データ、法線データの場所を指定する*/
	glVertexPointer(3, GL_FLOAT, 0, mpAnimateVertex);
	glNormalPointer(GL_FLOAT, 0, mpAnimateNormal);
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoords);

	/* 頂点のインデックスの場所を指定して図形を描画する*/
	//glDrawElements(GL_TRIANGLES, 3 * mFaceNum, GL_UNSIGNED_INT, mpVertexIndex);
	/* 頂点のインデックスの場所を指定して図形を描画する*/
	for (int i = 0; i < mFaceNum; i++){
		//マテリアルを適用する
		mMaterial[mpMaterialIndex[i]]->SetMaterial();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (mpVertexIndex + i * 3));
		//マテリアルを解除する
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
	//テクスチャの解放
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
	//文字列の最後まで繰り返し
	while (*mpPointer != '\0'){
		GetToken();//単語の所得
		//template 読み飛ばし
		if (strcmp(mToken, "template") == 0){
			SkipNode();
		}
		//Material の時
		else if (strcmp(mToken, "Material") == 0){
			new CMaterial(this);
		}
		//単語がFrameの場合
		else if (strcmp(mToken, "Frame") == 0){
			//フレームを作成する
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
		//単語がAnimationSetの場合
		else if (strcmp(mToken, "AnimationSet") == 0){
			new CAnimationSet(this);
		}
	}
	SAFE_DELETE_ARRAY(buf);
	//スキンウェイトのフレーム番号設定
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
	//ダミーのマテリアルの追加
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
			//ダミーマテリアルの前まで検索
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
メッシュの面数が0以外なら描画する
*/
void CModelXFrame::Render(){
	if (mMesh.mFaceNum != 0)
		mMesh.Render();
}
/*
Render
全てのフレームの描画処理を呼び出す
*/
void CModel::Render(){
	for (int i = 0; i < mFrame.size(); i++){
		mFrame[i]->Render();
	}
}
void CModel::Render(const CMatrix &matrix) {
	/* 頂点データ，法線データの配列を有効にする */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//テクスチャマッピングのデータを有効にする
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	/* 頂点データ，法線データの配列を指定する */
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoord);

	glPushMatrix();
	glMultMatrixf(&matrix.mM[0][0]);

	int first = 0, count;
	for (int i = 0; i < mMaterials.size(); i++) {
		count = mMaterials[i]->mVertexNo - first;
		/* 頂点配列の図形を描画する */
		mMaterials[i]->SetMaterial();
		glDrawArrays(GL_TRIANGLES, first, count);
		mMaterials[i]->UnSetMaterial();
		first += count;
	}

	glPopMatrix();

	/* 頂点データ，法線データの配列を無効にする */
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//テクスチャマッピングのデータを無効にする
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return;
}
/*
CSkinWeights
スキンウェイトの読み込み
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
	//フレーム名エリア確保、設定
	mpFrameName = new char[strlen(model->mToken) + 1];
	strcpy(mpFrameName, model->mToken);
	//頂点番号数所得
	mIndexNum = model->GetIntToken();
	//頂点番号数が0を超える
	if (mIndexNum > 0){
		//頂点番号と頂点ウェイトのエリア確保
		mpIndex = new int[mIndexNum];
		mpWeight = new float[mIndexNum];
		//頂点番号所得
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = model->GetIntToken();
		//頂点ウェイト所得
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = model->GetFloatToken();
	}
	//オフセット行列所得
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
	//アニメーションの名前を退避
	mpName = new char[strlen(model->mToken) + 1];
	strcpy(mpName, model->mToken);
	model->GetToken(); //{
	while (*model->mpPointer != '\0'){
		model->GetToken(); // } or Animation
		if (strchr(model->mToken, '}'))break;
		if (strcmp(model->mToken, "Animation") == 0){
			////とりあえず読み飛ばし
			//model->SkipNode();
			//Animation要素読み込み
			mAnimation.push_back(new CAnimation(model));
		}
	}
	//終了時間設定
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

	//キーの配列を保存しておく配列
	CMatrix44 *key[4] = { 0, 0, 0, 0 };
	//時間の配列を保存しておく配列
	float *time[4] = { 0, 0, 0, 0 };
	while (*model->mpPointer != '\0'){
		model->GetToken(); // } or AnimationKey
		if (strchr(model->mToken, '}'))break;
		if (strcmp(model->mToken, "AnimationKey") == 0){
			model->GetToken();//{
			//データのタイプ所得
			int type = model->GetIntToken();
			//時間数所得
			mKeyNum = model->GetIntToken();
			switch (type){
			case 0: // Rotation Quaternion
				//行列の配列を時間数分確保
				key[type] = new CMatrix44[mKeyNum];
				//時間の配列を時間数分確保
				time[type] = new float[mKeyNum];
				//時間数分繰り返す
				for (int i = 0; i < mKeyNum; i++){
					//時間所得
					time[type][i] = model->GetFloatToken();
					model->GetToken(); // 4を読み飛ばし
					//w,x,y,zを所得
					float w = model->GetFloatToken();
					float x = model->GetFloatToken();
					float y = model->GetFloatToken();
					float z = model->GetFloatToken();
					//クォータニオンから回転行列に変換
					key[type][i].SetQuaternion(x, y, z, w);
				}
				break;
			case 1: //拡大・縮小の行列作成
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
			case 2: //移動の行列作成
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
			case 4: //行列データを所得
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
	//行列データではないとき
	if (mpKey == 0){
		//時間数分キーを作成
		mpKey = new CAnimationKey[mKeyNum];
		for (int i = 0; i < mKeyNum; i++){
			//時間設定
			mpKey[i].mTime = time[2][i]; // Time
			//行列作成 Position * Rotation * Size
			mpKey[i].mMatrix = key[2][i] * key[0][i] * key[1][i];
		}
	}
	//確保したエリア開放
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
フレームの変換行列をアニメーションデータで更新する
*/
void CModel::AnimateFrame(){
	//アニメーションで適用されるフレームの
	//変換行列をゼロクリアする
	for (int i = 0; i < mAnimationSet.size(); i++){
		CAnimationSet* anim = mAnimationSet[i];
		//重みが０は飛ばす
		if (anim->mWeight == 0)continue;
		//フレーム分(Animation分)繰り返す
		for (int j = 0; j < anim->mAnimation.size(); j++){
			CAnimation* animation = anim->mAnimation[j];
			//該当するフレームの変換行列をゼロクリアする
			memset(&mFrame[animation->mFrameIndex]->mTransformMatrix, 0, sizeof(CMatrix44));
		}
	}
	//アニメーションに該当するフレームの変換行列を
	//アニメーションのデータで設定する
	for (int i = 0; i < mAnimationSet.size(); i++){
		CAnimationSet* anim = mAnimationSet[i];
		//重みが０は飛ばす
		if (anim->mWeight == 0)continue;
		//フレーム分(Animation分)繰り返す
		for (int j = 0; j < anim->mAnimation.size(); j++){
			//フレームを所得する
			CAnimation* animation = anim->mAnimation[j];
			CModelXFrame* frame = mFrame[animation->mFrameIndex];
			//キーがない場合は飛ばず
			if (animation->mpKey == 0)continue;
			//時間を所得
			float time = anim->mTime;
			//最初の時間より小さい場合
			if (time < animation->mpKey[0].mTime){
				//変換行列を０コマ目の行列で更新
				frame->mTransformMatrix += animation->mpKey[0].mMatrix * anim->mWeight;
			}
			//最後の時間より大きい場合
			else if (time >= animation->mpKey[animation->mKeyNum - 1].mTime){
				//変換行列を最後のコマの行列で更新
				frame->mTransformMatrix += animation->mpKey[animation->mKeyNum - 1].mMatrix * anim->mWeight;
			}
			else{
				//時間の途中の場合
				for (int k = 1; k < animation->mKeyNum; k++){
					//変換行列を、線形補間にて更新
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
	//親からの変換行列に、自分の変換行列をかける
	mCombinedMatrix = (*parent) * mTransformMatrix;
	//子フレームの合成行列を作成する
	for (int i = 0; i < mChild.size(); i++){
		mChild[i]->AnimateCombined(&mCombinedMatrix);
	}
}
/*
SetSkinWeightFrameIndex
スキンウェイトにフレーム番号を設定する
*/
void CModel::SetSkinWeightFrameIndex(){
	//フレーム数分繰り返し
	for (int i = 0; i < mFrame.size(); i++){
		//メッシュに面があれば
		if (mFrame[i]->mMesh.mFaceNum > 0){
			//スキンウェイト分繰り返し
			for (int j = 0; j < mFrame[i]->mMesh.mSkinWeights.size(); j++){
				//フレーム名のフレームを所得する
				CModelXFrame* frame = FindFrame(mFrame[i]->mMesh.mSkinWeights[j]->mpFrameName);
				//フレーム番号を設定する
				mFrame[i]->mMesh.mSkinWeights[j]->mFrameIndex = frame->mIndex;
			}
		}
	}
}
/* AnimateVertex 頂点にアニメーションを適用 */
void CMesh::AnimateVertex(CModel *model){
	//アニメーション用の頂点エリアクリア
	memset(mpAnimateVertex, 0, sizeof(CVector3)* mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector3)* mNormalNum);
	//スキンウェイト分繰り返し
	for (int i = 0; i < mSkinWeights.size(); i++){
		//フレーム番号所得
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//フレーム合成行列にオフセット行列を合成
		CMatrix44 mSkinningMatrix = model->mFrame[frameIndex]->mCombinedMatrix * mSkinWeights[i]->mOffset;
		//頂点数分繰り返し
		for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++){
			//頂点番号所得
			int index = mSkinWeights[i]->mpIndex[j];
			//重み所得
			float weight = mSkinWeights[i]->mpWeight[j];
			//頂点と法線を更新する
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}
	}
	//法線を正規化する
	for (int i = 0; i < mNormalNum; i++){
		mpAnimateNormal[i] = mpAnimateNormal[i].normalize();
	}
}
/*
AnimateVertex
頂点にアニメーションを適用する
*/
void CModel::AnimateVertex(){
	//フレーム数分繰り返し
	for (int i = 0; i < mFrame.size(); i++){
		//メッシュに面があれば
		if (mFrame[i]->mMesh.mFaceNum > 0){
			//頂点をアニメーションで更新する
			mFrame[i]->mMesh.AnimateVertex(this);
		}
	}
}
/*
FindMaterial
マテリアル名に該当するマテリアルを返却する
*/
CMaterial* CModel::FindMaterial(char* name){
	//マテリアル配列のイテレータ作成
	std::vector<CMaterial*>::iterator itr;
	//マテリアル配列を先頭から順に検索
	for (itr = mMaterial.begin(); itr != mMaterial.end(); itr++){
		//名前が一致すればマテリアルのポインタを返却
		if (strcmp(name, (*itr)->mpName) == 0){
			return *itr;
		}
	}
	//無い時はNULLを返却
	return NULL;
}
/*
FindTexture
nameに該当するファイル名を持つテクスチャを検索して
返却する
*/
CTexture* CModel::FindTexture(char* name){
	//テクスチャ配列のイテレータ作成
	std::vector<CTexture*>::iterator itr;
	//テクスチャ配列の先頭から順に検索
	for (itr = mTexture.begin(); itr != mTexture.end(); itr++){
		//名前が一致すればテクスチャのポインタを返却
		if (strcmp(name, (*itr)->mpName) == 0){
			return *itr;
		}
	}
	//無い時はNULLを返却
	return NULL;
}

