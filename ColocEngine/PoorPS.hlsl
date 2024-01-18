#include "PoorHeader.hlsli"
#include "Functions.hlsli"
#include "Keyword.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput res@= (PSoutput)0;

    res.color = colmap[0].Sample(colsmp, inp.uv);
    res.color.a = 1;

    return res;
}