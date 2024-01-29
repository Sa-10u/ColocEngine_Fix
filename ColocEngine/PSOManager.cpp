#include "PSOManager.h"

void PSOManager::Init()
{
    for (auto i = 0u; i < static_cast<size_t>(Shader3D::AMMOUNT); i++) {

    }

    for (auto i = 0u; i < static_cast<size_t>(ShaderPost::AMMOUNT); i++) {

    }
}

ID3D12PipelineState* PSOManager::GetPSO(Shader3D ind)
{
    return PSO3D[static_cast<size_t>(ind)];
}

ID3D12PipelineState* PSOManager::GetPSO(ShaderPost ind)
{
    return PSOPost[static_cast<size_t>(ind)];
}

namespace PSOManager
{
    std::array<ID3D12PipelineState*, static_cast<uint32_t>(Shader3D::AMMOUNT)> PSO3D;
    std::array<ID3D12PipelineState*, static_cast<uint32_t>(ShaderPost::AMMOUNT)> PSOPost;
}