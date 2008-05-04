#ifndef __HW_WORLD_MANAGER_H_
#define __HW_WORLD_MANAGER_H_

#include <OgreString.h>
#include <OgreSceneManager.h>
#include <OgreSceneQuery.h>
#include <OgreEntity.h>
#include <OgreStringConverter.h>

class WorldManager {
public:
	WorldManager( void );
	~WorldManager( void );
	void loadWorld(Ogre::String, Ogre::SceneManager*);
	bool addBuilding(Ogre::Vector3, const char*);
	Ogre::SceneManager   *mSceneMgr;	
private:
	int mBuildCount;
	Ogre::SceneNode *mWorldNode; // attach all ground objects to this SceneNode
};
#endif

