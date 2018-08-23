//-----------------------------------------------------------------------------
//
//@file  
//	UserUtils.h
//
//@date 
//	2009/12/14
//
//@author
//	Martin FLEURENT aka 'martignasse'
//
//@brief  
//	Utilities global functions
//
//@IMPORTANT
//	This file is part of the Usine CPP SDK Version 5
//
//  Please, report bugs and patch to Usine forum :
//  http://www.sensomusic.com/forums/ 
//  in the 'Module development SDK' section
//
//-----------------------------------------------------------------------------

// include once, no more
#ifndef _USERUTILS_H_
#define _USERUTILS_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include "UsineDefinitions.h"
#include <string>

const float PI = std::atan(float(1.0))*4; //MODS 'std::atanf' --> 'atanf' //MODS 'atanf(1.0)' --> std::atan(float(1.0))
const float COEFF_TO_DB_MIN = -200;
const float GAIN_MIN = -80; // db
const float LN10S20 = 0.115129254f;

const int MID_NOTE_ON   = 144;
const int MID_VOL_RANGE = 127;

//-----------------------------------------------------------------------------
// global color utilities functions
//-----------------------------------------------------------------------------

// Calculate HSL from RGB
// Hue is in degrees
// Lightness is between 0 and 1
// Saturation is between 0 and 1
TColorHSL ColorRGBtoHSL(TColorRGB c1);

// Calculate RGB from HSL, reverse of RGB2HSL()
// Hue is in degrees
// Lightness is between 0 and 1
// Saturation is between 0 and 1
TColorRGB ColorHSLtoRGB(TColorHSL c1);

// RGB color to Usine color format
TColorUSINE ColorRGBtoUSINE( TColorRGB c1 );

// Usine color format to RGB color
TColorRGB ColorUSINEtoRGB(TColorUSINE c1);

// HSL color to Usine color format
TColorUSINE ColorHSLtoUSINE(TColorHSL c1);

// Usine color format to HSL color
TColorHSL ColorUSINEtoHSL(TColorUSINE c1);

// RGB color constructor
TColorRGB ColorRGB(float r, float g, float b);

// HSL color constructor
// h : float between 0 and 360
// s : float between 0 and 1
// l : float between 0 and 1
// TColorHSL : the new created color
TColorHSL ColorHSL(float h, float s, float l);

// Usine color constructor
// r : float between 0 and 1
// g : float between 0 and 1
// b : float between 0 and 1
// TColorUSINE : the new created color
TColorUSINE ColorUSINE( float r, float g, float b );

// Usine color constructor
// r : int between 0 and 255
// g : float between 0 and 255
// b : float between 0 and 255
// TColorUSINE : the new created color
TColorUSINE ColorUSINE( int r, int g, int b );

//-----------------------------------------------------------------------------
// global geometric utilities functions
//-----------------------------------------------------------------------------

// Test if coordinate is in a rect bound
// X : x coordinates to test
// Y : y coordinates to test
// LeftTop : left top coordinates of the rect bound
// RightBottom : right bottom coordinates of the rect bound
bool isPointInRect(int X, int Y, TPointI LeftTop, TPointI RightBottom);

// Test if coordinate is in a rect bound
// Coords : coordinates to test
// LeftTop : left top coordinates of the rect bound
// RightBottom : right bottom coordinates of the rect bound
bool isPointInRect(TPointI Coords, TPointI LeftTop, TPointI RightBottom);

/*
   Four control point Bezier interpolation
   mu ranges from 0 to 1, start to end of curve
*/
TPointF Bezier4(TPointF p1, TPointF p2, TPointF p3, TPointF p4, float mu);
float Bezier4(float p1, float p2, float p3, float p4, float mu);

//-----------------------------------------------------------------------------
// global numeric utilities functions
//-----------------------------------------------------------------------------
//
float CoeffToDb(const float Coeff);
float DbToCoeff(const float Gain);

//
std::string FloatToString(const float Number, const int Prec=0, const int Width=0);

//
std::string IntToString(const int Number);

#endif // _USERUTILS_H_