//#include "stdafx.h"									//dEmon (c) 2007
#include "zalloc.h"
#include "zdbg.h"

// this part moved from header.h only for libjpeg compile
#include <windows.h>

#define ZA_INIT_SIZE			1048576		// 1MB
#define ZA_MAX_SIZE				0			// unlimited

#define ZA_HEAP_OPTIONS			HEAP_GENERATE_EXCEPTIONS

//#define zfree(ptr)		abs_zfree(ptr); (ptr) = 0;

HANDLE zcreate();
void zdestroy();
// end of moved part

//* class for auto create process heap and destroy on exit
class CHeap
{
	protected:
		HANDLE heap;

	public:
		CHeap()
		{
			/*
			SYSTEM_INFO si;
			GetSystemInfo(&si);
			ZTRACE("DBG: CHeap(): System Info:\n  Oem Id: %d\n  Page Size: %d\n  Minimum Application Address: %d\n  Maximum Application Address: %d\n  Active Processor Mask: %d\n  Number Of Processors: %d\n  Processor Type: %d\n  Allocation Granularity: %d\n  Processor Level: %d\n  Processor Revision: %d\n", si.dwOemId, si.dwPageSize, si.lpMinimumApplicationAddress, si.lpMaximumApplicationAddress, si.dwActiveProcessorMask, si.dwNumberOfProcessors, si.dwProcessorType, si.dwAllocationGranularity, si.wProcessorLevel, si.wProcessorRevision);
			heap = HeapCreate(ZA_HEAP_OPTIONS, ZA_INIT_SIZE + (ZA_INIT_SIZE%si.dwPageSize), ZA_MAX_SIZE);
			*/
			heap = GetProcessHeap();
		}

		~CHeap()
		{
			/*
			if(heap)
			{
				HeapDestroy(heap);
				heap = NULL;
			}
			*/
		}

		HANDLE GetHandle() const
		{
			return heap;
		}
};

CHeap cHeapProcess;

void *zalloc(unsigned long size)
{
	if(cHeapProcess.GetHandle())
	{
		/*
		if(size == 255)
		{
			_asm int 3;
		}
		*/
		return HeapAlloc(cHeapProcess.GetHandle(), ZA_HEAP_OPTIONS, size);
		/*
		void *j;
		j = HeapAlloc(cHeapProcess.GetHandle(), ZA_HEAP_OPTIONS, size);

		ZTRACE("HA: 0x%.8X (%d)\n", j, size);
		return j;
		*/
	}
	else
	{
		ZTRACE("DBG: zalloc(): process heap not created\n");
		return NULL;
	}
}

// Идея: Контроля размера выделенного блока.
// Выделять память размером size + 4 байта
// В первых 4 байтах выделенного блока хранить размер
// этого самого блока (примерно как сделано в Паскале со строками)
// Функция будет возвращать адрес buffer + 4
// При освобождении памяти будет использоваться address - 4
// Таким образом на существующие программы это никак не повлияет,
// зато можно более гибко реализовать функции со сторками (alloc_str...).
void *zrealloc(void *lpbuf, unsigned long size)
{
	if(cHeapProcess.GetHandle())
	{
		if(!size)
		{
			HeapFree(cHeapProcess.GetHandle(), 0, lpbuf);
		}
		else
		{
			if(lpbuf)
			{
				void *lpdstbuf;
				lpdstbuf = HeapReAlloc(cHeapProcess.GetHandle(), ZA_HEAP_OPTIONS, lpbuf, size);
				if(!lpdstbuf)
				{
					unsigned long src_size;
					lpdstbuf = HeapAlloc(cHeapProcess.GetHandle(), ZA_HEAP_OPTIONS, size);
					src_size = HeapSize(cHeapProcess.GetHandle(), 0, lpbuf);
					if(src_size != 0xFFFFFFFF)
					{
						if(src_size > size)
						{
							src_size = size;
						}

						memcpy(lpbuf, lpdstbuf, src_size);
					}
					else
					{
						ZTRACE("DBG: zrealloc(): can not determine source size to copy it\n");
					}

					HeapFree(cHeapProcess.GetHandle(), 0, lpbuf);
				}
				
				lpbuf = lpdstbuf;
			}
			else
			{
				lpbuf = HeapAlloc(cHeapProcess.GetHandle(), ZA_HEAP_OPTIONS, size);
			}
			//ZTRACE("HA: 0x%.8X (%d)\n", lpbuf, size);
			return lpbuf;
		}
	}
	return NULL;
}

void zfree(void *lpbuf)
{
	if(lpbuf && cHeapProcess.GetHandle())
	{
		HeapFree(cHeapProcess.GetHandle(), 0, lpbuf);
		//lpbuf = NULL;
	}
}

/*
void zfree(void *lpbuf)
{
	if(lpbuf && cHeapProcess.GetHandle())
	{
		HeapFree(cHeapProcess.GetHandle(), 0, lpbuf);
		//lpbuf = NULL;
	}
}
//*/
/* without auto destroy on exit
HANDLE heapProcess = zcreate();

HANDLE zcreate()
{
	if(!heapProcess)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		return HeapCreate(ZA_HEAP_OPTIONS, ZA_INIT_SIZE + (ZA_INIT_SIZE%si.dwPageSize), ZA_MAX_SIZE);
	}
	else
	{
		ZTRACE("DBG: zcreate(): error. process heap already created\n");
		return heapProcess;
	}
}

void zdestroy()
{
	if(heapProcess)
	{
		HeapDestroy(heapProcess);
	}
}

void *zalloc(unsigned long size)
{
	if(!heapProcess)
	{
		ZTRACE("DBG: zalloc(): error. process heap not created. trying create...\n");
		heapProcess = zcreate();
	}

	if(heapProcess)
	{
		return HeapAlloc(heapProcess, ZA_HEAP_OPTIONS, size);
	}
	else
	{
		ZTRACE("DBG: zalloc(): process heap not created\n");
		return NULL;
	}
}

void *zrealloc(void *lpbuf, unsigned long size)
{
	if(heapProcess)
	{
		if(!size)
		{
			HeapFree(heapProcess, 0, lpbuf);
		}
		else
		{
			if(lpbuf)
			{
				lpbuf = HeapAlloc(heapProcess, ZA_HEAP_OPTIONS, size);
			}
			else
			{
				lpbuf = HeapReAlloc(heapProcess, ZA_HEAP_OPTIONS, lpbuf, size);
			}
			return lpbuf;
		}
	}
	return NULL;
}

void zfree(void *lpbuf)
{
	if(lpbuf && heapProcess)
	{
		HeapFree(heapProcess, 0, lpbuf);
		//lpbuf = NULL;
	}
}
//*/