#include "computeAdvection.h"
#include "2DNavierStokes.h"

void computeAdvection(struct Field *fld){
	int i;

	// Copy the velocities into the w-arrays, and transform back to position space
	for(i=0; i<NTOTAL_COMPLEX; i++){
		fld->w1[i] = fld->vx[i];
		fld->w2[i] = fld->vy[i];
		if(isnan(cabs(fld->w1[i]))) printf("w1 : i = %d\n",i);
		if(isnan(cabs(fld->w2[i]))) printf("w2 : i = %d\n",i);
		if(isnan(cabs(fld->vx[i]))) printf("vx : i = %d\n",i);
		if(isnan(cabs(fld->vy[i]))) printf("vy : i = %d\n",i);
	}
	fftw_execute_dft_c2r(fld->c2r_velocity, (complex *)fld->w1, (double *)fld->w1);
	fftw_execute_dft_c2r(fld->c2r_velocity, (complex *)fld->w2, (double *)fld->w2);

	// FIXME - add in anti-aliasing
	// Compute u_x*u_x, u_x*u_y, u_y*u_y
	for(i=0; i<NX*NY; i++){	// Loop over spatial points FIXME - is this actually looping over the correct points? See FFTW3 manual
		((double*)fld->Axx)[i] = ((double*)fld->w1)[i]*((double*)fld->w1)[i];
		((double*)fld->Ayy)[i] = ((double*)fld->w2)[i]*((double*)fld->w2)[i];
		((double*)fld->Axy)[i] = ((double*)fld->w1)[i]*((double*)fld->w2)[i];
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
