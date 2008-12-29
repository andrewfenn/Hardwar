/*  This file is part of Hardwar.

    Hardwar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Hardwar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Hardwar.  If not, see <http://www.gnu.org/licenses/>.
*/

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
   CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative); /* useful for logging messages */
}

GUIManager* GUIManager::getSingletonPtr( void ) {
    if( !mGUIManager ) {
        mGUIManager = new GUIManager();
    }
    return mGUIManager;
}

