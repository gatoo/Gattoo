#pragma once

#include <winioctl.h>

#pragma pack(push,1)
struct FATBootSector
{
	char BS_jmpBoot[3];
	char BS_OEMName[8];
	WORD BPB_BytsPerSec;
	BYTE PBP_SecPerClus;
	WORD BPB_RsvdSecCnt;
	BYTE BPB_NumFATs;
	WORD BPB_RootEntCnt;
	WORD BPB_TotSec16;
	BYTE BPB_Media;
	WORD BPB_FATsz16;
	WORD BPB_SecPerTrk;
	WORD BPB_NumHeads;
	DWORD BPB_HiddSec;
	DWORD BPB_TotSec32;

	DWORD BPB_FATsz32;
	WORD BPB_ExtFlags;
	WORD BPB_FSVer;
	DWORD BPB_RootClus;
	WORD BPB_FSInfo;
	WORD BPB_BkBootSec;
	BYTE BPB_Reserved[12];
	BYTE BS_DrvNum;
	BYTE BS_Reserverd1;
	BYTE BS_BootSig;
	DWORD BS_VolID;
	BYTE BS_VolLab[11];
	char BS_FilSysType[8];
};
#pragma pack(pop)

class CVolumeAccess
{
private:

	HANDLE			m_hDevice;
	DWORD			m_sectorSize;
	FATBootSector	m_bootSector;
	DWORD			m_clusterSizeBytes;
	DWORD*			m_FAT1Data;
	DWORD*			m_FAT2Data;

	// Static members
	static const DWORD FAT_END_CHAIN = 0x0FFFFFFF;

	static TCHAR			s_driveLetter;
	static CVolumeAccess*	s_instance;

	// Member functions
	bool lockAndDismount();
	void readBootSector();
	void initData();

	bool goToSector(DWORD aSectorNum);
	bool readBytesFromDeviceCluster(BYTE* aBuffer, DWORD aSizeOfData, DWORD aStartCluster);
	bool readBytesFromDeviceSector(BYTE* aBuffer, DWORD aSizeOfData, DWORD aStartSector);
	bool writeBytesToDeviceCluster(BYTE* aBuffer, DWORD aSizeOfData, DWORD aStartCluster);

	// Clean resources
	void clean();

public:

	CVolumeAccess(TCHAR aVolumeDriveLetter);
	~CVolumeAccess();

	bool IsDeviceReady();

	bool writeBytesToDeviceSector(BYTE* aBuffer, DWORD aSizeOfData, DWORD aStartSector);

	DWORD getSectorSize() const;
	DWORD getResrvdSctCount() const;

	bool checkVolumeParams();
};