#include"BaseUI.h"

VSoutput main(VSinput inp)
{
	VSoutput res = (VSoutput)0;
	res.pos = Data[inp.ID].pos[inp.VID];

	res.uv = inp.uv;
	res.ID = inp.ID;
	res.VID = inp.VID;

	return res;
}