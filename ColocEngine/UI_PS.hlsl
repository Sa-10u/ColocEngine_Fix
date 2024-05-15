#include"BaseUI.hlsli"

PSoutput main(VSoutput inp)
{
	PSoutput res = (PSoutput)(0);

    res.color = colmap[Map[inp.ID].isD].Sample(colsmp, inp.uv);
    res.depth = inp.pos;
    res.emission = 0;
    res.normal = 0;
    res.position = inp.pos;
    res.t0 = 0;
    res.t1 = 0;
    res.t2 = 0;
	
	return res;
}