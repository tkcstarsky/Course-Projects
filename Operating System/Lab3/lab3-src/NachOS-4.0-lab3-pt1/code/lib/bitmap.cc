// bitmap.cc
//	Routines to manage a bitmap -- an array of bits each of which
//	can be either on or off.  Represented as an array of integers.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "debug.h"
#include "bitmap.h"

//----------------------------------------------------------------------
// BitMap::BitMap
// 	Initialize a bitmap with "numItems" bits, so that every bit is clear.
//	it can be added somewhere on a list.
//
//	"numItems" is the number of bits in the bitmap.
//----------------------------------------------------------------------

Bitmap::Bitmap(int numItems) //位图初始化，全部置0，且最大值设置为numitem
{ 
    int i;

    ASSERT(numItems > 0);

    numBits = numItems;
    numWords = divRoundUp(numBits, BitsInWord);
    map = new unsigned int[numWords];
    for (i = 0; i < numWords; i++) {
	map[i] = 0;		// initialize map to keep Purify happy
    }
    for (i = 0; i < numBits; i++) {
        Clear(i);
    }
}

//----------------------------------------------------------------------
// Bitmap::~Bitmap
// 	De-allocate a bitmap.
//----------------------------------------------------------------------

Bitmap::~Bitmap()
{ 
    delete map;
}

//----------------------------------------------------------------------
// Bitmap::Set
// 	Set the "nth" bit in a bitmap.
//
//	"which" is the number of the bit to be set.
//----------------------------------------------------------------------

void
Bitmap::Mark(int which) //在位图中添加which
{ 
    ASSERT(which >= 0 && which < numBits);

    map[which / BitsInWord] |= 1 << (which % BitsInWord);

    ASSERT(Test(which));
}
    
//----------------------------------------------------------------------
// Bitmap::Clear
// 	Clear the "nth" bit in a bitmap.
//
//	"which" is the number of the bit to be cleared.
//----------------------------------------------------------------------

void 
Bitmap::Clear(int which) //清除which在位图中的存储
{
    ASSERT(which >= 0 && which < numBits);

    map[which / BitsInWord] &= ~(1 << (which % BitsInWord));

    ASSERT(!Test(which));
}

//----------------------------------------------------------------------
// Bitmap::Test
// 	Return TRUE if the "nth" bit is set.
//
//	"which" is the number of the bit to be tested.
//----------------------------------------------------------------------

bool 
Bitmap::Test(int which) const//判断which是否已存在在位图中
{
    ASSERT(which >= 0 && which < numBits);
    
    if (map[which / BitsInWord] & (1 << (which % BitsInWord))) {
	return TRUE;
    } else {
	return FALSE;
    }
}

//----------------------------------------------------------------------
// Bitmap::FindAndSet
// 	Return the number of the first bit which is clear.
//	As a side effect, set the bit (mark it as in use).
//	(In other words, find and allocate a bit.)
//
//	If no bits are clear, return -1.
//----------------------------------------------------------------------

int 
Bitmap::FindAndSet() //查找并添加从0到numbits的第一个没有的数字进入位图，返回该数字，全部有时返回-1
{
    for (int i = 0; i < numBits; i++) {
	if (!Test(i)) {
	    Mark(i);
	    //printf("bitmap find %d\n",i);
	    return i;
	}
    }
    return -1;
}

//----------------------------------------------------------------------
// Bitmap::NumClear
// 	Return the number of clear bits in the bitmap.
//	(In other words, how many bits are unallocated?)
//----------------------------------------------------------------------

int 
Bitmap::NumClear() const//查找位图中有多少空缺数字
{
    int count = 0;

    for (int i = 0; i < numBits; i++) {
	if (!Test(i)) {
	    count++;
	}
    }
    return count;
}

//----------------------------------------------------------------------
// Bitmap::Print
// 	Print the contents of the bitmap, for debugging.
//
//	Could be done in a number of ways, but we just print the #'s of
//	all the bits that are set in the bitmap.
//----------------------------------------------------------------------

void
Bitmap::Print() const//打印位图中的存在的数字
{
    cout << "Bitmap set:\n"; 
    for (int i = 0; i < numBits; i++) {
	if (Test(i)) {
	    cout << i << ", ";
	}
    }
    cout << "\n"; 
}


//----------------------------------------------------------------------
// Bitmap::SelfTest
// 	Test whether this module is working.
//----------------------------------------------------------------------

void
Bitmap::SelfTest() //测试程序
{
    int i;
    
    ASSERT(numBits >= BitsInWord);	// bitmap must be big enough

    ASSERT(NumClear() == numBits);	// bitmap must be empty
    ASSERT(FindAndSet() == 0);
    Mark(31);
    ASSERT(Test(0) && Test(31));

    ASSERT(FindAndSet() == 1);
    Clear(0);
    Clear(1);
    Clear(31);

    for (i = 0; i < numBits; i++) {
        Mark(i);
    }
    ASSERT(FindAndSet() == -1);		// bitmap should be full!
    for (i = 0; i < numBits; i++) {
        Clear(i);
    }
}
