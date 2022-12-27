#include "ZEpch.h"
#include "Engine/Math/Math.h"

#include <glm/gtx/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace ZeoEngine::Math {

	Vec3 GetForwardVector(const Vec3& rotation)
	{
		return glm::rotate(Quat(rotation), { 0.0f, 0.0f, -1.0f });
	}

	Vec3 GetRightVector(const Vec3& rotation)
	{
		return glm::rotate(Quat(rotation), { 1.0f, 0.0f, 0.0f });
	}

	Vec3 GetUpVector(const Vec3& rotation)
	{
		return glm::rotate(Quat(rotation), { 0.0f, 1.0f, 0.0f });
	}

	Mat4 ComposeTransform(const Vec3& translation, const Vec3& rotation, const Vec3& scale)
	{
		return glm::translate(Mat4(1.0f), translation) *
			glm::toMat4(Quat(rotation)) *
			glm::scale(Mat4(1.0f), scale);
	}

	bool DecomposeTransform(const Mat4& transform, Vec3& outTranslation, Vec3& outRotation, Vec3& outScale)
	{
		// From glm::decompose in matrix_decompose.inl

		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>())) return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		outTranslation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3];

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
		{
			for (length_t j = 0; j < 3; ++j)
			{
				Row[i][j] = LocalMatrix[i][j];
			}
		}

		// Compute X scale factor and normalize first row.
		outScale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		outScale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		outScale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		outRotation.y = asin(-Row[0][2]);
		if (cos(outRotation.y) != 0)
		{
			outRotation.x = atan2(Row[1][2], Row[2][2]);
			outRotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else
		{
			outRotation.x = atan2(-Row[2][0], Row[1][1]);
			outRotation.z = 0;
		}

		return true;
	}

	Vec3 GetTranslationFromTransform(const Mat4& transform)
	{
		return { transform[3] };
	}

	float InterpTo(float current, float target, DeltaTime dt, float interpSpeed)
	{
		// If no interp speed, jump to target value
		if (interpSpeed <= 0.0f)
		{
			return target;
		}

		// Distance to reach
		const float dist = target - current;

		// If distance is too small, just set the desired location
		if (dist * dist < SMALL_NUMBER)
		{
			return target;
		}

		// Delta Move, clamp so we do not over shoot
		const float deltaMove = dist * glm::clamp<float>(dt * interpSpeed, 0.0f, 1.0f);

		return current + deltaMove;
	}

	float InterpConstantTo(float current, float target, DeltaTime dt, float interpSpeed)
	{
		const float dist = target - current;

		// If distance is too small, just set the desired location
		if (dist * dist < SMALL_NUMBER)
		{
			return target;
		}

		const float step = interpSpeed * dt;
		return current + glm::clamp(dist, -step, step);
	}

	bool IsNearlyEqual(float x, float y, float errorTolerance)
	{
		return glm::abs(x - y) <= errorTolerance;
	}

	bool IsNearlyEqual(double x, double y, double errorTolerance)
	{
		return glm::abs(x - y) <= errorTolerance;
	}

	bool IsNearlyEqual(const Vec2& x, const Vec2& y, float errorTolerance)
	{
		return glm::abs(x.x - y.x) <= errorTolerance && glm::abs(x.y - y.y) <= errorTolerance;
	}

	bool IsNearlyEqual(const Vec3& x, const Vec3& y, float errorTolerance)
	{
		return glm::abs(x.x - y.x) <= errorTolerance && glm::abs(x.y - y.y) <= errorTolerance && glm::abs(x.z - y.z) <= errorTolerance;
	}

	Vec3 FindLookAtRotation(const Vec3& from, const Vec3& to, const Vec3& up)
	{
		const Vec3 direction = glm::normalize(to - from);
		const Quat rotation = glm::quatLookAt(direction, up);
		return glm::eulerAngles(rotation);
	}

}
