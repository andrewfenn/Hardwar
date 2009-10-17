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

#include "BuildEditor.h"

using namespace Client;

BuildEditor::BuildEditor(void)
{
   mEditorObjCreated = false;
   mShow = false;
   mBuildingPage = 0;
   mGUI = MyGUI::Gui::getInstancePtr();
   MyGUI::LayoutManager::getInstance().load("build_editor.layout");

   mCollision = new MOC::CollisionTools(Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr"));

   mMenuBar = mGUI->findWidget<MyGUI::StaticImage>("BuildEditorMenuTop");
   mMenuBar->setVisible(false);
   mMenuPanelAdd = mGUI->findWidget<MyGUI::Widget>("BuildEditorMenuAdd");
   mMenuPanelAdd->setVisible(false);
   mMenuPanelEdit = mGUI->findWidget<MyGUI::Widget>("BuildEditorMenuEdit");
   mMenuPanelEdit->setVisible(false);

   // Add console command
   Console::getSingletonPtr()->addCommand(Ogre::UTFString("cl_showeditor"), MyGUI::newDelegate(this, &BuildEditor::cmd_showEditor));

   // Make the buttons function
   MyGUI::ButtonPtr lButton = mGUI->findWidget<MyGUI::Button>("EditorButtonMinimise");
   lButton->eventMouseButtonClick = MyGUI::newDelegate(this, &BuildEditor::toggleMinimise);

   lButton = mGUI->findWidget<MyGUI::Button>("BuildEditorMenuNext");
   lButton->eventMouseButtonClick = MyGUI::newDelegate(this, &BuildEditor::buttonNext);

   lButton = mGUI->findWidget<MyGUI::Button>("BuildEditorMenuPrevious");
   lButton->eventMouseButtonClick = MyGUI::newDelegate(this, &BuildEditor::buttonPrevious);

   mRoot = Ogre::Root::getSingletonPtr();
   mRoot->createSceneManager(Ogre::ST_GENERIC,"EditorSceneMgr");
   mSceneMgr = mRoot->getSceneManager("EditorSceneMgr");
   mSceneMgr->setAmbientLight(Ogre::ColourValue(1,1,1));

   generateBuildingList();

   renderBuildingList(0);
   show(false);
}

BuildEditor::~BuildEditor(void)
{
   /* TODO: unload build editor resources */
   delete mCollision;
}

void BuildEditor::toggleMinimise(MyGUI::WidgetPtr lWidget)
{
   if (mShow)
   {
      mShow = false;
      mGUI->hidePointer();
      mAxis.remove();
   }
   else
   {
      mShow = true;
   }
   mMenuPanelAdd->setVisible(mShow);
}

void BuildEditor::buttonNext(MyGUI::WidgetPtr lWidget)
{
   if (mBuildingPage == mBuildingMaxPage)
      return;

   renderBuildingList(++mBuildingPage);
}

void BuildEditor::buttonPrevious(MyGUI::WidgetPtr lWidget)
{
   if (mBuildingPage == 0)
      return;

   renderBuildingList(--mBuildingPage);
}

void BuildEditor::checkUIButtons(void)
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

void BuildEditor::generateBuildingList(void)
{
   boost::filesystem::path lPath("../media/models/hangers");
	unsigned short x = 1;
   unsigned short y = 1;
   unsigned short pageNum = 0;

   /* Loop over all files in the directory and if they are .mesh place them in our building list. */
	if (boost::filesystem::is_directory(lPath))
   {
	   for (boost::filesystem::directory_iterator itr(lPath); itr!=boost::filesystem::directory_iterator(); ++itr)
      {
	      Ogre::String temp = (Ogre::UTFString) itr->path().leaf();
	      if (temp.substr(temp.length()-4, 4).compare("mesh") == 0)
         {
            mBuildingList[pageNum].push_back((Ogre::UTFString) itr->path().leaf());
            y++;
            if (y > 4)
            {
               x++;
               y=1;
            }
            if (x > 4)
            {
               /* See have so many buildings per page so that when we click the arrow buttons
               on the UI we can go to another page. */
               pageNum++;
               x = 1;
               y = 1;
            }
	      }
	   }
      mBuildingMaxPage = pageNum;
	}
}

void BuildEditor::renderBuildingList(unsigned short pageNum)
{
   try
   {
      // Remove existing icons in the UI
      mBoxMgr.clear();
      Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("BuildEditorIcons");
   }
   catch (Ogre::Exception e) { }

   unsigned short x = 1;
   unsigned short y = 1;
   BuildingPage buildinglist = mBuildingList[pageNum];
   BuildingPage::iterator buildingItr; 
   for (buildingItr = buildinglist.begin(); buildingItr < buildinglist.end(); buildingItr++)
   {
      if (x > 4)
      {
         buildingItr = buildinglist.end();
      }
      else
      {
         Ogre::UTFString lPanelName = Ogre::UTFString("RenderBox")+Ogre::StringConverter::toString(x)+"_"+Ogre::StringConverter::toString(y);
	      renderMesh((Ogre::String)(*buildingItr), lPanelName);
      }
      y++;
      if (y > 4)
      {
         x++;
         y=1;
      }
   }
   checkUIButtons();
}

void BuildEditor::renderMesh(const Ogre::UTFString lMesh, const Ogre::UTFString lPanelName)
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

   mSceneMgr->destroyEntity(lPanelName);
   lEditorNode->removeAndDestroyAllChildren();
   renderTexture->setActive(false);
   renderTexture->removeAllViewports();
   mSceneMgr->destroyAllCameras();
   mSceneMgr->destroySceneNode(lPanelName);
}

void BuildEditor::cmd_showEditor(const Ogre::UTFString &key, const Ogre::UTFString &value)
{
   bool show = false;
   Console* lConsole = Console::getSingletonPtr();

   if (!MyGUI::utility::parseComplex(value, show))
   {
      if (!value.empty())
      {
         lConsole->addToConsole(lConsole->getConsoleError(), key, value);
      }
      lConsole->addToConsole(lConsole->getConsoleFormat(), key, "[true|false] - "+Ogre::UTFString(gettext("Show the hardwar editor")));
   }
   else
   {
      bool isAdmin = Ogre::StringConverter::parseBool(GameSettings::getSingletonPtr()->getOption("isAdmin"));
      if (isAdmin)
      {
         this->show(show);
         lConsole->addToConsole(lConsole->getConsoleSuccess(), key, Ogre::StringConverter::toString(show));
      }
      else
      {
         lConsole->addToConsole(lConsole->getConsoleError(), key, Ogre::UTFString(gettext("Not logged in as admin. (use rcon_password)")));
      }
   }
}

bool BuildEditor::isVisible()
{
   if (mBoxMgr.isIconActive())
   {
      return true;
   }
   return mShow;
}

void BuildEditor::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   if (id == OIS::MB_Left)
   {
      const OIS::MouseState &mouseState = e.state;
      if ((mouseState.X.abs > 360 || mouseState.Y.abs > 570) && mShow)
      {
         mAxis.selectBuilding(makeRay(mouseState.X.abs, mouseState.Y.abs));
      }
   }
}

void BuildEditor::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   mAxis.clearSelectedAxis();
}

void BuildEditor::mouseMoved(const OIS::MouseEvent &e)
{
   if (mAxis.getSelectedAxis() != 0 && mShow)
   {
      const OIS::MouseState &mouseState = e.state;
      mAxis.moveBuilding(makeRay(mouseState.X.abs, mouseState.Y.abs));
   }
}

void BuildEditor::keyReleased(const OIS::KeyEvent &e)
{
   if (e.key == OIS::KC_LSHIFT)
   {
      mAxis.showRotAxis(!mAxis.isRotAxis());
   }
}

Ogre::Ray BuildEditor::makeRay(const unsigned int x, const unsigned int y)
{
   Ogre::Camera* lCamera = GameManager::getSingletonPtr()->getCamera();
   Ogre::Viewport* lViewport = GameManager::getSingletonPtr()->getViewport();
   return lCamera->getCameraToViewportRay(x / Ogre::Real(lViewport->getActualWidth()), 
                                          y / Ogre::Real(lViewport->getActualHeight()));
}

void BuildEditor::update(unsigned long lTimeElapsed)
{
   if (mShow)
   {
      if (mAxis.objectSelected())
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

         if (mBoxMgr.isPlaceable())
         {
            Ogre::Vector3 lResult = Ogre::Vector3::ZERO;
            unsigned long lTarget = 0;
            float lDistance = -1.0f;

            GameManager * lGameMgr = GameManager::getSingletonPtr();
            Ogre::Ray lmouseRay = lGameMgr->getCamera()->getCameraToViewportRay(mBoxMgr.getPoint().left / Ogre::Real(lGameMgr->getViewport()->getActualWidth()),
                                                                            mBoxMgr.getPoint().top  / Ogre::Real(lGameMgr->getViewport()->getActualHeight()));

            Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");

            if(mCollision->raycast(lmouseRay, lResult, (unsigned long&)lTarget, lDistance, 0xFFFFFFFF))
            {
               Ogre::Entity* lEntity;
               if (mEditorObjCreated)
               {
                  lEntity = static_cast<Ogre::Entity*>(lSceneMgr->getSceneNode("EditorNode")->getAttachedObject("EditorObject"));
               }
               else
               {
                  mEditorNode = lSceneMgr->getRootSceneNode()->createChildSceneNode("EditorNode");
                  /* The object we're placing hasn't been loaded yet. */
                  mEditorObjMeshName = mBoxMgr.getMeshName();
                  lEntity = lSceneMgr->createEntity("EditorObject", mEditorObjMeshName);
                  lEntity->setQueryFlags(0x00000000);
                  mEditorNode->attachObject(lEntity);
                  mEditorObjCreated = true;
               }

               lResult.y += lEntity->getBoundingBox().getSize().y*0.5;
               /* We want ints because that's what is going in the db */
               lResult.x = floor(lResult.x);
               lResult.z = floor(lResult.z);
               lResult.y = floor(lResult.y);
               mEditorNode->setPosition(lResult);
            }
         }
         else
         {
            if (mEditorObjCreated)
            {
               /* We have just let go of an object and placed it. */
               HWBuilding lBuilding;
               lBuilding.position = mEditorNode->getPosition();
               lBuilding.mesh = mEditorObjMeshName;

               /* Kill the editor object */
               mEditorNode->detachObject("EditorObject");
               Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->destroyEntity("EditorObject");
               Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->destroySceneNode(mEditorNode);

               mEditorObjCreated = false;

               if (!mBoxMgr.isIconActive())
               {
                  /* Tell the server to place down a new building */
                  dataPacket lPacket = dataPacket(add_building);
                  lPacket.append(&lBuilding, sizeof(HWBuilding));
                  GameManager::getSingletonPtr()->getNetwork()->message(lPacket, SERVER_CHANNEL_ADMIN, ENET_PACKET_FLAG_RELIABLE);
               }
            }
         }
      }
   }
}

void BuildEditor::show(bool lShow)
{
   if (!lShow)
   {
      /* Don't know which panel we're on so set them all hidden */
      mMenuPanelAdd->setVisible(false);
      mMenuPanelEdit->setVisible(false);
      mMenuBar->setVisible(false);
      mAxis.remove();
      mShow = false;
   }
   else
   {
      mMenuPanelAdd->setVisible(true);
      mMenuBar->setVisible(true);
      mShow = true;
      mGUI->showPointer();
   }
}
