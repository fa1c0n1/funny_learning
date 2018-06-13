// Pack.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "ShPack.h"


// ���ǵ���������һ��ʾ��
PACK_API int nPack = 0;

// ���ǵ���������һ��ʾ����
PACK_API int fnPack(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� Pack.h
CShPack::CShPack()
{
	return;
}

#include <Psapi.h>
#pragma comment(lib,"psapi.lib")
#include "../ShStub/ShStub.h"
#ifdef DEBUG
#pragma comment(lib,"../Debug/ShStub.lib")
#endif // DEBUG
#ifdef NDEBUG
#pragma comment(lib,"../Release/ShStub.lib")
#endif // NDEBUG

#include "PE.h"
BOOL Pack(CString szPath, BYTE byXor)
{
	// 1.��ȡĿ���ļ�PE��Ϣ
	CPE objPE;
	objPE.InitPE(szPath);
	BOOL bRet = FALSE;

	//���ܴ����, ���ش���εĻ�ַRVA
	DWORD dwVirtualAddr = objPE.XorCode(byXor);

	//2. ��ȡStub�ļ�PE��Ϣ,����Ҫ����Ϣ���õ�Stub��
	HMODULE hMod = LoadLibrary(L"ShStub.dll");
	PGLOBAL_PARAM pstcParam = (PGLOBAL_PARAM)GetProcAddress(hMod, "g_stcParam");
	pstcParam->dwImageBase = objPE.m_dwImageBase;
	pstcParam->dwCodeSize = objPE.m_dwCodeSize;
	pstcParam->dwOEP = objPE.m_dwOEP;
	pstcParam->byXor = byXor;
	pstcParam->lpStartVA = (PBYTE)dwVirtualAddr;

	// 3. ���Stub����ε����ӿǳ�����
	// 3.1 ��ȡStub�����
	MODULEINFO modinfo = { 0 };
	GetModuleInformation(GetCurrentProcess(), hMod, &modinfo, sizeof(MODULEINFO));
	PBYTE  lpMod = new BYTE[modinfo.SizeOfImage];
	memcpy_s(lpMod, modinfo.SizeOfImage, hMod, modinfo.SizeOfImage);
	PBYTE pCodeSection = NULL;
	DWORD dwCodeBaseRVA = 0;
	DWORD dwSize = objPE.GetSectionData(lpMod, 0, pCodeSection, dwCodeBaseRVA);

	// 3.2 �޸�Stub���еĴ���
	objPE.FixReloc(lpMod, pCodeSection, objPE.m_dwNewSectionRVA);

	// 3.3 �޸ı��ӿǳ����OEP��ָ��stub
	DWORD dwStubOEPRVA = pstcParam->dwStart - (DWORD)hMod;
	DWORD dwNewOEP = dwStubOEPRVA - dwCodeBaseRVA;
	//StubOEP = dwStubOEPRVA - ԭRVA + �����ε�RVA;
	objPE.SetNewOEP(dwNewOEP);
	//objPE.ClearRandBase();
	objPE.ClearBundleImport();//�����а��������ĳ���

	printf("aaaaaaaaaaaaaaaaaaaa\n");
	// 3.4 ��ȡ�ض�λ����Ϣ���޸�����
	if (objPE.AddSection(pCodeSection, dwSize, ".Soho", true))
	{
		bRet = TRUE;
	}

	printf("bbbbbbbbbbbbbbbbbbbb\n");
	PBYTE pNewRelocSection = NULL;
	DWORD dwNewRelocTableSize = 0;
	objPE.ChangeReloc(lpMod, pNewRelocSection, dwNewRelocTableSize);

	//-------------д���ļ�-----------------
	//��������ļ�·��
	CString strPath = objPE.m_objFile.GetFilePath();
	TCHAR szOutPath[MAX_PATH] = { 0 };
	LPWSTR strSuffix = PathFindExtension(strPath);                     // ��ȡ�ļ��ĺ�׺��
	wcsncpy_s(szOutPath, MAX_PATH, strPath, wcslen(strPath));          // ����Ŀ���ļ�·����szOutPath
	PathRemoveExtension(szOutPath);                                    // ��szOutPath�б���·���ĺ�׺��ȥ��
	wcscat_s(szOutPath, MAX_PATH, L"_SohoPack");                       // ��·����󸽼�"_SohoPack"
	wcscat_s(szOutPath, MAX_PATH, strSuffix);                          // ��·����󸽼Ӹոձ���ĺ�׺��

	//�����ļ�
	CFile objFile(szOutPath, CFile::modeCreate | CFile::modeReadWrite);
	objFile.Write(objPE.m_pFileBase, objPE.m_dwFileSize);
	objFile.Write(pCodeSection, dwSize);
	objFile.Write(pNewRelocSection, dwNewRelocTableSize);
	objFile.Close();

	// �ͷ���Դ
	if (pNewRelocSection != NULL) {
		delete[] pNewRelocSection;
		pNewRelocSection = NULL;
	}

	if (lpMod != NULL) {
		delete[] lpMod;
		lpMod = NULL;
	}

	return bRet;
}