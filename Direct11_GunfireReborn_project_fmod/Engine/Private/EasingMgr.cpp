#include "..\Public\EasingMgr.h"

IMPLEMENT_SINGLETON(CEasingMgr)


CEasingMgr::CEasingMgr()
{
}

_vector CEasingMgr::Easing(_uint eEasingType, _vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	if (eEasingType >= TYPE_End)
		return _vector();

	switch (eEasingType)
	{
	case Engine::CEasingMgr::TYPE_Linear:
		return Linear(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_QuadIn:
		return QuadIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_QuadOut:
		return  QuadOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_QuadInOut:
		return  QuadInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_CubicIn:
		return  CubicIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_CubicOut:
		return  CubicOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_CubicInOut:
		return  CubicInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_QuarticIn:
		return  QuarticIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_QuarticOut:
		return  QuarticOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_QuarticInOut:
		return QuarticInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_QuinticIn:
		return  QuinticIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_QuinticOut:
		return QuinticOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_QuinticInOut:
		return QuinticInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_SinIn:
		return SinIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_SinOut:
		return  sinfOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_SinInOut:
		return sinfInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_ExpoIn:
		return  ExpoIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_ExpoOut:
		return  ExpoOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_ExpoInOut:
		return  ExpoInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_CircularIn:
		return  CircularIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_CircularOut:
		return  CircularOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_CircularInOut:
		return  CircularInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_ElasticIn:
		return  ElasticEaseIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_ElasticOut:
		return  ElasticEaseOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_ElasticInOut:
		return  ElasticEaseInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_BounceIn:
		return  BounceEaseIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	case Engine::CEasingMgr::TYPE_BounceOut:
		return  BounceEaseOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);

	default:
		break;
	}

	return _vector();
}


_vector CEasingMgr::Linear(_vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	return _vector((vTargetPoint - vStartPoint) * dPassedTime / dTotalTime + vStartPoint);
}

_vector CEasingMgr::QuadIn(_vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	dPassedTime /= dTotalTime;
	return _vector((vTargetPoint - vStartPoint) * dPassedTime*dPassedTime + vStartPoint);
}

_vector CEasingMgr::QuadOut(_vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	dPassedTime /= dTotalTime;
	return _vector(-(vTargetPoint - vStartPoint) * dPassedTime*(dPassedTime - 2.0) + vStartPoint);
}

_vector CEasingMgr::QuadInOut(_vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	dPassedTime /= dTotalTime / 2.0;
	if (dPassedTime < 1.0) return _vector((vTargetPoint - vStartPoint) / 2.0 * dPassedTime*dPassedTime + vStartPoint);
	dPassedTime--;
	return _vector(-(vTargetPoint - vStartPoint) / 2.0 * (dPassedTime*(dPassedTime - 2.0) - 1.0) + vStartPoint);
}

_vector CEasingMgr::CubicIn(_vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	dPassedTime /= dTotalTime;
	return _vector((vTargetPoint - vStartPoint) * dPassedTime*dPassedTime*dPassedTime + vStartPoint);
}

_vector CEasingMgr::CubicOut(_vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	dPassedTime /= dTotalTime;
	dPassedTime--;
	return _vector((vTargetPoint - vStartPoint) * (dPassedTime*dPassedTime*dPassedTime + 1.0) + vStartPoint);
}

_vector CEasingMgr::CubicInOut(_vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	dPassedTime /= dTotalTime / 2.f;
	if (dPassedTime < 1.f) return (vTargetPoint - vStartPoint) / 2.f * dPassedTime*dPassedTime*dPassedTime + vStartPoint;
	dPassedTime -= 2.f;
	return _vector((vTargetPoint - vStartPoint) / 2.f * (dPassedTime*dPassedTime*dPassedTime + 2.f) + vStartPoint);
}

_vector CEasingMgr::QuarticIn(_vector vStartPoint, _vector vTargetPoint, _double dPassedTime, _double dTotalTime)
{
	dPassedTime /= dTotalTime;
	return _vector((vTargetPoint - vStartPoint) * dPassedTime*dPassedTime*dPassedTime*dPassedTime + vStartPoint);
}

_vector CEasingMgr::QuarticOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector(-(fTargetPoint - fStartPoint) * (fPassedTime*fPassedTime*fPassedTime*fPassedTime - 1) + fStartPoint);
}

_vector CEasingMgr::QuarticInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _vector((fTargetPoint - fStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime*fPassedTime + fStartPoint);
	fPassedTime -= 2;
	return _vector(-(fTargetPoint - fStartPoint) / 2 * (fPassedTime*fPassedTime*fPassedTime*fPassedTime - 2) + fStartPoint);
}

_vector CEasingMgr::QuinticIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector((fTargetPoint - fStartPoint) * fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + fStartPoint);
}

_vector CEasingMgr::QuinticOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector((fTargetPoint - fStartPoint) * (fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + 1) + fStartPoint);
}

_vector CEasingMgr::QuinticInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _vector((fTargetPoint - fStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + fStartPoint);
	fPassedTime -= 2;
	return _vector((fTargetPoint - fStartPoint) / 2 * (fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + 2) + fStartPoint);
}

_vector CEasingMgr::SinIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector(-(fTargetPoint - fStartPoint) * cosf(fPassedTime / fTotalTime * (PI / 2)) + (fTargetPoint - fStartPoint) + fStartPoint);
}

_vector CEasingMgr::sinfOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector((fTargetPoint - fStartPoint) * sinf(fPassedTime / fTotalTime * (PI / 2)) + fStartPoint);
}

_vector CEasingMgr::sinfInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector(-(fTargetPoint - fStartPoint) / 2 * (cosf(PI*fPassedTime / fTotalTime) - 1) + fStartPoint);
}

_vector CEasingMgr::ExpoIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector((fTargetPoint - fStartPoint) * pow(2, 10.0f * (fPassedTime / fTotalTime - 1)) + fStartPoint);
}

_vector CEasingMgr::ExpoOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector((fTargetPoint - fStartPoint) * (-pow(2, -10 * fPassedTime / fTotalTime) + 1) + fStartPoint);
}

_vector CEasingMgr::ExpoInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _vector((fTargetPoint - fStartPoint) / 2 * pow(2, 10 * (fPassedTime - 1)) + fStartPoint);
	fPassedTime--;
	return _vector((fTargetPoint - fStartPoint) / 2 * (-pow(2, -10 * fPassedTime) + 2) + fStartPoint);
}

_vector CEasingMgr::CircularIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _vector(-(fTargetPoint - fStartPoint) * (sqrtf(1 - fPassedTime * fPassedTime) - 1) + fStartPoint);
}

_vector CEasingMgr::CircularOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _vector((fTargetPoint - fStartPoint) * sqrtf(1 - fPassedTime * fPassedTime) + fStartPoint);
}

_vector CEasingMgr::CircularInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _vector(-(fTargetPoint - fStartPoint) / 2 * (sqrtf(1 - fPassedTime * fPassedTime) - 1) + fStartPoint);
	fPassedTime -= 2;
	return _vector((fTargetPoint - fStartPoint) / 2 * (sqrtf(1 - fPassedTime * fPassedTime) + 1) + fStartPoint);
}

_vector CEasingMgr::ElasticEaseOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{

	if ((fPassedTime /= fTotalTime) == 1)
		return fStartPoint + (fTargetPoint - fStartPoint);

	_double p = fTotalTime * .3f;
	_double s = p / 4;

	return _vector((fTargetPoint - fStartPoint) * pow(2, -10 * fPassedTime) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p) + (fTargetPoint - fStartPoint) + fStartPoint);
}

_vector CEasingMgr::ElasticEaseIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	if ((fPassedTime /= fTotalTime) == 1)
		return fStartPoint + (fTargetPoint - fStartPoint);

	_double p = fTotalTime * .3f;
	_double s = p / 4.f;

	return _vector(-((fTargetPoint - fStartPoint) * pow(2, 10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p)) + fStartPoint);
}

_vector CEasingMgr::ElasticEaseInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	if ((fPassedTime /= fTotalTime / 2) == 2)
		return _vector(fStartPoint + (fTargetPoint - fStartPoint));

	_double p = fTotalTime * (.3f * 1.5f);
	_double s = p / 4;

	if (fPassedTime < 1)
		return _vector(-.5f * ((fTargetPoint - fStartPoint) * pow(2, 10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p)) + fStartPoint);
	return _vector((fTargetPoint - fStartPoint) * pow(2, -10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p) * .5f + (fTargetPoint - fStartPoint) + fStartPoint);
}

_vector CEasingMgr::BounceEaseOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	if ((fPassedTime /= fTotalTime) < (1 / 2.75f))
		return _vector((fTargetPoint - fStartPoint) * (7.5625f * fPassedTime * fPassedTime) + fStartPoint);
	else if (fPassedTime < (2 / 2.75f))
		return _vector((fTargetPoint - fStartPoint) * (7.5625f * (fPassedTime -= (1.5f / 2.75f)) * fPassedTime + .75f) + fStartPoint);
	else if (fPassedTime < (2.5f / 2.75f))
		return _vector((fTargetPoint - fStartPoint) * (7.5625f * (fPassedTime -= (2.25f / 2.75f)) * fPassedTime + .9375f) + fStartPoint);
	else
		return _vector((fTargetPoint - fStartPoint) * (7.5625f * (fPassedTime -= (2.625f / 2.75f)) * fPassedTime + .984375f) + fStartPoint);
}


_vector CEasingMgr::BounceEaseIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
{
	return _vector(BounceEaseOut(fTargetPoint, fStartPoint, fTotalTime - fPassedTime, fTotalTime));
}
//_float CEasingMgr::BounceEaseInOut(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
//{
//	if (dPassedTime < dTotalTime * 0.5f)
//		return BounceEaseIn(vStartPoint, vTargetPoint * 0.5f, dPassedTime , dTotalTime * 0.5f);
//	else
//		return BounceEaseOut(vTargetPoint * 0.5f, vTargetPoint , dPassedTime-(dTotalTime * 0.5f) , dTotalTime * 0.5f);
//}
//
//_float CEasingMgr::BounceEaseOutIn(_vector fStartPoint, _vector fTargetPoint, _double fPassedTime, _double fTotalTime)
//{
//
//	if (dPassedTime < dTotalTime * 0.5f )
//		return BounceEaseOut(vStartPoint, vTargetPoint * 0.5f, dPassedTime * 0.5f, dTotalTime * 0.5f);
//	return BounceEaseIn(vTargetPoint * 0.5f, vTargetPoint, dPassedTime * 0.5f, dTotalTime * 0.5f);
//}

void CEasingMgr::Free()
{
}
