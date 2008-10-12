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

//Includes---------------------------------------------------------------------
#include "ProgressCalculator.hpp"

using namespace Ogre;
using namespace OgreMax;


//Implementation---------------------------------------------------------------
ProgressCalculator::ProgressCalculator()
{
    this->progress = 0;
    this->range = 0;
}

ProgressCalculator::ProgressCalculator(const Ogre::String& name)
{
    this->name = name;
    this->progress = 0;
    this->range = 0;
}

ProgressCalculator::~ProgressCalculator()
{
    //Delete child calculators
    for (Calculators::iterator calculator = this->childCalculators.begin();
        calculator != this->childCalculators.end();
        ++calculator)
    {
        delete *calculator;
    }
}

const String& ProgressCalculator::GetName() const
{
    return this->name;
}

Real ProgressCalculator::GetProgress()
{
    if (!this->childCalculators.empty())        
    {
        //Reset progress
        this->progress = 0;

        //Determine the influence of each child calculator
        Real influence = (Real)1.0/this->childCalculators.size();        

        //Add the progress of all child calculators
        for (Calculators::iterator calculator = this->childCalculators.begin();
            calculator != this->childCalculators.end();
            ++calculator)
        {
            this->progress += (*calculator)->GetProgress() * influence;
        }
    }
    else if (this->range == 0)
    {
        //No child calculators and no range. There is no progress to be made
        this->progress = 1;
    }

    return this->progress;
}

void ProgressCalculator::SetProgress(Ogre::Real progress)
{
    this->progress = progress;
}

void ProgressCalculator::Update(Real amount)
{
    if (this->range > 0)
    {
        //Update the progress, scaling it by the inverse range
        this->progress += amount / this->range;

        //Limit the progress to [0, 1]    
        this->progress = std::max(this->progress, (Real)0);
        this->progress = std::min(this->progress, (Real)1);    
    }
}

Real ProgressCalculator::GetRange() const
{
    return this->range;
}

void ProgressCalculator::SetRange(Real range)
{
    this->progress = 0;
    this->range = range;
}

ProgressCalculator* ProgressCalculator::AddCalculator(const Ogre::String& name)
{
    ProgressCalculator* calculator = new ProgressCalculator(name);
    this->childCalculators.push_back(calculator);
    return calculator;
}

