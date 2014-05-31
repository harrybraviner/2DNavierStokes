#ifndef __TIMEINTEGRATION_H__
#define __TIMEINTEGRATION_H__

#include "problem.h"
#include "2DNavierStokes.h"

double dtFromCFL(struct Field *fld);

#endif
