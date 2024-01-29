#pragma once
#include <cstdint>
#include"C_Trans.h"

typedef DirectX::XMFLOAT3 Color;

constexpr uint16_t Lights_MAX = 256;

struct Light 
{
	Light() = default;
	virtual ~Light() = 0;

	virtual bool isON(int index) = 0;
	virtual bool isDisposal(int index) = 0;

	virtual void ToOFF(int index) = 0;
	virtual void ToON(int index) = 0;
	virtual void ToDisposal(int index) = 0;
	virtual void ToStore(int index) = 0;

	virtual void Reset(int index) = 0;

	virtual void SetFlag(int index, uint8_t flag) = 0;

	virtual void SetColor(int index , Color col) = 0;
	virtual void SetIntensity(int index , float inten) = 0;

	virtual void* pLight() = 0;
	virtual void* GetSize() = 0;
};

struct P_Light : Light
{
	P_Light();
	~P_Light() override;

	bool isON(int index) override;
	bool isDisposal(int index) override;

	void ToOFF(int index) override;
	void ToON(int index) override;
	void ToDisposal(int index) override;
	void ToStore(int index) override;

	void Reset(int index) override;

	void SetFlag(int index, uint8_t flag) override;

	void SetColor(int index, Color col) override;
	void SetIntensity(int index, float inten) override;
	void SetPosition(int index, Position pos);

	void* pLight() override;
	void* GetSize() override;

private:
	struct lgt
	{
		Color col;
		float inten;

		Position pos;
		uint8_t flag;
	}comp[Lights_MAX];

public:
	
	lgt GetComponent();

};

struct D_Light : Light
{
	D_Light();
	~D_Light() override;

	bool isON(int index) override;
	bool isDisposal(int index) override;

	void ToOFF(int index) override;
	void ToON(int index) override;
	void ToDisposal(int index) override;
	void ToStore(int index) override;

	void SetFlag(int index, uint8_t flag) override;

	void Reset(int index) override;

	void SetColor(int index, Color col) override;
	void SetIntensity(int index, float inten) override;
	void SetPosition(int index, Position pos);
	void SetRotation(int index, Rotation rot);
	void SetThreshold(int index, float thr);

	void* pLight() override;
	void* GetSize() override;

private:
	struct lgt
	{
		Color col;
		float inten;

		Position pos;
		uint8_t flag;

		Rotation rot;
		float thr;
	}comp[Lights_MAX];
};

struct A_Light : Light
{
	A_Light();
	~A_Light() override;

	bool isON(int index) override;
	bool isDisposal(int index) override;

	void ToOFF(int index) override;
	void ToON(int index) override;
	void ToDisposal(int index) override;
	void ToStore(int index) override;

	void Reset(int index) override;

	void SetFlag(int index, uint8_t flag) override;

	void SetColor(int index, Color col) override;
	void SetIntensity(int index, float inten) override;

	void* pLight() override;
	void* GetSize() override;

private:
	struct lgt
	{
		Color col;
		float inten;

		uint8_t flag;
		float val0;
		float val1;
		float val2;
	}comp[Lights_MAX];
};