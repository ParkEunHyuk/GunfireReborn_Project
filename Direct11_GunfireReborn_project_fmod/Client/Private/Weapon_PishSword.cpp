#include "stdafx.h"
#include "..\Public\Weapon_PishSword.h"
#include "GameInstance.h"

CWeapon_PishSword::CWeapon_PishSword(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CWeapon(pDevice, pDeviceContext)
{

}

CWeapon_PishSword::CWeapon_PishSword(const CWeapon_PishSword & rhs)
	: CWeapon(rhs)
{
}

HRESULT CWeapon_PishSword::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_PishSword::NativeConstruct(void * pArg)
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

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransformCom = static_cast<CTransform*>(pGameInstance->GetInstance()->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));

	//무기 와꾸 확인용
	m_pTransform->Set_State(CTransform::STATE_POSITION,OUT_POSITION);

	RELEASE_INSTANCE(CGameInstance);

	//SetUp_Trail();

	//m_pTrail->Set_NotRendering(false);

	return S_OK;
}

_int CWeapon_PishSword::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return _int();
}

_int CWeapon_PishSword::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (true == m_bTransformOn)
	{
		SetUp_Motion(TimeDelta);
		Attack(TimeDelta);
	}



	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_iAnimation, m_iNextAnimation, m_bOneTimeSwith);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CWeapon_PishSword::Render()
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

HRESULT CWeapon_PishSword::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_PishSword"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWeapon_PishSword::SetUp_ConstantTable()
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

HRESULT CWeapon_PishSword::SetUp_Motion(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down && false == m_bOneTimeSwith)
	{
		//if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Bullet_Energy"), TEXT("Prototype_GameObject_Bullet_Energy"))))
		//	return E_FAIL;

		m_iAnimation = 1;
		m_iNextAnimation = 0;
		m_bOneTimeSwith = true;

		GetSingle(CGameInstance)->PlaySound((L"Weapon_PishSword.mp3"), CHANNEL_EFFECT);

	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

//HRESULT CWeapon_PishSword::SetUp_Trail()
//{
//	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
//
//	CTrail::TRAILDESC TrailDesc;
//	TrailDesc.iTrailTextureIndex = CTrail::UNIVERSAL_TRAIL;
//	//TrailDesc.pTransformMatrix = &m_TransformMatrix;
//
//	TrailDesc.pModel = m_pModelCom;
//	TrailDesc.pSocketName = "Bip001 R Hand";
//	TrailDesc.pTransform = m_pTransform;
//	TrailDesc.pObject = this;
//
//
//	//TrailDesc.vHighPos = _float3(0.f, 0.f, 1.05f);
//	//TrailDesc.vLowPos = _float3(0.0f, 0.f, 0.15f);
//	//TrailDesc.vColor = _float4(138.f / 255.f, 3.f / 255.f, 3.f / 255.f, 1.f);
//
//
//	TrailDesc.vHighPos = _float3(-5.f, 15.f, 15.f);
//	TrailDesc.vLowPos = _float3(0.f, 0.f, 0.f);
//
//	TrailDesc.vColor = _float4(1.f / 255.f, 255.f / 175.f, 255.f / 255.f, 1.f);
//
//	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Trail"), TEXT("Prototype_GameObject_Trail"), &TrailDesc)))
//		return E_FAIL;
//
//	m_pTrail = static_cast<CTrail*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Trail"))); //이거 레이어 이름 다 다르게 해야함 ㅋㅋㅋ
//
//	RELEASE_INSTANCE(CGameInstance);
//
//	return S_OK;
//}

HRESULT CWeapon_PishSword::Set_WeaponPos()
{
	m_pTransform->Set_State(CTransform::STATE_RIGHT, m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_UP, m_pPlayerTransformCom->Get_State(CTransform::STATE_UP));
	m_pTransform->Set_State(CTransform::STATE_LOOK, m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK));

	_vector vPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);

	//vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT)) * 1;
	//vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_UP)) * -1;
	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK)) * 0.2f;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	return S_OK;
}

HRESULT CWeapon_PishSword::Attack(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == m_bOneTimeSwith)
	{
		m_dAttackTime += TimeDelta;
		if (m_dAttackTime > 0.2 && true == m_bAttack)
		{
			CUniversal_Bullet::UNIVERSAL_BULLETDESC Universal_BulletDesc;
			ZeroMemory(&Universal_BulletDesc, sizeof(CUniversal_Bullet::UNIVERSAL_BULLETDESC));

			Universal_BulletDesc.iBulletNumber = CUniversal_Bullet::BULLETTYPE::NONTEXTURE;
			Universal_BulletDesc.eType = BULLET;

			Universal_BulletDesc.fSpeedPerSec = 0.f;
			Universal_BulletDesc.vScale = _float3(1.f, 1.f, 1.f);
			Universal_BulletDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
			Universal_BulletDesc.dDuration = 0.1f;

			Universal_BulletDesc.pObject = this;
			Universal_BulletDesc.fPositioning = _float3(0, 0, 1.0f);

			Universal_BulletDesc.fColliderSphereScale = _float3(1.5f, 1.5f, 1.5f);
			Universal_BulletDesc.fColliderOBBScale = _float3(1.2f, 1.2f, 1.2f);

			Universal_BulletDesc.iTextureIndex = 0;

			if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_WeaponPishSword_Bullet"), TEXT("Prototype_GameObject_Universal_Bullet"), &Universal_BulletDesc)))
				return E_FAIL;

			m_bAttack = false;
		}
	}
	else
	{
		m_dAttackTime = 0;
		m_bAttack = true;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CWeapon_PishSword * CWeapon_PishSword::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CWeapon_PishSword*	pInstance = new CWeapon_PishSword(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CWeapon_PishSword");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWeapon_PishSword::Clone(void * pArg)
{
	CWeapon_PishSword*	pInstance = new CWeapon_PishSword(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CWeapon_PishSword");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWeapon_PishSword::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
