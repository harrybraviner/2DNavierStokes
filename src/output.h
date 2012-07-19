#ifndef	__OUTPUT_H__
#define	__OUTPUT_H__

#include "2DNavierStokes.h"

void initEnergyOutput(struct Field *fld, char *outfile);

void outputEnergy(struct Field *fld, double t, char *outfile);

#endif
