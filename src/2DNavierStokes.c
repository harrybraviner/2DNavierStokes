/* FIXME - stuff */

#include <math.h>
#include <complex.h>
#include <fftw3.h>

#include "2DNavierStokes.h"
#include "initialisation.h"
#include "computeAdvection.h"

int main(int argc, char *argv[]){
	int i,j;	// Dummy counter variables, used in IDX2D
	char *energyOutfile = "energy.dat";

	// FIXME - allocate memory for the field and build fftw plans
	struct Field fld;
	initialiseField(&fld);
	// FIXME - call main loop

	// Main loop - Forward Euler integration
	double t = 0.0, dt = 1.0;
	double p_k;
	struct Field dfld;	// delta-fld that we use for integration
	initialiseField(&dfld);
	initEnergyOutput(&fld, energyOutfile);
	while(t < T_FINAL){
		computeAdvection(&fld);
	

		// Subtract (u.grad)u
		for(i=0; i<NX; i++){	// Loop over k_x
			for(j=0; j<NY/2+1; j++){ // Loop over k_y
				dfld.vx[IDX2D] = +I*(K_X*fld.Axx[IDX2D] + K_Y*fld.Axy[IDX2D]);
				dfld.vy[IDX2D] = +I*(K_X*fld.Axy[IDX2D] + K_Y*fld.Ayy[IDX2D]);
			}
		}

		// Calculate the pressure
		for(i=0; i<NX; i++){	// Loop over k_x
			for(j=0; j<NY/2+1; j++){ // Loop over k_y
				// Calculate this wavenumber for p*k^2
				p_k = (K_X*dfld.vx[IDX2D] + K_Y*dfld.vy[IDX2D])/(K_X*K_X + K_Y*K_Y);
				// Subtract grad p from delta-u
				dfld.vx[IDX2D] += K_X*p_k;
				dfld.vy[IDX2D] += K_Y*p_k;
			}
		}

		outputEnergy(&fld, t, energyOutfile);

		t += dt;
	}

	return 0;
}
