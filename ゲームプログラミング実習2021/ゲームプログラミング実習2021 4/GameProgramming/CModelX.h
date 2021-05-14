#ifndef CMODELX_H
#define CMODELX_H
#include<vector>
#include"CMatrix.h"

                      //sizeof�F�T�C�Y�i�o�C�g���j��Ԃ�
#define ARRAY_SIZE(a)(sizeof(a)/sizeof(a[0]))
#define MODEL_FILE "sample.blend.x"//���̓t�@�C����
//�̈�J�����}�N����
#define SAFE_DELETE_ARRAY(a) {if(a)delete[]a;a=0;}

//��ɐ錾��������CModelXFlame�̏����ɓ��Ă͂܂点��
class CModelX;


class CModelXFrame {
public:

	
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

};

/*CModelX
X�t�@�C���̂RD���f���f�[�^���v���O�����ŔF������*/
class CModelX {
public:
	//���������_�f�[�^�̎擾
	float GetFloatToken();

	char* mpPointer;//�ǂݍ��݈ʒu
	char mToken[1024];//���o�����P��̗̈�0�`1023(1024��)
	std::vector<CModelXFrame*>mFrame;//�t���[���̔z��

	CModelX() 
		:mpPointer(0)
	{}
	~CModelX() {
		if (mFrame.size() > 0) {
			delete mFrame[0];
		}
	}
	//�t�@�C���ǂݍ���
	void Load(char* file);
	void GetToken();
	//�m�[�h�̓ǂݔ�΂�
	void SkipNode();
};
#endif 