// Pack.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ShPack.h"


// 这是导出变量的一个示例
PACK_API int nPack = 0;

// 这是导出函数的一个示例。
PACK_API int fnPack(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 Pack.h
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
	// 1.获取目标文件PE信息
	CPE objPE;
	objPE.InitPE(szPath);
	BOOL bRet = FALSE;

	//加密代码段, 返回代码段的基址RVA
	DWORD dwVirtualAddr = objPE.XorCode(byXor);

	//2. 获取Stub文件PE信息,将必要的信息设置到Stub中
	HMODULE hMod = LoadLibrary(L"ShStub.dll");
	PGLOBAL_PARAM pstcParam = (PGLOBAL_PARAM)GetProcAddress(hMod, "g_stcParam");
	pstcParam->dwImageBase = objPE.m_dwImageBase;
	pstcParam->dwCodeSize = objPE.m_dwCodeSize;
	pstcParam->dwOEP = objPE.m_dwOEP;
	pstcParam->byXor = byXor;
	pstcParam->lpStartVA = (PBYTE)dwVirtualAddr;

	// 3. 添加Stub代码段到被加壳程序中
	// 3.1 读取Stub代码段
	MODULEINFO modinfo = { 0 };
	GetModuleInformation(GetCurrentProcess(), hMod, &modinfo, sizeof(MODULEINFO));
	PBYTE  lpMod = new BYTE[modinfo.SizeOfImage];
	memcpy_s(lpMod, modinfo.SizeOfImage, hMod, modinfo.SizeOfImage);
	PBYTE pCodeSection = NULL;
	DWORD dwCodeBaseRVA = 0;
	DWORD dwSize = objPE.GetSectionData(lpMod, 0, pCodeSection, dwCodeBaseRVA);

	// 3.2 修复Stub段中的代码
	objPE.FixReloc(lpMod, pCodeSection, objPE.m_dwNewSectionRVA);

	// 3.3 修改被加壳程序的OEP，指向stub
	DWORD dwStubOEPRVA = pstcParam->dwStart - (DWORD)hMod;
	DWORD dwNewOEP = dwStubOEPRVA - dwCodeBaseRVA;
	//StubOEP = dwStubOEPRVA - 原RVA + 新区段的RVA;
	objPE.SetNewOEP(dwNewOEP);
	//objPE.ClearRandBase();
	objPE.ClearBundleImport();//兼容有绑定输入表项的程序

	printf("aaaaaaaaaaaaaaaaaaaa\n");
	// 3.4 读取重定位的信息，修复代码
	if (objPE.AddSection(pCodeSection, dwSize, ".Soho", true))
	{
		bRet = TRUE;
	}

	printf("bbbbbbbbbbbbbbbbbbbb\n");
	PBYTE pNewRelocSection = NULL;
	DWORD dwNewRelocTableSize = 0;
	objPE.ChangeReloc(lpMod, pNewRelocSection, dwNewRelocTableSize);

	//-------------写入文件-----------------
	//生成输出文件路径
	CString strPath = objPE.m_objFile.GetFilePath();
	TCHAR szOutPath[MAX_PATH] = { 0 };
	LPWSTR strSuffix = PathFindExtension(strPath);                     // 获取文件的后缀名
	wcsncpy_s(szOutPath, MAX_PATH, strPath, wcslen(strPath));          // 备份目标文件路径到szOutPath
	PathRemoveExtension(szOutPath);                                    // 将szOutPath中保存路径的后缀名去掉
	wcscat_s(szOutPath, MAX_PATH, L"_SohoPack");                       // 在路径最后附加"_SohoPack"
	wcscat_s(szOutPath, MAX_PATH, strSuffix);                          // 在路径最后附加刚刚保存的后缀名

	//创建文件
	CFile objFile(szOutPath, CFile::modeCreate | CFile::modeReadWrite);
	objFile.Write(objPE.m_pFileBase, objPE.m_dwFileSize);
	objFile.Write(pCodeSection, dwSize);
	objFile.Write(pNewRelocSection, dwNewRelocTableSize);
	objFile.Close();

	// 释放资源
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