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
	else if (mAnimationIndex == 1){
		ChangeAnimation(0, true, 60);
	}
	  
	 if (CKey::Push('A')) {
		mRotation.mY += 2.0f;
	}
     if (CKey::Push('D')) {
		mRotation.mY -= 2.0f;
	}
	 if (CKey::Once(VK_SPACE)) {
		 if (mAnimationIndex == 0) {
         ChangeAnimation(3, false, 30);
		 }
			
	}
	if (mAnimationIndex == 3) {
       if (mAnimationFrame >= 29) {
	        ChangeAnimation(4, false, 30);
	   }
	}
	
       if (mAnimationFrame >= mAnimationFrameSize) {
	
         ChangeAnimation(0, true, 60);
	   }
	
    
		
	
	
	
	
	
	CXCharacter::Update();
}