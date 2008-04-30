#include "EditorState.h"

using namespace Ogre;

EditorState* EditorState::mEditorState;

void EditorState::enter( void ) {

	mRoot         = Root::getSingletonPtr();
	mRoot->createSceneManager(ST_EXTERIOR_CLOSE, "EditorSceneMgr" );
	
	mOverlayMgr   = OverlayManager::getSingletonPtr();
	mSceneMgr     = mRoot->getSceneManager( "EditorSceneMgr" );
	mCamera       = mSceneMgr->createCamera( "EditCamera" );
	mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );

	mMouseOverlay     = mOverlayMgr->getByName( "Overlay/MousePointer" );
	mMousePointer     = mOverlayMgr->getOverlayElement( "MousePointer/Pointer" );
	mMouseOverlay->show();
	
	mEditorOverlay = mOverlayMgr->getByName( "Overlay/EditorState" );
	mEditorOverlay->show();
	
	loadTerrain();
   
   // setup some light
	Light *light = mSceneMgr->createLight("Light1");
   light->setType(Light::LT_POINT);
   light->setPosition(Vector3(1000, 10000, 1000));
   light->setDiffuseColour(ColourValue::White);
   light->setSpecularColour(ColourValue::White);
   
   mCamera->setPosition(Vector3(0, 2000,0));		
	mCamera->lookAt(Vector3(0.0, -90.0, 200.0));
   
   
   mObjectsNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
   mBuildingsNode = mObjectsNode->createChildSceneNode();
}

void EditorState::exit( void ) {
    mEditorOverlay->hide();
    
    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

// ----------------------
// Setup the alpha crater and place objects down
// ----------------------
void EditorState::loadTerrain( void ) {
	// Load level and display terrain
	mTerrainMgr = new TerrainManager;
   mTerrainMgr->loadFile("Alpha");
   
   mSceneMgr->setWorldGeometry("craters/alpha.cfg");
   mSceneMgr->getSuggestedViewpoint(false);
   
   
}

// No pausing or resume in the editor so leave it
void EditorState::pause( void ) { }
void EditorState::resume( void ) { }


void EditorState::update( unsigned long lTimeElapsed ) {

	// Do mouse movement
  	Vector3 pos = mCamera->getPosition();

   if (mMouseY < 10)
    	pos.z++;
   if (mMouseY > mViewport->getActualHeight()-10)
    	pos.z--;
   if (mMouseX < 10)
    	pos.x++;
   if (mMouseX > mViewport->getActualWidth()-10)
    	pos.x--;
   if (zoomin)
   	pos.y-= 10;
   if (zoomout)
   	pos.y+= 10;
	mCamera->setPosition(pos);
	
	// Do mouse rotation
	if (mMouseDownButton2) {
		int diffx=mMouseX-mMouseRotX;
		int diffy=mMouseY-mMouseRotY;
		std::cout << diffy << " "<< mMouseRotY << std::endl;
		CamRotatePos.x += 0.000005f * diffx;
		CamRotatePos.z += 0.000005f * diffy;
		
		mCamera->setDirection(CamRotatePos);
	}
}

void EditorState::keyPressed( const OIS::KeyEvent &e ) {
	 if( e.key == OIS::KC_DOWN ) {
	 		zoomout = true;
	 }
	 if( e.key == OIS::KC_UP ) {
	 		zoomin = true;
	 }	 
}

void EditorState::keyReleased( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_ESCAPE ) {
        this->requestShutdown();
    } else {
       // this->changeState( PlayState::getSingletonPtr() );
    }
    
   // camera controls
   
	if( e.key == OIS::KC_DOWN )
	 		zoomout = false;
	
	if( e.key == OIS::KC_UP )
	 		zoomin = false;
	
	
	// scale terrain up and down
	
	if(e.key == OIS::KC_1) {
		SceneNode* terrainNode = mSceneMgr->getSceneNode("Terrain");
		Vector3 scale = terrainNode->getScale();
		scale += 1; 
		terrainNode->setScale(scale);
	}
	if(e.key == OIS::KC_2) {
		SceneNode* terrainNode = mSceneMgr->getSceneNode("Terrain");
		Vector3 scale = terrainNode->getScale();
		scale -= 1; 
		terrainNode->setScale(scale);
	}
	
	if(e.key == OIS::KC_A) {
		
		Ogre::Entity *ent = mSceneMgr->createEntity("SilverY", "silvery.mesh" );
		mBuildingsNode->attachObject(ent);
		mBuildingsNode->scale( 0.1, .1, .1 );
	   mBuildingsNode->setPosition( Vector3( 0, 100, 0) );
	}
	
	
}

void EditorState::mouseMoved( const OIS::MouseEvent &e ) {
    const OIS::MouseState &mouseState = e.state;
    mMousePointer->setTop(mouseState.Y.abs);
    mMousePointer->setLeft(mouseState.X.abs);
    mMouseX = mouseState.X.abs;
    mMouseY = mouseState.Y.abs;
}

void EditorState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
	if (id == 1) { // right mouse button click, rotate camera
		const OIS::MouseState &mouseState = e.state;
		mMouseDownButton2 = true;
		mMouseRotX = mouseState.X.abs;
		mMouseRotY = mouseState.Y.abs;
		CamRotatePos = mCamera->getDirection();
	}
}

void EditorState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    //this->changeState( PlayState::getSingletonPtr() );
    if (id == 1) {
    	mMouseDownButton2 = false;
    }
}

EditorState* EditorState::getSingletonPtr( void ) {
    if( !mEditorState ) {
        mEditorState = new EditorState();
    }

    return mEditorState;
}
