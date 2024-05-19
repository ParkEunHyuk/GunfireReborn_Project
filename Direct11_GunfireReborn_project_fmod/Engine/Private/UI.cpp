#include "..\Public\UI.h"

CUI::CUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice,pDeviceContext)
{
}

CUI::CUI(const CUI & rhs)
	:CGameObject(rhs)
{
}

HRESULT CUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	return S_OK;
}

_int CUI::Tick(_double DeltaTime)
{
	if (FAILED(__super::Tick(DeltaTime)))
		return E_FAIL;

	return _int();
}

_int CUI::LateTick(_double DeltaTime)
{
	if (FAILED(__super::Tick(DeltaTime)))
		return E_FAIL;

	return _int();
}

HRESULT CUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Set_UI_Transform(_float4 vUIDesc, _uint g_iWinCX, _uint g_iWinCY)
{
	//UIDesc�� fX,fY,SizeX,SizeY ���� �������
	//x��ǥ,y��ǥ ,������x ������y = ����Ʈ, ž, ����Ʈ, ���� 

	if (m_pTransform == nullptr)
	{
		MSGBOX("UI�� Transform�� �������� �ʽ��ϴ�.");
		return E_FAIL;
	}

	m_fX = vUIDesc.x;
	m_fY = vUIDesc.y;
	m_fSizeX = vUIDesc.z;
	m_fSizeY = vUIDesc.w;


	m_pTransform->Scaled(XMVectorSet(m_fSizeX, m_fSizeY, 1.f,0.f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));

	return S_OK;
}

_float CUI::Get_Depth()
{
	return m_fDepth;
}

void CUI::Free()
{
	__super::Free();
}
