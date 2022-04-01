#include "myPrimitive.h"
#include "math.h"
void draw_torus(float r_1, float d_2){
	ModelerDrawState* mds = ModelerDrawState::Instance();
    int divisions;

    switch (mds->m_quality)
    {
    case HIGH:
        divisions = 32; break;
    case MEDIUM:
        divisions = 20; break;
    case LOW:
        divisions = 12; break;
    case POOR:
        divisions = 8; break;
    }
    float cylinder_Height = r_1 * 2 * sin((3.1415926)/ divisions);
    glPushMatrix();
    for (int i = 0; i < divisions; i++) {
        
        drawSphere(d_2);
        drawCylinder(cylinder_Height, d_2, d_2);
        glTranslatef(0, 0, cylinder_Height);
        glRotatef(static_cast<GLfloat>(-360) / divisions, 0, 1, 0);

    }
    glPopMatrix();
}