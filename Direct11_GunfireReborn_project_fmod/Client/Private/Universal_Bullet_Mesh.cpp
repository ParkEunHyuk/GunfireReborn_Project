#include "stdafx.h"
#include "..\Public\Universal_Bullet_Mesh.h"
#include "GameInstance.h"
#include "Player.h"
#include "Level.h"
#include "UI_Universal.h"
#include "Universal_Point_Instance.h"

const 	_tchar*	 m_pModel_BulletTag[CUniversal_Bullet_Mesh::MESH_END] = {
	TEXT("Prototype_Component_Model_Burn_Bullet"),
	TEXT("Prototype_Component_Model_Supplement_Bullet"),
	TEXT("Prototype_Component_Model_Meteorite_Bullet"),
	TEXT("Prototype_Component_Model_Terrain_Bullet"),
	TEXT("Prototype_Component_Model_ShockWave_Bullet"),
	TEXT("Prototype_Component_Model_SwordWave_Bullet"),
	TEXT("Prototype_Component_Model_Rock"),
	TEXT("Prototype_Component_Model_Tornado_1"),
	TEXT("Prototype_Component_Model_Tornado_2"),
	TEXT("Prototype_Component_Model_Triangle_Sword"),
	TEXT("Prototype_Component_Model_Blunt_Sword"),
	TEXT("Prototype_Component_Model_Bident_Spear"),
	TEXT("Prototype_Component_Model_Big_Sword"),
	TEXT("Prototype_Component_Model_Fatal_Rapier"),
	TEXT("Prototype_Component_Model_Sphere"),
	TEXT("Prototype_Component_Model_Lateral_Bullet"),
	TEXT("Prototype_Component_Model_Upper_Bullet"),
	TEXT("Prototype_Component_Model_Vertical_Bullet"),
	TEXT("Prototype_Component_Model_Phase_Two"),
	TEXT("Prototype_Component_Model_Sauron"),
	TEXT("Prototype_Component_Model_Sauron"),
	TEXT("Prototype_Component_Model_Devil"),
	TEXT("Prototype_Component_Model_Awl"),
	TEXT("Prototype_Component_Model_Mace_Bullet"),
	TEXT("Prototype_Component_Model_Mace_Bullet_Right"),
	TEXT("Prototype_Component_Model_Warp"),
	TEXT("Prototype_Component_Model_Tornado_2")
};

CUniversal_Bullet_Mesh::CUniversal_Bullet_Mesh(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CUniversal_Bullet_Mesh::CUniversal_Bullet_Mesh(const CUniversal_Bullet_Mesh & rhs)
	: CLandObject(rhs)
{
}

HRESULT CUniversal_Bullet_Mesh::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Universal_Bullet_MeshDesc, pArg, sizeof(UNIVERSAL_BULLET_MESHDESC));
	}
	else
	{
		MSGBOX("CItem_Universal에 pArg가 Nullptr입니다.");
	}

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = m_Universal_Bullet_MeshDesc.fSpeedPerSec;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	SetUp_Info();

	m_pNavigationCom->Find_CellIndex(m_pTransform);

	return S_OK;
}

_int CUniversal_Bullet_Mesh::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;
	m_dDeltaTime += TimeDelta;

	if (m_dDeltaTime > m_Universal_Bullet_MeshDesc.dDuration || m_iDead == OBJ_DEAD )
	{
		if (PHASE_TWO != m_Universal_Bullet_MeshDesc.iMeshNumber)
		{
			return OBJ_DEAD;
		}
	}
	Slow_On();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance->GetInstance()->Add_ObseverGroup(m_Universal_Bullet_MeshDesc.eType, this)))
	{
		MSGBOX("Monster_Bullet의 Add_ObseverGroup에 문제가 발생했습니다");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);


	SetUp_Fire(TimeDelta * m_dSlowTime);






	return OBJ_NOEVENT;
}

_int CUniversal_Bullet_Mesh::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	Renderer();


	return _int();
}

HRESULT CUniversal_Bullet_Mesh::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("CUniversal_Bullet_Mesh의 Render()에 문제가 발생했습니다.");
		return E_FAIL;
	}

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

HRESULT CUniversal_Bullet_Mesh::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_NonAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_pModel_BulletTag[m_Universal_Bullet_MeshDesc.iMeshNumber], TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	SetUp_Collider();

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

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

HRESULT CUniversal_Bullet_Mesh::SetUp_Collider()
{
	/* For.Com_Sphere */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = m_Universal_Bullet_MeshDesc.fColliderSphereScale;
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	ColliderDesc.vScale = m_Universal_Bullet_MeshDesc.fColliderOBBScale;
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::SetUp_Info()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (nullptr != m_Universal_Bullet_MeshDesc.pObject)
	{
		CTransform* pMonsterTransform = static_cast<CTransform*>(m_Universal_Bullet_MeshDesc.pObject->Get_Component(TEXT("Com_Transform")));

		m_pTransform->Scaled(XMLoadFloat3(&m_Universal_Bullet_MeshDesc.vScale));

		_vector		vPosition = pMonsterTransform->Get_State(CTransform::STATE_POSITION);

		vPosition += XMVector3Normalize(pMonsterTransform->Get_State(CTransform::STATE_RIGHT)) *m_Universal_Bullet_MeshDesc.fPositioning.x;
		vPosition += XMVector3Normalize(pMonsterTransform->Get_State(CTransform::STATE_UP)) * m_Universal_Bullet_MeshDesc.fPositioning.y;
		vPosition += XMVector3Normalize(pMonsterTransform->Get_State(CTransform::STATE_LOOK)) * m_Universal_Bullet_MeshDesc.fPositioning.z;

		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

		m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), 1);

		XMStoreFloat4(&m_fTempPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
		XMStoreFloat3(&m_fTempLook, m_pTransform->Get_State(CTransform::STATE_LOOK));

		//SetUp_Info에 몬스터의 룩을 넣어서 따라왔던거 같음
	}
	else {
		m_pTransform->Scaled(XMLoadFloat3(&m_Universal_Bullet_MeshDesc.vScale));
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_Universal_Bullet_MeshDesc.vTranslation));

		m_fTempPos = m_Universal_Bullet_MeshDesc.vTranslation;
		m_fTempLook = m_Universal_Bullet_MeshDesc.vLook;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::NavigationOn(_double TimeDelta)
{
	_float3 vDir;
	XMStoreFloat3(&vDir, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * m_pTransform->Get_TransformDesc().fSpeedPerSec * (_float)TimeDelta);

	if (false == m_pNavigationCom->Move_OnNavigation(m_pTransform->Get_State(CTransform::STATE_POSITION), &vDir))
	{
		m_iDead = OBJ_DEAD;
	}

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Renderer()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	switch (m_Universal_Bullet_MeshDesc.iMeshNumber)
	{
	case PHASE_TWO:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);
		}
		break;
	}
	case TORNADO_1:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case TORNADO_2:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case APPEAR:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case TRIANGLESWORD_BULLET:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case BLUNT_BULLET:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case BIDENT_SPEAR:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case BIG_SWORD:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case FATAL_RAPIER:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case SPHERE:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case LATERAL_BULLET:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case UPPER_BULLET:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case VERTICAL_BULLET:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
		}
		break;
	}
	case SAURON:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);
		}
		break;
	}
	case MACE_BULLET:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);
		}
		break;
	}
	case MACE_BULLET_RIGHT:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);
		}
		break;
	}
	case DEVIL:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 20.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
		}
		break;
	}
	default:
	{
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
		}
		break;
	}
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Devil_Out_Particle()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC Universal_PointDesc;
	ZeroMemory(&Universal_PointDesc, sizeof(CUniversal_Point_Instance::UNIVERSAL_POINT_INSTANCEDESC));

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::BLUE_ROCK_PARTICLE_0;
	Universal_PointDesc.iParticleType = CVIBuffer_Point_Instance::PARTICLE_FIRECRACKER;

	Universal_PointDesc.fMinSpeed = 4.f;
	Universal_PointDesc.fMaxSpeed = 12.f;
	Universal_PointDesc.fRange = _float3(1.f, 1.f, 1.f);
	Universal_PointDesc.fSize = 3;

	Universal_PointDesc.pObject = this;
	Universal_PointDesc.dDuration = 1.5;
	Universal_PointDesc.iTextureIndex = 0;
	Universal_PointDesc.fPositioning = _float3(0.f,15.f,0.f);

	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::BLUE_ROCK_PARTICLE_1;
	Universal_PointDesc.fSize = 3;
	Universal_PointDesc.fPositioning = _float3(0.f, 12.f, 0.f);
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::BLUE_ROCK_PARTICLE_2;
	Universal_PointDesc.fSize = 3;
	Universal_PointDesc.fPositioning = _float3(0.f, 9.f, 0.f);
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::BLUE_ROCK_PARTICLE_3;
	Universal_PointDesc.fSize = 3;
	Universal_PointDesc.fPositioning = _float3(0.f, 5.f, 0.f);
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::BLUE_ROCK_PARTICLE_1;
	Universal_PointDesc.fSize = 3;
	Universal_PointDesc.fPositioning = _float3(0.f, 10.f, 0.f);
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	Universal_PointDesc.iParticleTexture = CUniversal_Point_Instance::DEVIL_ROCK_PARTICLE_1;
	Universal_PointDesc.fSize = 1;
	Universal_PointDesc.fPositioning = _float3(0.f, 18.5f, 0.f);
	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Universal_Point_Instance"), &Universal_PointDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::SetUp_Fire(_double TimeDelta)
{
	switch (m_Universal_Bullet_MeshDesc.iMeshNumber)
	{
	case BURN_BULLET:
		NavigationOn(TimeDelta);
		Burn_Bullet(TimeDelta);
		break;
	case SUPPLEMENT_BULLET:
		NavigationOn(TimeDelta);
		Supplement_Bullet(TimeDelta);
		break;
	case METEORITE_BULLET:
		NavigationOn(TimeDelta);
		Meteorite_Bullet(TimeDelta);
		break;
	case TERRAIN_BULLET:
		Terrain_Bullet(TimeDelta);
		break;
	case SHOCKWAVE_BULLET:
		ShockWave_Bullet(TimeDelta);
		break;
	case SWORDWAVE_BULLET:
		NavigationOn(TimeDelta);
		SwordWave_Bullet(TimeDelta);
		break;
	case CRACK:
		Crack(TimeDelta);
		break;
	case TORNADO_1:
		Tornado_Shield(TimeDelta);
		break;
	case TORNADO_2:
		Tornado_Bullet(TimeDelta);
		break;
	case TRIANGLESWORD_BULLET:
		NavigationOn(TimeDelta);

		if (m_dDeltaTime > 5)
		{
			m_pTransform->Go_Straight(TimeDelta);

		}else {
			m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
		}

		m_dSoundTime += TimeDelta;
		if (m_dSoundTime >= 1.8)
		{
			GetSingle(CGameInstance)->PlaySound((L"Bear_Sword_Shake.mp3"), CHANNEL_OBJECT);

			m_dSoundTime = 0;
		}

		break;
	case BLUNT_BULLET:
		if (m_dDeltaTime > 4)
		{
			m_pTransform->Go_Straight(TimeDelta);

			//m_dSoundTime += TimeDelta;
			//if (m_dSoundTime >= 3)
			//{
			//	_float fSound_Dis = this->Get_CamDistance();
			//	fSound_Dis = fSound_Dis / 100;  //거리(소리나는위치 - 현재위치) / 최대거리
			//	GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1 - fSound_Dis);
			//	GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Meteorite_Straight.mp3"), CHANNEL_OBJECT);

			//	m_dSoundTime = 0;
			//}
		}else {
			m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
		}
		break;
	case BIDENT_SPEAR:
		if (m_dDeltaTime > 1)
		{
			m_pTransform->Go_Straight(TimeDelta);

			//m_dSoundTime += TimeDelta;
			//if (m_dSoundTime >= 3)
			//{
			//	_float fSound_Dis = this->Get_CamDistance();
			//	fSound_Dis = fSound_Dis / 100;  //거리(소리나는위치 - 현재위치) / 최대거리
			//	GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1 - fSound_Dis);
			//	GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Meteorite_Straight.mp3"), CHANNEL_OBJECT);

			//	m_dSoundTime = 0;
			//}
		}else {
			m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
		}
		break;
	case BIG_SWORD:
		if (m_dDeltaTime > 3){
			m_pTransform->Go_Straight(TimeDelta);

			//m_dSoundTime += TimeDelta;
			//if (m_dSoundTime >= 3)
			//{
			//	_float fSound_Dis = this->Get_CamDistance();
			//	fSound_Dis = fSound_Dis / 100;  //거리(소리나는위치 - 현재위치) / 최대거리
			//	GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1 - fSound_Dis);
			//	GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Meteorite_Straight.mp3"), CHANNEL_OBJECT);

			//	m_dSoundTime = 0;
			//}
		}else {
			m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
		}
		break;
	case FATAL_RAPIER:
		if (m_dDeltaTime > 2){
			m_pTransform->Go_Straight(TimeDelta);

			//m_dSoundTime += TimeDelta;
			//if (m_dSoundTime >= 3)
			//{
			//	_float fSound_Dis = this->Get_CamDistance();
			//	fSound_Dis = fSound_Dis / 100;  //거리(소리나는위치 - 현재위치) / 최대거리
			//	GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1 - fSound_Dis);
			//	GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Meteorite_Straight.mp3"), CHANNEL_OBJECT);

			//	m_dSoundTime = 0;
			//}

		}else {
			m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
		}
		break;
	case SPHERE:
		Sphere_Potal(TimeDelta);
		break;
	case LATERAL_BULLET:
		NavigationOn(TimeDelta);
		Lateral_Bullet(TimeDelta);
		break;
	case UPPER_BULLET:
		NavigationOn(TimeDelta);
		Upper_Bullet(TimeDelta);
		break;
	case VERTICAL_BULLET:
		NavigationOn(TimeDelta);
		Vertical_Bullet(TimeDelta);
		break;
	case PHASE_TWO:
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(65.f, 0.f, 65.f, 1.f));
		m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta *0.1);
		//m_pTransform->Scaling(10,250, TimeDelta);
		break;
	case REDSPHERE:
		Red_Sphere(TimeDelta);
		break;
	case SAURON:
		Sauron(TimeDelta);
		break;
	case DEVIL:
		Devil(TimeDelta);
		break;
	case AWL:
		Awl (TimeDelta);
		break;
	case MACE_BULLET:
		NavigationOn(TimeDelta);
		Mace_Bullet(TimeDelta);
		break;
	case MACE_BULLET_RIGHT:
		NavigationOn(TimeDelta);
		Mace_Bullet_Right(TimeDelta);
		break;
	case WARP:
		Warp(TimeDelta);
		break;
	case APPEAR:
		Appear(TimeDelta);
		break;
	default:
		MSGBOX("MeshType이 존재하지 않습니다.");
		break;
	}


	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Burn_Bullet(_double TimeDelta)
{
	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_Bullet_MeshDesc.fSpeedPerSec * TimeDelta;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Supplement_Bullet(_double TimeDelta)
{
	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_Bullet_MeshDesc.fSpeedPerSec * TimeDelta;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);
	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Meteorite_Bullet(_double TimeDelta)
{
	m_pTransform->Scaling(1,20, TimeDelta);

	//업 시키는 함수
	if (m_dDeltaTime < 1)
	{
		_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector vUp = m_pTransform->Get_State(CTransform::STATE_UP);

		vPosition += XMVector3Normalize(vUp) * (TimeDelta * 4);

		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);

		if (false == m_MeteoSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_MeteoriteUp.mp3"), CHANNEL_OBJECT);

			m_MeteoSwitch = true;
		}
	}
	else {
		m_pTransform->Go_Straight(TimeDelta,m_pNavigationCom);

		if (true == m_MeteoSwitch)
		{
			GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Meteorite_Straight.mp3"), CHANNEL_OBJECT);

			m_MeteoSwitch = false;
		}
	}

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Terrain_Bullet(_double TimeDelta)
{
	m_dFireTime += TimeDelta;

	//업 시키는 함수
	if (m_dDeltaTime < 1)
	{
		_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector vUp = m_pTransform->Get_State(CTransform::STATE_UP);

		vPosition += XMVector3Normalize(vUp) * m_Universal_Bullet_MeshDesc.fSpeedPerSec * TimeDelta;

		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		m_pTransform->LookAtNOTAxisY(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);

	}


	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::SwordWave_Bullet(_double TimeDelta)
{
	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_Bullet_MeshDesc.fSpeedPerSec * TimeDelta;

	vPosition = XMVectorSetY(vPosition, 0.f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::ShockWave_Bullet(_double TimeDelta)
{
	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Crack(_double TimeDelta)
{
	_vector vPosition = XMLoadFloat4(&m_fTempPos);

	//vPosition = XMVectorSetY(vPosition, 0.f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	if (true == m_bFire)
	{
		m_pTransform->LookAtNOTAxisY(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
		m_bFire = false;
	}
	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Tornado_Shield(_double TimeDelta)
{
	m_dEasingTime += TimeDelta * 0.1;
	m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_dEasingTime);

	m_pTransform->Scaling(1,52, TimeDelta);
	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Tornado_Bullet(_double TimeDelta)
{
	if (m_dDeltaTime <= 10)
	{
		m_dDeltaTime += TimeDelta;
		m_dEasingTime += TimeDelta * 0.03;
	}


	m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * 7);

	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_Bullet_MeshDesc.fSpeedPerSec * m_dEasingTime;

	vPosition = XMVectorSetY(vPosition, -2.f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Sphere_Potal(_double TimeDelta)
{
	m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta);

	//업 시키는 함수
	if (m_dDeltaTime < 2)
	{
		_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector vUp = m_pTransform->Get_State(CTransform::STATE_UP);

		vPosition += XMVector3Normalize(vUp) * (TimeDelta * 15);

		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
	}
	else {
		m_pTransform->Scaling(1, 90, TimeDelta);
	}

	m_dFireTime += TimeDelta;

	if (m_dFireTime > 5 && true == m_bFire)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		//==================트라이앵글 소드
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::TRIANGLESWORD_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 10.f;
		Universal_BulletMeshDesc.vScale = _float3(25.f, 25.f, 25.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 20;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, 15.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(1.f, 0.3f, 0.3f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		//==================브런트 소드
		//CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::BLUNT_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 30.f;
		Universal_BulletMeshDesc.vScale = _float3(10.f, 10.f, 10.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 20;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(-10.f, 10.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(2.5f, 2.5f, 2.5f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(2.f, 1.f, 0.5f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		//==================바이언트 스피어
		//CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::BIDENT_SPEAR;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 70.f;
		Universal_BulletMeshDesc.vScale = _float3(10.f, 10.f, 10.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 20;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(10.f, 10.f, -5.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(5.5f, 5.5f, 5.5f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(5.f, 1.f, 0.5f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		//==================빅 소드
		//CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::BIG_SWORD;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 20.f;
		Universal_BulletMeshDesc.vScale = _float3(10.f, 10.f, 10.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 20;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(10.f, 5.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(3.f, 3.f, 3.f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(2.f, 0.8f, 0.8f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		//==================페이탈 레이피어
		//CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::FATAL_RAPIER;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 50.f;
		Universal_BulletMeshDesc.vScale = _float3(10.f, 10.f, 10.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 20;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(-10.f, 5.f, 0.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(4.f, 4.f, 4.f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(3.5f, 1.f, 1.f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		m_bFire = false;
	}


	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Lateral_Bullet(_double TimeDelta)
{
	m_pTransform->Scaling(3, 2, TimeDelta);

	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_Bullet_MeshDesc.fSpeedPerSec * TimeDelta;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Upper_Bullet(_double TimeDelta)
{
	m_pTransform->Scaling(3, 2, TimeDelta);

	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_Bullet_MeshDesc.fSpeedPerSec * TimeDelta;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Vertical_Bullet(_double TimeDelta)
{
	m_pTransform->Scaling(3, 2, TimeDelta);

	_vector vPosition = XMLoadFloat4(&m_fTempPos);
	_vector vLook = XMLoadFloat3(&m_fTempLook);

	vPosition += XMVector3Normalize(vLook) * m_Universal_Bullet_MeshDesc.fSpeedPerSec * TimeDelta;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	XMStoreFloat4(&m_fTempPos, vPosition);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Red_Sphere(_double TimeDelta)
{
	m_pTransform->Scaling(1, 30, TimeDelta);

	if (m_dDeltaTime < 1.5)
	{
		_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);


		_vector vLook = -XMVector3Normalize((m_pPlayerTransform->Get_State(CTransform::STATE_POSITION) - vPos)) *XMVectorGetZ(m_pTransform->Get_Scale(CTransform::STATE_LOOK));

		_float3 Temp = _float3(0, 1, 0);
		_vector vRight = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&Temp), vLook)) *XMVectorGetX(m_pTransform->Get_Scale(CTransform::STATE_RIGHT));

		_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * XMVectorGetY(m_pTransform->Get_Scale(CTransform::STATE_UP));

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

	}
	else {
		m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
	}
	//m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Devil(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_dDeltaTime < 3)
	{
		_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector vUp = m_pTransform->Get_State(CTransform::STATE_UP);

		vPosition += XMVector3Normalize(vUp) * (TimeDelta * 10);

		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	}
	

	if (true == m_bFire && m_dDeltaTime > 5)
	{
		//악마의 눈동자 오른쪽 눈

		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::SAURON;
		Universal_BulletMeshDesc.eType = MONSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(5.f, 5.f, 5.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 100000;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(-2.f, -25.f, 3.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(0.1f, 0.1f, 0.1f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.07f, 0.07f, 0.07f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		//악마의 눈동자 왼쪽 눈
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::SAURON;
		Universal_BulletMeshDesc.eType = MONSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(7.f, 7.f, 7.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 100000;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(0.f, -24.f, 2.5f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(0.1f, 0.1f, 0.1f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.07f, 0.07f, 0.07f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;
	}

	m_dFireTime += TimeDelta;

	if (m_dFireTime > 7 && 0 == m_iFireCount)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::MACE_BULLET;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 15;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(-8.f, 9.f, 4.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(1.f, 1.f, 1.f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		m_dFireTime = 0;
		m_iFireCount += 1;
	}
	else if (m_dFireTime > 7 && 1 == m_iFireCount)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::MACE_BULLET_RIGHT;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 15;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(8.f, 9.f, 4.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(1.2f, 1.2f, 1.2f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(1.f, 1.f, 1.f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		m_dFireTime = 0;
		m_iFireCount = 0;
	}

	m_pTransform->LookAtNOTAxisY(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);

	m_dMovingTime += TimeDelta;


	switch (m_iMovingCount)
	{
	case 0:
		m_pTransform->Go_Down(TimeDelta*0.5);
		m_pTransform->Go_Backward(TimeDelta * 0.01);
		break;
	case 1:
		m_pTransform->Go_Up(TimeDelta * 0.5);
		m_pTransform->Go_Straight(TimeDelta * 0.01);
		break;
	default:
		break;
	}

	if (m_dMovingTime > 1.5)
	{
		m_iMovingCount += 1;
		m_dMovingTime = 0;

		if (m_iMovingCount > 1)
		{
			m_iMovingCount = 0;
		}
	}
	
	CUI_Universal* Hp = static_cast<CUI_Universal*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Hp")));

	if (0 >= Hp->Get_SizeX()) //이거 겟 사이즈 X 받아올 때 ui 체력바 삭제 시키는바람에 호출이 안됨 그래서 체력바 m_idead를 없애버림 즉, 없어진척함
	{
		Devil_Out_Particle();
		m_iDead = OBJ_DEAD;
	}

	//m_iDead = m_Universal_Bullet_MeshDesc.pObject->Get_Dead();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Sauron(_double TimeDelta)
{
	
	m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);

	m_iDead = m_Universal_Bullet_MeshDesc.pObject->Get_Dead();


	//////여기서부턴 소환용
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_dFireTime += TimeDelta;

	if (true == m_bFire)
	{

		//송곳
		CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC Universal_BulletMeshDesc;
		ZeroMemory(&Universal_BulletMeshDesc, sizeof(CUniversal_Bullet_Mesh::UNIVERSAL_BULLET_MESHDESC));

		Universal_BulletMeshDesc.iMeshNumber = CUniversal_Bullet_Mesh::MESHTYPE::AWL;
		Universal_BulletMeshDesc.eType = BULLET_MOSTER;

		Universal_BulletMeshDesc.fSpeedPerSec = 40.f;
		Universal_BulletMeshDesc.vScale = _float3(100.f, 100.f, 100.f);
		Universal_BulletMeshDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		Universal_BulletMeshDesc.dDuration = 5;

		Universal_BulletMeshDesc.pObject = this;
		Universal_BulletMeshDesc.fPositioning = _float3(9999.f, 9999.f, 9999.f);


		Universal_BulletMeshDesc.fColliderSphereScale = _float3(0.1f, 0.1f, 0.1f);
		Universal_BulletMeshDesc.fColliderOBBScale = _float3(0.07f, 0.07f, 0.02f);

		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Monster_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet_Mesh"), &Universal_BulletMeshDesc)))
			return E_FAIL;

		m_bFire = false;
	}

	if (m_dFireTime > 1)
	{
		m_dFireTime = 0;
		m_bFire = true;
	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Awl(_double TimeDelta)
{

	if (true == m_bFire)
	{

		m_pTransform->Rotation(XMVectorSet(1.f,0.f,0.f,0.f), XMConvertToRadians(90));


		_vector vPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);

		_float4 fTempPos;
		XMStoreFloat4(&fTempPos, vPos);

		_int Random1 = rand() % 20;
		_int Random2 = rand() % 20;

		if (0 == Random1 % 2)
		{
			Random1 *= -1;
		}

		if (0 == Random2 % 2)
		{
			Random2 *= -1;
		}


		fTempPos.x += Random1 + (0.1*Random2);
		fTempPos.y = fTempPos.y -12;
		fTempPos.z += Random2 + (0.1*Random1);



		m_pTransform->Set_State(CTransform::STATE_POSITION,XMLoadFloat4(&fTempPos));



		_uint	RandomRotation = rand() % 360;

		_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), RandomRotation);

		_vector		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
		_vector		vUp = m_pTransform->Get_State(CTransform::STATE_UP);
		_vector		vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

		vRight = XMVector4Transform(vRight, RotationMatrix);
		vUp = XMVector4Transform(vUp, RotationMatrix);
		vLook = XMVector4Transform(vLook, RotationMatrix);

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);


		m_bFire = false;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")))->Start_CameraQuake(2, 0.5f);
		RELEASE_INSTANCE(CGameInstance);

		//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 0.5);
		GetSingle(CGameInstance)->PlaySound((L"Devil_Awl.mp3"), CHANNEL_OBJECT);
	}

	m_dFireTime += TimeDelta;
	if (false == m_bFire && m_dFireTime <0.4)
	{
		_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector vUp = m_pTransform->Get_State(CTransform::STATE_LOOK);

		vPosition -= XMVector3Normalize(vUp) * (TimeDelta * 30);

		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		//m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
	}

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Mace_Bullet(_double TimeDelta)
{
	m_pTransform->Scaling(2,4,TimeDelta);

	if (m_dMovingTime < 7)
	{
		m_dMovingTime += TimeDelta;

		switch (m_iMovingCount)
		{
		case 0:
			m_pTransform->Go_Right(TimeDelta*0.05);
			m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
			break;
		case 1:
			m_pTransform->Go_Left(TimeDelta * 0.05);
			m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
			break;
		default:
			break;
		}

		if (m_dMovingTime > 1)
		{
			m_iMovingCount += 1;

			if (m_iMovingCount > 1)
			{
				m_iMovingCount = 0;
			}
		}
	}

	if (m_dDeltaTime > 7)
	{
		m_pTransform->Go_Straight(TimeDelta);

		if (false == m_bSoundSwitch)
		{
			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
			GetSingle(CGameInstance)->PlaySound((L"Devil_Mace.mp3"), CHANNEL_OBJECT);
			m_bSoundSwitch = true;
		}
	}

	m_iDead = m_Universal_Bullet_MeshDesc.pObject->Get_Dead();

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Mace_Bullet_Right(_double TimeDelta)
{
	m_pTransform->Scaling(2, 4, TimeDelta);

	if (m_dMovingTime < 7)
	{
		m_dMovingTime += TimeDelta;

		switch (m_iMovingCount)
		{
		case 0:
			m_pTransform->Go_Down(TimeDelta*0.5);
			m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
			break;
		case 1:
			m_pTransform->Go_Up(TimeDelta * 0.5);
			m_pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), TimeDelta);
			break;
		default:
			break;
		}

		if (m_dMovingTime > 1)
		{
			m_iMovingCount += 1;

			if (m_iMovingCount > 1)
			{
				m_iMovingCount = 0;
			}
		}
	}


	if (m_dDeltaTime > 7)
	{
		m_pTransform->Go_Straight(TimeDelta);

		if (false == m_bSoundSwitch)
		{
			//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_OBJECT, 1);
			GetSingle(CGameInstance)->PlaySound((L"Devil_Mace.mp3"), CHANNEL_OBJECT);
			m_bSoundSwitch = true;
		}

	}

	m_iDead = m_Universal_Bullet_MeshDesc.pObject->Get_Dead();

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Warp(_double TimeDelta)
{
	switch (m_eNowLevelNum)
	{
	case LEVEL_STAGE1:
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION,XMVectorSet(109.5f,1.f,114.f,1.f));

		m_iRotation += 1;
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180));

		_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), m_iRotation * 0.01);

		_vector		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
		_vector		vUp = m_pTransform->Get_State(CTransform::STATE_UP);
		_vector		vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

		vRight = XMVector4Transform(vRight, RotationMatrix);
		vUp = XMVector4Transform(vUp, RotationMatrix);
		vLook = XMVector4Transform(vLook, RotationMatrix);

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
		break;
	}
	case LEVEL_STAGE1_BOSSROOM:
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(88.5f, 1.f, 88.f, 1.f));

		m_iRotation += 1;
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180));

		_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), m_iRotation * 0.01);

		_vector		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
		_vector		vUp = m_pTransform->Get_State(CTransform::STATE_UP);
		_vector		vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

		vRight = XMVector4Transform(vRight, RotationMatrix);
		vUp = XMVector4Transform(vUp, RotationMatrix);
		vLook = XMVector4Transform(vLook, RotationMatrix);

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
		break;
	}
	case LEVEL_STAGE2:
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(124.f, 1.f, 106.f, 1.f));

		m_iRotation += 1;
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90));

		_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), m_iRotation * 0.01);

		_vector		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
		_vector		vUp = m_pTransform->Get_State(CTransform::STATE_UP);
		_vector		vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

		vRight = XMVector4Transform(vRight, RotationMatrix);
		vUp = XMVector4Transform(vUp, RotationMatrix);
		vLook = XMVector4Transform(vLook, RotationMatrix);

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
		break;
	}
	default:
		break;
	}

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Appear(_double TimeDelta)
{
	m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta);

	//m_pTransform->Scaling_Small(5,8, TimeDelta);

	return S_OK;
}

HRESULT CUniversal_Bullet_Mesh::Level_Change()
{

	switch (m_eNowLevelNum)
	{
	case LEVEL_STAGE1:
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CLevel* TempLevel = pGameInstance->Get_Level();
		TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE1_BOSSROOM);

		RELEASE_INSTANCE(CGameInstance);
		break;
	}
	case LEVEL_STAGE1_BOSSROOM:
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CLevel* TempLevel = pGameInstance->Get_Level();
		TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE2);

		RELEASE_INSTANCE(CGameInstance);
		break;
	}
	case LEVEL_STAGE2:
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CLevel* TempLevel = pGameInstance->Get_Level();
		TempLevel->Level_InGame_Chage(true, LEVEL::LEVEL_STAGE2_BOSSROOM);

		RELEASE_INSTANCE(CGameInstance);
		break;
	}
	default:
		break;
	}

	return S_OK;

}

HRESULT CUniversal_Bullet_Mesh::Slow_On()
{
	if (nullptr != m_pTempSlow_Bullet && m_fSlowBulletDistance < 200)
	{
		CTransform* Slow_Bullet_Transform = static_cast<CTransform*>(m_pTempSlow_Bullet->Get_Component(TEXT("Com_Transform")));
		_vector vDis = m_pTransform->Get_Distance(Slow_Bullet_Transform->Get_State(CTransform::STATE_POSITION));

		XMStoreFloat(&m_fSlowBulletDistance, vDis);
	}


	if (m_fSlowBulletDistance < 100)
	{
		if (m_pSphereCom->Collision_Sphere(m_pTempSlow_Bullet->m_pSphereCom))
		{
			m_dSlowTime = 0.1;
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

_int CUniversal_Bullet_Mesh::Obsever_Hit(void * pArg)
{
	CLandObject* SorceObject = static_cast<CLandObject*>(pArg);

	if (nullptr != SorceObject)//몬스터들의 탄환은 이걸로 계산됨
	{
		switch (m_Universal_Bullet_MeshDesc.iMeshNumber)
		{
		case BURN_BULLET:
			SorceObject->m_Info.fHp -= 30;
			break;
		case SUPPLEMENT_BULLET:
			SorceObject->m_Info.fHp -= 20;
			break;
		case METEORITE_BULLET:
			SorceObject->m_Info.fHp -= 50;
			break;
		case TERRAIN_BULLET:
			SorceObject->m_Info.fHp -= 20;
			break;
		case SWORDWAVE_BULLET:
			SorceObject->m_Info.fHp -= 30;
			break;
		case TORNADO_2:
			SorceObject->m_Info.fHp -= 50;
			break;
		case TRIANGLESWORD_BULLET:
			SorceObject->m_Info.fHp -= 50;
			break;
		case BLUNT_BULLET:
			SorceObject->m_Info.fHp -= 20;
			break;
		case BIDENT_SPEAR:
			SorceObject->m_Info.fHp -= 20;
			break;
		case BIG_SWORD:
			SorceObject->m_Info.fHp -= 30;
			break;
		case FATAL_RAPIER:
			SorceObject->m_Info.fHp -= 20;
			break;
		case LATERAL_BULLET:
			SorceObject->m_Info.fHp -= 20;
			break;
		case UPPER_BULLET:
			SorceObject->m_Info.fHp -= 20;
			break;
		case VERTICAL_BULLET:
			SorceObject->m_Info.fHp -= 20;
			break;
		case AWL:
			SorceObject->m_Info.fHp -= 20;
			break;
		case MACE_BULLET:
			SorceObject->m_Info.fHp -= 30;
			break;
		case WARP:
			Level_Change();
			break;
		}
	}

	return _int();
}

_int CUniversal_Bullet_Mesh::Obsever_Event(void * pArg)
{
	CUniversal_Bullet* SorceBullet = static_cast<CUniversal_Bullet*>(pArg);

	if (nullptr != SorceBullet)
	{
		switch (SorceBullet->m_Universal_BulletDesc.iBulletNumber)
		{
		case CUniversal_Bullet::SLOWTIME_SKILL:
		{
			m_pTempSlow_Bullet = SorceBullet;
			m_dSlowTime = 0.05;
			break;
		}
		default:
			break;
		}
	}

	return _int();
}

_int CUniversal_Bullet_Mesh::Dead_Trigger()
{
	switch (m_Universal_Bullet_MeshDesc.iMeshNumber)
	{
	case METEORITE_BULLET:
	{
		GetSingle(CGameInstance)->PlaySound((L"Dokkaebi_Meteorite.mp3"), CHANNEL_OBJECT);

		break;
	}
	case DEVIL:
	{
		GetSingle(CGameInstance)->PlaySound((L"Devil_Dead.mp3"), CHANNEL_OBJECT);

		break;
	}
	}
	return _int();
}
CUniversal_Bullet_Mesh * CUniversal_Bullet_Mesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUniversal_Bullet_Mesh*	pInstance = new CUniversal_Bullet_Mesh(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CUniversal_Bullet_Mesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUniversal_Bullet_Mesh::Clone(void * pArg)
{
	CUniversal_Bullet_Mesh*	pInstance = new CUniversal_Bullet_Mesh(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CUniversal_Bullet_Mesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUniversal_Bullet_Mesh::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
