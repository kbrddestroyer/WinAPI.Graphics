#pragma once

#define APR_PARAM_A 3										// SIZE_T
#define APR_PARAM_B 6										// SIZE_T
#define APR_PARAM_M 11										// SIZE_T
#define APR_PARAM_N 11										// SIZE_T

#define X_MAX		25										// SIZE_T
#define Y_MAX		25										// SIZE_T
#define X_MIN		-10										// SIZE_T
#define Y_MIN		-10										// SIZE_T

#define MI_SET_W	0.2									// FLOAT
#define MI_SET_ESP	0.001										// FLOAT

#define X_VAL_START	APR_PARAM_A								// SIZE_T
#define X_VAL_END	APR_PARAM_B								// SIZE_T

#define COLOR_GRID	{ 0, 0, 0 }								// RGB()
#define COLOR_GRAPH { 0, 0, 255 }							// RGB()
#define COLOR_INT	{ 255, 0, 0 }							// RGB()

#define APR_ALG		POL

#define PIXEL_WIDTH 4										// SIZE_T
#define LINE_WIDTH  3										// SIZE_T
#define GRID_WIDTH  1										// SIZE_T
#define STEP		0.05									// FLOAT

#define APROXIMATE	1										// BOOL

#define FUNCTION(x, y) y = log(x) - 5 * sin(x) * sin(x)		// Function to render
