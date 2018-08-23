//----------------------------------------------------------------------------
//
//@file  
//	UserGui.cpp
//
//@date 
//	2009/12/14
//
//@author
//	Martin FLEURENT aka 'martignasse'
//
//@brief  
//	TGuiPaint and TGuiCanvas implementation
//
//@IMPORTANT
//	This file is part of the Usine CPP SDK Version 5
//
//  Please, report bugs and patch to Usine forum :
//  http://www.sensomusic.com/forums/ 
//  in the 'Module development SDK' section
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------
#include "UserGui.h"
#include "UserUtils.h"

using namespace Gdiplus;


WCHAR* TGuiCanvas::m_strBuffer = (WCHAR *)malloc( sizeof( wchar_t ) * 256);
Font* TGuiCanvas::m_txtFont = new Font(L"Arial", 16);
//-----------------------------------------------------------------------------
// TGuiPaint : constructors/destructors implémentation 
//-----------------------------------------------------------------------------
// constructor
TGuiPaint::TGuiPaint() 
{
    FillColor = OutlineColor = ColorUSINE(128, 128, 128);
    OutlineThickness = 0;
    TextSize = 14;
    TextOrientation = 0.0f;
}

// destructor
TGuiPaint::~TGuiPaint() {}

//-----------------------------------------------------------------------------
// TGuiCanvas : constructors/destructors implémentation 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// constructor
TGuiCanvas::TGuiCanvas(TUserModule* UserModule, HDC DC ) 
{
    hbmpCanvas = NULL;
    memDC = NULL;
    this->m_userModule = UserModule;

    m_clientAreaLeftTop.x = 0;
    m_clientAreaLeftTop.y = 0;
    m_clientAreaWidthHeight.x = 0;
    m_clientAreaWidthHeight.y = 0;
    
    m_borderWidth = 0;
    m_captionHeight = 0;

    m_usineColorSet = this->m_userModule->GetUsineColorSet();
}

//-----------------------------------------------------------------------------
// destructor
TGuiCanvas::~TGuiCanvas() 
{
	if ( hbmpCanvas != NULL) 
    { 
        DeleteObject(hbmpCanvas); 
    }
    
    if ( memDC != NULL) 
    { 
        DeleteObject(memDC); 
    }
}

//-----------------------------------------------------------------------------
// TGuiPaint : methodes implémentation 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Start a Paint session
HDC TGuiCanvas::BeginPaint(TUserModule* m_userModule, HDC DC)
{
    if ( hbmpCanvas != NULL) 
    { 
        DeleteObject(hbmpCanvas); 
    }

    hbmpCanvas = CreateCompatibleBitmap(DC, m_clientAreaWidthHeight.x, m_clientAreaWidthHeight.y);
    
    if ( memDC != NULL) 
    { 
        DeleteObject(memDC); 
    }		
    memDC = CreateCompatibleDC ( DC );
        
    // copy background in backbuffer
    SelectObject ( memDC, hbmpCanvas );
    BitBlt(memDC, 0, 0, m_clientAreaWidthHeight.x, m_clientAreaWidthHeight.y, DC, m_clientAreaLeftTop.x, m_clientAreaLeftTop.y, SRCCOPY);

    return memDC;
}

//-----------------------------------------------------------------------------
// Close a paint session
void TGuiCanvas::EndPaint(HDC DC)
{
    // Copy backbuffer on front buffer
    BitBlt(DC, m_clientAreaLeftTop.x, m_clientAreaLeftTop.y, m_clientAreaWidthHeight.x, m_clientAreaWidthHeight.y, memDC, 0, 0, SRCCOPY);
}

//-----------------------------------------------------------------------------
// Draw methodes 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Draw a line on the canvas from coeff coords
void TGuiCanvas::DrawLine( TPointF StartPoint, TPointF EndPoint, TGuiPaint Paint )
{
    DrawLine( CoordCoeffToLocal(StartPoint), CoordCoeffToLocal(EndPoint), Paint );
}

//-----------------------------------------------------------------------------
// Draw a line on the canvas from local coords
void TGuiCanvas::DrawLine( TPointI StartPoint, TPointI EndPoint, TGuiPaint Paint )
{
    // Create GDI objects
    HPEN penOutline = CreatePen(PS_SOLID, Paint.OutlineThickness, Paint.OutlineColor);

    // select GDI obj
    SelectObject(memDC, penOutline);

    MoveToEx(memDC, StartPoint.x, StartPoint.y, NULL);
    LineTo( memDC, EndPoint.x, EndPoint.y);

    // Free GDI objects
    DeleteObject(penOutline);
}

//-----------------------------------------------------------------------------
// Draw a line on the canvas from local coords
void TGuiCanvas::DrawPolyLines( TPointF Points[], int Size, bool Closed, TGuiPaint Paint )
{
    if (Size >=2)
    {
        for (int i = 0; i < Size - 1; i++)
        {
            DrawLine( Points[i], Points[i+1], Paint );
        }
        if (Closed)
            DrawLine( Points[Size - 1], Points[0], Paint );
    }
}

//-----------------------------------------------------------------------------
// Draw a line on the canvas from local coords
void TGuiCanvas::DrawPolyLines( TPointI Points[], int Size, bool Closed, TGuiPaint Paint )
{
    if (Size >=2)
    {
        for (int i = 0; i < Size - 1; i++)
        {
            DrawLine( Points[i], Points[i+1], Paint );
        }
        if (Closed)
            DrawLine( Points[Size - 1], Points[0], Paint );
    }
}

//-----------------------------------------------------------------------------
// Draw a rectangle on the canvas from coeff coords
void TGuiCanvas::DrawRectangle( TPointF LeftTop, TPointF RightBottom, TGuiPaint Paint )
{
    DrawRectangle( CoordCoeffToLocal(LeftTop), CoordCoeffToLocal(RightBottom), Paint );
}

//-----------------------------------------------------------------------------
// Draw a rectangle on the canvas from local coords
void TGuiCanvas::DrawRectangle( TPointI LeftTop, TPointI RightBottom, TGuiPaint Paint )
{
    // GDI objects
    HBRUSH brushFill = CreateSolidBrush( Paint.FillColor );
    
    TColorUSINE color;
    int outline;

    // GDI will draw a 1 pixel outline if outline = 0
    // so, we have to draw it with the fill color 
    if (Paint.OutlineThickness == 0)
    {
        color = Paint.FillColor;
        outline = 0;
    }
    else
    {
        color = Paint.OutlineColor;
        outline = (int)std::min( (float)Paint.OutlineThickness, 1 + std::min(0.5f *(RightBottom.y - LeftTop.y), 0.5f*(RightBottom.x - LeftTop.x)) ); //MODS 'min' --> 'std::min' //MODS 'Paint.OutlineThickness' --> '(float)Paint.OutlineThickness'
    }

    HPEN penOutline = CreatePen(PS_INSIDEFRAME, outline, color);

    // select colors
    SelectObject(memDC, brushFill);
    SelectObject(memDC, penOutline);

    Rectangle( memDC, LeftTop.x, LeftTop.y, RightBottom.x, RightBottom.y );

    DeleteObject(brushFill);
    DeleteObject(penOutline);
}

//-----------------------------------------------------------------------------
// Draw a rounded rectangle on the canvas from coeff coords
void TGuiCanvas::DrawRoundedRectangle( TPointF LeftTop, TPointF RightBottom, int Radius, TGuiPaint Paint )
{
    DrawRoundedRectangle( CoordCoeffToLocal(LeftTop), CoordCoeffToLocal(RightBottom), Radius, Paint );
}

//-----------------------------------------------------------------------------
// Draw a rounded rectangle on the canvas from local coords
void TGuiCanvas::DrawRoundedRectangle( TPointI LeftTop, TPointI RightBottom, int Radius, TGuiPaint Paint )
{
	int dia	= 2*Radius;
    Color col;
    int width = Paint.OutlineThickness;
    Rect r(LeftTop.x + (width/2), LeftTop.y + (width/2), RightBottom.x - LeftTop.x - width, RightBottom.y - LeftTop.y - width );

    Graphics pGraphics(memDC);
    pGraphics.SetSmoothingMode(SmoothingModeHighQuality);

	// set to pixel mode
	int oldPageUnit = pGraphics.SetPageUnit(UnitPixel);

	// get the corner path
	GraphicsPath path;

	// get path
	GetRoundRectPath(&path, r, dia);

    col.SetFromCOLORREF(Paint.FillColor);
    SolidBrush sbr(col);

	// fill
	pGraphics.FillPath(&sbr, &path);

	// draw the outline
    if (width > 0)
    {
        // define the pen
        col.SetFromCOLORREF(Paint.OutlineColor);
        Pen pen(col, (float)width);	

        // draw the border last so it will be on top in case the color is different
	    pGraphics.DrawPath(&pen, &path);
    }

	// restore page unit
	pGraphics.SetPageUnit((Unit)oldPageUnit);
    
}


// Draw  a serie off connected lines on the canvas from local coords
void TGuiCanvas::DrawPath( Gdiplus::GraphicsPath* path, TGuiPaint Paint )
{
    Graphics pGraphics(memDC);
    pGraphics.SetSmoothingMode(SmoothingModeHighQuality);

	// set to pixel mode
	int oldPageUnit = pGraphics.SetPageUnit(UnitPixel);
	 
	Color col;
    col.SetFromCOLORREF(Paint.FillColor);
    SolidBrush sbr(col);
	
    int width = Paint.OutlineThickness;

	// fill
	pGraphics.FillPath(&sbr, path);

	// draw the outline
    if (width > 0)
    {
        // define the pen
        col.SetFromCOLORREF(Paint.OutlineColor);
        Pen pen(col, (float)width);	

        // draw the border last so it will be on top in case the color is different
	    pGraphics.DrawPath(&pen, path);
    }

	// restore page unit
	pGraphics.SetPageUnit((Unit)oldPageUnit);
    
}

//-----------------------------------------------------------------------------
// Draw a circle on the canvas from coeff coords
void TGuiCanvas::DrawCircle( TPointF CenterPoint, int Radius, TGuiPaint Paint )
{
    DrawCircle( CoordCoeffToLocal(CenterPoint), Radius, Paint );
}

//-----------------------------------------------------------------------------
// Draw a circle on the canvas from local coords
void TGuiCanvas::DrawCircle( TPointI CenterPoint, int Radius, TGuiPaint Paint )
{
    // GDI objects
    HBRUSH brushFill = CreateSolidBrush( Paint.FillColor );
    
    TColorUSINE color;
    int outline;

    // GDI will draw a 1 pixel outline if outline = 0
    // so, we have to draw it with the fill color 
    if (Paint.OutlineThickness == 0)
    {
        color = Paint.FillColor;
        outline = 0;
    }
    else
    {
        color = Paint.OutlineColor;
        outline = std::min( Paint.OutlineThickness, 1 + Radius ); //MODS 'min' --> 'std::min'
    }

    HPEN penOutline = CreatePen(PS_INSIDEFRAME, outline, color);

    // select colors
    SelectObject(memDC, brushFill);
    SelectObject(memDC, penOutline);

    Ellipse( memDC, CenterPoint.x + 1 - Radius, CenterPoint.y + 1 - Radius, CenterPoint.x + Radius, CenterPoint.y + Radius );

    DeleteObject(brushFill);
    DeleteObject(penOutline);
}

//-----------------------------------------------------------------------------
// Draw a text line on the canvas
void TGuiCanvas::DrawString( PCHAR Text,  TGuiPaint Paint )
{
    TPointI LeftTop;

	LeftTop.x = 0;
	LeftTop.y = 0;

    DrawString( LeftTop, this->m_clientAreaWidthHeight, Text, Paint );
}

//-----------------------------------------------------------------------------
// Draw a text line on the canvas from coeff coords
void TGuiCanvas::DrawString( TPointF LeftTop, TPointF RightBottom, PCHAR Text, TGuiPaint Paint )
{
    DrawString( CoordCoeffToLocal(LeftTop), CoordCoeffToLocal(RightBottom), Text, Paint );
}
//-----------------------------------------------------------------------------
// Draw a text line on the canvas from local coords
void TGuiCanvas::DrawString( TPointI LeftTop, TPointI RightBottom, PCHAR Text, TGuiPaint Paint )
{
	// special version with gdiplus for rotated text
    if (Paint.TextOrientation != 0.0f)
	{
		DrawRotatedString( LeftTop, RightBottom, Text, Paint );
		return;
	}
	
	RECT rect;
    SetRect(&rect, LeftTop.x, LeftTop.y, RightBottom.x, RightBottom.y);

    HFONT hf;
    long lfHeight;

    // create font with appropriate size
    lfHeight = -MulDiv(Paint.TextSize, GetDeviceCaps(memDC, LOGPIXELSY), 72 );
    hf = CreateFont(lfHeight, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");

    // select the font and store the old one
    SelectObject(memDC, hf);

    SetTextColor(memDC, Paint.TextColor);

    SetBkMode(memDC, TRANSPARENT);

	UINT style;

	if (Paint.TextVertAlign == TGuiPaint::vaCenter)
	{
		style = DT_CENTER | DT_SINGLELINE | DT_VCENTER;
	}
	else
	{
		style = DT_CENTER | DT_SINGLELINE | DT_TOP;
	}

    // Display the Number. 
    DrawText(memDC, (LPSTR)Text, -1, &rect, style ); 

    // free GDI objects
    DeleteObject(hf);
}

//-----------------------------------------------------------------------------
// Accessors methodes 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Memory device context getter
HDC TGuiCanvas::GetBackBufferDC()
{
    return memDC;
}	

//-----------------------------------------------------------------------------
// client area rectangle setter
void TGuiCanvas::SetClientAreaRect()
{
    m_borderWidth = m_userModule->GetBorderWidth();
    m_captionHeight = m_userModule->GetCaptionHeight();
   
    // compute the client area
    m_clientAreaLeftTop.x = m_borderWidth;
    m_clientAreaLeftTop.y = m_captionHeight;
    m_clientAreaWidthHeight.x = m_userModule->m_moduleWidth - (m_borderWidth*2);
    m_clientAreaWidthHeight.y = m_userModule->m_moduleHeight - m_captionHeight - m_borderWidth;
}

//-----------------------------------------------------------------------------
// client area width getter
int TGuiCanvas::GetClientAreaWidth()
{
    return m_clientAreaWidthHeight.x;
}

//-----------------------------------------------------------------------------
// client area height getter
int TGuiCanvas::GetClientAreaHeight()
{
    return m_clientAreaWidthHeight.y;
}
    
//-----------------------------------------------------------------------------
// Coordinates helpers methodes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Convert parent to local 
TPointI TGuiCanvas::CoordParentToLocal(TPointI Coords)
{
    return CoordParentToLocal(Coords.x, Coords.y);
}

//-----------------------------------------------------------------------------
// Convert parent to local 
TPointI TGuiCanvas::CoordParentToLocal(int x, int y)
{
    TPointI coordLocal;

    // Translate to the local origin
    coordLocal.x = x - m_clientAreaLeftTop.x;
    coordLocal.y = y - m_clientAreaLeftTop.y;

    return coordLocal;
}

//-----------------------------------------------------------------------------
// Convert parent to coeff 
TPointF TGuiCanvas::CoordParentToCoeff(TPointI Coords)
{
    return CoordParentToCoeff(Coords.x, Coords.y);
}

//-----------------------------------------------------------------------------
// Convert parent to coeff 
TPointF TGuiCanvas::CoordParentToCoeff(int x, int y)
{
    TPointI coordLocal = CoordParentToLocal(x, y);

    return CoordLocalToCoeff(coordLocal);
}

//-----------------------------------------------------------------------------
// Convert local to coeff 
TPointF TGuiCanvas::CoordLocalToCoeff(TPointI Coords)
{
    return CoordLocalToCoeff(Coords.x, Coords.y);
}

//-----------------------------------------------------------------------------
// Convert local to coeff 
TPointF TGuiCanvas::CoordLocalToCoeff(int x, int y)
{
    TPointF coordCoeff;
    
    // down to 0->1 coeff
    coordCoeff.x = ((float)x) / (float)m_clientAreaWidthHeight.x;
    coordCoeff.y = ((float)y) / (float)m_clientAreaWidthHeight.y;

    return coordCoeff;
}

//-----------------------------------------------------------------------------
// Convert coeff to local
TPointI TGuiCanvas::CoordCoeffToLocal(TPointF Coords)
{
    return CoordCoeffToLocal(Coords.x, Coords.y);;
}

//-----------------------------------------------------------------------------
// Convert coeff to local
TPointI TGuiCanvas::CoordCoeffToLocal(float x, float y)
{
    TPointI coordLocal;

    // multiply by area dimensions
    coordLocal.x = (int)(x * m_clientAreaWidthHeight.x);
    coordLocal.y = (int)(y * m_clientAreaWidthHeight.y);

    return coordLocal;
}

//-----------------------------------------------------------------------------
//
// GetRoundRectPath()
//
// Purpose:     Defines a Rounded Rectangle and places it in the GraphicsPath
//
// Parameters:  pPath		- [out] pointer to GraphicsPath that will recieve the 
//									path data
//				r			- [in]	Rect that defines the round rectangle boundaries
//				dia			- [in]	diameter of the rounded corners (2*radius)
//
// Returns:     None
//
void TGuiCanvas::GetRoundRectPath(GraphicsPath *pPath, Rect r, int dia)
{
	// diameter can't exceed width or height
    if(dia + 2 > r.Width)
    {	
        dia = r.Width - 2;
    }
	if(dia + 2 > r.Height)
    {
        dia = r.Height - 2;
    }

	// define a corner 
	Rect Corner(r.X, r.Y, dia, dia);

	// begin path
	pPath->Reset();

	// top left
	pPath->AddArc(Corner, 180, 90);	

	// tweak needed for radius of 10 (dia of 20)
	if(dia == 20)
	{
		Corner.Width += 1; 
		Corner.Height += 1; 
		r.Width -=1; r.Height -= 1;
	}

	// top right
	Corner.X += (r.Width - dia);
	pPath->AddArc(Corner, 270, 90);	
	
	// bottom right
	Corner.Y += (r.Height - dia);
	pPath->AddArc(Corner,   0, 90);	
	
	// bottom left
	Corner.X -= (r.Width - dia);
	pPath->AddArc(Corner,  90, 90);

	// end path
	pPath->CloseFigure();

}

    
//-------------------------------------------------------------------------
// protected methods
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Draw a text line on the canvas from local coords
void TGuiCanvas::DrawRotatedString( TPointI LeftTop, TPointI RightBottom, PCHAR Text, TGuiPaint Paint )
{
	Color col;
	col.SetFromCOLORREF(Paint.TextColor);
	SolidBrush sbr(col);
	  
	const PointF origin(0, 0);
	const RectF layoutRect(
		  (Gdiplus::REAL)LeftTop.x
		, (Gdiplus::REAL)LeftTop.y
		, (Gdiplus::REAL)(RightBottom.x - LeftTop.x)
		, (Gdiplus::REAL)(RightBottom.y - LeftTop.y)
		);

	if (m_txtFont->GetSize() != Paint.TextSize)
	{
		delete m_txtFont;
		m_txtFont = new Font(L"Arial", (REAL)Paint.TextSize);
	}

	StringFormat stringFormat;
	
    // vertical alignment
	if (Paint.TextVertAlign == TGuiPaint::vaCenter)
	{
		stringFormat.SetLineAlignment(StringAlignmentCenter);
	}
	else if (Paint.TextVertAlign == TGuiPaint::vaTop)
	{
		stringFormat.SetLineAlignment(StringAlignmentNear);
	}
	else if (Paint.TextVertAlign == TGuiPaint::vaBottom)
	{
		stringFormat.SetLineAlignment(StringAlignmentFar);
	}

    // horizontal alignment
	if (Paint.TextHorizAlign == TGuiPaint::haCenter)
	{
		stringFormat.SetAlignment(StringAlignmentCenter);
	}
	else if (Paint.TextHorizAlign == TGuiPaint::haLeft)
	{
		stringFormat.SetAlignment(StringAlignmentNear);
	}
	else if (Paint.TextHorizAlign == TGuiPaint::haRight)
	{
		stringFormat.SetAlignment(StringAlignmentFar);
	}

	stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);
	
	int length = std::min(255, (int)strlen(Text)); //MODS 'min' --> 'std::min' //MODES 'strlen' --> '(int)strlen'

	mbstowcs(m_strBuffer, Text, length);

	Graphics pGraphics(memDC);
	pGraphics.SetSmoothingMode(SmoothingModeHighSpeed);
	pGraphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
	pGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	pGraphics.SetPixelOffsetMode(PixelOffsetModeHighSpeed);
	pGraphics.SetCompositingQuality(CompositingQualityHighSpeed);
		
	RectF transRect(-(layoutRect.Width/2), -(layoutRect.Height/2) + 2, layoutRect.Width, layoutRect.Height - 2);

	pGraphics.TranslateTransform(layoutRect.X + (layoutRect.Width/2), layoutRect.Y + (layoutRect.Height/2));

	// adjustment from the rotation
	// only tested with orientation = 90
	if (Paint.TextOrientation != 0.0f)
	{
		pGraphics.RotateTransform(Paint.TextOrientation);
		transRect.X = -(layoutRect.Height/2);
		transRect.Y = -(layoutRect.Width/2) + 2;
		transRect.Width = layoutRect.Height;
		transRect.Height = layoutRect.Width - 2;
	}
	
	pGraphics.DrawString(m_strBuffer, length, m_txtFont, transRect, &stringFormat, &sbr);
	
	// for debug, Draw layoutRect.
    //pGraphics.DrawRectangle(&Pen(Color::Red, 1), transRect);
	pGraphics.ResetTransform();

}
