#include <limits>

#include "Camera.h"

namespace urchin
{

	/**
	* @param angle Angle of the field of view (fovy)
	*/
	Camera::Camera(float angle, float nearPlane, float farPlane) :
			mView(Matrix4<float>()),
			mProjection(Matrix4<float>()),
			angle(angle),
			nearPlane(nearPlane),
			farPlane(farPlane),
			baseFrustum(Frustum<float>(angle, 1.0f, nearPlane, farPlane)),
			frustum(Frustum<float>(angle, 1.0f, nearPlane, farPlane)),
			position(Point3<float>(0.0f, 0.0f, 0.0f)),
			view(Point3<float>(0.0f, 0.0f, -1.0f)),
			up(Vector3<float>(0.0f, 1.0f, 0.0f)),
			currentRotationX(0.0f),
			maxRotationX(DEFAULT_MAX_ROTATION_X),
			distance(0.0f),
			bUseMouse(false),
			mouseSensitivity(DEFAULT_MOUSE_SENSITIVITY),
			middleScreenX(0),
			middleScreenY(0),
			oldMouseX(0),
			oldMouseY(0)
	{

	}

	void Camera::onResize(unsigned int width, unsigned int height)
	{
		middleScreenX = width/2;
		middleScreenY = height/2;

		if(bUseMouse)
		{
			moveMouse(middleScreenX, middleScreenY);
		}

		//projection matrix
		float fov = 1.0f / (float)std::tan((angle * PI_VALUE) / 360.0f);
		float ratio = (float)width/(float)height;
		mProjection.setValues(
			fov/ratio, 	0, 		0, 		0,
			0, 				fov, 	0, 		0,
			0, 				0, 		(farPlane+nearPlane)/(nearPlane-farPlane),  (2.0*farPlane*nearPlane)/(nearPlane-farPlane),
			0, 				0, 		-1,		0);

		//frustum
		baseFrustum.buildFrustum(angle, ratio, nearPlane, farPlane);
		frustum = baseFrustum * mView.inverse();
	}

	void Camera::useMouseToMoveCamera(bool use)
	{
		if(use)
		{
            if(middleScreenX!=0 || middleScreenY!=0)
            {
                moveMouse(middleScreenX, middleScreenY);
            }
		}else
		{
			moveMouse(oldMouseX, oldMouseY);
		}
		
		bUseMouse = use;
	}

	bool Camera::isUseMouseToMoveCamera() const
	{
		return bUseMouse;
	}

	void Camera::setMouseSensitivity(float mouseSensitivity)
	{
		this->mouseSensitivity = mouseSensitivity;
	}

	/**
	* @param distance Distance between the camera and the rotation point (0 : first person camera | >0 : third person camera)
	*/
	void  Camera::setDistance(float distance)
	{
		this->distance = distance;
	}

	void  Camera::setMaxRotationX(float maxRotationX)
	{
		this->maxRotationX = maxRotationX;
	}

	const Matrix4<float> &Camera::getViewMatrix() const
	{
		return mView;
	}

	const Matrix4<float> &Camera::getProjectionMatrix() const
	{
		return mProjection;
	}

	const Frustum<float> &Camera::getFrustum() const
	{
		return frustum;
	}

	const Point3<float> &Camera::getPosition() const
	{	
		return position;
	}

	const Point3<float> &Camera::getView() const
	{
		return view;
	}

	const Vector3<float> &Camera::getUp() const
	{
		return up;
	}

	float Camera::getAngle() const
	{
		return angle;
	}

	float Camera::getNearPlane() const
	{
		return nearPlane;
	}

	float Camera::getFarPlane() const
	{
		return farPlane;
	}

	void Camera::moveTo(const Point3<float> &newPos)
	{
        Vector3<float> axis = position.vector(newPos);
        position = newPos;
        view = view.translate(axis);

        updateMatrix();
	}

	void Camera::moveZ(float dist)
	{
        Vector3<float> axis = position.vector(view).normalize();
        axis = axis * dist;

        position = position.translate(axis);
        view = view.translate(axis);

        updateMatrix();
	}

	void Camera::moveX(float dist)
	{
        Vector3<float> axis = (view.vector(position)).crossProduct(up).normalize();
        axis = axis * dist;

        position = position.translate(axis);
        view = view.translate(axis);

        updateMatrix();
	}

	/**
	* @param angle Angle of the rotation in radian
	* @param x X coordinate of the vector where the camera turns around
	* @param y Y coordinate of the vector where the camera turns around
	* @param z Z coordinate of the vector where the camera turns around
	*/
	void Camera::rotate(const Quaternion<float> &quatRotation)
	{
        Point3<float> pivot;
        if (std::fabs(distance) > std::numeric_limits<float>::epsilon())
        {
            Vector3<float> axis = position.vector(view).normalize();
            pivot = position.translate(axis * distance);
        } else
        {
            pivot = position;
        }

        //moves view point
        Vector3<float> axis = pivot.vector(view);
        Quaternion<float> quatView(axis.X, axis.Y, axis.Z, 0.0f);
        const Quaternion<float> &resultView = (quatRotation * quatView.normalize()) * quatRotation.conjugate();
        view.X = resultView.X + pivot.X;
        view.Y = resultView.Y + pivot.Y;
        view.Z = resultView.Z + pivot.Z;

        //moves up vector
        Quaternion<float> quatUp(up.X, up.Y, up.Z, 0.0f);
        const Quaternion<float> &resultUp = (quatRotation * quatUp.normalize()) * quatRotation.conjugate();
        up.X = resultUp.X;
        up.Y = resultUp.Y;
        up.Z = resultUp.Z;

        //moves position point
        if (std::fabs(distance) > std::numeric_limits<float>::epsilon())
        {
            axis = pivot.vector(position);
            Quaternion<float> quatPosition(axis.X, axis.Y, axis.Z, 0.0f);
            const Quaternion<float> &resultPosition = (quatRotation * quatPosition.normalize()) * quatRotation.conjugate();

            position.X = resultPosition.X + pivot.X;
            position.Y = resultPosition.Y + pivot.Y;
            position.Z = resultPosition.Z + pivot.Z;
        }

        updateMatrix();
	}

	void Camera::onKeyDown(unsigned int)
	{
		//do nothing
	}

	void Camera::onKeyUp(unsigned int)
	{
		//do nothing
	}

	void Camera::onMouseMove(int mouseX, int mouseY)
	{
        if(mouseX > 0 || mouseY > 0)
        {
            if (!bUseMouse)
            {
                oldMouseX = static_cast<unsigned int>(mouseX);
                oldMouseY = static_cast<unsigned int>(mouseY);
                return;
            }
            if ((mouseX == middleScreenX) && (mouseY == middleScreenY))
            {
                return;
            }

            //move the mouse back to the middle of the screen
            moveMouse(middleScreenX, middleScreenY);

            //vector that describes mousePosition - center
            Vector2<float> mouseDirection;
            mouseDirection.X = (float)(static_cast<int>(middleScreenX) - mouseX) * mouseSensitivity;
            mouseDirection.Y = (float)(static_cast<int>(middleScreenY) - mouseY) * mouseSensitivity;

            //we don't want to rotate up/down more than "MaxRotationX" percent
            Vector3<float> axis = position.vector(view).normalize();
            currentRotationX = Vector3<float>(0.0, 1.0, 0.0).dotProduct(axis);

            currentRotationX += mouseDirection.Y;
            if (currentRotationX > 0.0 && currentRotationX > maxRotationX)
            {
                mouseDirection.Y -= (currentRotationX - maxRotationX);
                currentRotationX = maxRotationX;
            } else if (currentRotationX < 0.0 && currentRotationX < -maxRotationX)
            {
                mouseDirection.Y -= (currentRotationX + maxRotationX);
                currentRotationX = -maxRotationX;
            }

            //get the axis to rotate around the x axis.
            axis = ((position.vector(view)).crossProduct(up)).normalize();

            //rotate around the y and x axis
            rotate(Quaternion<float>(axis, mouseDirection.Y));
            rotate(Quaternion<float>(Vector3<float>(0.0, 1.0, 0.0), mouseDirection.X));

            updateMatrix();
        }
	}

	void Camera::updateMatrix()
	{
		 //gluLookAt :
		const Vector3<float> &f = position.vector(view).normalize();
		const Vector3<float> &s = f.crossProduct(up).normalize();
		const Vector3<float> &u = s.crossProduct(f).normalize();

		Matrix4<float> M(
				s[0], s[1], s[2], 0,
				u[0], u[1], u[2], 0,
				-f[0], -f[1], -f[2], 0,
				0, 0, 0, 1);

		Matrix4<float> eye;
		eye.buildTranslation(-position.X, -position.Y, -position.Z);
		mView = M * eye;

		frustum = baseFrustum * mView.inverse();
	}

}
