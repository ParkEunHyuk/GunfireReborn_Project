#pragma once

#include "Base.h"

/* �ϳ��� �ִϸ��̼� */

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

	//�׽�Ʈ���Դϴ�.!!!!!!!!!!!!!!!
public:
	_uint	Get_Chennal_Num() { return m_iNumChannels; }

	void Reset();//�߰���

	//////////////////////////

private:
	char		m_szName[MAX_PATH] = "";

	/* ��ü����ð�. */
	_double		m_Duration = 0.0;

	/* �ð��� �ִϸ��̼� ��� �ӵ�. */
	_double		m_TickPerSecond = 0.0;

	_double		m_PlayTimeAcc = 0.0;

	/* �� �ִԿ��Ӽ��� ����ϴµ� ����ϴ� ���� ����. */
	_uint		m_iNumChannels = 0;

	/* ���� �ִϸ��̼��� ǥ���ϱ����� ����ϴ� ����. */
	vector<class CChannel*>			m_Channels;
	typedef vector<class CChannel*>	CHANNELS;

	_bool		m_bIsFinished = false;


public:
	static CAnimation* Create(const char* pName, _double Duration, _double TickPerSecond);
	virtual void Free() override;
};

END