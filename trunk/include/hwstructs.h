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

#ifndef __HW_STRUCTS_H_
#define __HW_STRUCTS_H_

#include <string.h>
#include <OgreEntity.h>
#include "srvstructs.h"
#include "enet/enet.h"

typedef struct {
    char name[40];          /* Players name */
    clientStatus conState;  /* Connection state */
} Player;

typedef struct {
	int type;               /* type of good. software, equipment, food, etc */
} Item;

typedef struct {
	Item good;              /* software, equipment, goods, etc */
	int price;              /* offered price */
} Order;

typedef struct {
	int owner;              /* id of the owner */
	Player driver;          /* the player in the moth */
	Player passenger;       /* the player in the passenger seat */
} Moth;

typedef struct {
    unsigned int id;				/* unique id */
    unsigned int targetBuilding;	/*	same as id in Building struct */
    char[40] name;                  /*	buildings name */
    unsigned int* software;         /* Software installed in the building */
    unsigned int* function;			/* Building's function */
    unsigned int* subfunction;      /* any other functions the building provides*/
    unsigned int owner;
    unsigned int faction;
    unsigned int isPublic;			/* 0=Public, 1=Private. */
    unsigned long cash;				/* Amount of cash the building has.*/

    unsigned long population;       /* number of people living in the building
								     * determines how much food they need 
                                     */
    int health;                     /* building's health value */

    /* requires sales software installed */
    double purchaseScale;           /* If 0, the building won't buy goods at all. */
    double sellScale;               /* Value scale factor for selling goods.  0 == not selling */
    Order* Orders;                  /* Orders put in for new goods, equipment, software, etc. */

    /* requires fitting equipement installed */
    double fitCharge;               /* Fraction of goods value to charge for */


    /* Vehicles */

    Moth* vehiclesParked;           /* number of Moths in the hanger */
    int inTransit;                  /* Holds the number of the current moth in transit */
    int transitStatus;              /* How far or near to the airlock. 0 = in airlock.. 100 = on pad */
    Moth* vehiclesWaiting;          /* number of vehicals waiting to get into the hanger */

    /* requires cloning equipment installed */
    int clonesInstalled;            /* How many clones (lives) the player has. */

    /* Monorail related stuff. */
    unsigned int monoId;            /* Id location to the outside. */

    /* Visual stuff. */
    double seed1, seed2;            /* Seed for randomising hanger internals */
    unsigned short searchLight;	    /* if building has a searchlight or not */
    Ogre::Vector3 searchColour;	    /* Colour of the buildings searchlight */
    int logo;                       /* id number of the logo used inside the building */
} Hanger;

typedef struct {
    Hanger* hanger;                 /* target hanger, same as id in Hanger struct */
    unsigned int type;              /* type of door used on the building */
    Ogre::Vector3 position;         /* position of the door */
    Ogre::Vector3 rotation;         /* rotation of the door */
} Airlock;

typedef struct {
    unsigned int id;                /* id of the row in our database */
    unsigned int crater;            /* which crater the building is located in */
    Airlock* airlocks;              /* number of airlocks on the building */
    std::string mesh;               /* mesh type */
    Ogre::Vector3 position;
    Ogre::Vector3 rotation;
} Building;

#endif /* __HW_STRUCTS_H_ */

