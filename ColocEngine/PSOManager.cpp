#include "PSOManager.h"
#include"ResourceManager.h"
#include"ShaderCompiler.h"

namespace PSOManager
{
    std::array<PSO*, static_cast<uint32_t>(ShaderDeferred::AMOUNT)> PSODeferred = { new DefDeferred()};
    std::array<PSO*, static_cast<uint32_t>(Shader3D::AMOUNT)> PSO3D = {new Def3D()};
    std::array<PSO*, static_cast<uint32_t>(ShaderPost::AMOUNT)> PSOPost = {new DefPost()};
    std::array<PSO*, static_cast<uint32_t>(ShaderUI::AMOUNT)>PSOUI = {new DefUI()};
    std::array<PSO*, static_cast<uint32_t>(ShaderCompute::AMOUNT)>PSOComp = {new BoneAnimCompute()};
}

bool PSOManager::Init()
{
    ShaderModel6_8::Init();

    constexpr size_t SigmaBufferRegisterSize = D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT;
    {
        D3D12_ROOT_PARAMETER r_param[D_Amount] = {};

        {
            r_param[D_CB_U].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[D_CB_U].Descriptor.RegisterSpace = 0;
            r_param[D_CB_U].Descriptor.ShaderRegister = 0;
            r_param[D_CB_U].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        {
            r_param[D_CB_C].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[D_CB_C].Descriptor.RegisterSpace = 256;
            r_param[D_CB_C].Descriptor.ShaderRegister = 0;
            r_param[D_CB_C].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        {
            r_param[D_CB_L].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[D_CB_L].Descriptor.RegisterSpace = 512;
            r_param[D_CB_L].Descriptor.ShaderRegister = 0;
            r_param[D_CB_L].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_tex = {};
        {
            range_tex.BaseShaderRegister = 512 + SigmaBufferRegisterSize;
            range_tex.NumDescriptors = -1;
            range_tex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_tex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_tex.RegisterSpace = 0;
        }

        {
            r_param[D_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[D_TEX].DescriptorTable.NumDescriptorRanges = 1;
            r_param[D_TEX].DescriptorTable.pDescriptorRanges = &range_tex;
            r_param[D_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        }

        D3D12_DESCRIPTOR_RANGE range_render[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};

        for(uint16_t i = D_R_Color; i < D_Amount; ++i){
            r_param[i] = {};

            auto _index = i - D_R_Color;
            range_render[_index].BaseShaderRegister = _index;
            range_render[_index].NumDescriptors = 1;
            range_render[_index].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_render[_index].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_render[_index].RegisterSpace = 0;

            r_param[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[i].DescriptorTable.NumDescriptorRanges = 1;
            r_param[i].DescriptorTable.pDescriptorRanges = &range_render[_index];
            r_param[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
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

            D3D12_ROOT_SIGNATURE_FLAGS flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

            for (auto i = 0u; i < static_cast<size_t>(ShaderDeferred::AMOUNT); i++) {

                if (!PSODeferred[i]->Init(r_param, &sampler, flag, D_Amount, 1))   return false;
            }
        }
    }

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
            r_param[SB_MB].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
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
            r_param[SB_MTL].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
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

            D3D12_ROOT_SIGNATURE_FLAGS flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
            flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

            for (auto i = 0u; i < static_cast<size_t>(Shader3D::AMOUNT); i++) {

                if (!PSO3D[i]->Init(r_param, &sampler, flag, Amount, 1))   return false;
            }
        }
    }

    {
        D3D12_ROOT_PARAMETER r_param[P_Amount] = {};
        {
            r_param[P_CB].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            r_param[P_CB].Descriptor.RegisterSpace = 0;
            r_param[P_CB].Descriptor.ShaderRegister = 0;
            r_param[P_CB].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        }

        D3D12_DESCRIPTOR_RANGE range_mb = {};
        {
            range_mb.BaseShaderRegister = 0 + SigmaBufferRegisterSize;
            range_mb.NumDescriptors = ResourceManager::CBCOUNT;
            range_mb.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_mb.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_mb.RegisterSpace = 0;
        }

        {
            r_param[P_SB_MB].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[P_SB_MB].DescriptorTable.pDescriptorRanges = &range_mb;
            r_param[P_SB_MB].DescriptorTable.NumDescriptorRanges = 1;
            r_param[P_SB_MB].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        }

        D3D12_DESCRIPTOR_RANGE range_render[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};

        for (uint16_t i = P_R_Color; i < P_Amount; ++i) {
            r_param[i] = {};

            auto _index = i - P_R_Color;
            range_render[_index].BaseShaderRegister = _index;
            range_render[_index].NumDescriptors = 1;
            range_render[_index].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_render[_index].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_render[_index].RegisterSpace = 0;

            r_param[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[i].DescriptorTable.NumDescriptorRanges = 1;
            r_param[i].DescriptorTable.pDescriptorRanges = &range_render[_index];
            r_param[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        }

        D3D12_DESCRIPTOR_RANGE range_tex = {};
        {
            range_tex.BaseShaderRegister = 512 + SigmaBufferRegisterSize;
            range_tex.NumDescriptors = -1;
            range_tex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_tex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_tex.RegisterSpace = 0;
        }

        {
            r_param[P_TEX].DescriptorTable.NumDescriptorRanges = 1;
            r_param[P_TEX].DescriptorTable.pDescriptorRanges = &range_tex;
            r_param[P_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[P_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
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

        D3D12_ROOT_SIGNATURE_FLAGS flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        for (auto i = 0u; i < static_cast<size_t>(ShaderPost::AMOUNT); i++) {

            if (!PSOPost[i]->Init(r_param, &sampler, flag, P_Amount, 1))   return false;
        }
    }

    {
        D3D12_ROOT_PARAMETER r_param[U_Amount] = {};
        {
            r_param[U_CB].Descriptor.RegisterSpace = 0;
            r_param[U_CB].Descriptor.ShaderRegister = 0;
            r_param[U_CB].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param[U_CB].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_oi = {};
        {
            range_oi.BaseShaderRegister = 0;
            range_oi.NumDescriptors = ResourceManager::CBCOUNT;
            range_oi.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_oi.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_oi.RegisterSpace = 0;
        }
        {
            r_param[U_SB_OI].DescriptorTable.NumDescriptorRanges = 1;
            r_param[U_SB_OI].DescriptorTable.pDescriptorRanges = &range_oi;
            r_param[U_SB_OI].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[U_SB_OI].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }

        D3D12_DESCRIPTOR_RANGE range_mb = {};
        {
            range_mb.BaseShaderRegister = 256;
            range_mb.NumDescriptors = ResourceManager::CBCOUNT;
            range_mb.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_mb.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_mb.RegisterSpace = 0;
        }
        {
            r_param[U_SB_MB].DescriptorTable.NumDescriptorRanges = 1;
            r_param[U_SB_MB].DescriptorTable.pDescriptorRanges = &range_mb;
            r_param[U_SB_MB].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[U_SB_MB].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        }

        D3D12_DESCRIPTOR_RANGE range_tex = {};
        {
            range_tex.BaseShaderRegister = 512;
            range_tex.NumDescriptors = -1;
            range_tex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            range_tex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            range_tex.RegisterSpace = 0;
        }
        {
            r_param[U_TEX].DescriptorTable.NumDescriptorRanges = 1;
            r_param[U_TEX].DescriptorTable.pDescriptorRanges = &range_tex;
            r_param[U_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            r_param[U_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
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

        D3D12_ROOT_SIGNATURE_FLAGS flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;


        for (auto i = 0u; i < static_cast<size_t>(ShaderUI::AMOUNT); i++) {

            if (!PSOUI[i]->Init(r_param, &sampler, flag, U_Amount, 1))  return false;
        }
    }
    
    //---compute
    {
        D3D12_ROOT_PARAMETER r_param = {};
        {
            r_param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        }
    }

    return true;
}

void PSOManager::Term()
{

    auto Term_and_Delete = [&](PSO** arr, size_t length)->void {

        for (auto i = 0u; i < length; ++i) {

            arr[i]->Term();
            delete arr[i];
        }

        };
    Term_and_Delete(PSODeferred.data(), static_cast<size_t>(ShaderDeferred::AMOUNT));
    Term_and_Delete(PSO3D.data(), static_cast<size_t>(Shader3D::AMOUNT));
    Term_and_Delete(PSOPost.data(), static_cast<size_t>(ShaderPost::AMOUNT));
    Term_and_Delete(PSOUI.data(), static_cast<size_t>(ShaderUI::AMOUNT));
    Term_and_Delete(PSOComp.data(), static_cast<size_t>(ShaderCompute::AMOUNT));
}

PSO* PSOManager::GetPSO(ShaderDeferred ind)
{
    return PSODeferred.at(static_cast<size_t>(ind));
}

PSO* PSOManager::GetPSO(Shader3D ind)
{
    return PSO3D.at(static_cast<size_t>(ind));
}

PSO* PSOManager::GetPSO(ShaderPost ind)
{
    return PSOPost.at(static_cast<size_t>(ind));
}

PSO* PSOManager::GetPSO(ShaderUI ind)
{
    return PSOUI.at(static_cast<size_t>(ind));
}

PSO* PSOManager::GetPSO(ShaderCompute ind)
{
    return PSOComp.at(static_cast<size_t>(ind));
}

