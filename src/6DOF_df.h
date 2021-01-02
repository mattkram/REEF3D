/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2020 Hans Bihs

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
Author: Tobias Martin
--------------------------------------------------------------------*/

#include"6DOF.h"
#include"6DOF_df_object.h"
#include<vector>

class mooring;
class net;

using namespace std;

#ifndef SIXDOF_DF_H_
#define SIXDOF_DF_H_

class sixdof_df : public sixdof
{
public:
	sixdof_df(lexer*, fdm*, ghostcell*);
	virtual ~sixdof_df();
	virtual void start(lexer*,fdm*,ghostcell*,double,vrans*,vector<net*>&);
	virtual void initialize(lexer*,fdm*,ghostcell*,vector<net*>&);
    
    void forcing(lexer*,fdm*,ghostcell*,vrans*,vector<net*>&,double,field&,field&,field&,field1&,field2&,field3&,bool);

private:
   
    int number6DOF;
    vector<sixdof_df_object*> p_df_obj;
};

#endif
