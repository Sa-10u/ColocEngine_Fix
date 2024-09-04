#pragma once
#include "IComp.h"
#include "MACRO.h"
#include"ResourceManager.h"

class C_Trans;

namespace S_Draw
{
	void Draw(XMMATRIX* wld, uint16_t md, MapBOOL** arr, uint16_t size);
	void Draw(C_Trans* trans, uint16_t md, MapBOOL** arr, uint16_t size);
	void Draw(XMMATRIX mat, uint16_t md, MapBOOL** arr, uint16_t size);
	void Draw(ObjInfo *info, uint16_t md, MapBOOL** arr, uint16_t size);

	void Draw(XMMATRIX* wld, uint16_t md, MapBOOL** arr, uint16_t size,vector<XMMATRIX> mats);
	void Draw(C_Trans* trans, uint16_t md, MapBOOL** arr, uint16_t size, vector<XMMATRIX> mats);
	void Draw(XMMATRIX mat, uint16_t md, MapBOOL** arr, uint16_t size, vector<XMMATRIX> mats);
	void Draw(ObjInfo* info, uint16_t md, MapBOOL** arr, uint16_t size, vector<XMMATRIX> mats);

	inline void setTex(uint16_t md, MapBOOL** arr, uint16_t size);

	void Flush(uint16_t md);
}
