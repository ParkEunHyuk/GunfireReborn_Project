#include "..\Public\Obsever_Manager.h"
#include "GameObject.h"
#include "Component_Manager.h"

IMPLEMENT_SINGLETON(CObsever_Manager)

CObsever_Manager::CObsever_Manager()
{

}

HRESULT CObsever_Manager::Add_ObseverGroup(OBSEVERGROUP eObseverGroup, CGameObject * _pGameObject)
{
	if (eObseverGroup >= OBSEVER_END || nullptr == _pGameObject)
	{
		MSGBOX("������ �Ŵ����� �ݶ��̴��� �������� �ʽ��ϴ�.");
		return E_FAIL;
	}

	Obsever[eObseverGroup].push_back(_pGameObject);

	Safe_AddRef(_pGameObject);

	return S_OK;
}

HRESULT CObsever_Manager::Obsever_Quest(_double DeltaTime)
{
	for (_uint i = 0; i < OBSEVER_END; i++)
	{
		for (auto& pGameObject : Obsever[i])
		{
			if (nullptr == pGameObject)
				continue;

			CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(TEXT("Com_Transform")));

			for (_uint j = 0; j < COLLIDER_END; j++)
			{
				Collider[j] = static_cast<CCollider*>(pGameObject->Get_Component(TAG_COLLIDER(j)));
				if (nullptr != Collider[j])
				{
					Collider[j]->Update_Transform(pTransform->Get_WorldMatrix());
				}
			}
		}
	}

	for (auto& pMonster : Obsever[MONSTER])
	{
		CCollider* pMonster_Sphere = static_cast<CCollider*>(pMonster->Get_Component(TAG_COLLIDER(SPHERE)));

		for (auto& pPlayerBullet : Obsever[BULLET])
		{
			CCollider* pBullet_Sphere = static_cast<CCollider*>(pPlayerBullet->Get_Component(TAG_COLLIDER(SPHERE)));
			if (true == pMonster_Sphere->Collision_Sphere(pBullet_Sphere))
			{
				pMonster->Obsever_Event(pPlayerBullet); //Slow_Time ��ų�� ���� �־��

				Obsever_Complete(DeltaTime, pPlayerBullet, pMonster);
			}
			//if (nullptr != pBullet_Sphere)
			//{
			//	if (true == pMonster_Sphere->Collision_Sphere(pBullet_Sphere))
			//	{
			//		Obsever_Complete(DeltaTime, pPlayerBullet, pMonster);
			//	}
			//}

		}
	}
	for (auto& pPlayer : Obsever[PLAYER])
	{
		CCollider* pPlayer_Sphere = static_cast<CCollider*>(pPlayer->Get_Component(TAG_COLLIDER(SPHERE)));

		for (auto& pMonsterBullet : Obsever[BULLET_MOSTER])
		{
			CCollider* pBullet_Sphere = static_cast<CCollider*>(pMonsterBullet->Get_Component(TAG_COLLIDER(SPHERE)));
			if (true == pPlayer_Sphere->Collision_Sphere(pBullet_Sphere))
			{
				pMonsterBullet->Obsever_Event(pPlayer);

				Obsever_Complete(DeltaTime, pMonsterBullet, pPlayer);
			}
		}
	}

	return S_OK;
}

HRESULT CObsever_Manager::Obsever_Complete(_double DeltaTime, CGameObject* _pSorceObject, CGameObject* _pDestObject)
{

	for (_uint i = AABB; i < COLLIDER_END; i++)
	{
		CCollider* pBullet_OBB = static_cast<CCollider*>(_pSorceObject->Get_Component(TAG_COLLIDER(i)));

		if (nullptr == pBullet_OBB)
			continue;

		for (_uint i = AABB; i < COLLIDER_END; i++)
		{
			Collider[i] = static_cast<CCollider*>(_pDestObject->Get_Component(TAG_COLLIDER(i)));
			if (nullptr != Collider[i])
			{
				if (true == Collider[i]->Collision_OBB(pBullet_OBB)) //OBB�� �����
				{
					switch (i)
					{
					case SPHERE:
						MSGBOX("����ó�� ������");
						break;
					case AABB:
						MSGBOX("����ó�� ������");
						break;
					case OBB:
						_pSorceObject->Obsever_Hit(_pDestObject); //�Ѿ˿� ������Ʈ �־��ְ� ����   //�ҽ����� �־������ ���� ����� ���� �Ǵµ�? Sorce�� �Ѿ��̰� Dest�� ���� �޴� �༮�̶�?
						_pDestObject->Obsever_Hit(_pSorceObject);  //_pDestObject�� ��Ʈ ���Ҷ�
						_pSorceObject->Set_Dead(OBJ_DEAD);
						break;
					case SHIELD:
						_pDestObject->Obsever_Event(); //_pDestObject�� ��������
						_pSorceObject->Set_Dead(OBJ_DEAD);
						break;
					case WEAKNESS:
						_pDestObject->Obsever_StrongHit(); //_pDestObject�� ġ����� �޾����� ���а� �ִ� ģ������ Obsever_Trigger2�� ���� ��, ġ����� ����
						_pSorceObject->Set_Dead(OBJ_DEAD);
						break;
					case EVENT:
						_pSorceObject->Obsever_Event(_pDestObject);
						_pDestObject->Obsever_Event(_pSorceObject);
						break;
					case STATIC:
						_pSorceObject->Obsever_Event(_pDestObject); //�Ѿ��� �������� �ʵ��� STATIC 
						_pDestObject->Obsever_Event(_pSorceObject); //_pDestObject�� ��Ʈ����
						break;
					default:
						MSGBOX("Obsever_Manager�� ������ �߻�");
						break;
					}
				}
			}
		}
	}

	//if (true == pBullet_AABB->Collision_OBB(pMonster_OBB))
	//{
	//	_pSorceObject->Obsever_Trigger();
	//	_pDestObject->Obsever_Trigger();
	//}

	//if (true == pMonster_OBB->Collision_AABB(pBullet_AABB))
	//{
	//	_pSorceObject->Obsever_Trigger();
	//	_pDestObject->Obsever_Trigger();
	//}


	//if (true == pMonster_OBB->Collision_OBB(pBullet_OBB))
	//{
	//	_pSorceObject->Obsever_Trigger();
	//	_pDestObject->Obsever_Trigger();
	//}

	return S_OK;
}

#ifdef _DEBUG
HRESULT CObsever_Manager::Obsever_Render()
{
	for (_uint i = 0; i < OBSEVER_END; i++)
	{
		for (auto& pGameObject : Obsever[i])
		{
			if (nullptr == pGameObject)
				continue;

			for (_uint i = SPHERE; i < COLLIDER_END; i++)
			{
				Collider[i] = static_cast<CCollider*>(pGameObject->Get_Component(TAG_COLLIDER(i)));
				if (nullptr != Collider[i])
				{
					Collider[i]->Render();
				}
			}
		}
	}


	return S_OK;
}
#endif // _DEBUG
HRESULT CObsever_Manager::Release_ObseverGroup()
{
	for (_uint i = 0; i < OBSEVER_END; ++i)
	{
		for (auto& iter : Obsever[i])
		{
			Safe_Release(iter);
		}
		Obsever[i].clear();
	}

	return S_OK;
}

void CObsever_Manager::Free()
{
	for (auto& iter : Obsever)
	{
		for (auto& pGameObject : iter)
		{
			Safe_Release(pGameObject);
		}
		iter.clear();
	}
}
