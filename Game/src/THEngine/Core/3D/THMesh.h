#ifndef THMESH_H
#define THMESH_H

#include <Core\THGameObject.h>

namespace THEngine
{
	class Texture;

	class Material
	{
	public:
		Vector4f ambient = Vector4f(0, 0, 0, 0);
		Vector4f diffuse = Vector4f(0, 0, 0, 0);
		Vector4f specular = Vector4f(0, 0, 0, 0);
		Vector4f emissive = Vector4f(0, 0, 0, 0);
		float power = 0.0f;
		Ptr<Texture> texture = nullptr;
	};

	struct MeshVertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
		MeshVertex() {}
		MeshVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) :
			x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), u(u), v(v) {}
	};

	class Mesh : public GameObject
	{
	public:
		enum PrimitiveType
		{
			TRIANGLE_LIST,
			TRIANGLE_STRIP 
		};

	private:
		class D3DMesh : public Object
		{
		private:
			ID3DXMesh* mesh = nullptr;
			int numMaterials = 0;
			Material* materialList = nullptr;
			bool isCloned = false;

		public:
			D3DMesh();
			D3DMesh(const D3DMesh& d3dMesh);
			virtual ~D3DMesh();

			static Ptr<D3DMesh> LoadMesh(const String& filePath);

			virtual Ptr<Object> Clone() const override;

			void DrawGeometry();

			friend class MeshRenderer;
		};

	protected:
		int vertexCount = 0;

		Material material;
		PrimitiveType primitiveType = TRIANGLE_LIST;

		Ptr<D3DMesh> mesh = nullptr;
		IDirect3DVertexBuffer9* vertexBuffer = nullptr;
		IDirect3DIndexBuffer9* indexBuffer = nullptr;

	private:
		bool LoadFromX(const String& filePath);

	public:
		Mesh();
		Mesh(const Mesh& mesh);
		virtual ~Mesh();

		virtual void Update() override;
		virtual void Draw() override;
		virtual void SendToRenderQueue() override;
		virtual Ptr<Object> Clone() const override;
		virtual void DrawGeometry() override;

		inline void SetMaterial(const Material& material) { this->material = material; }

		inline void SetPrimitiveType(PrimitiveType primitiveType) { this->primitiveType = primitiveType; }

		void InitVertexBuffer(int size);

		void SetVertexData(void* data);

		static Ptr<Mesh> CreateMeshFromFile(const String& filePath);

	

		friend class MeshRenderer;
	};
}

#endif