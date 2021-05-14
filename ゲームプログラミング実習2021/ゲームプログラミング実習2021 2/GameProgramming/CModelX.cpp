#include"glut.h"
#include <stdio.h>
#include"CModelX.h"
#include<string.h>//文字列関数のインクルード

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


	 //文字列の最後まで繰り返し
	while (*mpPointer != '\0') {
		GetToken();//単語の取得
		//単語がflameの場合
		if (strcmp(mToken, "Frame") == 0) {
			printf("%s", mToken);//Frame出力
			GetToken();//Frame名を取得
			printf("%s\n", mToken);//Frame名を出力
		}
		//単語がAnimationSetの場合
		if (strcmp(mToken, "AnimationSet") == 0) {
			printf("%s", mToken);//Frame出力
			GetToken();//Frame名を取得
			printf("%s\n", mToken);//Frame名を出力
		}
	}                    
	

    fclose(fp);//ファイルのクローズ
	SAFE_DELETE_ARRAY(buf);
}
/*
GetToken
文字列データから単語を一つ取得する*/
void CModelX::GetToken() {
	//*＝ポインタ変数
	char* p = mpPointer;
	char* q = mToken;
	//空白（）タブ（\r）改行（\ｒ）（\n）以外の文字になるまで読み飛ばす
	/*
	strchr(文字列、文字)
	文字列に文字が含まれていれば、見つかった文字へのポインタをかえす
	見つからなかったらNULLを返す
	*/
	// ||,,,or(もしくは)

	while (*p != '\0' && strchr("\t \r\n,;\"", *p))p++;
	if (*p == '{'|| * p == '}') {
		//{または}ならmTokenに代入し、次の文字へ
		*q++ = *p++;
	}
	else {
		//空白、タブ、改行、｝の文字になるまで、mTokenに代入する
		while (*p != '\0' && !strchr("\t \r\n,;\"}", *p))
			*q++ = *p++;
	}
	*q = '\0';//mTokenの最後に￥０を代入
	mpPointer = p;//次の読み込むポイントを更新する
	//もしmTokenが//の場合はコメントなので改行まで読み飛ばす
	/*
	strcmp(文字列１、文字列2）
	文字列１と文字列２が等しい場合０を返します
	文字列１と文字列２が等しくない場合０以外を返します
	*/
	if (!strcmp("//", mToken)) {
		//改行まで読み飛ばす
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//読み込み位置を更新
		mpPointer = p;
		//単語を取得する
		GetToken();
	}
}