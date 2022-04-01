#include "solver.h"

#include <math.h>

#define PI 3.14159
Solver::Solver() {}
Solver::~Solver() {}
Vec4f matrix_multipule(vector<vector<float>> M, Vec4f position) {
	Vec4f new_P{0,0,0,0};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			new_P[i] += position[j] * M[i][j];
		}
	}
	return new_P;
}

double dist(Vec4f position_1, Vec4f position_2)
{
	return ((position_1[0]- position_2[0]) * (position_1[0] - position_2[0]) + 
		(position_1[1] - position_2[1]) * (position_1[1] - position_2[1]) + 
		(position_1[2] - position_2[2]) * (position_1[2] - position_2[2]));
}

// take care when use this function to trans the tar_position as the angle should be negative 
Vec4f Solver::Solver_POS_rotate(Vec4f position, int angle, int rotate_type) {
	float sin_v = sin(angle* PI / 180.f);
	float cos_v = cos(angle * PI / 180.f);
	
	// x
	if (rotate_type == 0) {
		vector<vector<float>> M = {
			{1,0,0,0},
			{0,cos_v,-sin_v,0},
			{0,sin_v,cos_v,0},
			{0,0,0,1}
		};
		return matrix_multipule(M, position);
	}
	// y
	else if (rotate_type == 1) {
		vector<vector<float>> M= {
			{cos_v,0,sin_v,0},
			{0,1,0,0},
			{-sin_v,0,cos_v,0},
			{0,0,0,1}
		};
		return matrix_multipule(M, position);
	}
	//z
	else{
		vector<vector<float>> M= {
			{cos_v,-sin_v,0,0},
			{sin_v,cos_v,0,0},
			{0,0,1,0},
			{0,0,0,1}
		};
		return matrix_multipule(M, position);
	}
	
	

}
Vec4f Solver::Solver_POS_trans(Vec4f position, float x, float y, float z) {
	position[0] = position[0] - x;
	position[1] = position[1] - y;
	position[2] = position[2] - z;
	return position;
}

int Solver::Solver_angle_computation(Vec4f tar_position, int type) {
	int optimal_angle = 0;
	if (type == 0) {
		float min = 100000;
		Vec4f dir= { 1, 0 ,0 ,0 };
		for (int i = ROTATE_0_min; i < ROTATE_0_max; i++) {
			float temp = dist(Solver_POS_rotate(tar_position, i, 1), dir);
			if (temp < min) {
				min = temp;
				optimal_angle = i;				
			}
		}
		return optimal_angle;

	}
	else if(type == 1) {
		// this is a special case
		/*
		float min = 100000;
		Vec4f dir = { -1, 0 ,1 ,0 };
		for (int i = ROTATE_1_min; i < ROTATE_1_max; i++) {
			float temp = dist(Solver_POS_rotate(position, i, 2), Solver_POS_rotate(dir, i, 1));
			if (temp < min) {
				min = temp;
				optimal_angle = i;	
			}
		}
		return optimal_angle;
		*/
		if (tar_position[2] <= 0) return 0;
		else return 180;

	}
	else if (type == 2) {
		float min = 100000;
		Vec4f dir = { 1, 0 ,0 ,0 };
		for (int i = ROTATE_2_min; i < ROTATE_2_max; i++) {
			float temp = dist(Solver_POS_rotate(tar_position, -i, 1), dir);
			if (temp < min) {
				min = temp;
				optimal_angle = i;
			}
		}
		return optimal_angle;

	}
	return -360;
}
float Solver::Solver_height_computation(int length_type) {
	return 0.0;
}