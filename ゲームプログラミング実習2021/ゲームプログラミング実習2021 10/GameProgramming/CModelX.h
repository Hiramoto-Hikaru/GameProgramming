#ifndef CMODELX_H
#define CMODELX_H
#include<vector>
#include"CMatrix.h"
#include"CVector.h"
//sizeof：サイズ（バイト数）を返す
#define ARRAY_SIZE(a)(sizeof(a)/sizeof(a[0]))
#define MODEL_FILE "sample.blend.x"//入力ファイル名
//領域開放をマクロ化
#define SAFE_DELETE_ARRAY(a) {if(a)delete[]a;a=0;}

//先に宣言だけしてCModelXFlameの処理に当てはまらせる
class CModelX;
class CMaterial;
class CSkinWeights;
class CAnimationSet;
//CMeshクラスの定義
class CMesh {
public:

	int mMaterialNum;//マテリアル数
	int mMaterialIndexNum;//マテリアル番号数(面数)
	int* mpMaterialIndex;//マテリアル番号
	std::vector<CMaterial*>mMaterial;//マテリアルデータ
	//アニメーションセットの配列
	std::vector<CAnimationSet*>mAnimationSet;
	int mVertexNum;
	CVector* mpVertex;
	int mFaceNum;//面数
	int* mpVertexIndex;//面を構成する頂点番号
	int mNormalNum;//法線数
	CVector* mpNormal;//法線データ
	//スキンウェイト
	std::vector<CSkinWeights*>mSkinWeights;
	//コンストラクタ
	CMesh()

		:mVertexNum(0)
		, mpVertex(0)
		,mFaceNum(0)
		,mpVertexIndex(nullptr)
		,mNormalNum(0)
		,mpNormal(nullptr)
		,mMaterialNum(0)
		,mMaterialIndexNum(0)
	    ,mpMaterialIndex(nullptr)
	{}
	//デストラクタ
	~CMesh() {
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
		//スキンウェイトの削除
		for (int i=0; i < mSkinWeights.size(); i++) {
			delete mSkinWeights[i];
		}
		for (int i = 0; i < mAnimationSet.size(); i++) {
			delete mAnimationSet[i];
		}
	}
	//読み込み処理
	void Init(CModelX* model);
	void Render();
};
class CModelXFrame {
public:

	CMesh mMesh;//Meshデータ
	//std::vector 可変長配列（後から数を増やせる配列）
	std::vector<CModelXFrame*>mChild;//子フレームの配列
	CMatrix mTransformMatrix;//変換行列
	char* mpName;//フレーム名前
	int mIndex;//フレーム番号
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame() {
		//子フレームをすべて開放する
		//イテレーター（ポインタの亜種）を使用
		std::vector<CModelXFrame*>::iterator itr;
		for (itr = mChild.begin(); itr != mChild.end(); itr++) {
			delete* itr;
		}
		//名前のエリアを開放する
		SAFE_DELETE_ARRAY(mpName);
	}
	void Render();
};

/*CModelX
Xファイルの３Dモデルデータをプログラムで認識する*/
class CModelX {
public:
	//浮動小数点データの取得
	float GetFloatToken();
	//整数データの取得
	int GetIntToken();
	char* mpPointer;//読み込み位置
	char mToken[1024];//取り出した単語の領域0～1023(1024個)
	std::vector<CModelXFrame*>mFrame;//フレームの配列
	std::vector<CAnimationSet*>mAnimationSet;
	CModelX() 
		:mpPointer(0)
	{}
	~CModelX() {
		if (mFrame.size() > 0) {
			delete mFrame[0];
		}
		for (int i = 0; i < mAnimationSet.size(); i++) {
			delete mAnimationSet[i];
		}
	}
	//ファイル読み込み
	void Load(char* file);
	void GetToken();
	//ノードの読み飛ばし
	void SkipNode();
	void Render();
};
/*CSkinWeights
スキンウェイトクラス
*/
class CSkinWeights {
public:
	char* mpFrameName;//フレーム名
	int mFrameIndex;//フレーム番号
	int mIndexNum;//頂点番号数
	int* mpIndex;//頂点番号配列
	float* mpWeight;//配点ウェイト配列
	CMatrix mOffset;//オフセットマトリックス

	CSkinWeights(CModelX* model);

	~CSkinWeights() {
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpIndex);
		SAFE_DELETE_ARRAY(mpWeight);
	}

};
/*
CAnimationSet
アニメーションセット
*/
class CAnimationSet {
public:
	//アニメーションセット名
	char* mpName;
	CAnimationSet(CModelX* model);
	~CAnimationSet() {
		SAFE_DELETE_ARRAY(mpName);

	}
};
#endif 