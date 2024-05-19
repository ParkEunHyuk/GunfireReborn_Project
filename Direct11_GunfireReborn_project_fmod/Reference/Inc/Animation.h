#pragma once

#include "Base.h"

/* 하나의 애니메이션 */

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	void Reserve(_uint iNumChannels) {
		m_Channels.reserve(iNumChannels);
		m_iNumChannels = iNumChannels;
	}

	const vector<class CChannel*>* Get_Channels() const {
		return &m_Channels;
	}
public:
	HRESULT NativeConstruct(const char* pName, _double Duration, _double TickPerSecond);
	HRESULT Update_TransformMatrices(_double TimeDelta);
	HRESULT Update_TransformMatrices(_double TimeDelta,_bool& _bOneTimeSwith);
	_bool	Update_Once(_double TimeDelta);

	void Add_Channel(class CChannel* pChannel) {
		m_Channels.push_back(pChannel);
	}

	//테스트중입니다.!!!!!!!!!!!!!!!
public:
	_uint	Get_Chennal_Num() { return m_iNumChannels; }

	void Reset();//추가됨

	//////////////////////////

private:
	char		m_szName[MAX_PATH] = "";

	/* 전체재생시간. */
	_double		m_Duration = 0.0;

	/* 시간당 애니메이션 재생 속도. */
	_double		m_TickPerSecond = 0.0;

	_double		m_PlayTimeAcc = 0.0;

	/* 이 애님에ㅣ션을 재생하는데 사용하는 뼈의 갯수. */
	_uint		m_iNumChannels = 0;

	/* 현재 애니메이션을 표현하기위해 사용하는 뼈들. */
	vector<class CChannel*>			m_Channels;
	typedef vector<class CChannel*>	CHANNELS;

	_bool		m_bIsFinished = false;


public:
	static CAnimation* Create(const char* pName, _double Duration, _double TickPerSecond);
	virtual void Free() override;
};

END