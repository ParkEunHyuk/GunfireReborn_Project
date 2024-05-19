#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Timer_Manager.h"
#include "Component_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "PipeLine.h"
#include "Light_Manager.h"
#include "Picking.h"
#include "Obsever_Manager.h"
#include "EasingMgr.h"
#include "Frustum_Manager.h"
#include "Target_Manager.h"


BEGIN(Engine)

class CSound_Manager;

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(HINSTANCE hInstance, _uint iNumLevels, const CGraphic_Device::GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut, _float fDoubleInterver = 0.3f);
	_int Tick_Engine(_double TimeDelta);	
	HRESULT Clear_LevelResource(_uint iLevelIndex);

public: /* For.Graphic_Device */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState);
	_byte Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);

public: /* For.Timer_Manager */
	_double	Get_TimeDelta(const _tchar* pTimerTag);
	HRESULT Add_Timer(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT OpenLevel(_uint iLevelIndex, class CLevel* pNextLevel);
	HRESULT Render_Level();
	_uint	Get_NowLevelIndex();
	void	Set_NowLevelIndex(_int iLevelNum);
	CLevel*	Get_Level();

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: /* For.Object_Manager */
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iLayerIndex = 0);
	class list<CGameObject*>* Get_ObjectList_from_Layer(_uint iLevelIndex, const _tchar* tagLayer);

public:	/* For.PipeLine */
	HRESULT	Set_Transform(CPipeLine::TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix);
	_matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATETYPE eStateType);
	_float4x4 Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATETYPE eStateType);
	_float4x4 Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATETYPE eStateType);
	_vector Get_CamPosition();


public: /* For.Sound_Manager */
	_int  Channel_VolumeUp(CHANNELID eID, _float _vol);
	_int  Channel_VolumeDown(CHANNELID eID, _float _vol);
	_int  Channel_Pause(CHANNELID eID);

	HRESULT PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float fLouderMultiple = 1.f);
	HRESULT PlayBGM(TCHAR* pSoundKey, _float fLouderMultiple = 1.f);
	void Stop_ChannelSound(CHANNELID eID);
	void Stop_AllChannel();

	_float  Get_Channel_Volume(CHANNELID eID);
	_bool  Get_Channel_IsPaused(CHANNELID eID);

public: /* For.Light_Manager */
	const LIGHTDESC*	Get_LightDesc(_uint iIndex) const;
	HRESULT	Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);
	HRESULT	Release_Light();


public: /* For.Obsever_Manager */
	HRESULT	Add_ObseverGroup(OBSEVERGROUP eObseverGroup, class CGameObject* _pGameObject);
	HRESULT	Obsever_Quest(_double DeltaTime);
#ifdef _DEBUG
	HRESULT	Obsever_Render();
#endif // _DEBUG
	HRESULT	Release_ObseverGroup();


public: /* For. Picking */
	HRESULT Transform_ToLocalSpace(_float4x4 WorldMatrixInverse);
	_bool	isPick(_float3* pLocalPoints, _float3* pOut = nullptr);
	_float3& Get_RayDirInLocal();
	_float3& Get_RayPosInLocal();


public:/* For.Easing */
	_vector Easing(_uint eEasingType, _vector vStartPoint, _vector vTargetPoint,  _double dPassedTime, _double dTotalTime = 1.0f);


public: /* For.Frustum */
	_bool isIn_WorldSpace(_fvector vPoint, _float fRange = 0.f);

public: /* For.Target_Manager */



private:	
	CGraphic_Device*			m_pGraphic_Device = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
	CPipeLine*					m_pPipeLine = nullptr;
	CLight_Manager*				m_pLight_Manager = nullptr;
	CSound_Manager*				m_pSound_Manager = nullptr;
	CPicking*					m_pPicking = nullptr;
	CObsever_Manager*			m_pObsever_Manager = nullptr;
	CEasingMgr*					m_pEasingMgr = nullptr;
	CFrustum_Manager*			m_pFrustum_Manager = nullptr;
	CTarget_Manager*			m_pTarget_Manager = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;

//IMGUI========================================================================================================================================================================================
#if defined(USE_IMGUI)

	HRESULT Clear_LevelObject_For_Editer(_uint iLevelIndex);

	map<const _tchar*, class CGameObject*>* Get_Prototypes()
	{
		return m_pObject_Manager->Get_Prototypes();
	}

	map<const _tchar*, class CLayer*>* Get_Layers()
	{
		return m_pObject_Manager->Get_Layers();
	}
#endif
//IMGUI========================================================================================================================================================================================
};

END