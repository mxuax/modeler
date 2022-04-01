# modeler
In this project you define a 3D model and controls for moving it, and then display and operate the model.  The project skeleton contains the framework code for the UI as well as some functions for drawing primitives. You will specify your model in a separate source file that will be compiled and linked in with the existing code.
You might notice after building your model that it's difficult to have very "goal-oriented" motion. Given a model of a human, for instance, if the goal is to move the hand to a certain coordinate, we might have to change the shoulder angle, elbow angle -- maybe even the angle of the knees if the feet are constrained to one position. Implement a method, given a set of position constraints like

left foot is at (1,0,2)
right foot is at (3,0,4)
left hand is at (7,8,2)
that computes the intermediate angles necessary such that all constrains are satisfied (or, if the constraints can not be satisfied, the square of the distance violations is minimized)
