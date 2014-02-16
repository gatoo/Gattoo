class ILogger
{
public:

	enum ECategory
	{
		enInf,
		enWrn,
		enErr
	};

	virtual void LogMsg(ECategory cat, char const * lpszFormat, ...) = 0;
};