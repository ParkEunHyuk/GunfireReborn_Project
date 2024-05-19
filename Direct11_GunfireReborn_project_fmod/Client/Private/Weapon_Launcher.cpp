#include "stdafx.h"
#include "..\Public\Weapon_Launcher.h"
#include "GameInstance.h"


CWeapon_Launcher::CWeapon_Launcher(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CWeapon(pDevice, pDeviceContext)
{

}

CWeapon_Launcher::CWeapon_Launcher(const CWeapon_Launcher & rhs)
	: CWeapon(rhs)
{
}

HRESULT CWeapon_Launcher::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Launcher::NativeConstruct(void * pArg)
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
	m_pBlueMagazine = static_cast<CUI_Bullet_Number*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_BlueMagazine")));

	m_pTransform->Set_State(CTransform::STATE_POSITION, OUT_POSITION);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CWeapon_Launcher::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;


	return _int();
}

_int CWeapon_Launcher::LateTick(_double TimeDelta)
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

HRESULT CWeapon_Launcher::Render()
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

HRESULT CWeapon_Launcher::Set_WeaponPos()
{
	m_pTransform->Set_State(CTransform::STATE_RIGHT, m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_UP, m_pPlayerTransformCom->Get_State(CTransform::STATE_UP));
	m_pTransform->Set_State(CTransform::STATE_LOOK, m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK));

	_vector vPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);

	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT)) * 1;
	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_UP)) * -1;
	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK)) * 1;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	return S_OK;
}

HRESULT CWeapon_Launcher::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Launcher"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;




	return S_OK;
}

HRESULT CWeapon_Launcher::SetUp_ConstantTable()
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

HRESULT CWeapon_Launcher::SetUp_Motion(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Get_DIKeyState(DIK_R)& DIS_Down && false == m_bOneTimeSwith && true == m_pBlueMagazine->Get_CountingNumberOn())
	{
		m_iAnimation = 2;
		m_iNextAnimation = 1;
		m_bOneTimeSwith = true;

		m_iNumber[m_iSlotNumber] = 1;

		m_pBlueMagazine->Set_Magazine(-1);

		//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_EFFECT, 0.5f);
		GetSingle(CGameInstance)->PlaySound((L"LauncherReload_1.mp3"), CHANNEL_EFFECT);

		m_SoundTime = 0;
		m_SoundOn = true;
	}
	else if (pGameInstance->Get_DIKeyState(DIK_R)& DIS_Down && false == m_bOneTimeSwith && false == m_pBlueMagazine->Get_CountingNumberOn())
	{
		//flase면 탄창이 없습니다 어쩌고 사운드 ㄱㄱㄱ + 진동

		GetSingle(CGameInstance)->PlaySound((L"Embarrassment.mp3"), CHANNEL_EFFECT);

		m_pCamera_Player->Start_CameraQuake(3, 0.3f);
	}

	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down && false == m_bOneTimeSwith && 0 != m_iNumber[m_iSlotNumber])
	{
		if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Bullet_Launcher"), TEXT("Prototype_GameObject_Bullet_Launcher"))))
			return E_FAIL;

		m_iAnimation = 0;
		m_iNextAnimation = 1;
		m_bOneTimeSwith = true;

		m_iNumber[m_iSlotNumber] -= 1;

		//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_EFFECT, 0.5f);
		GetSingle(CGameInstance)->PlaySound((L"LauncherShot.mp3"), CHANNEL_EFFECT);
	}
	else if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down && 0 == m_iNumber[m_iSlotNumber])
	{
		m_pCamera_Player->Start_CameraQuake(3, 0.3f);

		GetSingle(CGameInstance)->PlaySound((L"NotLauncherBullet.mp3"), CHANNEL_EFFECT);
	}

	RELEASE_INSTANCE(CGameInstance);

	m_SoundTime += TimeDelta;

	if (true == m_SoundOn && m_SoundTime >= 0.8f)
	{
		GetSingle(CGameInstance)->PlaySound((L"LauncherReload_2.mp3"), CHANNEL_EFFECT);

		m_SoundOn = false;
	}

	return S_OK;
}



CWeapon_Launcher * CWeapon_Launcher::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CWeapon_Launcher*	pInstance = new CWeapon_Launcher(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CWeapon_Launcher");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWeapon_Launcher::Clone(void * pArg)
{
	CWeapon_Launcher*	pInstance = new CWeapon_Launcher(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CWeapon_Launcher");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWeapon_Launcher::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
