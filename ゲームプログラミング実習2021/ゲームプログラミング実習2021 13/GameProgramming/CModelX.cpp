#include"glut.h"
#include <stdio.h>
#include"CModelX.h"
#include<string.h>//������֐��̃C���N���[�h
#include"CMaterial.h"


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
		//�P�ꂪAnimetionSet�̏ꍇ
		else if (strcmp(mToken, "AnimationSet") == 0) {
			new CAnimationSet(this);
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
/*
Render
���b�V���̖ʐ����O�ȊO�Ȃ�`�悷��
*/
void CModelXFrame::Render() {
	if (mMesh.mFaceNum != 0) {
		mMesh.Render();
	}
}
/*FineFame
�t���[�����̊Y������t���[���̃A�h���X��Ԃ�
*/
CModelXFrame* CModelX::FindFrame(char* name) {
	//�C�e���[�^�̍쐬
	std::vector<CModelXFrame*>::iterator itr;
	//�擪����Ō�ڂŌJ��Ԃ�
	for (itr = mFrame.begin(); itr!= mFrame.end(); itr++) {
		//���O����v�������ǂ���
		if (strcmp(name, (*itr)->mpName) == 0) {
			//��v�����炻�̃A�h���X��Ԃ�
			return *itr;
		}
	}
	//��v����t���[�����Ȃ��ꍇ��NULL��Ԃ�
		return NULL;
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
void CModelX::Render() {
	for (int i = 0; i < mFrame.size(); i++) {
		mFrame[i]->Render();
	}
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
		mpVertexIndex[i + 1] = model->GetIntToken();
		mpVertexIndex[i + 2] = model->GetIntToken();
	}
	//�������Ȃ��Ȃ�����I���
	while (model->mpPointer != '\0') {

		model->GetToken();//Meshormals
		//}�������̏ꍇ�͏I��
		if (strchr(model->mToken, '}'))
			break;
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
				mpNormal[i + 1] = pNormal[ni];
				ni = model->GetIntToken();
				mpNormal[i + 2] = pNormal[ni];
			}
			delete[] pNormal;
			model->GetToken();//}
		}
		else if (strcmp(model->mToken, "MeshMaterialList") == 0) {
			model->GetToken();//{
			//Material�̐�
			mMaterialNum = model->GetIntToken();
			//FaceNum
			mMaterialIndexNum = model->GetIntToken();
			//�}�e���A���C���f�b�N�X�̍쐬
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i < mMaterialIndexNum; i++) {
				mpMaterialIndex[i] = model->GetIntToken();

			}
			//�}�e���A���f�[�^�̍쐬
			for (int i = 0; i < mMaterialNum; i++) {
				model->GetToken(); //Material
				if (strcmp(model->mToken, "Material") == 0) {
					mMaterial.push_back(new CMaterial(model));

				}
				model->GetToken();//}//End of MeshMaterialList
			}
		}
		else if (strcmp(model->mToken, "SkinWeights") == 0) {
			//CSkinWeights�N���X�̃C���X�^���X���쐬���z��ɒǉ�
			mSkinWeights.push_back(new CSkinWeights(model));
		}
		else {
			//�ȊO�̃m�[�h�͓ǂݔ�΂�
			model->SkipNode();
		}
	}
#ifdef  _DEBUG

	//int�^�̕ϐ��̏o�͂́���
	printf("VertexNum:%d\n", mVertexNum);
	for (int i = 0; i < mVertexNum; i++) {
		//10f=�P�O����float�^
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
/*
Render
��ʂɕ`�悷��*/
void CMesh::Render() {
	/*���_�f�[�^�A�@���f�[�^�̔z���L���ɂ���*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	/*���_�f�[�^�A�@���f�[�^�̏ꏊ���w�肷��*/
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);

	for (int i = 0; i < mFaceNum; i++) {
		//�}�e���A����K�p����
		mMaterial[mpMaterialIndex[i]]->Enabled();
		/*���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��*/
		glDrawElements(GL_TRIANGLES, 3 , GL_UNSIGNED_INT, mpVertexIndex+i*3);
	}
	/*���_�f�[�^�A�@���f�[�^�̔z��𖳌��ɂ���*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	/*���_�̃C���f�b�N�X���ꏊ���w�肵�Đ}�`��`�悷��
	*/
	
}
/*
CSkinWeights
�X�L���E�F�C�g�̓ǂݍ���
*/
CSkinWeights::CSkinWeights(CModelX* model) 
	: mpFrameName(0)
	,mFrameIndex(0)
	,mIndexNum(0)
    ,mpIndex(nullptr)
    ,mpWeight(nullptr)
{
	model->GetToken();  //{
	model->GetToken(); //FrameName
	//�t���[�����G���A�m�ہA�ݒ�
	mpFrameName = new char[strlen(model->mToken) + 1];
	strcpy(mpFrameName, model->mToken);
	//���_�ԍ����擾
	mIndexNum = model->GetIntToken();
	//���_�ԍ������O�𒴂���
	if (mIndexNum > 0) {
		//���_�ԍ��ƒ��_�E�F�C�g�̃G���A�m��
		mpIndex = new int[mIndexNum];
		mpWeight = new float[mIndexNum];
		//���_�ԍ��擾
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = model->GetIntToken();
		//���_�E�F�C�g�擾
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = model->GetFloatToken();

	}
	//�I�t�Z�b�g�s��擾
	for (int i = 0; i < 16; i++) {
		mOffset.mF[i] = model->GetFloatToken();
	}
	model->GetToken();//}
   #ifdef _DEBUG
	
	printf("SkinWeights %s\n",mpFrameName);
	for (int i = 0; i < mIndexNum; i++) {
		printf("%10d\t", mpIndex[i]);
		printf("%10f\n", mpWeight[i]);
	}
	for (int i = 0; i < 13; i+=4) {
		printf("%10f\t", mOffset.mF[i]);
		printf("%10f\t", mOffset.mF[i+1]);
		printf("%10f\t", mOffset.mF[i+2]);
		printf("%10f\n", mOffset.mF[i+3]);
	}
   #endif
}
/*
CAnimationSet
*/
CAnimationSet::CAnimationSet(CModelX* model)
	:mpName(nullptr)
{
	model->mAnimationSet.push_back(this);
	model->GetToken();//Animation Name
	//�A�j���[�V�����Z�b�g����ޔ�
	mpName = new char[strlen(model->mToken) + 1];
	strcpy(mpName, model->mToken);
	model->GetToken(); //{
	while (*model->mpPointer != '\0') {
		model->GetToken();//}orAnimetion
		if (strchr(model->mToken, '}'))break;
		if (strcmp(model->mToken, "Animation") == 0) {
			//Animation�v�f�ǂݍ���
			mAnimation.push_back(new CAnimation(model));
			
			
		}
	}
#ifdef _DEBUG
	printf("AnimationSet:%s\n", mpName);
#endif
}
CAnimation::CAnimation(CModelX* model)
	:mpFrameName(0)
	, mFrameIndex(0)
	, mKeyNum(0)
	, mpKey(nullptr)


{
	model->GetToken();//{or Animation Name
	if (strchr(model->mToken, '{')) {
		model->GetToken();//{
	}
	else {
		model->GetToken();//{
		model->GetToken();//{
	}
	model->GetToken();//FrameName
	mpFrameName = new char[strlen(model->mToken) + 1];
	strcpy(mpFrameName, model->mToken);
	mFrameIndex = model->FindFrame(model->mToken)->mIndex;
	model->GetToken();//}
	//�L�[�̔z���ۑ����Ă����z��
	CMatrix* key[4] = { 0,0,0,0 };
	//���Ԃ̔z���ۑ����Ă����z��
	float* time[4] = { 0,0,0,0 };
	while (*model->mpPointer != '\0') {
		model->GetToken();//}or AnimetionKey
		if (strchr(model->mToken, '}'))break;
		if (strcmp(model->mToken, "AnimationKey") == 0) {

			model->GetToken();//{
			//�f�[�^�̃^�C�v�擾
			int type = model->GetIntToken();
			//���Ԑ��擾

			mKeyNum = model->GetIntToken();
			switch (type) {
			case 0://Rotation Quaternion
				//�s��̔z������Ԑ����m��
				key[type] = new CMatrix[mKeyNum];
				//���Ԃ̔z������Ԑ����m��
				time[type] = new float[mKeyNum];
				//���Ԑ����J��Ԃ�
				for (int i = 0; i < mKeyNum; i++) {
					//���Ԏ擾
					time[type][i] = model->GetFloatToken();
					model->GetToken();//4��ǂݔ�΂�
					//w,x,y,z���擾
					float w = model->GetFloatToken();
					float x = model->GetFloatToken();
					float y = model->GetFloatToken();
					float z = model->GetFloatToken();
					//�N�H�[�^�j�I�������]�s��ɕϊ�
					key[type][i].SetQuaternion(x, y, z, w);

				
				}
				break;
			case 1://�g��k���̍s��쐬
				key[type] = new CMatrix[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++) {
					time[type][i] = model->GetFloatToken();
					model->GetToken();//3
					float x = model->GetFloatToken();
					float y = model->GetFloatToken();
					float z = model->GetFloatToken();
					key[type][i].mM[0][0] = x;
					key[type][i].mM[1][1] = y;
					key[type][i].mM[2][2] = z;

				}
				break;
			case 2://�ړ��̍s��쐬
				key[type] = new CMatrix[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++) {
					time[type][i] = model->GetFloatToken();
					model->GetToken();//3
					float x = model->GetFloatToken();
					float y = model->GetFloatToken();
					float z = model->GetFloatToken();
					key[type][i].Translate(x, y, z);

				}
				break;
			case 4://�s��f�[�^���擾
				mpKey = new CAnimationKey[mKeyNum];
				for (int i = 0; i < mKeyNum; i++) {
					mpKey[i].mTime = model->GetFloatToken();//Time
					model->GetToken();//16
					for (int j = 0; j < 16; j++) {
						mpKey[i].mMatrix.mF[j] = model->GetFloatToken();
					}
				}
				break;
			}
			model->GetToken();//}
		}
		else {
			model->SkipNode();
		}
	}//while�̏I���
	//�s��f�[�^�ł͂Ȃ��Ƃ�
	if (mpKey == 0) {
		//���Ԑ����L�[���쐬
		mpKey = new CAnimationKey[mKeyNum];
		for (int i = 0; i < mKeyNum; i++) {
			//���Ԑݒ�
			mpKey[i].mTime = time[2][i];//Time
			//�s��쐬Size*Rotation*Position
			mpKey[i].mMatrix = key[1][i] * key[0][i] * key[2][i];
           
		}
	}
	//�m�ۂ����G���A�J��
	for (int i = 0; i < ARRAY_SIZE(key); i++) {
		SAFE_DELETE_ARRAY(time[i]);
		SAFE_DELETE_ARRAY(key[i]);
		
		
	}


				
	
#ifdef _DEBUG
	printf("Animation:%s\n", mpFrameName);//Frame�o��
	mpKey[0].mMatrix.Print();
	
#endif
}