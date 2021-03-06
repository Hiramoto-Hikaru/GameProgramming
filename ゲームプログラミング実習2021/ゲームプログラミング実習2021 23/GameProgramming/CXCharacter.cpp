#include"CXCharacter.h"
#include"CKey.h"
/*Init
初期化処理
*/
void CXCharacter::Init(CModelX* model) {
	mpModel = model;
	//最初のアニメーションにする
	mAnimationIndex = 0;
	//繰り返し再生する
	mAnimationLoopFlg = true;
	//1アニメーション目の最大フレーム数
	mAnimationFrameSize = model->mAnimationSet[0]->mMaxTime;
	//アニメーションのフレームを最初にする
	mAnimationFrame = 0.0f;
	mpModel->mAnimationSet[mAnimationIndex]->mTime = mAnimationFrame;
	//アニメーションの重みを１．０にする
	mpModel->mAnimationSet[mAnimationIndex]->mWeight = 1.0f;
	//合成行列退避エリアの確保
	mpCombinedMatrix = new CMatrix[model->mFrame.size()];


}
CXCharacter::CXCharacter() {
	mScale = CVector(1.0f, 1.0f, 1.0f);
	mpCombinedMatrix=nullptr;
}
/*
ChangeAnimation
アニメーションを切り替える
index アニメーションの番号
loop:true 繰り返す
framesize最後まで再生されるフレーム数
*/
void CXCharacter::ChangeAnimation(int index, bool loop, float framesize) {
	
	//同じ場合は切り替えない
	if (mAnimationIndex == index)return;
	//今のアニメ地温の重みを０．０にする
	mpModel->mAnimationSet[mAnimationIndex]->mWeight = 0.0f;
	//番号、繰り返し、フレーム数を設定
	mAnimationIndex = index % mpModel->mAnimationSet.size();
	mAnimationLoopFlg = loop;
	mAnimationFrameSize = framesize;
	//アニメーションのフレームを無効にする
	mAnimationFrame = 0.0f;
	mpModel->mAnimationSet[mAnimationIndex]->mWeight = 1.0f;

}
/*
Update
更新する
matrix:移動、回転、拡大縮小の行列
*/
void CXCharacter::Update(CMatrix& matrix) {
	



	//最後まで再生していない
	if (mAnimationFrame < mAnimationFrameSize) {
		//アニメーションの時間を計算
		mpModel->mAnimationSet[mAnimationIndex]->mTime =
			mpModel->mAnimationSet[mAnimationIndex]->mMaxTime * mAnimationFrame / mAnimationFrameSize;
		//フレームを進める
		mAnimationFrame++;
	}
	else {
		//繰り返しの場合はフレームを０にする
		if (mAnimationLoopFlg) {
			//アニメーションのフレームを最初にする
			mAnimationFrame = 0.0f;
			mpModel->mAnimationSet[mAnimationIndex]->mTime = mAnimationFrame;

		}
		else {
			mpModel->mAnimationSet[mAnimationIndex]->mTime = mpModel->mAnimationSet[mAnimationIndex]->mMaxTime;
		}
	}
	//フレームの変換行列をアニメーションで更新する
	mpModel->AnimateFrame();
	//フレームの合成行列を計算する
    mpModel->mFrame[0]->AnimateCombined(&matrix);
	//合成行列の退避
	for (int i= 0; i < mpModel->mFrame.size(); i++) {
		mpCombinedMatrix[i] =
			mpModel->mFrame[i]->mCombinedMatrix;
	}
	//頂点にアニメーションを適用する
	//削除 mpModel->AnimateVertex();
}
//更新処理
void CXCharacter::Update() {
	mpModel->mAnimationSet[mAnimationIndex]->mWeight = 0.0f;
	mpModel->mAnimationSet[0]->mWeight = 1.0f;
	if (CKey::Push('W')) {
		
           mpModel->mAnimationSet[1]->mWeight = 1.0f;
	}
	
	//mpModel->mAnimationSet[2]->mWeight = 1.0f;
	//変換行列の更新
	CTransform::Update();
	//アニメーションを更新する
	Update(mMatrix);
}
/*
描画する
*/
void CXCharacter::Render() {
	//頂点にアニメーションを適用する
	mpModel->AnimateVertex(mpCombinedMatrix);
	mpModel->Render();
}