#pragma once
#include "IComp.h"
#include "MACRO.h"
#include"ResourceManager.h"

class C_Trans;

namespace S_Draw
{
	void Draw(XMMATRIX* wld, UINT md);
	void Draw(C_Trans* trans, UINT md);
	void Draw(XMMATRIX mat, UINT md);

	void Draw(ObjInfo *info, UINT md);

	void Flush(UINT md);
}
