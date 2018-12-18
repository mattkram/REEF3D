/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2018 Hans Bihs

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

#include"fnpf_convection.h"
#include"increment.h"

#ifndef FNPF_DISCRETE_WEIGHTS_H_
#define FNPF_DISCRETE_WEIGHTS_H_

using namespace std;

class fnpf_discrete_weights : public increment
{
public:
	fnpf_discrete_weights(lexer*);
	virtual ~fnpf_discrete_weights();

    void ck_weights_x(lexer*, double*, int, int);
	void ck_weights_y(lexer*, double*, int, int);
    void ck_weights_z(lexer*, double*, int, int);

private:
   

};

#endif