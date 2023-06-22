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
#include"slice1.h"
#include"slice2.h"
#include"increment.h"

class nhflow_flux;
class nhflow_reconstruct;
class nhflow_signal_speed;
class patchBC_interface;
class ghostcell;

#ifndef NHFLOW_HLL_H_
#define NHFLOW_HLL_H_

using namespace std;

class nhflow_HLL : public nhflow_convection, public increment
{

public:

	nhflow_HLL (lexer*,ghostcell*,patchBC_interface*);
	virtual ~nhflow_HLL();

    double start_E(lexer*, fdm_nhf*, double*, int, double*, double*, double*);
    double start_U(lexer*, fdm_nhf*, double*, int, double*, double*, double*);
    double start_V(lexer*, fdm_nhf*, double*, int, double*, double*, double*);
    double start_W(lexer*, fdm_nhf*, double*, int, double*, double*, double*);

private:
    double aij(lexer*, fdm_nhf*, double*, int, double*, double*, double*, double*, double*, double*);
    
    
    
    
	double dx,dy,dz;
	double udir,vdir,wdir;
	double L;
    double denom;

    double ivel1,ivel2,jvel1,jvel2,kvel1,kvel2;

    ghostcell *pgc;
    nhflow_flux *pflux;
    patchBC_interface *pBC;
    nhflow_reconstruct *precon;
    nhflow_signal_speed *pss;
};

#endif
