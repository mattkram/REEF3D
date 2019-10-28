/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2019 Hans Bihs

This file is part of REEF3D.

REEF3D is fra->eps software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Fra->eps Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. Sa->eps the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, sa->eps <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
--------------------------------------------------------------------*/

#include"sflow_momentum.h"
#include"slice1.h"
#include"slice2.h"
#include"slice4.h"
#include"increment.h"

class sflow_convection;
class sflow_fsf;
class sflow_diffusion;
class sflow_boussinesq;
class sflow_roughness;
class sflow_sediment_RK;

using namespace std;

#ifndef SFLOW_MOMENTUM_RK2_H_
#define SFLOW_MOMENTUM_RK2_H_

class sflow_momentum_RK2 : public sflow_momentum, public increment
{
public:
	sflow_momentum_RK2(lexer*, fdm2D*, sflow_convection*, sflow_diffusion*, sflow_pressure*, 
                        solver2D*, solver2D*, ioflow*, sflow_fsf*, sflow_boussinesq*);
	virtual ~sflow_momentum_RK2();
	virtual void start(lexer*, fdm2D*, ghostcell*);

    slice1 Prk1;
	slice2 Qrk1;
    slice4 wrk1;
    slice4 etark1;

private:
	void irhs(lexer*,fdm2D*,ghostcell*,slice&,double);
	void jrhs(lexer*,fdm2D*,ghostcell*,slice&,double);
	
	int gcval_u, gcval_v,gcval_w;
	int gcval_urk, gcval_vrk,gcval_wrk;
    int gcval_eta, gcval_erk;
	double starttime;

	sflow_convection *pconvec;
	sflow_diffusion *pdiff;
	sflow_pressure *ppress;
	solver2D *psolv;
    solver2D *ppoissonsolv;
	ioflow *pflow;
	sflow_fsf *pfsf;
    sflow_boussinesq *pbouss;
    sflow_roughness *prough;
    sflow_sediment_RK *psedstep;
};

#endif
