/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2022 Hans Bihs

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
Author: Hans Bihs
--------------------------------------------------------------------*/
#include"interpolation.h"
#include"field.h"
#include"lexer.h"
#include"fdm.h"


             
double interpolation::sl_ipol1(slice &f)
{
    v1=v2=0;

    pip=4;
    if(p->flagslice1[IJ]>0)
    v1=f(i,j);
    if(p->flagslice1[IJp1]>0)
    v2=f(i,j+1);
    pip=0;

    value = 0.5*(v1+v2);

    return value;
}

double interpolation::sl_ipol2(slice &f)
{
    v1=v2=0;

    pip=4;
    if(p->flagslice2[IJ]>0)
    v1=f(i,j);
    if(p->flagslice2[Ip1J]>0)
    v2=f(i+1,j);
    pip=0;

    value = 0.5*(v1+v2);

    return value;
}

double interpolation::sl_ipol1a(slice &f)
{
    v1=v2=0;

    pip=4;
    v1=f(i,j);
    v2=f(i,j+1);
    pip=0;

    value = 0.5*(v1+v2);

    return value;
}

double interpolation::sl_ipol2a(slice &f)
{
    v1=v2=0;

    pip=4;
    v1=f(i,j);
    v2=f(i+1,j);
    pip=0;

    value = 0.5*(v1+v2);

    return value;
}

double interpolation::sl_ipol4(slice &f)
{
    v1=v2=v3=v4=0;

    pip=4;
    v1=f(i,j);

    v2=f(i+1,j);

    v3=f(i,j+1);

    v4=f(i+1,j+1);
    pip=0;

    value = 0.25*(v1+v2+v3+v4);
    
    return value;
}

double interpolation::sl_ipol4eta(slice &f, slice &bed)
{
    double bedvalue;
    double wd_criterion=0.00005;
    int jj;
    
    if(p->A244==1)
    wd_criterion=p->A244_val;
    
    if(p->A245==1)
    wd_criterion=p->A245_val*p->dx;
    
    
    
    v1=v2=v3=v4=0.0;
    
    if(p->j_dir==0)
    {
    jj=j;
    
    j=0;
    pip=4;
    v1=f(i,j);

    v2=f(i+1,j);

    pip=0;

    value = 0.5*(v1+v2);
    
    //bed
    pip=4;
    v1=bed(i,j);

    v2=bed(i+1,j);

    pip=0;

    bedvalue = 0.5*(v1+v2);
    
    j=jj;
    }
    
    if(p->j_dir==1)
    {
    pip=4;
    v1=f(i,j);

    v2=f(i+1,j);

    v3=f(i,j+1);

    v4=f(i+1,j+1);
    pip=0;

    value = 0.25*(v1+v2+v3+v4);
    
    //bed
    pip=4;
    v1=bed(i,j);

    v2=bed(i+1,j);
    
    v3=bed(i,j+1);

    v4=bed(i+1,j+1);
    pip=0;

    bedvalue = 0.25*(v1+v2+v3+v4);
    }
    
    
    if(value+p->wd>bedvalue)
    if(value+p->wd-bedvalue<wd_criterion)
    value=value-3.0*wd_criterion;
    
  

    return value;
}

double interpolation::sl_ipolint(sliceint &f)
{
    v1=v2=v3=v4=0;

    pip=4;
    v1=double(f(i,j));

    v2=double(f(i+1,j));

    v3=double(f(i,j+1));

    v4=double(f(i+1,j+1));
    pip=0;

    value = 0.25*(v1+v2+v3+v4);

    return value;
}
