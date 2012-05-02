#include <fftw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "2DNavierStokes.h"
#include "initialisation.h"

void initialiseField(struct Field *fld){
	int i,j;	// counters for k_x, k_y position in array

	fld->vx = (complex*)fftw_malloc(sizeof(complex)*NTOTAL_COMPLEX);
	fld->vy = (complex*)fftw_malloc(sizeof(complex)*NTOTAL_COMPLEX);

	for(i=0; i<NX; i++){	// loop over k_x
		for(j=0; j<NY/2+1; j++){	// loop over k_y
			fld->vx[IDX2D] = 0.0;
			fld->vy[IDX2D] = 0.0;
		}
	}
}
