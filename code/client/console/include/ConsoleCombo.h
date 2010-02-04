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

#ifndef __CONSOLE_COMBO_H__
#define __CONSOLE_COMBO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_ComboBox.h"

namespace MyGUI
{
   namespace factory	{ class ConsoleComboBoxFactory; }

   class MYGUI_EXPORT ConsoleComboBox : public ComboBox
   {
	   friend class factory::BaseWidgetFactory<ConsoleComboBox>;      MYGUI_RTTI_CHILD_HEADER( ConsoleComboBox, Edit );
      public:
         ConsoleComboBox(const ComboBox& _combo) : ComboBox(_combo) { };
         ConsoleComboBox(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info,
                         WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, 
                        const std::string & _name) : 
            ComboBox(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name) { };
         virtual ~ConsoleComboBox(void) {}
         friend void show(ComboBox _combo);
      protected:
      //   void onKeyButtonPressed(KeyCode _key, Char _char);
   };

   typedef ConsoleComboBox * ConsoleComboBoxPtr;

   namespace factory
   {
      class MYGUI_EXPORT ConsoleComboBoxFactory : public BaseWidgetFactory<ConsoleComboBox>
      {
         public:
            ConsoleComboBoxFactory(void)
            {
               MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
               manager.registerDelegate("ConsoleComboBox_ModeDrop") = newDelegate(this, &ConsoleComboBoxFactory::ComboBox_ModeDrop);
               manager.registerDelegate("ConsoleComboBox_AddItem") = newDelegate(this, &ConsoleComboBoxFactory::ComboBox_AddItem);
            }

            ~ConsoleComboBoxFactory(void)
            {
               MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
               manager.unregisterDelegate("ConsoleComboBox_ModeDrop");
               manager.unregisterDelegate("ConsoleComboBox_AddItem");
            }

            void ComboBox_ModeDrop(WidgetPtr _widget, const std::string &_key, const std::string &_value)
            {
               if (isFalseType(_widget, _key)) return;
                  static_cast<ConsoleComboBoxPtr>(_widget)->setComboModeDrop(utility::parseBool(_value));
            }

            void ComboBox_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value)
            {
               if (isFalseType(_widget, _key)) return;
                  static_cast<ConsoleComboBoxPtr>(_widget)->addItem(_value);
            }      };
   } // namespace factory
}
#endif /* __CONSOLE_COMBO_H__ */
