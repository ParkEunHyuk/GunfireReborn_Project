#pragma once

#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CUI : public CGameObject
{
protected:
	explicit CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double DeltaTime) override;
	virtual _int LateTick(_double DeltaTime) override;
	virtual HRESULT Render() override;

	//UIDesc�� fX,fY,SizeX,SizeY ���� ������� �׸��� ���� Transform�� ���� ������Ʈ ���ο� �ֱ� ������ ���� �޾ƿ��� �ʰ���
	HRESULT	Set_UI_Transform(_float4 vUIDesc, _uint g_iWinCX, _uint g_iWinCY);
public:
	_float	Get_Depth();

protected:
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	RECT				m_rcRect;
	_float				m_fDepth;
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END