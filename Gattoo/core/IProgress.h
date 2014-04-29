#pragma once

class IProcessor
{
public:

	virtual bool stop() = 0;
	virtual bool start() = 0;
};

class IProgress
{
public:

	virtual bool doStep() = 0;
	virtual bool init(int iMax, int iStep) = 0;
};