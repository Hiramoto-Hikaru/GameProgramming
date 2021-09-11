#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include"CModelX.h"
#include"CMatrix.h"
#include"CCharacter.h"
class CXCharacter:public CCharacter {
public:
	CModelX* mpModel;//���f���f�[�^
	CMatrix* mpCombinedMatrix;//�����s��Δ�
	int mAnimationIndex;//�A�j���[�V�����ԍ�
	bool mAnimationLoopFlg;//true�F�A�j���[�V�������J��Ԃ�
	float mAnimationFrame;//�A�j���[�V�����̍Đ��t���[��
	float mAnimationFrameSize;//�A�j���[�V�����̍Đ��t���[��
	//����������
	void Init(CModelX* model);
	//�A�j���[�V�����̕ύX
	void ChangeAnimation(int index, bool llo, float framesize);
	//�X�V����
	void Update(CMatrix& m);
	CXCharacter();
	
		virtual~CXCharacter() {
			SAFE_DELETE_ARRAY(mpCombinedMatrix);
		}
	
	//�`�揈��
	void Render();
	void Update();
};

#endif
