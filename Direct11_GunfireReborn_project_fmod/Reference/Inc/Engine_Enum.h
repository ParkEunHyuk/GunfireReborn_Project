#pragma once

namespace Engine {
	enum CHANNELID
	{
		CHANNEL_PLAYER, CHANNEL_OBJECT, CHANNEL_EFFECT, CHANNEL_UI, CHANNEL_BGM, CHANNEL_MAXCHANNEL };
}

namespace Engine {
	enum OBSEVERGROUP { BULLET, BULLET_MOSTER, MONSTER, PLAYER, OBSEVER_END };
}

namespace Engine {
	enum COLLIDERGROUP { SPHERE, AABB, OBB, SHIELD, WEAKNESS, EVENT, STATIC, COLLIDER_END }; //WEAKNESS 약할때,//AVERAGE보통 //STRENGTH강한
}

//새로운 콜라이더의 컴포넌트를 만들고 싶다면 위에랑 아래 그리고 옵저버 매니저에서 스위치에 추가만 해주면 됨
namespace Engine {
	static const _tchar* Tag_Collider(_uint eColliderTag)
	{
		switch (eColliderTag)
		{
		case SPHERE:
			return TEXT("Com_SPHERE");
		case AABB:
			return TEXT("Com_AABB");
		case OBB:
			return TEXT("Com_OBB");
		case SHIELD:
			return TEXT("Com_SHIELD");
		case WEAKNESS:
			return TEXT("Com_WEAKNESS");
		case EVENT:
			return TEXT("Com_EVENT");
		case STATIC:
			return TEXT("Com_STATIC");
		default:
			MSGBOX("Engine_Enum에 문제 발생");
			return nullptr;
		}
	}
}
#define TAG_COLLIDER Tag_Collider