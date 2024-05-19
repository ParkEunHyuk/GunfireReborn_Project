#pragma once
#include "Base.h"

BEGIN(Engine)

class CEasingMgr :public CBase
{
private:
	enum EasingTypeID
	{
		TYPE_Linear = 0,

		TYPE_QuadIn = 1,
		TYPE_QuadOut,
		TYPE_QuadInOut,

		TYPE_CubicIn = 4,
		TYPE_CubicOut,
		TYPE_CubicInOut,

		TYPE_QuarticIn = 7,
		TYPE_QuarticOut,
		TYPE_QuarticInOut,

		TYPE_QuinticIn = 10,
		TYPE_QuinticOut,
		TYPE_QuinticInOut,

		TYPE_SinIn = 13,
		TYPE_SinOut,
		TYPE_SinInOut,

		TYPE_ExpoIn = 16,
		TYPE_ExpoOut,
		TYPE_ExpoInOut,

		TYPE_CircularIn = 19,
		TYPE_CircularOut,
		TYPE_CircularInOut,

		TYPE_ElasticIn = 22,
		TYPE_ElasticOut,
		TYPE_ElasticInOut,

		TYPE_BounceIn = 25,
		TYPE_BounceOut,


		TYPE_End
	};
	DECLARE_SINGLETON(CEasingMgr);
private:
	explicit CEasingMgr();
	virtual ~CEasingMgr() = default;

public:
	_vector	Easing(_uint eEasingType, _vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime);


private:
	_vector Linear(_vector vStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);

	_vector QuadIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector QuadOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector QuadInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);

	_vector CubicIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector CubicOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector CubicInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);

	_vector QuarticIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector QuarticOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector QuarticInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);

	_vector QuinticIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector QuinticOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector QuinticInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);

	_vector SinIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector sinfOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector sinfInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);

	_vector ExpoIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector ExpoOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector ExpoInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);

	_vector CircularIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector CircularOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector CircularInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);

	_vector ElasticEaseIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector ElasticEaseOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector ElasticEaseInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);

	_vector BounceEaseIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);
	_vector BounceEaseOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime);



public:
	virtual void Free() override;

};

END