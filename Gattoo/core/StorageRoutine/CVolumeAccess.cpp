#include "StdAfx.h"

#include "CVolumeAccess.h"
#include <iostream>
#include <fstream>
#include <vector>

CVolumeAccess::CVolumeAccess(TCHAR aVolumeDriveLetter)

{
	TCHAR lVolume[9];
	_tcsncpy_s(lVolume, _T("\\\\.\\"), 7);
	_tcsncat_s(lVolume, &aVolumeDriveLetter, 1);
	_tcsncat_s(lVolume, _T(":"), 1);

	HANDLE hDevice = CreateFile(lVolume,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	// If we connected successfully to the drive
	if (FAILED(hDevice))
	{
		LogErr(_T("Error opening connection to drive [%s:]\n"), aVolumeDriveLetter);
	}
	else
	{
		m_hDevice = hDevice;
		
		if (!lockAndDismount())
		{
			clean();
		}
		
		// Initialize mandatory data needed for the communication with the volume
		initData();
	}
}

CVolumeAccess::~CVolumeAccess()
{
	clean();
}

void CVolumeAccess::clean()
{
	if (m_hDevice != NULL)
		CloseHandle(m_hDevice);
}

bool CVolumeAccess::lockAndDismount()
{
	LogDbg(_T("Dismounting and Locking the volume..."));

	DWORD dwReturned;
	BOOL bRes = DeviceIoControl(m_hDevice, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0, &dwReturned, 0);

	if(!bRes )
	{
		LogErr("Error dismounting the volume (Error=0x%X)\n", GetLastError());
		return false;
	}
	
	bRes = DeviceIoControl( m_hDevice, FSCTL_LOCK_VOLUME, 0, 0, 0, 0, &dwReturned, 0 );
	if (!bRes)
	{
		LogErr("Error locking the volume (Error=0x%X)\n",GetLastError());
		return false;
	}
	
	LogDbg("Done!\n");
	return true;
}

void CVolumeAccess::readBootSector()
{
	LogDbg(_T("Boot sector size = %d Bytes\n"), sizeof(FATBootSector));

	DWORD lRet = SetFilePointer(m_hDevice, 0, NULL, FILE_BEGIN);

	if (lRet == INVALID_SET_FILE_POINTER) 
	{
		LogErr("SetPointer FAILED! 0x%X\n", GetLastError());
	}
	else
	{
		DWORD lBytesRead;
		
		BYTE* lTemp = new BYTE[m_sectorSize];

		// only in sector multiplications!
		if (!ReadFile(m_hDevice, lTemp, m_sectorSize, &lBytesRead, NULL))
		{	
			LogErr("Error reading from the file 0x%X\n", GetLastError());
		}

		memcpy_s(&m_bootSector, sizeof(m_bootSector), lTemp, sizeof(m_bootSector));
	}	
}

void CVolumeAccess::initData()
{
	DISK_GEOMETRY_EX lDiskGeo;
	DWORD lBytes;
	
	if (DeviceIoControl(m_hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, &lDiskGeo, sizeof(DISK_GEOMETRY_EX), &lBytes, NULL))
	{
		m_sectorSize = lDiskGeo.Geometry.BytesPerSector;
		LogDbg(_T("Sector size is %d bytes\n"), m_sectorSize);
	}

	// Initializing the boot sector data member
	readBootSector();

	// Init the cluster size
	m_clusterSizeBytes = m_bootSector.PBP_SecPerClus * m_sectorSize;
}

bool CVolumeAccess::writeBytesToDeviceSector(BYTE* aBuffer, DWORD aSizeOfData, DWORD aStartSector)
{
	// First - go to the appropriate position in the device
	if (!goToSector(aStartSector)) return false;

	DWORD lBytesRead;

	// only in sector multiplications!
	if (!WriteFile(m_hDevice, aBuffer, aSizeOfData, &lBytesRead, NULL))
	{	
		LogErr("Error writing to the device, Code: 0x%X\n", GetLastError());
		return false;
	}
	
	return true;
}

// Sets the device's pointer to the argumented cluster number
bool CVolumeAccess::goToSector(DWORD aSectorNum)
{
	// Compute the absolute position in the Device. 
	// The argumented sector is relative to the first available sector, which is after the reserved sectors
	// (Actually, it's also after the hidded sectors, but when opening the partition as we did - 
	//  there's no need to compute the hidden sectors number)
	LARGE_INTEGER liPos;
	liPos.QuadPart = (static_cast<LONGLONG>(m_bootSector.BPB_RsvdSecCnt)+aSectorNum)*m_sectorSize;
	
	DWORD lRet = SetFilePointer(m_hDevice, liPos.LowPart, &liPos.HighPart, FILE_BEGIN);

	if (lRet == INVALID_SET_FILE_POINTER) 
	{
		LogErr("Failed to access sector number %d - SetPointer FAILED! 0x%X\n", aSectorNum, GetLastError());
		return false;
	}

	// All is swell!
	return true;
}

bool CVolumeAccess::checkVolumeParams()
{
	_ASSERTE(m_hDevice != INVALID_HANDLE_VALUE);
	if (m_hDevice == INVALID_HANDLE_VALUE) return false;

	const int iNeedSize = 512;
	const char szFATName[] = "FAT32";
	const char szVolLbl[] = "GRANIT";

	if (m_bootSector.BPB_BytsPerSec != iNeedSize)
		return false;

//  	if (memcmp(m_bootSector.BS_FilSysType, szFATName, sizeof(szFATName)-1))
//  		return false;
// 
//  	if (memcmp(m_bootSector.BS_VolLab, szVolLbl, sizeof(szVolLbl)-1))
//  		return false;

	return true;
}

DWORD CVolumeAccess::getSectorSize() const
{
	return m_sectorSize;
}

DWORD CVolumeAccess::getResrvdSctCount() const
{
	return m_bootSector.BPB_RsvdSecCnt;
}



