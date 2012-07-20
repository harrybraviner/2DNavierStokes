#ifndef	__OUTPUT_H__
#define	__OUTPUT_H__

#include "2DNavierStokes.h"

void initEnergyOutput(struct Field *fld, char *outfile);
void initVelocityOutput(char *outdir);

void outputEnergy(struct Field *fld, double t, char *outfile);
void outputVelocity(struct Field *fld, int N, double t, char *outdir);

#endif
