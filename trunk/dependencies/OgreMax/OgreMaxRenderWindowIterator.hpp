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

#ifndef OgreMax_OgreMaxRenderWindowIterator_INCLUDED
#define OgreMax_OgreMaxRenderWindowIterator_INCLUDED


//Classes----------------------------------------------------------------------
namespace OgreMax
{   
    /** 
     * An interface for iterating through a collection of Ogre::RenderWindow objects 
     * This should not be used to natively store render windows. Instead,
     * it should be created as needed, wrapping access to a real collection that is 
     * stored elsewhere.
     */
    class OgreMaxRenderWindowIterator
    {
    public:
        virtual ~OgreMaxRenderWindowIterator() {}

        virtual bool Start() = 0;
        virtual Ogre::RenderWindow* GetCurrent() = 0;
        virtual bool MoveNext() = 0;
    };

    /** Accesses one render window */
    class OgreMaxOneRenderWindow : public OgreMaxRenderWindowIterator
    {
    public:
        OgreMaxOneRenderWindow(Ogre::RenderWindow* renderWindow)
        {
            this->renderWindow = renderWindow;
            this->iterationEnabled = false;
        }

        bool Start()
        {
            this->iterationEnabled = this->renderWindow != 0;
            return this->iterationEnabled;
        }

        Ogre::RenderWindow* GetCurrent()
        {
            if (this->iterationEnabled)
            {
                this->iterationEnabled = false;                
                return this->renderWindow;
            }
            return 0;
        }

        bool MoveNext()
        {
            this->iterationEnabled = false;                
            return this->iterationEnabled;
        }

    protected:
        Ogre::RenderWindow* renderWindow;
        bool iterationEnabled;
    };

    /** Accesses a render window array */
    class OgreMaxRenderWindowArray : public OgreMaxRenderWindowIterator
    {
    public:
        OgreMaxRenderWindowArray(Ogre::RenderWindow** renderWindows, size_t renderWindowCount)
        {
            this->renderWindows = renderWindows;
            this->renderWindowCount = renderWindowCount;
            this->iterationEnabled = false;
        }

        bool Start()
        {
            this->iterationEnabled = this->renderWindowCount > 0;
            if (this->iterationEnabled)
                this->iterationIndex = 0;
            return this->iterationEnabled;
        }

        Ogre::RenderWindow* GetCurrent()
        {
            if (this->iterationEnabled)
                return this->renderWindows[this->iterationIndex];
            return 0;
        }

        bool MoveNext()
        {
            if (this->iterationEnabled)
            {
                if (++this->iterationIndex == this->renderWindowCount)
                    this->iterationEnabled = false;                            
            }
            return this->iterationEnabled;
        }

    protected:
        Ogre::RenderWindow** renderWindows;
        size_t renderWindowCount;        
        size_t iterationIndex;
        bool iterationEnabled;
    };

    /** 
     * Accesses a render window collection (std::list, std::vector, std::set, and so on) 
     * Example usage with a non-const list: 
     *    OgreMaxRenderWindowCollection<std::list<Ogre::RenderWindow*>&, std::list<Ogre::RenderWindow*>::iterator> renderWindows(myRenderWindowList);
     * Example usage with a const vector: 
     *    OgreMaxRenderWindowCollection<const std::vector<Ogre::RenderWindow*>&, std::vector<Ogre::RenderWindow*>::const_iterator> renderWindows(myRenderWindowVector);
     */
    template <class COLLECTION, class ITERATOR>
    class OgreMaxRenderWindowCollection : public OgreMaxRenderWindowIterator
    {
    public:
        OgreMaxRenderWindowCollection(COLLECTION renderWindowCollection) : renderWindows(renderWindowCollection)
        {
            this->iterationEnabled = false;
        }

        bool Start()
        {
            this->iterationEnabled = !this->renderWindows.empty();
            if (this->iterationEnabled)
                this->iterator = this->renderWindows.begin();
            return this->iterationEnabled;
        }

        Ogre::RenderWindow* GetCurrent()
        {
            if (this->iterationEnabled)
                return *this->iterator;
            return 0;
        }

        bool MoveNext()
        {
            if (this->iterationEnabled)
            {
                if (++this->iterator == this->renderWindows.end())
                    this->iterationEnabled = false;                                   
            }
            return this->iterationEnabled;
        }

    protected:
        COLLECTION renderWindows;
        ITERATOR iterator;
        bool iterationEnabled;
    };

}

#endif
