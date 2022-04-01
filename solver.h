#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include "vec.h"
#include "mat.h"

class Solver
{
	public:
		Vec4f position;

		explicit Solver();
		~Solver();
		Vec4f Solver_POS_rotate(Vec4f position, int angle, int rotate_type);
		Vec4f Solver_POS_trans(Vec4f position, float x, float y, float z);
		int Solver_angle_computation(Vec4f position, int type);
		float Solver_height_computation(int length_type);
		void set_parameter(vector<float>);
	private:

		/*
		2, 3, 0.1f, 2);
		controls[ROTATE_0] = ModelerControl("Rotate_0", -179, 180, 1, 0.0);
		controls[HEIGHT_1] = ModelerControl("Height_1", 1, 1.5, 0.05f, 1.0);
		controls[ROTATE_1] = ModelerControl("Rotate_1", -179, 180, 1, 0.0);
		controls[HEIGHT_2] = ModelerControl("Height_2", 0.3, 0.7, 0.05f, 1.0);
		controls[ROTATE_2] = ModelerControl("Rotate_2", -180, 0
		*/
	

		float HEIGHT_0_min = 2;
		float HEIGHT_0_max = 3;
		int ROTATE_0_min = -179;
		int ROTATE_0_max = 180;
		float HEIGHT_1_min = 1;
		float HEIGHT_1_max = 1.5;
		int ROTATE_1_min = -179;
		int ROTATE_1_max = 180;
		float HEIGHT_2_min = 0.3;
		float HEIGHT_2_max = 0.7;
		int ROTATE_2_min = -180;
		int ROTATE_2_max = 0;
};


#endif