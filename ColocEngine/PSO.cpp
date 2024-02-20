#include "PSO.h"
#include"D3D.h"

PSO::PSO():ptr(nullptr),rtsg(nullptr)
{
}

void PSO::Term()
{
	SAFE_RELEASE(ptr);
	ptr = nullptr;

	SAFE_RELEASE(rtsg);
	rtsg = nullptr;
}

ID3D12PipelineState* PSO::GetPSO()
{
	return ptr;
}

ID3D12RootSignature* PSO::GetRTSG()
{
    return rtsg;
}

bool Def3D::Init(D3D12_ROOT_PARAMETER* params, D3D12_STATIC_SAMPLER_DESC* sampler, D3D12_ROOT_SIGNATURE_FLAGS flag, uint16_t paramcnt, uint16_t sampcnt)
{
	auto device_ = PTR_D3D::ptr->GetDevice();
	ID3DBlob* S_blob = nullptr;
	ID3DBlob* E_blob = nullptr;
	HRESULT res = NULL;

	{
		D3D12_ROOT_SIGNATURE_DESC rtdesc = {};
		{
			rtdesc.Flags = flag;
			rtdesc.NumParameters = paramcnt;
			rtdesc.NumStaticSamplers = sampcnt;
			rtdesc.pParameters = params;
			rtdesc.pStaticSamplers = sampler;
		}

		res = D3D12SerializeRootSignature
		(
			&rtdesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0,
			&S_blob,
			&E_blob
		);
		if (FAILED(res))	return false;
	}

	res = device_->CreateRootSignature
	(
		NULL,
		S_blob->GetBufferPointer(),
		S_blob->GetBufferSize(),
		IID_PPV_ARGS(&rtsg)
	);
	if (FAILED(res))	return false;

//-------------------------

	D3D12_RASTERIZER_DESC rs_desc = {};
	{
		rs_desc.FillMode = D3D12_FILL_MODE_SOLID;
		rs_desc.CullMode = D3D12_CULL_MODE_FRONT;
		rs_desc.FrontCounterClockwise = false;
		rs_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rs_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rs_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rs_desc.AntialiasedLineEnable = false;
		rs_desc.DepthClipEnable = false;
		rs_desc.MultisampleEnable = false;
		rs_desc.ForcedSampleCount = 0;
		rs_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}

	D3D12_RENDER_TARGET_BLEND_DESC rtb_desc =
	{
		true,false,
		D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,D3D12_COLOR_WRITE_ENABLE_ALL
	};

	D3D12_BLEND_DESC bs_desc = {};
	{
		bs_desc.AlphaToCoverageEnable = false;
		bs_desc.IndependentBlendEnable = false;
		for (auto i = 0u; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {

			bs_desc.RenderTarget[i] = rtb_desc;
		}
	};

	D3D12_DEPTH_STENCIL_DESC dss_desc = {};
	{
		dss_desc.DepthEnable = 1;
		dss_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		dss_desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		dss_desc.StencilEnable = 0;
	}

	//----------------------

	ID3DBlob* VSblob = nullptr;
	res = D3DReadFileToBlob(SHADER_FILENAME::DefVS, &VSblob);
	if (FAILED(res))     return 0;

	ID3DBlob* PSblob = nullptr;
	res = D3DReadFileToBlob(SHADER_FILENAME::DefPS, &PSblob);
	if (FAILED(res))     return 0;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
	{
		pso_desc.InputLayout = VERTEX::inp_Layout;
		pso_desc.pRootSignature = rtsg;
		pso_desc.VS.pShaderBytecode = VSblob->GetBufferPointer();
		pso_desc.PS.pShaderBytecode = PSblob->GetBufferPointer();
		pso_desc.VS.BytecodeLength = VSblob->GetBufferSize();
		pso_desc.PS.BytecodeLength = PSblob->GetBufferSize();
		pso_desc.RasterizerState = rs_desc;
		pso_desc.BlendState = bs_desc;
		pso_desc.SampleDesc.Count = 1;
		pso_desc.SampleDesc.Quality = 0;
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso_desc.DepthStencilState = dss_desc;
		pso_desc.NumRenderTargets = 1;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	}

	res = device_->CreateGraphicsPipelineState
	(
		&pso_desc,
		IID_PPV_ARGS(&ptr)
	);
	if (FAILED(res))return false;

	return true;
}

bool DefToon::Init(D3D12_ROOT_PARAMETER* params, D3D12_STATIC_SAMPLER_DESC* sampler, D3D12_ROOT_SIGNATURE_FLAGS flag, uint16_t paramcnt, uint16_t sampcnt)
{
	auto device_ = PTR_D3D::ptr->GetDevice();
	ID3DBlob* S_blob = nullptr;
	ID3DBlob* E_blob = nullptr;
	HRESULT res = NULL;

	{
		D3D12_ROOT_SIGNATURE_DESC rtdesc = {};
		{
			rtdesc.Flags = flag;
			rtdesc.NumParameters = paramcnt;
			rtdesc.NumStaticSamplers = sampcnt;
			rtdesc.pParameters = params;
			rtdesc.pStaticSamplers = sampler;
		}

		res = D3D12SerializeRootSignature
		(
			&rtdesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0,
			&S_blob,
			&E_blob
		);
		if (FAILED(res))	return false;
	}

	res = device_->CreateRootSignature
	(
		NULL,
		S_blob->GetBufferPointer(),
		S_blob->GetBufferSize(),
		IID_PPV_ARGS(&rtsg)
	);
	if (FAILED(res))	return false;

	//-------------------------

	D3D12_RASTERIZER_DESC rs_desc = {};
	{
		rs_desc.FillMode = D3D12_FILL_MODE_SOLID;
		rs_desc.CullMode = D3D12_CULL_MODE_FRONT;
		rs_desc.FrontCounterClockwise = false;
		rs_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rs_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rs_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rs_desc.AntialiasedLineEnable = false;
		rs_desc.DepthClipEnable = false;
		rs_desc.MultisampleEnable = false;
		rs_desc.ForcedSampleCount = 0;
		rs_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}

	D3D12_RENDER_TARGET_BLEND_DESC rtb_desc =
	{
		true,false,
		D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,D3D12_COLOR_WRITE_ENABLE_ALL
	};

	D3D12_BLEND_DESC bs_desc = {};
	{
		bs_desc.AlphaToCoverageEnable = false;
		bs_desc.IndependentBlendEnable = false;
		for (auto i = 0u; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {

			bs_desc.RenderTarget[i] = rtb_desc;
		}
	};

	D3D12_DEPTH_STENCIL_DESC dss_desc = {};
	{
		dss_desc.DepthEnable = 1;
		dss_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		dss_desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		dss_desc.StencilEnable = 0;
	}

	//----------------------

	ID3DBlob* VSblob = nullptr;
	res = D3DReadFileToBlob(SHADER_FILENAME::DefVS, &VSblob);
	if (FAILED(res))     return 0;

	ID3DBlob* PSblob = nullptr;
	res = D3DReadFileToBlob(SHADER_FILENAME::ToonPS , &PSblob);
	if (FAILED(res))     return 0;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
	{
		pso_desc.InputLayout = VERTEX::inp_Layout;
		pso_desc.pRootSignature = rtsg;
		pso_desc.VS.pShaderBytecode = VSblob->GetBufferPointer();
		pso_desc.PS.pShaderBytecode = PSblob->GetBufferPointer();
		pso_desc.VS.BytecodeLength = VSblob->GetBufferSize();
		pso_desc.PS.BytecodeLength = PSblob->GetBufferSize();
		pso_desc.RasterizerState = rs_desc;
		pso_desc.BlendState = bs_desc;
		pso_desc.SampleDesc.Count = 1;
		pso_desc.SampleDesc.Quality = 0;
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso_desc.DepthStencilState = dss_desc;
		pso_desc.NumRenderTargets = 1;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	}

	res = device_->CreateGraphicsPipelineState
	(
		&pso_desc,
		IID_PPV_ARGS(&ptr)
	);
	if (FAILED(res))return false;

	return true;
}

bool DefPost::Init(D3D12_ROOT_PARAMETER* params, D3D12_STATIC_SAMPLER_DESC* sampler, D3D12_ROOT_SIGNATURE_FLAGS flag, uint16_t paramcnt, uint16_t sampcnt)
{
	auto device_ = PTR_D3D::ptr->GetDevice();
	ID3DBlob* S_blob = nullptr;
	ID3DBlob* E_blob = nullptr;
	HRESULT res = NULL;

	{
		D3D12_ROOT_SIGNATURE_DESC rtdesc = {};
		{
			rtdesc.Flags = flag;
			rtdesc.NumParameters = paramcnt; //paramcnt;
			rtdesc.NumStaticSamplers = sampcnt;// sampcnt;
			rtdesc.pParameters = params;
			rtdesc.pStaticSamplers = sampler;
		}

		res = D3D12SerializeRootSignature
		(
			&rtdesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0,
			&S_blob,
			&E_blob
		);
		if (FAILED(res))	return false;
	}

	res = device_->CreateRootSignature
	(
		NULL,
		S_blob->GetBufferPointer(),
		S_blob->GetBufferSize(),
		IID_PPV_ARGS(&rtsg)
	);
	if (FAILED(res))	return false;

	//-------------------------

	D3D12_RASTERIZER_DESC rs_desc = {};
	{
		rs_desc.FillMode = D3D12_FILL_MODE_SOLID;
		rs_desc.CullMode = D3D12_CULL_MODE_FRONT;
		rs_desc.FrontCounterClockwise = false;
		rs_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rs_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rs_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rs_desc.AntialiasedLineEnable = false;
		rs_desc.DepthClipEnable = false;
		rs_desc.MultisampleEnable = false;
		rs_desc.ForcedSampleCount = 0;
		rs_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}

	D3D12_RENDER_TARGET_BLEND_DESC rtb_desc =
	{
		false,false,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,D3D12_COLOR_WRITE_ENABLE_ALL
	};

	D3D12_BLEND_DESC bs_desc = {};
	{
		bs_desc.AlphaToCoverageEnable = false;
		bs_desc.IndependentBlendEnable = false;
		for (auto i = 0u; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {

			bs_desc.RenderTarget[i] = rtb_desc;
		}
	};

	//----------------------

	ID3DBlob* VSblob = nullptr;
	res = D3DReadFileToBlob(SHADER_FILENAME::DefPostVS , &VSblob);
	if (FAILED(res))     return 0;

	ID3DBlob* PSblob = nullptr;
	res = D3DReadFileToBlob(SHADER_FILENAME::DefPostPS, &PSblob);
	if (FAILED(res))     return 0;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
	{
		pso_desc.InputLayout = SIMPLEVERTEX::inp_Layout;
		pso_desc.pRootSignature = rtsg;
		pso_desc.VS.pShaderBytecode = VSblob->GetBufferPointer();
		pso_desc.PS.pShaderBytecode = PSblob->GetBufferPointer();
		pso_desc.VS.BytecodeLength = VSblob->GetBufferSize();
		pso_desc.PS.BytecodeLength = PSblob->GetBufferSize();
		pso_desc.RasterizerState = rs_desc;
		pso_desc.BlendState = bs_desc;
		pso_desc.SampleDesc.Count = 1;
		pso_desc.SampleDesc.Quality = 0;
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso_desc.NumRenderTargets = 1;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		pso_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	}

	res = device_->CreateGraphicsPipelineState
	(
		&pso_desc,
		IID_PPV_ARGS(&ptr)
	);
	if (FAILED(res))return false;

	return true;
}

bool DefUI::Init(D3D12_ROOT_PARAMETER* params, D3D12_STATIC_SAMPLER_DESC* sampler, D3D12_ROOT_SIGNATURE_FLAGS flag, uint16_t paramcnt, uint16_t sampcnt)
{

	auto device_ = PTR_D3D::ptr->GetDevice();
	ID3DBlob* S_blob = nullptr;
	ID3DBlob* E_blob = nullptr;
	HRESULT res = NULL;

	{
		D3D12_ROOT_SIGNATURE_DESC rtdesc = {};
		{
			rtdesc.Flags = flag;
			rtdesc.NumParameters = paramcnt;
			rtdesc.NumStaticSamplers = sampcnt;
			rtdesc.pParameters = params;
			rtdesc.pStaticSamplers = sampler;
		}

		res = D3D12SerializeRootSignature
		(
			&rtdesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0,
			&S_blob,
			&E_blob
		);
		if (FAILED(res))		return false;


		res = device_->CreateRootSignature
		(
			NULL,
			S_blob->GetBufferPointer(),
			S_blob->GetBufferSize(),
			IID_PPV_ARGS(&rtsg)
		);
		if (FAILED(res))		return false;

		D3D12_RENDER_TARGET_BLEND_DESC alp_desc =
		{
			true,false,
			D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,D3D12_COLOR_WRITE_ENABLE_ALL
		};

		D3D12_BLEND_DESC bld_desc = {};
		{
			bld_desc.AlphaToCoverageEnable = false;
			bld_desc.IndependentBlendEnable = false;
			for (auto i = 0u; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
				bld_desc.RenderTarget[i] = alp_desc;
			}
		}

		D3D12_DEPTH_STENCIL_DESC dss_desc = {};
		{
			dss_desc.DepthEnable = 0;
			dss_desc.StencilEnable = 0;
		}

		ID3DBlob* VSBlob = nullptr;
		ID3DBlob* PSBlob = nullptr;

		res = D3DReadFileToBlob(SHADER_FILENAME::UI_PS, &PSBlob);
		if (FAILED(res))		return false;

		res = D3DReadFileToBlob(SHADER_FILENAME::UI_VS, &VSBlob);
		if (FAILED(res))			return false;

		D3D12_RASTERIZER_DESC rs_desc = {};
		{
			rs_desc.FillMode = D3D12_FILL_MODE_SOLID;
			rs_desc.CullMode = D3D12_CULL_MODE_FRONT;
			rs_desc.FrontCounterClockwise = false;
			rs_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			rs_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			rs_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			rs_desc.AntialiasedLineEnable = false;
			rs_desc.DepthClipEnable = false;
			rs_desc.MultisampleEnable = false;
			rs_desc.ForcedSampleCount = 0;
			rs_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		}

		D3D12_GRAPHICS_PIPELINE_STATE_DESC gps_desc = {};
		{
			gps_desc.InputLayout = SIMPLEVERTEX::inp_Layout;
			gps_desc.BlendState = bld_desc;
			gps_desc.NumRenderTargets = 1;
			gps_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			gps_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			gps_desc.RasterizerState = rs_desc;
			gps_desc.PS.BytecodeLength = PSBlob->GetBufferSize();
			gps_desc.PS.pShaderBytecode = PSBlob->GetBufferPointer();
			gps_desc.VS.BytecodeLength = VSBlob->GetBufferSize();
			gps_desc.VS.pShaderBytecode = VSBlob->GetBufferPointer();
			gps_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
			gps_desc.SampleMask = UINT_MAX;
			gps_desc.SampleDesc.Count = 1;
			gps_desc.SampleDesc.Quality = 0;
			gps_desc.pRootSignature = rtsg;
		}

		res = device_->CreateGraphicsPipelineState
		(
			&gps_desc,
			IID_PPV_ARGS(&ptr)
		);
		if (FAILED(res)) return false;

	}

	return true;
}
