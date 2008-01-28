#include "Main.h"

void Main::createCamera() {
	// create camera, but leave at default position
	mCamera = mSceneMgr->createCamera("PlayerCam"); 
   mCamera->setNearClipDistance(5);
}

void Main::chooseSceneManager() {
	mSceneMgr = mRoot->createSceneManager(ST_EXTERIOR_CLOSE);
}

void Main::createScene() {
		// setup sky and terrain
		ColourValue fadeColour(0.4, 0.4, 0.9);
        mWindow->getViewport(0)->setBackgroundColour(fadeColour);
        mSceneMgr->setFog(FOG_LINEAR, fadeColour, 0.0, 50, 7000);

        mSceneMgr->setWorldGeometry("hardwar/terrain.cfg");
        mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

		mSceneMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));

		// setup some light
		Light *light = mSceneMgr->createLight("Light1");
        light->setType(Light::LT_POINT);
        light->setPosition(Vector3(1000, 10000, 1000));
        light->setDiffuseColour(ColourValue::White);
        light->setSpecularColour(ColourValue::White);

		// setup everything else
		
        Entity *ent = mSceneMgr->createEntity( "Moth", "razor.mesh" );
        SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "MothNode" );
        node->attachObject( ent );

        // Create the scene node
        node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1");
        //node->yaw(Degree(-45));

		// place our camera in the crater
		mCamera->setPosition( Vector3( 4000,1000,4000));
		node->attachObject(mCamera);

        // create the second camera node
     //   node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode2", Vector3(0, 200, 400));
}

void Main::createFrameListener()
{
	// Create the FrameListener
   mFrameListener = new Game(mWindow, mCamera, mSceneMgr);
	mRoot->addFrameListener(mFrameListener);
   // Show the frame stats overlay
   mFrameListener->showDebugOverlay(true);
}
