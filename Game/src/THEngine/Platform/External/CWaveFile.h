/*将directx中的sdkwavefile改了一下，能够不要dxut.h就能够编译的Cwavefile类，能够对wav文件进行操作。

下面是头文件和实现文件，其中stdafx.h中至少要包含windows.h。同时在项目设置中包含winmm.lib,dsound.lib,
DxErr.lib;在附加包含目录和附加库目录中加上directx sdk中的include和lib目录。*/

//-----------------------------------------------------------------------------
// File: WaveFile.h
//
// Copyright (c) Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef DXUTWAVEFILE_H
#define DXUTWAVEFILE_H

#include <Windows.h>
//#include <mmreg.h>
#include <mmsystem.h>
#include <DxErr.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif
//-----------------------------------------------------------------------------
// Typing macros 
//-----------------------------------------------------------------------------
#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2

//-----------------------------------------------------------------------------
// Name: class CWaveFile
// Desc: Encapsulates reading or writing sound data to or from a wave file
//-----------------------------------------------------------------------------
class CWaveFile
{
public:
	WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure
	HMMIO m_hmmio;       // MM I/O handle for the WAVE
	MMCKINFO m_ck;          // Multimedia RIFF chunk
	MMCKINFO m_ckRiff;      // Use in opening a WAVE file
	DWORD m_dwSize;      // The size of the wave file
	MMIOINFO m_mmioinfoOut;
	DWORD m_dwFlags;
	BOOL m_bIsReadingFromMemory;
	BYTE* m_pbData;
	BYTE* m_pbDataCur;
	ULONG m_ulDataSize;
	CHAR* m_pResourceBuffer;

protected:
	HRESULT ReadMMIO();
	HRESULT WriteMMIO(WAVEFORMATEX* pwfxDest);

public:
	CWaveFile();
	~CWaveFile();

	HRESULT Open(LPWSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags);
	HRESULT OpenFromMemory(BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags);
	HRESULT Close();

	HRESULT Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead);
	HRESULT Write(UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote);

	DWORD   GetSize();
	HRESULT ResetFile();
	WAVEFORMATEX* GetFormat()
	{
		return m_pwfx;
	};
};


#endif // DXUTWAVEFILE_H
