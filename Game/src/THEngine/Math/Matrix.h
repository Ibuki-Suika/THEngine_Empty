#ifndef MATRIX_H
#define MATRIX_H

namespace THEngine
{
	struct Vector2f;
	struct Vector3f;
	struct Vector4f;
	class Matrix;

	struct Vector2f
	{
		float x;
		float y;

		Vector2f();
		Vector2f(float x, float y);
		float Norm();
		Vector2f Normalize();
		Vector2f operator +(Vector2f a);
		Vector2f operator -(Vector2f a);
		Vector2f operator -();
		bool operator ==(Vector2f a);
		bool operator !=(Vector2f a);
	};

	struct Vector3f
	{
		float x;
		float y;
		float z;

		Vector3f();
		Vector3f(float a1, float a2, float a3);
		float Norm();
		Vector3f Normalize();
		Vector3f operator +(const Vector3f& a);
		Vector3f operator -(const Vector3f& a);
		Vector3f operator -();
		Vector3f operator *(float a);
		bool operator ==(const Vector3f& a);
		bool operator !=(const Vector3f& a);

		static Vector3f Cross(Vector3f a, Vector3f b);
		static float Dot(Vector3f a, Vector3f b);
	};

	Vector3f operator*(float a, Vector3f v);

	struct Vector4f
	{
	public:
		float _data[4];
	public:
		Vector4f();
		Vector4f(float a1, float a2, float a3, float a4);

		Vector4f operator *(const Matrix& a);

		float& operator[](const int& index);
	};

	class Matrix
	{
	public:
		float _data[4][4];
	public:
		Matrix();
		Matrix(float **a);
		Matrix operator *(Matrix a);

		static Matrix Identity();
		static Matrix Perspective(float fov, float aspect, float zn, float zf);
		static Matrix LookAt(Vector3f position, Vector3f up, Vector3f lookAt);
		static Matrix Viewport(int x, int y, int width, int height, int MinZ, int MaxZ);
	};

	class Rect
	{
	public:
		float left, right, top, bottom;

	public:
		Rect();
		Rect(float l, float r, float t, float b) :left(l), right(r), top(t), bottom(b) {}
		inline float Width() { return right - left; }
		inline float Height() { return bottom - top; }
	};
}

#endif