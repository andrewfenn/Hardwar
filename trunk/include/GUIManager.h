/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2008  Andrew Fenn
    
    Hardwar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Hardwar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GUIManager_H
#define GUIManager_H

#ifndef _CEGUI_fix_h_
	#define _CEGUI_fix_h_
	#pragma warning (push)
	#pragma warning( disable : 4267 )
	/** This file eases an issue CEGUI has with OGRE's memory manager. Simply include this in
		any files that are going to be doing 'CEGUI stuff', rather than CEGUI.h, OgreCEGUIRender.h, 
		OgreCEGUIResourceProvider.h, and OgreCEGUITexture.h (etc). You will be in for smooth(er) 
		sailing.
	**/ 

	#include <OgreNoMemoryMacros.h>
	#include <CEGUI/CEGUI.h>
	#include <OgreCEGUIRenderer.h>
	#include <OgreCEGUIResourceProvider.h>
	#include <OgreCEGUITexture.h>
	#include <OgreMemoryMacros.h>
	#pragma warning (pop)
#endif

class GUIManager {
public:
    virtual ~GUIManager( void );

    void initialise(Ogre::SceneManager*, Ogre::RenderWindow*);

    static GUIManager* getSingletonPtr( void );

    // User interface
    CEGUI::System *mSystem;
	 CEGUI::OgreCEGUIRenderer *mRenderer;
private:
    GUIManager( void );
    GUIManager( const GUIManager& ) { }
    GUIManager & operator = ( const GUIManager& );

    
    static GUIManager *mGUIManager;
};
#endif // GUIManager_H

