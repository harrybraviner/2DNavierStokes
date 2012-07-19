#include "output.h"
#include <stdio.h>

void initEnergyOutput(struct Field *fld, char *outfileName){
	FILE *outfile = fopen(outfileName, "w");
	fprintf(outfile, "# Total kinetic energy against time\n# t\tE_tot\n");
	fclose(outfile);
}

void outputEnergy(struct Field *fld, double t, char *outfileName){
	FILE *outfile = fopen(outfileName, "a");
	int i,j;
	double E = 0.0;
	for(i=0; i<NX; i++){	// Loop over k_x
		// For j=0 each fourier coefficient is held in memory
		j=0;
		E += creal(fld->vx[IDX2D])*creal(fld->vx[IDX2D]) + cimag(fld->vx[IDX2D])*cimag(fld->vx[IDX2D]);
		E += creal(fld->vy[IDX2D])*creal(fld->vy[IDX2D]) + cimag(fld->vy[IDX2D])*cimag(fld->vy[IDX2D]);
		for(j=1; j<NY/2+1; j++){	// Loop over k_y
			// Only half the j!=0 fourier coefficients are held, due to it being a DFT of real data
			E += 2*(creal(fld->vx[IDX2D])*creal(fld->vx[IDX2D]) + cimag(fld->vx[IDX2D])*cimag(fld->vx[IDX2D]));
			E += 2*(creal(fld->vy[IDX2D])*creal(fld->vy[IDX2D]) + cimag(fld->vy[IDX2D])*cimag(fld->vy[IDX2D]));
		}
	}
	E *= 0.5;	// E = 0.5*u.u
	fprintf(outfile, "%lf\t%lf\n", t, E);
	fclose(outfile);
}
