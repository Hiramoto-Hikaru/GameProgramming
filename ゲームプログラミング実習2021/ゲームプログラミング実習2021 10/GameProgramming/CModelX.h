#ifndef CMODELX_H
#define CMODELX_H
#include<vector>
#include"CMatrix.h"
#include"CVector.h"
//sizeof�F�T�C�Y�i�o�C�g���j��Ԃ�
#define ARRAY_SIZE(a)(sizeof(a)/sizeof(a[0]))
#define MODEL_FILE "sample.blend.x"//���̓t�@�C����
//�̈�J�����}�N����
#define SAFE_DELETE_ARRAY(a) {if(a)delete[]a;a=0;}

//��ɐ錾��������CModelXFlame�̏����ɓ��Ă͂܂点��
class CModelX;
class CMaterial;
class CSkinWeights;
class CAnimationSet;
//CMesh�N���X�̒�`
class CMesh {
public:

	int mMaterialNum;//�}�e���A����
	int mMaterialIndexNum;//�}�e���A���ԍ���(�ʐ�)
	int* mpMaterialIndex;//�}�e���A���ԍ�
	std::vector<CMaterial*>mMaterial;//�}�e���A���f�[�^
	//�A�j���[�V�����Z�b�g�̔z��
	std::vector<CAnimationSet*>mAnimationSet;
	int mVertexNum;
	CVector* mpVertex;
	int mFaceNum;//�ʐ�
	int* mpVertexIndex;//�ʂ��\�����钸�_�ԍ�
	int mNormalNum;//�@����
	CVector* mpNormal;//�@���f�[�^
	//�X�L���E�F�C�g
	std::vector<CSkinWeights*>mSkinWeights;
	//�R���X�g���N�^
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
	//�f�X�g���N�^
	~CMesh() {
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
		//�X�L���E�F�C�g�̍폜
		for (int i=0; i < mSkinWeights.size(); i++) {
			delete mSkinWeights[i];
		}
		for (int i = 0; i < mAnimationSet.size(); i++) {
			delete mAnimationSet[i];
		}
	}
	//�ǂݍ��ݏ���
	void Init(CModelX* model);
	void Render();
};
class CModelXFrame {
public:

	CMesh mMesh;//Mesh�f�[�^
	//std::vector �ϒ��z��i�ォ�琔�𑝂₹��z��j
	std::vector<CModelXFrame*>mChild;//�q�t���[���̔z��
	CMatrix mTransformMatrix;//�ϊ��s��
	char* mpName;//�t���[�����O
	int mIndex;//�t���[���ԍ�
	//�R���X�g���N�^
	CModelXFrame(CModelX* model);
	//�f�X�g���N�^
	~CModelXFrame() {
		//�q�t���[�������ׂĊJ������
		//�C�e���[�^�[�i�|�C���^�̈���j���g�p
		std::vector<CModelXFrame*>::iterator itr;
		for (itr = mChild.begin(); itr != mChild.end(); itr++) {
			delete* itr;
		}
		//���O�̃G���A���J������
		SAFE_DELETE_ARRAY(mpName);
	}
	void Render();
};

/*CModelX
X�t�@�C���̂RD���f���f�[�^���v���O�����ŔF������*/
class CModelX {
public:
	//���������_�f�[�^�̎擾
	float GetFloatToken();
	//�����f�[�^�̎擾
	int GetIntToken();
	char* mpPointer;//�ǂݍ��݈ʒu
	char mToken[1024];//���o�����P��̗̈�0�`1023(1024��)
	std::vector<CModelXFrame*>mFrame;//�t���[���̔z��
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
	//�t�@�C���ǂݍ���
	void Load(char* file);
	void GetToken();
	//�m�[�h�̓ǂݔ�΂�
	void SkipNode();
	void Render();
};
/*CSkinWeights
�X�L���E�F�C�g�N���X
*/
class CSkinWeights {
public:
	char* mpFrameName;//�t���[����
	int mFrameIndex;//�t���[���ԍ�
	int mIndexNum;//���_�ԍ���
	int* mpIndex;//���_�ԍ��z��
	float* mpWeight;//�z�_�E�F�C�g�z��
	CMatrix mOffset;//�I�t�Z�b�g�}�g���b�N�X

	CSkinWeights(CModelX* model);

	~CSkinWeights() {
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpIndex);
		SAFE_DELETE_ARRAY(mpWeight);
	}

};
/*
CAnimationSet
�A�j���[�V�����Z�b�g
*/
class CAnimationSet {
public:
	//�A�j���[�V�����Z�b�g��
	char* mpName;
	CAnimationSet(CModelX* model);
	~CAnimationSet() {
		SAFE_DELETE_ARRAY(mpName);

	}
};
#endif 