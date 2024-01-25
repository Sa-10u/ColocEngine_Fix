#include "PoorHeader.hlsli"
#include "Functions.hlsli"
#include "Keyword.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput resÅ@= (PSoutput)0;

    res.color = colmap[Map[inp.ID].isD].Sample(colsmp, inp.uv);

 /*   for (int i = 0u; i < MAX_LIGHT; i++) {

        res.color.r = pl[i].inten;
    }

    for (int i = 0u; i < MAX_LIGHT; i++) {

        res.color.r = al[i].inten;
    }

    for (int i = 0u; i < MAX_LIGHT; i++) {

        res.color.r = dl[i].inten;
    }*/

    return res;
}