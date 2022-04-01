// The sample model.  You should build a file
// very similar to this for when you make your model.
//#include "modelerview.h"
#include "camera.h"
#include <stack> 
#include <vector>
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.h>
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>


#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>

#include "modelerglobals.h"
#include "myPrimitive.h"
#include "solver.h"
// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }
	int ios = 0;
    virtual void draw();
private:
	stack<vector<float>> animation_stack;
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel



//int ios = 0;
void SampleModel::draw()
{
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...

	// we disable the ModelerView::draw(); so that we can implement the light source
    /*ModelerView::draw();*/
	GLfloat lightPosition0[] = { VAL(Light_0_x), VAL(Light_0_y), VAL(Light_0_z), 0 };
	GLfloat lightDiffuse0[] = { 1,1,1,1 };
	GLfloat lightPosition1[] = { VAL(Light_1_x), VAL(Light_1_y), VAL(Light_1_z), 0 };
	GLfloat lightDiffuse1[] = { 1, 1, 1, 1 };
	if (!valid())
	{
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		//glEnable(GL_LIGHT2);
		glEnable(GL_NORMALIZE);
	}
	glViewport(0, 0, w(), h());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, float(w()) / float(h()), 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera->applyViewingTransform();
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
	/*
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse2);
	*/

	if (ModelerApplication::Instance()->GetAnimating()) {
		if (!animation_stack.empty()) {
			for (int i = 0; i < NUMCONTROLS;i++) {
				ModelerApplication::Instance()->SetControlValue(i, animation_stack.top()[i]);				
			}
			animation_stack.pop();
		}
	}
	else {
	//	/*
	//	while (!animation_stack.empty()) {
	//		animation_stack.pop();
	//	}*/
		vector<float> temp;
		for (int i = 0; i < NUMCONTROLS; i++) {
			temp.push_back( ModelerApplication::Instance()->GetControlValue(i));
		}
		if (animation_stack.size() < 1000000) {
			animation_stack.push(temp);
		}
	}
	// draw the target and try to find the solution
	
	//Solver cur_solver;
	Solver current_Solver;
	if (VAL(Target_enable) == 1) {
		
		setAmbientColor(.1f, .1f, .1f);
		setDiffuseColor(0.1, 0.7, 0.4);
		glPushMatrix();
		glTranslated(VAL(Target_x), VAL(Target_y), VAL(Target_z));
		drawSphere(0.3);
		glPopMatrix();
		//compute and set the parameter downwards
		/*
		cur_solver.Solver_angle_computation();
		cur_solver.Solver_height_computation();
		cur_solver.set_parameter();
		for (int i = 0; i < 6; i++) {
			ModelerApplication::Instance()->SetControlValue(i, cur_solver.parameter_list[i]);
		}
		*/

		current_Solver.position[0] = VAL(Target_x);
		current_Solver.position[1] = VAL(Target_y);
		current_Solver.position[2] = VAL(Target_z);
	}
	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	glTranslated(-5,0,-5);
	drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the sample model

	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(0.4, 0.7, 0.2);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	if (VAL(Target_enable) == 1) {
		current_Solver.position = current_Solver.Solver::Solver_POS_trans(current_Solver.position, VAL(XPOS), VAL(YPOS), VAL(ZPOS));
		setAmbientColor(.1f, .1f, .1f);
		setDiffuseColor(0.4, 0.7, 0.2);
	}
		/*
		// draw base
		glPushMatrix();
		glTranslated(-1.5, 0, -2);
		glScaled(3, 1, 4);
		drawBox(1,1,1);
		glPopMatrix();

		// draw cannon
		glPushMatrix();
		// rotate around vertex (0, 1, 0)
		glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
		glRotated(-90, 1.0, 0.0, 0.0);
		// add the thin stick in the center
		drawCylinder(VAL(HEIGHT), 0.1, 0.1);

		glTranslated(0.0, 0.0, VAL(HEIGHT));
		// add the trapezoid on the top
		drawCylinder(1, 1.0, 0.9);

		glTranslated(0.0, 0.0, 0.5);
		glRotated(90, 1.0, 0.0, 0.0);
		// add small gun barrel
		drawCylinder(4, 0.1, 0.2);
		glPopMatrix();
	*/
	// draw base and torus wheel
	glPushMatrix();
	glTranslated(-1.5, 1 + 0.3, -1);

	glPushMatrix();
	glScaled(3, 1, 2);
	drawBox(1, 1, 1);
	glPopMatrix();

	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(0.1, 0.3, 0.4);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0.85, 0, 0);
	draw_torus(0.85, 0.3);
	glTranslatef(3, 0, 0);
	draw_torus(0.85, 0.3);
	glTranslatef(0, 2, 0);
	draw_torus(0.85, 0.3);
	glTranslatef(-3, 0, 0);
	draw_torus(0.85, 0.3);

	glPopMatrix();

	
	
	// robotic arm
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_GREEN);
	glPushMatrix();

	// rotate around vertex (0, 1, 0)
	glTranslated(0.0, 2 + 0.3 / 2, 0);
	// solver check
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(0.2, 0.4, 0.9);
	if (abs(VAL(Target_enable) - 1) < 0.001) {
		current_Solver.position = current_Solver.Solver_POS_trans(current_Solver.position, 0, 2 + 0.3 / 2, 0);
		int angle_0 = current_Solver.Solver_angle_computation(current_Solver.position, 0);
		ModelerApplication::Instance()->SetControlValue(ROTATE_0, -angle_0);
		
	}
	glRotated(VAL(ROTATE_0), 0.0, 1.0, 0.0);
	
	glRotated(-90, 1.0, 0.0, 0.0);
	
	// add the 0 stick on the base
	drawCylinder(VAL(HEIGHT_0), 0.2, 0.2); // will just follow the z as the height
	glTranslated(0.0, 0.0, VAL(HEIGHT_0));
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_RED);
	// solver check
	if (abs(VAL(Target_enable) - 1) < 0.001) {
		current_Solver.position = current_Solver.Solver_POS_rotate(current_Solver.position, -VAL(ROTATE_0), 1);
		current_Solver.position = current_Solver.Solver_POS_rotate(current_Solver.position, 90, 0);
		current_Solver.position = current_Solver.Solver_POS_trans(current_Solver.position, 0.0, 0.0, VAL(HEIGHT_0));
		int angle_1 = current_Solver.Solver_angle_computation(current_Solver.position, 1);
		ModelerApplication::Instance()->SetControlValue(ROTATE_1, angle_1);

	}
	
	
	glRotated(VAL(ROTATE_1), 1.0, 0.0, 0.0);
	glRotated(90, 0.0, 1.0, 0.0);

	// add sphere and the 1 stick
	drawSphere(0.3);
	drawCylinder(VAL(HEIGHT_1), 0.2, 0.2);
	glTranslated(0.0, 0.0, VAL(HEIGHT_1));
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_GREEN);
	//solver check
	if (abs(VAL(Target_enable) - 1) < 0.001) {
		current_Solver.position = current_Solver.Solver_POS_rotate(current_Solver.position, -VAL(ROTATE_1), 0);
		current_Solver.position = current_Solver.Solver_POS_rotate(current_Solver.position, -90, 1);
		current_Solver.position = current_Solver.Solver_POS_trans(current_Solver.position, 0.0, 0.0, VAL(HEIGHT_1));
		int angle_2 = current_Solver.Solver_angle_computation(current_Solver.position, 2);
		ModelerApplication::Instance()->SetControlValue(ROTATE_2, angle_2);
	}
	glRotated(VAL(ROTATE_2), 0.0, 1.0, 0.0);
	glRotated(90, 0.0, 1.0, 0.0);

	// add sphere and the 2 stick
	drawSphere(0.25);
	drawCylinder(VAL(HEIGHT_2), 0.15, 0.15);
	glTranslated(0.0, 0.0, VAL(HEIGHT_2));
	glRotated(VAL(ROTATE_3), 0.0, 0.0, 1.0);
	//glRotated(90, 0.0, 1.0, 0.0);

	// add grill head
	drawSphere(0.15);
	drawCylinder(0.3, 0.15, 0.05);
	glTranslated(0.0, 0.0, 0.3);

	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(0.8, 0.5, 0.0);

	glPushMatrix();
	glTranslated(0.0, 0.05, 0.0);
	glRotated(VAL(ROTATE_4), 0.0, 1.0, 0.0);
	glRotated(90, 1.0, 0.0, 0.0);
	drawCylinder(0.1, 0.1, 0.1);
	drawBox(0.2, 0.1, 0.1);
	glTranslated(0.2, 0.0, 0.0);
	drawBox(0.1, 0.2, 0.1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(0.0, 0.05, 0.0);
	glRotated(-VAL(ROTATE_4), 0.0, 1.0, 0.0);
	glRotated(90, 1.0, 0.0, 0.0);
	drawCylinder(0.1, 0.1, 0.1);
	drawBox(-0.2, 0.1, 0.1);
	glTranslated(-0.3, 0.0, 0.0);
	drawBox(0.1, 0.2, 0.1);
	glPopMatrix();

	glRotated(90, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslated(0.0, 0.05, 0.0);
	glRotated(VAL(ROTATE_4), 0.0, 1.0, 0.0);
	glRotated(90, 1.0, 0.0, 0.0);
	drawCylinder(0.1, 0.1, 0.1);
	drawBox(0.2, 0.1, 0.1);
	glTranslated(0.2, 0.0, 0.0);
	drawBox(0.1, 0.2, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.05, 0.0);
	glRotated(-VAL(ROTATE_4), 0.0, 1.0, 0.0);
	glRotated(90, 1.0, 0.0, 0.0);
	drawCylinder(0.1, 0.1, 0.1);
	drawBox(-0.2, 0.1, 0.1);
	glTranslated(-0.3, 0.0, 0.0);
	drawBox(0.1, 0.2, 0.1);
	glPopMatrix();

	glPopMatrix();
	
	glPopMatrix();
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0.0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0.0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0.0);
    controls[HEIGHT_0] = ModelerControl("Height_0", 2, 3, 0.1f, 2);
	controls[ROTATE_0] = ModelerControl("Rotate_0", -179, 180, 1, 0.0);
	controls[HEIGHT_1] = ModelerControl("Height_1", 1, 1.5, 0.05f, 1.0);
	controls[ROTATE_1] = ModelerControl("Rotate_1", -179, 180, 1, 0.0);
	controls[HEIGHT_2] = ModelerControl("Height_2", 0.5, 1.2, 0.05f, 1.0);
	controls[ROTATE_2] = ModelerControl("Rotate_2", -180, 0, 1, 0.0);
	controls[ROTATE_3] = ModelerControl("Rotate_3", -179, 0, 1, 0.0);
	controls[ROTATE_4] = ModelerControl("Rotate_4", -45, 45, 1, 0.0);
	controls[Light_0_x] = ModelerControl("Light_0_x", -5, 5, 1, 4);
	controls[Light_0_y] = ModelerControl("Light_0_y", -5, 5, 1, 2);
	controls[Light_0_z] = ModelerControl("Light_0_z", -5, 5, 1, -4);
	controls[Light_1_x] = ModelerControl("Light_1_x", -5, 5, 1, -2);
	controls[Light_1_y] = ModelerControl("Light_1_y", -5, 5, 1, 1);
	controls[Light_1_z] = ModelerControl("Light_1_z", -5, 5, 1, 5);

	controls[Target_enable] = ModelerControl("Target_enable", 0, 1, 1, 0);
	controls[Target_x] = ModelerControl("Target_x", -5, 7, 0.1, 3);
	controls[Target_y] = ModelerControl("Target_y", -5, 7, 0.1, 3);
	controls[Target_z] = ModelerControl("Target_z", -5, 7, 0.1, 3);
	//controls[ROTATE_5] = ModelerControl("Rotate_5", 0, 40, 1, 0.0);
	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
