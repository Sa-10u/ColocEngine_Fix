#include "PSO.h"
#include"D3D.h"

PSO::PSO()
{
}

void PSO::Term()
{
	ptr->Release();
	ptr = nullptr;
}

ID3D12PipelineState* PSO::GetPSO()
{
	return ptr;
}

ID3D12RootSignature* PSO::GetRTSG()
{
    return rtsg;
}
