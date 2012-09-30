/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2012  Andrew Fenn
    
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

#include "GameRootWin.h"

namespace Client
{

bool GameRootWindows::isLocked()
{
    std::cerr << "STUB: GameRootWindows::isLocked" << std::endl;
    return true;
}

void GameRootWindows::setLocked(const bool& locked)
{
    std::cerr << "STUB: GameRootWindows::setLocked" << std::endl;
    return true;
}

bool GameRootWindows::loadPlugins()
{
    HRESULT hr;
    DWORD dwDirectXVersion = 0; 
    TCHAR strDirectXVersion[10]; 

    hr = GetDXVersion( &dwDirectXVersion, strDirectXVersion, 10 ); 

    if(!SUCCEEDED(hr))
        return false;

    ostringstream dxinfoStream; 
    dxinfoStream << "DirectX version: " << strDirectXVersion; 
    LogManager::getSingleton().logMessage(dxinfoStream.str()); 

    if(dwDirectXVersion < 0x00090000)
        return false;

    if (!this->loadPlugin("RenderSystem_Direct3D9"))
    {
        if (!this->loadPlugin("RenderSystem_GL"))
            return false;
    }

    if (!this->loadPlugin("Plugin_OctreeSceneManager"))
        return false;

    if (!this->loadPlugin("Plugin_CgProgramManager"))
        return false;
}

} /* namespace Client */

