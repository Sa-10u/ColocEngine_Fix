#include "S_Draw.h"
#include"C_Trans.h"
#include<cassert>
#include"BUFFER.h"

//fix draw count , over instance's count then run GPU (255)
namespace S_Draw
{
	void ParseBoneInfo
	(	vector<BONE_INFO>& bns, vector<AnimationData_BONE>* data0,
		vector<AnimationData_BONE>* data1, int32_t prog0, int32_t prog1,float linear,
		vector<Mat>& mat0, vector<Mat>& mat1, vector<float>linears
	);

	void ParseFrameData(Mat* pose,vector<AnimationData_BONE>& data, int32_t prog, vector<Mat>& dst);
}

void S_Draw::Draw(XMMATRIX* wld, uint16_t md, MapBOOL** mb, uint16_t size)
{
	++ResourceManager::GetPointer_Mdl()[md].DrawCount_;

	ObjInfo i;
	i.wld = *wld;

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(C_Trans* trans, uint16_t md, MapBOOL** mb, uint16_t size)
{
	++ResourceManager::GetPointer_Mdl()[md].DrawCount_;

	ObjInfo i;
	i.wld = trans->WLDGetMTX();

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(XMMATRIX mat, uint16_t md, MapBOOL** mb, uint16_t size)
{
	++ResourceManager::GetPointer_Mdl()[md].DrawCount_;

	ObjInfo i;
	i.wld = mat;

	ResourceManager::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);
}

void S_Draw::Draw(ObjInfo* info, uint16_t md, MapBOOL** mb, uint16_t size)
{
	++ResourceManager::GetPointer_Mdl()[md].DrawCount_;

	ResourceManager::GetPointer_Mdl()[md].info.push_back(*info);
	setTex(md, mb, size);
}

void S_Draw::Draw(XMMATRIX* wld, uint16_t md, MapBOOL** mb, uint16_t size, AnimData& ad)
{ 
	namespace R = ResourceManager;

	++R::GetPointer_Mdl()[md].DrawCount_;

	ObjInfo i;
	i.wld = *wld;

	R::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);

	auto& amt = R::GetPointer_Mdl()[md].armature_[ad.armatureIndex];

	auto anim0 = amt.AnimnameIndex_.contains(ad.AnimName0) ? &amt.anims_[(amt.AnimnameIndex_.at(ad.AnimName0))] : nullptr;
	auto anim1 = amt.AnimnameIndex_.contains(ad.AnimName1) ? &amt.anims_[(amt.AnimnameIndex_.at(ad.AnimName1))] : nullptr;

	amt.mat0_for_tex.resize(R::GetPointer_Mdl()[md].DrawCount_);
	amt.mat1_for_tex.resize(R::GetPointer_Mdl()[md].DrawCount_);

	auto& matTex0 = amt.mat0_for_tex[R::GetPointer_Mdl()[md].DrawCount_ - 1];
	auto& matTex1 = amt.mat1_for_tex[R::GetPointer_Mdl()[md].DrawCount_ - 1];

	amt.linear_tex.resize(R::GetPointer_Mdl()[md].DrawCount_);

	ParseBoneInfo(amt.bnsinfo_, anim0, anim1, ad.prog0, ad.prog1, ad.linear, matTex0, matTex1, amt.linear_tex);
}

void S_Draw::Draw(C_Trans* trans, uint16_t md, MapBOOL** mb, uint16_t size, AnimData& ad)
{
	namespace R = ResourceManager;
	++R::GetPointer_Mdl()[md].DrawCount_;

	ObjInfo i;
	i.wld = trans->WLDGetMTX();

	R::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);

	auto& amt = R::GetPointer_Mdl()[md].armature_[ad.armatureIndex];

	auto anim0 = amt.AnimnameIndex_.contains(ad.AnimName0) ? &amt.anims_[(amt.AnimnameIndex_.at(ad.AnimName0))] : nullptr;
	auto anim1 = amt.AnimnameIndex_.contains(ad.AnimName1) ? &amt.anims_[(amt.AnimnameIndex_.at(ad.AnimName1))] : nullptr;

	amt.mat0_for_tex.resize(R::GetPointer_Mdl()[md].DrawCount_);
	amt.mat1_for_tex.resize(R::GetPointer_Mdl()[md].DrawCount_);

	auto& matTex0 = amt.mat0_for_tex[R::GetPointer_Mdl()[md].DrawCount_ - 1];
	auto& matTex1 = amt.mat1_for_tex[R::GetPointer_Mdl()[md].DrawCount_ - 1];

	amt.linear_tex.resize(R::GetPointer_Mdl()[md].DrawCount_);

	ParseBoneInfo(amt.bnsinfo_, anim0, anim1, ad.prog0, ad.prog1, ad.linear, matTex0, matTex1, amt.linear_tex);
}

void S_Draw::Draw(XMMATRIX mat, uint16_t md, MapBOOL** mb, uint16_t size, AnimData& ad)
{
	namespace R = ResourceManager;
	++R::GetPointer_Mdl()[md].DrawCount_;

	ObjInfo i;
	i.wld = mat;

	R::GetPointer_Mdl()[md].info.push_back(i);
	setTex(md, mb, size);

	auto& amt = R::GetPointer_Mdl()[md].armature_[ad.armatureIndex];

	auto anim0 = amt.AnimnameIndex_.contains(ad.AnimName0) ? &amt.anims_[(amt.AnimnameIndex_.at(ad.AnimName0))] : nullptr;
	auto anim1 = amt.AnimnameIndex_.contains(ad.AnimName1) ? &amt.anims_[(amt.AnimnameIndex_.at(ad.AnimName1))] : nullptr;

	amt.mat0_for_tex.resize(R::GetPointer_Mdl()[md].DrawCount_);
	amt.mat1_for_tex.resize(R::GetPointer_Mdl()[md].DrawCount_);

	auto& matTex0 = amt.mat0_for_tex[R::GetPointer_Mdl()[md].DrawCount_ - 1];
	auto& matTex1 = amt.mat1_for_tex[R::GetPointer_Mdl()[md].DrawCount_ - 1];

	amt.linear_tex.resize(R::GetPointer_Mdl()[md].DrawCount_);

	ParseBoneInfo(amt.bnsinfo_, anim0, anim1, ad.prog0, ad.prog1, ad.linear, matTex0, matTex1, amt.linear_tex);
}

void S_Draw::Draw(ObjInfo* info, uint16_t md, MapBOOL** mb, uint16_t size, AnimData& ad)
{
	namespace R = ResourceManager;

	auto& DrawCount = R::GetPointer_Mdl()[md].DrawCount_;
	++DrawCount;
	auto ArrayTail = DrawCount - 1;

	R::GetPointer_Mdl()[md].info.push_back(*info);
	setTex(md, mb, size);

	auto& amt = R::GetPointer_Mdl()[md].armature_[ad.armatureIndex];

	auto anim0 = amt.AnimnameIndex_.contains(ad.AnimName0) ? &amt.anims_[(amt.AnimnameIndex_.at(ad.AnimName0))] : nullptr;
	auto anim1 = amt.AnimnameIndex_.contains(ad.AnimName1) ? &amt.anims_[(amt.AnimnameIndex_.at(ad.AnimName1))] : nullptr;

	amt.mat0_for_tex.resize(DrawCount);
	amt.mat1_for_tex.resize(DrawCount);

	auto& matTex0 = amt.mat0_for_tex[ArrayTail];
	auto& matTex1 = amt.mat1_for_tex[ArrayTail];

	amt.linear_tex.resize(DrawCount);

	ParseBoneInfo(amt.bnsinfo_, anim0, anim1, ad.prog0, ad.prog1, ad.linear, matTex0, matTex1, amt.linear_tex);

	R::GetPointer_Mdl()[md].mats0_.resize(DrawCount);
	R::GetPointer_Mdl()[md].mats1_.resize(DrawCount);

	R::GetPointer_Mdl()[md].mats0_[ArrayTail] = matTex0;
	R::GetPointer_Mdl()[md].mats1_[ArrayTail] = matTex1;
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
	ResourceManager::GetPointer_Mdl()[md].DrawCount_ = 0;
	ResourceManager::GetPointer_Mdl()[md].info.clear();
	ResourceManager::GetPointer_Mdl()[md].armature_.clear();

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
	if (data0 != nullptr && !(linear >= 1.0f)) { mat0.resize(BoneNum); ParseFrameData(arr_mat, *data0, prog0, mat0); }
	if (data1 != nullptr && !(0.0f >= linear)) { mat1.resize(BoneNum); ParseFrameData(arr_mat, *data1, prog1, mat1); }

	free(arr_parents), free(arr_mat);
}

void S_Draw::ParseFrameData(Mat* pose, vector<AnimationData_BONE>& data, int32_t prog, vector<Mat>& dst)
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

		dst[boneindex] = pose[boneindex] * trans.LCLGetMTX();
		++boneindex;
	}
}