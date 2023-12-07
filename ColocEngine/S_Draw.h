#pragma once
#include "IComp.h"
#include "MACRO.h"
#include"ResourceManager.h"

class C_Trans;

namespace S_Draw
{
	void Draw(XMMATRIX* wld, RModel* md);
	void Draw(C_Trans* trans, RModel* md);
	void Draw(XMMATRIX mat, RModel* md);

	void Draw(ObjInfo *info, RModel* md);

	void Flush(RModel* md);
}
