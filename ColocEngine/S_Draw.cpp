#include "S_Draw.h"

void S_Draw::Draw(XMMATRIX* wld, RModel* md)
{
	md->DrawCount_++;
	md->mat.push_back(*wld);
}

void S_Draw::Flush(RModel* md)
{
	md->DrawCount_ = NULL;
	md->mat.clear();
}
