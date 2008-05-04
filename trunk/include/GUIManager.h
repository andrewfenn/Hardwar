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
#endif

