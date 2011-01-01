/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010  Andrew Fenn
    
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

#pragma once

#include <Ogre.h>
 
namespace Ogre {
  class TextureUnitState;
  class Overlay;
}

namespace Helper {

class FaderCallback
{
public:
  virtual void fadeInCallback() {}
  virtual void fadeOutCallback() {}
};

class Fader
{
public:
  Fader(const char *OverlayName, const char *MaterialName, FaderCallback *instance = 0);
  ~Fader();

  void startFadeIn(double duration = 1.0f);
  void startFadeOut(double duration = 1.0f);
  void fade(const double timeSinceLastFrame);

protected:
  double _alpha;
  double _current_dur;
  double _total_dur;
  FaderCallback *_inst;
  Ogre::TextureUnitState *_tex_unit;
  Ogre::Overlay *_overlay;

  enum _fadeop {
      FADE_NONE,
      FADE_IN,
      FADE_OUT,
  } _fadeop;
};

}
