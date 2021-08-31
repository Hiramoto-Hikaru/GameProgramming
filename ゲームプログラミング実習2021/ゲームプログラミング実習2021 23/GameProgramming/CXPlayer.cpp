#include"CXPlayer.h"
#include"CKey.h"
#include"CXCharacter.h"
CXPlayer::CXPlayer()
	

{

}
void CXPlayer::Update() {
	if (CKey::Push('W')) {
		ChangeAnimation(1, true, 60);
		mPosition =mPosition+ CVector(0.0f,0.0f,0.1f)*mMatrixRotate;
		
	}
	else if (CKey::Push('A')) {
		mRotation.mY += 2.0f;
	}
	else if (CKey::Push('D')) {
		mRotation.mY -= 2.0f;
	}
	else if (CKey::Push(VK_SPACE)) {
	

		ChangeAnimation(3, false, 30);
		 if (mAnimationFrame >=30) {
			ChangeAnimation(4, false, 30);
		 }
	}
	else {
		ChangeAnimation(0, true, 60);
	}
	
	
	CXCharacter::Update();
}