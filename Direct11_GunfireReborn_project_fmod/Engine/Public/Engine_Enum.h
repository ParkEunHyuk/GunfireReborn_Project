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
	enum COLLIDERGROUP { SPHERE, AABB, OBB, SHIELD, WEAKNESS, EVENT, STATIC, COLLIDER_END }; //WEAKNESS ���Ҷ�,//AVERAGE���� //STRENGTH����
}

//���ο� �ݶ��̴��� ������Ʈ�� ����� �ʹٸ� ������ �Ʒ� �׸��� ������ �Ŵ������� ����ġ�� �߰��� ���ָ� ��
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
			MSGBOX("Engine_Enum�� ���� �߻�");
			return nullptr;
		}
	}
}
#define TAG_COLLIDER Tag_Collider