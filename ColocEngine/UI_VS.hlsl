#include"BaseUI.h"

VSoutput main(VSinput inp)
{
	VSoutput res = (VSoutput)(0);
	res.pos = float4(Data[inp.ID].pos[inp.VID],1);

	res.uv = inp.uv;
	res.ID = inp.ID;
	res.VID = inp.VID;

	return res;
}