#include "WorldManager.h"

WorldManager::WorldManager() { 
	mBuildCount = 0;
}

WorldManager::~WorldManager() { 

}


// Used for loading up the locations of objects
void WorldManager::loadWorld(Ogre::String filename, Ogre::SceneManager* mSceneMgr) {

	// Assign a pointer to the scene manager and create our scene node to attach buildings to
	this->mSceneMgr = mSceneMgr;
   mWorldNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
   
   // Load up our craters
	mSceneMgr->setWorldGeometry("craters/alpha.cfg");
/*	Ogre::SceneNode* terrainNode = mSceneMgr->getSceneNode("Terrain");
	terrainNode->setScale(10.0f, 10.0f, 10.0f);

	mSceneMgr->getSuggestedViewpoint(false);*/
}

bool WorldManager::addBuilding(Ogre::Vector3 position, const char* meshName) {

	Ogre::Entity *ent = mSceneMgr->createEntity("Building["+Ogre::StringConverter::toString(mBuildCount)+"]",
	 "models/hangers/"+(std::string)meshName);
//	ent->setMaterialName("DefaultBuilding");
	Ogre::SceneNode* mBuildingNode = mWorldNode->createChildSceneNode();
	mBuildingNode->attachObject(ent);
	mBuildingNode->scale(15, 15, 15);
	position.y += 500;
   mBuildingNode->setPosition(position);
   mBuildCount++;
}
