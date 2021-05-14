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

//CMeshクラスの定義
class CMesh {
public:
	
	int mVertexNum0;
	int mVertexNum;
	CVector* mpVertex;
	//コンストラクタ
	CMesh()
		:mVertexNum(0)
		, mpVertex(0)
	{}
	//デストラクタ
	~CMesh() {
		SAFE_DELETE_ARRAY(mpVertex);
	}
	//読み込み処理
	void Init(CModelX* model);
	
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
	char mToken[1024];//取り出した単語の領域0〜1023(1024個)
	std::vector<CModelXFrame*>mFrame;//フレームの配列

	CModelX() 
		:mpPointer(0)
	{}
	~CModelX() {
		if (mFrame.size() > 0) {
			delete mFrame[0];
		}
	}
	//ファイル読み込み
	void Load(char* file);
	void GetToken();
	//ノードの読み飛ばし
	void SkipNode();
};
#endif 