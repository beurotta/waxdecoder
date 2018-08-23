//-----------------------------------------------------------------------------
//
//@file  
//	UserUtils.cpp
//
//@date 
//	2009/12/14
//
//@author
//	Martin FLEURENT aka 'martignasse'
//
//@brief  
//	Utilities global functions implementation
//
//@IMPORTANT
//	This file is part of the Usine CPP SDK Version 5
//
//  Please, report bugs and patch to Usine forum :
//  http://www.sensomusic.com/forums/ 
//  in the 'Module development SDK' section
//
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------
#include <cmath> //MODS '<math.h>' --> <cmath>
#include <sstream>
#include <iomanip>
#include "UserUtils.h" //MODS moved down, especially after <cmath>

//-----------------------------------------------------------------------------
// global color utilities functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// calculate HSL from RGB
TColorHSL ColorRGBtoHSL(TColorRGB c1)
{
    float themin,themax,delta;
    TColorHSL c2;

    themin = std::min( c1.r, std::min(c1.g,c1.b) ); //MODSx2 : 'min' --> 'std::min'
    themax = std::max( c1.r, std::max(c1.g,c1.b) ); //MODSx2 : 'max' --> 'std::max'
    delta = themax - themin;
    c2.l = (themin + themax) / 2;
    c2.s = 0;
    if (c2.l > 0 && c2.l < 1)
        c2.s = delta / (c2.l < 0.5 ? (2*c2.l) : (2-2*c2.l));
    c2.h = 0;
    if (delta > 0) {
        if (themax == c1.r && themax != c1.g)
            c2.h += (c1.g - c1.b) / delta;
        if (themax == c1.g && themax != c1.b)
            c2.h += (2 + (c1.b - c1.r) / delta);
        if (themax == c1.b && themax != c1.r)
            c2.h += (4 + (c1.r - c1.g) / delta);
        c2.h *= 60;
    }
    
    while (c2.h < 0)
        c2.h += 360;
    while (c2.h > 360)
        c2.h -= 360;

    return c2;
}

//-----------------------------------------------------------------------------
// calculate RGB from HSL, reverse of RGB2HSL()
TColorRGB ColorHSLtoRGB(TColorHSL c1)
{
    TColorRGB c2,sat,ctmp;

    while (c1.h < 0)
        c1.h += 360;
    while (c1.h > 360)
        c1.h -= 360;

    if (c1.h < 120) {
        sat.r = (120 - c1.h) / 60.0f;
        sat.g = c1.h / 60.0f;
        sat.b = 0;
    } else if (c1.h < 240) {
        sat.r = 0;
        sat.g = (240 - c1.h) / 60.0f;
        sat.b = (c1.h - 120) / 60.0f;
    } else {
        sat.r = (c1.h - 240) / 60.0f;
        sat.g = 0;
        sat.b = (360 - c1.h) / 60.0f;
    }
    sat.r = std::min(sat.r,float(1)); //MODS : 'min' --> 'std::min' //MODS '1' --> 'float(1)'
    sat.g = std::min(sat.g,float(1)); //MODS : 'min' --> 'std::min' //MODS '1' --> 'float(1)'
    sat.b = std::min(sat.b,float(1)); //MODS : 'min' --> 'std::min' //MODS '1' --> 'float(1)'

    ctmp.r = 2 * c1.s * sat.r + (1 - c1.s);
    ctmp.g = 2 * c1.s * sat.g + (1 - c1.s);
    ctmp.b = 2 * c1.s * sat.b + (1 - c1.s);

    if (c1.l < 0.5) {
        c2.r = c1.l * ctmp.r;
        c2.g = c1.l * ctmp.g;
        c2.b = c1.l * ctmp.b;
    } else {
        c2.r = (1 - c1.l) * ctmp.r + 2 * c1.l - 1;
        c2.g = (1 - c1.l) * ctmp.g + 2 * c1.l - 1;
        c2.b = (1 - c1.l) * ctmp.b + 2 * c1.l - 1;
    }

    return c2;
}

//-----------------------------------------------------------------------------
// RGB color to Usine color format
TColorUSINE ColorRGBtoUSINE( TColorRGB c1 )
{
    TColorUSINE c2 = 0;

    c2 += ((int)(c1.r * 255.0f)) << 0;
    c2 += ((int)(c1.g * 255.0f)) << 8;
    c2 += ((int)(c1.b * 255.0f)) << 16;

    return c2;
}

//-----------------------------------------------------------------------------
// Usine color format to RGB color
TColorRGB ColorUSINEtoRGB(TColorUSINE c1)
{
    TColorRGB c2;

    c2.a = 1.0;
    c2.r = ((float)( c1        & 0x000000FF)) / 255.0f;
    c2.g = ((float)((c1 >> 8)  & 0x000000FF)) / 255.0f;
    c2.b = ((float)((c1 >> 16) & 0x000000FF)) / 255.0f;

    return c2;
}

//-----------------------------------------------------------------------------
// HSL color to Usine color format
TColorUSINE ColorHSLtoUSINE(TColorHSL c1)
{
    TColorUSINE c2 = ColorRGBtoUSINE( ColorHSLtoRGB( c1 ) );
    return c2;
}

//-----------------------------------------------------------------------------
// Usine color format to HSL color
TColorHSL ColorUSINEtoHSL(TColorUSINE c1)
{
    TColorHSL c2 = ColorRGBtoHSL( ColorUSINEtoRGB( c1 ) );
    return c2;
}

//-----------------------------------------------------------------------------
// RGB color constructor
TColorRGB ColorRGB(float r, float g, float b)
{
    TColorRGB c2;
    
    c2.a = 1.0;
    c2.r = r;
    c2.g = g;
    c2.b = b;

    return c2;
}

//-----------------------------------------------------------------------------
// HSL color constructor
TColorHSL ColorHSL(float h, float s, float l)
{
    TColorHSL c2;

    c2.a = 1.0;
    c2.h = h;
    c2.s = s;
    c2.l = l;

    return c2;
}

//-----------------------------------------------------------------------------
// Usine color constructor
TColorUSINE ColorUSINE( float r, float g, float b )
{
    TColorUSINE c2 = 0;

    c2 = ColorRGBtoUSINE(ColorRGB(r, g, b));

    return c2;
}

//-----------------------------------------------------------------------------
// Usine color constructor
TColorUSINE ColorUSINE( int r, int g, int b )
{
    TColorUSINE c2 = 0;

    //float fr, fg, fb;

    //fr = r/255;
    c2 = ColorRGBtoUSINE(ColorRGB(r / 255.f, g / 255.f, b / 255.f));

    return c2;
}

//-----------------------------------------------------------------------------
// global geometric utilities functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Test if coordinate is in a rect bound
bool isPointInRect(int X, int Y, TPointI LeftTop, TPointI RightBottom)
{
    TPointI Coords;
    Coords.x = X;	
    Coords.y = Y;	
    return isPointInRect(Coords, LeftTop, RightBottom);
}

//-----------------------------------------------------------------------------
// Test if coordinate is in a rect bound
bool isPointInRect(TPointI Coords, TPointI LeftTop, TPointI RightBottom)
{
    return ((Coords.x >= LeftTop.x) && (Coords.y >= LeftTop.y)
        && (Coords.x < RightBottom.x)
        && (Coords.y < RightBottom.y));
}

//-----------------------------------------------------------------------------
// Four control point Bezier interpolation
TPointF Bezier4(TPointF p1, TPointF p2, TPointF p3, TPointF p4, float mu)
{
    float mum1,mum13,mu3;
    TPointF p;

    mum1 = 1 - mu;
    mum13 = mum1 * mum1 * mum1;
    mu3 = mu * mu * mu;

    p.x = mum13*p1.x + 3*mu*mum1*mum1*p2.x + 3*mu*mu*mum1*p3.x + mu3*p4.x;
    p.y = mum13*p1.y + 3*mu*mum1*mum1*p2.y + 3*mu*mu*mum1*p3.y + mu3*p4.y;

    return(p);
}

//
float Bezier4(float p1, float p2, float p3, float p4, float mu)
{
    float mum1,mum13,mu3;
    float p;

    mum1 = 1 - mu;
    mum13 = mum1 * mum1 * mum1;
    mu3 = mu * mu * mu;

    p= mum13*p1 + 3*mu*mum1*mum1*p2 + 3*mu*mu*mum1*p3 + mu3*p4;

    return(p);
}

//
float CoeffToDb(const float Coeff)
{
    float result = COEFF_TO_DB_MIN;

    if (Coeff > 0.0f)
        result = log10f(Coeff) / LN10S20;

    return result;
}

float DbToCoeff(const float Gain)
{
    float result;
    if (Gain == 0.0f)
        result = 1.0f;
    else if (Gain > GAIN_MIN)
        result = exp(Gain * LN10S20);
    return result;
}

//
std::string FloatToString(const float Number, const int Prec, const int Width)
{
    std::ostringstream os;
    os << std::setiosflags(std::ios::fixed) << std::setprecision(Prec) << Number; // << std::setw(Width) 
    return os.str();
}

//
std::string IntToString(const int Number)
{
    std::ostringstream os;
    os << Number; // << std::setw(Width) 
    return os.str();
}
