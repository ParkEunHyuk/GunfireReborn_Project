#include "stdafx.h"
#include "..\Public\Monster_Npc.h"
#include "GameInstance.h"
#include "Universal_TextureEffect.h"
#include "UI_Universal.h"


CMonster_Npc::CMonster_Npc(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CMonster_Npc::CMonster_Npc(const CMonster_Npc & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Npc::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Npc::NativeConstruct(void * pArg)
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
	m_Info.fHp = 100;
	m_Info.fMaxHp = m_Info.fHp;

	m_pModelCom->SetUp_AnimIndex(0);

	m_pNavigationCom->Find_CellIndex(m_pTransform);

	SetUp_Info();

	return S_OK;
}

_int CMonster_Npc::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->GetInstance()->Add_ObseverGroup(MONSTER, this)))
	{
		MSGBOX("CMonster_Npc의 Add_ObseverGroup에 문제가 발생했습니다");
		return E_FAIL;
	}


	if (false == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
	{
	}
	else {

		PlayAnim(TimeDelta);
	}

	//PlayAnim(TimeDelta);

	Distance();

	//m_pSphereCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	RELEASE_INSTANCE(CGameInstance);

	return OBJ_NOEVENT;
}

_int CMonster_Npc::LateTick(_double TimeDelta)
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

HRESULT CMonster_Npc::Render()
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

HRESULT CMonster_Npc::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Model_Npc"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Sphere */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 1.f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;


	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 1.2f, 0.f, 1.f);

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

HRESULT CMonster_Npc::SetUp_ConstantTable()
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

HRESULT CMonster_Npc::PlayAnim(_double TimeDelta)
{
	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta, m_bOneTimeSwith);

	return S_OK;
}

HRESULT CMonster_Npc::SetUp_Info()
{
	switch (m_eNowLevelNum)
	{
	case LEVEL_STAGE1:
	{
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180));
		m_pTransform->Scaled(XMVectorSet(1.f, 1.f, 1.f, 0.f));
		m_pTransform->Set_State(CTransform::STATE_POSITION,XMVectorSet(109.5f,1.f,114.f,1.f));
		//m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(15.f, 1.f, 15.f, 1.f));
		break;
	}
	case LEVEL_STAGE1_BOSSROOM:
	{
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180));
		m_pTransform->Scaled(XMVectorSet(1.f, 1.f, 1.f, 0.f));
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(88.5f, 1.f, 88.f, 1.f));
		break;
	}
	case LEVEL_STAGE2:
	{
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90));
		m_pTransform->Scaled(XMVectorSet(1.f, 1.f, 1.f, 0.f));
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(124.f, 1.f, 106.f, 1.f));
		break;
	}
	case LEVEL_STAGE2_BOSSROOM:
	{
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180));
		m_pTransform->Scaled(XMVectorSet(1.f, 1.f, 1.f, 0.f));
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(77.f, 1.f, 119.f, 1.f));
		break;
	}
	}


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMonster_Npc::Distance()
{
	_vector vDis = m_pTransform->Get_Distance(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	m_fDistance;

	XMStoreFloat(&m_fDistance, vDis);

	if (false == m_bMessageOn)
	{
		if (m_fDistance < 10)
		{
			if (true == m_bEnterOn)
			{
				Enter_Effect();

				m_bEnterOn = false;
			}
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
			if (pGameInstance->Get_DIKeyState(DIK_RETURN) & DIS_Down)
			{
				Talk_Box();
				m_bMessageOn = true;
			}
			RELEASE_INSTANCE(CGameInstance);
		}

		if (m_fDistance > 10)
		{
			m_bEnterOn = true;
			m_bMessageOn = false;
		}
	}

	return S_OK;
}

HRESULT CMonster_Npc::Enter_Effect()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC Universal_EffectDesc;
	ZeroMemory(&Universal_EffectDesc, sizeof(CUniversal_TextureEffect::UNIVERSAL_EFFECTDESC));

	Universal_EffectDesc.iEffectNumber = CUniversal_TextureEffect::EFFECTTYPE::ENTER_EFFECT;

	Universal_EffectDesc.vScale = _float3(0.1f, 0.1f, 0.1f);
	Universal_EffectDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	Universal_EffectDesc.dDuration = 1000000;

	Universal_EffectDesc.pObject = this;
	Universal_EffectDesc.fPositioning = _float3(0.f, 1.f, 2.f);

	Universal_EffectDesc.iTextureIndex = 0;


	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_Universal_TextureEffect"), TEXT("Prototype_GameObject_Universal_TextureEffect"), &Universal_EffectDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Npc::Talk_Box()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUI_Universal::UI_Universal_DESC UI_UniversalDesc;

	UI_UniversalDesc.iUI_Number = CUI_Universal::TALK_BOX;
	UI_UniversalDesc.fSizeX = 640;
	UI_UniversalDesc.fSizeY = 360;
	UI_UniversalDesc.fX = 700;
	UI_UniversalDesc.fY = 350;
	UI_UniversalDesc.fDepth = 1.f;
	switch (m_eNowLevelNum)
	{
	case LEVEL_STAGE1:
		UI_UniversalDesc.iTextureIndex = 0;
		break;
	case LEVEL_STAGE1_BOSSROOM:
		UI_UniversalDesc.iTextureIndex = 1;
		break;
	case LEVEL_STAGE2:
		UI_UniversalDesc.iTextureIndex = 2;
		break;
	case LEVEL_STAGE2_BOSSROOM:
		UI_UniversalDesc.iTextureIndex = 3;
		break;
	}
	UI_UniversalDesc.pGameObject = this;
	UI_UniversalDesc.dDuration = 9999;



	if (FAILED(pGameInstance->Add_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Universal_Talk_Box"), TEXT("Prototype_GameObject_UI_Universal"), &UI_UniversalDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CMonster_Npc::Obsever_Hit(void* pArg)
{

	return _int();
}

_int CMonster_Npc::Obsever_Event(void * pArg)
{

	return _int();
}

_int CMonster_Npc::Dead_Trigger()
{

	return _int();
}

CMonster_Npc * CMonster_Npc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Npc*	pInstance = new CMonster_Npc(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMonster_Npc");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Npc::Clone(void * pArg)
{
	CMonster_Npc*	pInstance = new CMonster_Npc(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Npc");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Npc::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}