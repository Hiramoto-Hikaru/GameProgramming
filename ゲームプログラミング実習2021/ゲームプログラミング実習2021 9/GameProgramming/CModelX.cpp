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
		//単語がflameの場合
		if (strcmp(mToken, "Frame") == 0) {
			//フレーム名を作成する
			new CModelXFrame(this);
		}
		//単語がAnimationSetの場合
		if (strcmp(mToken, "AnimationSet") == 0) {
			printf("%s", mToken);//Frame出力
			GetToken();//Frame名を取得
			printf("%s\n", mToken);//Frame名を出力
		}
	}                    
    fclose(fp);//ファイルのクローズ
	SAFE_DELETE_ARRAY(buf);
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
/*
Render
メッシュの面数が０以外なら描画する
*/
void CModelXFrame::Render() {
	if (mMesh.mFaceNum != 0) {
		mMesh.Render();
	}
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
/*
Init
Meshのデータを取り込む
*/
void CMesh::Init(CModelX* model) {
	model->GetToken();  //{or名前
	if (!strchr(model->mToken, '{')) {
		//名前の場合次が｛
		model->GetToken();  //{
	}
	//頂点数の取得
	mVertexNum = model->GetIntToken();
	//頂点数分エリア確保
	mpVertex = new CVector[mVertexNum];//mVertexNumは８４個ある
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
				model->GetToken();//}//End of MeshMaterialList
			}
		}
	}

#ifdef  _DEBUG

	//int型の変数の出力は％ｄ
	printf("VertexNum:%d\n", mVertexNum);
	for (int i = 0; i < mVertexNum; i++) {
		//10f=１０桁のfloat型
		printf("%10f", mpVertex[i].mX);
		printf("%10f", mpVertex[i].mY);
		printf("%10f\n", mpVertex[i].mZ);
	}

	printf("FaceNum:%d\n", mFaceNum);
	for (int i = 0; i < mFaceNum * 3; i += 3) {

		printf("%d\t", mpVertexIndex[i]);
		printf("%d\t", mpVertexIndex[i + 1]);
		printf("%d\n", mpVertexIndex[i + 2]);

	}
	//法線データを頂点の数だけ出力
	printf("NormalNum:%d\n", mNormalNum);
	for (int i = 0; i < mNormalNum; i += 3) {
		printf("%10f\t", mpNormal[i].mX);
		printf("%10f\t", mpNormal[i].mY);
		printf("%10f\n", mpNormal[i].mZ);
		printf("%10f\t", mpNormal[i + 1].mX);
		printf("%10f\t", mpNormal[i + 1].mY);
		printf("%10f\n", mpNormal[i + 1].mZ);
		printf("%10f\t", mpNormal[i + 2].mX);
		printf("%10f\t", mpNormal[i + 2].mY);
		printf("%10f\n", mpNormal[i + 2].mZ);
	}
	printf("Material\n");
	printf("Diffuse:\t");
	for (int i = 0; i < mFaceNum; i++) {
		printf("10f\t", mpMaterialIndex[i]);
        printf("10f\t", mpMaterialIndex[i + 1]);
		printf("10f\n", mpMaterialIndex[i + 2]);
	}
	printf("Diffuse:\t");

	printf("10f\t", CMaterial;; mDiffuse[0]);
	printf("10f\t", mDiffuse[1]);
	printf("10f\t", mDiffuse[2]);
	printf("10f\n", mDiffuse[3]);

	printf("Power:10f\n", mPower);

	printf("Specular:\t");
	printf("10f\t", mSpecular[0]);
	printf("10f\t", mSpecular[1]);
	printf("10f\n", mSpecular[2]);

	printf("Emissive\t");
	printf("Emissive:10f\t", mEmissive[0]);
	printf("Emissive:10f\t", mEmissive[1]);
	printf("Emissive:10f\n", mEmissive[2]);
	
	
	#endif
	
}
/*
Render
画面に描画する*/
void CMesh::Render() {
	/*頂点データ、法線データの配列を有効にする*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	/*頂点データ、法線データの場所を指定する*/
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);

	for (int i = 0; i < mFaceNum; i++) {
		//マテリアルを適用する
		mMaterial[mpMaterialIndex[i]]->Enabled;
		/*頂点のインデックスの場所を指定して図形を描画する*/
		glDrawElements(GL_TRIANGLES, 3 * mFaceNum, GL_UNSIGNED_INT, mpVertexIndex+i*3);
	}
	/*頂点データ、法線データの配列を無効にする*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	/*頂点のインデックスｎ場所を指定して図形を描画する
	*/
	
}
