/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2021 Hans Bihs

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

#include"patchBC_interface.h"
#include"increment.h"

using namespace std;

#ifndef PATCHBC_H_
#define PATCHBC_H_

class patchBC : public increment
{
public:
	patchBC(lexer*);
	virtual ~patchBC();
    
    void patchBC_ini(lexer *p, ghostcell *pgc);
    
    // ini
    void patchBC_gcb_convert(lexer *p, ghostcell *pgc);
    void patchBC_IDcount(lexer *p, ghostcell *pgc);
    void patchBC_fillobj(lexer *p, ghostcell *pgc);
    
    // BC update
    void patchBC_inflow(lexer*, fdm*, ghostcell*, field&,field&,field&);
    void patchBC_outflow(lexer*, fdm*, ghostcell*, field&,field&,field&);
    void patchBC_pressure(lexer*, fdm*, ghostcell*, field&);
    void patchBC_waterlevel(lexer*, fdm*, ghostcell*, field&);
    


        
private:
    int q,n,qn,qq,count,ID_count;
    int istart,iend,jstart,jend,kstart,kend;
    
    int *inflow_ID;
    int *outflow_ID;
    
    int geo_count,obj_count;
    int *ID_array;
    
    patch_obj **patch;
    

};

#endif
