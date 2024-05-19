#include "stdafx.h"
#include "..\Public\Bullet_Launcher.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "Universal_Point_Instance.h"

CBullet_Launcher::CBullet_Launcher(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CBullet(pDevice, pDeviceContext)
{

}

CBullet_Launcher::CBullet_Launcher(const CBullet_Launcher & rhs)
	: CBullet(rhs)
{

}

HRESULT CBullet_Launcher::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet_Launcher::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 30.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pWeaponTransformCom = static_cast<CTransform*>(pGameInstance->GetInstance()->Get_Component(m_eNowLevelNum, TEXT("Layer_Weapon_Launcher"), TEXT("Com_Transform")));

	m_Camera = static_cast<CCamera_Player*>(pGameInstance->GetInstance()->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")));


	RELEASE_INSTANCE(CGameInstance);

	m_pTransform->Scaled(XMVectorSet(1, 1, 1.f, 0.0f));

	Set_BulletPos();

	m_pNavigationCom->Find_CellIndex(m_pTransform);

	return S_OK;
}


_int CBullet_Launcher::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;
	m_dDeadTime += TimeDelta;
	if (m_dDeadTime >= 10 || m_iDead == OBJ_DEAD)
	{
		return OBJ_DEAD;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->GetInstance()->Add_ObseverGroup(BULLET, this)))
	{
		MSGBOX("CBullet_Shotgun의 Add_ObseverGroup에 문제가 발생했습니다");
		return E_FAIL;
	}

	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Down)
	{
		m_iDead = true;
	}

	Set_GuidedBullets(TimeDelta*0.3);

	_float3 vDir;
	XMStoreFloat3(&vDir, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * m_pTransform->Get_TransformDesc().fSpeedPerSec * (_float)TimeDelta);

	if (false == m_pNavigationCom->Move_OnNavigation(m_pTransform->Get_State(CTransform::STATE_POSITION), &vDir))
	{
		m_iDead = OBJ_DEAD;
	}



	RELEASE_INSTANCE(CGameInstance);


	return OBJ_NOEVENT;
}

_int CBullet_Launcher::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	RELEASE_INSTANCE(CGameInstance);

#ifdef _DEBUG	
	m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif // _DEBUG


	return _int();
}

HRESULT CBullet_Launcher::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, 0, i);
	}

	return S_OK;
}

_int CBullet_Launcher::Dead_Trigger()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
	ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

	Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::LAUNCHER_BOOM;
	Universal_BulletDesc.eType = BULLET;

	Universal_BulletDesc.fSpeedPerSec = 0.f;
	Universal_BulletDesc.vScale = _float3(1.f, 1.f, 1.f);         //현재 불렛의 크기는 1이며 충돌체의 크기만 20으로 큼
	Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_BulletDesc.dDuration = 0.01f;
	//Universal_BulletDesc.dDuration = 9999.f;

	Universal_BulletDesc.pObject = this;
	Universal_BulletDesc.fPositioning = _float3(0, 0, 1.0f);

	Universal_BulletDesc.fColliderSphereScale = _float3(20.f, 20.f, 20.f);
	Universal_BulletDesc.fColliderOBBScale = _float3(20.f, 20.f, 20.f);

	Universal_BulletDesc.iTextureIndex = 0;

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Launcher_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
		return E_FAIL;

	m_Camera->Start_CameraQuake(20, 0.5);

	RELEASE_INSTANCE(CGameInstance);

	Boom_Particle();

	//둘 다 다른 방식으로 거리를 보간
	//_float fSound_Dis = this->Get_CamDistance();
	//fSound_Dis = (100 - fSound_Dis) / 100;  //최대거리- 현재거리 / 100(퍼센트를 계산하기 위한 100)
	//FAILED_CHECK(GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_EFFECT, fSound_Dis));
	//FAILED_CHECK(GetSingle(CGameInstance)->PlaySound((L"M1_33.mp3"), CHANNEL_EFFECT));

	//둘 다 다른 방식으로 거리를 보간
	//_float fSound_Dis = this->Get_CamDistance();
	//fSound_Dis = fSound_Dis / 100;  //거리(소리나는위치 - 현재위치) / 최대거리
	//FAILED_CHECK(GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_EFFECT, 1 - fSound_Dis));
	//FAILED_CHECK(GetSingle(CGameInstance)->PlaySound((L"M1_33.mp3"), CHANNEL_EFFECT));


	GetSingle(CGameInstance)->PlaySound((L"Bullet_Launcher.mp3"), CHANNEL_EFFECT);



	return _int();
}

HRESULT CBullet_Launcher::Set_GuidedBullets(_double TimeDelta)
{
	m_pTransform->Set_State(CTransform::STATE_RIGHT, m_pWeaponTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_UP, m_pWeaponTransformCom->Get_State(CTransform::STATE_UP));

	m_pTransform->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(m_pWeaponTransformCom->Get_State(CTransform::STATE_LOOK)));

	m_pTransform->Go_Straight(TimeDelta);

	return S_OK;
}

_bool CBullet_Launcher::Set_BulletPos()
{
	//m_pTransform->Set_State(CTransform::STATE_RIGHT, m_pWeaponTransformCom->Get_State(CTransform::STATE_RIGHT));
	//m_pTransform->Set_State(CTransform::STATE_UP, m_pWeaponTransformCom->Get_State(CTransform::STATE_UP));
	//m_pTransform->Set_State(CTransform::STATE_LOOK, m_pWeaponTransformCom->Get_State(CTransform::STATE_LOOK));

	_vector vPos = m_pWeaponTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos -= XMVector3Normalize(m_pWeaponTransformCom->Get_State(CTransform::STATE_RIGHT)) * 0.8f;
	vPos += XMVector3Normalize(m_pWeaponTransformCom->Get_State(CTransform::STATE_UP)) * 1.f;
	vPos += XMVector3Normalize(m_pWeaponTransformCom->Get_State(CTransform::STATE_LOOK)) * 1.f;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);


	return true;
}

HRESULT CBullet_Launcher::Boom_Particle()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC Universal_PointDesc;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::GLOW_RED_PARTICLE;
	Universal_PointDesc.iParticleType = CVIBuffer_Point_Instance::PARTICLE_FIRECRACKER;

	//Universal_PointDesc.fMinSpeed = 5.f;
	//Universal_PointDesc.fMaxSpeed = 10.f;

	Universal_PointDesc.fMinSpeed = 10.f;
	Universal_PointDesc.fMaxSpeed = 10.f;
	Universal_PointDesc.fRange = _float3(1.f, 1.f, 1.f);

	Universal_PointDesc.pObject = this;
	Universal_PointDesc.dDuration = 0.7;
	Universal_PointDesc.iTextureIndex = 0;

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SMALL_GLOW_BLUE_PARTICLE;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SMALL_GLOW_PURPLE_PARTICLE;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SMALL_GLOW_GREEN_PARTICLE;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SMALL_GLOW_YELLOW_PARTICLE;
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;


	///////////////////////////테스트중우우우우우웅 이거 파티클 스트라이트임 즉 이건 삭제 가능하고 위에만 주석을 풀자
	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC Universal_PointDesc;

	//Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::GLOW_RED_PARTICLE;
	//Universal_PointDesc.iParticleType = CVIBuffer_Point_Instance::PARTICEL_STRAIGHT;

	////Universal_PointDesc.fMinSpeed = 5.f;
	////Universal_PointDesc.fMaxSpeed = 10.f;

	//Universal_PointDesc.fMinSpeed = 2.f;
	//Universal_PointDesc.fMaxSpeed = 10.f;
	//Universal_PointDesc.fRange = _float3(1.f, 1.f, 1.f);

	//_float3 fDis;
	//XMStoreFloat3(&fDis, m_pTransform->Get_State(CTransform::STATE_LOOK));
	//Universal_PointDesc.fDirection = fDis;

	//Universal_PointDesc.pObject = this;
	//Universal_PointDesc.dDuration = 10000;
	//Universal_PointDesc.iTextureIndex = 0;

	//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
	//	return E_FAIL;

	//Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SMALL_GLOW_BLUE_PARTICLE;
	//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
	//	return E_FAIL;

	//Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SMALL_GLOW_PURPLE_PARTICLE;
	//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
	//	return E_FAIL;

	//Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SMALL_GLOW_GREEN_PARTICLE;
	//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
	//	return E_FAIL;

	//Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::SMALL_GLOW_YELLOW_PARTICLE;
	//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
	//	return E_FAIL;
	////////////////////////////////////////

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CBullet_Launcher::Obsever_Hit(void * pArg)
{
	CLandObject* DestObject = static_cast<CLandObject*>(pArg);

	DestObject->m_Info.fHp -= 1;

	return _int();
}

HRESULT CBullet_Launcher::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_NonAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bullet_Launcher"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Sphere */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	ColliderDesc.vScale = _float3(1.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
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

HRESULT CBullet_Launcher::SetUp_ConstantTable()
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

CBullet_Launcher * CBullet_Launcher::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBullet_Launcher*	pInstance = new CBullet_Launcher(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CBullet_Launcher");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBullet_Launcher::Clone(void * pArg)
{
	CBullet_Launcher*	pInstance = new CBullet_Launcher(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Copying CBullet_Launcher");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_Launcher::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}