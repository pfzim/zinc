//#include "stdafx.h"								//pf_ziminski  [2008]
#include "zshell.h"
#include <windows.h>
#include <shlobj.h>

//#pragma comment (lib, "shell32.lib")

void cs(void)
{
	HRESULT hres;
	IShellLink* psl;

	//if(!validpathspec(buf2)) GetStringFromParm(0x21);

	//log_printf8("CreateShortCut: out: \"%s\", in: \"%s %s\", icon: %s,%d, sw=%d, hk=%d", buf1,buf2,buf0,buf3,parm4&0xff,(parm4&0xff00)>>8,parm4>>16);

	hres = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (void **) &psl);
	if(SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		hres = psl->lpVtbl->QueryInterface(psl,&IID_IPersistFile, (void **) &ppf);
		if(SUCCEEDED(hres))
		{
			hres = psl->lpVtbl->SetPath(psl,buf2);
			psl->lpVtbl->SetWorkingDirectory(psl, state_output_directory);
			if((parm4&0xff00)>>8)
			{
				psl->lpVtbl->SetShowCmd(psl, (parm4&0xff00)>>8);
			}
			psl->lpVtbl->SetHotkey(psl, (unsigned short) (parm4>>16));
			if(buf3[0])
			{
				psl->lpVtbl->SetIconLocation(psl, buf3, parm4&0xff);
			}
			psl->lpVtbl->SetArguments(psl, buf0);
			psl->lpVtbl->SetDescription(psl, buf4);

			if(SUCCEEDED(hres))
			{
				static WCHAR wsz[1024];
				hres=E_FAIL;
				if(MultiByteToWideChar(CP_ACP, 0, buf1, -1, wsz, 1024))
				{
					hres=ppf->lpVtbl->Save(ppf, (const WCHAR*) wsz, TRUE);
				}
			}
			ppf->lpVtbl->Release(ppf);
		}
		psl->lpVtbl->Release(psl);
	}
}
	  
