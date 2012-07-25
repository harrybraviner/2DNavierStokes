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
	char *velocityOutdir = "./velocityData/";

	struct Field fld;
	initialiseField(&fld);

	// Main loop - Forward Euler integration
	double t = 0.0, dt = 0.01;
	int velocityOutputNumber = 1;
	double lastVelocityOutputT = 0.0;
	complex p_k;
	struct Field dfld;	// delta-fld that we use for integration
	initialiseField(&dfld);
	// FIXME - think about replacing these with some kind of 'output structs' to handle things more transparrantly
	initEnergyOutput(&fld, energyOutfile);
	initVelocityOutput(velocityOutdir);
	while(t < T_FINAL){
		computeAdvection(&fld);

		if(t - lastVelocityOutputT >= V_OUT_DELTA_T){
			outputVelocity(&fld, velocityOutputNumber, t, velocityOutdir);
			lastVelocityOutputT = t;
			velocityOutputNumber++;
		}
	

		/* Explicit integration */

		// Subtract (u.grad)u
		for(i=0; i<NX; i++){	// Loop over k_x
			for(j=0; j<NY/2+1; j++){ // Loop over k_y
				dfld.vx[IDX2D] = +I*(K_X*fld.Axx[IDX2D] + K_Y*fld.Axy[IDX2D]);
				dfld.vy[IDX2D] = +I*(K_X*fld.Axy[IDX2D] + K_Y*fld.Ayy[IDX2D]);
			}
		}

		// Calculate the pressure
		for(i=0; i<NX; i++){	// Loop over k_x
			for(j=1; j<NY/2+1; j++){ // Loop over k_y != 0
				// Calculate this wavenumber for p/k^2
				p_k = -(K_X*dfld.vx[IDX2D] + K_Y*dfld.vy[IDX2D])/(K_X*K_X + K_Y*K_Y);
				// Subtract grad p from delta-u
				dfld.vx[IDX2D] += K_X*p_k;
				dfld.vy[IDX2D] += K_Y*p_k;
			}
		}
		for(i=1; i<NX; i++){	// Loop over k_x != 0
			j=0;
			// Calculate this wavenumber for p/k^2
			p_k = -(K_X*dfld.vx[IDX2D] + K_Y*dfld.vy[IDX2D])/(K_X*K_X + K_Y*K_Y);
			// Subtract grad p from delta-u
			dfld.vx[IDX2D] += K_X*p_k;
			dfld.vy[IDX2D] += K_Y*p_k;
		}

		for(i=0; i<NTOTAL_COMPLEX; i++){
				fld.vx[i] += dfld.vx[i];
				fld.vy[i] += dfld.vy[i];
		}

		/* End of explicit integration */

		// Implicit integration for the viscous terms
		for(i=0; i<NX; i++){	// Loop over k_x
			for(j=0; j<NY/2+1; j++){	// Loop over k_y
			//printf("%f\n",exp(-dt*NU*(K_X*K_X+K_Y*K_Y)));
				fld.vx[IDX2D] *= exp(-dt*NU*(K_X*K_X+K_Y*K_Y));
				fld.vy[IDX2D] *= exp(-dt*NU*(K_X*K_X+K_Y*K_Y));
			}
		}


		outputEnergy(&fld, t, energyOutfile);

		t += dt;
	}

	return 0;
}
