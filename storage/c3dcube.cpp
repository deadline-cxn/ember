#include "c3dcube.h"

C3dCube::C3dCube() {
    pNext=NULL;

    cr=(float)rand()/((float)RAND_MAX/0.02f);
    cg=(float)rand()/((float)RAND_MAX/0.02f);
    cb=(float)rand()/((float)RAND_MAX/0.02f);

    x=(float)rand()/((float)RAND_MAX);
    y=(float)rand()/((float)RAND_MAX);
    z=(float)rand()/((float)RAND_MAX);

    xi=(float)rand()/((float)RAND_MAX/0.05f);
    yi=(float)rand()/((float)RAND_MAX/0.05f);
    zi=(float)rand()/((float)RAND_MAX/0.05f);

    xs=42.0f+(float)rand()/((float)RAND_MAX/42.0f);
    ys=42.0f+(float)rand()/((float)RAND_MAX/42.0f);
    zs=128.0f+(float)rand()/((float)RAND_MAX/12.0f);

    xspini=(float)rand()/((float)RAND_MAX/3.8f);
    yspini=(float)rand()/((float)RAND_MAX/3.8f);
    zspini=(float)rand()/((float)RAND_MAX/3.8f);
}

C3dCube::~C3dCube() {

}

void C3dCube::Update() {
    if((rd==0) || (cr<0)) rd=(float)rand()/((float)RAND_MAX/0.05f);
    if((gd==0) || (cg<0)) gd=(float)rand()/((float)RAND_MAX/0.05f);
    if((bd==0) || (cb<0)) bd=(float)rand()/((float)RAND_MAX/0.05f);

    if(cr>1.0f) { rd=-(float)rand()/((float)RAND_MAX/0.05f); cr=1.0f; }
    if(cg>1.0f) { gd=-(float)rand()/((float)RAND_MAX/0.05f); cg=1.0f; }
    if(cb>1.0f) { bd=-(float)rand()/((float)RAND_MAX/0.05f); cb=1.0f; }

    x+=xi;
    y+=yi;
    z+=zi;

    xx=sin(x)*xs;
    yy=cos(y)*ys;
    zz=sin(z)*zs;

    cr+=rd;
    cg+=gd;
    cb+=bd;

    xspin+=xspini;
    yspin+=yspini;
    zspin+=zspini;
}

void C3dCube::Draw(){

    glLoadIdentity();

    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,1.0f);

    glTranslated(xx,yy,zz);

    glRotatef(xspin,1.0,0,0);
    glRotatef(yspin,0,1.0,0);
    glRotatef(zspin,0,0,1.0);

    glBegin(GL_QUADS);

        glColor3f(cr,cg,cb);
        // Front Face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad

        glColor3f(cg,cb,cr);
        // Back Face
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad

        glColor3f(cb,cr,cg);
        // Top Face
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad

        glColor3f(cr,cb,cg);
        // Bottom Face
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad

        glColor3f(cb,cg,cr);
        // Right face
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
        // Left Face

        glColor3f(cg,cr,cb);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glEnd();
/*  // enable vertex arrays
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        // before draw, specify vertex arrays
        glNormalPointer(GL_FLOAT, 0, Cubenormals);
        glColorPointer(3, GL_FLOAT, 0, Cubecolors);
        glVertexPointer(3, GL_FLOAT, 0, Cubevertices);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);*/
}
