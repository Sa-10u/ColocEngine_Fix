#include"BaseUI.hlsli"

PSoutput main(VSoutput inp)
{
	PSoutput res = (PSoutput)(0);

    res.col = colmap[Map[inp.ID].isD].Sample(colsmp, inp.uv);
	
	return res;
}