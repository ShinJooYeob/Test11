#include "..\Public\Layer.h"

#include "GameObject.h"


CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	m_ObjectList.push_back(pGameObject);
	return S_OK;
}

_int CLayer::Update(_float fDeltaTime)
{
	for (auto& pGameObject : m_ObjectList) 
	{

		if (pGameObject->Update(fDeltaTime) < 0) 
			return -1;

	}

	return _int();
}

_int CLayer::LateUpdate(_float fDeltaTime)
{
	for (auto& pGameObject : m_ObjectList) {

		if (pGameObject->LateUpdate(fDeltaTime) < 0)
			return -1;

	}
	return _int();
}



CLayer * CLayer::Create()
{
	CLayer* pLayer = new CLayer();
	return pLayer;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList)
	{
		Safe_Release(pGameObject);
	}
	m_ObjectList.clear();

}
