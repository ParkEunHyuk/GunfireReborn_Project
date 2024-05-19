#include "stdafx.h"
#include "..\Public\Bullet_Pistol.h"
#include "GameInstance.h"
#include "Universal_TextureEffect.h"

CBullet_Pistol::CBullet_Pistol(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CBullet(pDevice, pDeviceContext)
{

}

CBullet_Pistol::CBullet_Pistol(const CBullet_Pistol & rhs)
	: CBullet(rhs)
{

}

HRESULT CBullet_Pistol::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet_Pistol::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 20.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pWeaponTransformCom = static_cast<CTransform*>(pGameInstance->GetInstance()->Get_Component(m_eNowLevelNum, TEXT("Layer_Weapon_Pistol"), TEXT("Com_Transform")));

	RELEASE_INSTANCE(CGameInstance);

	m_pTransform->Scaled(XMVectorSet(0.3f, 0.3f, 0.3f, 0.0f));

	SetUp_BulletPos();

	m_pNavigationCom->Find_CellIndex(m_pTransform);

	return S_OK;
}


_int CBullet_Pistol::Tick(_double TimeDelta)
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
		MSGBOX("Bullet_Pistol의 Add_ObseverGroup에 문제가 발생했습니다");
		return E_FAIL;
	}



	m_pTransform->Go_Straight(TimeDelta);

	_float3 vDir;
	XMStoreFloat3(&vDir, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * m_pTransform->Get_TransformDesc().fSpeedPerSec * (_float)TimeDelta);

	if (false == m_pNavigationCom->Move_OnNavigation(m_pTransform->Get_State(CTransform::STATE_POSITION), &vDir))
	{
		m_iDead = OBJ_DEAD;
	}


	RELEASE_INSTANCE(CGameInstance);

	return OBJ_NOEVENT;
}

_int CBullet_Pistol::LateTick(_double TimeDelta)
{

	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	//m_pSphereCom->Collision_Sphere((CCollider*)pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_Monster"), TEXT("Com_Sphere")));


	RELEASE_INSTANCE(CGameInstance);

#ifdef _DEBUG	
	m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif // _DEBUG


	return _int();
}

HRESULT CBullet_Pistol::Render()
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

HRESULT CBullet_Pistol::SetUp_BulletPos()
{
	_vector vRight = m_pWeaponTransformCom->Get_State(CTransform::STATE_RIGHT);
	_vector vUp = m_pWeaponTransformCom->Get_State(CTransform::STATE_UP);
	_vector vLook = m_pWeaponTransformCom->Get_State(CTransform::STATE_LOOK);

	vRight = m_pTransform->Get_Scale(CTransform::STATE_RIGHT) * XMVector3Normalize(vRight);
	vUp = m_pTransform->Get_Scale(CTransform::STATE_UP) * XMVector3Normalize(vUp);
	vLook = m_pTransform->Get_Scale(CTransform::STATE_LOOK) * XMVector3Normalize(vLook);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

	_vector		vPosition = m_pWeaponTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition -= XMVector3Normalize(m_pWeaponTransformCom->Get_State(CTransform::STATE_RIGHT)) *0.6;
	vPosition += XMVector3Normalize(m_pWeaponTransformCom->Get_State(CTransform::STATE_UP)) * 0.7;
	vPosition += XMVector3Normalize(m_pWeaponTransformCom->Get_State(CTransform::STATE_LOOK)) *0.08;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CBullet_Pistol::Hit_Effect()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC Universal_EffectDesc;
	ZeroMemory(&Universal_EffectDesc, sizeof(CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC));

	Universal_EffectDesc.iEffectNumber = CUniversal_TextureEffect::EFFECTTYPE::PISTOL_EFFECT;

	Universal_EffectDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	Universal_EffectDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_EffectDesc.dDuration = 0.2;

	Universal_EffectDesc.pObject = this;
	Universal_EffectDesc.fPositioning = _float3(0.f, 0.f, -1.f);

	Universal_EffectDesc.iTextureIndex = 15;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CBullet_Pistol::Obsever_Hit(void * pArg)
{
	CLandObject* DestObject = static_cast<CLandObject*>(pArg);

	DestObject->m_Info.fHp -= PISTOL_DAMAGE;

	Hit_Effect();

	GetSingle(CGameInstance)->PlaySound((L"Bullet_Pistol.mp3"), CHANNEL_EFFECT);

	return _int();
}

_int CBullet_Pistol::Obsever_Sound(void * pArg)
{
	Hit_Effect();

	GetSingle(CGameInstance)->PlaySound((L"Bullet_Pistol.mp3"), CHANNEL_EFFECT);

	return _int();
}

_int CBullet_Pistol::Dead_Trigger()
{

	return _int();
}

HRESULT CBullet_Pistol::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_NonAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bullet_Pistol"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
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
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
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

HRESULT CBullet_Pistol::SetUp_ConstantTable()
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

CBullet_Pistol * CBullet_Pistol::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBullet_Pistol*	pInstance = new CBullet_Pistol(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CBullet_Pistol");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBullet_Pistol::Clone(void * pArg)
{
	CBullet_Pistol*	pInstance = new CBullet_Pistol(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CBullet_Pistol");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_Pistol::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}