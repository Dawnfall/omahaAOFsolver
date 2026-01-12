#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_File_Chooser.H>
#include "UI/RenderCursor.h"
#include "Core.h"

namespace RenderUtils
{
	inline Fl_Color GetCardColor(phevaluator::Card card)
	{
		char suit = card.describeSuit();

		if (suit == 's')
			return fl_rgb_color(120, 120, 120);
		if (suit == 'd')
			return fl_rgb_color(128, 128, 255);
		if (suit == 'c')
			return fl_rgb_color(128, 255, 128);
		if (suit == 'h')
			return fl_rgb_color(255, 128, 128);
		return FL_BLACK;
	}

	inline Fl_Check_Button* CreateCheckButton(RenderCursor& cursor, int w, int h, bool isOn)
	{
		Fl_Check_Button* checkbox = new Fl_Check_Button(cursor.posX, cursor.posY, w, h);
		checkbox->value(isOn);
		cursor.MoveX(w, h);
		return checkbox;
	}

	inline void CreateAlert(const std::string& msg)
	{
		//fl_message TODO
		fl_alert(msg.c_str());
	}

	inline void  OpenFileDialog(const std::string& startFolder, const std::string& ext, std::function<void(const std::string&)> onFileSelected)
	{
		std::string filter = "*." + ext;

		// Open a file chooser dialog to select files with ".txt" extension
		const char* filePath = fl_file_chooser("Select a File", filter.c_str(), (startFolder != "") ? startFolder.c_str() : nullptr);

		if (filePath)
		{
			onFileSelected(filePath);
		}
	}


}