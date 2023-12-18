#pragma once
#include"DirectXMath.h"
#include<d3d12.h>
#include <string>
#include <vector>
#include<cstdint>

using std::string;
using std::vector;
using namespace DirectX;

class RModel;

struct SIMPLEVERTEX
{
	XMFLOAT3 pos  ;
	XMFLOAT2 uv ;
};

struct VERTEX
{
	XMFLOAT3 pos_;
	XMFLOAT3 norm_;
	XMFLOAT2 uv_;
	XMFLOAT3 tan_;
	UINT	 mtlID_;

	static const D3D12_INPUT_LAYOUT_DESC inp_Layout;

	VERTEX() = default;
	VERTEX(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT2 const& uv, XMFLOAT3 const& tan , UINT const& MID);


	enum ELEMENT
	{
		POSITION = 0,
		NORMAL,
		UV,
		TANGENT,
		MTLID,

		AMMOUNT
	};

private:

	static const D3D12_INPUT_ELEMENT_DESC element[AMMOUNT];
};

//-------------

struct Texture
{
	ID3D12Resource* rsc_ptr = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
};

template<class t>
struct StructuredBuffer
{
	ID3D12Resource* rsc_ptr = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
	t* view;
};

struct MATERIAL
{
	XMFLOAT3 dif_;
	float alpha_;

	XMFLOAT3 spec_;
	float shin_;

	XMFLOAT3 emis_;
	float val0;

	string dmap_;	//for diffuse . A = Coefficient
	string smap_;	//for specular .A = Coefficient
	string emap_;	//for emission .A = Coefficient
	string nmap_;	//for normal 
	string ESBAmap_;//R = emissive intensity, G = shineness , B = bamp , A = alpha

	MATERIAL();
};

struct BONE_INFO
{
public:
	vector<UINT> IDs;
	vector<float> Weights;
};

struct MESH
{
public:
	vector<VERTEX> vtcs_;
	vector<uint32_t> indexes_;
	vector<BONE_INFO> bnsinfo_;

	uint32_t ID_Material;

	MESH();
	//-----
};

bool LoadMesh(const wchar_t* file, vector<MESH>& mesh, vector<MATERIAL>& material);
bool LoadMesh(const wchar_t* file, RModel* ptr);

//-----------------------------for S buffer

struct alignas(16) ObjInfo
{
	XMMATRIX wld;
	float tick;

	float val0;
	float val1;
	float val2;

	ObjInfo();
};

struct alignas(16) MapBOOL
{
	int isD;
	int isS;
	int isE;
	int isN;
	int isESB;

	MapBOOL();
};

struct alignas(16) Material
{
	XMFLOAT3 dif;
	float alp;

	XMFLOAT3 spec;
	float shin;

	XMFLOAT3 emis;
	float val0;
};

//--------------------------------for C buufer
struct alignas(256) Util
{
	XMMATRIX view;
	XMMATRIX proj;
	float time;
};

struct alignas(256) Cam
{
	XMFLOAT3 pos;
	XMFLOAT3 tgt;
};

template <typename T>
struct CBUFFERVIEW
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;
	T* ptr = nullptr;
};

