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
	HRESULT Add_ObseverGroup(OBSEVERGROUP eObseverGroup, class CGameObject* _pGameObject); //�� ��ü��

	//aabb���� �߰��Ϸ��� Add_Obsever2�� ����� �� ��? �ݸ��� Ʈ���� �Լ� ���� �װ� ���� �Ǹ� add_obseverGruop �ϴ°���

	HRESULT Obsever_Quest(_double DeltaTime); //���� �ۿ��� �ȵ� ƽ�̶� ����Ʈƽ�� �ϳ��� �������� �������� �� �������� �����

	HRESULT	Obsever_Complete(_double DeltaTime, CGameObject* _pSorceObject, CGameObject* _pDestObject);

#ifdef _DEBUG
	HRESULT	Obsever_Render(); //�� ��ü�� ���������� ���� ���� �� �ְ� ������ �׳� �ݶ��̴� ���� �״�� ���� �� ��?
							//��������� �����ϹǷ� �� �ؿ� ����
							//�̰� ���ξۿ� ���� �� ������? ������->���� ������ ���� �����Ұ� ����.
#endif // _DEBUG

	HRESULT	Release_ObseverGroup(); //���� ��

private:
	list<class CGameObject*>Obsever[OBSEVER_END];

	class CCollider* Collider[COLLIDER_END];

public:
	virtual void Free() override;
};

END