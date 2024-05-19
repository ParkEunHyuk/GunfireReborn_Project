#include "..\Public\AnimBlending.h"
#include "Animation.h"
#include "Channel.h"


CAnimBlending::CAnimBlending()
{
}

HRESULT CAnimBlending::Set_Blending(CAnimation * _sourAnim, CAnimation * _destAnim)
{
	if (nullptr == _sourAnim ||
		nullptr == _destAnim)
		return E_FAIL;

	m_pSourAnim = _sourAnim;
	m_pDestAnim = _destAnim;

	m_pSourAnimChannels = m_pSourAnim->Get_Channels();
	m_pDestAnimChannels = m_pDestAnim->Get_Channels();

	m_iNumSourChannel = m_pSourAnim->Get_Chennal_Num();
	m_iNumDestChannel = m_pDestAnim->Get_Chennal_Num();

	m_bIsFinished = false;
	m_fLerpRatio = 0.01f;

	return S_OK;
}

_bool CAnimBlending::Update_TranformMatrices(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumSourChannel; ++i)
	{
		for (_uint j = 0; j < m_iNumDestChannel; ++j)
		{
			if (!strcmp((*m_pSourAnimChannels)[i]->Get_Name(), (*m_pDestAnimChannels)[j]->Get_Name())) // ä�� �̸� ��
			{
				m_SourKeyFrame = (*m_pSourAnimChannels)[i]->Get_LastUsedKeyFrame(); // ������ Ű ������(���� �ִϸ��̼� ������ ���� ����)
				m_DestKeyFrame = *(*m_pDestAnimChannels)[j]->Get_FirstKeyFrame(); // ���� Ű ������(�Ѿ �ִϸ��̼�)
				m_Origin = true;
			}
			else
			{
				m_SourKeyFrame = (*m_pSourAnimChannels)[i]->Get_LastUsedKeyFrame(); // ������ Ű ������(���� �ִϸ��̼� ������ ���� ����
			}
			/* ���� ������ : ä���� �̸��� �ٸ���, ���� ��ȣ�� ä�ΰ� ������. (�Ƹ�...?) */
		}

		m_vSourScale = XMLoadFloat3(&m_SourKeyFrame.vScale);
		m_vSourRotation = XMLoadFloat4(&m_SourKeyFrame.vRotation);
		m_vSourPosition = XMLoadFloat3(&m_SourKeyFrame.vPosition);

		if (true == m_Origin) // Sour == Dest
		{
			m_vDestScale = XMLoadFloat3(&m_DestKeyFrame.vScale);
			m_vDestRotation = XMLoadFloat4(&m_DestKeyFrame.vRotation);
			m_vDestPosition = XMLoadFloat3(&m_DestKeyFrame.vPosition);
		}
		else if (false == m_Origin) // Sour != Dest
		{
			m_vDestScale = XMLoadFloat3(&m_SourKeyFrame.vScale);
			m_vDestRotation = XMLoadFloat4(&m_SourKeyFrame.vRotation);
			m_vDestPosition = XMLoadFloat3(&m_SourKeyFrame.vPosition);
		}


		m_vScale = XMVectorLerp(m_vSourScale, m_vDestScale, m_fLerpRatio);
		m_vRotation = XMQuaternionSlerp(m_vSourRotation, m_vDestRotation, m_fLerpRatio);
		m_vPosition = XMVectorLerp(m_vSourPosition, m_vDestPosition, m_fLerpRatio);

		m_vPosition = XMVectorSetW(m_vPosition, 1.f);
		_matrix   TransformationMatrix = XMMatrixAffineTransformation(m_vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), m_vRotation, m_vPosition);
		(*m_pSourAnimChannels)[i]->Set_TransformationMatrix(TransformationMatrix);
		m_Origin = false;

	}

	m_fLerpRatio += 5.f * (_float)TimeDelta;//1�� ��� ������ 0.2�� �Ѵ� 5�ϱ� 1

	// ���ϴ¸�ŭ�� �����۾��� ������ m_bIsFinished �� TRUE �� �ٲٱ�
	if (m_fLerpRatio >= 1.f)
	{
		m_bIsFinished = TRUE;
		return TRUE;
	}

	return FALSE;


	//if (m_iNumSourChannel <= m_iNumDestChannel) // sour�ִϸ��̼��� ä�� ���� dest�ִϸ��̼��� ä�� ������ ���ų� ���� ��
	//{
	//	for (_uint i = 0; i < m_iNumSourChannel; ++i)
	//	{
	//		m_SourKeyFrame = (*m_pSourAnimChannels)[i]->Get_LastUsedKeyFrame();
	//		m_DestKeyFrame = *(*m_pDestAnimChannels)[i]->Get_FirstKeyFrame();

	//		m_vSourScale = XMLoadFloat3(&m_SourKeyFrame.vScale);
	//		m_vSourRotation = XMLoadFloat4(&m_SourKeyFrame.vRotation);
	//		m_vSourPosition = XMLoadFloat3(&m_SourKeyFrame.vPosition);

	//		m_vDestScale = XMLoadFloat3(&m_DestKeyFrame.vScale);
	//		m_vDestRotation = XMLoadFloat4(&m_DestKeyFrame.vRotation);
	//		m_vDestPosition = XMLoadFloat3(&m_DestKeyFrame.vPosition);

	//		m_vScale = XMVectorLerp(m_vSourScale, m_vDestScale, m_fLerpRatio);
	//		m_vRotation = XMQuaternionSlerp(m_vSourRotation, m_vDestRotation, m_fLerpRatio);
	//		m_vPosition = XMVectorLerp(m_vSourPosition, m_vDestPosition, m_fLerpRatio);

	//		m_vPosition = XMVectorSetW(m_vPosition, 1.f);

	//		_matrix   TransformationMatrix = XMMatrixAffineTransformation(m_vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), m_vRotation, m_vPosition);

	//		(*m_pSourAnimChannels)[i]->Set_TransformationMatrix(TransformationMatrix);
	//	}
	//}
	//else // sour�ִϸ��̼��� ä�� ���� dest�ִϸ��̼��� ä�� ������ ���� ��
	//{
	//	for (_uint i = 0; i < m_iNumDestChannel; ++i) // �ϴ� dest�ִϸ��̼��� ä�� ����ŭ ����
	//	{
	//		m_SourKeyFrame = (*m_pSourAnimChannels)[i]->Get_LastUsedKeyFrame();
	//		m_DestKeyFrame = *(*m_pDestAnimChannels)[i]->Get_FirstKeyFrame();

	//		m_vSourScale = XMLoadFloat3(&m_SourKeyFrame.vScale);
	//		m_vSourRotation = XMLoadFloat4(&m_SourKeyFrame.vRotation);
	//		m_vSourPosition = XMLoadFloat3(&m_SourKeyFrame.vPosition);

	//		m_vDestScale = XMLoadFloat3(&m_DestKeyFrame.vScale);
	//		m_vDestRotation = XMLoadFloat4(&m_DestKeyFrame.vRotation);
	//		m_vDestPosition = XMLoadFloat3(&m_DestKeyFrame.vPosition);

	//		m_vScale = XMVectorLerp(m_vSourScale, m_vDestScale, m_fLerpRatio);
	//		m_vRotation = XMQuaternionSlerp(m_vSourRotation, m_vDestRotation, m_fLerpRatio);
	//		m_vPosition = XMVectorLerp(m_vSourPosition, m_vDestPosition, m_fLerpRatio);

	//		m_vPosition = XMVectorSetW(m_vPosition, 1.f);

	//		_matrix   TransformationMatrix = XMMatrixAffineTransformation(m_vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), m_vRotation, m_vPosition);

	//		(*m_pSourAnimChannels)[i]->Set_TransformationMatrix(TransformationMatrix);
	//	}

	//	for (_uint i = m_iNumDestChannel; i < m_iNumSourChannel; ++i) // �� �������ʹ� �׳� 0 �̶� ����
	//	{
	//		m_SourKeyFrame = (*m_pSourAnimChannels)[i]->Get_LastUsedKeyFrame();

	//		m_vSourScale = XMLoadFloat3(&m_SourKeyFrame.vScale);
	//		m_vSourRotation = XMLoadFloat4(&m_SourKeyFrame.vRotation);
	//		m_vSourPosition = XMLoadFloat3(&m_SourKeyFrame.vPosition);

	//		m_vDestScale = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	//		m_vDestRotation = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	//		m_vDestPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	//		m_vScale = XMVectorLerp(m_vSourScale, m_vDestScale, m_fLerpRatio);
	//		m_vRotation = XMQuaternionSlerp(m_vSourRotation, m_vDestRotation, m_fLerpRatio);
	//		m_vPosition = XMVectorLerp(m_vSourPosition, m_vDestPosition, m_fLerpRatio);

	//		m_vPosition = XMVectorSetW(m_vPosition, 1.f);

	//		_matrix   TransformationMatrix = XMMatrixAffineTransformation(m_vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), m_vRotation, m_vPosition);

	//		(*m_pSourAnimChannels)[i]->Set_TransformationMatrix(TransformationMatrix);
	//	}
	//}

	//m_fLerpRatio += 5.f * (_float)TimeDelta;

	//// ���ϴ¸�ŭ�� �����۾��� ������ m_bIsFinished �� TRUE �� �ٲٱ�
	//if (m_fLerpRatio >= 1.f)
	//{
	//	m_bIsFinished = TRUE;
	//	return TRUE;
	//}

	//return FALSE;
}

CAnimBlending * CAnimBlending::Create()
{
	CAnimBlending* pInstance = new CAnimBlending();
	return pInstance;
}

void CAnimBlending::Free()
{
}