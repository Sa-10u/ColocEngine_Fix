#include "MeshLoader.h"
#include "MACRO.h"
#include"Resource.h"
#include<cstdint>
#include"FileLoader.h"
#include "ResourceManager.h"
#include<functional>

static constexpr unsigned int TRIANGLE = 3;

const char* UrevType[] =
{
    ""
};
const char* VrevType[] =
{
    "FBX",
};

constexpr auto V_TYPESIZE = _countof(VrevType);
constexpr auto U_TYPESIZE = _countof(UrevType);

MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{
}

bool MeshLoader::Load(const wchar_t* file, vector<MESH>& mesh, vector<Material>& mtr , std::vector<Armature>& amt)
{
    if (file == nullptr) return false;
    auto path = wtoc(file);

    UVCheck(path);
    Assimp::Importer imp = {};
    //---------
    auto flag = 0;
    flag |= aiProcess_Triangulate;
    flag |= aiProcess_CalcTangentSpace;
    flag |= aiProcess_GenSmoothNormals;
    flag |= aiProcess_GenUVCoords;
    flag |= aiProcess_RemoveRedundantMaterials;
    flag |= aiProcess_OptimizeMeshes;
    flag |= aiProcess_LimitBoneWeights;
    flag |= aiProcess_FixInfacingNormals;
    flag |= aiProcess_PopulateArmatureData;
    //----------------
    auto scene = imp.ReadFile(path, flag);
    if (scene == nullptr)    return false;

    mesh.clear();
    mesh.resize(scene->mNumMeshes);

    uint16_t cnt_mesh = 0u;

    std::function<void(aiNode* ,Mat mat)> getMesh = [&](aiNode* node,Mat r_mat)->void
        {
            auto m = node->mTransformation;
            Mat mat =
            {
                m.a1,m.b1,m.c1,m.d1,
                m.a2,m.b2,m.c2,m.d2,
                m.a3,m.b3,m.c3,m.d3,
                m.a4,m.b4,m.c4,m.d4
            };

            mat = mat * r_mat;

            for (auto i = 0u; i < node->mNumChildren; ++i) {

                getMesh(node->mChildren[i],mat);
            }

            for (auto i = 0u; i < node->mNumMeshes; ++i) {

                auto index = cnt_mesh++;
                auto g = scene->mMeshes[node->mMeshes[i]]->mBones[0];

                //do for. this is only include first child , more than secondaly will been ignore.

                for (auto bn = 0u;bn < scene->mMeshes[node->mMeshes[i]]->mBones[0]->mArmature->mNumChildren;++bn){
                    ParseBone(amt, scene->mMeshes[node->mMeshes[i]]->mBones[0]->mArmature->mChildren[bn], mat, mesh[index]);
                }
                ParseMesh(mesh[index], scene->mMeshes[node->mMeshes[i]],mat,amt);
            }

        };

    Mat r_mat =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    getMesh(scene->mRootNode, r_mat);

    mtr.clear();
    mtr.resize(scene->mNumMaterials);
    for (size_t i = 0; i < mtr.size(); i++) {

        const auto pm = scene->mMaterials[i];
        ParseMaterial(mtr[i],mesh[i].defTex_ ,pm);
    }

    for (auto i = 0; i < scene->mNumAnimations; ++i) {

        auto anim = scene->mAnimations[i];
        for (auto c = 0u; c < anim->mNumChannels; ++c) {

            auto channel = anim->mChannels[c];
            auto p0 = channel->mPositionKeys[0];
            auto p1 = channel->mPositionKeys[1];

            auto t = 0;
        }

        for (auto mc = 0u; mc < anim->mNumMeshChannels; ++mc) {
        
            auto mesh = anim->mMeshChannels[mc];

            auto t = 0;
        }
        for (auto mm = 0u; mm < anim->mNumMorphMeshChannels; mm++) {

            auto morph = anim->mMorphMeshChannels[mm];
            morph->mNumKeys;

            auto t = 0;
        }
    }

    scene = nullptr;
    imp.FreeScene();

    return true;
}

void MeshLoader::ParseMesh(MESH& mesh, const aiMesh* src, Mat mat, vector<Armature>& amt)
{
    mesh.ID_Material = src->mMaterialIndex;

    aiVector3D vecdef(0.0f, 0.0f, 0.0f);

    mesh.vtcs_.resize(src->mNumVertices);
    

    for (auto i = 0u; i < src->mNumVertices; i++) {

        auto pos = &(src->mVertices[i]);
        auto norm = &(src->mNormals[i]);
        auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &vecdef;
        auto tan = (src->HasTangentsAndBitangents()) ? &(src->mTangents[i]) : &vecdef;
        auto bitan = (src->HasTangentsAndBitangents()) ? &(src->mBitangents[i]) : &vecdef;
        auto Mtl_ID = mesh.ID_Material;
        ParseUV(*uv);

        float3 p = float3(pos->x, pos->y, pos->z);
        float3 n = float3(norm->x, norm->y, norm->z);
        float3 t = float3(tan->x, tan->y, tan->z);
        float3 b = float3(bitan->x, bitan->y, bitan->z);

        mesh.vtcs_[i] = VERTEX
        (
            p*mat,
            n*mat,
            float2(uv->x, uv->y),
            t * mat,
            b * mat,
            Mtl_ID
        );
    }

    {
        mesh.indexes_.resize(src->mNumFaces * TRIANGLE);
    }

    auto weighting = [](float4& wgts,XMINT4& ids,float wgt,int id) 
        {
            if (wgts.x == .0f) { wgts.x = wgt; ids.x = id; return; }
            if (wgts.y == .0f) { wgts.y = wgt; ids.y = id; return; }
            if (wgts.z == .0f) { wgts.z = wgt; ids.z = id; return; }
            if (wgts.w == .0f) { wgts.w = wgt; ids.w = id; return; }
        };

    auto getBoneID = [&](const string& name)->uint16_t
        {
            return amt[mesh.index_armature].BonenameIndex_.at(name);
        };
    
    static int cnt = 0;
    for (auto i = 0u; i < src->mNumBones; ++i) {
        auto bone = src->mBones[i];
        if (!(bone->mNumWeights))    continue;

        auto bID = getBoneID(bone->mName.C_Str());
        for (auto j = 0u; j < bone->mNumWeights; ++j) {

            weighting
            (
                mesh.vtcs_[bone->mWeights[j].mVertexId].bWghts_,
                mesh.vtcs_[bone->mWeights[j].mVertexId].bIDs_,
                bone->mWeights[j].mWeight,
                bID
            );
        }
    }

    for (auto i = 0; i < src->mNumFaces; i++) {

        const auto& face = src->mFaces[i];

        mesh.indexes_[i * TRIANGLE + 0] = face.mIndices[0];
        mesh.indexes_[i * TRIANGLE + 1] = face.mIndices[1];
        mesh.indexes_[i * TRIANGLE + 2] = face.mIndices[2];
    }

   
}

void MeshLoader::ParseMaterial(Material& mtl, MapBOOL& mpb, const aiMaterial* src)
{
    {
        aiColor3D coltemp(0.0f, 0.0f, 0.0f);
        auto ResetColor = [&]()
        {
            coltemp.r = 0.0f;
            coltemp.g = 0.0f;
            coltemp.b = 0.0f;
        };

        __CREATE("DIFFUSE")
            if (src->Get(AI_MATKEY_COLOR_DIFFUSE, coltemp) == AI_SUCCESS)
            {
                mtl.dif.x = coltemp.r;
                mtl.dif.y = coltemp.g;
                mtl.dif.z = coltemp.b;
            }
            else
            {
                ResetColor();
                mtl.dif.x = coltemp.r;
                mtl.dif.y = coltemp.g;
                mtl.dif.z = coltemp.b;
            }

        __CREATE("SPECULAR")
            if (src->Get(AI_MATKEY_COLOR_SPECULAR, coltemp) == AI_SUCCESS)
            {
                mtl.spec.x = coltemp.r;
                mtl.spec.y = coltemp.g;
                mtl.spec.z = coltemp.b;
            }
            else
            {
                ResetColor();
                mtl.spec.x = coltemp.r;
                mtl.spec.y = coltemp.g;
                mtl.spec.z = coltemp.b;
            }

        __CREATE("SHININESS")   auto val = 0.0f;
        if (src->Get(AI_MATKEY_SHININESS, val) == AI_SUCCESS)
        {
            mtl.shin = val;
        }
        else
        {
            mtl.shin = val;
        }

        __CREATE("ALPHA")   val = 1.0f;
        if (src->Get(AI_MATKEY_OPACITY, val) == AI_SUCCESS)
        {
            mtl.alp = val;
        }
        else
        {
            mtl.shin = val;
        }

        __CREATE("EMISSION")
            if (src->Get(AI_MATKEY_EMISSIVE_INTENSITY, coltemp) == AI_SUCCESS)
            {
                mtl.emis.x = coltemp.r;
                mtl.emis.y = coltemp.g;
                mtl.emis.z = coltemp.b;
            }
            else
            {
                ResetColor();
                mtl.emis.x = coltemp.r;
                mtl.emis.y = coltemp.g;
                mtl.emis.z = coltemp.b;
            }

        auto FixTexIndex = [](int ind) {if (ind == NULL)   return -1; return ind; };
        auto nullTex = -1;

        __CREATE("DIFFUSE_MAP") aiString path = {};
        if (src->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
        {
            auto str = string(path.C_Str());
            str = FileNormalization(&str);

            auto wstr = ctow(str.c_str());
            mpb.isD = FixTexIndex(ResourceManager::TexLoad(wstr));
        }
        else
        {
            mpb.isD = nullTex;
        }

        __CREATE("EMISSION_MAP")
            if (src->GetTexture(aiTextureType_EMISSIVE, 0, &path) == AI_SUCCESS)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isE = FixTexIndex(ResourceManager::TexLoad(wstr));
            }
            else
            {
                mpb.isE = nullTex;
            }

        __CREATE("NORMAL_MAP")
            if (src->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isN = FixTexIndex(ResourceManager::TexLoad(wstr));
            }
            else
            {
                mpb.isN = nullTex;
            }

        __CREATE("SPECULAR_MAP")
            if (src->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isS = FixTexIndex(ResourceManager::TexLoad(wstr));
            }
            else
            {
                mpb.isS = nullTex;
            }

        __CREATE("ALPHA_MAP")
            if (src->GetTexture(aiTextureType_OPACITY, 0, &path) == AI_SUCCESS)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isESB = FixTexIndex(ResourceManager::TexLoad(wstr));
            }
            else
            {
                mpb.isESB = nullTex;
            }

        __CREATE("SHININESS_MAP")
        {
            if (mpb.isESB == NULL)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isESB = FixTexIndex(ResourceManager::TexLoad(wstr));
            }
        }

        __CREATE("EMISSIVE_INTENCITY_MAP")
        {
            if (mpb.isESB == NULL)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isESB = ResourceManager::TexLoad(wstr);
            }
        }
    }
}

void MeshLoader::ParseBone(std::vector<Armature>& arm, const aiNode* src, Mat mat, MESH& mesh)
{
    for (auto i = 0u; i < arm.size(); ++i) {

        if (src->mParent->mName.C_Str() == arm[i].name_) { mesh.index_armature = i; return; }
    }

    Armature temp = {};
    
    temp.name_ = src->mParent->mName.C_Str();

    std::function<void(aiNode* ,Mat)>pushBone = [&](aiNode* me,Mat mat) 
        {
            BONE_INFO info = {};
            
            
            auto m = me->mTransformation;
            Mat mymat =
            {
                m.a1,m.b1,m.c1,m.d1,
                m.a2,m.b2,m.c2,m.d2,
                m.a3,m.b3,m.c3,m.d3,
                m.a4,m.b4,m.c4,m.d4
            };
            info.bind_ = mymat * mat;

            temp.bnsinfo_.push_back(info);

            auto name = std::pair<string, uint16_t>{ me->mName.C_Str(),static_cast<uint16_t>(temp.bnsinfo_.size() - 1) };
            temp.BonenameIndex_.insert(name);

            for (auto i = 0u; i < me->mNumChildren; ++i) {

                pushBone(me->mChildren[i], mat);
            }
        };

    auto m = src->mTransformation;
    Mat offset =
    {
        m.a1,m.b1,m.c1,m.d1,
        m.a2,m.b2,m.c2,m.d2,
        m.a3,m.b3,m.c3,m.d3,
        m.a4,m.b4,m.c4,m.d4
    };

    mat = mat * offset;
    for (auto i = 0u; i < src->mNumChildren; ++i) {

        pushBone(src->mChildren[i], mat);
    }

    arm.push_back(temp);
    mesh.index_armature = arm.size() - 1;
}

void MeshLoader::ParseUV(aiVector3D& uv)
{
    (this->*NormU)(uv.x);
    (this->*NormV)(uv.y);
};

void MeshLoader::UVCheck(char* str)
{
    string js(str);
    auto type = FileType(&js);

    NormU = &MeshLoader::UnReverse;

    for (auto i = 0u; i < U_TYPESIZE; i++) {

        if (type == UrevType[i])
        {
            NormU = &MeshLoader::Reverse;
            break;
        }
    }

    for (auto i = 0u; i < V_TYPESIZE; i++) {

        if (type == VrevType[i])
        {
            NormV = &MeshLoader::Reverse;
            break;
        }
    }
}

void MeshLoader::UVCheck(wchar_t* str)
{
    std::wstring js(str);
    auto type = FileType(&js);
}

void MeshLoader::Reverse(ai_real& val)
{
    val = 1 - val;
}

void MeshLoader::UnReverse(ai_real& val)
{
}

//--------------------------------------------

bool LoadMesh(const wchar_t* file, RModel* ptr)
{
    MeshLoader ml = {};

    return (ml.Load(file, ptr->Mesh_, ptr->Mtr_ ,ptr->armature_));
}