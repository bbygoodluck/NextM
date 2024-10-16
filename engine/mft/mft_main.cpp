#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <windows.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#pragma pack(push,1)

struct BootSector {
    uint8_t     jump[3];
    char        name[8];
    uint16_t    bytesPerSector;         // The number of bytes in a sector. This should be 512.
    uint8_t     sectorsPerCluster;      // The number of sectors in a cluster. Clusters are used for less-granular access to the partition. They're usually 4KB.
    uint16_t    reservedSectors;
    uint8_t     unused0[3];
    uint16_t    unused1;
    uint8_t     media;
    uint16_t    unused2;
    uint16_t    sectorsPerTrack;
    uint16_t    headsPerCylinder;
    uint32_t    hiddenSectors;
    uint32_t    unused3;
    uint32_t    unused4;
    uint64_t    totalSectors;
    uint64_t    mftStart;           // The start of the MFT, given as a cluster index.
    uint64_t    mftMirrorStart;
    uint32_t    clustersPerFileRecord;
    uint32_t    clustersPerIndexBlock;
    uint64_t    serialNumber;
    uint32_t    checksum;
    uint8_t     bootloader[426];
    uint16_t    bootSignature;
};

struct FileRecordHeader {
    uint32_t    magic;
    uint16_t    updateSequenceOffset;
    uint16_t    updateSequenceSize;
    uint64_t    logSequence;
    uint16_t    sequenceNumber;
    uint16_t    hardLinkCount;
    uint16_t    firstAttributeOffset;
    uint16_t    inUse : 1;
    uint16_t    isDirectory : 1;
    uint32_t    usedSize;
    uint32_t    allocatedSize;
    uint64_t    fileReference;
    uint16_t    nextAttributeID;
    uint16_t    unused;
    uint32_t    recordNumber;
};

struct AttributeHeader {
    uint32_t    attributeType;
    uint32_t    length;
    uint8_t     nonResident;
    uint8_t     nameLength;
    uint16_t    nameOffset;
    uint16_t    flags;
    uint16_t    attributeID;
};

struct ResidentAttributeHeader : AttributeHeader {
    uint32_t    attributeLength;
    uint16_t    attributeOffset;
    uint8_t     indexed;
    uint8_t     unused;
};

struct FileNameAttributeHeader : ResidentAttributeHeader {
    uint64_t    parentRecordNumber : 48;
    uint64_t    sequenceNumber : 16;
    uint64_t    creationTime;
    uint64_t    modificationTime;
    uint64_t    metadataModificationTime;
    uint64_t    readTime;
    uint64_t    allocatedSize;
    uint64_t    realSize;
    uint32_t    flags;
    uint32_t    repase;
    uint8_t     fileNameLength;
    uint8_t     namespaceType;
    wchar_t     fileName[1];
};

struct NonResidentAttributeHeader : AttributeHeader {
    uint64_t    firstCluster;
    uint64_t    lastCluster;
    uint16_t    dataRunsOffset;
    uint16_t    compressionUnit;
    uint32_t    unused;
    uint64_t    attributeAllocated;
    uint64_t    attributeSize;
    uint64_t    streamDataSize;
};

struct RunHeader {
    uint8_t     lengthFieldBytes : 4;
    uint8_t     offsetFieldBytes : 4;
};

#pragma pack(pop)

struct File {
    uint64_t parent;
    char     *name;
};

File *files;

DWORD bytesAccessed;

BootSector bootSector;

#define MFT_FILE_SIZE (1024)
uint8_t mftFile[MFT_FILE_SIZE];

#define MFT_FILES_PER_BUFFER (65536)
uint8_t mftBuffer[MFT_FILES_PER_BUFFER * MFT_FILE_SIZE];

HANDLE drive = NULL;

char *DuplicateName(wchar_t *name, size_t nameLength)
{
    static char *allocationBlock = nullptr;
    static size_t bytesRemaining = 0;

    size_t bytesNeeded = WideCharToMultiByte(CP_UTF8, 0, name, nameLength, NULL, 0, NULL, NULL) + 1;

    if (bytesRemaining < bytesNeeded) {
        allocationBlock = (char *) malloc((bytesRemaining = 16 * 1024 * 1024));
    }

    char *buffer = allocationBlock;
    buffer[bytesNeeded - 1] = 0;
    WideCharToMultiByte(CP_UTF8, 0, name, nameLength, allocationBlock, bytesNeeded, NULL, NULL);

    bytesRemaining -= bytesNeeded;
    allocationBlock += bytesNeeded;

    return buffer;
}

void Read(void *buffer, uint64_t from, uint64_t count)
{
	DWORD bytesAccessed;
	LONG high = from >> 32;
	SetFilePointer(drive, from & 0xFFFFFFFF, &high, FILE_BEGIN);
	ReadFile(drive, buffer, count, &bytesAccessed, NULL);
	assert(bytesAccessed == count);
}

int main(int argc, char **argv)
{
	drive = CreateFile("\\\\.\\C:", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(drive == NULL)
	{
		printf("drive handle is null");
		return 0;
	}

	Read(&bootSector, 0, 512);

	uint64_t bytesPerCluster = bootSector.bytesPerSector * bootSector.sectorsPerCluster;
	Read(&mftFile, bootSector.mftStart * bytesPerCluster, MFT_FILE_SIZE);

	FileRecordHeader *fileRecord = (FileRecordHeader *) mftFile;
	AttributeHeader *attribute = (AttributeHeader *) (mftFile + fileRecord->firstAttributeOffset);

	NonResidentAttributeHeader *dataAttribute = nullptr;
	uint64_t approximateRecordCount = 0;
	assert(fileRecord->magic == 0x454C4946);

	while (true)
	{
        if (attribute->attributeType == 0x80)
        {
            dataAttribute = (NonResidentAttributeHeader *) attribute;
        }
		else if (attribute->attributeType == 0xB0)
        {
            approximateRecordCount = ((NonResidentAttributeHeader *) attribute)->attributeSize * 8;
        }
        else if (attribute->attributeType == 0xFFFFFFFF)
        {
            break;
        }

        attribute = (AttributeHeader *) ((uint8_t *) attribute + attribute->length);
    }

    assert(dataAttribute);
    RunHeader *dataRun = (RunHeader *) ((uint8_t *) dataAttribute + dataAttribute->dataRunsOffset);
    uint64_t clusterNumber = 0, recordsProcessed = 0;

    while (((uint8_t *) dataRun - (uint8_t *) dataAttribute) < dataAttribute->length && dataRun->lengthFieldBytes)
	{
        uint64_t length = 0, offset = 0;

        for (int i = 0; i < dataRun->lengthFieldBytes; i++)
		{
            length |= (uint64_t) (((uint8_t *) dataRun)[1 + i]) << (i * 8);
        }

        for (int i = 0; i < dataRun->offsetFieldBytes; i++)
		{
            offset |= (uint64_t) (((uint8_t *) dataRun)[1 + dataRun->lengthFieldBytes + i]) << (i * 8);
        }

        if (offset & ((uint64_t) 1 << (dataRun->offsetFieldBytes * 8 - 1)))
		{
            for (int i = dataRun->offsetFieldBytes; i < 8; i++)
            {
                offset |= (uint64_t) 0xFF << (i * 8);
            }
        }

        clusterNumber += offset;
        dataRun = (RunHeader *) ((uint8_t *) dataRun + 1 + dataRun->lengthFieldBytes + dataRun->offsetFieldBytes);

        uint64_t filesRemaining = length * bytesPerCluster / MFT_FILE_SIZE;
        uint64_t positionInBlock = 0;

        while (filesRemaining)
		{
            fprintf(stderr, "%d%% ", (int) (recordsProcessed * 100 / approximateRecordCount));

            uint64_t filesToLoad = MFT_FILES_PER_BUFFER;
            if (filesRemaining < MFT_FILES_PER_BUFFER) filesToLoad = filesRemaining;
            Read(&mftBuffer, clusterNumber * bytesPerCluster + positionInBlock, filesToLoad * MFT_FILE_SIZE);
            positionInBlock += filesToLoad * MFT_FILE_SIZE;
            filesRemaining -= filesToLoad;

            for (int i = 0; i < filesToLoad; i++)
			{
                // Even on an SSD, processing the file records takes only a fraction of the time to read the data,
                // so there's not much point in multithreading this.
                FileRecordHeader *fileRecord = (FileRecordHeader *) (mftBuffer + MFT_FILE_SIZE * i);
                recordsProcessed++;

                if (!fileRecord->inUse) continue;

                AttributeHeader *attribute = (AttributeHeader *) ((uint8_t *) fileRecord + fileRecord->firstAttributeOffset);
                assert(fileRecord->magic == 0x454C4946);

                while ((uint8_t *) attribute - (uint8_t *) fileRecord < MFT_FILE_SIZE)
				{
                    if (attribute->attributeType == 0x30) {
                        FileNameAttributeHeader *fileNameAttribute = (FileNameAttributeHeader *) attribute;

                        if (fileNameAttribute->namespaceType != 2 && !fileNameAttribute->nonResident)
						{
                            File file = {};
                            file.parent = fileNameAttribute->parentRecordNumber;
                            file.name = DuplicateName(fileNameAttribute->fileName, fileNameAttribute->fileNameLength);

						//	fprintf(stderr, "%s\n", file.name);
                            uint64_t oldLength = arrlenu(files);

                            if (fileRecord->recordNumber >= oldLength)
							{
                                arrsetlen(files, fileRecord->recordNumber + 1);
                                memset(files + oldLength, 0, sizeof(File) * (fileRecord->recordNumber - oldLength));
                            }

                            files[fileRecord->recordNumber] = file;
                        }
                    }
                    else if (attribute->attributeType == 0xFFFFFFFF)
					{
                        break;
                    }

                    attribute = (AttributeHeader *) ((uint8_t *) attribute + attribute->length);
                }
            }
        }
    }

    fprintf(stderr, "\nFound %lld files.\n", arrlen(files));

	return 0;
}
