/*FIXME - copyright stuff here */

#ifndef __2DNAVIERSTOKES_H__
#define __2DNAVIERSTOKES_H__

#include <complex.h>
#include <fftw3.h>
#include <math.h>

#include "problem.h"

#define NTOTAL_COMPLEX	NX*(NY/2 + 1)	// number of complex points of data in Fourier space
#define IDX2D	i*(NY/2 + 1) + j
#define K_X	i*M_PI
#define K_Y	j*M_PI

struct Field {	// Structure holding pointers to velocity fields. Could be updated later with other variables
	double complex *vx, *vy;	// Complex DFTs of the velocity components
	double complex *w1, *w2;	// Used for FFTs for computing self-advection
	double complex *Axx, *Ayy, *Axy;	// Used for holding the O(u^2) terms ("Advection")
	fftw_plan r2c_velocity, c2r_velocity;
};

#endif
