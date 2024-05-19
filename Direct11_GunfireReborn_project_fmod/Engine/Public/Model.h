#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum MODELTYPE { TYPE_ANIM, TYPE_NONANIM, TYPE_END };

public:
	typedef struct tagBoneMatrixPtr
	{
		_float4x4*			pOffsetMatrix;
		_float4x4*			pCombinedMatrix;
	}BONEMATRIX_PTR;

private:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMaterials() const {
		return m_iNumMaterials;
	}

	BONEMATRIX_PTR Get_BoneMatrixPtr(const char* pBoneName);
	_float4x4 Get_TransformMatrix() {
		return m_TransformMatrix;
	}

public:
	virtual HRESULT NativeConstruct_Prototype(MODELTYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT Update_CombinedTransformationMatrices(_double TimeDelta,_uint _iCurrentAnim, _uint _iNextAnim,_bool& _pSwitch); //true면 돌아가고 있단 뜻 false면 안돌아가고 있단 뜻
	HRESULT Bind_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pValueName);
	HRESULT Render(class CShader* pShader, _uint iPassIndex, _uint iMaterialIndex, const char* pBoneValueName = nullptr);

private:
	const aiScene*				m_pScene = nullptr;
	Importer					m_Importer;

private:
	_uint									m_iNumMeshContainers = 0;
	vector<class CMeshContainer*>*			m_pMeshContainers = nullptr;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;
	MODELTYPE								m_eModelType = TYPE_END;
	_float4x4								m_TransformMatrix;

private:
	_uint									m_iNumMaterials = 0;
	vector<MESHMATERIALDESC*>				m_Materials;
	typedef vector<MESHMATERIALDESC*>		MATERIALS;

private:
	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;

private:
	_uint									m_iNumAnimations;
	vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;

private:
	HRESULT Ready_MeshContainers();
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth);
	HRESULT Ready_OffsetMatrices();
	HRESULT Ready_Animation();
	HRESULT Link_ChannelToNode();

public:
	//테스트중입니다.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	HRESULT	SetUp_AnimIndex(_uint iAnimIndex);
	HRESULT Update_CombinedTransformationMatrices(_double TimeDelta,_bool& _bOneTimeSwith);
	vector<class CChannel*>					m_SaveChannel;
	class CAnimBlending*					m_AnimBlending = nullptr;

	_uint									m_iCurrentAnim = 0;
	_uint									m_iNextAnim = 0;
	_double									m_AnimTime = 0.0;

	/////////////////////////////////////////

private:
	CHierarchyNode* Find_HierarchyNode(const char* pName, _uint* pOut = nullptr);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, MODELTYPE eModelType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END