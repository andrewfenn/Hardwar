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
bool WorldManager::loadWorldData(std::string filename, Ogre::SceneManager* SceneMgr) {

    // Assign a pointer to the scene manager and create our scene node to attach buildings to
    this->mSceneMgr = SceneMgr;
    mRoot = Ogre::Root::getSingletonPtr();
    mWorldNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

    // Load up our crater
    Ogre::Entity *ent = mSceneMgr->createEntity("world", "hardwar/non-free/world.mesh");
    mWorldNode->attachObject(ent);
	mWorldNode->scale(200, 200, 200);

	// load the world database
	int result;
	result = sqlite3_open(filename.c_str(), &mWorldDatabase);
	if(result) {
		// couldn't load the file
		std::cerr << "ERROR: \"" << filename << "\" could not be opened" << std::endl << sqlite3_errmsg(mWorldDatabase) << std::endl; // FIXME: Add this to ogre log
		sqlite3_close(mWorldDatabase);

		return false;
	}
	
	loadBuildings(-1);
	drawBuildings();
	return true;
}

// Draws the list of buildings
bool WorldManager::drawBuildings() {

	for (unsigned int i=0; i < mBuildCount; i++) {
		// draw building
		Ogre::Entity *ent = mSceneMgr->createEntity("Building["+Ogre::StringConverter::toString(i)+"]",
		 "models/hangers/"+(std::string)mBuildings[i].mesh);
		Ogre::SceneNode* mBuildingNode = mWorldNode->createChildSceneNode();
		mBuildingNode->attachObject(ent);
		mBuildingNode->scale(15, 15, 15);
		mBuildingNode->setPosition(mBuildings[i].position);
//		mBuildingNode->setRotation(mBuildings[i].rotation);
	}
	
	return true;
}

// Loads all the building data in the SQL file
// if no crater number is given it will load all the buildings
bool WorldManager::loadBuildings(int crater = -1) {
	int result;
	bool noError = true;
	bool done = false;
	sqlite3_stmt* statement; // sql statement
	const char* errorMsg = 0;
	Ogre::Vector3 vect;
	
	std::string sql = "SELECT * FROM buildings";
	if (crater == -1) {
		sql += ";";
	} else {
		sql += "WHERE crater=:crater;";
	}

	// Add new building data
	Building newbuilding;

	sqlite3_prepare_v2(mWorldDatabase, sql.c_str(), sql.size(), &statement, NULL);
	while((result = sqlite3_step(statement)) && !done) {
		switch (result) {
			case SQLITE_DONE:
				done = true;
			break;
			case SQLITE_ROW:
				// get crater
				newbuilding.crater = sqlite3_column_int(statement,1);
				// get mesh name
				newbuilding.mesh = std::string( (const char * ) sqlite3_column_text(statement,2));
				// get position
				newbuilding.position.x = sqlite3_column_double(statement,3);
				newbuilding.position.y = sqlite3_column_double(statement,4);
				newbuilding.position.z = sqlite3_column_double(statement,5);
				// get rotation
				newbuilding.rotation.x = sqlite3_column_double(statement,6);
				newbuilding.rotation.y = sqlite3_column_double(statement,7);
				newbuilding.rotation.z = sqlite3_column_double(statement,8);
				newbuilding.airlocks = new Airlock; // TODO: unimplemented
				// add to the list
				mBuildings.push_back(newbuilding);
		
				// increase the build count
			   mBuildCount++;
			break;
		}
	}	
	
	result = sqlite3_finalize(statement);
	if( result!=SQLITE_OK ) {
		errorMsg = sqlite3_errmsg(mWorldDatabase);
		std::cerr << "SQL error: " << errorMsg << " in file WorldManager.cpp, loadBuildings()" << std::endl; // FIXME: Add this to ogre log
		noError = false;
	}
	return noError;
}

bool WorldManager::deleteBuilding(std::string name) {
	// convert the buildng[number] to number
	unsigned int id = atoi(name.substr(9, name.length()-10).c_str());
	unsigned int size = mBuildings.size();
	mBuildings.erase(mBuildings.begin()+id);
	if (size > mBuildings.size()) {
		mBuildCount--;
		return true;
	}
	return false; // building not found
}

bool WorldManager::saveWorldData() {
	int result;
	bool noError = true;
	sqlite3_stmt* statement; // sql statement
	const char* errorMsg = 0;
	
	std::string sql = "REPLACE INTO buildings ( id, crater, mesh, position_x, position_y, position_z, rotation_x, rotation_y, rotation_z) values(:id, :crater, :mesh, :position_x, :position_y, :position_z, :rotation_x, :rotation_y, :rotation_z);";

	// Save building data
	for (unsigned int i=0; i < mBuildCount; i++) {
	
		sqlite3_prepare_v2(mWorldDatabase, sql.c_str(), sql.size(), &statement, NULL);
	
		sqlite3_bind_int (statement, 1, i);
		sqlite3_bind_int (statement, 2, mBuildings[i].crater);
		sqlite3_bind_text(statement, 3, mBuildings[i].mesh.c_str(), mBuildings[i].mesh.size(), SQLITE_TRANSIENT);
		// insert position data
		sqlite3_bind_double(statement, 4, (double)mBuildings[i].position.x);
		sqlite3_bind_double(statement, 5, (double)mBuildings[i].position.y);
		sqlite3_bind_double(statement, 6, (double)mBuildings[i].position.z);
		// insert rotation data
		sqlite3_bind_double(statement, 7, (double)mBuildings[i].rotation.x);
		sqlite3_bind_double(statement, 8, (double)mBuildings[i].rotation.y);
		sqlite3_bind_double(statement, 9, (double)mBuildings[i].rotation.z);
		// execute statement
		result = sqlite3_step(statement);

		if( result!=SQLITE_DONE ){
			errorMsg = sqlite3_errmsg(mWorldDatabase);
			std::cerr << "SQL error: " << errorMsg << " in file WorldManager.cpp, saveWorldData()" << std::endl; // FIXME: Add this to ogre log
			noError = false;
		}
	
		sqlite3_reset(statement);
		// get id of building we just inserted and save airlock data
	}
	result = sqlite3_finalize(statement);
	if( result!=SQLITE_OK ) {
		errorMsg = sqlite3_errmsg(mWorldDatabase);
		std::cerr << "SQL error: " << errorMsg << " in file WorldManager.cpp, saveWorldData()" << std::endl; // FIXME: Add this to ogre log
		noError = false;
	}

	// delete any leftover buildings. 
	// Note: I am doing it this way rather then flushing the whole table incase something messes up with the inserts
	sql = "DELETE FROM buildings WHERE id > :id;";
	sqlite3_prepare_v2(mWorldDatabase, sql.c_str(), sql.size(), &statement, NULL);
	sqlite3_bind_int (statement, 1, mBuildCount);
	result = sqlite3_step(statement);
	if( result!=SQLITE_DONE ){
			errorMsg = sqlite3_errmsg(mWorldDatabase);
			std::cerr << "SQL error: " << errorMsg << " in file WorldManager.cpp, saveWorldData()" << std::endl; // FIXME: Add this to ogre log
			noError = false;
	}
	result = sqlite3_finalize(statement);
	if( result!=SQLITE_OK ) {
		errorMsg = sqlite3_errmsg(mWorldDatabase);
		std::cerr << "SQL error: " << errorMsg << " in file WorldManager.cpp, saveWorldData()" << std::endl; // FIXME: Add this to ogre log
		noError = false;
	}
	return noError;
}


// Adding a building Via the editor
bool WorldManager::addBuilding(Ogre::Vector3 position, const char* meshName) {

	// draw building
	Ogre::Entity *ent = mSceneMgr->createEntity("Building["+Ogre::StringConverter::toString(mBuildCount)+"]",
	 "models/hangers/"+(std::string)meshName);
	//	ent->setMaterialName("DefaultBuilding");
	Ogre::SceneNode* mBuildingNode = mWorldNode->createChildSceneNode();
	Ogre::AxisAlignedBox box = ent->getBoundingBox();
	mBuildingNode->attachObject(ent);
	mBuildingNode->scale(100, 100, 100);

	// push the building up out of the ground
	box.scale(Ogre::Vector3(100,100,100));
    Ogre::Vector3 size = box.getSize();
    position.y=+size.y;
    mBuildingNode->setPosition(position);
   
    // increase the build count
    mBuildCount++;
   
    // Add the building data to worldManager
	Building newbuilding;
	newbuilding.position = position;
	newbuilding.rotation = Ogre::Vector3::ZERO; // FIXME: Change this to a Quad
	newbuilding.crater = 0; // alpha crater TODO: Add code to figure out what crater we're editing
	newbuilding.airlocks = new Airlock; // TODO: unimplemented
	newbuilding.mesh = (std::string)meshName;

	// check the building was added
	int numBuildings = mBuildings.size();
	mBuildings.push_back(newbuilding);
	if (mBuildings.size() > numBuildings)	
		return true;
	return false;
}
