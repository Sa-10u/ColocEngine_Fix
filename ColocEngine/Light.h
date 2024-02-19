#pragma once
#include <cstdint>
#include"C_Trans.h"

typedef DirectX::XMFLOAT3 Color;

constexpr uint16_t Lights_MAX = 256;

struct Light 
{
	Light() = default;

	virtual void Reset(int index) = 0;

	virtual void SetFlag(int index, uint8_t flag) = 0;

	virtual void SetColor(int index , Color col) = 0;
	virtual void SetIntensity(int index , float inten) = 0;

	virtual void* pLight() = 0;
	virtual size_t GetSize() = 0;
};

struct P_Light : Light
{
	P_Light();
	~P_Light() = default;

	void Reset(int index) override;

	void SetFlag(int index, uint8_t flag) override;

	void SetColor(int index, Color col) override;
	void SetIntensity(int index, float inten) override;
	void SetPosition(int index, Position pos);

	void* pLight() override;
	size_t GetSize() override;

private:
	struct lgt
	{
		lgt();

		Color col;
		float inten;

		Position pos;
		int flag;
	}comp[Lights_MAX];

public:
	
	lgt GetComponent(int index);

};

struct D_Light : Light
{
	D_Light();
	~D_Light() = default;

	void SetFlag(int index, uint8_t flag) override;

	void Reset(int index) override;

	void SetColor(int index, Color col) override;
	void SetIntensity(int index, float inten) override;
	void SetPosition(int index, Position pos);
	void SetRotation(int index, Rotation rot);
	void SetThreshold(int index, float thr);

	void* pLight() override;
	size_t GetSize() override;

private:
	struct lgt
	{
		lgt();

		Color col;
		float inten;

		Position pos;
		int flag;

		Rotation rot;
		float thr;
	}comp[Lights_MAX];

public:

	lgt GetComponent(int index);

};

struct A_Light : Light
{
	A_Light();
	~A_Light() = default;

	void Reset(int index) override;

	void SetFlag(int index, uint8_t flag) override;

	void SetColor(int index, Color col) override;
	void SetIntensity(int index, float inten) override;

	void* pLight() override;
	size_t GetSize() override;

private:
	struct lgt
	{
		lgt();

		Color col;
		float inten;

		int flag;
		float val0;
		float val1;
		float val2;
	}comp[Lights_MAX];

public:

	lgt GetComponent(int index);

};