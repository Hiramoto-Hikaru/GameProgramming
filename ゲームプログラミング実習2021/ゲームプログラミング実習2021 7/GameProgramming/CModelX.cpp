#include"glut.h"
#include <stdio.h>
#include"CModelX.h"
#include<string.h>//������֐��̃C���N���[�h



void CModelX::Load(char* file) {
	//�t�@�C���T�C�Y���擾����
	FILE* fp;//�t�@�C���|�C���^�ϐ��̍쐬
	fp = fopen(file, "rb");//�t�@�C���̃I�[�v��
	if (fp == NULL) {
		printf("fopen error:%s\n", file);
		return;
	}
	//�t�@�C���̍Ō�ֈړ�
	fseek(fp, 0L, SEEK_END);
	//�t�@�C���T�C�Y�̎擾
	int size = ftell(fp);
	//�t�@�C���T�C�Y+�P�o�C�g���̗̈���m��
	char* buf = mpPointer = new char[size + 1];
	//�t�@�C������RD�̃f�[�^��ǂݍ���
	//�t�@�C���̐擪�ֈړ�
	fseek(fp, 0L, SEEK_SET);
	//�m�ۂ����̈�Ƀt�@�C���T�C�Y���f�[�^��ǂݍ���
	fread(buf, size, 1, fp);
	//�Ō��\�O��ݒ肷��
	buf[size]='\0';


	 //������̍Ō�܂ŌJ��Ԃ�
	while (*mpPointer != '\0') {
		GetToken();//�P��̎擾
		//�P�ꂪflame�̏ꍇ
		if (strcmp(mToken, "Frame") == 0) {
			//�t���[�������쐬����
			new CModelXFrame(this);
		}
		//�P�ꂪAnimationSet�̏ꍇ
		if (strcmp(mToken, "AnimationSet") == 0) {
			printf("%s", mToken);//Frame�o��
			GetToken();//Frame�����擾
			printf("%s\n", mToken);//Frame�����o��
		}
	}                    
    fclose(fp);//�t�@�C���̃N���[�Y
	SAFE_DELETE_ARRAY(buf);
}
/*
GetToken
������f�[�^����P�����擾����*/
void CModelX::GetToken() {
	//*���|�C���^�ϐ�(�P�o�C�g�j
	char* p =mpPointer;
	char* q = mToken;
	//�󔒁i�j�^�u�i\r�j���s�i\���j�i\n�j�ȊO�̕����ɂȂ�܂œǂݔ�΂�
	/*
	strchr(������A����)
	������ɕ������܂܂�Ă���΁A�������������ւ̃|�C���^��������
	������Ȃ�������NULL��Ԃ�
	*/
	// ||,,,or(��������)

	while (*p != '\0' && strchr("\t \r\n,;\"", *p))p++;
	if (*p == '{'|| * p == '}') {
		//{�܂���}�Ȃ�mToken�ɑ�����A���̕�����
		*q++ = *p++;
	}
	else {
		//�󔒁A�^�u�A���s�A�p�̕����ɂȂ�܂ŁAmToken�ɑ������
		while (*p != '\0' && !strchr("\t \r\n,;\"}", *p))
			*q++ = *p++;//1�o�C�g�ǉ�
	}
	*q = '\0';//mToken�̍Ō�Ɂ��O����
	mpPointer = p;//���̓ǂݍ��ރ|�C���g���X�V����
	//����mToken��//�̏ꍇ�̓R�����g�Ȃ̂ŉ��s�܂œǂݔ�΂�
	/*
	strcmp(������P�A������2�j
	������P�ƕ�����Q���������ꍇ�O��Ԃ��܂�
	������P�ƕ�����Q���������Ȃ��ꍇ�O�ȊO��Ԃ��܂�
	*/
	if (!strcmp("//", mToken)) {
		//���s�܂œǂݔ�΂�
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//�ǂݍ��݈ʒu���X�V
		mpPointer = p;
		//�P����擾����
		GetToken();
	}
}
/*SkipNode
�m�[�h��ǂݔ�΂�
*/
void CModelX::SkipNode() {
	//�������I�������I��
	while (*mpPointer != '\0') {
		GetToken();//���̒P��擾
		//{�����������烋�[�v�I��
		if (strchr(mToken, '{'))break;
	}
	int count= 1;
	//�������I��邩�A�J�E���g���O�ɂȂ�����I��
	while (*mpPointer!='\0'&&count>0)
		
	{
		GetToken();//���̒P��擾
		//{��������ƃJ�E���g�A�b�v
		if (strchr(mToken, '{'))count++;
		//}��������ƃJ�E���g�_�E��
		else if (strchr(mToken, '}'))count--;
	}
}
/*CModelXFrame
model:CModelX�C���X�^���X�ւ̃|�C���^
�t���[�����쐬����
�Ǎ�����Frame��������΃t���[�����쐬��
�q�t���[���ɒǉ�����*/
CModelXFrame::CModelXFrame(CModelX* model) {
	//���݂̃t���[���z��̗v�f�����擾���ݒ肷��
	mIndex = model->mFrame. size();
	//CModelX�̃t���[���s��ɒǉ�����
	model->mFrame.push_back(this);
	//�ϊ��s���P�ʐ���ɂ���
	mTransformMatrix.Identity();
	//���̒P��i�t���[�����̗\��j���擾����
	model->GetToken();//framename
	//�t���[�������G���A���m�ۂ���
	mpName = new char[strlen(model->mToken) + 1];
	//�t���[�������R�s�[����
	strcpy(mpName,model->mToken);
	//���̒P��({�̗\��)���擾�G
	model->GetToken();//{
	//�������Ȃ��Ȃ�����I���
	while (*model->mpPointer != '\0') {
		//���̒P��擾
		model->GetToken();//Frame
		//}�̏ꍇ�͏I��
		if (strchr(model->mToken, '}'))break;
		//�V���ȃt���[���̏ꍇ�͎q�t���[���ɒǉ�
		if (strcmp(model->mToken, "Frame") == 0) {
			//�t���[�����쐬���q�t���[���̔z��ɒǉ�
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
			//��L�ȊO�̗v�f�͓ǂݔ�΂�
			model->SkipNode();
		}
	}
	//�f�o�b�O�o�[�W�����̂ݗL��
	#ifdef _DEBUG
		printf("%s\n", mpName);
     mTransformMatrix.Print();
    #endif
}

/*GetfloatToken
�P��𕂓������_�^�̃f�[�^�ŕԂ�
*/
float CModelX::GetFloatToken() {
	GetToken();
	//atof
	//�������float�^�֕ϊ�
	return atof(mToken);
}
/*
GetIntToken
�P��𐮐��^�̃f�[�^�ŕԂ�
*/
int CModelX::GetIntToken() {
	GetToken();
	return atoi(mToken);//AtoI�F�������int�^�ɕϊ�
}
/*
Init
Mesh�̃f�[�^����荞��
*/
void CMesh::Init(CModelX* model) {
	model->GetToken();  //{or���O
	if (!strchr(model->mToken, '{')) {
		//���O�̏ꍇ�����o
		model->GetToken();  //{
	}
	//���_���̎擾
	mVertexNum = model->GetIntToken();
	//���_�����G���A�m��
	mpVertex = new CVector[mVertexNum];//mVertexNum�͂W�S����
	//���_�����f�[�^����荞�� 
	for (int i = 0; i < mVertexNum; i++) {
		mpVertex[i].mX = model->GetFloatToken();
		mpVertex[i].mY = model->GetFloatToken();
		mpVertex[i].mZ = model->GetFloatToken();
	}
	mFaceNum = model->GetIntToken();//�ʐ��ǂݍ���
	//���_���͂P�ʂɂR���_
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3) {
		model->GetToken();//���_�ǂݔ�΂�
		mpVertexIndex[i] = model->GetIntToken();
		mpVertexIndex[i+1] = model->GetIntToken();
		mpVertexIndex[i+2] = model->GetIntToken();
	}
	model->GetToken();//Meshormals
	if (strcmp(model->mToken, "MeshNormals") == 0) {
		model->GetToken();//{
        //�@���f�[�^���擾
		mNormalNum = model->GetIntToken();
		//�@���̃f�[�^��z��Ɏ�荞��	
		CVector* pNormal = new CVector[mNormalNum];
		for (int i = 0; i < mNormalNum; i++) {
			pNormal[i].mX = model->GetFloatToken();
			pNormal[i].mY = model->GetFloatToken();
			pNormal[i].mZ = model->GetFloatToken();
		}
		//�@�������ʐ�*�R
		mNormalNum = model->GetIntToken() * 3;//FaceNum
		int ni;
		//���_���Ƃɖ@���f�[�^��ݒ肷��
		mpNormal = new CVector[mNormalNum];
		for (int i = 0; i < mNormalNum; i += 3) {
			model->GetToken();//3
			ni = model->GetIntToken();
			mpNormal[i] = pNormal[ni];
			ni = model->GetIntToken();
			mpNormal[i+1] = pNormal[ni];
			ni = model->GetIntToken();
			mpNormal[i+2] = pNormal[ni];
		}
		delete[] pNormal;
		model->GetToken();//}
	}
		
#ifdef  _DEBUG

	//int�^�̕ϐ��̏o�͂́���
	printf("VertexNum:%d\n", mVertexNum);
	for (int i = 0; i < mVertexNum; i++) {
		printf("%10f", mpVertex[i].mX);
		printf("%10f", mpVertex[i].mY);
		printf("%10f\n", mpVertex[i].mZ);
	}

	printf("FaceNum:%d\n", mFaceNum );
	for (int i = 0; i < mFaceNum * 3; i += 3) {

		printf("%d\t", mpVertexIndex[i]);
		printf("%d\t", mpVertexIndex[i+1]);
		printf("%d\n", mpVertexIndex[i+2]);

	}
	//�@���f�[�^�𒸓_�̐������o��
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


	#endif
	
}
