/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2010  Andrew Fenn
    
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

#ifndef __InputManager_H_
#define __InputManager_H_

#include "OISInputManager.h"
#include "OISException.h"
#include "OISKeyboard.h"
#include "OISMouse.h"
#include "OISJoyStick.h"
#include "OISEvents.h"


#include <OgreRenderWindow.h>

namespace Client
{

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
};

}
#endif /* __InputManager_H_ */

