#include "stdafx.h"
#include "..\Public\Monster_Cube.h"
#include "GameInstance.h"
#include "Item_Universal.h"
#include "Universal_Point_Instance.h"


CMonster_Cube::CMonster_Cube(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CMonster_Cube::CMonster_Cube(const CMonster_Cube & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Cube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Cube::NativeConstruct(void * pArg)
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


	//���� ����
	m_Info.fHp = 50;
	m_Info.fMaxHp = m_Info.fHp;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	m_pPlayer = static_cast<CGameObject*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Player")));
	m_pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")));

	m_pModelCom->SetUp_AnimIndex(0);

	RELEASE_INSTANCE(CGameInstance);

	m_pNavigationCom->Find_CellIndex(m_pTransform);

	return S_OK;
}

_int CMonster_Cube::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;
	if (m_Info.fHp <= 0 || OBJ_DEAD == m_iDead)
	{
		return OBJ_DEAD;
	}

	Slow_On();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->GetInstance()->Add_ObseverGroup(MONSTER, this)))
	{
		MSGBOX("Monster_Lizard�� Add_ObseverGroup�� ������ �߻��߽��ϴ�");
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
	else {
		SetUp_Fight(TimeDelta * m_dSlowTime);

		PlayAnim(TimeDelta * m_dSlowTime);
	}

	//SetUp_Fight(TimeDelta * m_dSlowTime);

	//PlayAnim(TimeDelta * m_dSlowTime);


	//m_pSphereCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	RELEASE_INSTANCE(CGameInstance);

	return OBJ_NOEVENT;
}

_int CMonster_Cube::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//__super::SetUp_OnTerrain(m_eNowLevelNum, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	SetUp_OnNavigation(m_pNavigationCom, m_eNowLevelNum, TEXT("Layer_BackGround"), 0);


	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);


	RELEASE_INSTANCE(CGameInstance);

#ifdef _DEBUG	
	m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif // _DEBUG

	return _int();
}

HRESULT CMonster_Cube::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CMonster_Cube::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Model_Cube"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Sphere */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 1.f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;


	ColliderDesc.vScale = _float3(1.2f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 1.f, 0.f, 1.f);

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

HRESULT CMonster_Cube::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Cube::SetUp_Event_Tirgger(_double TimeDelta)
{
	return S_OK;
}

HRESULT CMonster_Cube::SetUp_Fight(_double TimeDelta)
{
	Fight_On();

	if (true == m_bFight)
	{
		m_pTransform->LookAtNOTAxisY(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);

		CoolTime_Manager(TimeDelta);

	}

	return S_OK;
}

HRESULT CMonster_Cube::Fight_On()
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

HRESULT CMonster_Cube::Once_Trigger()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (LEVEL_MAPTOOL != m_eNowLevelNum)//�̰� �����ؾ���...�̰� ������ �з��� ��ũ ����...
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

HRESULT CMonster_Cube::PlayAnim(_double TimeDelta)
{
	if (false == m_bOneTimeSwith) //�̰� �������Ǿ� �����鼭 �ѹ��� �����Ű�� ���� tmi)������ �ִ� �ִϸ��̼� ����� ���������� ����
	{
		Infinity_AnimMotion(TimeDelta);
		m_pModelCom->SetUp_AnimIndex(m_iInfinityAnimNumber); //���� Ư�� ������ ��� �ݺ������� ������
	}
	else {
		Once_AnimMotion(TimeDelta); //�̰� �����̶� �������� �Ǿ� �ִ°� �ѹ��� ����� ���ѽ������� �ٲٰ� �� �Լ� ���⿡ ������ �� ��?
		m_pModelCom->SetUp_AnimIndex(m_iOnceAnimNumber); //�ѹ��� �������� �ϴ� m_bOneTimeSwith�� true�� �ٲ������
	}

	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_bOneTimeSwith);

	return S_OK;
}

HRESULT CMonster_Cube::CoolTime_Manager(_double TimeDelta)
{
	if (true == m_bBoomOn && false == m_bOneTimeSwith)
	{
		m_Info.fHp = 0.001;
		m_iDead = OBJ_DEAD;
	}

	m_dOnceCoolTime += TimeDelta;
	if (m_dOnceCoolTime > 3)
	{
		m_iOncePattern = 0;
		m_bOneTimeSwith = true;
		m_dOnceCoolTime = 0;
		m_dInfinityCoolTime = 0;
	}

	if (m_fDistance < 3)
	{
		m_iOncePattern = 1;
		m_bOneTimeSwith = true;
		m_dOnceCoolTime = 0;
		m_dInfinityCoolTime = 0;
	}


	m_dInfinityCoolTime += TimeDelta;
	if (m_dInfinityCoolTime > 3)
	{
		m_iInfinityPattern = rand() % 2;
		m_dInfinityCoolTime = 0;

	}

	return S_OK;
}

HRESULT CMonster_Cube::Once_AnimMotion(_double TimeDelta) //�����ѹ��� ���ݳѹ��� �ٲ㵵 ��������?
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	switch (m_iOncePattern)
	{
	case 0: //������ �پ��
	{
		m_iOnceAnimNumber = 7;
		m_pTransform->Go_Straight(TimeDelta*2, m_pNavigationCom);
		break;
	}
	case 1: //������ �پ��
	{
		m_iOnceAnimNumber = 3;
		m_bBoomOn = true;
		//���� Ʈ��� �ٲٰ� �� �ִϸ��̼� ����� ������ �ٲ���!!!
		break;
	}
	default:
		break;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Cube::Infinity_AnimMotion(_double TimeDelta)
{
	if (true == m_bFight)
	{
		switch (m_iInfinityPattern)
		{
		case 0: //������ ������
			m_iInfinityAnimNumber = 4;
			m_pTransform->Go_Straight(TimeDelta, m_pNavigationCom);
			break;
		case 1: //������ ������
			m_iInfinityAnimNumber = 4;
			m_pTransform->Go_Straight(TimeDelta, m_pNavigationCom);
			break;
		default:
			break;
		}
	}
	return S_OK;
}

_int CMonster_Cube::Obsever_Hit(void* pArg)
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

	m_iOnceAnimNumber = 1;
	m_iOncePattern = 20;
	m_iInfinityAnimNumber = 4;
	m_bOneTimeSwith = true;

	m_bFight = true;

	//CUniversal_Bullet* SorceBullet = static_cast<CUniversal_Bullet*>(pArg);

	//if (nullptr != SorceBullet) //����� �ʰ� ���°� Reflect��ų�� ���� �� ���� źȯ�� ���⸦ ���� ������ ����� ��
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

_int CMonster_Cube::Obsever_Event(void * pArg)
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

	return _int();
}

_int CMonster_Cube::Dead_Trigger()
{
	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//CItem_Universal::ITEM_UNIVERSALDESC Item_UniversalDesc;
	////��ų��
	//Item_UniversalDesc.vScale = _float3(1.f, 1.f, 1.f);
	//Item_UniversalDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);

	//_float4 fTempPos;

	//XMStoreFloat4(&fTempPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
	//fTempPos.y += 0.5f;
	//Item_UniversalDesc.vTranslation = fTempPos;

	//for (_int i = 0; i < 5; i++)
	//{
	//	_int Rand = rand() % 4;
	//	Item_UniversalDesc.iItemNumber = Rand;
	//	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Item_Universal"), &Item_UniversalDesc)))
	//		return E_FAIL;
	//}

	//RELEASE_INSTANCE(CGameInstance);


	Universal_Boom();

	Boom_Particle();

	return _int();
}

HRESULT CMonster_Cube::Universal_Boom()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
	ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

	Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::CUBE_BOOM;
	Universal_BulletDesc.eType = BULLET_MOSTER;

	Universal_BulletDesc.fSpeedPerSec = 50.f;
	Universal_BulletDesc.vScale = _float3(10.f, 10.f, 10.f);
	Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_BulletDesc.dDuration = 0.1f;

	Universal_BulletDesc.pObject = this;
	Universal_BulletDesc.fPositioning = _float3(0.f, 1.5f, 0.f);


	Universal_BulletDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
	Universal_BulletDesc.fColliderOBBScale = _float3(1.f, 1.f, 1.f);

	Universal_BulletDesc.iTextureIndex = 0;

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	GetSingle(CGameInstance)->PlaySound((L"Cube_Boom.mp3"), CHANNEL_EFFECT);

	return S_OK;
}



HRESULT CMonster_Cube::Slow_On()
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

HRESULT CMonster_Cube::Notice()
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

HRESULT CMonster_Cube::Boom_Particle()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC Universal_PointDesc;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::EMBERS_PARTICLE;
	Universal_PointDesc.iParticleType = CVIBuffer_Point_Instance::PARTICLE_FIRECRACKER;

	Universal_PointDesc.fMinSpeed = 5.f;
	Universal_PointDesc.fMaxSpeed = 10.f;
	Universal_PointDesc.fRange = _float3(1.f, 1.f, 1.f);

	Universal_PointDesc.fSize = 5;
	Universal_PointDesc.iTextureIndex = 15;

	Universal_PointDesc.pObject = this;

	Universal_PointDesc.dDuration = 0.5;

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::FIRE_PARTICLE;
	Universal_PointDesc.fSize = 0.8;
	Universal_PointDesc.iTextureIndex = 0;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::FIRE_ROCK_PARTICLE;
	Universal_PointDesc.fSize = 0.8;
	Universal_PointDesc.iTextureIndex = 0;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CMonster_Cube * CMonster_Cube::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Cube*	pInstance = new CMonster_Cube(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMonster_Cube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Cube::Clone(void * pArg)
{
	CMonster_Cube*	pInstance = new CMonster_Cube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Cube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Cube::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}