#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	: m_p3D(nullptr) , m_pDevice(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{

}



HRESULT CGraphic_Device::InitDevice(const GRAPHICDESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut)
{
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9			DeviceCaps;
	
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	
	if (FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSGBOX("GetDeviceCaps Failed"); // mfc������ ��밡���� �޼��� �ڽ� �Լ�
		return E_FAIL;
	}



	DWORD	vp = 0;		// ���ؽ� ���μ��� : ���� ���� + ���� ó��

	
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// �׷��� ī��

	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// ���� ����(CPU)

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, GraphicDesc);

	if (FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GraphicDesc.hWnd, vp, &d3dpp, &m_pDevice)))
	{
		MSGBOX("CreateDevice Failed"); // mfc������ ��밡���� �޼��� �ڽ� �Լ�
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSGBOX("m_pSprite Failed");
		return E_FAIL;
	}

	D3DXFONT_DESCW	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ü�");
	
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		MSGBOX("m_pFont Failed");
		return E_FAIL;
	}	

	*ppOut = m_pDevice;

	Safe_AddRef(m_pDevice);

	return S_OK;
}

void CGraphic_Device::SetParameters(D3DPRESENT_PARAMETERS & d3dpp, const GRAPHICDESC& GraphicDesc)
{
	d3dpp.BackBufferWidth = GraphicDesc.iWinCX;
	d3dpp.BackBufferHeight = GraphicDesc.iWinCY;

	// �ĸ� ������ �ȼ� ���� , 32��Ʈ ���� ���
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	
	// �ĸ� ������ ����
	d3dpp.BackBufferCount = 1;
	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GraphicDesc.hWnd;	
	
	d3dpp.Windowed = GraphicDesc.eScreenMode; 

	/* ���� ���ٽ� ���۸� �����Ѵ٤�. */

	
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CGraphic_Device::Render_Begin(void)
{
	
	m_pDevice->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 0, 0, 255),	// ����� ����
		1.f, // z������ �ʱ�ȭ ��
		0);	 // ���ٽ� ������ �ʱ�ȭ ��

	m_pDevice->BeginScene();

}

void CGraphic_Device::Render_End()
{	
	m_pDevice->EndScene();

	// �ĸ���ۿ� ����Ǿ� �ִ� ���ǽ��� ���� ���ۿ� ����� ���ǽ��� ��ȯ�ϴ� ����
	m_pDevice->Present(nullptr, nullptr, 0, nullptr);
	// 1,2���� : ����ü�� ����� D3DSWAPEFFECT_COPY�� �ƴ� �̻� NULL
	// 3���� : ��� ��� ������ �ڵ�, �⺻ nullptr�� ��� d3dpp.hDeviceWindow�� ����
	// 4���� :  ����ü�� ����� D3DSWAPEFFECT_COPY�� �ƴ� �̻� NULL

}

void CGraphic_Device::Free(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	if (0 != Safe_Release(m_pDevice))
		MSGBOX("Failed to Release IDirect3DDevice9");
	
	if (0 != Safe_Release(m_p3D))
		MSGBOX("Failed to Release IDirect3D9");
}
