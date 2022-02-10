#pragma once

#include "Base.h"


BEGIN(Engine)

class CGameObject;

class CLayer :public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);

	_int Update(_float fDeltaTime);
	_int LateUpdate(_float fDeltaTime);


private:
	list< CGameObject*>			m_ObjectList;
	typedef list< CGameObject*>	OBJECTS;

public:
	static CLayer*	Create();
	virtual void Free() override;
};

END