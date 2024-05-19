#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL  CAnimBlending final : public CBase
{
public:
	CAnimBlending();
	virtual ~CAnimBlending() = default;

public:
	HRESULT Set_Blending(class CAnimation* _sourAnim, class CAnimation* _destAnim);
	_bool	Update_TranformMatrices(_double TimeDelta);
	const	_bool& Get_Blending_Finished() const { return m_bIsFinished; }

private:
	// 애니메이션
	class CAnimation* m_pSourAnim = nullptr;
	class CAnimation* m_pDestAnim = nullptr;

	// 애니메이션의 채널
	const vector<class CChannel*>* m_pSourAnimChannels = nullptr;
	const vector<class CChannel*>* m_pDestAnimChannels = nullptr;

	_uint m_iNumSourChannel = 0;
	_uint m_iNumDestChannel = 0;

	// 키프레임
	KEYFRAME m_SourKeyFrame;
	KEYFRAME m_DestKeyFrame;

	_vector m_vSourScale, m_vSourRotation, m_vSourPosition;
	_vector m_vDestScale, m_vDestRotation, m_vDestPosition;
	_vector m_vScale, m_vRotation, m_vPosition;

	_float   m_fLerpRatio = 0.01f;
	_bool   m_bIsFinished = FALSE;
	_bool	m_Origin = false;

public:
	static	CAnimBlending*	Create();
	virtual void Free() override;
};

END