//----------------------------------------------------------------------------
//
//@file  
//	UserGui.h
//
//@date 
//	2009/12/14
//
//@author
//	Martin FLEURENT aka 'martignasse'
//
//@brief  
//	Gui components for user module
//
//@IMPORTANT
//	This file is part of the Usine CPP SDK Version 5
//
//  Please, report bugs and patch to Usine forum :
//  http://www.sensomusic.com/forums/ 
//  in the 'Module development SDK' section
//
//----------------------------------------------------------------------------

// include once, no more
#ifndef _USERGUI_H_
#define _USERGUI_H_

//----------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------
#include "UserModule.h"
#include <gdiplus.h>

//MODS suppress '#pragma comment(lib,"Gdiplus.lib")', need to include -lgdi32 -lgdiplus to linkage flags

//-----------------------------------------------------------------------------
// class to manage drawing attributes
//-----------------------------------------------------------------------------
class TGuiPaint
{
    //-------------------------------------------------------------------------
    // public members
    //-------------------------------------------------------------------------
public:

	// text vertical alignement
    enum VerticalAlign {
        vaTop,
        vaCenter,
        vaBottom
    };

	// text horizontal alignement
    enum HorizontalAlign {
        haLeft,
        haCenter,
        haRight
    };

    TColorUSINE FillColor;
    TColorUSINE OutlineColor;
    int OutlineThickness;
    int TextSize;
    float TextOrientation;
    TColorUSINE TextColor;
	VerticalAlign TextVertAlign;
	HorizontalAlign TextHorizAlign;
    HFONT Font;
    // TODO : other attributes for string, like style...

    //-------------------------------------------------------------------------
    // public constructors/destructors
    //-------------------------------------------------------------------------
public:

    // constructor
    TGuiPaint(); 

    // destructor
    ~TGuiPaint();

}; // class TGuiPaint

//-----------------------------------------------------------------------------
// class to manage UI drawing
//-----------------------------------------------------------------------------
class TGuiCanvas 
{
    //-------------------------------------------------------------------------
    // public constructors/destructors
    //-------------------------------------------------------------------------
public:

    // constructor
    TGuiCanvas(TUserModule* UserModule, HDC DC ); 

    // destructor
    ~TGuiCanvas();

    //-------------------------------------------------------------------------
    // public methodes
    //-------------------------------------------------------------------------
public:

    // Start a Paint session
    HDC BeginPaint(TUserModule* UserModule, HDC DC);
    
    // Close a paint session
    void EndPaint(HDC DC);
    
    //-------------------------------------------------------------------------
    // Draw methodes 	
    
    // Draw a line on the canvas from coeff coords	
    void DrawLine( TPointF StartPoint, TPointF EndPoint, TGuiPaint Paint );

    // Draw a line on the canvas from local coords	
    void DrawLine( TPointI StartPoint, TPointI EndPoint, TGuiPaint Paint );
        
    // Draw  a serie off connected lines on the canvas from coeff coords
    void DrawPolyLines( TPointF Points[], int Size, bool Closed, TGuiPaint Paint );
        
    // Draw  a serie off connected lines on the canvas from local coords
    void DrawPolyLines( TPointI Points[], int Size, bool Closed, TGuiPaint Paint );
    
    // Draw  a rectangle on the canvas from coeff coords
    void DrawRectangle( TPointF LeftTop, TPointF RightBottom, TGuiPaint Paint );
    
    // Draw  a rectangle on the canvas from local coords
    void DrawRectangle( TPointI LeftTop, TPointI RightBottom, TGuiPaint Paint );
    
    // Draw  a rounded rectangle on the canvas from coeff coords
    void DrawRoundedRectangle( TPointF LeftTop, TPointF RightBottom, int Radius, TGuiPaint Paint );

    // Draw  a rounded rectangle on the canvas from coeff coords
    void DrawRoundedRectangle( TPointI LeftTop, TPointI RightBottom, int Radius, TGuiPaint Paint );
    
    // Draw  a serie off connected lines on the canvas from local coords
    void DrawPath(Gdiplus::GraphicsPath* path, TGuiPaint Paint );

    // TODO : make Radius a coeff value
    // Draw  a circle on the canvas	from coeff coords
    void DrawCircle( TPointF CenterPoint, int Radius, TGuiPaint Paint );
    
    // Draw  a circle on the canvas	from local coords
    void DrawCircle( TPointI CenterPoint, int Radius, TGuiPaint Paint );
    
    // TODO : make implementation
    // Draw  an ellipse on the canvas
    //void DrawEllipse();
    
    // TODO : complete implementation and paint attribs for strings
    // Draw  a text line on the canvas
    void DrawString( PCHAR Text, TGuiPaint Paint);
	void DrawString( TPointF LeftTop, TPointF RightBottom, PCHAR Text, TGuiPaint Paint );
    void DrawString( TPointI LeftTop, TPointI RightBottom, PCHAR Text, TGuiPaint Paint );

    //-------------------------------------------------------------------------
    // Accessors methodes
    
    // Memory device context getter
    HDC GetBackBufferDC();

    // client area rectangle setter
    void SetClientAreaRect();

    // client area width getter
    int GetClientAreaWidth();

    // client area height getter
    int GetClientAreaHeight();

    //-------------------------------------------------------------------------
    // Coordinates helpers methodes 
    
    // Convert parent to local
    TPointI CoordParentToLocal(TPointI Coords);

    // Convert parent to local
    TPointI CoordParentToLocal(int x, int y);

    // Convert parent to coeff
    TPointF CoordParentToCoeff(TPointI Coords);

    // Convert parent to coeff
    TPointF CoordParentToCoeff(int x, int y);

    // Convert local to coeff 
    TPointF CoordLocalToCoeff(TPointI Coords);

    // Convert local to coeff
    TPointF CoordLocalToCoeff(int x, int y);

    // Convert coeff to local
    TPointI CoordCoeffToLocal(TPointF Coords);

    // Convert coeff to local
    TPointI CoordCoeffToLocal(float x, float y);
    
    void GetRoundRectPath(Gdiplus::GraphicsPath *pPath, Gdiplus::Rect r, int dia);
    //-------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------
protected:
	void DrawRotatedString( TPointI LeftTop, TPointI RightBottom, PCHAR Text, TGuiPaint Paint );

    //-------------------------------------------------------------------------
    // protected members
    //-------------------------------------------------------------------------
protected:

    // handle to device context and clientArea bitmap
    HBITMAP hbmpCanvas;
    HDC memDC;

    // Pointer to the TUserModule class
    TUserModule* m_userModule;

    // Parent coordinates of the client area
    TPointI m_clientAreaLeftTop;
    TPointI m_clientAreaWidthHeight;
    
    // Infos on the parent area
    // Used to calculate the dimension of the client area
    int m_borderWidth;
    int m_captionHeight;

    // Colorset howned by Usine
    TUsineColorSet* m_usineColorSet;

	// for GDI+ implementation
	static WCHAR* m_strBuffer; 
	static Gdiplus::Font* m_txtFont;

}; // class TGuiCanvas

#endif // _USERGUI_H_