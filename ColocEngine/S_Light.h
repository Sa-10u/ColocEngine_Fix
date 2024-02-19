#pragma once
#include <cstdint>
#include"C_Trans.h"
#include"IComp.h"

typedef DirectX::XMFLOAT3 Color;

constexpr uint16_t Lights_MAX = 256;

struct Light : IComp
{
	Light();
	Light(string tag);

protected :
	uint16_t index_;
};

struct P_Light : Light
{
	P_Light();
	P_Light(string tag);
	~P_Light();

	void Reset() ;

	void SetFlag(uint8_t flag);

	void SetColor(Color col);
	void SetIntensity(float inten);
	void SetPosition(Position pos);

	static void* pLight();
	static size_t GetSize();
	static uint16_t GetCount();
	static void Flush();

private:
	static struct lgt
	{
		lgt();

		Color col;
		float inten;

		Position pos;
		int flag;
	}comp[Lights_MAX];
	static uint16_t cnt_;

	lgt datnum;

private:

	void initialize() override;
	void release() override;
	void Run(float tick) override;

};

struct D_Light : Light
{
	D_Light();
	~D_Light() = default;

	void Reset();

	void SetFlag(uint8_t flag);

	void SetColor(Color col);
	void SetIntensity(float inten);
	void SetPosition(Position pos);
	void SetRotation(Rotation rot);
	void SetThreshold(float thr);

	static void* pLight();
	static size_t GetSize();
	static uint16_t GetCount();
	static void Flush();

private:
	static struct lgt
	{
		lgt();

		Color col;
		float inten;

		Position pos;
		int flag;

		Rotation rot;
		float thr;
	}comp[Lights_MAX];
	static uint16_t cnt_;

	lgt datnum;

private:

	void initialize() override;
	void release() override;
	void Run(float tick) override;
};

struct A_Light : Light
{
	A_Light();
	~A_Light() = default;

	void Reset();

	void SetFlag(uint8_t flag);

	void SetColor(Color col);
	void SetIntensity(float inten);

	static void* pLight() ;
	static size_t GetSize() ;
	static uint16_t GetCount();
	static void Flush();

private:
	static struct lgt
	{
		lgt();

		Color col;
		float inten;

		int flag;
		float val0;
		float val1;
		float val2;
	}comp[Lights_MAX];
	static uint16_t cnt_;

	lgt datnum;

private:

	void initialize() override;
	void release() override;
	void Run(float tick) override;

};