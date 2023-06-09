#include "../../ThirdParty/Rxx/include/Hook.h"
#include "../../ThirdParty/Rxx/include/Mem.h"

using namespace Rut::MemX;
using namespace Rut::HookX;


static DWORD g_dwExeBase = (DWORD)GetModuleHandleW(NULL);


static DWORD g_dwReadCharRaw = NULL;
static DWORD g_dwReadCharRet = NULL;
VOID __declspec(naked) ProcDBCS_0()
{
	_asm
	{
		movzx esi, byte ptr[eax];
		cmp esi, 0x81;
		jb not_DBCS;

	//is_DBCS
		movzx esi, word ptr[eax];
		inc eax;

	not_DBCS:
		mov dword ptr[ebp - 0x10], esi;
		jmp g_dwReadCharRet;
	}
}

VOID StartHook()
{
	HookCreateFontA(0x80, "MS Mincho");

	g_dwReadCharRaw = g_dwExeBase + 0x10F227;
	g_dwReadCharRet = g_dwReadCharRaw + 6;
	WriteHookCode(g_dwReadCharRaw, (DWORD)ProcDBCS_0, 0x6);

	static BYTE aCharacterName0[] = { 0x20,0x81,0x7A };
	static BYTE aCharacterName1[] = { 0x81,0x79,0x20 };
	WriteMemory((LPVOID)(g_dwExeBase + 0x168DF4), aCharacterName0, sizeof(aCharacterName0));
	WriteMemory((LPVOID)(g_dwExeBase + 0x168DF8), aCharacterName1, sizeof(aCharacterName1));

	//UCHAR** pTable = (UCHAR**)(g_dwExeBase + 0x198720);
	//pTable[0] = pTable[1];
	//pTable[2] = pTable[3];
	//pTable[4] = pTable[5];
	//pTable[6] = pTable[7];
	//pTable[8] = pTable[9];
	//pTable[10] = pTable[11];
	//pTable[12] = pTable[13];
	//pTable[14] = pTable[15];
	//pTable[16] = pTable[17];

	static BYTE aProcDBCS_1_ASM[] = { 0x66, 0x89, 0x5D, 0x0C, 0x90, 0x90, 0x90 };
	WriteMemory((LPVOID)(g_dwExeBase + 0xC2971), aProcDBCS_1_ASM, sizeof(aProcDBCS_1_ASM));

	static BYTE aMapCharASM[] =
	{
		0x3D, 0x65, 0x81, 0x00, 0x00,            // cmp eax,8165          -->   ‘   SJIS
		0x75, 0x07,					             // jne eden_pm_en.4C29A9 	  	    
		0xBB, 0x81, 0x65, 0x00, 0x00,            // mov ebx,AEA1          -->   ‘   GBK
		0xEB, 0xC8,					             // jmp eden_pm_en.4C2971 	  	    
		0x3D, 0x66, 0x81, 0x00, 0x00,            // cmp eax,8166          -->   ’   SJIS
		0x75, 0x07,					             // jne eden_pm_en.4C29B7 	  	    
		0xBB, 0x81, 0x66, 0x00, 0x00,            // mov ebx,AFA1          -->   ’   GBK
		0xEB, 0xBA,					             // jmp eden_pm_en.4C2971 	  	 
																			 	 
		0x3D, 0x67, 0x81, 0x00, 0x00,            // cmp eax,8167           -->   “   SJIS
		0x75, 0x07,					             // jne eden_pm_en.4C29C5  	  	 
		0xBB, 0x81, 0x67, 0x00, 0x00,            // mov ebx,BAA1           -->   『   GBK
		0xEB, 0xAC,					             // jmp eden_pm_en.4C2971  	  	 
		0x3D, 0x68, 0x81, 0x00, 0x00,            // cmp eax,8168           -->   ”   SJIS
		0x75, 0x07,					             // jne eden_pm_en.4C29D3  	  	 
		0xBB, 0x81, 0x68, 0x00, 0x00,            // mov ebx,BBA1           -->   』   GBK
		0xEB, 0x9E,					             // jmp eden_pm_en.4C2971  	 	 
													 						 	 
		0x3D, 0x5C, 0x81, 0x00, 0x00,            // cmp eax,815C           -->   ―   SJIS
		0x75, 0x07,					             // jne eden_pm_en.4C29E1 	 	 
		0xBB, 0x81, 0x5C, 0x00, 0x00,            // mov ebx,44A8           -->   ―   GBK
		0xEB, 0x90,					             // jmp eden_pm_en.4C2971 
												    
		0x3D, 0x96, 0x81, 0x00, 0x00,            // cmp eax,8196           -->   ＊   SJIS
		0x75, 0x07,					             // jne eden_pm_en.4C29EF 
		0xBB, 0x81, 0x96, 0x00, 0x00,            // mov ebx,AAA3           -->   ＊   GBK
		0xEB, 0x82,					             // jmp eden_pm_en.4C2971
												    
		0x3D, 0x60, 0x81, 0x00, 0x00,            // cmp eax,8160           -->   〜   SJIS
		0x75, 0x0A,					             // jne eden_pm_en.4C2A00  
		0xBB, 0x81, 0x60, 0x00, 0x00,            // mov ebx,7E             -->   ~   ASCII
		0xE9, 0x71, 0xFF, 0xFF, 0xFF,            // jmp eden_pm_en.4C2971  
												    
		0x3D, 0xE1, 0x81, 0x00, 0x00,            // cmp eax,81E1          -->   ≪   SJIS
		0x75, 0x0A,					             // jne eden_pm_en.4C2A11
		0xBB, 0x81, 0xE1, 0x00, 0x00,            // mov ebx,3F            -->   <   ASCII
		0xE9, 0x60, 0xFF, 0xFF, 0xFF,            // jmp eden_pm_en.4C2971 
												    
		0x3D, 0xE2, 0x81, 0x00, 0x00,            // cmp eax,81E2           -->   ≫   SJIS
		0x75, 0x0A,					             // jne eden_pm_en.4C2A22  
		0xBB, 0x81, 0xE2, 0x00, 0x00,            // mov ebx,3F             -->   >   ASCII
		0xE9, 0x4F, 0xFF, 0xFF, 0xFF,            // jmp eden_pm_en.4C2971  
												    
		//处理不在预定义替换字符内的双字节字符		   
		0x89, 0xC3,                              // mov ebx,eax          
		0xC1, 0xEB, 0x08,                        // shr ebx,8            
		0xC1, 0xE0, 0x08,                        // shl eax,8            
		0x09, 0xC3,                              // or ebx,eax    
		0x90, 0x90, 0x90, 0x90,                  // NOP
		0x90, 0x90, 0x90, 0x90
	};
	WriteMemory((LPVOID)(g_dwExeBase + 0xC299B), aMapCharASM, sizeof(aMapCharASM));


	static BYTE aProcDBCS_2_ASM[] =
	{
		0x6A, 0x02,                               // push 0x2
		0x66, 0x89, 0x45, 0xF8,                   // mov word ptr ss:[ebp-0x8], ax
		0x8D, 0x45, 0xF4,                         // lea eax, ss:[ebp-0xC]
		0x50,                                     // push eax
		0x6A, 0x02,                               // push 0x2
		0x8D, 0x4D, 0xF8,                         // lea ecx, ss:[ebp-0x8]
		0x51,                                     // push ecx
		0x6A, 0x00,                               // push 0x0
		0x68, 0xA4, 0x03, 0x00, 0x00,             // push 0x3A4
		0x89, 0x55, 0xF4,                         // mov dword ptr ss:[ebp-0xC], edx
		0xFF, 0x15, 0x10, 0x92, 0x54, 0x00,       // call dword ptr ds:[%MulitiByteToWideChar]
		0x8B, 0x55, 0xF4,                         // mov edx, dword ptr ss:[ebp-0xC]
		0x89, 0x55, 0xFC,                         // mov dword ptr ss:[ebp-0x4], edx
		0x90, 0x90, 0x90, 0x90, 0x90,0x90,        // NOP
		0x90, 0x90, 0x90, 0x90, 0x90
	};
	WriteMemory((LPVOID)(g_dwExeBase + 0x11A22F), aProcDBCS_2_ASM, sizeof(aProcDBCS_2_ASM));
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