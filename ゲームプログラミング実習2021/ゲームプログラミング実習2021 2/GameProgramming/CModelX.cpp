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
			printf("%s", mToken);//Frame�o��
			GetToken();//Frame�����擾
			printf("%s\n", mToken);//Frame�����o��
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
	//*���|�C���^�ϐ�
	char* p = mpPointer;
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
			*q++ = *p++;
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