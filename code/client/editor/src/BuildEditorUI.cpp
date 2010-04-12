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

#include "BuildEditorUI.h"

using namespace Client;

BuildEditorUI::BuildEditorUI(void)
{
   mShow = false;
   mBuildingPage = 0;
   mMaxBuildingRows = mMaxBuildingCols = 4;
   mGUI = MyGUI::Gui::getInstancePtr();
   MyGUI::LayoutManager::getInstance().load("build_editor.layout");

   mMenuBar = mGUI->findWidget<MyGUI::StaticImage>("BuildEditorMenuTop");
   mMenuBar->setVisible(false);
   mMenuPanelAdd = mGUI->findWidget<MyGUI::Widget>("BuildEditorMenuAdd");
   mMenuPanelAdd->setVisible(false);
   mMenuPanelEdit = mGUI->findWidget<MyGUI::Widget>("BuildEditorMenuEdit");
   mMenuPanelEdit->setVisible(false);

   // Add console command
   Console* mConsole = GameManager::getSingletonPtr()->getConsole();
   mConsole->addCommand(Ogre::UTFString("cl_showeditor"), MyGUI::newDelegate(this, &BuildEditorUI::cmd_showEditor));

   // Make the buttons function
   MyGUI::ButtonPtr lButton;
   lButton = mGUI->findWidget<MyGUI::Button>("BuildEditorMenuNext");
   lButton->eventMouseButtonClick = MyGUI::newDelegate(this, &BuildEditorUI::buttonNext);

   lButton = mGUI->findWidget<MyGUI::Button>("BuildEditorMenuPrevious");
   lButton->eventMouseButtonClick = MyGUI::newDelegate(this, &BuildEditorUI::buttonPrevious);

   mRoot = Ogre::Root::getSingletonPtr();
   mRoot->createSceneManager(Ogre::ST_GENERIC,"EditorSceneMgr");
   mSceneMgr = mRoot->getSceneManager("EditorSceneMgr");
   mSceneMgr->setAmbientLight(Ogre::ColourValue(1,1,1));
   Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup("BuildEditorIcons");

   show(false);
}

BuildEditorUI::~BuildEditorUI(void)
{
   Ogre::ResourceGroupManager::getSingletonPtr()->destroyResourceGroup("BuildEditorIcons");
   mBoxMgr.clear();
}

void BuildEditorUI::addMesh(Ogre::UTFString name)
{
   mBuildingList.push_back(name);
   mBuildingMaxPage = mBuildingList.size() / (mMaxBuildingCols*mMaxBuildingRows);
}

void BuildEditorUI::buttonNext(MyGUI::WidgetPtr lWidget)
{
   if (mBuildingPage == mBuildingMaxPage)
      return;

   renderBuildingList(++mBuildingPage);
}

void BuildEditorUI::buttonPrevious(MyGUI::WidgetPtr lWidget)
{
   if (mBuildingPage == 0)
      return;

   renderBuildingList(--mBuildingPage);
}

void BuildEditorUI::checkUIButtons(void)
{
   MyGUI::ButtonPtr lButton;
   lButton = mGUI->findWidget<MyGUI::Button>("BuildEditorMenuNext");
   if (mBuildingPage == mBuildingMaxPage)
   {
      // hide next button
      lButton->setVisible(false);
   }
   else
   {
      lButton->setVisible(true);
   }

   lButton = mGUI->findWidget<MyGUI::Button>("BuildEditorMenuPrevious");
   if (mBuildingPage == 0)
   {
      // hide previous button
      lButton->setVisible(false);
   }
   else
   {
      lButton->setVisible(true);
   }
}

void BuildEditorUI::renderBuildingList(unsigned short pageNum)
{
   try
   {
      // Remove existing icons in the UI
      mBoxMgr.clear();
      Ogre::ResourceGroupManager::getSingletonPtr()->clearResourceGroup("BuildEditorIcons");
   }
   catch (Ogre::Exception e) { }

   unsigned short x = 1;
   unsigned short y = 1;
   unsigned short startFrom = ((mMaxBuildingCols*mMaxBuildingRows)*mBuildingPage);

   std::vector<Ogre::UTFString>::iterator buildingItr = mBuildingList.begin();
   advance(buildingItr, startFrom);

   while (buildingItr < mBuildingList.end())
   {
      if (x > mMaxBuildingRows)
      {
         buildingItr = mBuildingList.end();
      }
      else
      {
         Ogre::UTFString lPanelName = Ogre::UTFString("RenderBox")+Ogre::StringConverter::toString(x)+"_"+Ogre::StringConverter::toString(y);
	      renderMesh((Ogre::String)(*buildingItr), lPanelName);
      }
      y++;
      if (y > mMaxBuildingCols)
      {
         x++;
         y=1;
      }
      buildingItr++;
   }
   checkUIButtons();
}


void BuildEditorUI::renderMesh(const Ogre::UTFString lMesh, const Ogre::UTFString lPanelName)
{
   /* FIXME: We could do this better by moving some of this over to renderBuildingList
      instead of adding and removing it over and over */
   /* TODO: check if entity exists */
   Ogre::SceneNode *lEditorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(lPanelName, Ogre::Vector3(0, 0, 0 ));   
   Ogre::Entity *lent = mSceneMgr->createEntity(lPanelName, lMesh);
   lent->setMaterialName("shader/diffuse");
   lEditorNode->attachObject(lent);

   Ogre::Camera *lCamera = mSceneMgr->createCamera("EditorCamera_"+lPanelName);
   lCamera->setPosition(Ogre::Vector3(0, 0+(lent->getBoundingBox().getSize().y*0.5), (lent->getBoundingBox().getSize().y)*1.2)+lent->getBoundingBox().getSize().z);
   lCamera->lookAt(Ogre::Vector3(0,0,0));

   Ogre::TextureManager::getSingleton().remove(lPanelName);
   Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual(lPanelName, "BuildEditorIcons",
                                  Ogre::TEX_TYPE_2D, 512, 512, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET );
   Ogre::RenderTexture *renderTexture = texture->getBuffer()->getRenderTarget();

   renderTexture->addViewport(lCamera, 0);
   renderTexture->getViewport(0)->setDimensions(0, 0, 1, 1);
   renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::ZERO);
   renderTexture->getViewport(0)->setOverlaysEnabled(false);
   renderTexture->update(true);

   mBoxMgr.addItem(new Client::ItemBox(lPanelName, lPanelName, lMesh));

   renderTexture->setActive(false);
   renderTexture->removeAllViewports();

   mSceneMgr->destroyEntity(lPanelName);
   lEditorNode->removeAndDestroyAllChildren();
   mSceneMgr->destroyAllCameras();
   mSceneMgr->destroySceneNode(lPanelName);
}

void BuildEditorUI::cmd_showEditor(const Ogre::UTFString &key, const Ogre::UTFString &value)
{
   bool show = false;

   if (!MyGUI::utility::parseComplex(value, show))
   {
      if (!value.empty())
      {
         mConsole->print(mConsole->getConsoleError(), key, value);
      }
      mConsole->print(mConsole->getConsoleFormat(), key, "[true|false] - "+Ogre::UTFString(gettext("Show the hardwar editor")));
   }
   else
   {
      bool isAdmin = Ogre::StringConverter::parseBool(GameSettings::getSingletonPtr()->getOption("isAdmin"));
      if (isAdmin)
      {
         this->show(show);
         mConsole->print(mConsole->getConsoleSuccess(), key, Ogre::StringConverter::toString(show));
      }
      else
      {
         mConsole->print(mConsole->getConsoleError(), key, Ogre::UTFString(gettext("You must be logged in to do this. (use rcon_password)")));
      }
   }
}

bool BuildEditorUI::isVisible()
{
   if (mBoxMgr.isIconActive())
   {
      return true;
   }
   return mShow;
}

bool BuildEditorUI::hasPlaceableIcon()
{
   return mBoxMgr.isPlaceable();
}

MyGUI::IntPoint BuildEditorUI::getIconCoords()
{
   return mBoxMgr.getPoint();
}

Ogre::UTFString BuildEditorUI::getIconName()
{
   return mBoxMgr.getMeshName();
}

void BuildEditorUI::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   mGUI->injectMousePress(e, id);
}

void BuildEditorUI::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   mGUI->injectMouseRelease(e, id);
}

void BuildEditorUI::mouseMoved(const OIS::MouseEvent &e)
{
   mGUI->injectMouseMove(e);
}

void BuildEditorUI::showEditPane(bool show)
{
   if (show)
   {
      mMenuPanelAdd->setVisible(false);
      mMenuPanelEdit->setVisible(true);
   }
   else
   {
      mMenuPanelAdd->setVisible(true);
      mMenuPanelEdit->setVisible(false);

      if (!mGUI->isShowPointer())
      {
         if (!mBoxMgr.isIconActive())
         {
            mGUI->showPointer();
         }
      }
      mBoxMgr.update();
   }
}

void BuildEditorUI::update(unsigned long lTimeElapsed)
{
   
}

void BuildEditorUI::show(bool lShow)
{
   if (!lShow)
   {
      /* Don't know which panel we're on so set them all hidden */
      mMenuPanelAdd->setVisible(false);
      mMenuPanelEdit->setVisible(false);
      mMenuBar->setVisible(false);
      mShow = false;
   }
   else
   {
      renderBuildingList(mBuildingPage);
      mMenuPanelAdd->setVisible(true);
      mMenuBar->setVisible(true);
      mShow = true;
      mGUI->showPointer();
   }
   mMenuBar->setEnabled(mShow);
   mMenuPanelEdit->setEnabled(mShow);
   mMenuPanelAdd->setEnabled(mShow);
}
