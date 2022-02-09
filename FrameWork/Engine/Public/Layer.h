#pragma once

#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
public:
	CLayer();
	virtual ~CLayer() = default;
public:
	HRESULT			Add_GameObject(class CGameObject* pGameObject);
	CGameObject*	Get_GameObjet();
	_int Tick(_float fTimeDelta);
	_int LateTick(_float fTimeDelta);

private:
	list<class CGameObject*>			m_Objects;
	typedef list<class CGameObject*>	OBJECTS;
public:
	static CLayer* Create();
	virtual void Free();
};

END