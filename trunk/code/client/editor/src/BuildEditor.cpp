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
   mHasEditorObj = false;
   mShow = false;
   mGUI = MyGUI::Gui::getInstancePtr();
   MyGUI::LayoutManager::getInstance().load("build_editor.layout");

   mCollision = new MOC::CollisionTools(Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr"));

   mMenuBar = mGUI->findWidget<MyGUI::StaticImage>("BuildEditorMenuTop");
   mMenuBar->setVisible(false);
   mMenuPanel = mGUI->findWidget<MyGUI::Widget>("BuildEditorMenuBottom");
   mMenuPanel->setVisible(false);
   Console::getSingletonPtr()->addCommand(Ogre::UTFString("cl_showeditor"), MyGUI::newDelegate(this, &BuildEditor::cmd_showEditor));

   MyGUI::ButtonPtr lButton = mGUI->findWidget<MyGUI::Button>("EditorButtonMinimise");
   lButton->eventMouseButtonClick = MyGUI::newDelegate(this, &BuildEditor::toggleMinimise);

   mRoot = Ogre::Root::getSingletonPtr();
   mRoot->createSceneManager(Ogre::ST_GENERIC,"EditorSceneMgr");
   mSceneMgr = mRoot->getSceneManager("EditorSceneMgr");
   mSceneMgr->setAmbientLight(Ogre::ColourValue(1,1,1));

   mlines = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
   SceneNode *linesNode = mRoot->getSceneManager("GameSceneMgr")->getRootSceneNode()->createChildSceneNode("lines");
   linesNode->attachObject(mlines);

   renderBuildingList();
   toggleShow(true);
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
   }
   else
   {
      mShow = true;
   }
   mMenuPanel->setVisible(mShow);
}

void BuildEditor::renderBuildingList(void)
{
   boost::filesystem::path lPath("../media/models/hangers");
	unsigned short x = 1;
   unsigned short y = 1;

	if (boost::filesystem::is_directory(lPath))
   {
	   for (boost::filesystem::directory_iterator itr(lPath); itr!=boost::filesystem::directory_iterator(); ++itr)
      {
	      Ogre::String temp = (Ogre::String) itr->path().leaf();
	      if (temp.substr(temp.length()-4, 4).compare("mesh") == 0)
         {
            Ogre::UTFString lPanelName = Ogre::UTFString("RenderBox")+Ogre::StringConverter::toString(x)+"_"+Ogre::StringConverter::toString(y);
	         renderMesh((Ogre::String) itr->path().leaf(), lPanelName);
            y++;
            if (y > 4)
            {
               x++;
               y=1;
            }
            if (x > 4)
            {
               /* Build Editor is full so stop */
               return;
            }
	      }
	   }
	}
}

void BuildEditor::renderMesh(const Ogre::UTFString lMesh, const Ogre::UTFString lPanelName)
{
   /* FIXME: We could do this better by moving some of this over to renderBuildingList
      instead of adding and removing it over and over */
   /* TODO: if entity exists, remove * */
   Ogre::SceneNode *lEditorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(lPanelName, Ogre::Vector3(0, 0, 0 ));   
   Ogre::Entity *lent = mSceneMgr->createEntity(lPanelName, lMesh);
   lent->setMaterialName("shader/diffuse");
   lEditorNode->attachObject(lent);

   Ogre::Camera *lCamera = mSceneMgr->createCamera("EditorCamera_"+lPanelName);
   lCamera->setPosition(Ogre::Vector3(0, 0+(lent->getBoundingBox().getSize().y*0.5), (lent->getBoundingBox().getSize().y)*1.2)+lent->getBoundingBox().getSize().z);
   lCamera->lookAt(Ogre::Vector3(0,0,0));

   Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual(lPanelName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
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
         toggleShow(show);
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

void BuildEditor::update(unsigned long lTimeElapsed)
{
   if (mShow && !mGUI->isShowPointer())
   {
      if (!mBoxMgr.isIconActive())
      {
         mGUI->showPointer();
      }
   }
   mBoxMgr.update();

   if (mHasEditorObj)
   {
      /* Kill scenenode */
      mEditorNode->detachObject("EditorObject");
      Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->destroyEntity("EditorObject");
      Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->destroySceneNode(mEditorNode);
      mHasEditorObj = false;
   }

   if (mBoxMgr.isPlaceable())
   {
      Ogre::Vector3 lResult = Ogre::Vector3::ZERO;
      unsigned long lTarget = 0;
      float lDistance = -1.0f;

      GameManager * lGameMgr = GameManager::getSingletonPtr();
      Ogre::Ray lmouseRay = lGameMgr->mCamera->getCameraToViewportRay(mBoxMgr.getPoint().left / Ogre::Real(lGameMgr->mViewport->getActualWidth()),
                                                                      mBoxMgr.getPoint().top  / Ogre::Real(lGameMgr->mViewport->getActualHeight()));

      Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
      mEditorNode = lSceneMgr->getRootSceneNode()->createChildSceneNode();
      lSceneMgr->getRootSceneNode()->needUpdate();
      if(mCollision->raycast(lmouseRay, lResult, lTarget, lDistance))
      {
         /* The object we're placing hasn't been loaded yet. */
         Ogre::Entity *lent = lSceneMgr->createEntity("EditorObject", mBoxMgr.getMeshName());
         mEditorNode->attachObject(lent);
         lResult.y += lent->getBoundingBox().getSize().y*0.5;
         mEditorNode->setPosition(lResult);
         mHasEditorObj = true;
      }

      mlines->clear();
      mlines->addPoint(lGameMgr->mCamera->getPosition());
      mlines->addPoint(lmouseRay.getPoint(1000000));
      mlines->update();

   }
}

void BuildEditor::toggleShow(bool lShow)
{
   Console* lConsole = Console::getSingletonPtr();
   if (!lShow)
   {
      mShow = false;
      mMenuPanel->setVisible(false);
      mMenuBar->setVisible(false);
   }
   else
   {
      if (lConsole->isVisible())
      {
         lConsole->toggleShow();
      }
      mShow = true;
      mMenuPanel->setVisible(true);
      mMenuBar->setVisible(true);
   }
}
