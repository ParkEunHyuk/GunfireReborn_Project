#include "stdafx.h"
#include "..\Public\Weapon_Pistol.h"
#include "GameInstance.h"
#include "Universal_TextureEffect.h"


CWeapon_Pistol::CWeapon_Pistol(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CWeapon(pDevice, pDeviceContext)
{

}

CWeapon_Pistol::CWeapon_Pistol(const CWeapon_Pistol & rhs)
	: CWeapon(rhs)
{
}

HRESULT CWeapon_Pistol::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Pistol::NativeConstruct(void * pArg)
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

	m_pTransform->Set_State(CTransform::STATE_POSITION, OUT_POSITION);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CWeapon_Pistol::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;


	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_iAnimation, m_iNextAnimation, m_bOneTimeSwith);

	return _int();
}

_int CWeapon_Pistol::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (true == m_bTransformOn) //이걸로 총 상호작용 설정하는것
	{
		SetUp_Motion(TimeDelta);
	}


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CWeapon_Pistol::Render()
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

HRESULT CWeapon_Pistol::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Pistol"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;




	return S_OK;
}

HRESULT CWeapon_Pistol::SetUp_ConstantTable()
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

HRESULT CWeapon_Pistol::SetUp_Motion(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Get_DIKeyState(DIK_R)& DIS_Down && false == m_bOneTimeSwith)
	{
		m_iAnimation = 2;
		m_iNextAnimation = 1;
		m_bOneTimeSwith = true;

		m_iNumber[m_iSlotNumber] = 8;

		GetSingle(CGameInstance)->PlaySound((L"PistolReload.mp3"), CHANNEL_EFFECT);

		m_SoundTime = 0;
		m_SoundOn = true;

	}

	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down && false == m_bOneTimeSwith && 0 != m_iNumber[m_iSlotNumber])
	{
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Bullet_Pistol"), TEXT("Prototype_GameObject_Bullet_Pistol"))))
			return E_FAIL;

		m_iAnimation = 0;
		m_iNextAnimation = 1;
		m_bOneTimeSwith = true;

		m_iNumber[m_iSlotNumber] -= 1;

		Muzzle_Effect();

		GetSingle(CGameInstance)->PlaySound((L"PistolShot.mp3"), CHANNEL_EFFECT);
	}
	else if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down && 0 == m_iNumber[m_iSlotNumber])
	{
		m_pCamera_Player->Start_CameraQuake(3,0.3f);

		GetSingle(CGameInstance)->PlaySound((L"NotPistolBullet.mp3"), CHANNEL_EFFECT);
	}


	RELEASE_INSTANCE(CGameInstance);


	m_SoundTime += TimeDelta;

	if (true == m_SoundOn && m_SoundTime >= 0.6)
	{
		GetSingle(CGameInstance)->PlaySound((L"PistolReloadSuccess.mp3"), CHANNEL_EFFECT);

		m_SoundOn = false;
	}

	return S_OK;
}

HRESULT CWeapon_Pistol::Set_WeaponPos()
{
	m_pTransform->Set_State(CTransform::STATE_RIGHT, m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_UP, m_pPlayerTransformCom->Get_State(CTransform::STATE_UP));
	m_pTransform->Set_State(CTransform::STATE_LOOK, m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK));

	_vector vPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);

	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT)) * 1.f;
	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_UP)) * -1.f;
	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK)) * 1.6f;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	return S_OK;
}

HRESULT CWeapon_Pistol::Muzzle_Effect()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC Universal_EffectDesc;
	ZeroMemory(&Universal_EffectDesc, sizeof(CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC));

	Universal_EffectDesc.iEffectNumber = CUniversal_TextureEffect::EFFECTTYPE::PISTOL_MUZZLE_EFFECT;

	Universal_EffectDesc.vScale = _float3(1.2f, 1.2f, 1.2f);
	Universal_EffectDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_EffectDesc.dDuration = 0.05;

	Universal_EffectDesc.pObject = this;
	Universal_EffectDesc.fPositioning = _float3(-0.35f, 0.615f, 0.f);

	Universal_EffectDesc.iTextureIndex = 0;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CWeapon_Pistol * CWeapon_Pistol::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CWeapon_Pistol*	pInstance = new CWeapon_Pistol(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CWeapon_Pistol");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWeapon_Pistol::Clone(void * pArg)
{
	CWeapon_Pistol*	pInstance = new CWeapon_Pistol(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CWeapon_Pistol");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWeapon_Pistol::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
