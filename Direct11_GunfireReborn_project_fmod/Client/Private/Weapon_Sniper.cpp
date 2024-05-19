#include "stdafx.h"
#include "..\Public\Weapon_Sniper.h"
#include "GameInstance.h"


CWeapon_Sniper::CWeapon_Sniper(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CWeapon(pDevice, pDeviceContext)
{

}

CWeapon_Sniper::CWeapon_Sniper(const CWeapon_Sniper & rhs)
	: CWeapon(rhs)
{
}

HRESULT CWeapon_Sniper::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Sniper::NativeConstruct(void * pArg)
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
	m_pGreenMagazine = static_cast<CUI_Bullet_Number*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_GreenMagazine")));

	//무기 확인용
	m_pTransform->Set_State(CTransform::STATE_POSITION, OUT_POSITION);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CWeapon_Sniper::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;


	return _int();
}

_int CWeapon_Sniper::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (true == m_bTransformOn)
	{
		SetUp_Motion(TimeDelta);
	}

	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_iAnimation, m_iNextAnimation, m_bOneTimeSwith);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CWeapon_Sniper::Render()
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

HRESULT CWeapon_Sniper::Set_WeaponPos()
{
	m_pTransform->Set_State(CTransform::STATE_RIGHT, m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_UP, m_pPlayerTransformCom->Get_State(CTransform::STATE_UP));
	m_pTransform->Set_State(CTransform::STATE_LOOK, m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK));

	_vector vPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);

	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT)) * 0.6f;
	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_UP)) * -0.5f;
	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK)) * 0.2f;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	return S_OK;
}

_bool CWeapon_Sniper::SetUp_Zoom()
{
	return m_bZoom;
}

HRESULT CWeapon_Sniper::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Sniper"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;




	return S_OK;
}

HRESULT CWeapon_Sniper::SetUp_ConstantTable()
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

HRESULT CWeapon_Sniper::SetUp_Motion(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_R)& DIS_Down && false == m_bZoom && false == m_bOneTimeSwith && true == m_pGreenMagazine->Get_CountingNumberOn())
	{
		m_iAnimation = 2;
		m_iNextAnimation = 1;
		m_bOneTimeSwith = true;

		m_iNumber[m_iSlotNumber] = 5;

		m_pGreenMagazine->Set_Magazine(-1);


		GetSingle(CGameInstance)->PlaySound((L"SniperReload.mp3"), CHANNEL_EFFECT);

	}
	else if (pGameInstance->Get_DIKeyState(DIK_R)& DIS_Down && false == m_bOneTimeSwith && false == m_pGreenMagazine->Get_CountingNumberOn())
	{
		//flase면 탄창이 없습니다 어쩌고 사운드 ㄱㄱㄱ + 진동

		GetSingle(CGameInstance)->PlaySound((L"Embarrassment.mp3"), CHANNEL_EFFECT);

		m_pCamera_Player->Start_CameraQuake(3, 0.3f);
	}



	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down && true == m_bZoom && false == m_bOneTimeSwith && 0 != m_iNumber[m_iSlotNumber])
	{
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Bullet_Sniper"), TEXT("Prototype_GameObject_Bullet_Sniper"))))
			return E_FAIL;

		m_iAnimation = 0;
		m_iNextAnimation = 1;
		m_bOneTimeSwith = true;

		m_iNumber[m_iSlotNumber] -= 1;


		GetSingle(CGameInstance)->PlaySound((L"SniperShot.mp3"), CHANNEL_EFFECT);

	}
	else if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down && true == m_bZoom && 0 == m_iNumber[m_iSlotNumber])
	{
		m_pCamera_Player->Start_CameraQuake(20, 0.3f);

		GetSingle(CGameInstance)->PlaySound((L"NotSniperBullet.mp3"), CHANNEL_EFFECT);
	}
	else if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down && false == m_bZoom && 0 == m_iNumber[m_iSlotNumber])
	{
		m_pCamera_Player->Start_CameraQuake(3, 0.3f);

		GetSingle(CGameInstance)->PlaySound((L"NotSniperBullet.mp3"), CHANNEL_EFFECT);
	}

	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Press)
	{
		m_bZoom = true;
	}
	else {
		m_bZoom = false;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CWeapon_Sniper * CWeapon_Sniper::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CWeapon_Sniper*	pInstance = new CWeapon_Sniper(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CWeapon_Sniper");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWeapon_Sniper::Clone(void * pArg)
{
	CWeapon_Sniper*	pInstance = new CWeapon_Sniper(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CWeapon_Sniper");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWeapon_Sniper::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
