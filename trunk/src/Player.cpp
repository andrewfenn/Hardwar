#include "Player.h"

/*
void Player::processControls(InputHandler *mInput, SceneNode *mCamNode) {
	mothControls(mInput, mCamNode);
}

// Mouse and Keyboard controls when the player is outside the moth
void Player::freeControls(InputHandler *mInput, SceneNode *mCamNode) {

} 

// Mouse and Keyboard controls when the player is in a moth
void Player::mothControls(InputHandler *mInput, SceneNode *mCamNode) {
	
	Vector3 transVector = Vector3::ZERO;
	
	

	if (mInput->isKeyDown(OIS::KC_UP) || mInput->isKeyDown(OIS::KC_W))
      transVector.z -= mMove;
   if (mInput->isKeyDown(OIS::KC_DOWN) || mInput->isKeyDown(OIS::KC_S))
      transVector.z += mMove;

   if (mInput->isKeyDown(OIS::KC_LEFT) || mInput->isKeyDown(OIS::KC_A))
      transVector.x -= mMove;
   if (mInput->isKeyDown(OIS::KC_RIGHT) || mInput->isKeyDown(OIS::KC_D))
      transVector.x += mMove;

   if (mInput->isKeyDown(OIS::KC_PGUP) || mInput->isKeyDown(OIS::KC_Q))
      transVector.y += mMove;
   if (mInput->isKeyDown(OIS::KC_PGDOWN) || mInput->isKeyDown(OIS::KC_E))
   	transVector.y -= mMove;

   //mCamNode->translate(transVector * mTimeSinceLastFrame, Node::TS_LOCAL);
} //*/
