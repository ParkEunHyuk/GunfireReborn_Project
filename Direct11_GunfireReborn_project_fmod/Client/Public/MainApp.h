#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT NativeConstruct();
	_int Tick(_double TimeDelta);
	HRESULT Render();

	HRESULT Open_Level(LEVEL eLevelIndex);

private:
	CGameInstance*			m_pGameInstance = nullptr;
	CRenderer*				m_pRenderer = nullptr;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;




#ifdef _DEBUG
private:
	_tchar				m_szFPS[MAX_PATH] = TEXT("");
	_ulong				m_dwNumRender = 0;
	_double				m_dTimerAcc = 0.0f;
#endif // _DEBUG

private:
	HRESULT Ready_Prototype_Components();
	HRESULT Ready_Prototype_GameObjects();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END