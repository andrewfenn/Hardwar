#include "GameState.h"

using namespace Ogre;

void GameState::changeState( GameState *state ) { 
    GameManager::getSingletonPtr()->changeState( state ); 
}

void GameState::pushState( GameState *state ) { 
    GameManager::getSingletonPtr()->pushState( state ); 
}

void GameState::popState( void ) { 
    GameManager::getSingletonPtr()->popState(); 
}

void GameState::requestShutdown( void ) {
    GameManager::getSingletonPtr()->requestShutdown();
}

