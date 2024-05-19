#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 스레드를 생성한다. */
/* 스레드의 진입점함수를 정의한다. (각 레벨마다 로딩해야할 자원들을 생성한다.) */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	explicit CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CS() {
		return m_CS;
	}

	LEVEL Get_LevelID() const {
		return m_eLevel;
	}

	_bool isFinished() const {
		return m_bIsFinished;
	}

	//로딩바를 위한 함수
public:
	_bool	IsLoadFinished() { return m_bIsFinished; }
	_int	Get_ProgressCount() { return m_iLoadingProgressCount; }
	_int	Get_MaxCount() { return m_iLoadingMaxCount; }

public:
	HRESULT NativeConstruct(LEVEL eNextLevel);
	HRESULT Loading_ForLogo();
	HRESULT	Loading_ForLevel_Stage1();
	HRESULT	Loading_ForLevel_Stage1_BossRoom();
	HRESULT	Loading_ForLevel_Stage2();
	HRESULT	Loading_ForLevel_Stage2_BossRoom();
	HRESULT Loading_ForGamePlay();
	HRESULT Loading_ForBoss();
	HRESULT	Loading_ForMapTool();


private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	
	LEVEL				m_eLevel = LEVEL_END;
	HANDLE				m_hThread;

	CRITICAL_SECTION	m_CS;

	//로딩이 끝낫음을 알려주는 변수
	_bool			m_bIsFinished = false;
	_int			m_iLoadingMaxCount = 1;
	_int			m_iLoadingProgressCount = 0;

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eNextLevel);
	virtual void Free() override;
};

END