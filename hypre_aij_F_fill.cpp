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

#include"hypre_aij.h"

#ifdef HYPRE_COMPILATION
#include"lexer.h"
#include"fdm.h"
#include"ghostcell.h"


void hypre_aij::fill_matrix_F(lexer* p, ghostcell* pgc, matrix_diag &M, double *f, double *xvec, vec &rhsvec)
{
    
    fieldint4 rownum4(p);
    
    pgc->rownum7_update(p,rownum4);
    pgc->facenbx(p,rownum4,p->range_row4);

	n=0;
	LOOP
	{
	count=0;
	
	val[count] = M.p[n];
	col[count] = rownum4(i,j,k);
	rownum = rownum4(i,j,k);
	++count;
	
	
    if(p->flag4[Im1JK]>0)
	{
	val[count] = M.s[n];
	col[count] = rownum4(i-1,j,k);
	++count;
	}
    
    if(p->flag4[Ip1JK]>0)
	{
	val[count] = M.n[n];
	col[count] = rownum4(i+1,j,k);
	++count;
	}
    
    if(p->flag4[IJm1K]>0)
	{
	val[count] = M.e[n];
	col[count] = rownum4(i,j-1,k);
	++count;
	}
    
    if(p->flag4[IJp1K]>0)
	{
	val[count] = M.w[n];
	col[count] = rownum4(i,j+1,k);
	++count;
	}
    
    if(p->flag4[IJKm1]>0)
	{
	val[count] = M.b[n];
	col[count] = rownum4(i,j,k-1);
	++count;
	}
    
    if(p->flag4[IJKp1]>0)
	{
	val[count] = M.t[n];
	col[count] = rownum4(i,j,k+1);
	++count;
	}
	
	HYPRE_IJMatrixSetValues(A, 1, &count, &rownum, col, val);
	
	++n;
	}
	
	
	HYPRE_IJMatrixAssemble(A);
	HYPRE_IJMatrixGetObject(A, (void**) &parcsr_A);
	
    // vec
	n=0;
	LOOP
	{
		xvec[n] = (i,j,k);
		rows[n] = rownum4(i,j,k);
	++n;
	}

	HYPRE_IJVectorSetValues(b, p->N4_row, rows, rhsvec.V);
	HYPRE_IJVectorSetValues(x, p->N4_row, rows, xvec);
	
	HYPRE_IJVectorAssemble(b);
    HYPRE_IJVectorGetObject(b, (void **) &par_b);
    HYPRE_IJVectorAssemble(x);
    HYPRE_IJVectorGetObject(x, (void **) &par_x);
}


void hypre_aij::fill_matrix_F_13p(lexer* p, ghostcell* pgc, matrix_diag &M, double *f, double *xvec, vec &rhsvec)
{
    fieldint4 rownum4(p);
    
    pgc->rownum7_update(p,rownum4);
    pgc->facenbx(p,rownum4,p->range_row4);
    
    HYPRE_IJMatrixInitialize(A);
    HYPRE_IJVectorInitialize(x);
    HYPRE_IJVectorInitialize(b);
    
	n=0;
	LOOP
	{
	count=0;
	
	val[count] = M.p[n];
	col[count] = rownum4(i,j,k);
	rownum = rownum4(i,j,k);
	++count;
	
	
    if(p->flag4[Im1JK]>0)
	{
	val[count] = M.s[n];
	col[count] = rownum4(i-1,j,k);
	++count;
	}
    
    if(p->flag4[Ip1JK]>0)
	{
	val[count] = M.n[n];
	col[count] = rownum4(i+1,j,k);
	++count;
	}
    
    if(p->flag4[IJm1K]>0)
	{
	val[count] = M.e[n];
	col[count] = rownum4(i,j-1,k);
	++count;
	}
    
    if(p->flag4[IJp1K]>0)
	{
	val[count] = M.w[n];
	col[count] = rownum4(i,j+1,k);
	++count;
	}
    
    if(p->flag4[IJKm1]>0)
	{
	val[count] = M.b[n];
	col[count] = rownum4(i,j,k-1);
	++count;
	}
    
    if(p->flag4[IJKp1]>0)
	{
	val[count] = M.t[n];
	col[count] = rownum4(i,j,k+1);
	++count;
	}
    
    
    // -- 
    if(p->flag4[Im2JK]>0)
	{
	val[count] = M.ss[n];
	col[count] = rownum4(i-2,j,k);
	++count;
	}
    
    if(p->flag4[Ip2JK]>0)
	{
	val[count] = M.nn[n];
	col[count] = rownum4(i+2,j,k);
	++count;
	}
    
    if(p->flag4[IJm2K]>0)
	{
	val[count] = M.ee[n];
	col[count] = rownum4(i,j-2,k);
	++count;
	}
    
    if(p->flag4[IJp2K]>0)
	{
	val[count] = M.ww[n];
	col[count] = rownum4(i,j+2,k);
	++count;
	}
    
    if(p->flag4[IJKm2]>0)
	{
	val[count] = M.bb[n];
	col[count] = rownum4(i,j,k-2);
	++count;
	}
    
    if(p->flag4[IJKp2]>0)
	{
	val[count] = M.tt[n];
	col[count] = rownum4(i,j,k+2);
	++count;
	}

	
	HYPRE_IJMatrixSetValues(A, 1, &count, &rownum, col, val);
	
	++n;
	}
	
	
	HYPRE_IJMatrixAssemble(A);
	HYPRE_IJMatrixGetObject(A, (void**) &parcsr_A);
	
    // vec
	n=0;
	LOOP
	{
		xvec[n] = f[FIJK];
		rows[n] = rownum4(i,j,k);
	++n;
	}

	HYPRE_IJVectorSetValues(b, p->N4_row, rows, rhsvec.V);
	HYPRE_IJVectorSetValues(x, p->N4_row, rows, xvec);
	
	HYPRE_IJVectorAssemble(b);
    HYPRE_IJVectorGetObject(b, (void **) &par_b);
    HYPRE_IJVectorAssemble(x);
    HYPRE_IJVectorGetObject(x, (void **) &par_x);
}

void hypre_aij::fill_matrix_F_19p(lexer* p, ghostcell* pgc, matrix_diag &M, double *f, double *xvec, vec &rhsvec)
{
    fieldint4 rownum4(p);
    
    pgc->rownum7_update(p,rownum4);
    pgc->facenbx(p,rownum4,p->range_row4);
    
    HYPRE_IJMatrixInitialize(A);
    HYPRE_IJVectorInitialize(x);
    HYPRE_IJVectorInitialize(b);
    
	n=0;
	LOOP
	{
	count=0;
	
	val[count] = M.p[n];
	col[count] = rownum4(i,j,k);
	rownum = rownum4(i,j,k);
	++count;
	
	
    if(p->flag4[Im1JK]>0)
	{
	val[count] = M.s[n];
	col[count] = rownum4(i-1,j,k);
	++count;
	}
    
    if(p->flag4[Ip1JK]>0)
	{
	val[count] = M.n[n];
	col[count] = rownum4(i+1,j,k);
	++count;
	}
    
    if(p->flag4[IJm1K]>0)
	{
	val[count] = M.e[n];
	col[count] = rownum4(i,j-1,k);
	++count;
	}
    
    if(p->flag4[IJp1K]>0)
	{
	val[count] = M.w[n];
	col[count] = rownum4(i,j+1,k);
	++count;
	}
    
    if(p->flag4[IJKm1]>0)
	{
	val[count] = M.b[n];
	col[count] = rownum4(i,j,k-1);
	++count;
	}
    
    if(p->flag4[IJKp1]>0)
	{
	val[count] = M.t[n];
	col[count] = rownum4(i,j,k+1);
	++count;
	}
    
    
    // -- 
    if(p->flag4[Im2JK]>0)
	{
	val[count] = M.ss[n];
	col[count] = rownum4(i-2,j,k);
	++count;
	}
    
    if(p->flag4[Ip2JK]>0)
	{
	val[count] = M.nn[n];
	col[count] = rownum4(i+2,j,k);
	++count;
	}
    
    if(p->flag4[IJm2K]>0)
	{
	val[count] = M.ee[n];
	col[count] = rownum4(i,j-2,k);
	++count;
	}
    
    if(p->flag4[IJp2K]>0)
	{
	val[count] = M.ww[n];
	col[count] = rownum4(i,j+2,k);
	++count;
	}
    
    if(p->flag4[IJKm2]>0)
	{
	val[count] = M.bb[n];
	col[count] = rownum4(i,j,k-2);
	++count;
	}
    
    if(p->flag4[IJKp2]>0)
	{
	val[count] = M.tt[n];
	col[count] = rownum4(i,j,k+2);
	++count;
	}
    
    
    // -- 
    if(p->flag4[Im3JK]>0)
	{
	val[count] = M.sss[n];
	col[count] = rownum4(i-3,j,k);
	++count;
	}
    
    if(p->flag4[Ip3JK]>0)
	{
	val[count] = M.nnn[n];
	col[count] = rownum4(i+3,j,k);
	++count;
	}
    
    if(p->flag4[IJm3K]>0)
	{
	val[count] = M.eee[n];
	col[count] = rownum4(i,j-3,k);
	++count;
	}
    
    if(p->flag4[IJp3K]>0)
	{
	val[count] = M.www[n];
	col[count] = rownum4(i,j+3,k);
	++count;
	}
    
    if(p->flag4[IJKm3]>0)
	{
	val[count] = M.bbb[n];
	col[count] = rownum4(i,j,k-3);
	++count;
	}
    
    if(p->flag4[IJKp3]>0)
	{
	val[count] = M.ttt[n];
	col[count] = rownum4(i,j,k+3);
	++count;
	}
	
	HYPRE_IJMatrixSetValues(A, 1, &count, &rownum, col, val);
	
	++n;
	}
	
	
	HYPRE_IJMatrixAssemble(A);
	HYPRE_IJMatrixGetObject(A, (void**) &parcsr_A);
	
    // vec
	n=0;
	LOOP
	{
		xvec[n] = f[FIJK];
		rows[n] = rownum4(i,j,k);
	++n;
	}

	HYPRE_IJVectorSetValues(b, p->N4_row, rows, rhsvec.V);
	HYPRE_IJVectorSetValues(x, p->N4_row, rows, xvec);
	
	HYPRE_IJVectorAssemble(b);
    HYPRE_IJVectorGetObject(b, (void **) &par_b);
    HYPRE_IJVectorAssemble(x);
    HYPRE_IJVectorGetObject(x, (void **) &par_x);
}

#endif