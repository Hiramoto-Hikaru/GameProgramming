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

//�L�����N�^�̃C���X�^���X
CXPlayer mPlayer;
CMatrix Matrix;
//�G�̃C���X�^���X
CXEnemy mEnemy;
CSceneGame::~CSceneGame() {

}

void CSceneGame::Init() {

	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	mFont.LoadTexture("FontG.tga", 1, 4096 / 64);
	//�RD���f���̓ǂݍ���
	CRes::sModelX.Load(MODEL_FILE);
	//�L�����N�^�[�Ƀ��f����ݒ�
	mPlayer.Init(&CRes::sModelX);
	//�G�Ƀ��f����ݒ�
	mEnemy.Init(&CRes::sModelX);
	//�G�̔z�u
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
	//�L�����N�^�[�N���X�̍X�V
	mPlayer.Update();
	//�G�̍X�V
	mEnemy.Update();
	mEnemy.Render();

	//�ŏ��̃A�j���[�V�����̌��ݎ��Ԃ��S�T�ɂ���

	//CRes::sModelX.mAnimationSet[0]->mTime=0;
	//CRes::sModelX.mAnimationSet[0]->mTime += 1.0f;
	//CRes::sModelX.mAnimationSet[0]->mTime = (int)CRes::sModelX.mAnimationSet[0]->mTime %
		//(int)(CRes::sModelX.mAnimationSet[0]->mMaxTime + 1);
	//�ŏ��̃A�j���[�V�����̏d�݂��P�D�O�i�P�O�O���j�ɂ���
	//CRes::sModelX.mAnimationSet[0]->mWeight = 1.0f;
	//�t���[���̕ϊ��s����A�j���[�V�����ōX�V����
	//CRes::sModelX.AnimateFrame();
	//�t���[���̍����s����v�Z����
	//CRes::sModelX.mFrame[0]->AnimateCombined(&Matrix);
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;//���_�A�����_�A�����
	//���_�����߂�
	e = CVector(1.0f, 2.0f, 10.0f);
	//�����_�����߂�
	c = CVector();
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f);

	//�J�����N���X�̐ݒ�
	Camera.Set(e, c, u);
	Camera.Render();
	//X��+��]
	if (CKey::Push('K')) {
		Matrix = Matrix * CMatrix().RotateX(1);
	}
	//Y��+��]
	if (CKey::Push('L')) {
		Matrix = Matrix * CMatrix().RotateY(1);
	}
	//X��+��]
	if (CKey::Push('I')) {
		Matrix = Matrix * CMatrix().RotateX(-1);
	}
	//Y��+��]
	if (CKey::Push('J')) {
		Matrix = Matrix * CMatrix().RotateY(-1);
	}
	

	//�s��ݒ�
	glMultMatrixf(Matrix.mF);
	
	

   //���_�ɃA�j���[�V������K�p����
	//CRes::sModelX.AnimateVertex();
	//���f���`��
	//CRes::sModelX.Render();
	mPlayer.Render();
	//2D�`��J�n
	CUtil::Start2D(0, 800, 0, 600);

	mFont.DrawString("3D PROGRAMMING", 20, 20, 10, 12);

	//2D�̕`��I��
	CUtil::End2D();

	return;
}

