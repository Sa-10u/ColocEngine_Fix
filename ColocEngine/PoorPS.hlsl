#include "PoorHeader.hlsli"
#include "Functions.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput resÅ@= (PSoutput)0;

    float3 Lpos = float3(1, 0, 0);
    float3 Lvec = normalize(Lpos);

    float3 Ray = normalize(tgt - pos);
    float intens = 100;

    float p = 4;
    float rvec = dot(inp.norm, Lvec) * inp.norm;
    float rct =  2 *inp.norm* rvec - Lvec;
    rct = saturate(rct);
    rct = pow(rct, Mtl[inp.MID].shin) * intens;
    rct = saturate(rct);

    float near = 1/pow((distance(inp.Wpos , Lpos )),2);
    near = near;
   
    float ct = saturate(dot(Lvec, inp.norm));
    ct = ct* intens* (near);
    ct = saturate(ct * 0.01);
    
    res.color = float4(Mtl[inp.MID].dif * ct + Mtl[inp.MID].spec * rct, Mtl[inp.MID].alp);

    res.color.a = 1;
    float grad = 15;

    res.color.rgb = floor(res.color.rgb *grad) / grad ;
    //res.color.rgb = floor(inp.norm * 5);

    float ol = dot(normalize(inp.norm), Ray);
    ol = abs(ol);

    float s = 4.5;
    float tra = floor(ol * 9) /9;
    ol = floor(ol *s);

    ol = saturate(1-ol);
    tra= 1 - tra;
    tra = saturate(tra);

    res.color = 1;
    res.color.gb = res.color.gb - ol;
    res.color.a = tra + ol ;

    return res;
}