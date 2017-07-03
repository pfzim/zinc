//#include "stdafx.h"								//pf_ziminski  [2008]
#include "zwindows.h"
#include "zfiles.h"
#include "zalloc.h"
#include "utstrings.h"
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>

//#pragma comment (lib, "shell32.lib")

int installtosystem(char *key_name, char *file_name)
{
	//HKEY_LOCAL_MACHINE | HKEY_CURRENT_USER Software\\Microsoft\\Windows\\CurrentVersion\\Run
	//Software\\Microsoft\\Windows\\CurrentVersion\\RunServices

	HKEY hSubKey;
	char *this_file;
	char *target_file;

	this_file = (char *) zalloc(MAX_PATH+1);
	if(!this_file)
	{
		return -1;
	}

	if(RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hSubKey) != ERROR_SUCCESS)
	{
		zfree(this_file);
		return -1;
	}

	GetModuleFileName(NULL, this_file, MAX_PATH);

	if(file_name)
	{
		target_file = (char *) zalloc(MAX_PATH+1);
		if(	!target_file
			|| (SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_CURRENT, target_file) != S_OK)
		)
		{
			RegCloseKey(hSubKey);
			zfree(this_file);
			return -1;
		}

		// как-то надо сделать проверку (strcmp(this_file, target_file) == 0)
		// с учётом лишних и обратных слешей
		alloc_strcat(&target_file, "\\prd\\");
		if(!mkrdir(target_file))
		{
			alloc_strcat(&target_file, file_name);
			//CopyFile(this_file, target_file, TRUE);
			fcopy(this_file, target_file, 0);
		}
		else
		{
			RegCloseKey(hSubKey);
			zfree(this_file);
			return -1;
		}

		zfree(this_file);
	}
	else
	{
		target_file = this_file;
	}

	RegSetValueEx(hSubKey, key_name, NULL, REG_SZ, (BYTE *) target_file, strlen(target_file)+1);

	RegCloseKey(hSubKey);
	zfree(target_file);

	return 0;
	
	
	/*
	DWORD tmp = GetSystemDirectory(NULL, 0) + strlen(filename);
	if(tmp > MAX_PATH)
	{
		return 1L;
	}

	LPSTR sys_path = (LPSTR) zalloc(tmp+1);
	if(sys_path)
	{
		GetSystemDirectory(sys_path, tmp);
		strcat(sys_path, filename);

		HKEY hSubKey;
		//Software\\Microsoft\\Windows\\CurrentVersion\\Run
		//Software\\Microsoft\\Windows\\CurrentVersion\\RunServices
		if(RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hSubKey) == ERROR_SUCCESS)
		{
			RegSetValueEx(hSubKey, keyname, NULL, REG_SZ, (BYTE*)sys_path, tmp);
			RegCloseKey(hSubKey);
		}
			//CopyFile(thisfile, sys_path, TRUE);
			zfree(thisfile);
		}

		zfree(sys_path);

		return 0L;
	}
	*/
}

int deinstallfromsystem(char *key_name, char *file_name)
{
	HKEY hSubKey;

	if(RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hSubKey) != ERROR_SUCCESS)
	{
		return -1;
	}

	RegDeleteValue(hSubKey, key_name);
	RegCloseKey(hSubKey);

	return 0;
}

unsigned long exec(char *file, char *params)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	char *cmd_line;
	unsigned long exit_code;

	exit_code = 0;

	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if(strchr(file, ' '))
	{
		cmd_line = alloc_string("\"");
		alloc_strcat(&cmd_line, file);
		alloc_strcat(&cmd_line, "\"");
	}
	else
	{
		cmd_line = alloc_string(file);
	}

	if(!isempty(params))
	{
		alloc_strcat(&cmd_line, " ");
		alloc_strcat(&cmd_line, params);
	}

	if(CreateProcess(file, cmd_line, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		exit_code = pi.dwProcessId;
	}

	free_str(cmd_line);

	return exit_code;
}