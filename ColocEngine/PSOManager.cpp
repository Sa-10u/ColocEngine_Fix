#include "PSOManager.h"
#include"ResourceManager.h"
#include"PSOs.h"

namespace PSOManager
{
    std::array<PSO*, static_cast<uint32_t>(Shader3D::AMMOUNT)> PSO3D = {&def3D , &defToon};
    std::array<PSO*, static_cast<uint32_t>(ShaderPost::AMMOUNT)> PSOPost = {&defPost};
}

bool PSOManager::Init()
{
    {
        D3D12_ROOT_PARAMETER r_param[Amount] = {};
        {
            r_param[CB_U].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[CB_U].Descriptor.RegisterSpace = 0;
            r_param[CB_U].Descriptor.ShaderRegister = 0;
            r_param[CB_U].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        {
            r_param[CB_C].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[CB_C].Descriptor.RegisterSpace = 0;
            r_param[CB_C].Descriptor.ShaderRegister = 256;
            r_param[CB_C].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        {
            r_param[CB_L].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[CB_L].Descriptor.RegisterSpace = 0;
            r_param[CB_L].Descriptor.ShaderRegister = 512;
            r_param[CB_L].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_SBOI = {};
        {
            range_SBOI.BaseShaderRegister = 0;
            range_SBOI.NumDescriptors = ResourceManager::CBCOUNT;
            range_SBOI.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_SBOI.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_SBOI.RegisterSpace = 0;
        }

        r_param[SB_OI];
        {
            r_param[SB_OI].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[SB_OI].DescriptorTable.NumDescriptorRanges = 1;
            r_param[SB_OI].DescriptorTable.pDescriptorRanges = &range_SBOI;
            r_param[SB_OI].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_SBMB = {};
        {
            range_SBMB.BaseShaderRegister = 512;
            range_SBMB.NumDescriptors = ResourceManager::CBCOUNT;
            range_SBMB.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_SBMB.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_SBMB.RegisterSpace = 0;
        }

        r_param[SB_MB];
        {
            r_param[SB_MB].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[SB_MB].DescriptorTable.NumDescriptorRanges = 1;
            r_param[SB_MB].DescriptorTable.pDescriptorRanges = &range_SBMB;
            r_param[SB_MB].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_SBMTL = {};
        {
            range_SBMTL.BaseShaderRegister = 1024;
            range_SBMTL.NumDescriptors = ResourceManager::CBCOUNT;
            range_SBMTL.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_SBMTL.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_SBMTL.RegisterSpace = 0;
        }

        r_param[SB_MTL];
        {
            r_param[SB_MTL].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[SB_MTL].DescriptorTable.NumDescriptorRanges = 1;
            r_param[SB_MTL].DescriptorTable.pDescriptorRanges = &range_SBMTL;
            r_param[SB_MTL].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_Tex = {};
        {
            range_Tex.BaseShaderRegister = 1792;
            range_Tex.NumDescriptors = -1;
            range_Tex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_Tex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_Tex.RegisterSpace = 0;
        }

        r_param[TEX];
        {
            r_param[TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[TEX].DescriptorTable.NumDescriptorRanges = 1;
            r_param[TEX].DescriptorTable.pDescriptorRanges = &range_Tex;
            r_param[TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_STATIC_SAMPLER_DESC sampler = {};
        {
            sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
            sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            sampler.MaxAnisotropy = 1;
            sampler.MaxLOD = D3D12_FLOAT32_MAX;
            sampler.MinLOD = D3D12_FLOAT32_MAX * -1;
            sampler.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
            sampler.RegisterSpace = 0;
            sampler.ShaderRegister = 0;
            sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        }

        auto res = 0u;
        {

            D3D12_STATIC_SAMPLER_DESC sampler = {};
            {
                sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
                sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
                sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
                sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
                sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
                sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
                sampler.MaxAnisotropy = 1;
                sampler.MaxLOD = D3D12_FLOAT32_MAX;
                sampler.MinLOD = D3D12_FLOAT32_MAX * -1;
                sampler.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
                sampler.RegisterSpace = 0;
                sampler.ShaderRegister = 0;
                sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
            }

            D3D12_ROOT_SIGNATURE_FLAGS flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

            for (auto i = 0u; i < static_cast<size_t>(Shader3D::AMMOUNT); i++) {

                if (!PSO3D[i]->Init(r_param, &sampler, flag, Amount, 1))   return false;
            }
        }
    }

    {
        D3D12_ROOT_PARAMETER r_param[P_Amount] = {};

        D3D12_STATIC_SAMPLER_DESC sampler = {};
        {
            sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
            sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            sampler.MaxAnisotropy = 1;
            sampler.MaxLOD = D3D12_FLOAT32_MAX;
            sampler.MinLOD = D3D12_FLOAT32_MAX * -1;
            sampler.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
            sampler.RegisterSpace = 0;
            sampler.ShaderRegister = 0;
            sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        }

        D3D12_ROOT_SIGNATURE_FLAGS flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        for (auto i = 0u; i < static_cast<size_t>(ShaderPost::AMMOUNT); i++) {

          //  if (!PSOPost[i]->Init(r_param, &sampler, flag, 0, 0))   return false;
        }
    }

    return true;
}

void PSOManager::Term()
{
    for (auto i = 0u; i < static_cast<size_t>(Shader3D::AMMOUNT); i++) {

        PSO3D[i]->Term();
    }

    for (auto i = 0u; i < static_cast<size_t>(ShaderPost::AMMOUNT); i++) {

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

