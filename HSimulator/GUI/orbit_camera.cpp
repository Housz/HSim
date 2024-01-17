#include "orbit_camera.h"

namespace HSim
{

	OrbitCamera::OrbitCamera()
		: Camera()
	{
		mFocusDist = 3.0f;
		mEyePos = Vec3f(0.0f, 0.0f, 3.0f);
		mFov = 0.90f;

		this->setClipFar(20.0f);
	}

	Vec3f OrbitCamera::getEyePos() const
	{
		return mEyePos;
	}

	Vec3f OrbitCamera::getTargetPos() const
	{
		return mEyePos + mFocusDist * getViewDir();
	}

	void OrbitCamera::setEyePos(const Vec3f &p)
	{
		// Quaternionf q(0.0f, mRotAxis);
		Quaternionf q(mRotAxis, 0.0f);
		q.w = -q.w;

		// Camera coordinate system
		// Vec3f view = q.rotate(Vec3f(0, 0, 1));
		// Vec3f up = q.rotate(Vec3f(0, 1, 0));
		// Vec3f right = q.rotate(Vec3f(1, 0, 0));
		
		Vec3f view = q.mul(Vec3f(0, 0, 1));
		Vec3f up = q.mul(Vec3f(0, 1, 0));
		Vec3f right = q.mul(Vec3f(1, 0, 0));


		Vec3f tarPos = getTargetPos();

		Vec3f ND = p - tarPos;

		// float d = ND.norm();
		float d = ND.length();

		ND.normalize();
		if (d > mFocusDistMax)
		{
			mEyePos = tarPos - mFocusDistMax * ND;
		}
		else
		{
			mEyePos = p;
		}

		mFocusDist = (mEyePos - tarPos).length();
		mPitch = acosf(-ND.dot(up)) - PI / 2.0f;
		mYaw = -atan2f(ND.dot(right), ND.dot(view));
	}

	void OrbitCamera::setTargetPos(const Vec3f &p)
	{
		Quaternionf q(0.0f, mRotAxis);
		q.w = -q.w;

		// Camera coordinate system
		// Vec3f view = q.rotate(Vec3f(0, 0, 1));
		// Vec3f up = q.rotate(Vec3f(0, 1, 0));
		// Vec3f right = q.rotate(Vec3f(1, 0, 0));
		
		Vec3f view = q.mul(Vec3f(0, 0, 1));
		Vec3f up = q.mul(Vec3f(0, 1, 0));
		Vec3f right = q.mul(Vec3f(1, 0, 0));

		Vec3f tarPos = p;

		Vec3f ND = mEyePos - tarPos;

		float d = ND.length();
		ND.normalize();

		mFocusDist = std::min(mFocusDistMax, (mEyePos - tarPos).length());
		mPitch = acosf(-ND.dot(up)) - PI / 2.0f;
		mYaw = -atan2f(ND.dot(right), ND.dot(view));
	}

	void OrbitCamera::rotate(float dx, float dy)
	{
		float newYaw = mYaw + dx;
		float newPitch = mPitch + dy;

		Quaternionf oldQuat = getQuaternion(mYaw, mPitch);
		oldQuat.w = -oldQuat.w;
		// Vec3f curViewdir = oldQuat.rotate(Vec3f(0, 0, -1));
		Vec3f curViewdir = oldQuat.mul(Vec3f(0, 0, -1));
		Vec3f eyeCenter = mEyePos + mFocusDist * curViewdir;

		Quaternionf newQuat = getQuaternion(newYaw, newPitch);
		Quaternionf q2 = newQuat;
		q2.w = -q2.w;
		Quaternionf qFinal = q2;
		// Quaternionf qFinal = Quaternionf(newPitch, vecX) * q;

		// Vec3f newViewdir = q2.rotate(Vec3f(0, 0, -1));
		Vec3f newViewdir = q2.mul(Vec3f(0, 0, -1));

		mEyePos = eyeCenter - mFocusDist * newViewdir;

		mYaw = newYaw;
		mPitch = newPitch;
	}

	Vec3f OrbitCamera::getViewDir() const
	{
		Quaternionf q = getQuaternion(mYaw, mPitch);
		q.w = -q.w;

		// Vec3f viewdir = q.rotate(Vec3f(0, 0, -1));
		Vec3f viewdir = q.mul(Vec3f(0, 0, -1));
		return viewdir;
	}

	void OrbitCamera::getCoordSystem(Vec3f &view, Vec3f &up, Vec3f &right) const
	{
		Quaternionf q = getQuaternion(mYaw, mPitch);
		q.w = -q.w;

		// view = q.rotate(Vec3f(0, 0, -1));
		// up = q.rotate(Vec3f(0, 1, 0));

		view = q.mul(Vec3f(0, 0, -1));
		up = q.mul(Vec3f(0, 1, 0));

		right = view.cross(up);
	}

	void OrbitCamera::translate(const Vec3f translation)
	{
		Quaternionf q = getQuaternion(mYaw, mPitch);
		q.w = -q.w;

		// Vec3f xax = q.rotate(Vec3f(1, 0, 0));
		// Vec3f yax = q.rotate(Vec3f(0, 1, 0));
		// Vec3f zax = q.rotate(Vec3f(0, 0, 1));
		Vec3f xax = q.mul(Vec3f(1, 0, 0));
		Vec3f yax = q.mul(Vec3f(0, 1, 0));
		Vec3f zax = q.mul(Vec3f(0, 0, 1));

		mEyePos += translation[0] * xax +
				   translation[1] * yax +
				   translation[2] * zax;
	}

	void OrbitCamera::zoom(float amount)
	{
		Quaternionf oldQuat = getQuaternion(mYaw, mPitch);

		// calculate the view direction
		// Quaternionf oldQuat(mRotAngle, mRotAxis);
		oldQuat.w = -oldQuat.w;
		// Vec3f curViewdir = oldQuat.rotate(Vec3f(0, 0, -1));
		Vec3f curViewdir = oldQuat.mul(Vec3f(0, 0, -1));

		Vec3f eyeCenter = mEyePos + mFocusDist * curViewdir;

		float logDist = std::log10(mFocusDist);
		float logMin = std::log10(mFocusDistMin);
		float logMax = std::log10(mFocusDistMax);
		float frac = (logDist - logMax) / (logMax - logMin);

		mFocusDist += mZoomSpeed * amount * std::pow(10.0f, frac);
		mFocusDist = std::min(std::max(mFocusDist, mFocusDistMin), mFocusDistMax);
		mEyePos = eyeCenter - mFocusDist * curViewdir;
	}

	Vec3f OrbitCamera::getPosition(float x, float y)
	{
		float r = x * x + y * y;
		float t = 0.5f * 1 * 1;
		if (r < t)
		{
			Vec3f result(x, y, sqrt(2.0f * t - r));
			result.normalize();
			return result;
		}
		else
		{
			Vec3f result(x, y, t / sqrt(r));
			result.normalize();
			return result;
		}
	}

	Quaternionf OrbitCamera::getQuaternion(float x1, float y1, float x2, float y2)
	{
		if ((x1 == x2) && (y1 == y2))
		{
			return Quaternionf();
		}
		Vec3f pos1 = getPosition(x1, y1);
		Vec3f pos2 = getPosition(x2, y2);
		Vec3f rotaxis = pos1.cross(pos2);
		rotaxis.normalize();
		float rotangle = 2 * sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
		return Quaternionf(rotangle, rotaxis);
	}

	Quaternionf OrbitCamera::getQuaternion(float yaw, float pitch) const
	{
		Quaternionf oldQuat = Quaternionf(pitch, Vec3f(1.0f, 0.0f, 0.0f)) * Quaternionf(yaw, Vec3f(0.0f, 1.0f, 0.0f));

		float rotAngle = mRotAngle;
		Vec3f rotAxis = mRotAxis;

		// oldQuat.toRotationAxis(rotAngle, rotAxis);
		rotAngle = oldQuat.angle();
		rotAxis = oldQuat.axis();

		return oldQuat;
	}

	void OrbitCamera::registerPoint(float x, float y)
	{
		// 		mRegX = x;
		// 		mRegY = y;
		mRegX = float(x) / float(mViewportWidth);
		mRegY = float(mViewportHeight - y) / float(mViewportHeight);
	}

	glm::mat4 OrbitCamera::getViewMat()
	{
		Vec3f upDir;	// = Vec3f(0, 1, 0);
		Vec3f viewDir;	// = getViewDir();
		Vec3f rightDir; // = upDir.cross(viewDir).normalize();

		getCoordSystem(viewDir, upDir, rightDir);
		Vec3f targetPos = mEyePos + mFocusDist * viewDir;

		// return glm::lookAt(mEyePos.data_ * mUnitScale, targetPos.data_ * mUnitScale, upDir.data_);
		
		return glm::lookAt(
			glm::vec3(mEyePos.x, mEyePos.y, mEyePos.z) * mUnitScale, 
			glm::vec3(targetPos.x, targetPos.y, targetPos.z) * mUnitScale, 
			glm::vec3(upDir.x, upDir.y, upDir.z));
	}

	glm::mat4 OrbitCamera::getProjMat()
	{
		float aspect = std::max(float(mViewportWidth), 1.0f) / std::max(float(mViewportHeight), 1.0f);

		glm::mat4 projection;

		if (mProjectionType == Perspective)
		{
			projection = glm::perspective(mFov, aspect, mNear * mUnitScale, mFar * mUnitScale);
		}
		else
		{
			// float half_depth = (mEyePos - mTargetPos).norm() * mUnitScale;
			float half_depth = (mEyePos - mTargetPos).length() * mUnitScale;
			projection = glm::ortho(-half_depth * aspect, half_depth * aspect, -half_depth, half_depth, -5.0f * half_depth, 5.0f * half_depth);
		}

		return projection;
	}

	void OrbitCamera::rotateToPoint(float x, float y)
	{
		float tx = float(x) / float(mViewportWidth);
		float ty = float(mViewportHeight - y) / float(mViewportHeight);

		float dx = tx - mRegX;
		float dy = ty - mRegY;
		Quaternionf q = getQuaternion(mRegX, mRegY, tx, ty);
		rotate(mSpeed * dx, -mSpeed * dy);

		registerPoint(x, y);
	}

	void OrbitCamera::translateToPoint(float x, float y)
	{
		float tx = float(x) / float(mViewportWidth);
		float ty = float(mViewportHeight - y) / float(mViewportHeight);

		float dx = tx - mRegX;
		float dy = ty - mRegY;
		float dz = 0;
		translate(mSpeed * Vec3f(-dx, -dy, -dz));

		registerPoint(x, y);
	}

} // namespace HSim
