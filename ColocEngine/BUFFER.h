#pragma once
#include"DirectXMath.h"
#include<d3d12.h>
#include <string>
#include <vector>
#include<cstdint>
#include"S_Light.h"

using std::string;
using std::vector;
using namespace DirectX;

class RModel;

struct Rect2D
{
	Rect2D(float lx, float ty, float rx, float by);
	Rect2D();

	float lx;
	float ty;
	float rx;
	float by;
};

//-----------------------

struct SIMPLEVERTEX
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;

	static const D3D12_INPUT_LAYOUT_DESC inp_Layout;

	enum ELEMENT
	{
		POSITION = 0,
		UV,

		AMMOUNT
	};

private:

	static const D3D12_INPUT_ELEMENT_DESC element[AMMOUNT];
};

struct VERTEX
{
	XMFLOAT3 pos_;
	XMFLOAT3 norm_;
	XMFLOAT2 uv_;
	XMFLOAT3 tan_;
	XMFLOAT3 bitan_;
	UINT	 mtlID_;

	static const D3D12_INPUT_LAYOUT_DESC inp_Layout;

	VERTEX() = default;
	VERTEX(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT2 const& uv, XMFLOAT3 const& tan, XMFLOAT3 const& bitan, UINT const& MID);


	enum ELEMENT
	{
		POSITION = 0,
		NORMAL,
		UV,
		TANGENT,
		BITANGENT,
		MTLID,

		AMMOUNT
	};

private:

	static const D3D12_INPUT_ELEMENT_DESC element[AMMOUNT];
};

//-----------------------------for S buffer

struct alignas(16) ObjInfo
{
	XMMATRIX wld;
	float tick;

	float val0;
	float val1;
	float val2;

	ObjInfo() :tick(0), val0(0), val1(0), val2(0)
	{
		wld =
		{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	}
};

struct alignas(16) MapBOOL
{
	int isD;		//for diffuse . A = Coefficient
	int isS;		//for specular .A = Coefficient
	int isE;		//for emission .A = Coefficient
	int isN;		//for normal 
	int isESB;		//R = emissive intensity, G = shineness , B = bamp , A = alpha

	float val0;
	float val1;
	float val2;

	MapBOOL();
};

struct alignas(16) Material
{
	XMFLOAT3 dif;
	float alp;

	XMFLOAT3 spec;
	float shin;

	XMFLOAT3 emis;
	float emis_str;
};

//--------------------------------for C buufer
struct alignas(256) Util
{
	XMMATRIX view;
	XMMATRIX proj;
	float time;
	uint16_t PL_CNT;
	uint16_t DL_CNT;
	uint16_t AL_CNT;
};

struct alignas(256) Cam
{
	XMFLOAT3 pos;
	XMFLOAT3 tgt;
};

struct alignas(256) Lights
{
	P_Light* pl;
	D_Light* dl;
	A_Light* al;
};

template <typename T>
struct CBUFFERVIEW
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;
	T* ptr = nullptr;
};

//-------------

struct Texture
{
	ID3D12Resource* rsc_ptr = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
};

template<class t , size_t arrSize>
struct StructuredBuffer
{
	ID3D12Resource* rsc_ptr = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
	t view[arrSize];
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

	MapBOOL defTex_;
	vector<MapBOOL> texIndex_;
	uint32_t ID_Material;

	MESH();
	//-----
};

bool LoadMesh(const wchar_t* file, vector<MESH>& mesh, vector<Material>& material);
bool LoadMesh(const wchar_t* file, RModel* ptr);