#include "..\Public\Level.h"

CLevel::CLevel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CLevel::NativeConstruct()
{
	return S_OK;
}

_int CLevel::Tick(_double TimeDelta)
{
	return _int();
}

_int CLevel::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CLevel::Render()
{
	return S_OK;
}

HRESULT CLevel::Level_InGame_Chage(_bool Level_Chage_Switch, _uint _INextLevel)
{
	m_bLevel_Switch = Level_Chage_Switch;
	m_INextLevel = _INextLevel;

	return S_OK;
}

void CLevel::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
