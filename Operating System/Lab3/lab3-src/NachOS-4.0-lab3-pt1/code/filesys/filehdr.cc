// filehdr.cc
//	Routines for managing the disk file header (in UNIX, this
//	would be called the i-node).
//
//	The file header is used to locate where on disk the
//	file's data is stored.  We implement this as a fixed size
//	table of pointers -- each entry in the table points to the
//	disk sector containing that portion of the file data
//	(in other words, there are no indirect or doubly indirect
//	blocks). The table size is chosen so that the file header
//	will be just big enough to fit in one disk sector,
//
//      Unlike in a real system, we do not keep track of file permissions,
//	ownership, last modification date, etc., in the file header.
//
//	A file header can be initialized in two ways:
//	   for a new file, by modifying the in-memory data structure
//	     to point to the newly allocated data blocks
//	   for a file already on disk, by reading the file header from disk
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "filehdr.h"
#include "debug.h"
#include "synchdisk.h"
#include "main.h"

//----------------------------------------------------------------------
// FileHeader::Allocate
// 	Initialize a fresh file header for a newly created file.
//	Allocate data blocks for the file out of the map of free disk blocks.
//	Return FALSE if there are not enough free blocks to accomodate
//	the new file.
//
//	"freeMap" is the bit map of free disk sectors
//	"fileSize" is the size of bits of free disk sectors
//----------------------------------------------------------------------

/* ++++++++++++++ LAB 3 ++++++++++++++++++++ */
bool FileHeader::Allocate(PersistentBitmap *freeMap, int fileSize)
{
     //printf("file allocate");
    numBytes = fileSize;
    numSectors = divRoundUp(fileSize, SectorSize);//向上取整除法，计算所需扇区数
    numIndirectSectors = ((numSectors - NumDirect) + NumInDirectIndex - 1) / NumInDirectIndex;//计算所需二级索引数,没有时为0
    // check if space is enough
    if (freeMap->NumClear() < (numSectors + numIndirectSectors))//判断位图中有无空白位置用以存放索引总共需要的扇区号
        return FALSE;//位图中不够存放扇区号时返回错误

    int i;
    for (i = 0; i < numSectors && i < NumDirect; i++)//完成一级索引内的所有索引位置
    {
        dataSectors[i] = freeMap->FindAndSet();// 赋值为下一个索引，占据位图的下一个空白位置
        // since we checked that there was enough free space,
        // we expect this to succeed
        ASSERT(dataSectors[i] >= 0);//此部检查以在上面检查过故应该不会出现-1的情况
    }

    int doneSec = i;//将此时已用的一级索引数保存给donesec,代表已完成编号的sector
    for (int j = 0; j < numIndirectSectors && doneSec < numSectors; j++){
        int sectors[NumInDirectIndex];
        // 洞1:begin
	clearIndexTable(sectors);
	indirectSectors[j]=freeMap->FindAndSet();//分配位图给二级索引
	for (int k = 0; k < NumInDirectIndex && doneSec < numSectors; k++){//完成二级索引下一级索引的分配
	    sectors[k] = freeMap->FindAndSet();
	    doneSec++;
	}
        kernel->synchDisk->WriteSector(indirectSectors[j], (char*)sectors);
	//将二级索引下的一级索引内容写回二级索引索引对应的sector内
        // 洞1:end
    }
    return TRUE;
}
/* ++++++++++++++++++++++++++++++++++ */

//----------------------------------------------------------------------
// FileHeader::Deallocate
// 	De-allocate all the space allocated for data blocks for this file.
//
//	"freeMap" is the bit map of free disk sectors
//----------------------------------------------------------------------

/* +++++++++++++++ LAB3 ++++++++++++ */
void FileHeader::Deallocate(PersistentBitmap *freeMap)
{
    int i;
     //printf("file Deallocate");
    for (i = 0; i < numSectors && i < NumDirect; i++)
    {
        ASSERT(freeMap->Test((int)dataSectors[i])); // ought to be marked!
        freeMap->Clear((int)dataSectors[i]);
    }
    int doneSec =i;
    for (int j = 0; j < numIndirectSectors && doneSec < numSectors; j++)
    {
        int sectors[NumInDirectIndex];
        // 洞2:begin
	kernel->synchDisk->ReadSector(indirectSectors[j], (char*)sectors);
	for (int k = 0; k < NumInDirectIndex && doneSec < numSectors; k++){
	    ASSERT(freeMap->Test((int)sectors[k]));
	    freeMap->Clear((int)sectors[k]);
	    doneSec++;
	}
	ASSERT(freeMap->Test((int)indirectSectors[j]));
	freeMap->Clear((int)indirectSectors[j]);
        // 洞2:end
    }
}
/* +++++++++++++++++++++++++++++++ */

//----------------------------------------------------------------------
// FileHeader::FetchFrom
// 	Fetch contents of file header from disk.
//
//	"sector" is the disk sector containing the file header
//----------------------------------------------------------------------

void FileHeader::FetchFrom(int sector)
{
    kernel->synchDisk->ReadSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::WriteBack
// 	Write the modified contents of the file header back to disk.
//
//	"sector" is the disk sector to contain the file header
//----------------------------------------------------------------------

void FileHeader::WriteBack(int sector)
{
    kernel->synchDisk->WriteSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::ByteToSector
// 	Return which disk sector is storing a particular byte within the file.
//      This is essentially a translation from a virtual address (the
//	offset in the file) to a physical address (the sector where the
//	data at the offset is stored).
//
//	"offset" is the location within the file of the byte in question
//----------------------------------------------------------------------

/* +++++ LAB3 +++++ */
int FileHeader::ByteToSector(int offset)
{
    //if(offset>0) offset--;
    // printf("find offset\n");
    int sectors[NumInDirectIndex]; //direct sectors under inderect sector
    int indirectIndex = (offset - DirectSize) / InDirectSectorSize;
    int index;
    // 洞3:begin
    if((offset - DirectSize)<0) {
      index=offset/SectorSize;
      //printf("offset=%d,sector=%d\n",offset,dataSectors[index]);
      return dataSectors[index];
    }
    else{
	index=(offset-DirectSize-indirectIndex*InDirectSectorSize)/SectorSize;
	//ASSERT(freeMap->Test((int)dataSectors[i]));
	kernel->synchDisk->ReadSector(indirectSectors[indirectIndex], (char*)sectors);
	//sectors[index]=0;
	//printf("offset=%d,sector=%d\n",offset,sectors[index]);
	return (int)sectors[index];
    }
    // 洞3:end
    return 0;
}
/* +++++++++++++++++++ */

//----------------------------------------------------------------------
// FileHeader::FileLength
// 	Return the number of bytes in the file.
//----------------------------------------------------------------------

int FileHeader::FileLength()
{
    return numBytes;
}

//----------------------------------------------------------------------
// FileHeader::Print
// 	Print the contents of the file header, and the contents of all
//	the data blocks pointed to by the file header.
//----------------------------------------------------------------------

void FileHeader::Print()
{
    int i, j, k;
    char *data = new char[SectorSize];

    printf("FileHeader contents.  File size: %d.", numBytes);
    printf("\nFile contents:\n");
    //direct block
    /* ++++++++++++++++++ LAB 3 ++++++++++++++ */
    for (i = k = 0; i < NumDirect; i++)
    {
        if (dataSectors[i] == -1 || dataSectors[i] > NumSectors)
            break;
        kernel->synchDisk->ReadSector(dataSectors[i], data);
        for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++)
        {
            if ('\040' <= data[j] && data[j] <= '\176') // isprint    (data[j])
                printf("%c", data[j]);
            else
                printf("\\%x", (unsigned char)data[j]);
        }
    }
    if (numBytes > DirectSize)
    {
        for (i = 0; i < MaxIndirect; i++)
        {
            if (indirectSectors[i] == -1)
                break;
            int sectors[NumInDirectIndex];
            kernel->synchDisk->ReadSector(indirectSectors[i], (char *)sectors);
            for (int l = 0; l < NumInDirectIndex; l++)
            {
                if (sectors[l] == -1)
                    break;
                int secid = sectors[l];
                if (secid == -1 || secid > NumSectors)
                    break;
                kernel->synchDisk->ReadSector(sectors[l], data);
                for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++)
                {
                    if ('\040' <= data[j] && data[j] <= '\176') //    isprint(data[j])
                        printf("%c", data[j]);
                    else
                        printf("\\%x", (unsigned char)data[j]);
                }
            }
        }
    }
    printf("\n");
    /* ++++++++++++++++++++++++++++++++++++ */
    delete[] data;
}

/* +++++ LAB3 +++++ */
void FileHeader::clearIndexTable(int *sectors)
{
    for (int i = 0; i < NumInDirectIndex; i++)
        sectors[i] = -1;
}

FileHeader::FileHeader() : numIndirectSectors(0)
{
    for (int i = 0; i < NumDirect; i++)
        dataSectors[i] = -1;
    for (int i = 0; i < MaxIndirect; i++)
        indirectSectors[i] = -1;
}

// 在write的文件大小超过原本给的大小的时候边长.
//expand file to numsector data sectors
int FileHeader::expandFile(int numSec, PersistentBitmap *freeMap)
{
    // if (numSec <= numSectors)
    //     return 1;
    // 需要indirect sector的数量
    int indirectsNeed;//所需的二级索引数量
    if (numSec > NumDirect)//总共所需的sec大于一级索引总量
        indirectsNeed = ((numSec - NumDirect) + NumInDirectIndex - 1) / NumInDirectIndex;
    else
        indirectsNeed = 0;
    // 需要的总的sector的数量
    int sectorsNeed = numSec + indirectsNeed - numSectors - numIndirectSectors;//重新计算所需分配位图的sector数量
    if (freeMap->NumClear() < sectorsNeed)
        // 没有足够的sector
        return -1;

    int i;
    // allocate direct sectors
    for (i = numSectors; i < NumDirect && i < numSec;)//从一级直接索引开始分配
        if (dataSectors[i] == -1)//此处判断意义不大 一直成立 如果判断错误会进入死循环
        {
            dataSectors[i] = freeMap->FindAndSet();//分配一级直接索引
	   // printf("add sector: %d\n",dataSectors[i] );
            i++;
        }

    int doneSec = i;
    // allocate indirect sectors
    for (int j = 0; j < indirectsNeed && doneSec < numSec; j++)
    {
        // 洞4:begin
       int sectors[NumInDirectIndex]; //index table
        
        if (indirectSectors[j] == -1){   
	    clearIndexTable(sectors);
	    indirectSectors[j]= freeMap->FindAndSet();
	   // printf("add indirectsector: %d", indirectSectors[j]);
	    for(int k=0;k<NumInDirectIndex && doneSec < numSec;k++){
		sectors[k]= freeMap->FindAndSet();
		//printf("add sector: %d\n",sectors[k] );
		doneSec++;
	    }
	    kernel->synchDisk->WriteSector(indirectSectors[j], (char*)sectors);
        }
        else{ 
            kernel->synchDisk->ReadSector(indirectSectors[j], (char*)sectors);
	    for(int n=0;n<NumInDirectIndex && doneSec < numSec;n++){
		if(sectors[n]==-1){
		    sectors[n]= freeMap->FindAndSet();
		    doneSec++;
		}
	    }
	    kernel->synchDisk->WriteSector(indirectSectors[j], (char*)sectors);
        }
        // 洞4:end
    }

    numBytes = numSec * SectorSize;
    numSectors = numSec;
    numIndirectSectors = indirectsNeed;
    //printf("%d %d %d\n",numBytes, numSectors, numIndirectSectors);
    return 1;
}

/* ++++++++++ */