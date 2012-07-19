#include "computeAdvection.h"
#include "2DNavierStokes.h"

void computeAdvection(struct Field *fld){
	int i;

	// Copy the velocities into the w-arrays, and transform back to position space
	for(i=0; i<NTOTAL_COMPLEX; i++){
		fld->w1[i] = fld->vx[i];
		fld->w2[i] = fld->vy[i];
	}
	fftw_execute_dft_c2r(fld->c2r_velocity, (complex *)fld->w1, (double *)fld->w1);
	fftw_execute_dft_c2r(fld->c2r_velocity, (complex *)fld->w2, (double *)fld->w2);

	// FIXME - add in anti-aliasing
	// Compute u_x*u_x, u_x*u_y, u_y*u_y
	for(i=0; i<NX*NY; i++){	// Loop over spatial points
		fld->Axx[i] = ((double)fld->w1[i])*((double)fld->w1[i]);
		fld->Ayy[i] = ((double)fld->w2[i])*((double)fld->w2[i]);
		fld->Axy[i] = ((double)fld->w1[i])*((double)fld->w2[i]);
	}

	// Transform the Advection arrays back to wavenumber space
	fftw_execute_dft_r2c(fld->r2c_velocity, (double *)fld->Axx, fld->Axx);
	fftw_execute_dft_r2c(fld->r2c_velocity, (double *)fld->Ayy, fld->Ayy);
	fftw_execute_dft_r2c(fld->r2c_velocity, (double *)fld->Axy, fld->Axy);

	// A DFT followed by its inverse in FFTW3 multiplies by the total number of grid points, we correct for this now
	for(i=0; i<NTOTAL_COMPLEX; i++){
		fld->Axx[i] /= NTOTAL_COMPLEX;
		fld->Ayy[i] /= NTOTAL_COMPLEX;
		fld->Axy[i] /= NTOTAL_COMPLEX;
	}
}
