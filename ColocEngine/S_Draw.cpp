#include "S_Draw.h"
#include"C_Trans.h"
#include<cassert>
#include"BUFFER.h"

//fix draw count , over instance's count then run GPU 
namespace S_Draw
{
	void ParseBoneInfo
	(	vector<BONE_INFO>& bns, vector<AnimationData_BONE>* data0,
		vector<AnimationData_BONE>* data1, int32_t prog0, int32_t prog1,float linear,
		vector<Mat>& mat0, vector<Mat>& mat1, vector<float>linears
	);

	void ParseFrameData(vector<AnimationData_BONE>& data, int32_t prog, vector<Mat>& info);
}

void S_Draw::Draw(XMMATRIX* wld, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;

	ObjInfo i;
	i.wld = *wld;

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(C_Trans* trans, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;

	ObjInfo i;
	i.wld = trans->WLDGetMTX();

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(XMMATRIX mat, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;

	ObjInfo i;
	i.wld = mat;

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(ObjInfo* info, uint16_t md, MapBOOL** mb, uint16_t size)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;
	ResourceManager::GetPointer_Mdl()[md].info.push_back(*info);
	setTex(md, mb, size);
}

void S_Draw::Draw(XMMATRIX* wld, uint16_t md, MapBOOL** mb, uint16_t size, AnimData& ad)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;

	ObjInfo i;
	i.wld = *wld;

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(C_Trans* trans, uint16_t md, MapBOOL** mb, uint16_t size, AnimData& ad)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;

	ObjInfo i;
	i.wld = trans->WLDGetMTX();

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(XMMATRIX mat, uint16_t md, MapBOOL** mb, uint16_t size, AnimData& ad)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;

	ObjInfo i;
	i.wld = mat;

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(ObjInfo* info, uint16_t md, MapBOOL** mb, uint16_t size, AnimData& ad)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_++;
	ResourceManager::GetPointer_Mdl()[md].info.push_back(*info);
	setTex(md, mb, size);
}

void S_Draw::setTex(uint16_t md ,MapBOOL** arr, uint16_t size)
{
	auto ind = ResourceManager::GetPointer_Mdl()[md].info.size() - 1;
	auto sz = min(size, ResourceManager::GetPointer_Mdl()[md].Mesh_.size());


	for (auto i = 0u; i < sz; i++) {
		
		MapBOOL temp = {};
		temp = arr[i] == nullptr ? ResourceManager::GetPointer_Mdl()[md].Mesh_[i].defTex_ : *arr[i];

		ResourceManager::GetPointer_Mdl()[md].Mesh_[i].texIndex_.push_back(temp);
	}

	for (auto i = sz; i < ResourceManager::GetPointer_Mdl()[md].Mesh_.size(); i++) {

		ResourceManager::GetPointer_Mdl()[md].Mesh_[i].texIndex_.push_back(ResourceManager::GetPointer_Mdl()[md].Mesh_[i].defTex_);
	}
}

void S_Draw::Flush(uint16_t md)
{
	ResourceManager::GetPointer_Mdl()[md].DrawCount_ = NULL;
	ResourceManager::GetPointer_Mdl()[md].info.clear();
	for (auto i = 0u; i < ResourceManager::GetPointer_Mdl()[md].Mesh_.size(); i++) {
		ResourceManager::GetPointer_Mdl()[md].Mesh_[i].texIndex_.clear();
	}
}

void S_Draw::ParseBoneInfo
(
	vector<BONE_INFO>& bns, vector<AnimationData_BONE>* data0,
	vector<AnimationData_BONE>* data1, int32_t prog0, int32_t prog1, float linear,
	vector<Mat>& mat0, vector<Mat>& mat1, vector<float>linears
){
	auto&& BoneNum = bns.size();

	auto arr_parents = static_cast<int16_t*>(malloc(sizeof(int16_t) * BoneNum));
	auto arr_mat = static_cast<Mat*>(malloc(sizeof(Mat) * BoneNum));

	for (auto i = 0u; i < BoneNum; ++i) {

		arr_parents[i] = bns[i].parent_; arr_mat[i] = bns[i].pose_;
	}
	free(arr_parents), free(arr_mat);

}

void S_Draw::ParseFrameData(vector<AnimationData_BONE>& data, int32_t prog, vector<Mat>& info)
{
	bool isOK = false;

	C_Trans trans = C_Trans("");
	uint8_t boneindex = 0;

	for (auto& bone : data) {//data is some bones!

		float3 pos = {};
		float3 rot = {};
		float3 size = {};

		for (auto i = 0u; i < bone.Ptime_.size() - 1;) {

			if (!(bone.Ptime_[i] <= prog <= bone.Ptime_[i + i]))	continue;

			float per = GetGradation(bone.Ptime_[i], bone.Ptime_[i + 1], prog);
			
			pos = (bone.pos_[i] * per) + (bone.pos_[i + 1] * (1 - per));

			break;
		}

		for (auto i = 0u; i < bone.Rtime_.size() - 1;) {

			if (!(bone.Rtime_[i] <= prog <= bone.Rtime_[i + i]))	continue;
			
			float per = GetGradation(bone.Rtime_[i], bone.Rtime_[i + 1], prog);

			pos = (bone.rot_[i] * per) + (bone.rot_[i + 1] * (1 - per));


			break;
		}

		for (auto i = 0u; i < bone.Stime_.size() - 1;) {

			if (!(bone.Stime_[i] <= prog <= bone.Stime_[i + i]))	continue;

			float per = GetGradation(bone.Stime_[i], bone.Stime_[i + 1], prog);

			pos = (bone.scale_[i] * per) + (bone.scale_[i + 1] * (1 - per));


			break;
		}

		trans.pos = pos;
		trans.rot = rot;
		trans.scale = size;

		info[boneindex] = trans.LCLGetMTX();
		++boneindex;
	}
}