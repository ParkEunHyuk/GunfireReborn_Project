#include "stdafx.h"
#include "..\Public\Bullet_Energy.h"
#include "GameInstance.h"

CBullet_Energy::CBullet_Energy(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CBullet(pDevice, pDeviceContext)
{

}

CBullet_Energy::CBullet_Energy(const CBullet_Energy & rhs)
	: CBullet(rhs)
{

}

HRESULT CBullet_Energy::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet_Energy::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(&TransformDesc)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pWeaponTransformCom = static_cast<CTransform*>(pGameInstance->GetInstance()->Get_Component(m_eNowLevelNum, TEXT("Layer_Weapon_Cannon"), TEXT("Com_Transform")));

	m_Energy_Damage = ENERGY_DAMAGE;

	RELEASE_INSTANCE(CGameInstance);

	//m_pNavigationCom->Find_CellIndex(m_pTransform); 밑에 따로 써두었음

	return S_OK;
}


_int CBullet_Energy::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_dDeadTime += TimeDelta;

	if (m_dDeadTime >= 10 || true == m_bNavigationDead)
	{
		return OBJ_DEAD;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_bColliderOn == true)
	{
		if (FAILED(pGameInstance->GetInstance()->Add_ObseverGroup(BULLET, this)))
		{
			MSGBOX("CBullet_Energy의 Add_ObseverGroup에 문제가 발생했습니다");
			return E_FAIL;
		}
	}

	Fire_Input(TimeDelta);

	
	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CBullet_Energy::LateTick(_double TimeDelta)
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

HRESULT CBullet_Energy::Render()
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

_int CBullet_Energy::Dead_Trigger()
{
	return _int();
}

HRESULT CBullet_Energy::Fire_Input(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
	{
		if (false == m_bFire)
		{
			SetUp_Magnet(TimeDelta);
			m_pTransform->Scaling(5,10, TimeDelta);
			if (m_Energy_Damage <= 0.5)
			{
				m_Energy_Damage += 0.004;
			}
			else {
				int a = 10;
			}
		}
		else {
			m_pTransform->Go_Straight(TimeDelta);
		}
	}
	else
	{
		if (false == m_bFire)
		{
			m_bFire = SetUp_BulletPos();
			SetUp_Collider(); //Set부터하고 콜라이더 온 시켜야함 순서 잘 보셈
			m_bColliderOn = true;
			m_pNavigationCom->Find_CellIndex(m_pTransform);

		}

		m_pTransform->Go_Straight(TimeDelta);

		_float3 vDir;
		XMStoreFloat3(&vDir, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * m_pTransform->Get_TransformDesc().fSpeedPerSec * (_float)TimeDelta);

		if (false == m_pNavigationCom->Move_OnNavigation(m_pTransform->Get_State(CTransform::STATE_POSITION), &vDir))
		{
			m_bNavigationDead = true;
		}
	}



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_bool CBullet_Energy::SetUp_BulletPos()
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

	return true;
}

HRESULT CBullet_Energy::SetUp_Magnet(_double TimeDelta)
{
	_vector		vPosition = m_pWeaponTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(m_pWeaponTransformCom->Get_State(CTransform::STATE_LOOK)) * 2;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CBullet_Energy::SetUp_Collider()
{

	/* For.Com_Sphere */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.0f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

_int CBullet_Energy::Obsever_Hit(void * pArg)
{
	CLandObject* DestObject = static_cast<CLandObject*>(pArg);

	DestObject->m_Info.fHp -= m_Energy_Damage;

	return _int();
}

_int CBullet_Energy::Obsever_Event(void * pArg)
{
	return _int();
}

HRESULT CBullet_Energy::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(m_eNowLevelNum, TEXT("Prototype_Component_Shader_NonAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bullet_Energy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
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

HRESULT CBullet_Energy::SetUp_ConstantTable()
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

CBullet_Energy * CBullet_Energy::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBullet_Energy*	pInstance = new CBullet_Energy(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CBullet_Energy");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBullet_Energy::Clone(void * pArg)
{
	CBullet_Energy*	pInstance = new CBullet_Energy(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Copying CBullet_Energy");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_Energy::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}