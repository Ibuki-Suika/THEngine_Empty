#include "THMesh.h"
#include <Core\THGame.h>
#include <Platform\THDevice.h>
#include <Asset\THAssetManager.h>
#include <Asset\THTexture.h>
#include <Renderer\THRenderPipeline.h>
#include <Renderer\THMeshRenderer.h>

namespace THEngine
{
	Mesh::Mesh()
	{
	}

	Mesh::Mesh(const Mesh& mesh) : GameObject(mesh)
	{
		vertexCount = mesh.vertexCount;
		material = mesh.material;
		primitiveType = mesh.primitiveType;

		if (mesh.mesh)
		{
			this->mesh = (D3DMesh*)mesh.mesh->Clone().Get();
		}

		if (mesh.vertexBuffer)
		{
			InitVertexBuffer(vertexCount);

			void* src;
			mesh.vertexBuffer->Lock(0, 0, &src, D3DLOCK_READONLY | D3DLOCK_NOOVERWRITE);

			SetVertexData(src);

			mesh.vertexBuffer->Unlock();
		}
	}

	Mesh::~Mesh()
	{
		TH_SAFE_RELEASE(vertexBuffer);
		TH_SAFE_RELEASE(indexBuffer);
	}

	void Mesh::SendToRenderQueue()
	{
		Game::GetInstance()->GetRenderPipeline()->SendToRenderQueue(RenderPipeline::NORMAL, this);
	}

	void Mesh::InitVertexBuffer(int size)
	{
		auto device = Device::GetInstance()->GetD3DDevice();

		device->CreateVertexBuffer(size * sizeof(MeshVertex), 0,
			D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, D3DPOOL_MANAGED, &vertexBuffer, nullptr);

		vertexCount = size;
	}

	void Mesh::SetVertexData(void* data)
	{
		void* lockedBuffer;
		vertexBuffer->Lock(0, vertexCount * sizeof(MeshVertex), &lockedBuffer, D3DLOCK_DISCARD);

		memcpy(lockedBuffer, data, vertexCount * sizeof(MeshVertex));

		vertexBuffer->Unlock();
	}

	void Mesh::Update()
	{
		GameObject::Update();
	}

	void Mesh::Draw()
	{
		Game::GetInstance()->GetRenderPipeline()->GetMeshRenderer()->Render(this);
	}

	void Mesh::DrawGeometry()
	{
		auto device = Device::GetInstance();

		if (this->mesh != nullptr)
		{
			this->mesh->DrawGeometry();
		}
		else
		{
			D3DVERTEXBUFFER_DESC vbdesc;
			this->vertexBuffer->GetDesc(&vbdesc);
			device->GetD3DDevice()->SetFVF(vbdesc.FVF);

			switch (this->primitiveType)
			{
			case Mesh::TRIANGLE_LIST:
				device->GetD3DDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, this->vertexCount / 3);
				break;
			case Mesh::TRIANGLE_STRIP:
				device->GetD3DDevice()->SetStreamSource(0, this->vertexBuffer, 0, sizeof(MeshVertex));
				device->GetD3DDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, this->vertexCount - 2);
				break;
			}
		}
	}

	Ptr<Mesh> Mesh::CreateMeshFromFile(const String& filePath)
	{
		auto exceptionManager = ExceptionManager::GetInstance();

		Ptr<Mesh> mesh = Ptr<Mesh>::New();

		String ext = filePath.SubString(filePath.LastIndexOf(TCHAR('.')) + 1, filePath.GetLength());
		if (ext == "x")
		{
			if (mesh->LoadFromX(filePath) == false)
			{
				Ptr<Exception> exception = Ptr<Exception>::New((String)"加载模型文件失败：" + filePath + "原因是:\n"
					+ exceptionManager->GetException()->GetInfo());
				exceptionManager->PushException(exception);
				return nullptr;
			}
		}
		else
		{
			Ptr<Exception> exception = Ptr<Exception>::New("不支持的文件格式。");
			exceptionManager->PushException(exception);
			return nullptr;
		}
		return mesh;
	}

	bool Mesh::LoadFromX(const String& filePath)
	{
		this->mesh = D3DMesh::LoadMesh(filePath);
		if (this->mesh != nullptr)
		{
			return true;
		}
		return false;
	}

	Ptr<Object> Mesh::Clone() const
	{
		return Ptr<Mesh>::New(*this).Get();
	}

	//////////////////////////////////////////////////////////////////////
	Mesh::D3DMesh::D3DMesh()
	{
	}

	Mesh::D3DMesh::D3DMesh(const D3DMesh& d3dMesh) : Object(d3dMesh)
	{
		auto device = Device::GetInstance()->GetD3DDevice();

		IDirect3DVertexBuffer9* vb;
		D3DVERTEXBUFFER_DESC vbdesc;
		d3dMesh.mesh->GetVertexBuffer(&vb);
		vb->GetDesc(&vbdesc);
		vb->Release();

		d3dMesh.mesh->CloneMeshFVF(D3DXMESH_MANAGED, vbdesc.FVF, device, &mesh);

		this->numMaterials = d3dMesh.numMaterials;
		this->materialList = d3dMesh.materialList;
		this->isCloned = true;
	}

	Mesh::D3DMesh::~D3DMesh()
	{
		auto assetManager = AssetManager::GetInstance();

		if (isCloned == false && materialList)
		{
			delete[] materialList;
		}

		if (mesh)
		{
			mesh->Release();
		}
	}

	Ptr<Mesh::D3DMesh> Mesh::D3DMesh::LoadMesh(const String& filePath)
	{
		Ptr<D3DMesh> d3dMesh = Ptr<D3DMesh>::New();

		auto device = Device::GetInstance()->GetD3DDevice();
		auto exceptionManager = ExceptionManager::GetInstance();

		ID3DXBuffer* adjacency = nullptr;
		ID3DXBuffer* materialBuffer = nullptr;
		DWORD numMaterials;

		if (FAILED(D3DXLoadMeshFromX(filePath.GetBuffer(), D3DXMESH_MANAGED, device, &adjacency, &materialBuffer,
			nullptr, &numMaterials, &d3dMesh->mesh)))
		{
			Ptr<Exception> exception = Ptr<Exception>::New("无法加载模型文件。文件可能已损坏。");
			exceptionManager->PushException(exception);
			return nullptr;
		}

		//优化网格
		if (FAILED(d3dMesh->mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
			(DWORD*)adjacency->GetBufferPointer(), nullptr, nullptr, nullptr)))
		{
			Ptr<Exception> exception = Ptr<Exception>::New("模型优化失败。");
			exceptionManager->PushException(exception);
			return nullptr;
		}

		//加载纹理
		auto assetManager = AssetManager::GetInstance();
		d3dMesh->materialList = new Material[numMaterials];
		d3dMesh->numMaterials = numMaterials;

		D3DXMATERIAL* materials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();
		String folder = filePath.SubString(0, filePath.LastIndexOf(TCHAR('/')) + 1);
		for (int i = 0; i < d3dMesh->numMaterials; i++)
		{
			if (materials[i].pTextureFilename)
			{
				String path = folder + materials[i].pTextureFilename;
				d3dMesh->materialList[i].texture = assetManager->CreateTextureFromFile(path);
				if (d3dMesh->materialList[i].texture == nullptr)
				{
					return nullptr;
				}
			}
			d3dMesh->materialList[i].ambient[0] = materials[i].MatD3D.Diffuse.r;
			d3dMesh->materialList[i].ambient[1] = materials[i].MatD3D.Diffuse.g;
			d3dMesh->materialList[i].ambient[2] = materials[i].MatD3D.Diffuse.b;
			d3dMesh->materialList[i].ambient[3] = materials[i].MatD3D.Diffuse.a;
			d3dMesh->materialList[i].diffuse[0] = materials[i].MatD3D.Diffuse.r;
			d3dMesh->materialList[i].diffuse[1] = materials[i].MatD3D.Diffuse.g;
			d3dMesh->materialList[i].diffuse[2] = materials[i].MatD3D.Diffuse.b;
			d3dMesh->materialList[i].diffuse[3] = materials[i].MatD3D.Diffuse.a;
			d3dMesh->materialList[i].specular[0] = materials[i].MatD3D.Specular.r;
			d3dMesh->materialList[i].specular[1] = materials[i].MatD3D.Specular.g;
			d3dMesh->materialList[i].specular[2] = materials[i].MatD3D.Specular.b;
			d3dMesh->materialList[i].specular[3] = materials[i].MatD3D.Specular.a;
			d3dMesh->materialList[i].emissive[0] = materials[i].MatD3D.Emissive.r;
			d3dMesh->materialList[i].emissive[1] = materials[i].MatD3D.Emissive.g;
			d3dMesh->materialList[i].emissive[2] = materials[i].MatD3D.Emissive.b;
			d3dMesh->materialList[i].emissive[3] = materials[i].MatD3D.Emissive.a;
			d3dMesh->materialList[i].power = materials[i].MatD3D.Power;
		}

		return d3dMesh;
	}

	Ptr<Object> Mesh::D3DMesh::Clone() const
	{
		return Ptr<D3DMesh>::New(*this).Get();
	}

	void Mesh::D3DMesh::DrawGeometry()
	{
		auto meshShader = Device::GetInstance()->GetRenderState()->GetCurrentShader();
		for (int i = 0; i < this->numMaterials; i++)
		{
			Material& mat = this->materialList[i];
			if (mat.texture != nullptr)
			{
				meshShader->SetTexture("tex", mat.texture);
				meshShader->SetBoolean("hasTexture", true);
			}
			else
			{
				meshShader->SetBoolean("hasTexture", false);
			}
			meshShader->SetValue("material", &mat, sizeof(Material) - sizeof(Texture*));
			meshShader->CommitChanges();
			this->mesh->DrawSubset(i);
		}
	}
}