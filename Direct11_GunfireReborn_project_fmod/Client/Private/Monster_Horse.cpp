#include "stdafx.h"
#include "..\Public\Monster_Horse.h"
#include "GameInstance.h"
#include "Item_Universal.h"
#include "Monster_Shield.h"
#include "Bullet.h"


CMonster_Horse::CMonster_Horse(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CMonster_Horse::CMonster_Horse(const CMonster_Horse & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Horse::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Horse::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.0f)))))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		memcpy(&m_tObjectDataDesc, pArg, sizeof(OBJECTDATADESC));

		m_pTransform->Set_WorldFloat4x4(XMLoadFloat4x4(&m_tObjectDataDesc.TransformMatrix));
	}


	//몬스터 정보
	m_Info.fHp = 150;
	m_Info.fMaxHp = m_Info.fHp;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	m_pPlayer = static_cast<CGameObject*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Player")));
	m_pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")));

	RELEASE_INSTANCE(CGameInstance);


	SetUp_Shield();

	m_pNavigationCom->Find_CellIndex(m_pTransform);

	SetUp_Trail();

	return S_OK;
}

_int CMonster_Horse::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;
	if (m_Info.fHp <= 0)
	{
		m_bDissolveOn = true;
		if (true == m_bDissolveOn)
		{
			m_fDissolveTime += (_float)TimeDelta * 0.5;
			if (m_fDissolveTime >= 0.5)
			{
				return OBJ_DEAD;
			}
		}
	}

	Slow_On();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->GetInstance()->Add_ObseverGroup(MONSTER, this)))
	{
		MSGBOX("Monster_Lizard의 Add_ObseverGroup에 문제가 발생했습니다");
		return E_FAIL;
	}

	if (true == m_bFight && true == m_bOnce_Switch)
	{
		Once_Trigger();
		m_bOnce_Switch = false;
	}

	if (false == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f) && false == m_bFight)
	{
	}
	else if(false == m_bDissolveOn){
		SetUp_Fight(TimeDelta * m_dSlowTime);

		PlayAnim(TimeDelta * m_dSlowTime);
	}

	//SetUp_Fight(TimeDelta * m_dSlowTime);

	//PlayAnim(TimeDelta * m_dSlowTime);


	//m_pSphereCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	RELEASE_INSTANCE(CGameInstance);

	return OBJ_NOEVENT;
}

_int CMonster_Horse::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	SetUp_OnNavigation(m_pNavigationCom, m_eNowLevelNum, TEXT("Layer_BackGround"), 0);


	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);


	RELEASE_INSTANCE(CGameInstance);

#ifdef _DEBUG	
	m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif // _DEBUG


	return _int();
}

HRESULT CMonster_Horse::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");

		//m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");

		m_pModelCom->Render(m_pShaderCom, (false == m_bDissolveOn) ? 0 : 1, i, "g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CMonster_Horse::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Model_Horse"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve_1"), TEXT("Com_Texture"), (CComponent**)&m_pDissolveTextrueCom)))
		return E_FAIL;

	/* For.Com_Sphere */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 1.f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;


	ColliderDesc.vScale = _float3(1.0f, 2.0f, 2.0f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 1.2f, -0.2f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.pParent = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_BackGround"), CGameObject::m_pTransformTag));;
	NaviDesc.iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}

HRESULT CMonster_Horse::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pDissolveTextrueCom->SetUp_OnShader(m_pShaderCom, "g_DissolveTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fDissolveTime", &m_fDissolveTime, sizeof(_float))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Horse::SetUp_Event_Tirgger(_double TimeDelta)
{
	return S_OK;
}

HRESULT CMonster_Horse::SetUp_Fight(_double TimeDelta)
{
	Fight_On();

	if (true == m_bFight)
	{
		m_pTransform->LookAtNOTAxisY(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
		if (m_fDistance < 2)
		{
			m_pTransform->Go_Backward(TimeDelta,m_pNavigationCom);
		}


		CoolTime_Manager(TimeDelta);

	}

	return S_OK;
}

HRESULT CMonster_Horse::Fight_On()
{

	_vector vDis = m_pTransform->Get_Distance(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	m_fDistance;

	XMStoreFloat(&m_fDistance, vDis);

	if (m_fDistance < 10 && false == m_bFight)
	{
		m_bFight = true;
	}


	return S_OK;
}

HRESULT CMonster_Horse::Once_Trigger()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (LEVEL_MAPTOOL != m_eNowLevelNum)//이거 조심해야함...이거 때문에 밀려서 포크 나옴...
	{

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_MonsterBar"), TEXT("Prototype_GameObject_UI_MonsterBar"), this)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Monster_HpBar"), TEXT("Prototype_GameObject_UI_Monster_HpBar"), this)))
			return E_FAIL;

	}

	RELEASE_INSTANCE(CGameInstance);

	Notice();

	return S_OK;
}

HRESULT CMonster_Horse::PlayAnim(_double TimeDelta)
{
	if (false == m_bOneTimeSwith) //이건 블렌딩되어 있으면서 한번만 실행시키는 변수 tmi)웨폰에 있는 애니메이션 제어는 블렌딩하지 않음
	{
		Infinity_AnimMotion(TimeDelta);
		m_pModelCom->SetUp_AnimIndex(m_iInfinityAnimNumber); //만약 특정 변수로 계속 반복적으로 움직임
	}
	else {
		Once_AnimMotion(TimeDelta); //이거 어택이랑 무빙으로 되어 있는거 한번만 실행과 무한실행으로 바꾸고 그 함수 여기에 박으면 될 듯?
		m_pModelCom->SetUp_AnimIndex(m_iOnceAnimNumber); //한번만 돌릴려면 일단 m_bOneTimeSwith를 true로 바꿔줘야함
	}
	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_bOneTimeSwith);

	return S_OK;
}

HRESULT CMonster_Horse::CoolTime_Manager(_double TimeDelta)
{
	m_dOnceCoolTime += TimeDelta;

	if (m_fDistance < 4 && m_dOnceCoolTime > 5)
	{
		_uint randomAttack = rand() % 5;

		m_iOncePattern = randomAttack;
		m_bOneTimeSwith = true;
		m_dOnceCoolTime = 0;
		m_dInfinityCoolTime = 0;

		m_bFire = true;
		m_dFireTime = 0;
	}

	m_dInfinityCoolTime += TimeDelta;
	if (m_dInfinityCoolTime > 3)
	{
		m_iInfinityPattern = rand() % 6;
		m_dInfinityCoolTime = 0;

	}

	return S_OK;
}

HRESULT CMonster_Horse::Once_AnimMotion(_double TimeDelta) //무빙넘버와 공격넘버로 바꿔도 괜찮을듯?
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	switch (m_iOncePattern)
	{
	case 0:
	{
		m_iOnceAnimNumber = 16; //칼 가볍게 휘두르기
		m_bOneTimeSwith = true;
		Universal_Wield(TimeDelta);
		m_pTrail->Set_NotRendering(false);
		break;
	}
	case 1:
	{
		m_iOnceAnimNumber = 17; //칼 찍어내리기
		m_bOneTimeSwith = true;
		Universal_StrikeDown(TimeDelta);
		m_pTrail->Set_NotRendering(false);
		break;
	}
	case 2:
	{
		m_iOnceAnimNumber = 18; //방패 휘두르기
		m_bOneTimeSwith = true;
		Universal_ShieldAttack(TimeDelta);
		break;
	}
	case 3:
	{
		m_iOnceAnimNumber = 16; //칼 가볍게 휘두르기
		m_bOneTimeSwith = true;
		Universal_Wield(TimeDelta);
		m_pTrail->Set_NotRendering(false);
		break;
	}
	case 4:
	{
		m_iOnceAnimNumber = 17; //칼 찍어내리기
		m_bOneTimeSwith = true;
		Universal_StrikeDown(TimeDelta);
		m_pTrail->Set_NotRendering(false);
		break;
	}
	default:
		break;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Horse::Infinity_AnimMotion(_double TimeDelta)
{
	m_pTrail->Set_NotRendering(true);
	if (true == m_bFight)
	{
		switch (m_iInfinityPattern)
		{
		case 0: //앞으로 움직임
			m_iInfinityAnimNumber = 4;
			m_pTransform->Go_Straight(TimeDelta, m_pNavigationCom);
			break;
		case 1: //뒤로 움직임
			m_iInfinityAnimNumber = 6;
			m_pTransform->Go_Backward(TimeDelta*0.2, m_pNavigationCom);
			break;
		case 2: //왼쪽으로 움직임
			m_iInfinityAnimNumber = 7;
			m_pTransform->Go_Left(TimeDelta, m_pNavigationCom);
			break;
		case 3: //오른쪽으로 움직임
			m_iInfinityAnimNumber = 5;
			m_pTransform->Go_Right(TimeDelta, m_pNavigationCom);
			break;
		case 4: //앞으로 움직임
			m_iInfinityAnimNumber = 4;
			m_pTransform->Go_Straight(TimeDelta, m_pNavigationCom);
			break;
		case 5: //앞으로 움직임
			m_iInfinityAnimNumber = 4;
			m_pTransform->Go_Straight(TimeDelta, m_pNavigationCom);
			break;
		default:
			break;
		}
	}
	return S_OK;
}

_int CMonster_Horse::Obsever_Hit(void* pArg)
{
	CUniversal_Bullet* SorceBullet = static_cast<CUniversal_Bullet*>(pArg);

	if (CUniversal_Bullet::NOTICE == SorceBullet->m_Universal_BulletDesc.iBulletNumber)
	{
		m_iInfinityAnimNumber = 4;
		m_iInfinityPattern = 0;

		m_dOnceCoolTime = 0;
		m_dInfinityCoolTime = 0;

		m_bFight = true;

		return _int();
	}

	m_iOnceAnimNumber = 15;
	m_iOncePattern = 15;
	m_iInfinityAnimNumber = 1;
	m_bOneTimeSwith = true;

	m_bFight = true;

	//CUniversal_Bullet* SorceBullet = static_cast<CUniversal_Bullet*>(pArg);

	//if (nullptr != SorceBullet) //까먹지 않게 쓰는것 Reflect스킬을 쓰고 난 이후 탄환은 여기를 통해 데미지 계산이 됨
	//{
	//	switch (SorceBullet->m_Universal_BulletDesc.iBulletNumber)
	//	{
	//	case CUniversal_Bullet::LIZARD:
	//	{
	//		m_Info.fHp -= 10;
	//		break;
	//	}
	//	}
	//}

	return _int();
}

_int CMonster_Horse::Obsever_Event(void * pArg)
{
	CUniversal_Bullet* SorceBullet = static_cast<CUniversal_Bullet*>(pArg);

	if (nullptr != SorceBullet)
	{
		switch (SorceBullet->m_Universal_BulletDesc.iBulletNumber)
		{
		case CUniversal_Bullet::SLOWTIME_SKILL:
		{
			m_pTempSlow_Bullet = SorceBullet;
			m_dSlowTime = 0.1;
			break;
		}
		default:
			break;
		}
	}

	if (CUniversal_Bullet::SLOWTIME_SKILL != SorceBullet->m_Universal_BulletDesc.iBulletNumber)
	{
		CBullet* SorcePlayerBullet = static_cast<CBullet*>(pArg);
		if (nullptr != SorceBullet)
		{
			CCollider* pSPHERE = static_cast<CCollider*>(SorceBullet->Get_Component(TEXT("Com_SPHERE")));
			if (nullptr == pSPHERE)
			{
				return _int();
			}
			if (true == m_pShieldSphereCom->Collision_Sphere(pSPHERE))
			{
				CCollider* pOBB = static_cast<CCollider*>(SorceBullet->Get_Component(TEXT("Com_OBB")));
				if (true == m_pShieldOBBCom->Collision_OBB(pOBB))
				{
					SorceBullet->Obsever_Sound();
					SorceBullet->Set_Dead(OBJ_DEAD);
				}
			}
		}

		if (nullptr != SorcePlayerBullet)
		{
			CCollider* pSPHERE = static_cast<CCollider*>(SorcePlayerBullet->Get_Component(TEXT("Com_SPHERE")));
			if (true == m_pShieldSphereCom->Collision_Sphere(pSPHERE))
			{
				CCollider* pOBB = static_cast<CCollider*>(SorcePlayerBullet->Get_Component(TEXT("Com_OBB")));

				if (true == m_pShieldOBBCom->Collision_OBB(pOBB))
				{
					SorcePlayerBullet->Obsever_Sound();
					SorcePlayerBullet->Set_Dead(OBJ_DEAD);
				}
			}
		}
	}

	return _int();
}

_int CMonster_Horse::Dead_Trigger()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CItem_Universal::ITEM_UNIVERSALDESC Item_UniversalDesc;
	//스킬들
	Item_UniversalDesc.vScale = _float3(1.f, 1.f, 1.f);
	Item_UniversalDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);

	_float4 fTempPos;

	XMStoreFloat4(&fTempPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	fTempPos.y += 0.5f;
	Item_UniversalDesc.vTranslation = fTempPos;

	for (_int i = 0; i < 5; i++)
	{
		_int Rand = rand() % 4;
		Item_UniversalDesc.iItemNumber = Rand;
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Item_Universal"), &Item_UniversalDesc)))
			return E_FAIL;
	}



	RELEASE_INSTANCE(CGameInstance);

	m_pTrail->Stop_ComputeTrail();

	return _int();
}

HRESULT CMonster_Horse::Universal_Wield(_double TimeDelta)
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.5)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::MELEE_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(8.5f, 8.5f, 8.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0, 1.5, 0.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.0f, 1.0f, 1.0f);
		Universal_BulletDesc.fColliderOBBScale = _float3(0.7f, 0.7f, 0.7f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;

		GetSingle(CGameInstance)->PlaySound((L"Horse_Wield.mp3"), CHANNEL_EFFECT);

	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Horse::Universal_StrikeDown(_double TimeDelta)
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.5)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::MELEE_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(8.5f, 8.5f, 8.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1; //0.1

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0, 1.0, 2.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.0f, 1.0f, 1.0f);
		Universal_BulletDesc.fColliderOBBScale = _float3(0.7f, 0.7f, 0.7f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;

		GetSingle(CGameInstance)->PlaySound((L"Horse_StrikeDown.mp3"), CHANNEL_EFFECT);
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Horse::Universal_ShieldAttack(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;
	if (true == m_bFire && m_dFireTime > 0.5)
	{
		CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
		ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

		Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::MELEE_ATTACK;
		Universal_BulletDesc.eType = BULLET_MOSTER;

		Universal_BulletDesc.fSpeedPerSec = 0.f;
		Universal_BulletDesc.vScale = _float3(8.5f, 8.5f, 8.5f);
		Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletDesc.dDuration = 0.1;

		Universal_BulletDesc.pObject = this;
		Universal_BulletDesc.fPositioning = _float3(0, 1.5, 0.f);


		Universal_BulletDesc.fColliderSphereScale = _float3(1.0f, 1.0f, 1.0f);
		Universal_BulletDesc.fColliderOBBScale = _float3(0.7f, 0.7f, 0.7f);

		Universal_BulletDesc.iTextureIndex = 0;

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
			return E_FAIL;

		m_bFire = false;

		GetSingle(CGameInstance)->PlaySound((L"Horse_ShieldAttack.mp3"), CHANNEL_EFFECT);
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMonster_Horse::SetUp_Shield()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CModel*		pModel = m_pModelCom;
	CTransform*	pTransform = m_pTransform;

	CMonster_Shield::SHIELDDESC			ShieldDesc;
	ShieldDesc.eShieldNumber = CMonster_Shield::HORSE_SHIELD;
	ShieldDesc.pModel = pModel;
	ShieldDesc.pSocketName = "Bip001 L Finger0Nub";
	ShieldDesc.pTransform = pTransform;
	ShieldDesc.pObject = this;

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Shield"), TEXT("Prototype_GameObject_Monster_Shield"), &ShieldDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMonster_Horse::Set_Shield(CCollider* _pShieldSphere, CCollider * _pShieldOBB)
{
	m_pShieldSphereCom = _pShieldSphere;
	m_pShieldOBBCom = _pShieldOBB;
	return S_OK;
}

HRESULT CMonster_Horse::Slow_On()
{
	if (nullptr != m_pTempSlow_Bullet && m_fSlowBulletDistance < 200)
	{
		CTransform* Slow_BulletTransform = static_cast<CTransform*>(m_pTempSlow_Bullet->Get_Component(TEXT("Com_Transform")));
		_vector vDis = m_pTransform->Get_Distance(Slow_BulletTransform->Get_State(CTransform::STATE_POSITION));

		XMStoreFloat(&m_fSlowBulletDistance, vDis);
	}


	if (m_fSlowBulletDistance < 100)
	{
		if (m_pSphereCom->Collision_Sphere(m_pTempSlow_Bullet->m_pSphereCom))
		{
			m_dSlowTime = 0.2;
		}
		else {
			//m_dSlowTime = 1;
			if (m_dSlowTime <= 1)
			{
				m_dSlowTime += 0.01;
			}
		}
	}
	else {
		if (m_dSlowTime <= 1)
		{
			m_dSlowTime += 0.01;
		}
	}

	return S_OK;
}

HRESULT CMonster_Horse::Notice()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
	ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

	Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::NOTICE;
	Universal_BulletDesc.eType = BULLET;

	Universal_BulletDesc.fSpeedPerSec = 50.f;
	Universal_BulletDesc.vScale = _float3(30.f, 30.f, 30.f);
	Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_BulletDesc.dDuration = 20;

	Universal_BulletDesc.pObject = this;
	Universal_BulletDesc.fPositioning = _float3(0.f, 0.f, 0.f);


	Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
	Universal_BulletDesc.fColliderOBBScale = _float3(1.f, 1.f, 1.f);

	Universal_BulletDesc.iTextureIndex = 0;

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Horse::SetUp_Trail()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTrail::TRAILDESC TrailDesc;
	TrailDesc.iTrailTextureIndex = CTrail::UNIVERSAL_TRAIL;
	//TrailDesc.pTransformMatrix = &m_TransformMatrix;

	TrailDesc.pModel = m_pModelCom;
	TrailDesc.pSocketName = "Bip001 R Finger1Nub";
	TrailDesc.pTransform = m_pTransform;
	TrailDesc.pObject = this;
	TrailDesc.eMonsterType = CTrail::HORSE;

	TrailDesc.vHighPos = _float3(0.2f, -0.f, 1.36f);
	TrailDesc.vLowPos = _float3(-0.1f, 0.f, 0.1f);

	TrailDesc.vColor = _float4(255.f / 255.f, 1.f / 255.f, 255.f / 255.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Trail"), TEXT("Prototype_GameObject_Trail"), &TrailDesc)))
		return E_FAIL;


	m_pTrail->Set_NotRendering(true);
	//m_pTrail->Set_NotRendering(false);

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

void CMonster_Horse::Set_Trail(CTrail * pTrail)
{
	m_pTrail = pTrail;
}

CMonster_Horse * CMonster_Horse::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Horse*	pInstance = new CMonster_Horse(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMonster_Horse");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Horse::Clone(void * pArg)
{
	CMonster_Horse*	pInstance = new CMonster_Horse(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Horse");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Horse::Free()
{
	__super::Free();

	Safe_Release(m_pDissolveTextrueCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
