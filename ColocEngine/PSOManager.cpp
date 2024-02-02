#include "PSOManager.h"

bool PSOManager::Init()
{
    auto res = 0u;

    for (auto i = 0u; i < static_cast<size_t>(Shader3D::AMMOUNT); i++) {

        if (!PSO3D[i]->Init())   return false;
        if (!PSOPost[i]->Init())   return false;
    }
}

void PSOManager::Term()
{
    for (auto i = 0u; i < static_cast<size_t>(Shader3D::AMMOUNT); i++) {

        PSO3D[i]->Term();
        PSOPost[i]->Term();
    }
}

PSO* PSOManager::GetPSO(Shader3D ind)
{
    return PSO3D[static_cast<size_t>(ind)];
}

PSO* PSOManager::GetPSO(ShaderPost ind)
{
    return PSOPost[static_cast<size_t>(ind)];
}

namespace PSOManager
{
    std::array<PSO*, static_cast<uint32_t>(Shader3D::AMMOUNT)> PSO3D = {&Def3D , &DefToon};
    std::array<PSO*, static_cast<uint32_t>(ShaderPost::AMMOUNT)> PSOPost = {&DefPost};
}