/*FIXME - copyright stuff here */

#ifndef __2DNAVIERSTOKES_H__
#define __2DNAVIERSTOKES_H__

#include <complex.h>

#include "problem.h"

#define NTOTAL_COMPLEX	NX*(NY/2 + 1)	// number of complex points of data in Fourier space
#define IDX2D	i*(NY/2 + 1) + j

struct Field {	// Structure holding pointers to velocity fields. Could be updated later with other variables
	double complex *vx, *vy;
};

#endif
