#pragma once
#include <cmath>

#include "framework.h"
#include "settings.h"

#include "Graphics.h"
#include "Solver.h"

#define ctg(rad) 1 / tan(rad)							// May be used in FUNC
#define STORGB(color) RGB(color.r, color.g, color.b)	// Cast RGBColor to RGB

namespace RenderedObjects
{
	/*
	* 
	* 
	*				TYPES
	* 
	* 
	*/

	typedef unsigned int	coord_t;					// Coordinate type
	typedef unsigned short	color_t;					// Color param type
	

	/*
	* 
	* 
	*				DATA STRUCTURES
	* 
	* 
	*/

	struct RGBColor
	{
		color_t r, g, b;								// RGB color (no alpha)
	};

	template <typename T_>
	struct Point
	{
		T_ x, y;										// Point (accepts different data types f.e. float, unsigned int etc.)
	};

	/*
	* 
	* 
	*				FUNCTIONS
	* 
	* 
	*/

	/* Function, that converts grid point coordinates to WinAPI Window coordinates */
	template <typename T> Point<coord_t> CoordinatesToWindowPoint(Point<T> point);
	
	/* Interpolate function (uses settings.h) */
	Point<float>* POL();
	Point<float>* PNL();
	Point<float>* LG();

	/*
	*
	*
	*				CLASSES
	*
	*
	*/

	/* Rendering object base class */
	class DCObject
	{
	protected:
		HDC				hdc;
		RGBColor		color;
		unsigned int	width;
	public:
		DCObject(HDC hdc = nullptr, unsigned int width = 1, RGBColor color = { 0, 0, 0 }) :
			hdc(hdc), width(width), color(color) {}

		void SetDC(HDC& hdc) { this->hdc = hdc; }
		void SetColor(RGBColor color) { this->color = color; }
		void SetWidth(unsigned int width) { this->width = width; }

		virtual void update() = 0;
	};

	/* Single point class */
	class Pixel : virtual public DCObject
	{
	private:
		Point<float>    point;
		bool			LineToPrevious;
		unsigned int	lineWidth;
	public:
		Pixel(HDC hdc = nullptr, int width = 1, int lineWidth = 1, float x = 0, float y = 0, RGBColor color = {0, 0, 0}, bool LineToPrevious = false) :
			point({ x, y }), lineWidth(lineWidth), DCObject(hdc, width, color), LineToPrevious(LineToPrevious) {}

		void SetPosition(float x, float y) { this->point.x = x; this->point.y = y; }
		void SetLineToPreviousEnabled(bool enable) { this->LineToPrevious = enable; }
		void SetLineWidth(unsigned int lineWidth) { this->lineWidth = lineWidth; }
		
		void update() override;
	};

	/* World grid (setups coordinates) */
	class Grid
	{
	private:
		HDC hdc;
	public:
		Grid(HDC hdc = nullptr) : hdc(hdc) {}

		void Print();
	};

	/********************************************************/
	/*						ENTRY POINT						*/
	/********************************************************/
	
	class Core
	{
	private:
		HDC hdc;
		Grid grid;
	public:
		Core(HDC hdc = nullptr) : hdc(hdc) {}

		void StartRender();

		void Update();
	};
}