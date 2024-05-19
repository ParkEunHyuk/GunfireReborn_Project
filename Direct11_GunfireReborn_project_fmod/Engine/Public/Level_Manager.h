#pragma once

#include "Base.h"

/* ���� ȭ�鿡 ��������� ������ �ּҸ� �����Ѵ�. */
/* ���� ��ü�ÿ� ���� ������ �����ϰ�(�����ϰ�), ���ο� ������ �����Ѵ�. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;
public:
	HRESULT OpenLevel(_uint iLevelIndex, class CLevel* pNextLevel);
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);
	HRESULT Render();

public:
	_uint	Get_NowLevelIndex() { return (_uint)m_iNowLevelIndex; }
	void	Set_NowLevelIndex(_int iLevelNum) { m_iNowLevelIndex = iLevelNum; }
	CLevel*				Get_Level();

private:
	_uint					m_iCurrentLevelIndex = 0;
	class CLevel*			m_pCurrentLevel = nullptr;

	_int				m_iNowLevelIndex = 0;

public:
	virtual void Free() override;
};

END