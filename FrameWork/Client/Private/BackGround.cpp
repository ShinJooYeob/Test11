#include "stdafx.h"
#include "..\Public\BackGround.h"
#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBackGround::CBackGround(const CBackGround & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBackGround::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Scaled(_float3(2.f, 2.f, 2.f));

	return S_OK;
}

_int CBackGround::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	return _int();
}

_int CBackGround::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	return _int();
}

HRESULT CBackGround::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBackGround::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround*	pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround::Clone(void* pArg )
{
	/* ���οü�� �����Ͽ� �����Ѵ�. */
	CBackGround*	pInstance = new CBackGround(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom); 
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
