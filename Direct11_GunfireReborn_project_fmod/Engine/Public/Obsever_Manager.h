#pragma once

#include "Base.h"

BEGIN(Engine)

class CObsever_Manager final : public CBase
{
	DECLARE_SINGLETON(CObsever_Manager)
private:
	CObsever_Manager();
	virtual ~CObsever_Manager() = default;

public:
	HRESULT Add_ObseverGroup(OBSEVERGROUP eObseverGroup, class CGameObject* _pGameObject); //각 객체들

	//aabb까지 추가하려면 Add_Obsever2로 만들면 될 듯? 콜리전 트리거 함수 만들어서 그게 온이 되면 add_obseverGruop 하는거지

	HRESULT Obsever_Quest(_double DeltaTime); //메인 앱에선 안됨 틱이랑 레이트틱이 하나로 합쳐져서 엔진으로 감 엔진에서 만들것

	HRESULT	Obsever_Complete(_double DeltaTime, CGameObject* _pSorceObject, CGameObject* _pDestObject);

#ifdef _DEBUG
	HRESULT	Obsever_Render(); //각 객체들 내부적으로 렌더 돌릴 수 있게 만들자 그냥 콜라이더 렌더 그대로 쓰면 될 듯?
							//투영행렬이 들어가야하므로 맨 밑에 넣자
							//이거 메인앱에 만들 수 있을듯? 렌더러->렌더 다음에 쓰면 가능할거 같다.
#endif // _DEBUG

	HRESULT	Release_ObseverGroup(); //메인 앱

private:
	list<class CGameObject*>Obsever[OBSEVER_END];

	class CCollider* Collider[COLLIDER_END];

public:
	virtual void Free() override;
};

END