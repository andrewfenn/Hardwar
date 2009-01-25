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

#ifndef OgreMax_OgreMaxPlatform_INCLUDED
#define OgreMax_OgreMaxPlatform_INCLUDED

                  
//Macros-----------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #if defined(OGREMAX_SHARED_LIBRARY_EXPORT)
        //Exporting from a shared library
        #if defined(__MINGW32__)
            #define _OgreMaxExport
        #else
            #define _OgreMaxExport __declspec(dllexport)
        #endif
    #elif defined(OGREMAX_SHARED_LIBRARY_IMPORT)
        //Importing from a shared library
        #if defined(__MINGW32__)
            #define _OgreMaxExport
        #else
            #define _OgreMaxExport __declspec(dllimport)
        #endif
    #else
        //Using the source code directly or from a static library
        #define _OgreMaxExport
    #endif
#else
    //Either using the source code directly or from a static library...
    //Or compiling on a platform that doesn't require import/export decorations
    #define _OgreMaxExport
#endif


#endif