#include "timeIntegration.h"
#include "2DNavierStokes.h"
#include <math.h>

double dtFromCFL(struct Field *fld){
	int i;
	double kxmax, kymax;
	double vxmax=0.0, vymax=0.0;
	// Find the maximum velocity and the corresponding wavenumber
	for(i=0; i<NTOTAL_REAL; i++){
		if(fabs(fld->w1[i]) > vxmax) vxmax = fabs(fld->w1[i]);
		if(fabs(fld->w2[i]) > vymax) vymax = fabs(fld->w2[i]);
	}
	kxmax = 1.0 / (NX/2 - 1);
	kymax = 1.0 / (NY/2 - 1);

	return CFL_PARAM / (vxmax*kxmax + vymax*kymax + CFL_SAFETY_PARAM);
}
