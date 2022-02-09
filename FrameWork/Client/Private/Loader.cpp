#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"

#include "Player.h"
#include "Terrain.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY ThreadFunction(void* pArg)
{	
	/* CLoader��ü�ȿ� �ε��� �ʿ��� �����͵��� ��Ƴ��µ� �װ� �����? */
	/* CLoader��ü�� �ּҸ� ������������ �����Ͱ���. */
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	switch (pLoader->Get_LevelID())
	{
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlay();
		break;
	case LEVEL_BOSS:
		pLoader->Loading_ForBoss();
		break;
	}	

	LeaveCriticalSection(&pLoader->Get_CS());	

	return 0;
}



HRESULT CLoader::NativeConstruct(LEVEL eNextLevel)
{
	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunction, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	m_eLevel = eNextLevel;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlay()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* ����������Ʈ�� �����Ѵ�. */
#pragma region PROTOTYPE_COMPONENT

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga"),2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/idle_ani.png")))))
		return E_FAIL;

		/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;	

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	/* ������ü�� �����Ѵ�. */
	/* For.Prototype_GameObject_Terrain */


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForBoss()
{
	for (_uint i = 0; i < 9999999999; ++i)
		int a = 10;

	m_isFinished = true;
	
	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CLoader*	pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSGBOX("Failed to Creating CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteObject(m_hThread);

	DeleteCriticalSection(&m_CS);

	Safe_Release(m_pGraphic_Device);


}
