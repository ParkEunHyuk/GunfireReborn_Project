#include "stdafx.h"
#include "..\Public\Item_Universal.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_HpBar.h"
#include "UI_MpBar.h"

const 	_tchar*	 m_pModelTag[CItem_Universal::ITEM_END] = {
	TEXT("Prototype_Component_Model_Item_Hp"),
	TEXT("Prototype_Component_Model_Item_Mp"),
	TEXT("Prototype_Component_Model_Item_GreenMagazine"),
	TEXT("Prototype_Component_Model_Item_BlueMagazine")
};

CItem_Universal::CItem_Universal(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CItem_Universal::CItem_Universal(const CItem_Universal & rhs)
	: CLandObject(rhs)
{
}

HRESULT CItem_Universal::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CItem_Universal::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Item_UniversalDesc, pArg, sizeof(ITEM_UNIVERSALDESC));
	}
	else
	{
		MSGBOX("CItem_Universal에 pArg가 Nullptr입니다.");
	}

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 20;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));

	m_pTransform->Scaled(XMLoadFloat3(&m_Item_UniversalDesc.vScale));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_Item_UniversalDesc.vTranslation));


	Random_Pos();


	m_pModelCom->SetUp_AnimIndex(m_iOnceAnimNumber);

	RELEASE_INSTANCE(CGameInstance);

	m_pTransform->LookAtNOTAxisY(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION), 0);

	return S_OK;
}

_int CItem_Universal::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	XMStoreFloat(&m_fDistance, m_pTransform->Get_Distance(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION)));

	if (false == m_bEasingOff)
	{
		Easing(TimeDelta);
	}

	Animation(TimeDelta);

	if (true == m_bEasingOff)
	{
		Trace(TimeDelta);
	}
	


	m_dDeltaTime += TimeDelta;




	if (m_fDistance < 1)
	{
		return OBJ_DEAD;
	}else if (m_dDeltaTime > 30)
	{
		m_bDisappearOn = true;
		return OBJ_DEAD;
	}


	return m_iDead;
}

_int CItem_Universal::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CItem_Universal::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Item_Universal의 Render()에 문제가 발생했습니다.");
		return E_FAIL;
	}

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

HRESULT CItem_Universal::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_pModelTag[m_Item_UniversalDesc.iItemNumber], TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_Universal::SetUp_ConstantTable()
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

_int CItem_Universal::Dead_Trigger()
{
	if (false == m_bDisappearOn)
	{
		switch (m_Item_UniversalDesc.iItemNumber)
		{
		case ITEM_HP:
		{
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
			CUI_HpBar* pHpBar = static_cast<CUI_HpBar*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_HpBar")));
			pHpBar->SetUp_HpBar(-0.2f); //지금 현재 내 코드에선 0.1를 넣으면 체력이 10퍼센트 깍임 그러므로 -0.1를 넣음으로써 반대로 10퍼센트가 증가함

			RELEASE_INSTANCE(CGameInstance);

			GetSingle(CGameInstance)->PlaySound((L"Iten_Recovery.wav"), CHANNEL_EFFECT);

			break;
		}
		case ITEM_MP:
		{
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
			CUI_MpBar* pMpBar = static_cast<CUI_MpBar*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_MpBar")));
			pMpBar->SetUp_MpBar(-0.3f); //지금 현재 코드에선 0.1를 넣으면 마나가 10퍼센트 깍임 그러므로 -0.1를 넣음으로써 반대로 10퍼센트가 증가함

			RELEASE_INSTANCE(CGameInstance);

			GetSingle(CGameInstance)->PlaySound((L"Iten_Recovery.wav"), CHANNEL_EFFECT);

			break;
		}
		case ITEM_GREENBULLET:
		{
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
			CUI_Bullet_Number*	pGreenMagazine = static_cast<CUI_Bullet_Number*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_GreenMagazine")));

			pGreenMagazine->Set_Magazine(1);

			RELEASE_INSTANCE(CGameInstance);

			GetSingle(CGameInstance)->PlaySound((L"Item_MagazineBox.mp3"), CHANNEL_EFFECT);

			break;
		}
		case ITEM_BLUEBULLET:
		{
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
			CUI_Bullet_Number*	pBlueMagazine = static_cast<CUI_Bullet_Number*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_BlueMagazine")));

			pBlueMagazine->Set_Magazine(1);

			RELEASE_INSTANCE(CGameInstance);

			GetSingle(CGameInstance)->PlaySound((L"Item_MagazineBox.mp3"), CHANNEL_EFFECT);

			break;
		}
		}
	}
	return _int();
}

HRESULT CItem_Universal::FollowMe(_double TimeDelta)
{
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector	vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector	vDir = vPlayerPos - vPos;

	vPos += XMVector4Normalize(vDir) *20*(_float)TimeDelta;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	return S_OK;
}

HRESULT CItem_Universal::Trace(_double TimeDelta)
{

	if (m_fDistance < 5)
	{
		m_bTraceOn = true;
	}

	if (true == m_bTraceOn)
	{
		FollowMe(TimeDelta*0.5);
	}

	return S_OK;
}

HRESULT CItem_Universal::Random_Pos()
{

	_float				RandomX, RandomY, RandomZ, RandomPositive;
	RandomX	=	(rand() % 3000) * 0.001f;
	RandomY = (rand() % 3000) * 0.001f;
	RandomZ = (rand() % 3000) * 0.001f;
	RandomPositive = (_float)(rand() % 2);

	if (RandomPositive == 0)
	{
		RandomPositive = -1;
	}

	fTargetFirstPos = m_Item_UniversalDesc.vTranslation;
	fTargetSecondPos = fTargetFirstPos;

	fTargetFirstPos.x += (RandomX*0.5f) * RandomPositive;
	fTargetFirstPos.y += RandomY;
	fTargetFirstPos.z += (RandomZ*0.5f) * RandomPositive;

	fTargetSecondPos.x += RandomX * RandomPositive;
	fTargetSecondPos.z += RandomZ * RandomPositive;

	if (ITEM_HP == m_Item_UniversalDesc.iItemNumber || ITEM_MP == m_Item_UniversalDesc.iItemNumber)
	{
		m_pTransform->Scaled(XMVectorSet(2.5f, 2.5f, 2.5f, 0.f));
		fTargetSecondPos.y = 0.5;
	}
	else if (ITEM_GREENBULLET == m_Item_UniversalDesc.iItemNumber || ITEM_BLUEBULLET == m_Item_UniversalDesc.iItemNumber)
	{
		m_pTransform->Scaled(XMVectorSet(2.f, 2.f, 2.f, 0.f));
		fTargetSecondPos.y = 0.f;
	}

	return S_OK;
}

HRESULT CItem_Universal::Easing(_double TimeDelta)
{
	_vector	vTemPos;

	m_dTimer += TimeDelta;
	m_dEasingTime += TimeDelta;

	if (true == m_bEasingFirst)
	{
		vTemPos = GetSingle(CGameInstance)->Easing(TYPE_SinOut, XMLoadFloat4(&m_Item_UniversalDesc.vTranslation), XMLoadFloat4(&fTargetFirstPos), m_dEasingTime, 0.5f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, vTemPos);
		if (m_dEasingTime > 0.5)
		{
			m_dEasingTime = 0;
			m_bEasingFirst = false;
		}

	}
	else if (false == m_bEasingFirst)
	{
		vTemPos = GetSingle(CGameInstance)->Easing(TYPE_SinIn, XMLoadFloat4(&fTargetFirstPos), XMLoadFloat4(&fTargetSecondPos), m_dEasingTime, 0.4f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, vTemPos);

		if (m_dEasingTime > 0.4)
		{
			//m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vTemPos,0));
			m_dEasingTime = 0;
			m_bEasingOff = true;

			switch (m_Item_UniversalDesc.iItemNumber)
			{
			case ITEM_HP:
				m_pTransform->Set_State(CTransform::STATE_POSITION,XMVectorSetY(vTemPos,0.5f));
				break;
			case ITEM_MP:
				m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vTemPos, 0.5f));
				break;
			case ITEM_GREENBULLET:
				m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vTemPos, 0.f));
				break;
			case ITEM_BLUEBULLET:
				m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vTemPos, 0.f));
				break;
			}
		}
	}

	return S_OK;
}

HRESULT CItem_Universal::Animation(_double TimeDelta)
{
	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, 0, 1, m_bOneTimeSwith);

	return S_OK;
}

CItem_Universal * CItem_Universal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CItem_Universal*	pInstance = new CItem_Universal(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CItem_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CItem_Universal::Clone(void * pArg)
{
	CItem_Universal*	pInstance = new CItem_Universal(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CItem_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItem_Universal::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
