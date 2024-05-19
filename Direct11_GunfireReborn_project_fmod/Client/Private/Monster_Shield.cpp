#include "stdafx.h"
#include "..\Public\Monster_Shield.h"
#include "GameInstance.h"
#include "Monster_Horse.h"
#include "Monster_PowerLizard.h"

CMonster_Shield::CMonster_Shield(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CMonster_Shield::CMonster_Shield(const CMonster_Shield & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster_Shield::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Shield::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_ShieldDesc, pArg, sizeof(SHIELDDESC));

	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.0f)))))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransform->Scaled(XMVectorSet(0.1f, 0.1f, 0.1f, 1.f));
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.0f));


	m_BoneMatrixPtr = m_ShieldDesc.pModel->Get_BoneMatrixPtr(m_ShieldDesc.pSocketName);
	m_SocketTransformMatrix = m_ShieldDesc.pModel->Get_TransformMatrix();



	SetUp_Shield();

	m_pMonsterObject = reinterpret_cast<CLandObject*>(m_ShieldDesc.pObject);

	return S_OK;
}

_int CMonster_Shield::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	if (m_pMonsterObject->m_Info.fHp <= 0.01)
	{
		return OBJ_DEAD;
	}


	/* 모델의 뼈 : offsetMatrix, CombinedTransformationMatrix, TransformMatrix */

	//_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(m_pTransform->Get_State(CTransform::STATE_RIGHT)));
	//_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(m_pTransform->Get_State(CTransform::STATE_UP)));
	//_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(m_pTransform->Get_State(CTransform::STATE_LOOK)));


	//_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(350));
	//RotationMatrix = RotationMatrix *  XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(40));
	//RotationMatrix = RotationMatrix * XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(120));

	











	//원래 코드
	_matrix			TransformMatrix = XMLoadFloat4x4(m_BoneMatrixPtr.pOffsetMatrix) * XMLoadFloat4x4(m_BoneMatrixPtr.pCombinedMatrix) * XMLoadFloat4x4(&m_SocketTransformMatrix) * m_ShieldDesc.pTransform->Get_WorldMatrix();

	m_pSphereCom->Update_Transform(TransformMatrix);
	m_pOBBCom->Update_Transform(TransformMatrix);

	return _int();
}

_int CMonster_Shield::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);

	RELEASE_INSTANCE(CGameInstance);

#ifdef _DEBUG
	m_pRendererCom->Add_Debug(m_pSphereCom);
	m_pRendererCom->Add_Debug(m_pOBBCom);
#endif // _DEBUG

	return _int();
}

HRESULT CMonster_Shield::Render()
{
	if (nullptr == m_pSphereCom || nullptr == m_pOBBCom)
		return E_FAIL;


//#ifdef _DEBUG
//	m_pSphereCom->Render();
//	m_pOBBCom->Render();
//#endif // _DEBUG

	return S_OK;
}

HRESULT CMonster_Shield::SetUp_Shield()
{
	switch (m_ShieldDesc.eShieldNumber)
	{
	case HORSE_SHIELD:
	{
		if (nullptr != m_ShieldDesc.pObject)
		{
			static_cast<CMonster_Horse*>(m_ShieldDesc.pObject)->Set_Shield(m_pSphereCom, m_pOBBCom);
		}
		break;
	case POWERLIZARD_SHIELD:
	{
		if (nullptr != m_ShieldDesc.pObject)
		{
			static_cast<CMonster_PowerLizard*>(m_ShieldDesc.pObject)->Set_Shield(m_pSphereCom, m_pOBBCom);
		}
		break;
	}
	}
	default:
	{
		MSGBOX("Monster_Shield의 SetUp_Shield에 문제가 발생했습니다.");
	}
	}

	return S_OK;
}

_int CMonster_Shield::Obsever_Hit(void * pArg)
{
	int a = 10;
	return _int();
}

_int CMonster_Shield::Obsever_Event(void * pArg)
{
	int a = 10;
	return _int();
}

HRESULT CMonster_Shield::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	//콜라이더 준비 중
	switch (m_ShieldDesc.eShieldNumber)
	{
	case HORSE_SHIELD:
	{

		/* For.Com_SPHERE */
		CCollider::COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

		ColliderDesc.vScale = _float3(3.0f, 3.0f, 3.0f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vTranslation = _float4(0.f, 0.0f, 0.f, 1.f);

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
			return E_FAIL;

		_vector vQuaternion;
		vQuaternion = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(45), XMConvertToRadians(-45), XMConvertToRadians(-45));//피치 야 롤을 이용해서 회전 시키자 벡터값이 나오는데 이걸 플롯으로 바꾸고 W에 1을 넣자.

		_float4	fQuaternion;
		XMStoreFloat4(&fQuaternion, vQuaternion);

		ColliderDesc.vScale = _float3(0.5f, 2.5f, 2.5f);
		//ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vRotation = fQuaternion;
		ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
			return E_FAIL;
		break;
	}
	case POWERLIZARD_SHIELD:
	{
		
	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(3.5f, 3.5f, 3.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	_vector vQuaternion;
	vQuaternion = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0), XMConvertToRadians(0), XMConvertToRadians(0));//피치 야 롤을 이용해서 회전 시키자 벡터값이 나오는데 이걸 플롯으로 바꾸고 W에 1을 넣자.

	_float4	fQuaternion;
	XMStoreFloat4(&fQuaternion,vQuaternion);

	ColliderDesc.vScale = _float3(0.3, 2.5, 2.5);
	ColliderDesc.vRotation = fQuaternion;
	ColliderDesc.vTranslation = _float4(0.f, 0.f,0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;
		break;
	}
	default:
	{
		MSGBOX("CMonster_Shield의 콜라이더 부분에 문제가 발생했습니다.");
		break;
	}
	}

	return S_OK;
}


CMonster_Shield * CMonster_Shield::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Shield*	pInstance = new CMonster_Shield(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMonster_Shield");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Shield::Clone(void * pArg)
{
	CMonster_Shield*	pInstance = new CMonster_Shield(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Shield");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Shield::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
}