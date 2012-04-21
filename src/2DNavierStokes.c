/* FIXME - stuff */

#include <math.h>
#include <complex.h>
#include <fftw3.h>

#include "2DNavierStokes.h"
#include "initialisation.h"

int main(int argc, char *argv[]){

	// FIXME - allocate memory for the field and build fftw plans
	struct Field fld;
	initialiseField(&fld);
	// FIXME - call main loop

	return 0;
}
