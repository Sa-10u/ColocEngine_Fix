#include "MeshLoader.h"
#include "MACRO.h"
#include"Resource.h"
#include<cstdint>
#include"FileLoader.h"
#include "ResourceManager.h"

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

bool MeshLoader::Load(const wchar_t* file, vector<MESH>& mesh, vector<MATERIAL>& mtr)
{
    if (file == nullptr) return false;
    auto path = wtoc(file);

    UVCheck(path);
    Assimp::Importer imp = {};
    //---------
    auto flag = 0;
    flag |= aiProcess_Triangulate;
    flag |= aiProcess_PreTransformVertices;
    flag |= aiProcess_CalcTangentSpace;
    flag |= aiProcess_GenSmoothNormals;
    flag |= aiProcess_GenUVCoords;
    flag |= aiProcess_RemoveRedundantMaterials;
    flag |= aiProcess_OptimizeMeshes;
    //----------------
    auto scene = imp.ReadFile(path, flag);
    if (scene == nullptr)    return false;

    mesh.clear();
    mesh.resize(scene->mNumMeshes);
    for (size_t i = 0; i < mesh.size(); i++) {

        const auto pm = scene->mMeshes[i];
        ParseMesh(mesh[i], pm);
    }

    mtr.clear();
    mtr.resize(scene->mNumMaterials);
    for (size_t i = 0; i < mtr.size(); i++) {

        const auto pm = scene->mMaterials[i];
        ParseMaterial(mtr[i],mesh[i].defTex_ ,pm);
    }
    scene = nullptr;
    imp.FreeScene();

    return true;
}

bool MeshLoader::Load(const wchar_t* file, RModel* ptr)
{
    if (file == nullptr) return false;
    auto path = wtoc(file);

    UVCheck(path);
    Assimp::Importer imp = {};
    //---------
    auto flag = 0;
    flag |= aiProcess_Triangulate;
    flag |= aiProcess_PreTransformVertices;
    flag |= aiProcess_CalcTangentSpace;
    flag |= aiProcess_GenSmoothNormals;
    flag |= aiProcess_GenUVCoords;
    flag |= aiProcess_RemoveRedundantMaterials;
    flag |= aiProcess_OptimizeMeshes;
    //----------------
    auto scene = imp.ReadFile(path, flag);
    if (scene == nullptr)    return false;

    //--------------------------------------------
    ptr->Mesh_.clear();
    ptr->Mesh_.resize(scene->mNumMeshes);
    for (size_t i = 0; i < ptr->Mesh_.size(); i++) {

        const auto pm = scene->mMeshes[i];
        ParseMesh(ptr->Mesh_[i], pm);
    }
    //--------------------------------------------
    ptr->Mtr_.clear();
    ptr->Mtr_.resize(scene->mNumMaterials);
    for (size_t i = 0; i < ptr->Mtr_.size(); i++) {


        const auto pm = scene->mMaterials[i];
        ParseMaterial(ptr->Mtr_[i],ptr->Mesh_[i].defTex_, pm);
    }
    //--------------------------------------------
    ptr->TexName_.clear();
    ptr->TexName_.resize(scene->mNumTextures);
    for (auto i = 0u; i < scene->mNumMaterials; i++) {

    }
    //----------

    for (size_t i = 0; i < ptr->Mesh_.size(); i++) {
        ptr->Mesh_[i].bnsinfo_.clear();
        ptr->Mesh_[i].bnsinfo_.resize(ptr->Mesh_[i].vtcs_.size());
        const auto pm = scene->mMeshes[i];

    }

    scene = nullptr;

    return true;
}

void MeshLoader::ParseMesh(MESH& mesh, const aiMesh* src)
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

        mesh.vtcs_[i] = VERTEX
        (
            XMFLOAT3(pos->x, pos->y, pos->z),
            XMFLOAT3(norm->x, norm->y, norm->z),
            XMFLOAT2(uv->x, uv->y),
            XMFLOAT3(tan->x, tan->y, tan->z),
            XMFLOAT3(bitan->x, bitan->y, bitan->z),
            Mtl_ID
        );
    }

    {
        auto n = src->mNumBones;
        mesh.indexes_.resize(src->mNumFaces * TRIANGLE);
    }

    for (auto i = 0; i < src->mNumFaces; i++) {

        const auto& face = src->mFaces[i];

        mesh.indexes_[i * TRIANGLE + 0] = face.mIndices[0];
        mesh.indexes_[i * TRIANGLE + 1] = face.mIndices[1];
        mesh.indexes_[i * TRIANGLE + 2] = face.mIndices[2];
    }
}

void MeshLoader::ParseMaterial(MATERIAL& mtl, MapBOOL& mpb, const aiMaterial* src)
{
    {
        aiColor3D coltemp(0.5f, 0.5f, 0.5f);
        auto ResetColor = [&]()
        {
            coltemp.r = 0.5f;
            coltemp.g = 0.5f;
            coltemp.b = 0.5f;
        };

        __CREATE("DIFFUSE")
            if (src->Get(AI_MATKEY_COLOR_DIFFUSE, coltemp) == AI_SUCCESS)
            {
                mtl.dif_.x = coltemp.r;
                mtl.dif_.y = coltemp.g;
                mtl.dif_.z = coltemp.b;
            }
            else
            {
                ResetColor();
                mtl.dif_.x = coltemp.r;
                mtl.dif_.y = coltemp.g;
                mtl.dif_.z = coltemp.b;
            }

        __CREATE("SPECULAR")
            if (src->Get(AI_MATKEY_COLOR_SPECULAR, coltemp) == AI_SUCCESS)
            {
                mtl.spec_.x = coltemp.r;
                mtl.spec_.y = coltemp.g;
                mtl.spec_.z = coltemp.b;
            }
            else
            {
                ResetColor();
                mtl.spec_.x = coltemp.r;
                mtl.spec_.y = coltemp.g;
                mtl.spec_.z = coltemp.b;
            }

        __CREATE("SHININESS")   auto val = 0.0f;
        if (src->Get(AI_MATKEY_SHININESS, val) == AI_SUCCESS)
        {
            mtl.shin_ = val;
        }
        else
        {
            mtl.shin_ = val;
        }

        __CREATE("ALPHA")   val = 1.0f;
        if (src->Get(AI_MATKEY_OPACITY, val) == AI_SUCCESS)
        {
            mtl.alpha_ = val;
        }
        else
        {
            mtl.shin_ = val;
        }

        __CREATE("EMISSION")
            if (src->Get(AI_MATKEY_EMISSIVE_INTENSITY, coltemp) == AI_SUCCESS)
            {
                mtl.emis_.x = coltemp.r;
                mtl.emis_.y = coltemp.g;
                mtl.emis_.z = coltemp.b;
            }
            else
            {
                ResetColor();
                mtl.emis_.x = coltemp.r;
                mtl.emis_.y = coltemp.g;
                mtl.emis_.z = coltemp.b;
            }

        __CREATE("DIFFUSE_MAP") aiString path = {};
        if (src->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
        {
            auto str = string(path.C_Str());
            str = FileNormalization(&str);

            auto wstr = ctow(str.c_str());
            mpb.isD = ResourceManager::TexLoad(wstr);
        }
        else
        {
            mpb.isD = NULL;
        }

        __CREATE("EMISSION_MAP")
            if (src->GetTexture(aiTextureType_EMISSIVE, 0, &path) == AI_SUCCESS)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isE = ResourceManager::TexLoad(wstr);
            }
            else
            {
                mpb.isE = NULL;
            }

        __CREATE("NORMAL_MAP")
            if (src->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isN = ResourceManager::TexLoad(wstr);
            }
            else
            {
                mpb.isN = NULL;
            }

        __CREATE("SPECULAR_MAP")
            if (src->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isS = ResourceManager::TexLoad(wstr);
            }
            else
            {
                mpb.isS = NULL;
            }

        __CREATE("ALPHA_MAP")
            if (src->GetTexture(aiTextureType_OPACITY, 0, &path) == AI_SUCCESS)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isESB = ResourceManager::TexLoad(wstr);
            }
            else
            {
                mpb.isESB = NULL;
            }

        __CREATE("SHININESS_MAP")
        {
            if (mpb.isESB == NULL)
            {
                auto str = string(path.C_Str());
                str = FileNormalization(&str);

                auto wstr = ctow(str.c_str());
                mpb.isESB = ResourceManager::TexLoad(wstr);
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

void MeshLoader::ParseBone(BONE_INFO& bns, const aiMesh* src)
{
    for (auto b = 0u; b < src->mNumBones; b++) {

        UINT ind = 0;
        string bname = src->mBones[b]->mName.data;

    }
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


bool LoadMesh(const wchar_t* file, vector<MESH>& mesh, vector<MATERIAL>& material)
{
    MeshLoader ml = {};

    //Flyweight
    return (ml.Load(file, mesh, material));
}

bool LoadMesh(const wchar_t* file, RModel* ptr)
{
    MeshLoader ml = {};

    return (ml.Load(file, ptr->Mesh_, ptr->Mtr_));
}