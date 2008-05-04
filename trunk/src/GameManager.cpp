#include "GameState.h"

#include "PlayState.h"
#include "IntroState.h"
#include "PauseState.h"
#include "EditorState.h"

#include "GameManager.h"

using namespace Ogre;

GameManager* GameManager::mGameManager;

GameManager::GameManager( void ) :
    mRoot( 0 ),
    mInputMgr( 0 ),
    mIntroState( 0 ),
    mPlayState( 0 ),
    mPauseState( 0 ),
    bShutdown( false ) {
}

GameManager::~GameManager( void ) {
    // Clean up all the states
    while( !mStates.empty() ) {
        mStates.back()->exit();
        mStates.pop_back();
    }

    if( mInputMgr ) {
        delete mInputMgr;
        mInputMgr = 0;
    }

    if( mGUIMgr ) {
        delete mGUIMgr;
        mGUIMgr = 0;
    }

    if( mIntroState ) {
        delete mIntroState;
        mIntroState = 0;
    }	
    if( mPlayState ) {
        delete mPlayState;
        mPlayState  = 0;
    }
	
    if( mPauseState ) {
        delete mPauseState;
        mPauseState = 0;
    }
	
    if( mRoot ) {
        delete mRoot;
        mRoot = 0;
    }
}

void GameManager::startGame( GameState *gameState ) {
    mRoot = new Root();

    // Setup states
/*    mIntroState = IntroState::getSingletonPtr();
  	 mPlayState  = PlayState::getSingletonPtr();
    mPauseState = PauseState::getSingletonPtr();
   */ 
	 mEditorState = EditorState::getSingletonPtr();
	 
    // Setup and configure game
    this->setupResources();
    if( !this->configureGame() ) {
        // If game can't be configured, throw exception and quit application
        throw Ogre::Exception( Ogre::Exception::ERR_INTERNAL_ERROR,
            "Error - Couldn't Configure Renderwindow",
            "Example - Error" );
        return;
    }

    // Setup input
    mInputMgr = InputManager::getSingletonPtr();
    mInputMgr->initialise( mRenderWindow );
    mInputMgr->addKeyListener( this, "GameManager" );
    mInputMgr->addMouseListener( this, "GameManager" );
    mInputMgr->getJoystick( 1 );

    mGUIMgr = 0;

    // Change to first state
    this->changeState( gameState );

    // lTimeLastFrame remembers the last time that it was checked
    // We use it to calculate the time since last frame
    unsigned long lTimeLastFrame = 0;

    // Main while-loop
    while( !bShutdown ) {
        // Calculate time since last frame and remember current time for next frame
        unsigned long lTimeCurrentFrame = mRoot->getTimer()->getMilliseconds();
        unsigned long lTimeSinceLastFrame = lTimeCurrentFrame - lTimeLastFrame;
        lTimeLastFrame = lTimeCurrentFrame;

        // Update inputmanager
        mInputMgr->capture();

        // Update current state
        mStates.back()->update( lTimeSinceLastFrame );

        // Render next frame
        mRoot->renderOneFrame();

        // Deal with platform specific issues
        //PlatformManager::getSingletonPtr()->messagePump( mRenderWindow );
        WindowEventUtilities::messagePump();

    }
}

bool GameManager::configureGame( void ) {
    // Load config settings from ogre.cfg
    if( !mRoot->restoreConfig() ) {
        // If there is no config file, show the configuration dialog
        if( !mRoot->showConfigDialog() ) {
            return false;
        }
    }

    // Initialise and create a default rendering window
    mRenderWindow = mRoot->initialise( true, "Hardwar" );

    // Initialise resources
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create needed scenemanagers
   // mRoot->createSceneManager( ST_GENERIC, "ST_GENERIC" );
   


    return true;
}

void GameManager::setupResources( void ) {
    // Load resource paths from config file
    ConfigFile cf;
    cf.load( "resources.cfg" );

    // Go through all settings in the file
    ConfigFile::SectionIterator itSection = cf.getSectionIterator();

    String sSection, sType, sArch;
    while( itSection.hasMoreElements() ) {
        sSection = itSection.peekNextKey();

        ConfigFile::SettingsMultiMap *mapSettings = itSection.getNext();
        ConfigFile::SettingsMultiMap::iterator itSetting = mapSettings->begin();
        while( itSetting != mapSettings->end() ) {
            sType = itSetting->first;
            sArch = itSetting->second;
                            
            ResourceGroupManager::getSingleton().addResourceLocation(
                sArch, sType, sSection );

            ++itSetting;
        }
    }
}

void GameManager::changeState( GameState *gameState ) {
    // Cleanup the current state
    if( !mStates.empty() ) {
        mStates.back()->exit();
        mStates.pop_back();
    }
    // Store and init the new state
    mStates.push_back( gameState );
    mStates.back()->enter();
}

void GameManager::pushState( GameState *gameState ) {
    // Pause current state
    if( !mStates.empty() ) {
        mStates.back()->pause();
    }

    // Store and init the new state
    mStates.push_back( gameState );
    mStates.back()->enter();
}

void GameManager::popState() {
    // Cleanup the current state
    if( !mStates.empty() ) {
        mStates.back()->exit();
        mStates.pop_back();
    }

    // Resume previous state
    if( !mStates.empty() ) {
        mStates.back()->resume();
    }
}

void GameManager::requestShutdown( void ) {
    bShutdown = true;
}

bool GameManager::keyPressed( const OIS::KeyEvent &e ) {
    // Call keyPressed of current state
    mStates.back()->keyPressed( e );

    return true;
}

bool GameManager::keyReleased( const OIS::KeyEvent &e ) {
    // Call keyReleased of current state
    mStates.back()->keyReleased( e );

    return true;
}

bool GameManager::mouseMoved( const OIS::MouseEvent &e ) {
    // Call mouseMoved of current state
    mStates.back()->mouseMoved( e );

    return true;
}

bool GameManager::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    // Call mousePressed of current state
    mStates.back()->mousePressed( e, id );

    return true;
}

bool GameManager::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    // Call mouseReleased of current state
    mStates.back()->mouseReleased( e, id );

    return true;
}

GameManager* GameManager::getSingletonPtr( void ) {
    if( !mGameManager ) {
        mGameManager = new GameManager();
    }

    return mGameManager;
}

