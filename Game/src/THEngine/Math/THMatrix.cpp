#include "THMatrix.h"
#include "THMathUtil.h"

namespace THEngine
{
	Matrix::Matrix()
	{
	}

	Matrix::~Matrix()
	{
	}

	void Matrix::Identity(Matrix* matrix)
	{
		D3DXMatrixIdentity(&matrix->matrix);
	}

	void Matrix::Translate(Matrix* matrix, float x, float y, float z)
	{
		D3DXMatrixTranslation(&matrix->matrix, x, y, z);
	}

	void Matrix::Ortho(Matrix* matrix, float left, float right, float bottom, float top, float znear, float zfar)
	{
		D3DXMatrixOrthoOffCenterLH(&matrix->matrix, left, right, bottom, top, znear, zfar);
	}

	void Matrix::Perspective(Matrix* matrix, int fov, float aspect, float znear, float zfar)
	{
		D3DXMatrixPerspectiveFovLH(&matrix->matrix, Math::ToRad(fov), aspect, znear, zfar);
	}

	void Matrix::LookAt(Matrix* matrix, const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up)
	{
		D3DXMatrixLookAtLH(&matrix->matrix, &D3DXVECTOR3(eye.x, eye.y, eye.z), &D3DXVECTOR3(lookAt.x, lookAt.y, lookAt.z),
			&D3DXVECTOR3(up.x, up.y, up.z));
	}

	void Matrix::Scale(Matrix* matrix, float scaleX, float scaleY, float scaleZ)
	{
		D3DXMatrixScaling(&matrix->matrix, scaleX, scaleY, scaleZ);
	}

	void Matrix::RotateX(Matrix* matrix, float degree)
	{
		D3DXMatrixRotationX(&matrix->matrix, Math::ToRad(degree));
	}

	void Matrix::RotateY(Matrix* matrix, float degree)
	{
		D3DXMatrixRotationY(&matrix->matrix, Math::ToRad(degree));
	}

	void Matrix::RotateZ(Matrix* matrix, float degree)
	{
		D3DXMatrixRotationZ(&matrix->matrix, Math::ToRad(degree));
	}

	void Matrix::RotateQuarternion(Matrix* matrix, const Quaternion& quaternion)
	{
		D3DXMatrixRotationQuaternion(&matrix->matrix, &quaternion.quaternion);
	}

	Matrix& Matrix::Inverse()
	{
		D3DXMatrixInverse(&this->matrix, nullptr, &this->matrix);
		return *this;
	}

	Matrix& Matrix::Transpose()
	{
		D3DXMatrixTranspose(&this->matrix, &this->matrix);
		return *this;
	}

	void Matrix::Set(int row, int coloum, float value)
	{
		this->matrix.m[row][coloum] = value;
	}

	float Matrix::Get(int row, int coloum)
	{
		return this->matrix.m[row][coloum];
	}

	Matrix& Matrix::operator*=(const Matrix& matrix)
	{
		this->matrix *= matrix.matrix;
		return *this;
	}

	Matrix Matrix::operator*(const Matrix& matrix) const
	{
		Matrix result;
		result.matrix = this->matrix * matrix.matrix;
		return result;
	}

	Vector4f operator*(const Vector4f& vector, const Matrix& matrix)
	{
		D3DXVECTOR4 dxVector = D3DXVECTOR4(vector._data);
		D3DXVec4Transform(&dxVector, &dxVector, &matrix.matrix);
		return Vector4f(dxVector.x, dxVector.y, dxVector.z, dxVector.w);
	}

	Vector4f& operator*=(Vector4f& vector, const Matrix& matrix)
	{
		D3DXVECTOR4 dxVector = D3DXVECTOR4(vector._data);
		D3DXVec4Transform(&dxVector, &dxVector, &matrix.matrix);
		vector.x = dxVector.x;
		vector.y = dxVector.y;
		vector.z = dxVector.z;
		vector.w = dxVector.w;
		return vector;
	}
}