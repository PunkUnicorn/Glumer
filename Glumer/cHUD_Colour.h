// cHUD_Colour.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CHUD_COLOUR_H
#define CHUD_COLOUR_H

namespace Glumer
{

class cHUD_Colour
{
	private:
		inline int GetAltColour(int original, int addition)
		{
			if (original + addition > 255)
				return original - addition;

			return original + addition;
		};

    public:
		int GetSecondRed(void) { return GetAltColour(m_red, 100);  }
		int GetSecondGreen(void) { return GetAltColour(m_green, 100); }
		int GetSecondBlue(void) { return GetAltColour(m_blue, 100); }
		int m_red, m_green, m_blue;
        cHUD_Colour(void) : m_red(0), m_green(0), m_blue(0) {}
};

}

#endif