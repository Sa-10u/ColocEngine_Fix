#include "S_Draw.h"
#include"C_Trans.h"

void S_Draw::Draw(XMMATRIX* wld, RModel* md)
{
	md->DrawCount_++;

	ObjInfo i;
	i.wld = *wld;

	md->info.push_back(i);
}

void S_Draw::Draw(C_Trans* trans, RModel* md)
{
	md->DrawCount_++;

	ObjInfo i;
	i.wld = trans->WLDGetMTX();

	md->info.push_back(i);
}

void S_Draw::Draw(XMMATRIX mat, RModel* md)
{
	md->DrawCount_++;

	ObjInfo i;
	i.wld = mat;

	md->info.push_back(i);
}

void S_Draw::Draw(ObjInfo* info, RModel* md)
{
	md->DrawCount_++;
	md->info.push_back(*info);
}

void S_Draw::Flush(RModel* md)
{
	md->DrawCount_ = NULL;
	md->info.clear();
}
