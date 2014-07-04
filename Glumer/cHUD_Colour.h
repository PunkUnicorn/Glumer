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
    public:
        int m_red, m_green, m_blue;
        cHUD_Colour(void) : m_red(0), m_green(0), m_blue(0) {}
};

}

#endif