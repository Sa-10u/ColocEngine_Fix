#include "PoorHeader.hlsli"
#include "Functions.hlsli"
#include "Keyword.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput resÅ@= (PSoutput)0;

    res.color = colmap[Map[inp.ID].isD].Sample(colsmp, inp.uv);

    int sz = 5;

   for (int i = 0u; i < sz; i++) {

        res.color.r += pl[i].inten;
    }

    for (int i = 0u; i < sz; i++) {

        res.color.g += al[i].inten;
    }

    for (int i = 0u; i < sz; i++) {

        res.color.b += dl[i].inten;
    }

    res.color.rgb = pl[0].inten;

    return res;
}