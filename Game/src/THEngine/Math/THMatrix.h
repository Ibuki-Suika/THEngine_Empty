#ifndef TH_MATRIX_H
#define TH_MATRIX_H

#include <Common\THCommon.h>
#include "THQuaternion.h"

namespace THEngine
{
	class Matrix : public Object
	{
	private:
		D3DXMATRIX matrix;

	public:
		Matrix();
		virtual ~Matrix();

		static void Identity(Matrix* matrix);
		static void Translate(Matrix* matrix, float x, float y, float z);
		static void Ortho(Matrix* matrix, float left, float right, float bottom, float top, float znear, float zfar);
		static void Perspective(Matrix* matrix, int fov, float aspect, float znear, float zfar);
		static void LookAt(Matrix* matrix, const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up);
		static void Scale(Matrix* matrix, float scaleX, float scaleY, float scaleZ);
		static void RotateX(Matrix* matrix, float degree);
		static void RotateY(Matrix* matrix, float degree);
		static void RotateZ(Matrix* matrix, float degree);
		static void RotateQuarternion(Matrix* matrix, const Quaternion& quaternion);

		Matrix& Inverse();
		Matrix& Transpose();

		void Set(int row, int coloum, float value);
		float Get(int row, int coloum);

		Matrix& operator =(const Matrix& matrix) = default;
		Matrix& operator *=(const Matrix& matrix);
		Matrix operator *(const Matrix& matrix) const;

		friend class Shader;
		friend Vector4f operator* (const Vector4f& vector, const Matrix& matrix);
		friend Vector4f& operator*= (Vector4f& vector, const Matrix& matrix);
	};

	Vector4f operator* (const Vector4f& vector, const Matrix& matrix);
	Vector4f& operator*= (Vector4f& vector, const Matrix& matrix);
}

#endif