#include "../../ThirdParty/Rxx/include/Hook.h"
#include "../../ThirdParty/Rxx/include/Hook_API_DEF.h"
#include "../../ThirdParty/Rxx/include/Mem.h"

using namespace Rut::MemX;
using namespace Rut::HookX;


static DWORD g_dwExeBase = (DWORD)GetModuleHandleW(NULL);

static pGetGlyphOutlineA RawGetGlyphOutlineA = (pGetGlyphOutlineA)GetGlyphOutlineA;
DWORD WINAPI NewGetGlyphOutlineA(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2)
{
	if (uChar > 0x7F) { uChar = ((uChar << 8) | (uChar >> 8)) & 0x0000FFFF; }

	return RawGetGlyphOutlineA(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}

static DWORD g_dwReadCharRaw = NULL, g_dwReadCharRet = NULL;
VOID __declspec(naked) ProcDBCS_0()
{
	_asm
	{
		movzx ecx, byte ptr[eax];
		cmp ecx, 0x81;
		jb not_DBCS;

	//is_DBCS:
		movzx ecx, word ptr[eax];
		inc eax;

	not_DBCS:
		mov dword ptr[ebp - 0x10], ecx;
		jmp g_dwReadCharRet;
	}
}

VOID StartHook()
{

	HookCreateFontA(0x86, "黑体");
	DetourAttachFunc(&RawGetGlyphOutlineA, NewGetGlyphOutlineA);

	g_dwReadCharRaw = g_dwExeBase + 0x10E363;
	g_dwReadCharRet = g_dwReadCharRaw + 6;
	WriteHookCode(g_dwReadCharRaw, (DWORD)ProcDBCS_0, 0x6);

	static BYTE aCharacterName0[] = { 0x20,0xA1,0xBF };
	static BYTE aCharacterName1[] = { 0xA1,0xBE,0x20 };
	WriteMemory((LPVOID)(g_dwExeBase + 0x167DBC), aCharacterName0, sizeof(aCharacterName0));
	WriteMemory((LPVOID)(g_dwExeBase + 0x167DC0), aCharacterName1, sizeof(aCharacterName1));

	//00  "Elica"	01  "エリカ"	 -> 艾丽卡	//02  "General" 03  "本部長"		-> 总部部长
	//04  "Inaba"	05  "稲葉"	 -> 稻叶		//06  "Lavie"   07  "ラヴィ"		-> 拉维
	//08  "Maya"	09  "真夜"	 -> 真夜		//10  "Natsume" 11  "ナツメ"		-> 夏目
	//12  "Ryou"	13  "亮"		 -> 亮		//14  "Sion"    15  "シオン"		-> 诗音
	//16  "Soldier"	17  "兵士" 	 -> 士兵	
	static UCHAR** pTable = (UCHAR**)(g_dwExeBase + 0x197720);

	static UCHAR aElica[]   = { 0xB0, 0xAC, 0xC0, 0xF6, 0xBF, 0xA8, 0x00 };
	static UCHAR aGeneral[] = { 0xD7, 0xDC, 0xB2, 0xBF, 0xB2, 0xBF, 0xB3, 0xA4, 0x00 };
	static UCHAR aInaba[]   = { 0xB5, 0xBE, 0xD2, 0xB6, 0x0 };
	static UCHAR aLavie[]   = { 0xC0, 0xAD, 0xCE, 0xAC,0x00 };
	static UCHAR aMaya[]    = { 0xD5, 0xE6, 0xD2, 0xB9, 0x00 };
	static UCHAR aNatsume[] = { 0xCF, 0xC4, 0xC4, 0xBF, 0x00 };
	static UCHAR aRyou[]    = { 0xC1, 0xC1, 0x00 };
	static UCHAR aSion[]    = { 0xCA, 0xAB, 0xD2, 0xF4, 0x00 };
	static UCHAR aSoldier[] = { 0xCA, 0xBF, 0xB1, 0xF8, 0x00 };

	pTable[0] = aElica;
	pTable[2] = aGeneral;
	pTable[4]  = aInaba;
	pTable[6] = aLavie;
	pTable[8] = aMaya;
	pTable[10] = aNatsume;
	pTable[12] = aRyou;
	pTable[14] = aSion;
	pTable[16] = aSoldier;

	static BYTE aSJISRange[] = { 0xFE };
	WriteMemory((LPVOID)(g_dwExeBase + 0xC25BF), aSJISRange, sizeof(aSJISRange));
	WriteMemory((LPVOID)(g_dwExeBase + 0xD20DE), aSJISRange, sizeof(aSJISRange));
	WriteMemory((LPVOID)(g_dwExeBase + 0x122395), aSJISRange, sizeof(aSJISRange));
	WriteMemory((LPVOID)(g_dwExeBase + 0x10E18D), aSJISRange, sizeof(aSJISRange));

	static BYTE aProcDBCS_1_ASM[] = { 0x66, 0x89, 0x5D, 0x0C, 0x90, 0x90, 0x90 };
	WriteMemory((LPVOID)(g_dwExeBase + 0xC25F1), aProcDBCS_1_ASM, sizeof(aProcDBCS_1_ASM));

	static BYTE aMapCharASM[] =
	{
		0x3D, 0x65, 0x81, 0x00, 0x00, 
		0x75, 0x07,
		0xBB, 0x65, 0x81, 0x00, 0x00,
		0xEB, 0xC8, 
		0x3D, 0x66, 0x81, 0x00, 0x00, 
		0x75, 0x07, 
		0xBB, 0x66, 0x81, 0x00, 0x00,
		0xEB, 0xBA, 
		0x3D, 0x67, 0x81, 0x00, 0x00, 
		0x75, 0x07, 
		0xBB, 0x67, 0x81, 0x00, 0x00,
		0xEB, 0xAC, 
		0x3D, 0x68, 0x81, 0x00, 0x00, 
		0x75, 0x07, 
		0xBB, 0x68, 0x81, 0x00, 0x00,
		0xEB, 0x9E,
		0x3D, 0x5C, 0x81, 0x00, 0x00,
		0x75, 0x07,
		0xBB, 0x5C, 0x81, 0x00, 0x00,
		0xEB, 0x90, 
		0x3D, 0x96, 0x81, 0x00, 0x00, 
		0x75, 0x07, 
		0xBB, 0x96, 0x81, 0x00, 0x00,
		0xEB, 0x82, 
		0x3D, 0x60, 0x81, 0x00, 0x00, 
		0x75, 0x0A, 
		0xBB, 0x60, 0x81, 0x00, 0x00,
		0xE9, 0x71, 0xFF, 0xFF, 0xFF, 
		0x3D, 0xE1, 0x81, 0x00, 0x00, 
		0x75, 0x0A,
		0xBB, 0xE1, 0x81, 0x00, 0x00,
		0xE9, 0x60, 0xFF, 0xFF, 0xFF, 

		0xC1, 0xE0, 0x08, 
		0xC1, 0xEB, 0x08, 
		0x09, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
	};

	WriteMemory((LPVOID)(g_dwExeBase + 0xC261B), aMapCharASM, sizeof(aMapCharASM));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		StartHook();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

VOID __declspec(dllexport) DirA() {}
