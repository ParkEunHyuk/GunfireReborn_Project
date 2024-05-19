#include "stdafx.h"
#include "..\Public\Weapon.h"
#include "GameInstance.h"

CWeapon::CWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CWeapon::CWeapon(const CWeapon & rhs)
	: CGameObject(rhs)
{

}

HRESULT CWeapon::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::NativeConstruct(const CTransform::TRANSFORMDESC * pTransformDesc)
{
	if (FAILED(__super::NativeConstruct(pTransformDesc)))
		return E_FAIL;


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransformCom = static_cast<CTransform*>(pGameInstance->GetInstance()->Get_Component(m_eNowLevelNum, TEXT("Layer_Player"), TEXT("Com_Transform")));

	m_pBullet_Number = reinterpret_cast<CUI_Bullet_Number*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_UI_Bullet_Number")));

	m_pCamera_Player = static_cast<CCamera_Player*>(pGameInstance->Get_GameObject(m_eNowLevelNum, TEXT("Layer_Camera")));

	RELEASE_INSTANCE(CGameInstance);

	m_pTransform->Set_State(CTransform::STATE_POSITION, OUT_POSITION);
	return S_OK;
}

_int CWeapon::Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == m_bTransformOn)
	{
		CTransform* UI_NumberTransform = static_cast<CTransform*>(m_pBullet_Number->Get_Component(TEXT("Com_Transform")));

		Set_WeaponPos();

		if (PISHSWORD == m_iSlotNumber)
		{
			UI_NumberTransform->Set_State(CTransform::STATE_POSITION, OUT_POSITION);
		}
		else {
			m_pBullet_Number->Set_Number(m_iNumber[m_iSlotNumber]);
			UI_NumberTransform->Set_State(CTransform::STATE_POSITION, m_pBullet_Number->Get_UI_NumberPos());
		}
	}
	else {
		m_pTransform->Set_State(CTransform::STATE_POSITION, OUT_POSITION);
	}


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CWeapon::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CWeapon::Render()
{
	return S_OK;
}

void CWeapon::Set_SlotNumber(_uint _iSlotNumber)
{
	m_iSlotNumber = _iSlotNumber;
}

HRESULT CWeapon::Set_WeaponPos()
{
	m_pTransform->Set_State(CTransform::STATE_RIGHT, m_pPlayerTransformCom->Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_UP, m_pPlayerTransformCom->Get_State(CTransform::STATE_UP));
	m_pTransform->Set_State(CTransform::STATE_LOOK, m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK));

	_vector vPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);

	//XMVectorSetY(vPos, 0.5);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	//m_pTransform->Set_State(CTransform::STATE_POSITION, m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

void CWeapon::SetUp_TransformOn(_bool _bTransformOn)
{
	m_bTransformOn = _bTransformOn;
}

void CWeapon::Free()
{
	__super::Free();

}
