#pragma once

#include "Base.h"


/* 특정 애님에ㅣ션에서 사용되는 뼈. */

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	CChannel();
	virtual ~CChannel() = default;

public:
	const vector<KEYFRAME*>* Get_KeyFrames() const {
		return &m_KeyFrames;
	}
	_uint Get_CurrentKeyFrame() const {
		return m_iCurrentKeyFrame;
	}

	const char* Get_Name() const {
		return m_szName;
	}

	void Set_CurrentKeyFrame(_uint iKeyFrameIndex) {
		m_iCurrentKeyFrame = iKeyFrameIndex;
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix);

	void Set_HierarchyNodePtr(class CHierarchyNode* pNode);

public:
	void Reserve(_uint iNumKeyFrames) {
		m_KeyFrames.reserve(iNumKeyFrames);
		m_iNumKeyFrames = iNumKeyFrames;
	}
	void Add_KeyFrame(KEYFRAME* pKeyFrame) {
		m_KeyFrames.push_back(pKeyFrame);
	}

public:
	HRESULT NativeConstruct(const char* pName/*, class CHierarchyNode* pNode*/);

	//테스트중입니다!!!!!!!!!!!!!!
	void	Set_SaveFrame(_fvector _vScale, _fvector _vRotation, _fvector _vPosition)
	{
		XMStoreFloat3(&m_LastKeyFrame.vScale, _vScale);
		XMStoreFloat4(&m_LastKeyFrame.vRotation, _vRotation);
		XMStoreFloat3(&m_LastKeyFrame.vPosition, _vPosition);
		m_LastKeyFrame.Time = 0.0;
	}
	//추가됨
	KEYFRAME	Get_LastUsedKeyFrame() const {
		return m_LastKeyFrame;
	}
	//추가됨
	KEYFRAME*	Get_FirstKeyFrame() { return m_KeyFrames[0]; }

	KEYFRAME					m_LastKeyFrame;//추가됨
	/////////////////////

private:
	char						m_szName[MAX_PATH] = "";

	_uint						m_iCurrentKeyFrame = 0;
	_uint						m_iNumKeyFrames = 0;
	vector<KEYFRAME*>			m_KeyFrames;
	typedef vector<KEYFRAME*>	KEYFRAME;

	CHierarchyNode*				m_pNode = nullptr;

public:
	static CChannel* Create(const char* pName/*, class CHierarchyNode* pNode*/);
	virtual void Free() override;
};

END