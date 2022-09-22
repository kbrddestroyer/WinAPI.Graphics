#pragma once
#include <cmath>

#include "settings.h"

class Solver
{
public:
	template <const unsigned int n1, typename T>
	void MP(T a[][n1], T b[n1], T*& x)
	{
		T q[n1];
		T p[n1];
		T r[n1 - 1];
		p[0] = 0;
		for (unsigned int i = 0; i < n1 - 1; i++)
		{
			q[i] = a[i][i];
			p[i + 1] = a[i + 1][i];
			r[i] = a[i][i + 1];
		}
		q[n1 - 1] = a[n1 - 1][n1 - 1];

		T esp[n1];
		T n_[n1];

		for (unsigned int i = 0; i < n1; i++)
			esp[i] = 0.0;
		for (unsigned int i = 0; i < n1; i++)
			n_[i] = 0.0;

		int n = n1 - 1;
		esp[0] = 0 - r[0] / q[0];
		n_[0] = b[0] / q[0];
		for (unsigned int i = 1; i < n; i++)
		{
			T z = q[i] + p[i] / esp[i - 1];
			esp[i] = 0 - r[i] / z;
			n_[i] = (b[i] - p[i] * n_[i - 1]) / z;
		}
		x[n - 1] = (b[n - 1] - p[n - 1] * n_[n - 1]) / (q[n - 1] + p[n - 1] * esp[n - 1]);

		for (unsigned int i = n; i > 0; i--)
		{
			x[i - 1] = esp[i - 1] * x[i] + n_[i - 1];
		}
	}

	template <const unsigned int n, typename T>
	void MG(T a[][n], T b[n], T*& x)
	{
		for (unsigned int k = 0; k < n - 1; k++)
		{
			unsigned int p = k;
			for (unsigned int m = k + 1; m < n; m++)
				if (fabs(a[p][k]) < a[m][k])
					p = m;
			T r;
			for (unsigned int j = k; j < n; j++)
			{
				r = a[k][j];
				a[k][j] = a[p][j];
				a[p][j] = r;
			}
			r = b[k];
			b[k] = b[p];
			b[p] = r;
			for (unsigned int m = k + 1; m < n; m++)
			{
				T c = a[m][k] / a[k][k];
				b[m] = b[m] - c * b[k];
				for (unsigned int i = k; i < n; i++)
					a[m][i] = a[m][i] - c * a[k][i];
			}
			x[n - 1] = b[n - 1] / a[n - 1][n - 1];
			for (int k = n - 2; k >= 0; k--)
			{
				T s = 0;
				for (int i = k + 1; i < n; i++)
				{
					s += a[k][i] * x[i];
				}
				x[k] = (b[k] - s) / a[k][k];
			}
		}
	}

	template <const unsigned int n, typename T>
	void MI(T a[][n], T b[n], T*& x)
	{
		T w = MI_SET_W;
		T esp = MI_SET_ESP;
		T x0[n];
		for (unsigned int i = 0; i < n; i++)
		{
			x0[i] = 1;
			x[i] = 1;
		}

		unsigned int it = 0;
		T de;
		do
		{
			de = 0;
			T s;
			for (unsigned int i = 0; i < n; i++)
			{
				T s = b[i];
				for (unsigned int j = 0; j < n; j++)
				{
					if (i != j)
					{
						s -= a[i][j] * x0[j];
					}
				}
				s = s / a[i][i];
				s = w * s + (1 - w) * x[i];
				T d = fabs(x[i] - s);
				x[i] = s;
				if (d > de)
					de = d;
			}
			it++;
			for (unsigned int i = 0; i < n; i++)
				x0[i] = x[i];
		} while (it > 100 or de <= esp);
	}
};

