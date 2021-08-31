#include "CMaterial.h"
//memsetのインクルード
#include <string.h>
#include "glut.h"

//デフォルトコンストラクタ
CMaterial::CMaterial()
:mVertexNum(0)
,mpTextureFilename(0)

{
	
	//名前を0で埋める
	memset(mName, 0, sizeof(mName));
	//0で埋める
	memset(mDiffuse, 0, sizeof(mDiffuse));
   
}
/*Materialデータの読み込みと設定
*/
CMaterial::CMaterial(CModelX* model)
	:mpTextureFilename(0)
   ,mVertexNum(0)

{ 
	//CModelXにマテリアルを追加する
	model->mMaterial.push_back(this);
	model->GetToken();//{//{?Name
	if (strcmp(model->mToken, "{") != 0) {
		//{でないときはマテリアル名
		strcpy(mName, model->mToken);
		model->GetToken();//{

	}
	mDiffuse[0] = model->GetFloatToken();
	mDiffuse[1] = model->GetFloatToken();
	mDiffuse[2] = model->GetFloatToken();
	mDiffuse[3] = model->GetFloatToken();
	mPower = model->GetFloatToken();

	mSpecular[0] = model->GetFloatToken();
	mSpecular[1] = model->GetFloatToken();
	mSpecular[2] = model->GetFloatToken();

	mEmissive[0] = model->GetFloatToken();
	mEmissive[1] = model->GetFloatToken();
	mEmissive[2] = model->GetFloatToken();

	model->GetToken();//TextureFilenameor}
	if (strcmp(model->mToken, "TextureFilename") == 0) {
		//テクスチャありの場合、テクスチャファイル名取得	
		model->GetToken();//{
		model->GetToken();//filename
		mpTextureFilename = new char[strlen(model->mToken) + 1];
		strcpy(mpTextureFilename, model->mToken);

		//テクスチャファイルのよみこみ
		mTexture.Load(mpTextureFilename);
		model->GetToken();//}
		model->GetToken();//}
	}
#ifdef  _DEBUG
	printf("%s\n",mName);
	printf("Diffuse:\t");

	printf("%10f\t", mDiffuse[0]);
	printf("%10f\t", mDiffuse[1]);
	printf("%10f\t", mDiffuse[2]);
	printf("%10f\n", mDiffuse[3]);

	printf("Power:%10f\n", CMaterial::mPower);

	printf("Specular:\t");
	printf("%10f\t", mSpecular[0]);
	printf("%10f\t", mSpecular[1]);
	printf("%10f\n", mSpecular[2]);

	printf("Emissive:\t");
	printf("%10f\t", mEmissive[0]);
	printf("%10f\t", mEmissive[1]);
	printf("%10f\n", mEmissive[2]);




#endif
}

//マテリアルを有効にする
void CMaterial::Enabled() {
	//拡散光の設定
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
	//テクスチャ有り
	if (mTexture.mId)
	{
		//テクスチャを使用可能にする
		glEnable(GL_TEXTURE_2D);
		//テクスチャをバインドする
		glBindTexture(GL_TEXTURE_2D, mTexture.mId);
		//アルファブレンドを有効にする
		glEnable(GL_BLEND);
		//ブレンド方法を指定
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

//テクスチャを読み込む
void CMaterial::LoadTexture(char *file)
{
	mTexture.Load(file);
}
//マテリアルを無効にする
void CMaterial::Disabled()
{
	//テクスチャ有り
	if (mTexture.mId)
	{
		//アルファブレンドを無効
		glDisable(GL_BLEND);
		//テクスチャのバインドを解く
		glBindTexture(GL_TEXTURE_2D, 0);
		//テクスチャを無効にする
		glDisable(GL_TEXTURE_2D);
	}
}
