#include "GUIManager.h"

GUIManager *GUIManager::mGUIManager;

GUIManager::GUIManager( void ) {
	mRenderer = 0;
	mSystem = 0;	
}

GUIManager::~GUIManager( void ) {
	
}

void GUIManager::initialise(Ogre::SceneManager *mSceneMgr, Ogre::RenderWindow *mRenderWindow ) {
   mRenderer = new CEGUI::OgreCEGUIRenderer(mRenderWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
   mSystem = new CEGUI::System(mRenderer);
   CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative); // useful for logging messages
}

GUIManager* GUIManager::getSingletonPtr( void ) {
    if( !mGUIManager ) {
        mGUIManager = new GUIManager();
    }
    return mGUIManager;
}

