/*
 * OgreMaxViewer - An Ogre 3D-based viewer for .scene and .mesh files
 * Copyright 2008 Derek Nedelman
 *
 * This code is available under the OgreMax Free License:
 *   -You may use this code for any purpose, commercial or non-commercial.
 *   -If distributing derived works (that use this source code) in binary or source code form, 
 *    you must give the following credit in your work's end-user documentation: 
 *        "Portions of this work provided by OgreMax (www.ogremax.com)"
 *
 * Derek Nedelman assumes no responsibility for any harm caused by using this code.
 * 
 * OgreMaxViewer was written by Derek Nedelman and released at www.ogremax.com 
 */

#ifndef OgreMax_ProgressCalculator_INCLUDED
#define OgreMax_ProgressCalculator_INCLUDED


//Includes---------------------------------------------------------------------
#include <OgreString.h>
#include "OgreMaxPlatform.hpp"


//Classes----------------------------------------------------------------------
namespace OgreMax
{
    /**
     * A hierarchical collection of progress calculators. Having a hierarchy
     * allows a collection of small events to have their progress tracked separately
     * and aggregated as a larger event, such as loading a file and all of the
     * objects in that file. Progress is calculated as a number between 0 and 1, inclusive
     */
    class _OgreMaxExport ProgressCalculator
    {
    public:
        ProgressCalculator();
        ProgressCalculator(const Ogre::String& name);
        virtual ~ProgressCalculator();

        /** Gets the name of the calculator. */
        const Ogre::String& GetName() const;

        /** Calculates and returns the total progress, on a scale of 0 to 1. */
        Ogre::Real GetProgress();

        /** 
         * Manually sets the progress
         * This will not normally need to be called, but it is useful in circumstances
         * where there are multiple progress calculators virtually connected, and the
         * progress of one calculator needs to be input into a separate one elsewhere.
         * Calling this is pointless if the calculator has child calculators.
         * @param progress [in] - The total progress, on a scale of 0 to 1
         */
        void SetProgress(Ogre::Real progress);
        
        /** 
         * Updates the progress for this calculator. 
         * Calling this is pointless if the calculator has child calculators.
         * @param amount [in] - The amount of progress to add. The amount is 
         * in the same units as those passed into SetRange(). The amount may
         * be positive or negative
         */
        void Update(Ogre::Real amount);

        /** 
         * Gets the current value range. 
         * Calling this is pointless if the calculator has child calculators.
         */
        Ogre::Real GetRange() const;

        /** 
         * Sets the value range of this calculator. 
         * Note that calling this method resets the progress to 0.
         * Also note that calling this is pointless if the calculator has child calculators
         * @param range [in] - The range is used to scale amounts passed to Update() and, in turn,
         * calculate the total progress. As an example, if using the calculator
         * to track the number of objects loaded, SetRange() should be called with
         * the total number of objects, and Update(1) should be called every
         * time one object is loaded.
         */
        void SetRange(Ogre::Real range);

        /**
         * Creates a child calculator.
         * @param name [in] - The name of the calculator
         * @return A child calculator is returned. This calculator should not 
         * be deleted.
         */
        ProgressCalculator* AddCalculator(const Ogre::String& name);
        
    protected:
        /** The name of the calculator. This is provided for the user's benefit. */
        Ogre::String name;

        /** The current progress. */
        Ogre::Real progress;

        /** The range used to scale progress to [0,1]. */
        Ogre::Real range;
        
        typedef std::list<ProgressCalculator*> Calculators;
        
        /** Child calculators. */
        Calculators childCalculators;
    };

}

#endif
