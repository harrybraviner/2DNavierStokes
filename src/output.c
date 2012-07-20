#include "output.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <math.h>

// FIXME - add return int to indicate success or failure
void initEnergyOutput(struct Field *fld, char *outfileName){
	FILE *outfile = fopen(outfileName, "w");
	fprintf(outfile, "# Total kinetic energy against time\n# t\tE_tot\n");
	fclose(outfile);
}

// FIXME - add return int to indicate success or failure
void initVelocityOutput(char *outdir){
	if(mkdir(outdir, S_IRWXU)){
		// Deal with this having failed
		if(errno==EEXIST) fprintf(stderr, "The directory %s already exists. Previous velocity data may be overwritten.\n", outdir);
		else fprintf(stderr, "The directory %s does not exist and could not be created. Velocity data may not be written.\n", outdir);
	} else fprintf(stderr, "Made the directory %s successfully.\n", outdir);
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

void outputVelocity(struct Field *fld, int N, double t, char *outdir){
	// FIXME - this whole function is really cumbersome, can I rewrite it?
	// Build the filename to which the velocities will be written
#if 0
	int digits = 1, tempN = N/10;
	int i;
	char *outfileName;
	while(tempN!=0){ digits++; tempN=tempN/10;}
	outfileName = fftw_malloc(sizeof(char)*(6 + digits));
	outfileName[0] = 'v', outfileName[6+(digits)-5] = '.', outfileName[6+(digits)-4] = 'd', outfileName[6+(digits)-3] = 'a', outfileName[6+(digits)-2] = 't', outfileName[6+(digits)-1] = '\0';
	tempN = N;
	for(i=0; i<digits; i++){
		outfileName[digits-i] = (char)(tempN%10+48);
		tempN = tempN/10;
	}
#endif
	FILE *outfile;
	char *outfileName; char *numStr;
	numStr = fftw_malloc(sizeof(char)*15);
	outfileName = fftw_malloc(sizeof(char)*50); outfileName[0] = '\0';
	strcat(outfileName, outdir);
	strcat(outfileName, "v");
	sprintf(numStr, "%d", N);
	strcat(outfileName, numStr);
	strcat(outfileName, ".dat");

	printf("Trying to write to %s\n", outfileName);

	if((outfile = fopen(outfileName, "w")) == NULL) printf("Failed to open %s for writing. The velocity data at time %f was not written",  outfileName, t);
	else{
		int i,j;
		fprintf(outfile, "# Spatial velocity data at time %f\n",t);
		fprintf(outfile, "# x\ty\tvx\tvy\n");
		for(i=0; i<NX; i++){	// Loop over x
			for(j=0; j<NY; j++){	// Loop over y
				fprintf(outfile, "%f\t%f\t%f\t%f\n",
				(2*M_PI*i)/NX,
				(2*M_PI*j)/NY,
				((double*)fld->w1)[i+(NY+2)*j],
				((double*)fld->w2)[i+(NY+2)*j]);
			}
		}
		fclose(outfile);
	}

	// Clean up
	fftw_free(outfileName); fftw_free(numStr);
}
