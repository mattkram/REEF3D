/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2023 Hans Bihs

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

#include"nhflow_sigma.h"
#include"lexer.h"
#include"fdm_nhf.h"
#include"ghostcell.h"
#include"fnpf_ddx_cds2.h"
#include"fnpf_ddx_cds4.h"
#include"fnpf_cds2.h"
#include"fnpf_cds4.h"

#define WLVL (fabs(d->WL(i,j))>0.00005?d->WL(i,j):1.0e20)
#define HX (fabs(d->hx(i,j))>1.0e-20?d->hx(i,j):1.0e20)
#define HXP (fabs(0.5*(d->WL(i,j)+d->WL(i+1,j)))>1.0e-20?0.5*(d->WL(i,j)+d->WL(i+1,j)):1.0e20)
#define HY (fabs(d->hy(i,j))>1.0e-20?d->hy(i,j):1.0e20)

void nhflow_sigma::sigma_update(lexer *p, fdm_nhf *d, ghostcell *pgc, slice &eta, slice &eta_n, double alpha)
{
    double wl,sigval;
    double bx,by,ex,ey;
    
    // calculate: Ex,Ey,Exx,Eyy
    // 3D
    if(p->i_dir==1 && p->j_dir==1)
    SLICELOOP4
    {
    d->Ex(i,j) = sx(eta);
    d->Ey(i,j) = sy(eta);
    
    d->Exx(i,j) = sxx(eta);
    d->Eyy(i,j) = syy(eta);
    }
    
    // 2D
    if(p->j_dir==0)
    SLICELOOP4
    {
    d->Ex(i,j) = sx(eta);    
    d->Exx(i,j) = sxx(eta);
    }
    
    SLICELOOP4
    if(p->wet[IJ]==0 || p->wet[Im1J]==0 || p->wet[Ip1J]==0 || p->wet[IJm1]==0 || p->wet[IJp1]==0)
    {
    d->Ex(i,j)=0.0;
    d->Ey(i,j)=0.0;
    }
        
    pgc->gcsl_start4(p,d->Ex,1);
    pgc->gcsl_start4(p,d->Ey,1);
    
    // calculate: Bx,By,Bxx,Byy
    // 3D
    if(p->j_dir==1)
    SLICELOOP4
    {
    d->Bx(i,j) = sx(d->depth);
    d->By(i,j) = sy(d->depth);
    
    d->Bxx(i,j) = sxx(d->depth);
    d->Byy(i,j) = syy(d->depth);
    }

    // 2D
    if(p->j_dir==0 && p->A312!=1)
    SLICELOOP4
    {
    d->Bx(i,j) = sx(d->depth);    
    d->Bxx(i,j) = sxx(d->depth);
    }
    
    SLICELOOP4
    if(p->wet[IJ]==0 || p->wet[Im1J]==0 || p->wet[Ip1J]==0 || p->wet[IJm1]==0 || p->wet[IJp1]==0)
    {
    d->Bx(i,j)=0.0;
    d->By(i,j)=0.0;
    }

    pgc->gcsl_start4(p,d->Bx,1);
    pgc->gcsl_start4(p,d->By,1);
    
    // -----------------------------------------------------
    
    // sigx
    FLOOP
    {
    if(p->wet[IJ]==0)
    p->sigx[FIJK] = 0.0;
    
    if(p->wet[IJ]==1)
    p->sigx[FIJK] = (1.0 - p->sig[FIJK])*(d->Bx(i,j)/WLVL) - p->sig[FIJK]*(d->Ex(i,j)/WLVL);
    }
    
    // sigy
    FLOOP
    {
    if(p->wet[IJ]==0)
    p->sigy[FIJK] = 0.0;
    
    if(p->wet[IJ]==1)
    p->sigy[FIJK] = (1.0 - p->sig[FIJK])*(d->By(i,j)/WLVL) - p->sig[FIJK]*(d->Ey(i,j)/WLVL);
    }
    
    // sigxy4
    LOOP
    {
    sigval = 0.5*(p->sig[FIJK]+p->sig[FIJKp1]);
    
    if(p->wet[IJ]==0)
    {
    p->sigx4[IJK] = 0.0;
    p->sigy4[IJK] = 0.0;
    }
    
    if(p->wet[IJ]==1)
    {
    p->sigx4[IJK] = (1.0 - sigval)*(d->Bx(i,j)/WLVL) - sigval*(d->Ex(i,j)/WLVL);
    p->sigy4[IJK] = (1.0 - sigval)*(d->By(i,j)/WLVL) - sigval*(d->Ey(i,j)/WLVL);
    }
    }
    
    // sigz
    SLICELOOP4
    {
    wl = MAX(0.00005, eta(i,j) + p->wd - d->bed(i,j));
    wl = (fabs(wl)>1.0e-20?wl:1.0e20);
    
    if(p->wet[IJ]==0)
    p->sigz[IJ] = 0.0;
    
    if(p->wet[IJ]==1)
    p->sigz[IJ] = 1.0/wl;
    }

    // sigt
    FLOOP
    p->sigt[FIJK] = -(p->sig[FIJK]/WLVL)*d->detadt(i,j);
    // p->sigt[FIJK] = -(p->sig[FIJK]/WLVL)*(d->WL_n1(i,j)-d->WL_n0(i,j))/(p->dt);
    // p->sigt[FIJK] = -(p->sig[FIJK]/WLVL)*d->detadt(i,j);

    // sigxx
    FLOOP
    {
        p->sigxx[FIJK] = ((1.0 - p->sig[FIJK])/WLVL)*(d->Bxx(i,j) - pow(d->Bx(i,j),2.0)/WLVL) // xx
        
                      - (p->sig[FIJK]/WLVL)*(d->Exx(i,j) - pow(d->Ex(i,j),2.0)/WLVL)
                      
                      - (p->sigx[FIJK]/WLVL)*(d->Bx(i,j) + d->Ex(i,j))
                      
                      - ((1.0 - 2.0*p->sig[FIJK])/pow(WLVL,2.0))*(d->Bx(i,j)*d->Ex(i,j))
                      
                      
                      + ((1.0 - p->sig[FIJK])/WLVL)*(d->Byy(i,j) - pow(d->By(i,j),2.0)/WLVL) // yy
        
                      - (p->sig[FIJK]/WLVL)*(d->Eyy(i,j) - pow(d->Ey(i,j),2.0)/WLVL)
                      
                      - (p->sigy[FIJK]/WLVL)*(d->By(i,j) + d->Ey(i,j))
                      
                      - ((1.0 - 2.0*p->sig[FIJK])/pow(WLVL,2.0))*(d->By(i,j)*d->Ey(i,j));
    }
    
    
    // sig BC
    SLICELOOP4
    {
        k=0;
        if(p->nb5==-2)
        {
            p->sigx[FIJKm1] = p->sigx[FIJK];
            p->sigx[FIJKm2] = p->sigx[FIJK];
            p->sigx[FIJKm3] = p->sigx[FIJK];
        }
        
        k=p->knoz;
        if(p->nb6==-2)
        {
            p->sigx[FIJKp1] = p->sigx[FIJK];
            p->sigx[FIJKp2] = p->sigx[FIJK];
            p->sigx[FIJKp3] = p->sigx[FIJK];
        } 
    }
    
    SLICELOOP4
    {
        k=0;
        if(p->nb5==-2)
        {
            p->sigy[FIJKm1] = p->sigy[FIJK];
            p->sigy[FIJKm2] = p->sigy[FIJK];
            p->sigy[FIJKm3] = p->sigy[FIJK];
        }
        
        k=p->knoz;
        if(p->nb6==-2)
        {
            p->sigy[FIJKp1] = p->sigy[FIJK];
            p->sigy[FIJKp2] = p->sigy[FIJK];
            p->sigy[FIJKp3] = p->sigy[FIJK];
        } 
    }
    
    SLICELOOP4
    {
        k=0;
        if(p->nb5==-2)
        {
            p->sigxx[FIJKm1] = p->sigxx[FIJK];
            p->sigxx[FIJKm2] = p->sigxx[FIJK];
            p->sigxx[FIJKm3] = p->sigxx[FIJK];
        }
        
        k=p->knoz;
        if(p->nb6==-2)
        {
            p->sigxx[FIJKp1] = p->sigxx[FIJK];
            p->sigxx[FIJKp2] = p->sigxx[FIJK];
            p->sigxx[FIJKp3] = p->sigxx[FIJK];
        } 
    }
    
    
    FLOOP
    p->ZSN[FIJK] = p->ZN[KP]*d->WL(i,j) + d->bed(i,j);
    
    LOOP
    p->ZSP[IJK]  = p->ZP[KP]*d->WL(i,j) + d->bed(i,j);
    
    //FLOOP
    //d->test[FIJK] = p->sigx[FIJK];
    
    //pgc->start5V(p,d->test,1);

    
    pgc->start7S(p,p->sigx,1);
    pgc->start7S(p,p->sigy,1);
    pgc->start7S(p,p->sigxx,1);
    pgc->start7S(p,p->sigt,1);
    pgc->start7S(p,p->ZSN,1);
    pgc->start5V(p,p->sigx4,1);
    pgc->start5V(p,p->sigy4,1);
    pgc->gcslparaxijk(p, p->sigz, 1);
    
}

void nhflow_sigma::omega_update(lexer *p, fdm_nhf *d, ghostcell *pgc, double *U, double *V, double *W, slice &eta, slice &eta_n, double alpha)
{ 
    double wval,Pval,Qval,Rval;
    
    LOOP
    {
        if(p->A517==1)
        {
        if(U[IJK]>=0.0)
        Pval=0.5*(U[Im1JK] + U[IJK]);
            
        if(U[IJK]<0.0)
        Pval=0.5*(U[IJK] + U[Ip1JK]);
        
        
        if(V[IJK]>=0.0)
        Qval=0.5*(V[IJm1K] + V[IJK]);
            
        if(V[IJK]<0.0)
        Qval=0.5*(V[IJK] + V[IJp1K]);
        
        
        if(W[IJK]>=0.0)
        Rval=0.5*(W[IJKm1] + W[IJK]);
            
        if(W[IJK]<0.0)
        Rval=0.5*(W[IJK] + W[IJKp1]);
        }
        
        if(p->A517==2)
        {
        Pval = U[IJK];
        Qval = V[IJK];
        Rval = W[IJK];
        }
    
    // omega
        d->omega[IJK] =    0.5*(p->sigt[FIJK]+p->sigt[FIJKp1])
                        
                        +  Pval*p->sigx4[IJK]
                        
                        +  Qval*p->sigy4[IJK]
                        
                        +  Rval*p->sigz[IJ];
    }
    
    
    if(p->A517==1 || p->A517==2)
    FLOOP
    {
        if(p->A517==1)
        {
        if(U[IJK]>=0.0)
        Pval=0.5*(U[Im1JK] + U[IJK]);
            
        if(U[IJK]<0.0)
        Pval=0.5*(U[IJK] + U[Ip1JK]);
        
        
        if(V[IJK]>=0.0)
        Qval=0.5*(V[IJm1K] + V[IJK]);
            
        if(V[IJK]<0.0)
        Qval=0.5*(V[IJK] + V[IJp1K]);
        
        
        if(W[IJK]>=0.0)
        Rval=0.5*(W[IJKm1] + W[IJK]);
            
        if(W[IJK]<0.0)
        Rval=0.5*(W[IJK] + W[IJKp1]);
        }
        
        if(p->A517==2)
        {
        Pval = 0.5*(U[IJK]+U[IJKm1]);
        Qval = 0.5*(V[IJK]+V[IJKm1]);
        Rval = 0.5*(W[IJK]+W[IJKm1]);
        }
    
    // omega
        d->omegaF[FIJK] =    p->sigt[FIJK]
                        
                        +  Pval*p->sigx[FIJK]
                        
                        +  Qval*p->sigy[FIJK]
                        
                        +  Rval*p->sigz[IJ];
                        
        //if(p->wet[IJ] != p->wet_n[IJ])
        //d->omegaF[FIJK]=0.0;
    }
    
    if(p->A517==3)
    {
    FLOOP
    d->omegaF[FIJK] = 0.0;
    
    LOOP
    d->omegaF[FIJKp1] =    d->omegaF[FIJK]
                        
                        - p->DZN[KP]*(d->detadt(i,j) 
                        
                        + ((d->Fx[IJK] - d->Fx[Im1JK])/p->DXN[IP]  + (d->Fy[IJK] - d->Fy[IJm1K])/p->DYN[JP]*p->y_dir) );
                        
    }
      
    GC4LOOP
    if(p->gcb4[n][3]==6 && p->gcb4[n][4]==3)
    {
    i=p->gcb4[n][0];
    j=p->gcb4[n][1];
    k=p->gcb4[n][2];
    
        d->omega[IJK] =  0.0;
        d->omega[IJKp1] =  0.0;
        d->omega[IJKp2] =  0.0;
        d->omega[IJKp3] =  0.0;
        
        k+=1;
        
        d->omegaF[FIJKp1] =  0.0;
        d->omegaF[FIJKp2] =  0.0;
        d->omegaF[FIJKp3] =  0.0;
        
    }
    
    GC4LOOP
    if(p->gcb4[n][3]==5 && p->gcb4[n][4]==21)
    {
    i=p->gcb4[n][0];
    j=p->gcb4[n][1];
    k=p->gcb4[n][2];
        
        d->omega[IJK] =  0.0;
        d->omega[IJKm1] =  0.0;
        d->omega[IJKm2] =  0.0;
        d->omega[IJKm3] =  0.0;
        
        
        d->omegaF[FIJKm1] =  0.0;
        d->omegaF[FIJKm2] =  0.0;
        d->omegaF[FIJKm3] =  0.0;
    }
    
    LOOP
    if(p->wet[IJ]==0)
    d->omega[IJK] = 0.0;
    
    FLOOP
    if(p->wet[IJ]==0)
    d->omegaF[FIJK] = 0.0;
    
    pgc->start3V(p,d->omega,17);
    pgc->start7S(p,d->omegaF,17);
    
    SLICELOOP4
    d->test2D(i,j)=0.0;
    
    k=p->knox-1;
    SLICELOOP4
    d->test2D(i,j)=p->sigx4[IJK];
    //d->test2D(i,j) = MAX(d->test2D(i,j),fabs(d->omegaF[FIJK]));
    
    pgc->gcsl_start4(p,d->test2D,1);
}



