#include <fftw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "2DNavierStokes.h"
#include "initialisation.h"

void initialiseField(struct Field *fld){
	int i,j;	// counters for k_x, k_y position in array

	fld->vx = (complex*)fftw_malloc(sizeof(complex)*NTOTAL_COMPLEX);
	fld->vy = (complex*)fftw_malloc(sizeof(complex)*NTOTAL_COMPLEX);
	fld->w1 = (complex*)fftw_malloc(sizeof(complex)*NTOTAL_COMPLEX);
	fld->w2 = (complex*)fftw_malloc(sizeof(complex)*NTOTAL_COMPLEX);
	fld->Axx = (complex*)fftw_malloc(sizeof(complex)*NTOTAL_COMPLEX);
	fld->Ayy = (complex*)fftw_malloc(sizeof(complex)*NTOTAL_COMPLEX);
	fld->Axy = (complex*)fftw_malloc(sizeof(complex)*NTOTAL_COMPLEX);

	// Set all the modes to zero (just for test purposes for now)
	for(i=0; i<NX; i++){	// loop over k_x
		for(j=0; j<NY/2+1; j++){	// loop over k_y
			fld->vx[IDX2D] = 0.0;
			fld->vy[IDX2D] = 0.0;
		}
	}
	// FIXME - implement proper initialisation
#if 1
	i=0; j=1;
	fld->vx[IDX2D] = -0.5*I;
#endif
#if 0
	i=1; j=0;
	fld->vx[IDX2D] = -0.5*I;
	i=NX-1; j=0;	// FIXME - This is causing a segfault - figure out why!
	fld->vx[IDX2D] = +0.5*I;
#endif
	
	fld->r2c_velocity = fftw_plan_dft_r2c_2d(NX, NY, (double *)fld->w1, (double (*)[2])fld->w1, FFTW_ESTIMATE);
	fld->c2r_velocity = fftw_plan_dft_c2r_2d(NX, NY, (double (*)[2])fld->w1, (double *)fld->w1, FFTW_ESTIMATE);

}
