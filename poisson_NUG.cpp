/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2018 Hans Bihs

This file is part of REEF3D.

REEF3D is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
--------------------------------------------------------------------*/

#include"poisson_NUG.h"
#include"lexer.h"
#include"fdm.h"

poisson_NUG::poisson_NUG(lexer * p) : density(p)
{
}

poisson_NUG::~poisson_NUG()
{
}

void poisson_NUG::estart(lexer* p, fdm *a, field &press)
{	
   
	n=0;
    LOOP
	{
	a->M.p[n]  =   (CPOR1*PORVAL1)/(roface(p,a,1,0,0)*p->DXP[IP]*0.5*(p->DXP[IP]+p->DXP[IM1]))*p->x_dir
                + (CPOR1m*PORVAL1m)/(roface(p,a,-1,0,0)*p->DXP[IM1]*0.5*(p->DXP[IP]+p->DXP[IM1]))*p->x_dir
                
                + (CPOR2*PORVAL2)/(roface(p,a,0,1,0)*p->DYP[JP]*0.5*(p->DYP[JP]+p->DYP[JM1]))*p->y_dir
                + (CPOR2m*PORVAL2m)/(roface(p,a,0,-1,0)*p->DYP[JM1]*0.5*(p->DYP[JP]+p->DYP[JM1]))*p->y_dir
                
                + (CPOR3*PORVAL3)/(roface(p,a,0,0,1)*p->DZP[KP]*0.5*(p->DZP[KP]+p->DZP[KM1]))*p->z_dir
                + (CPOR3m*PORVAL3m)/(roface(p,a,0,0,-1)*p->DZP[KM1]*0.5*(p->DZP[KP]+p->DZP[KM1]))*p->z_dir;


   	a->M.n[n] = -(CPOR1*PORVAL1)/(roface(p,a,1,0,0)*p->DXP[IP]*0.5*(p->DXP[IP]+p->DXP[IM1]))*p->x_dir;
	a->M.s[n] = -(CPOR1m*PORVAL1m)/(roface(p,a,-1,0,0)*p->DXP[IM1]*0.5*(p->DXP[IP]+p->DXP[IM1]))*p->x_dir;

	a->M.w[n] = -(CPOR2*PORVAL2)/(roface(p,a,0,1,0)*p->DYP[JP]*0.5*(p->DYP[JP]+p->DYP[JM1]))*p->y_dir;
	a->M.e[n] = -(CPOR2m*PORVAL2m)/(roface(p,a,0,-1,0)*p->DYP[JM1]*0.5*(p->DYP[JP]+p->DYP[JM1]))*p->y_dir;

	a->M.t[n] = -(CPOR3*PORVAL3)/(roface(p,a,0,0,1)*p->DZP[KP]*0.5*(p->DZP[KP]+p->DZP[KM1]))*p->z_dir;
	a->M.b[n] = -(CPOR3m*PORVAL3m)/(roface(p,a,0,0,-1)*p->DZP[KM1]*0.5*(p->DZP[KP]+p->DZP[KM1]))*p->z_dir;
	
	++n;
	}
    
    n=0;
	LOOP
	{
		if(p->flag4[Im1JK]<0)
		{
		a->rhsvec.V[n] -= a->M.s[n]*press(i-1,j,k);
		a->M.s[n] = 0.0;
		}
		
		if(p->flag4[Ip1JK]<0)
		{
		a->rhsvec.V[n] -= a->M.n[n]*press(i+1,j,k);
		a->M.n[n] = 0.0;
		}
		
		if(p->flag4[IJm1K]<0)
		{
		a->rhsvec.V[n] -= a->M.e[n]*press(i,j-1,k);
		a->M.e[n] = 0.0;
		}
		
		if(p->flag4[IJp1K]<0)
		{
		a->rhsvec.V[n] -= a->M.w[n]*press(i,j+1,k);
		a->M.w[n] = 0.0;
		}
		
		if(p->flag4[IJKm1]<0)
		{
		a->rhsvec.V[n] -= a->M.b[n]*press(i,j,k-1);
		a->M.b[n] = 0.0;
		}
		
		if(p->flag4[IJKp1]<0)
		{
		a->rhsvec.V[n] -= a->M.t[n]*press(i,j,k+1);
		a->M.t[n] = 0.0;
		}

	++n;
	}
}

void poisson_NUG::istart(lexer* p, fdm* a, field &apu, field &apv, field &apw, field &pcorr)
{
}
