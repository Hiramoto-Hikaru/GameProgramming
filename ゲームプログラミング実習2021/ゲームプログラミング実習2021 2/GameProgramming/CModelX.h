#ifndef CMODELX_H
#define CMODELX_H

#define MODEL_FILE "sample.blend.x"//入力ファイル名
//領域開放をマクロ化
#define SAFE_DELETE_ARRAY(a) {if(a)delete[]a;a=0;}
/*CModelX
Xファイルの３Dモデルデータをプログラムで認識する*/
class CModelX {
public:
	char* mpPointer;//読み込み位置
	char mToken[1024];//取り出した単語の領域
	CModelX() 
		:mpPointer(0)
	{}
	//ファイル読み込み
	void Load(char* file);
	void GetToken();
};
#endif 