#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glext.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

class C3dCube {
public:
C3dCube();
~C3dCube();

void Draw();
void Update();

C3dCube *pNext;

float x,y,z,xx,yy,zz,xi,yi,zi,xs,ys,zs;
float xspin,yspin,zspin,xspini,yspini,zspini;
float cr,cg,cb,rd,gd,bd;

};
