#include "RenderedObject.h"

RenderedObjects::Point<float>* RenderedObjects::LG()
{
	const unsigned int N = APR_PARAM_N;
	const unsigned int M = APR_PARAM_M;
	const unsigned int A = APR_PARAM_A;
	const unsigned int B = APR_PARAM_B;

	float x[N];
	float y[N];
	for (unsigned int i = 0; i < N; i++)
	{
		x[i] = A + i * (B - A) / (M - 1);
		FUNCTION(x[i], y[i]);
	}
	
	Point<float>* points = new Point<float>[N];
	for (unsigned int cur = 0; cur < N; cur++)
	{
		float sum = 0;
		for (int i = 0; i < N; i++)
		{
			float mul = 1;
			for (int j = 0; j < N; j++)
			{
				if (i != j) mul *= (x[cur] - x[j]) / (x[i] - x[j]);
			}
			sum += y[i] * mul;
		}
		points[cur] = { x[cur], sum };
	}

	return points;
}

RenderedObjects::Point<float>* RenderedObjects::POL()
{
	const unsigned int N = APR_PARAM_N;
	const unsigned int M = APR_PARAM_M;
	const unsigned int A = APR_PARAM_A;
	const unsigned int B = APR_PARAM_B;
	float x[M];
	float y[M];
	for (unsigned int i = 0; i < M; i++)
	{
		x[i] = A + i * (float)(B - A) / (M - 1);
		FUNCTION(x[i], y[i]);
	}

	Point<float>* points = new Point<float>[M];
	float a[N][N];
	for (unsigned int k = 0; k < N; k++)
	{
		a[k][0] = 1;
		int i = -1;
		for (unsigned int m = 1; m < N; m++)
		{
			i++;
			if (i == k) i++;
			float d = x[k] - x[i];
			a[k][m] = a[k][m - 1] / d;
			for (unsigned int j = m - 1; j > 0; j--)
			{
				a[k][j] = (a[k][j - 1] - a[k][j] * x[i]) / d;
			}
			a[k][0] = (-1) * a[k][0] * x[i] / d;
		}
	}
	float c[N];
	for (unsigned int i = 0; i < N; i++)
	{
		c[i] = 0;
		for (unsigned int k = 0; k < N; k++)
		{
			c[i] = c[i] + a[k][i] * y[k];
		}
	}

	Solver solver;
	float* fi = new float[N];
	solver.MG<N>(a, c, fi);
	for (unsigned int i = 0; i < N; i++)
		points[i] = { x[i], fi[i] };
	return points; // TODO
}

RenderedObjects::Point<float>* RenderedObjects::PNL()
{
	const unsigned int N = APR_PARAM_N;
	const unsigned int M = APR_PARAM_M;
	const unsigned int A = APR_PARAM_A;
	const unsigned int B = APR_PARAM_B;

	float x[M];
	float y[M];
	for (unsigned int i = 0; i < M; i++)
	{
		x[i] = A + i * (float) (B - A) / (M - 1);
		FUNCTION(x[i], y[i]);
	}
	
	Point<float>* points = new Point<float>[M];
	for (int current = 0; current < M; current++)
	{
		float xT = x[current];
		unsigned int i = 0;
		if (x[0] <= xT && xT <= x[M - 1])
		{
			i = 1;
			while (xT > x[i])
				i++;
			points[current] = { x[current], y[i - 1] + (xT - x[i - 1]) * ((y[i] - y[i - 1]) / (x[i] - x[i - 1])) };
		}
	}
	return points;
}

template <typename T> RenderedObjects::Point<RenderedObjects::coord_t> RenderedObjects::CoordinatesToWindowPoint(Point<T> point)
{
	return
	{
		static_cast<coord_t>((float)WIDTH / (X_MAX - X_MIN) * (point.x - X_MIN)),
		static_cast<coord_t>((float)HEIGHT - HEIGHT / (Y_MAX - Y_MIN) * (point.y - Y_MIN))
	};
}

void RenderedObjects::Pixel::update()
{
	HPEN hPen = CreatePen(PS_SOLID, width, STORGB(color));
	SelectObject(hdc, hPen);

	Point<coord_t> hwnd_point = CoordinatesToWindowPoint(point);

	Rectangle(hdc, hwnd_point.x, hwnd_point.y, hwnd_point.x + 1, hwnd_point.y + 1);
	hPen = CreatePen(PS_SOLID, lineWidth, STORGB(color));
	SelectObject(hdc, hPen);
	if (LineToPrevious)
		LineTo(hdc, hwnd_point.x, hwnd_point.y);
	MoveToEx(hdc, hwnd_point.x, hwnd_point.y, NULL);
}

void RenderedObjects::Grid::Print()
{
	/************************************************************/
	/*						Grind Rendering						*/
	/************************************************************/

	Pixel pixel;
	pixel.SetColor(COLOR_GRID);
	pixel.SetDC(hdc);
	pixel.SetWidth(PIXEL_WIDTH);
	pixel.SetLineWidth(GRID_WIDTH);
	pixel.SetLineToPreviousEnabled(true);

	Point<coord_t> hwnd_point = CoordinatesToWindowPoint(Point<float> { 0, 0 });

	MoveToEx(hdc, 0, hwnd_point.y, NULL);
	for (int i = X_MIN; i < X_MAX; i++)
	{
		pixel.SetPosition(i, 0);
		pixel.update();
	}
	MoveToEx(hdc, hwnd_point.x, 0, NULL);
	for (int i = Y_MAX; i >= Y_MIN; i--)
	{
		pixel.SetPosition(0, i);
		pixel.update();
	}
}

void RenderedObjects::Core::StartRender()
{
	grid = Grid(hdc);
	grid.Print();		// Initialise grid

	Update();
}

void RenderedObjects::Core::Update()
{
	float x, y;

	Pixel pixel;
	pixel.SetColor(COLOR_GRAPH);
	pixel.SetDC(hdc);
	pixel.SetWidth(PIXEL_WIDTH);
	pixel.SetLineWidth(LINE_WIDTH);

	for (x = X_VAL_START; x <= X_VAL_END; x += STEP)
	{
		FUNCTION(x, y);
		pixel.SetPosition(x, y);
		pixel.update();
		pixel.SetLineToPreviousEnabled(true);
	}
	pixel.SetLineToPreviousEnabled(false);
	pixel.SetColor(COLOR_INT);
	Point<float>* points = APR_ALG();
	for (unsigned int i = 0; i < APR_PARAM_M; i++)
	{
		pixel.SetPosition(points[i].x, points[i].y);
		pixel.update();
		pixel.SetLineToPreviousEnabled(true);
	}
	pixel.SetLineToPreviousEnabled(false);
	delete[] points;
}