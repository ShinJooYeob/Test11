#pragma once

#include "Base.h"

/* Ŭ���̾�Ʈ���� ������ �������� �θ� �Ǵ� Ŭ������. */

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
public:
	explicit CLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
public:
	virtual void Free() override;
};

END