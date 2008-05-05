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

#include "WorldManager.h"

WorldManager::WorldManager() { 
	mBuildCount = 0;
}

WorldManager::~WorldManager() { 
	if(mWorldDatabase) {
		sqlite3_close(mWorldDatabase);
	}
}


// Used for loading up the locations of objects
bool WorldManager::loadWorldData(std::string filename, Ogre::SceneManager* mSceneMgr) {

	// Assign a pointer to the scene manager and create our scene node to attach buildings to
	this->mSceneMgr = mSceneMgr;
   mWorldNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
   
   // Load up our craters
	mSceneMgr->setWorldGeometry("craters/alpha.cfg");
/*	Ogre::SceneNode* terrainNode = mSceneMgr->getSceneNode("Terrain");
	terrainNode->setScale(10.0f, 10.0f, 10.0f);

	mSceneMgr->getSuggestedViewpoint(false);*/
	
	
	// load the world database
	int result;
	result = sqlite3_open(filename.c_str(), &mWorldDatabase);
	if(result) {
		// couldn't load the file
		std::cerr << "ERROR: \"" << filename << "\" could not be opened" << std::endl << sqlite3_errmsg(mWorldDatabase) << std::endl;
		sqlite3_close(mWorldDatabase);

		return false;
	}
	
	return true;
}

bool WorldManager::saveWorldData() {
	
	return true;
}


bool WorldManager::addBuilding(Ogre::Vector3 position, const char* meshName) {
   // Increase the list
   Building* templist = new Building[mBuildCount+1];
   mBuildings = templist;

	// Add new building data
	Building newbuilding;
	newbuilding.position = position;
	newbuilding.rotation = Ogre::Vector3::ZERO;
	newbuilding.crater = 0; // alpha crater
	newbuilding.airlocks = new Airlock; // TODO: unimplemented
	
	// add to the list
	mBuildings[mBuildCount] = newbuilding;

	// draw building
	Ogre::Entity *ent = mSceneMgr->createEntity("Building["+Ogre::StringConverter::toString(mBuildCount)+"]",
	 "models/hangers/"+(std::string)meshName);
	//	ent->setMaterialName("DefaultBuilding");
	Ogre::SceneNode* mBuildingNode = mWorldNode->createChildSceneNode();
	mBuildingNode->attachObject(ent);
	mBuildingNode->scale(15, 15, 15);
   mBuildingNode->setPosition(position);
   
   // increase the build count
   mBuildCount++;
}
