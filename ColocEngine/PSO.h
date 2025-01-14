#pragma once
#include<d3d12.h>
#include<cstdint>

class PSO
{
public:

	PSO();

	virtual bool Init
	(
		D3D12_ROOT_PARAMETER* params ,
		D3D12_STATIC_SAMPLER_DESC* sampler,
		D3D12_ROOT_SIGNATURE_FLAGS flag,
		uint16_t paramcnt ,
		uint16_t sampcnt
	) = 0 ;

	virtual void Term();

	ID3D12PipelineState* GetPSO();
	ID3D12RootSignature* GetRTSG();

protected:
	ID3D12PipelineState* ptr;
	ID3D12RootSignature* rtsg;
};

//----------------------------------------------

class Def3D : public PSO
{
public:

	Def3D();

	bool Init
	(
		D3D12_ROOT_PARAMETER* params,
		D3D12_STATIC_SAMPLER_DESC* sampler,
		D3D12_ROOT_SIGNATURE_FLAGS flag,
		uint16_t paramcnt,
		uint16_t sampcnt
	) override;
};

class DefDeferred : public PSO
{
public:

	DefDeferred();

	bool Init
	(
		D3D12_ROOT_PARAMETER* params,
		D3D12_STATIC_SAMPLER_DESC* sampler,
		D3D12_ROOT_SIGNATURE_FLAGS flag,
		uint16_t paramcnt,
		uint16_t sampcnt
	) override;
};

class DefPost : public PSO
{
public:

	DefPost();

	bool Init
	(D3D12_ROOT_PARAMETER* params, D3D12_STATIC_SAMPLER_DESC* sampler, D3D12_ROOT_SIGNATURE_FLAGS flag, uint16_t paramcnt, uint16_t sampcnt)override;
};

class DefUI : public PSO
{
public:

	DefUI();

	bool Init
	(D3D12_ROOT_PARAMETER* params, D3D12_STATIC_SAMPLER_DESC* sampler, D3D12_ROOT_SIGNATURE_FLAGS flag, uint16_t paramcnt, uint16_t sampcnt)override;
};

class BoneAnimCompute : public PSO
{
public:
	BoneAnimCompute();

	bool Init
	(D3D12_ROOT_PARAMETER* params, D3D12_STATIC_SAMPLER_DESC* sampler, D3D12_ROOT_SIGNATURE_FLAGS flag, uint16_t paramcnt, uint16_t sampcnt)override;
};