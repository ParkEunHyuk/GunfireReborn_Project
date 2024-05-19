#include "stdafx.h"
#include "..\Public\Monster_Lazer.h"
#include "GameInstance.h"
#include "Monster_Raven.h"

CMonster_Lazer::CMonster_Lazer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CMonster_Lazer::CMonster_Lazer(const CMonster_Lazer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster_Lazer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Lazer::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_LazerDesc, pArg, sizeof(LAZERDESC));

	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.0f)))))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransform->Scaled(XMVectorSet(2.f, 2.f, 2.f, 2.f));
	//m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.0f));

	//_vector vAxis = XMLoadFloat4(&_float4(1.f, 0.f, 0.f, 0.f));

	//m_pTransform->Rotation(vAxis, XMConvertToRadians(90));


	m_BoneMatrixPtr = m_LazerDesc.pModel->Get_BoneMatrixPtr(m_LazerDesc.pSocketName);
	m_SocketTransformMatrix = m_LazerDesc.pModel->Get_TransformMatrix();


	m_pMonsterObject = reinterpret_cast<CLandObject*>(m_LazerDesc.pObject);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CMonster_Lazer::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	_bool TempAimOn = static_cast<CMonster_Raven*>(m_LazerDesc.pObject)->Get_AimingOn();

	if (m_pMonsterObject->m_Info.fHp <= 0.01 || OBJ_DEAD == m_iDead || false == TempAimOn)
	{
		return OBJ_DEAD;
	}






	//원래 코드
	_matrix			TransformMatrix = XMLoadFloat4x4(m_BoneMatrixPtr.pOffsetMatrix) * XMLoadFloat4x4(m_BoneMatrixPtr.pCombinedMatrix) * XMLoadFloat4x4(&m_SocketTransformMatrix) * m_LazerDesc.pTransform->Get_WorldMatrix();

	m_pTransform->Set_WorldFloat4x4(TransformMatrix);

	//NavigationOn(TimeDelta);
	TargetOn(TimeDelta);
	Pivot(TimeDelta);



	////신께서 도와준 코드
	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//_vector vRight = m_pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
	//_vector vUp = m_pPlayerTransform->Get_State(CTransform::STATE_UP);
	//_vector vLook = m_pPlayerTransform->Get_State(CTransform::STATE_LOOK);

	//vRight = m_pTransform->Get_Scale(CTransform::STATE_RIGHT) * XMVector3Normalize(vRight);
	//vUp = m_pTransform->Get_Scale(CTransform::STATE_UP) * XMVector3Normalize(vUp);
	//vLook = m_pTransform->Get_Scale(CTransform::STATE_LOOK) * XMVector3Normalize(vLook);

	//m_pTransform->Set_State(CTransform::STATE_RIGHT, vLook);
	//m_pTransform->Set_State(CTransform::STATE_UP, -vRight);
	//m_pTransform->Set_State(CTransform::STATE_LOOK, -vUp);

	////SetUp_Info에 몬스터의 룩을 넣어서 따라왔던거 같음

	//RELEASE_INSTANCE(CGameInstance);





	//_vector vAxis = XMLoadFloat4(&_float4(1.f, 0.f, 0.f, 0.f));
	//
	//m_pTransform->Rotation(vAxis, XMConvertToRadians(90));

	return _int();
}

_int CMonster_Lazer::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

#ifdef _DEBUG	
	m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif // _DEBUG

	return _int();
}

HRESULT CMonster_Lazer::Render()
{
	if ( nullptr == m_pVIBufferCom)
	{
		MSGBOX("CMonster_Lazer부분의 렌더에 문제가 발생");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom,1);

	return S_OK;
}

_int CMonster_Lazer::Obsever_Hit(void * pArg)
{
	int a = 10;
	return _int();
}

_int CMonster_Lazer::Obsever_Event(void * pArg)
{
	int a = 10;
	return _int();
}

HRESULT CMonster_Lazer::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXTEX"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Raven_Lazer"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

HRESULT CMonster_Lazer::SetUp_ConstantTable()
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

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Lazer::NavigationOn(_double TimeDelta)
{
	_float3 vDir;
	XMStoreFloat3(&vDir, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK)) * m_pTransform->Get_TransformDesc().fSpeedPerSec * (_float)TimeDelta);

	if (false == m_pNavigationCom->Move_OnNavigation(m_pTransform->Get_State(CTransform::STATE_POSITION), &vDir))
	{
		m_iDead = OBJ_DEAD;
	}

	return S_OK;
}

HRESULT CMonster_Lazer::TargetOn(_double TimeDelta)
{
	////일단은 이게 맞는 코드 LookAtNOTAxisY
	_vector vTempPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector vTarPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	vTempPos = XMVectorSetY(vTempPos, 0.f);
	vTarPos = XMVectorSetY(vTarPos, 0.f);

	_vector vLook = vTarPos - vTempPos;

	vLook = XMVector3Normalize(vLook);

	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));

	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vLook * XMVectorGetZ(m_pTransform->Get_Scale(CTransform::STATE_LOOK)));
	m_pTransform->Set_State(CTransform::STATE_UP, -vRight *XMVectorGetX(m_pTransform->Get_Scale(CTransform::STATE_RIGHT)));
	m_pTransform->Set_State(CTransform::STATE_LOOK, -vUp * XMVectorGetY(m_pTransform->Get_Scale(CTransform::STATE_UP)));
	/////////////////////////////////


	return S_OK;
}

HRESULT CMonster_Lazer::Pivot(_double TimeDelta)
{
	//초당 거리를 계산
	_vector vDistance = m_pTransform->Get_Distance(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
	_float	fDistance;
	XMStoreFloat(&fDistance, vDistance);
	
	_float Dispersec = fDistance / 40; //초당 거리를 계산중 1초를 기존으로 하고 있으니 더 빠르게 하고 싶으면 60이 아니라 50 또는 40 이런식으로 하면 될 듯

	if (fTempDis < fDistance )
	{
		fTempDis += Dispersec;
	}
	else if(fTempDis > fDistance){
		fTempDis -= Dispersec;
	}

	m_pTransform->Scaled(XMVectorSet(fTempDis, 0.15f, 1.f, 0.f));

	_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT)) *(fTempDis * 0.5);
	vPosition += XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK))*-0.1;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	
	////test
	//_vector		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	//_vector		vUp = m_pTransform->Get_State(CTransform::STATE_UP);
	//_vector		vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

	//// vRight = XMVectorSetW(vRight, 0.f);

	//_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f,1.f,0.f,0.f),10);

	//vRight = XMVector4Transform(vRight, RotationMatrix);
	//vUp = XMVector4Transform(vUp, RotationMatrix);
	//vLook = XMVector4Transform(vLook, RotationMatrix);

	//m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	//m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	//m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

	////test

	//////////////////////////////초당 계산없이 깔끔하게 나오지만 보간처리가 되어 있지 않음
	//_vector vDistance = m_pTransform->Get_Distance(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
	//_float	fDistance;
	//XMStoreFloat(&fDistance, vDistance);
	//m_pTransform->Scaled(XMVectorSet(fDistance, 0.3f, 1.f, 0.f));

	//_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

	//vPosition += XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT)) *(fDistance * 0.5);
	//vPosition += XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK))*-0.1;

	//m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}


CMonster_Lazer * CMonster_Lazer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Lazer*	pInstance = new CMonster_Lazer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMonster_Lazer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Lazer::Clone(void * pArg)
{
	CMonster_Lazer*	pInstance = new CMonster_Lazer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Lazer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Lazer::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}