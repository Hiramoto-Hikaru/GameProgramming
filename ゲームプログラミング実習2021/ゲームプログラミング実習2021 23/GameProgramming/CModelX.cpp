#include"glut.h"
#include <stdio.h>
#include"CModelX.h"
#include<string.h>//文字列関数のインクルード
#include"CMaterial.h"


void CModelX::Load(char* file) {
	//ファイルサイズを取得する
	FILE* fp;//ファイルポインタ変数の作成
	fp = fopen(file, "rb");//ファイルのオープン
	if (fp == NULL) {
		printf("fopen error:%s\n", file);
		return;
	}
	//ファイルの最後へ移動
	fseek(fp, 0L, SEEK_END);
	//ファイルサイズの取得
	int size = ftell(fp);
	//ファイルサイズ+１バイト分の領域を確保
	char* buf = mpPointer = new char[size + 1];
	//ファイルから３Dのデータを読み込む
	//ファイルの先頭へ移動
	fseek(fp, 0L, SEEK_SET);
	//確保した領域にファイルサイズ分データを読み込む
	fread(buf, size, 1, fp);
	//最後に\０を設定する
	buf[size]='\0';


	 //文字列の最後まで繰り返し
	while (*mpPointer != '\0') {
		GetToken();//単語の取得
		//template 読み飛ばし
		if (strcmp(mToken, "template") == 0) {
			SkipNode();

		}
		//Materialのとき
		else if (strcmp(mToken, "Material") == 0) {
			new CMaterial(this);
		}
		//単語がflameの場合
		else if (strcmp(mToken, "Frame") == 0) {
			//フレーム名を作成する
			new CModelXFrame(this);
		}
		//単語がAnimetionSetの場合
		else if (strcmp(mToken, "AnimationSet") == 0) {
			new CAnimationSet(this);
		}
	}                    
    fclose(fp);//ファイルのクローズ
	SAFE_DELETE_ARRAY(buf);
	//スキンウェイトのフレーム番号設定
	SetSkinWeightFrameIndex();
}
/*
GetToken
文字列データから単語を一つ取得する*/
void CModelX::GetToken() {
	//*＝ポインタ変数(１バイト）
	char* p =mpPointer;
	char* q = mToken;
	//空白（）タブ（\r）改行（\ｒ）（\n）以外の文字になるまで読み飛ばす
	/*
	strchr(文字列、文字)
	文字列に文字が含まれていれば、見つかった文字へのポインタをかえす
	見つからなかったらNULLを返す
	*/
	// ||,,,or(もしくは)

	while (*p != '\0' && strchr("\t \r\n,;\"", *p))p++;
	if (*p == '{'|| * p == '}') {
		//{または}ならmTokenに代入し、次の文字へ
		*q++ = *p++;
	}
	else {
		//空白、タブ、改行、｝の文字になるまで、mTokenに代入する
		while (*p != '\0' && !strchr("\t \r\n,;\"}", *p))
			*q++ = *p++;//1バイト追加
	}
	*q = '\0';//mTokenの最後に￥０を代入
	mpPointer = p;//次の読み込むポイントを更新する
	//もしmTokenが//の場合はコメントなので改行まで読み飛ばす
	/*
	strcmp(文字列１、文字列2）
	文字列１と文字列２が等しい場合０を返します
	文字列１と文字列２が等しくない場合０以外を返します
	*/
	if (!strcmp("//", mToken)) {
		//改行まで読み飛ばす
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//読み込み位置を更新
		mpPointer = p;
		//単語を取得する
		GetToken();
	}
}
/*SkipNode
ノードを読み飛ばす
*/
void CModelX::SkipNode() {
	//文字が終わったら終了
	while (*mpPointer != '\0') {
		GetToken();//次の単語取得
		//{が見つかったらループ終了
		if (strchr(mToken, '{'))break;
	}
	int count= 1;
	//文字が終わるか、カウントが０になったら終了
	while (*mpPointer!='\0'&&count>0)
		
	{
		GetToken();//次の単語取得
		//{を見つけるとカウントアップ
		if (strchr(mToken, '{'))count++;
		//}を見つけるとカウントダウン
		else if (strchr(mToken, '}'))count--;
	}
}

/*SetSkinWeightFrameIndex
スキンウェイトにフレーム番号を設定する*/
void CModelX::SetSkinWeightFrameIndex() {
	//フレーム分繰り返し
	for (int i = 0; i < mFrame.size(); i++) {
		//メッシュに面があれば
		if (mFrame[i]->mMesh.mFaceNum > 0) {
			//スキンウェイト分繰り返し
			for (int j = 0; j < mFrame[i]->mMesh.mSkinWeights.size(); j++) {
				//フレーム名のフレームを取得する
				CModelXFrame* frame = FindFrame(mFrame[i]->mMesh.mSkinWeights[j]->mpFrameName);
				//フレーム番号を設定する
				mFrame[i]->mMesh.mSkinWeights[j]->mFrameIndex = frame->mIndex;
			}
		}
	}
}
/*CModelXFrame
model:CModelXインスタンスへのポインタ
フレームを作成する
読込中にFrameが見つかればフレームを作成し
子フレームに追加する*/
CModelXFrame::CModelXFrame(CModelX* model) {
	//現在のフレーム配列の要素数を取得し設定する
	mIndex = model->mFrame. size();
	//CModelXのフレーム行列に追加する
	model->mFrame.push_back(this);
	//変換行列を単位数列にする
	mTransformMatrix.Identity();
	//次の単語（フレーム名の予定）を取得する
	model->GetToken();//framename
	//フレーム名分エリアを確保する
	mpName = new char[strlen(model->mToken) + 1];
	//フレーム名をコピーする
	strcpy(mpName,model->mToken);
	//次の単語({の予定)を取得；
	model->GetToken();//{
	//文字がなくなったら終わり
	while (*model->mpPointer != '\0') {
		//次の単語取得
		model->GetToken();//Frame
		//}の場合は終了
		if (strchr(model->mToken, '}'))break;
		//新たなフレームの場合は子フレームに追加
		if (strcmp(model->mToken, "Frame") == 0) {
			//フレームを作成し子フレームの配列に追加
            mChild.push_back(new CModelXFrame(model));
		}
		else if (strcmp(model->mToken, "FrameTransformMatrix") == 0) {
			model->GetToken();//{
			for (int i = 0; i< ARRAY_SIZE(mTransformMatrix.mF); i++) {
				mTransformMatrix.mF[i]=model->GetFloatToken();	
				
			}
			model->GetToken();//}
		}
		else if (strcmp(model->mToken, "Mesh") == 0) {
			mMesh.Init(model);
		}
		else {
			//上記以外の要素は読み飛ばす
			model->SkipNode();
		}
	}
	//デバッグバージョンのみ有効
	#ifdef _DEBUG
	
	printf("%s\n", mpName);

     mTransformMatrix.Print();
	 
    #endif
}
void CModelX::AnimateVertex(CMatrix* mat) {
	//フレーム数分繰り返し
	for (int i = 0; i < mFrame.size(); i++) {
		//メッシュに面があれば
		if (mFrame[i]->mMesh.mFaceNum > 0) {
			//頂点をアニメーションで更新する
			mFrame[i]->mMesh.AnimateVertex(mat);
		}
	}
}
/*
Render
メッシュの面数が０以外なら描画する
*/
void CModelXFrame::Render() {
	if (mMesh.mFaceNum != 0) {
		mMesh.Render();
	}
}
/*FineFame
フレーム名の該当するフレームのアドレスを返す
*/
CModelXFrame* CModelX::FindFrame(char* name) {
	//イテレータの作成
	std::vector<CModelXFrame*>::iterator itr;
	//先頭から最後目で繰り返す
	for (itr = mFrame.begin(); itr!= mFrame.end(); itr++) {
		//名前が一致したかどうか
		if (strcmp(name, (*itr)->mpName) == 0) {
			//一致したらそのアドレスを返す
			return *itr;
		}
	}
	//一致するフレームがない場合はNULLを返す
		return NULL;
}
/*AnimateCombined
合成行列の作成*/
void CModelXFrame::AnimateCombined(CMatrix* parent) {
	//自分の返還行列に、親からの返還行列をかける
	mCombinedMatrix = mTransformMatrix * (*parent);
	//子フレームの合成行列を作成する
	for (int i = 0; i < mChild.size(); i++) {
		mChild[i]->AnimateCombined(&mCombinedMatrix);

	}
#ifdef _DEBUG
	
		/*printf("Frame:%s\n", mpName);
		mCombinedMatrix.Print();*/
	
	                      
#endif
}
/*GetfloatToken
単語を浮動小数点型のデータで返す
*/
float CModelX::GetFloatToken() {
	GetToken();
	//atof
	//文字列をfloat型へ変換
	return atof(mToken);
}
/*
GetIntToken
単語を整数型のデータで返す
*/
int CModelX::GetIntToken() {
	GetToken();
	return atoi(mToken);//AtoI：文字列をint型に変換
}
void CModelX::Render() {
	for (int i = 0; i < mFrame.size(); i++) {
		mFrame[i]->Render();
	}
}

/*AnimateFrame
フレームの変換行列をアニメーションデータで更新する
*/
void CModelX::AnimateFrame() {
	//アニメーションで適用されるフレームの変換行列をゼロクリアする
	for (int i = 0; i < mAnimationSet.size(); i++) {
		CAnimationSet* anim = mAnimationSet[i];
		//重みが０は飛ばす
		if (anim->mWeight == 0) continue;
		//フレーム分（Animation)分繰り返す
		for (int j = 0; j < anim->mAnimation.size(); j++) {
			CAnimation* animation = anim->mAnimation[j];
			//外応するフレームの変換行列をゼロクリアする
			memset(&mFrame[animation->mFrameIndex]->mTransformMatrix, 0, sizeof(CMatrix));
		}

	}
	//アニメーションに該当するフレームの変換行列を
	//アニメーションのデータで設定する
	for (int i = 0; i < mAnimationSet.size(); i++) {
		CAnimationSet* anim = mAnimationSet[i];
		//重みが０は飛ばす
		if (anim->mWeight == 0) continue;
		//フレーム分（Animation)分繰り返す
		for (int j = 0; j < anim->mAnimation.size(); j++) {
			//フレームを取得する
			CAnimation* animation = anim->mAnimation[j];
			CModelXFrame* frame = mFrame[animation->mFrameIndex];
			//キーがない場合はとばす
			if (animation->mpKey == 0)continue;
			//時間を取得
			float time = anim->mTime;
			//最初の時間より小さい時
			if (time < animation->mpKey[0].mTime) {
				//変換行列を０コマ目の行列で更新
				frame->mTransformMatrix += animation->mpKey[0].mMatrix *anim->mWeight;
			}
			//最後の時間より大きい場合
			else if (time >= animation->mpKey[animation->mKeyNum - 1].mTime) {
				//変換行列を最後のコマの行列で更新
				frame->mTransformMatrix += animation->mpKey[animation->mKeyNum - 1].mMatrix * anim->mWeight;
			}
			else {
				//時間の途中の場合
				for (int k = 1; k < animation->mKeyNum; k++) {
					//変換行列を線形補間にて更新
					if (time < animation->mpKey[k].mTime && animation->mpKey[k - 1].mTime != animation->mpKey[k].mTime) {
						float r = (animation->mpKey[k].mTime - time) /
							(animation->mpKey[k].mTime - animation->mpKey[k - 1].mTime);
						frame->mTransformMatrix +=
							(animation->mpKey[k - 1].mMatrix * r + animation->mpKey[k].mMatrix * (1 - r)) * anim->mWeight;
						break;
					}\
				}
			}
         #ifdef _DEBUG
			/*printf("Frame:%s\n", frame->mpName);
			
         frame->mTransformMatrix.Print();*/

        #endif
		}
	}
    


}
/*AnimateVertex
頂点にアニメーションを適用する
*/
void CModelX::AnimateVertex() {
	//フレーム数分繰り返し
	for (int i = 0; i < mFrame.size(); i++) {
		//メッシュに点があれば
		if (mFrame[i]->mMesh.mFaceNum > 0) {
			//頂点をアニメーションで更新する
			mFrame[i]->mMesh.AnimateVertex(this);
		}
	}
}
/*FindMaterial
マテリアル名に該当するマテリアルを返却する
*/
CMaterial* CModelX::FindMaterial(char* name) {
	//マテリアル配列のイテレータ作成
	std::vector<CMaterial*>::iterator itr;
	//マテリアル愛列を戦闘から順に検索
	for (itr = mMaterial.begin(); itr != mMaterial.end(); itr++) {
		//名前が一致すればマテリアルのポインタを返却
		if (strcmp(name, (*itr)->mName) == 0) {
			return *itr;
	    }
	}
	//ないときはNULLを返却
	return NULL;
}
/*
Init
Meshのデータを取り込む
*/
void CMesh::Init(CModelX* model)

{
	model->GetToken();  //{or名前
	if (!strchr(model->mToken, '{')) {
		//名前の場合次が｛
		model->GetToken();  //{
	}
	//頂点数の取得
	mVertexNum = model->GetIntToken();
	//頂点数分エリア確保
	mpVertex = new CVector[mVertexNum];//mVertexNumは８４個ある
	mpAnimateVertex = new CVector[mVertexNum];

	//頂点数分データを取り込む 
	for (int i = 0; i < mVertexNum; i++) {
		mpVertex[i].mX = model->GetFloatToken();
		mpVertex[i].mY = model->GetFloatToken();
		mpVertex[i].mZ = model->GetFloatToken();
	}
	mFaceNum = model->GetIntToken();//面数読み込み
	//頂点数は１面に３頂点
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3) {
		model->GetToken();//頂点読み飛ばし
		mpVertexIndex[i] = model->GetIntToken();
		mpVertexIndex[i + 1] = model->GetIntToken();
		mpVertexIndex[i + 2] = model->GetIntToken();
	}
	//文字がなくなったら終わり
	while (model->mpPointer != '\0') {

		model->GetToken();//Meshormals
		//}かっこの場合は終了
		if (strchr(model->mToken, '}'))
			break;
		if (strcmp(model->mToken, "MeshNormals") == 0) {
			model->GetToken();//{
			//法線データを取得
			mNormalNum = model->GetIntToken();
			//法線のデータを配列に取り込む	
			CVector* pNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i++) {
				pNormal[i].mX = model->GetFloatToken();
				pNormal[i].mY = model->GetFloatToken();
				pNormal[i].mZ = model->GetFloatToken();
			}
			//法線数＝面数*３
			mNormalNum = model->GetIntToken() * 3;//FaceNum
			int ni;
			//頂点ごとに法線データを設定する
			mpNormal = new CVector[mNormalNum];
			mpAnimateNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i += 3) {
				model->GetToken();//3
				ni = model->GetIntToken();
				mpNormal[i] = pNormal[ni];
				ni = model->GetIntToken();
				mpNormal[i + 1] = pNormal[ni];
				ni = model->GetIntToken();
				mpNormal[i + 2] = pNormal[ni];
			}
			delete[] pNormal;
			model->GetToken();//}
		}
		else if (strcmp(model->mToken, "MeshMaterialList") == 0) {
			model->GetToken();//{
			//Materialの数
			mMaterialNum = model->GetIntToken();
			//FaceNum
			mMaterialIndexNum = model->GetIntToken();
			//マテリアルインデックスの作成
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i < mMaterialIndexNum; i++) {
				mpMaterialIndex[i] = model->GetIntToken();

			}
			//マテリアルデータの作成
			for (int i = 0; i < mMaterialNum; i++) {
				model->GetToken(); //Material
				if (strcmp(model->mToken, "Material") == 0) {
					mMaterial.push_back(new CMaterial(model));

				}
				else {
					//{既出
					model->GetToken();//MaterialName
					mMaterial.push_back(
						model->FindMaterial(model->mToken));
					model->GetToken();//}
				}
				
			}
              model->GetToken();//End of MeshMaterialList
		}
		else if (strcmp(model->mToken, "SkinWeights") == 0) {
			//CSkinWeightsクラスのインスタンスを作成し配列に追加
			mSkinWeights.push_back(new CSkinWeights(model));
		}
		//テクスチャ座標のとき
		else if (strcmp(model->mToken, "MeshTextureCoords") == 0) {
			model->GetToken();//{
			//テクスチャ座標数を獲得
			int textureCoordsNum = model->GetIntToken() * 2;
			//テクスチャ座標のデータを配列に取り込む
			mpTextureCoords = new float[textureCoordsNum];
			for (int i = 0; i < textureCoordsNum; i++) {
				mpTextureCoords[i] = model->GetFloatToken() ;
			}
			model->GetToken();//}
		}
		else {
			//以外のノードは読み飛ばし
			model->SkipNode();
		}
	}
#ifdef  _DEBUG

	#endif
	
}

void CMesh::AnimateVertex(CMatrix* mat) {
	//アニメーション用の頂点エリアクリア
	memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);

	//スキンウェイト分繰り返し
	for (int i = 0; i < mSkinWeights.size(); i++) {
		//フレーム番号取得
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//フレーム合成行列にオフセット行列を合成
		CMatrix mSkinningMatrix = mSkinWeights[i]->mOffset * mat[frameIndex];
		//頂点数分繰り返し
		for (int j = 0;j < mSkinWeights[i]->mIndexNum; j++) {
			//頂点番号取得
			int index = mSkinWeights[i]->mpIndex[j];

			//重み取得
			float weight = mSkinWeights[i]->mpWeight[j];
			//頂点と法線を更新する
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}
		
	}
	//法線を正規化する
	for (int i = 0; i < mNormalNum; i++) {
		mpAnimateNormal[i] = mpAnimateNormal[i].Normalize();
	}
}
/*
Render
画面に描画する*/
void CMesh::Render() {
	/*頂点データ、法線データの配列を有効にする*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	/*頂点データ、法線データの場所を指定する*/
	glVertexPointer(3, GL_FLOAT, 0, mpAnimateVertex);
	glNormalPointer(GL_FLOAT, 0, mpAnimateNormal);
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoords);
	//テクスチャマッピングの配列を有効にする
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	for (int i = 0; i < mFaceNum; i++) {
		//マテリアルを適用する
		mMaterial[mpMaterialIndex[i]]->Enabled();
		/*頂点のインデックスの場所を指定して図形を描画する*/
		glDrawElements(GL_TRIANGLES, 3 , GL_UNSIGNED_INT,( mpVertexIndex+i*3));
		mMaterial[mpMaterialIndex[i]]->Disabled(); 
	}
	/*頂点データ、法線データの配列を無効にする*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	/*頂点のインデックスｎ場所を指定して図形を描画する
	*/
	
}
/*AnimateVertex 頂点にアニメーションを適用*/

void CMesh::AnimateVertex(CModelX* model) {
	//アニメーション用の頂点エリアクリア
	memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);
	//スキンウェイト分繰り返し
	for (int i = 0; i < mSkinWeights.size(); i++) {
		//フレーム番号取得
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//オフセット行列とフレーム合成行列を合成
		CMatrix mSkinningMatrix = mSkinWeights[i]->mOffset * model->mFrame[frameIndex]->mCombinedMatrix;
		//頂点数分繰り返し
		for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++) {
			//頂点番号取得
			int index = mSkinWeights[i]->mpIndex[j];
			//重み取得
			float weight = mSkinWeights[i]->mpWeight[j];
			//頂点と法線を更新する
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}
     }
	//法線を正規化する
	for (int i = 0; i < mNormalNum; i++) {
		mpAnimateNormal[i] = mpAnimateNormal[i].Normalize();
	}
}
/*
CSkinWeights
スキンウェイトの読み込み
*/
CSkinWeights::CSkinWeights(CModelX* model) 
	: mpFrameName(0)
	,mFrameIndex(0)
	,mIndexNum(0)
    ,mpIndex(nullptr)
    ,mpWeight(nullptr)
{
	model->GetToken();  //{
	model->GetToken(); //FrameName
	//フレーム名エリア確保、設定
	mpFrameName = new char[strlen(model->mToken) + 1];
	strcpy(mpFrameName, model->mToken);
	//頂点番号数取得
	mIndexNum = model->GetIntToken();
	//頂点番号数が０を超える
	if (mIndexNum > 0) {
		//頂点番号と頂点ウェイトのエリア確保
		mpIndex = new int[mIndexNum];
		mpWeight = new float[mIndexNum];
		//頂点番号取得
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = model->GetIntToken();
		//頂点ウェイト取得
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = model->GetFloatToken();

	}
	//オフセット行列取得
	for (int i = 0; i < 16; i++) {
		mOffset.mF[i] = model->GetFloatToken();
	}
	model->GetToken();//}
   #ifdef _DEBUG
	/*
	printf("SkinWeights %s\n",mpFrameName);
	for (int i = 0; i < mIndexNum; i++) {
		printf("%10d\t", mpIndex[i]);
		printf("%10f\n", mpWeight[i]);
	}
	for (int i = 0; i < 13; i+=4) {
		printf("%10f\t", mOffset.mF[i]);
		printf("%10f\t", mOffset.mF[i+1]);
		printf("%10f\t", mOffset.mF[i+2]);
		printf("%10f\n", mOffset.mF[i+3]);
	}
	*/
   #endif
}
/*
CAnimationSet
*/
CAnimationSet::CAnimationSet(CModelX* model)
	:mpName(nullptr)
	,mTime(0)
	,mWeight(0)
	,mMaxTime(0)
{
	model->mAnimationSet.push_back(this);
	model->GetToken();//Animation Name
	//アニメーションセット名を退避
	mpName = new char[strlen(model->mToken) + 1];
	strcpy(mpName, model->mToken);
	model->GetToken(); //{
	while (*model->mpPointer != '\0') {
		model->GetToken();//}orAnimetion
		if (strchr(model->mToken, '}'))break;
		if (strcmp(model->mToken, "Animation") == 0) {
			//Animation要素読み込み
			mAnimation.push_back(new CAnimation(model));
			
			
		}
	}
#ifdef _DEBUG
	//printf("AnimationSet:%s\n", mpName);
#endif
	//終了時間設定
	mMaxTime = mAnimation[0]->mpKey[mAnimation[0]->mKeyNum - 1].mTime;
}
CAnimation::CAnimation(CModelX* model)
	:mpFrameName(0)
	, mFrameIndex(0)
	, mKeyNum(0)
	, mpKey(nullptr)


{
	model->GetToken();//{or Animation Name
	if (strchr(model->mToken, '{')) {
		model->GetToken();//{
	}
	else {
		model->GetToken();//{
		model->GetToken();//{
	}
	model->GetToken();//FrameName
	mpFrameName = new char[strlen(model->mToken) + 1];
	strcpy(mpFrameName, model->mToken);
	mFrameIndex = model->FindFrame(model->mToken)->mIndex;
	model->GetToken();//}
	//キーの配列を保存しておく配列
	CMatrix* key[4] = { 0,0,0,0 };
	//時間の配列を保存しておく配列
	float* time[4] = { 0,0,0,0 };
	while (*model->mpPointer != '\0') {
		model->GetToken();//}or AnimetionKey
		if (strchr(model->mToken, '}'))break;
		if (strcmp(model->mToken, "AnimationKey") == 0) {

			model->GetToken();//{
			//データのタイプ取得
			int type = model->GetIntToken();
			//時間数取得

			mKeyNum = model->GetIntToken();
			switch (type) {
			case 0://Rotation Quaternion
				//行列の配列を時間数分確保
				key[type] = new CMatrix[mKeyNum];
				//時間の配列を時間数分確保
				time[type] = new float[mKeyNum];
				//時間数分繰り返す
				for (int i = 0; i < mKeyNum; i++) {
					//時間取得
					time[type][i] = model->GetFloatToken();
					model->GetToken();//4を読み飛ばし
					//w,x,y,zを取得
					float w = model->GetFloatToken();
					float x = model->GetFloatToken();
					float y = model->GetFloatToken();
					float z = model->GetFloatToken();
					//クォータニオンから回転行列に変換
					key[type][i].SetQuaternion(x, y, z, w);

				
				}
				break;
			case 1://拡大縮小の行列作成
				key[type] = new CMatrix[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++) {
					time[type][i] = model->GetFloatToken();
					model->GetToken();//3
					float x = model->GetFloatToken();
					float y = model->GetFloatToken();
					float z = model->GetFloatToken();
					key[type][i].mM[0][0] = x;
					key[type][i].mM[1][1] = y;
					key[type][i].mM[2][2] = z;

				}
				break;
			case 2://移動の行列作成
				key[type] = new CMatrix[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++) {
					time[type][i] = model->GetFloatToken();
					model->GetToken();//3
					float x = model->GetFloatToken();
					float y = model->GetFloatToken();
					float z = model->GetFloatToken();
					key[type][i].Translate(x, y, z);

				}
				break;
			case 4://行列データを取得
				mpKey = new CAnimationKey[mKeyNum];
				for (int i = 0; i < mKeyNum; i++) {
					mpKey[i].mTime = model->GetFloatToken();//Time
					model->GetToken();//16
					for (int j = 0; j < 16; j++) {
						mpKey[i].mMatrix.mF[j] = model->GetFloatToken();
					}
				}
				break;
			}
			model->GetToken();//}
		}
		else {
			model->SkipNode();
		}
	}//whileの終わり
	//行列データではないとき
	if (mpKey == 0) {
		//時間数分キーを作成
		mpKey = new CAnimationKey[mKeyNum];
		for (int i = 0; i < mKeyNum; i++) {
			//時間設定
			mpKey[i].mTime = time[2][i];//Time
			//行列作成Size*Rotation*Position
			mpKey[i].mMatrix = key[1][i] * key[0][i] * key[2][i];
           
		}
	}
	//確保したエリア開放
	for (int i = 0; i < ARRAY_SIZE(key); i++) {
		SAFE_DELETE_ARRAY(time[i]);
		SAFE_DELETE_ARRAY(key[i]);
	}
}
