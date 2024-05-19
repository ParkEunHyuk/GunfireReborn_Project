#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* �δ��� ��üȭ�Ͽ� �����ϳ�. */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
public:
	explicit CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Loading() = default;
public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Layer_Loading(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Loading_Bar(const _tchar* pLayerTag);

private:
	LEVEL				m_eNextLevel = LEVEL_END;
	class CLoader*		m_pLoader = nullptr;

public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eNextLevel);
	virtual void Free() override;
};

END