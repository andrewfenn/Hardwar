/*
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2015  Andrew Fenn

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

#include "GameSettings.h"
#include <boost/filesystem.hpp>

using namespace boost;

GameSettings* GameSettings::mGameSettings;

GameSettings::GameSettings(void)
{
    mParser = new ConfigParserArgv(ConfigParser::Default, "Hardwar\nCopyright (C) 2006-2015 Andrew Fenn\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nFor bug reporting instructions, please see:\n<https://github.com/andrewfenn/Hardwar/issues>.\nFor help, type \"--help\".");
}

GameSettings::~GameSettings(void)
{
    delete mParser;
    mParser = nullptr;
}

bool GameSettings::parseArgv(const int argc, const char** argv)
{
    StandardOptions options(mParser);
    /* Define command line settings before parsing */
    StringOption configSetting(mParser, StringOption::Option::Default, "config", "c", "settings.cfg", "Specifies the configuration file to use");
    bool shouldContinue = true;

    mParser->Parse(argc, argv);
    if (mParser->HasMessages()) {
        mParser->PrintMessages(&std::cout);
        shouldContinue = false;
    }

    if (mParser->HasErrors())
        mParser->PrintErrors(&std::cerr);

    if (mParser->ShouldExit() || !shouldContinue)
    {
        return false;
    }

    Ogre::UTFString configFile = configSetting.Get();
    if (!filesystem::exists(configSetting.Get()))
    {
        std::cerr << "Error: Config file does not exist '" << configFile << "'" << std::endl;
        return false;
    }

    setOption("ConfigFile", configSetting.Get());
    Ogre::ConfigFile lconfig;
    lconfig.load(getOption("ConfigFile"));

    filesystem::path mediaPath = lconfig.getSetting("GameContentDir", "Game", "");
    if (!filesystem::exists(mediaPath))
    {
        std::cerr << "Error: Media path does not exist '" << mediaPath << "'" << std::endl;
        return false;
    }
    setOption("DirectoryMedia", filesystem::canonical(mediaPath).c_str());

    setOption("NetworkRetryLimit", lconfig.getSetting("RetryLimit", "Network", "5"));
    setOption("NetworkTimeout", lconfig.getSetting("Timeout", "Network", "5"));

    return true;
}

void GameSettings::setOption(const Ogre::UTFString lName, Ogre::UTFString lValue)
{
   mOptions[lName] = lValue;
}

Ogre::UTFString GameSettings::getOption(const Ogre::UTFString lName)
{
   Option::iterator iter = mOptions.find(lName);
   if (iter != mOptions.end())
   {
      return mOptions[lName];
   }
   return Ogre::UTFString("");
}

GameSettings* GameSettings::getSingletonPtr(void)
{
   if(!mGameSettings)
   {
      mGameSettings = new GameSettings();
   }

   return mGameSettings;
}
