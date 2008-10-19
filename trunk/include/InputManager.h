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

#ifndef InputManager_H
#define InputManager_H

#include "OISInputManager.h"
#include "OISException.h"
#include "OISKeyboard.h"
#include "OISMouse.h"
#include "OISJoyStick.h"
#include "OISEvents.h"


#include <OgreRenderWindow.h>

////////////////////////////////////Needed Windows Headers////////////
#if defined OIS_WIN32_PLATFORM
#  define WIN32_LEAN_AND_MEAN
#  include "windows.h"
#  ifdef min
#    undef min
#  endif
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by CommandLine.rc
//
#define IDD_DIALOG1                     101
#define IDC_BUTTON1                     1001

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        102
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1002
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

   LRESULT DlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//////////////////////////////////////////////////////////////////////
////////////////////////////////////Needed Linux Headers//////////////
#elif defined OIS_LINUX_PLATFORM
#  include <X11/Xlib.h>
   void checkX11Events();
//////////////////////////////////////////////////////////////////////
////////////////////////////////////Needed Mac Headers//////////////
#elif defined OIS_APPLE_PLATFORM
#  include <Carbon/Carbon.h>
   void checkMacEvents();
#endif
//////////////////////////////////////////////////////////////////////

class InputManager : public OIS::KeyListener, OIS::MouseListener, OIS::JoyStickListener {
public:
    virtual ~InputManager( void );

    void initialise( Ogre::RenderWindow *renderWindow );
    void capture( void );

    void addKeyListener( OIS::KeyListener *keyListener, const std::string& instanceName );
    void addMouseListener( OIS::MouseListener *mouseListener, const std::string& instanceName );
    void addJoystickListener( OIS::JoyStickListener *joystickListener, const std::string& instanceName );

    void removeKeyListener( const std::string& instanceName );
    void removeMouseListener( const std::string& instanceName );
    void removeJoystickListener( const std::string& instanceName );

    void removeKeyListener( OIS::KeyListener *keyListener );
    void removeMouseListener( OIS::MouseListener *mouseListener );
    void removeJoystickListener( OIS::JoyStickListener *joystickListener );

    void removeAllListeners( void );
    void removeAllKeyListeners( void );
    void removeAllMouseListeners( void );
    void removeAllJoystickListeners( void );

    void setWindowExtents( int width, int height );

    OIS::Mouse*    getMouse( void );
    OIS::Keyboard* getKeyboard( void );
    OIS::JoyStick* getJoystick( unsigned int index );

    int getNumOfJoysticks( void );

    static InputManager* getSingletonPtr( void );
private:
    InputManager( void );
    InputManager( const InputManager& ) { }
    InputManager & operator = ( const InputManager& );

    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

    bool povMoved( const OIS::JoyStickEvent &e, int pov );
    bool axisMoved( const OIS::JoyStickEvent &e, int axis );
    bool sliderMoved( const OIS::JoyStickEvent &e, int sliderID );
    bool buttonPressed( const OIS::JoyStickEvent &e, int button );
    bool buttonReleased( const OIS::JoyStickEvent &e, int button );

    OIS::InputManager *mInputSystem;
    OIS::Mouse        *mMouse;
    OIS::Keyboard     *mKeyboard;

    std::vector<OIS::JoyStick*> mJoysticks;
    std::vector<OIS::JoyStick*>::iterator itJoystick;
    std::vector<OIS::JoyStick*>::iterator itJoystickEnd;

    std::map<std::string, OIS::KeyListener*> mKeyListeners;
    std::map<std::string, OIS::MouseListener*> mMouseListeners;
    std::map<std::string, OIS::JoyStickListener*> mJoystickListeners;

    std::map<std::string, OIS::KeyListener*>::iterator itKeyListener;
    std::map<std::string, OIS::MouseListener*>::iterator itMouseListener;
    std::map<std::string, OIS::JoyStickListener*>::iterator itJoystickListener;

    std::map<std::string, OIS::KeyListener*>::iterator itKeyListenerEnd;
    std::map<std::string, OIS::MouseListener*>::iterator itMouseListenerEnd;
    std::map<std::string, OIS::JoyStickListener*>::iterator itJoystickListenerEnd;

    static InputManager *mInputManager;
};
#endif

