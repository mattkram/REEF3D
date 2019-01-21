/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2019 Hans Bihs

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
--------------------------------------------------------------------*/

#include"field4.h"
#include"fieldint5.h"
#include"slice4.h"
#include"sliceint4.h"
#include"increment.h"
#include"vec.h"
#include"matrix_diag.h"
#include"cpt.h"

class lexer;

#ifndef FDM_FNPF_H_
#define FDM_FNPF_H_

using namespace std;

class fdm_fnpf : public increment
{
public:

    fdm_fnpf(lexer*);
   
    field4 u,v,w,press;
    field4 Fi4;
    field4 test;
    fieldint5 nodeval;
    
    slice4 eta,eta_n,WL;
    slice4 bed,depth;
    slice4 Fifsf,Fibed,Fz;
    slice4 K;
    sliceint4 etaloc;
    
    slice4 Fx,Fy;
    slice4 Ex,Ey;
    slice4 Exx,Eyy;
    slice4 Bx,By;
    slice4 Bxx,Byy;
    slice4 Hx,Hy;
    

	
    vec rhsvec;
    double *Fi;

	matrix_diag M;    
    
    int **pvccnode,*ccedge;
    
    double gi,gj,gk;
};

#endif
