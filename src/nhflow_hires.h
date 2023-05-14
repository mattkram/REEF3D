/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2023 Hans Bihs

This file is part of REEF3D.

REEF3D is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
Author: Hans Bihs
--------------------------------------------------------------------*/

#include"nhflow_convection.h"
#include"increment.h"

class nhflow_flux;
class nhflow_fluxlim;

#ifndef NHFLOW_HIRES_H_
#define NHFLOW_HIRES_H_

using namespace std;

class nhflow_hires : public nhflow_convection, public increment
{

public:

	nhflow_hires (lexer*,int);
	virtual ~nhflow_hires();

	virtual void start(lexer*, fdm_nhf*, double*, int, double*, double*,double*);

private:
    double aij(lexer*, fdm_nhf*, double*, int, double*, double*, double*, double*, double*, double*);
	
	double udir,vdir,wdir;

	double dx,dy,dz;
	double L;
    
    double ivel1,ivel2,jvel1,jvel2,kvel1,kvel2;
	
	nhflow_fluxlim *plim;
    
    nhflow_flux *pflux;
};

#endif
