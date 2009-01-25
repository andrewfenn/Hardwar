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

WorldManager::WorldManager()
{
	mBuildCount = 0;
}

WorldManager::~WorldManager()
{
	if(mSQLdb)
    {
		sqlite3_close(mSQLdb);
	}
}


bool WorldManager::loadWorldData(Ogre::String filename)
{
    /*
     * Load up our world.db file which contains building locations and other
     * data which is used to get a game going
     */
	int result;
	result = sqlite3_open(filename.c_str(), &mSQLdb);
	if(result) {
		// couldn't load the file
		std::cerr << "ERROR: \"" << filename << "\" could not be opened" << 
                                          std::endl << sqlite3_errmsg(mSQLdb) << 
                                       std::endl;/*FIXME: Add this to ogre log*/
		sqlite3_close(mSQLdb);
		return false;
	}
	
	if (loadBuildings())
    {
        printf("Loaded buildings: %d\n", mBuildCount);
    	return true;
    }
    else
    {
        printf("Problem loading buildings\n");
    }
    return false;
}

bool WorldManager::loadBuildings(void)
{
    /* 
     * Loads all the building data in the SQL file 
     */
	int result;
	bool noError = true;
	bool done = false;
	sqlite3_stmt* statement;
	const char* errorMsg = 0;
	Ogre::Vector3 vect;
	
	std::string sql = "SELECT * FROM buildings";

	/* Add new building data */
	Building newbuilding;

	sqlite3_prepare_v2(mSQLdb,sql.c_str(),sql.size(),&statement,NULL);
	while((result = sqlite3_step(statement)) && !done) {
		switch (result) {
			case SQLITE_DONE:
				done = true;
			break;
			case SQLITE_ROW:
				/* get crater id */
				newbuilding.crater = sqlite3_column_int(statement,1);
				/* get mesh name */
				newbuilding.mesh = std::string( (const char * ) 
                                              sqlite3_column_text(statement,2));
				/* get position */
				newbuilding.position.x = sqlite3_column_double(statement,3);
				newbuilding.position.y = sqlite3_column_double(statement,4);
				newbuilding.position.z = sqlite3_column_double(statement,5);
				/* get rotation */
				newbuilding.rotation.x = sqlite3_column_double(statement,6);
				newbuilding.rotation.y = sqlite3_column_double(statement,7);
				newbuilding.rotation.z = sqlite3_column_double(statement,8);

                /* TODO: unimplemented airlocks */
				newbuilding.airlocks = new Airlock;

				/* add to the list and increase the building count */
    			mBuildings.push_back(newbuilding);
                mBuildCount++;
			break;
		}
	}	
	
	result = sqlite3_finalize(statement);
	if( result!=SQLITE_OK ) {
		errorMsg = sqlite3_errmsg(mSQLdb);
		std::cerr << "SQL error: " << errorMsg << 
                                 " in file WorldManager.cpp, loadBuildings()" <<
                                       std::endl; /*FIXME:Add this to ogre log*/
		noError = false;
	}
	return noError;
}

bool WorldManager::deleteBuilding(Ogre::String name)
{
	/*
     * Get rid of a building in our current state
     */
	unsigned int id = atoi(name.substr(9, name.length()-10).c_str());
	unsigned int size = mBuildings.size();
	mBuildings.erase(mBuildings.begin()+id);
	if (size > mBuildings.size()) {
		mBuildCount--;
		return true;
	}
	return false; /* building not found */
}

bool WorldManager::saveWorldData()
{
    /*
     * Save the current state of our game to a database file
     */
	int result;
	bool noError = true;
	sqlite3_stmt* statement;
	const char* errorMsg = 0;

	std::string sql = "REPLACE INTO buildings ( id, crater, mesh, position_x, position_y, position_z, rotation_x, rotation_y, rotation_z) values(:id, :crater, :mesh, :position_x, :position_y, :position_z, :rotation_x, :rotation_y, :rotation_z);";

	/* Save building data */
	for (unsigned int i=0; i < mBuildCount; i++)
    {
	
		sqlite3_prepare_v2(mSQLdb, sql.c_str(), sql.size(), &statement, NULL);
	
		sqlite3_bind_int (statement, 1, i);
		sqlite3_bind_int (statement, 2, mBuildings[i].crater);
		sqlite3_bind_text(statement, 3, mBuildings[i].mesh.c_str(), mBuildings[i].mesh.size(), SQLITE_TRANSIENT);
		/* insert position data */
		sqlite3_bind_double(statement, 4, (double)mBuildings[i].position.x);
		sqlite3_bind_double(statement, 5, (double)mBuildings[i].position.y);
		sqlite3_bind_double(statement, 6, (double)mBuildings[i].position.z);
		/* insert rotation data */
		sqlite3_bind_double(statement, 7, (double)mBuildings[i].rotation.x);
		sqlite3_bind_double(statement, 8, (double)mBuildings[i].rotation.y);
		sqlite3_bind_double(statement, 9, (double)mBuildings[i].rotation.z);
		/* execute statement */
		result = sqlite3_step(statement);

		if( result!=SQLITE_DONE )
        {
			errorMsg = sqlite3_errmsg(mSQLdb);
            /* FIXME: Add this to ogre log */
			std::cerr << "SQL error: " << errorMsg << 
                      " in file WorldManager.cpp, saveWorldData()" << std::endl;
			noError = false;
		}
	
		sqlite3_reset(statement);
		// get id of building we just inserted and save airlock data
	}
	result = sqlite3_finalize(statement);
	if( result!=SQLITE_OK )
    {
		errorMsg = sqlite3_errmsg(mSQLdb);
		/* FIXME: Add this to ogre log */
	    std::cerr << "SQL error: " << errorMsg << 
                      " in file WorldManager.cpp, saveWorldData()" << std::endl;
		noError = false;
	}

	/*
     * delete any leftover buildings.
	 * Note: I am doing it this way rather then flushing the whole table in case
     * something messes up with the inserts and then ruins the database.
     */
	sql = "DELETE FROM buildings WHERE id > :id;";
	sqlite3_prepare_v2(mSQLdb, sql.c_str(), sql.size(), &statement, NULL);
	sqlite3_bind_int (statement, 1, mBuildCount);
	result = sqlite3_step(statement);
	if( result!=SQLITE_DONE )
    {
			errorMsg = sqlite3_errmsg(mSQLdb);
			/* FIXME: Add this to ogre log */
			std::cerr << "SQL error: " << errorMsg << 
                      " in file WorldManager.cpp, saveWorldData()" << std::endl;
			noError = false;
	}
	result = sqlite3_finalize(statement);
	if( result!=SQLITE_OK )
    {
		errorMsg = sqlite3_errmsg(mSQLdb);
		/* FIXME: Add this to ogre log */
		std::cerr << "SQL error: " << errorMsg << 
                      " in file WorldManager.cpp, saveWorldData()" << std::endl;
		noError = false;
	}
	return noError;
}

bool WorldManager::addBuilding(Ogre::Vector3 position, const char* meshName)
{
    /* 
     * Add a new building to our current game state. Not saved until 
     * saveWorldData() is called.
     */

    mBuildCount++;

	Building newbuilding;
	newbuilding.position = position;
	newbuilding.rotation = Ogre::Vector3::ZERO; /*FIXME: Change this to a Quad*/
    /* TODO: Add code to figure out what crater we're editing */
	newbuilding.crater = 0;
    /* TODO: unimplemented airlocks */
	newbuilding.airlocks = new Airlock;
	newbuilding.mesh = (std::string)meshName;

	/* check the building was added */
	unsigned int numBuildings = mBuildings.size();
	mBuildings.push_back(newbuilding);
	if (mBuildings.size() > numBuildings)	
		return true;
	return false;
}
