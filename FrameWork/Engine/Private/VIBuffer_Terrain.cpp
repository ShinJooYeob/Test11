#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{

}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j, 0.0f, (_float)i);
			pVertices[iIndex].vTexUV = _float2((_float)j / (m_iNumVerticesX - 1) * 20.f, (_float)i / (m_iNumVerticesZ - 1)* 20.f);
		}
	}

	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint		iNumPrimitive = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[1];
			pIndices[iNumPrimitive]._2 = iIndices[2];
			++iNumPrimitive;

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[2];
			pIndices[iNumPrimitive]._2 = iIndices[3];
			++iNumPrimitive;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar * szHightFilePath)
{

	HANDLE pFile = CreateFile(szHightFilePath,GENERIC_READ,0,nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (pFile == nullptr)
		return E_FAIL;

	DWORD dwNumReaded= 0;

	BITMAPFILEHEADER		FileHeader;
	BITMAPINFOHEADER		InfoHeader;

	ReadFile(pFile, &FileHeader, sizeof(BITMAPFILEHEADER), &dwNumReaded, nullptr);
	ReadFile(pFile, &InfoHeader, sizeof(BITMAPINFOHEADER), &dwNumReaded, nullptr);

	_ulong*		pPixel = new _ulong[InfoHeader.biHeight * InfoHeader.biWidth];
	ZeroMemory(pPixel, sizeof(_ulong) * InfoHeader.biHeight * InfoHeader.biWidth);

	ReadFile(pFile, pPixel, sizeof(_ulong) * InfoHeader.biHeight * InfoHeader.biWidth, &dwNumReaded, nullptr);

	CloseHandle(pFile);

	if (dwNumReaded <= 0)
	{
		return E_FAIL;
		Safe_Delete_Array(pPixel);
	}


	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = InfoHeader.biHeight * InfoHeader.biWidth;
	m_iNumVerticesX = InfoHeader.biHeight;
	m_iNumVerticesZ = InfoHeader.biWidth;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j, (pPixel[iIndex] & 0x000000ff)*0.1f, (_float)i);
			pVertices[iIndex].vTexUV = _float2((_float)j / (m_iNumVerticesX - 1) * 20.f, (_float)i / (m_iNumVerticesZ - 1)* 20.f);
		}
	}

	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint		iNumPrimitive = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[1];
			pIndices[iNumPrimitive]._2 = iIndices[2];
			++iNumPrimitive;

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[2];
			pIndices[iNumPrimitive]._2 = iIndices[3];
			++iNumPrimitive;
		}
	}

	m_pIB->Unlock();


	Safe_Delete_Array(pPixel);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	return S_OK;
}

_float CVIBuffer_Terrain::PointOnTerrain(_float3 vPos)
{

	_float fResult = 0;

	_uint iIndex[4];

	iIndex[0] = int(vPos.z) * (m_iNumVerticesX) + int(vPos.x);
	iIndex[1] = iIndex[0] + 1;
	iIndex[2] = iIndex[0] + m_iNumVerticesX;
	iIndex[3] = iIndex[2] + 1;
	
	_float3 vPoligon[3];

	VTXTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	
	if (pVertices[iIndex[2]].vPosition.z - vPos.z > vPos.x - pVertices[iIndex[2]].vPosition.x)
		//¾Æ·¡ »ï°¢Çü
	{
		vPoligon[0] = pVertices[iIndex[0]].vPosition;
		vPoligon[1] = pVertices[iIndex[2]].vPosition;
		vPoligon[2] = pVertices[iIndex[1]].vPosition;
	}
	else 
		//À§ »ï°¢Çü
	{
		vPoligon[0] = pVertices[iIndex[2]].vPosition;
		vPoligon[1] = pVertices[iIndex[3]].vPosition;
		vPoligon[2] = pVertices[iIndex[1]].vPosition;
	}
	
	m_pVB->Unlock();


	D3DXPLANE tPlane;
	D3DXPlaneFromPoints(&tPlane, &vPoligon[0], &vPoligon[1], &vPoligon[2]);

	fResult = (tPlane.a * vPos.x + tPlane.c * vPos.z + tPlane.d) / -tPlane.b;





	return fResult;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * szHightFilePath)
{

	if (szHightFilePath == nullptr)
		return nullptr;

	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(szHightFilePath)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

}
