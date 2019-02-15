#ifndef CAMERAMOVEMENT_H
#define CAMERAMOVEMENT_H

namespace Glumer
{
	namespace CameraMovement
	{
		void PointerMotionChange(unsigned int mouse_x, unsigned int mouse_y, int mouse_z);
		void FirstPersonRightTurn(float step);
		void FirstPersonLeftTurn(float step);
		void FirstPersonAngleChange(float heading_angle_x, float heading_angle_y, float forward_unit);
		void FirstPersonStop(void);
		void FirstPersonUpMove(float step);
		void FirstPersonDownMove(float step);
		void FirstPersonBackMove(float step);
		void FirstPersonForwardMove(float step);
		void FirstPersonStrafeLeft(float step);
		void FirstPersonStrafeRight(float step);
		void SetCameraId(unsigned int newCameraId);


		static const float piover180 = 0.0175f;

		class cMovementDirection
		{
			public:
			enum MovementDirection
			{
				Stop = 0,
				Forward,
				Backwards,
				Up,
				Down,
				Left,
				Right
			};
		};

		
	}

}

#endif
