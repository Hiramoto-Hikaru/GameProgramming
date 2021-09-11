#include "CSceneGame.h"
//
#include "CCamera.h"
//
#include "CUtil.h"
#include"CRes.h"
#include"CKey.h"
#include"CMaterial.h"
#include"CXCharacter.h"
#include"CXPlayer.h"
#include"CXEnemy.h"

//キャラクタのインスタンス
CXPlayer mPlayer;
CMatrix Matrix;
//敵のインスタンス
CXEnemy mEnemy;
CSceneGame::~CSceneGame() {

}

void CSceneGame::Init() {

	//テキストフォントの読み込みと設定
	mFont.LoadTexture("FontG.tga", 1, 4096 / 64);
	//３Dモデルの読み込み
	CRes::sModelX.Load(MODEL_FILE);
	//キャラクターにモデルを設定
	mPlayer.Init(&CRes::sModelX);
	//敵にモデルを設定
	mEnemy.Init(&CRes::sModelX);
	//敵の配置
	mEnemy.mPosition = CVector(7.0f, 0.0f, 0.0f);
	CMatrix matrix;
	matrix.Print();
	i = 0;
}


void CSceneGame::Update() {

	/*if (mPlayer.mAnimationFrame >= mPlayer.mAnimationFrameSize) {
           mPlayer.ChangeAnimation(i, true, 60);
			i++;
	}*/
	//キャラクタークラスの更新
	mPlayer.Update();
	//敵の更新
	mEnemy.Update();
	mEnemy.Render();

	//最初のアニメーションの現在時間を４５にする

	//CRes::sModelX.mAnimationSet[0]->mTime=0;
	//CRes::sModelX.mAnimationSet[0]->mTime += 1.0f;
	//CRes::sModelX.mAnimationSet[0]->mTime = (int)CRes::sModelX.mAnimationSet[0]->mTime %
		//(int)(CRes::sModelX.mAnimationSet[0]->mMaxTime + 1);
	//最初のアニメーションの重みを１．０（１００％）にする
	//CRes::sModelX.mAnimationSet[0]->mWeight = 1.0f;
	//フレームの変換行列をアニメーションで更新する
	//CRes::sModelX.AnimateFrame();
	//フレームの合成行列を計算する
	//CRes::sModelX.mFrame[0]->AnimateCombined(&Matrix);
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
	e = CVector(1.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector();
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);

	//カメラクラスの設定
	Camera.Set(e, c, u);
	Camera.Render();
	//X軸+回転
	if (CKey::Push('K')) {
		Matrix = Matrix * CMatrix().RotateX(1);
	}
	//Y軸+回転
	if (CKey::Push('L')) {
		Matrix = Matrix * CMatrix().RotateY(1);
	}
	//X軸+回転
	if (CKey::Push('I')) {
		Matrix = Matrix * CMatrix().RotateX(-1);
	}
	//Y軸+回転
	if (CKey::Push('J')) {
		Matrix = Matrix * CMatrix().RotateY(-1);
	}
	

	//行列設定
	glMultMatrixf(Matrix.mF);
	
	

   //頂点にアニメーションを適用する
	//CRes::sModelX.AnimateVertex();
	//モデル描画
	//CRes::sModelX.Render();
	mPlayer.Render();
	//2D描画開始
	CUtil::Start2D(0, 800, 0, 600);

	mFont.DrawString("3D PROGRAMMING", 20, 20, 10, 12);

	//2Dの描画終了
	CUtil::End2D();

	return;
}

