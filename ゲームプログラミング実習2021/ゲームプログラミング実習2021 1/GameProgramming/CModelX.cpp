#include"glut.h"
#include <stdio.h>
#include"CModelX.h"


void CModelX::Load(char* file) {
	//ファイルサイズを取得する
	FILE* fp;//ファイルポインタ変数の作成
	fp = fopen(file, "rb");//ファイルのオープン
	
	if (fp == NULL) {
		printf("fopen error:%s\n", file);
		return;
	}
	//ファイルの最後へ移動
	fseek(fp, 0L, SEEK_END);
	//ファイルサイズの取得
	int size = ftell(fp);
	//ファイルサイズ+１バイト分の領域を確保
	char* buf = mpPointer = new char[size + 1];
	//ファイルから３Dのデータを読み込む
	//ファイルの先頭へ移動
	fseek(fp, 0L, SEEK_SET);
	//確保した領域にファイルサイズ分データを読み込む
	fread(buf, size, 1, fp);
	//最後に\０を設定する
	buf[size]='\0';
	fclose(fp);//ファイルのクローズ
	printf(buf);
}