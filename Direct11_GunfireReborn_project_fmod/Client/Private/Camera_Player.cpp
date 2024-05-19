#include "stdafx.h"
#include "..\Public\Camera_Player.h"
#include "GameInstance.h"

CCamera_Player::CCamera_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CCamera(pDevice, pDeviceContext)
{
}

CCamera_Player::CCamera_Player(const CCamera_Player & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Player::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Player::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	m_pPlayerTransformCom = static_cast<CTransform*>(pGameInstance->GetInstance()->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));
	

	//게임 오브젝트 정보 가져오는것
	//CGameObject* pPlayer = static_cast<CGameObject*>(pGameInstance->GetInstance()->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Player")));
	
	//객체의 컴포넌트 정보를 가져오는것 인덱스는 디폴트 매개변수로 0으로 되어 있음
	//m_pPlayerTransformCom = static_cast<CTransform*>(pGameInstance->GetInstance()->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));

	//m_pPlayerTransformCom = static_cast<CTransform*>(pPlayer->Get_Component(TEXT("Com_Transform")));
	//m_pPlayerTransformCom = static_cast<CTransform*>(pGameInstance->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));

	RELEASE_INSTANCE(CGameInstance);


	//디버그중일땐 마우뜨를 가두지말장
	//FocusCursorToScreen();

	return S_OK;
}

_int CCamera_Player::Tick(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_ESCAPE) & DIS_Up)
	{
		PostQuitMessage(NULL);
	}


	Set_CameraPos();

	if (true == m_bQuakeStart)
	{
		Camera_Quaking(TimeDelta);
	}



	RELEASE_INSTANCE(CGameInstance);

	return __super::Tick(TimeDelta);
}

_int CCamera_Player::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	return _int();
}

HRESULT CCamera_Player::Render()
{
	return S_OK;
}

void CCamera_Player::Camera_Fovy(_float _Degree)
{
	m_CameraDesc.fFovy = XMConvertToRadians(_Degree);
}

HRESULT CCamera_Player::Set_CameraPos()
{
	m_pTransform->Set_State(CTransform::STATE_RIGHT,m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_UP, m_pPlayerTransformCom->Get_State(CTransform::STATE_UP));
	m_pTransform->Set_State(CTransform::STATE_LOOK, m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK));

	_vector vPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);

	XMVectorSetY(vPos, 0.5);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	//m_pTransform->Set_State(CTransform::STATE_POSITION, m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

void CCamera_Player::FocusCursorToScreen()
{
	//디버깅중일땐 마우스를 가두지말자 일단 주석처리하고 게임 플레이할 땐 풀자

	RECT rc = {};
	POINT p1, p2;
	GetClientRect(g_hWnd, &rc);
	p1.x = rc.left;
	p1.y = rc.top;
	p2.x = rc.right;
	p2.y = rc.bottom;

	ClientToScreen(g_hWnd, &p1);
	ClientToScreen(g_hWnd, &p2);
	rc.left = p1.x;
	rc.top = p1.y;
	rc.right = p2.x;
	rc.bottom = p2.y;

	//ScreenToClient
	ClipCursor(&rc);

}

HRESULT CCamera_Player::Start_CameraQuake(_uint _iQuake, _double _dQuakeTime)
{
	m_bQuakeStart = true;
	m_iQuake = _iQuake;
	m_dQuakeMaxTime = _dQuakeTime;

	return S_OK;
}

HRESULT CCamera_Player::Camera_Quaking(_double TimeDelta)
{
	m_dQuakeTime += TimeDelta;

	_float Random1 = (rand() % m_iQuake)* 0.01f;
	_float Random2 = (rand() % m_iQuake)* 0.01f;

	_vector vPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);

	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT)) * Random1;
	vPos += XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_UP)) * Random2;

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	if (m_dQuakeTime >= m_dQuakeMaxTime)
	{
		m_dQuakeTime = 0.f;
		m_bQuakeStart = false;
	}

	return S_OK;
}

CCamera_Player * CCamera_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CCamera_Player*	pInstance = new CCamera_Player(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Player::Clone(void * pArg)
{
	CCamera_Player*	pInstance = new CCamera_Player(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCamera_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Player::Free()
{
	__super::Free();



}
