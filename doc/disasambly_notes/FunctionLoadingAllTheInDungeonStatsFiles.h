const char * Table_20AF6A0h[6] =
{
    0x02094B78, //"MONSTER/monster.bin"
    0x02094BB4, //"MONSTER/m_attack.bin"
    0x02094BCC, //"MONSTER/m_ground.bin"
    0x02094B64, //"EFFECT/effect.bin"
    0x02094BA0, //"DUNGEON/dungeon.bin"
    0x02094B8C, //"BALANCE/m_level.bin"
};

20AF6A0h            //PtrPackFileNamesTable 020AF6A0



///
/// 0200C2DC LoadPackFiles 200C2DCh
///
// This loads all "pack" files used inside dungeons
0200C2DC E92D40F8 push    r3-r7,r14
0200C2E0 E3A00F7E mov     r0,1F8h           //r0 = 0x1F8
0200C2E4 E3A01000 mov     r1,0h             //r1 = 0
0200C2E8 EBFFD3A0 bl      2001170h          //MemAlloc(elemlen, nbelem)
0200C2EC E59F2040 ldr     r2,=20AF69Ch      //r2 = 20AF69Ch //ptrallocatedbuff
0200C2F0 E3A01F7E mov     r1,1F8h           //r1 = 0x1F8
0200C2F4 E5820000 str     r0,[r2]           //[20AF69Ch] = AllocatedBuffer
0200C2F8 EBFFDBD4 bl      2003250h          ///FillBufferWithZeros( R0=buffaddress, R1=BuffLen )
0200C2FC E59F6034 ldr     r6,=20AF6A0h      //PtrPackFileNamesTable 020AF6A0
0200C300 E59F502C ldr     r5,=20AF69Ch      //r5 = 20AF69Ch //ptrallocatedbuff
0200C304 E3A07000 mov     r7,0h             //r7 = 0 //CNT
0200C308 E3A04054 mov     r4,54h            //r4 = 0x54
//200C30C LOOP_BEG
0200C30C E5950000 ldr     r0,[r5]           //r0 = [r5]
0200C310 E7961107 ldr     r1,[r6,r7,lsl 2h]     //Load the current filename str pointer from the pointer table!
0200C314 E0200497 mla     r0,r7,r4,r0           //R0 = CNT * 0x54 + BaseAddress
0200C318 EB000030 bl      200C3E0h
{
    0200C3E0 E92D4078 push    r3-r6,r14
    0200C3E4 E24DD004 sub     r13,r13,4h
    0200C3E8 E1A06000 mov     r6,r0             //R0 = StructFileDetails
    0200C3EC E1A05001 mov     r5,r1             //R5 = PtrStringFName
    0200C3F0 EBFFEF5C bl      2008168h          //FStreamAlloc()    
    0200C3F4 E286000C add     r0,r6,0Ch
    0200C3F8 EBFFEF81 bl      2008204h          //FStreamCtor(r0 = PtrFStreamStruct)
    0200C3FC E1A01005 mov     r1,r5             //R1 = PtrFilename
    0200C400 E286000C add     r0,r6,0Ch
    0200C404 EBFFEF81 bl      2008210h          //FStreamFOpen(r0 = PtrFStreamStruct, r1 = PtrFPath)
    0200C408 E286000C add     r0,r6,0Ch
    0200C40C E2861004 add     r1,r6,4h
    0200C410 E3A02008 mov     r2,8h
    0200C414 EBFFEF8E bl      2008254h          ///FStreamRead(r0 = PtrFStreamStruct, r1 = PtrOutBuffer, r2 = BegOffset, r3 = EndOffset )
    0200C418 E286000C add     r0,r6,0Ch
    0200C41C E3A01008 mov     r1,8h
    0200C420 E3A02000 mov     r2,0h
    0200C424 EBFFEF9F bl      20082A8h          ///020082A8 FStreamSeek( r0 = PtrFStreamStruct, r1 = OffsetToSeekTo, r2 = unknown?(usually 0)  ) 
    0200C428 E5960008 ldr     r0,[r6,8h]
    0200C42C E3A01000 mov     r1,0h
    0200C430 E1A04180 mov     r4,r0,lsl 3h
    0200C434 E1A00004 mov     r0,r4
    0200C438 EBFFD34C bl      2001170h          ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
    0200C43C E5860000 str     r0,[r6]
    0200C440 E5961000 ldr     r1,[r6]
    0200C444 E286000C add     r0,r6,0Ch
    0200C448 E1A02004 mov     r2,r4
    0200C44C EBFFEF80 bl      2008254h          ///FStreamRead(r0 = PtrFStreamStruct, r1 = PtrOutBuffer, r2 = BegOffset, r3 = EndOffset )
    0200C450 E58D5000 str     r5,[r13]
    0200C454 E59F0014 ldr     r0,=2094BE4h
    0200C458 E1A03004 mov     r3,r4
    0200C45C E9960006 ldmib   [r6],r1,r2
    0200C460 EBFFFF65 bl      200C1FCh
    0200C464 EBFFEF4A bl      2008194h
    0200C468 E28DD004 add     r13,r13,4h
    0200C46C E8BD8078 pop     r3-r6,r15
    0200C470 02094BE4
}
0200C31C E2870001 add     r0,r7,1h
0200C320 E1A00800 mov     r0,r0,lsl 10h
0200C324 E1A07840 mov     r7,r0,asr 10h
0200C328 E3570006 cmp     r7,6h
if(R7 < 6)
    0200C32C BAFFFFF6 blt     200C30Ch      //200C30C LOOP_BEG
0200C330 E8BD80F8 pop     r3-r7,r15
0200C334 020AF69C
0200C338 020AF6A0



///
/// 0200C33C LookupPackFileSubFiles 200C33Ch (r0 = fileid (0 to 5), r1 = subfileindex )
///
//Was called from : 0201D544
0200C33C E59F3010 ldr     r3,=20AF69Ch
0200C340 E3A02054 mov     r2,54h                //r2 = 0x54
0200C344 E5933000 ldr     r3,[r3]               //r3 = [20AF69Ch]
0200C348 E59FC008 ldr     r12,=200C474h         //r12 = 200C474h
0200C34C E0203092 mla     r0,r2,r0,r3           ///r0 = (0x54 * r0) + [20AF69Ch]
0200C350 E12FFF1C bx      r12
{
    0200C474 E5900000 ldr     r0,[r0]           //r0 = PtrBegBuffer
    0200C478 E0800181 add     r0,r0,r1,lsl 3h   //r0 = PtrBegBuffer + (subfileindex * 8)
    0200C47C E5900004 ldr     r0,[r0,4h]        //r0 = [(PtrBegBuffer + (subfileindex * 8)),4h]
    0200C480 E12FFF1E bx      r14
}
0200C354 020AF69C 
0200C358 0200C474 

///
///
///
0200C35C E92D4008 push    r3,r14
0200C360 E59F301C ldr     r3,=20AF69Ch
0200C364 E1A0C001 mov     r12,r1
0200C368 E5933000 ldr     r3,[r3]
0200C36C E3A01054 mov     r1,54h
0200C370 E0203091 mla     r0,r1,r0,r3
0200C374 E1A01002 mov     r1,r2
0200C378 E1A0200C mov     r2,r12
0200C37C EB000040 bl      200C484h
{
    0200C484 E92D4070 push    r4-r6,r14
    0200C488 E1A06000 mov     r6,r0
    0200C48C E1A05001 mov     r5,r1
    0200C490 E1A04002 mov     r4,r2
    0200C494 EBFFEF33 bl      2008168h
    0200C498 E5962000 ldr     r2,[r6]
    0200C49C E286000C add     r0,r6,0Ch
    0200C4A0 E7921184 ldr     r1,[r2,r4,lsl 3h]
    0200C4A4 E0824184 add     r4,r2,r4,lsl 3h
    0200C4A8 E3A02000 mov     r2,0h
    0200C4AC EBFFEF7D bl      20082A8h
    0200C4B0 E5942004 ldr     r2,[r4,4h]
    0200C4B4 E1A01005 mov     r1,r5
    0200C4B8 E286000C add     r0,r6,0Ch
    0200C4BC EBFFEF64 bl      2008254h
    0200C4C0 EBFFEF33 bl      2008194h
    0200C4C4 E5940004 ldr     r0,[r4,4h]
    0200C4C8 E8BD8070 pop     r4-r6,r15
}
0200C380 E8BD8008 pop     r3,r15
0200C384 020AF69C 

///
/// 0200C388 fun_200C388h  200C388h (r0 = packfileid, r1 = subfileindex, r2 = targetpair (ptrfbuffer,flen), r3 = nbelem??)
///
//r0 = packfileid
//r1 = subfileindex
//r2 = targetpair (ptrfbuffer,flen)
//r3 = nbelem??
0200C388 E92D40F8 push    r3-r7,r14
0200C38C E3A0C054 mov     r12,54h               //r12 = 0x54
0200C390 E0040C90 mul     r4,r0,r12             //r4 = packfileid * 0x54
0200C394 E59F0040 ldr     r0,=20AF69Ch          //r0 = 20AF69Ch
0200C398 E1A06002 mov     r6,r2                 //r6 = targetpair
0200C39C E5900000 ldr     r0,[r0]               //r0 = [20AF69Ch]
0200C3A0 E1A05003 mov     r5,r3                 //r5 = r3
0200C3A4 E0800004 add     r0,r0,r4              //r0 = r0 + (packfileid * 0x54)
0200C3A8 E1A07001 mov     r7,r1                 //r7 = subfileindex
0200C3AC EB000030 bl      200C474h
{
    0200C474 E5900000 ldr     r0,[r0]           //r0 = PtrBegBuffer
    0200C478 E0800181 add     r0,r0,r1,lsl 3h   //r0 = PtrBegBuffer + (subfileindex * 8)
    0200C47C E5900004 ldr     r0,[r0,4h]        //r0 = [(PtrBegBuffer + (subfileindex * 8)),4h] //Subfilelen
    0200C480 E12FFF1E bx      r14
}
0200C3B0 E5860004 str     r0,[r6,4h]            //[r6,4h] = Subfilelen
0200C3B4 E1A01005 mov     r1,r5                 //r1 = r5
0200C3B8 EBFFD36C bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
0200C3BC E5860000 str     r0,[r6]               //[r6] = ptrallocatedbuff
0200C3C0 E59F0014 ldr     r0,=20AF69Ch          //r0 = 20AF69Ch
0200C3C4 E5961000 ldr     r1,[r6]               //r1 = [ptrallocatedbuff]
0200C3C8 E5900000 ldr     r0,[r0]               //r0 = [20AF69Ch]
0200C3CC E1A02007 mov     r2,r7                 //r2 = subfileindex
0200C3D0 E0800004 add     r0,r0,r4              //r0 = [20AF69Ch] + (packfileid * 0x54)
0200C3D4 EB00002A bl      200C484h
{
    0200C484 E92D4070 push    r4-r6,r14
    0200C488 E1A06000 mov     r6,r0
    0200C48C E1A05001 mov     r5,r1
    0200C490 E1A04002 mov     r4,r2
    0200C494 EBFFEF33 bl      2008168h
    0200C498 E5962000 ldr     r2,[r6]
    0200C49C E286000C add     r0,r6,0Ch
    0200C4A0 E7921184 ldr     r1,[r2,r4,lsl 3h]
    0200C4A4 E0824184 add     r4,r2,r4,lsl 3h
    0200C4A8 E3A02000 mov     r2,0h
    0200C4AC EBFFEF7D bl      20082A8h
    0200C4B0 E5942004 ldr     r2,[r4,4h]
    0200C4B4 E1A01005 mov     r1,r5
    0200C4B8 E286000C add     r0,r6,0Ch
    0200C4BC EBFFEF64 bl      2008254h
    0200C4C0 EBFFEF33 bl      2008194h
    0200C4C4 E5940004 ldr     r0,[r4,4h]
    0200C4C8 E8BD8070 pop     r4-r6,r15
}
0200C3D8 E8BD80F8 pop     r3-r7,r15
0200C3DC 020AF69C 

///
/// 0205882C fun_205882Ch 205882Ch
///
//Loads from dungeon.bin!
//r0 = param1 022F454C E3A00D0F mov     r0,3C0h
0205882C E92D47F0 push    r4-r10,r14
02058830 E24DD030 sub     r13,r13,30h
02058834 E59F1268 ldr     r1,=3E3h                  //r1 = 3E3h //File #996
02058838 E1A04000 mov     r4,r0                     //r4 = param1
0205883C E3A03000 mov     r3,0h                     //r3 = 0
02058840 E28D2014 add     r2,r13,14h
02058844 E3A00004 mov     r0,4h                     //4 aka "DUNGEON/dungeon.bin"
02058848 E58D301C str     r3,[r13,1Ch]
0205884C EBFECECD bl      200C388h                  ///fun_200C388h(r0 = packfileid, r1 = subfileindex, r2 = targetpair (ptrfbuffer,flen), r3 = nbelem??)
02058850 E59D1014 ldr     r1,[r13,14h]              //r1 = PtrLoadedsubfile
02058854 E28D001C add     r0,r13,1Ch                //r0 = r13 + 0x1C 
02058858 EBFF1B15 bl      201F4B4h                  ///HandleSir0Translation(R0=OutPtrToSIR0Data, R1=filebuffer)
0205885C E1A00804 mov     r0,r4,lsl 10h             //r0 = param1 << 16
02058860 E1A00840 mov     r0,r0,asr 10h             //r0 = r0 >> 16 (signed)    
02058864 E3A01000 mov     r1,0h                     //r1 = 0
02058868 EB000093 bl      2058ABCh                  //
{
    02058ABC E59F3030 ldr     r3,=20AFC4Ch          //r3 = 20AFC4Ch
    02058AC0 E3A02070 mov     r2,70h                //r2 = 0x70   
    02058AC4 E593C000 ldr     r12,[r3]              //r12 = [20AFC4Ch]
    02058AC8 E59F3028 ldr     r3,=20AFC28h          //r3  = 20AFC28h
    02058ACC E022C291 mla     r2,r1,r2,r12          //r2  = (r1 * 0x70) + [20AFC4Ch]
    02058AD0 E1D210BC ldrh    r1,[r2,0Ch]           //r1  = [r2,0Ch] (ex: 0x0002)
    02058AD4 E592C064 ldr     r12,[r2,64h]          //r12 = [r2,64h] (ex: 0x06400000)
    02058AD8 E1A02FA1 mov     r2,r1,lsr 1Fh         //r2  = r1 >> 31
    02058ADC E0621F01 rsb     r1,r2,r1,lsl 1Eh      //r1  = (r1 << 30) - r2
    02058AE0 E0821F61 add     r1,r2,r1,ror 1Eh      //r1  = r2 + (r1 ror 30)
    02058AE4 E1A01081 mov     r1,r1,lsl 1h          //r1  = r1 * 2
    02058AE8 E19310F1 ldrsh   r1,[r3,r1]            //r1  = [20AFC28h,r1]   (ex: 0x0080)
    02058AEC E100C180 smlabb  r0,r0,r1,r12          //r0  = (r0 * r1) + r12
    02058AF0 E12FFF1E bx      r14
    02058AF4 020AFC4C
    02058AF8 020AFC28
}
0205886C E1A00804 mov     r0,r4,lsl 10h                 //r0 = param1 << 16
02058870 E59D501C ldr     r5,[r13,1Ch]                  //r5 = ptrsir0datablkbeg
02058874 E1A00840 mov     r0,r0,asr 10h                 //r0 = r0 >> 16 (signed)
02058878 E3A01000 mov     r1,0h                         //r1 = 0
0205887C EB00008E bl      2058ABCh
{
    02058ABC E59F3030 ldr     r3,=20AFC4Ch          //r3 = 20AFC4Ch
    02058AC0 E3A02070 mov     r2,70h                //r2 = 0x70   
    02058AC4 E593C000 ldr     r12,[r3]              //r12 = [20AFC4Ch]
    02058AC8 E59F3028 ldr     r3,=20AFC28h          //r3  = 20AFC28h
    02058ACC E022C291 mla     r2,r1,r2,r12          //r2  = (r1 * 0x70) + [20AFC4Ch]
    02058AD0 E1D210BC ldrh    r1,[r2,0Ch]           //r1  = [r2,0Ch] (ex: 0x0002)
    02058AD4 E592C064 ldr     r12,[r2,64h]          //r12 = [r2,64h] (ex: 0x06400000)
    02058AD8 E1A02FA1 mov     r2,r1,lsr 1Fh         //r2  = r1 >> 31
    02058ADC E0621F01 rsb     r1,r2,r1,lsl 1Eh      //r1  = (r1 << 30) - r2
    02058AE0 E0821F61 add     r1,r2,r1,ror 1Eh      //r1  = r2 + (r1 ror 30)
    02058AE4 E1A01081 mov     r1,r1,lsl 1h          //r1  = r1 * 2
    02058AE8 E19310F1 ldrsh   r1,[r3,r1]            //r1  = [20AFC28h,r1]   (ex: 0x0080)
    02058AEC E100C180 smlabb  r0,r0,r1,r12          //r0  = (r0 * r1) + r12
    02058AF0 E12FFF1E bx      r14
    02058AF4 020AFC4C
    02058AF8 020AFC28
}
02058880 E2851004 add     r1,r5,4h                  //r1 = ptrsir0datablkbeg + 4
02058884 E3A02040 mov     r2,40h                    //r2 = 0x40
02058888 E3A03009 mov     r3,9h                     //r3 = 9
0205888C EBFF0DD7 bl      201BFF0h
{
    0201BFF0 E59FC004 ldr     r12,=1FF8000h
    0201BFF4 E1A020C2 mov     r2,r2,asr 1h          //r2 = 0x40 >> 1
    0201BFF8 E12FFF1C bx      r12
    {
        0201BFFC 01FF8000 mvneqs  r8,r0
        0201C000 E59F201C ldr     r2,=0FFFFh        //r2 = 0xFFFF    
        0201C004 E3A01000 mov     r1,0h             //r1 = 0
        0201C008 E1C020B0 strh    r2,[r0]           //[r0] = r2
        0201C00C E1C020B2 strh    r2,[r0,2h]
        0201C010 E1C020B4 strh    r2,[r0,4h]
        0201C014 E1C010B6 strh    r1,[r0,6h]
        0201C018 E1C010B8 strh    r1,[r0,8h]
        0201C01C E1C010BA strh    r1,[r0,0Ah]
        0201C020 E12FFF1E bx      r14
        0201C024 0000FFFF 
    }
    0201BFFC 01FF8000 
}
02058890 E2840001 add     r0,r4,1h                  //r0 = param1 + 1
02058894 E1A00800 mov     r0,r0,lsl 10h             //r0 = r0 << 16
02058898 E1A00840 mov     r0,r0,asr 10h             //r0 = r0 >> 16 (signed)
0205889C E3A01000 mov     r1,0h                     //r1 = 0
020588A0 EB000085 bl      2058ABCh
{
    02058ABC E59F3030 ldr     r3,=20AFC4Ch          //r3 = 20AFC4Ch
    02058AC0 E3A02070 mov     r2,70h                //r2 = 0x70   
    02058AC4 E593C000 ldr     r12,[r3]              //r12 = [20AFC4Ch]
    02058AC8 E59F3028 ldr     r3,=20AFC28h          //r3  = 20AFC28h
    02058ACC E022C291 mla     r2,r1,r2,r12          //r2  = (r1 * 0x70) + [20AFC4Ch]
    02058AD0 E1D210BC ldrh    r1,[r2,0Ch]           //r1  = [r2,0Ch] (ex: 0x0002)
    02058AD4 E592C064 ldr     r12,[r2,64h]          //r12 = [r2,64h] (ex: 0x06400000)
    02058AD8 E1A02FA1 mov     r2,r1,lsr 1Fh         //r2  = r1 >> 31
    02058ADC E0621F01 rsb     r1,r2,r1,lsl 1Eh      //r1  = (r1 << 30) - r2
    02058AE0 E0821F61 add     r1,r2,r1,ror 1Eh      //r1  = r2 + (r1 ror 30)
    02058AE4 E1A01081 mov     r1,r1,lsl 1h          //r1  = r1 * 2
    02058AE8 E19310F1 ldrsh   r1,[r3,r1]            //r1  = [20AFC28h,r1]   (ex: 0x0080)
    02058AEC E100C180 smlabb  r0,r0,r1,r12          //r0  = (r0 * r1) + r12
    02058AF0 E12FFF1E bx      r14
    02058AF4 020AFC4C
    02058AF8 020AFC28
}
020588A4 E2851024 add     r1,r5,24h                 //r1 = ptrsir0datablkbeg + 0x24
020588A8 E3A02080 mov     r2,80h                    //r2 = 0x80
020588AC E3A03009 mov     r3,9h                     //r3 = 9
020588B0 EBFF0DCE bl      201BFF0h
{
    0201BFF0 E59FC004 ldr     r12,=1FF8000h
    0201BFF4 E1A020C2 mov     r2,r2,asr 1h          //r2 = r2 >> 1
    0201BFF8 E12FFF1C bx      r12
    {
        0201BFFC 01FF8000 mvneqs  r8,r0
        0201C000 E59F201C ldr     r2,=0FFFFh        //r2 = 0xFFFF    
        0201C004 E3A01000 mov     r1,0h             //r1 = 0
        0201C008 E1C020B0 strh    r2,[r0]           //[r0] = r2
        0201C00C E1C020B2 strh    r2,[r0,2h]
        0201C010 E1C020B4 strh    r2,[r0,4h]
        0201C014 E1C010B6 strh    r1,[r0,6h]
        0201C018 E1C010B8 strh    r1,[r0,8h]
        0201C01C E1C010BA strh    r1,[r0,0Ah]
        0201C020 E12FFF1E bx      r14
        0201C024 0000FFFF 
    }
    0201BFFC 01FF8000 
}
020588B4 E2840002 add     r0,r4,2h                  //r0 = param1 + 2
020588B8 E1A00800 mov     r0,r0,lsl 10h             //r0 = r0 << 16
020588BC E1A00840 mov     r0,r0,asr 10h             //r0 = r0 >> 16
020588C0 E3A01000 mov     r1,0h                     //r1 = 0
020588C4 EB00007C bl      2058ABCh
{
    02058ABC E59F3030 ldr     r3,=20AFC4Ch          //r3 = 20AFC4Ch
    02058AC0 E3A02070 mov     r2,70h                //r2 = 0x70   
    02058AC4 E593C000 ldr     r12,[r3]              //r12 = [20AFC4Ch]
    02058AC8 E59F3028 ldr     r3,=20AFC28h          //r3  = 20AFC28h
    02058ACC E022C291 mla     r2,r1,r2,r12          //r2  = (r1 * 0x70) + [20AFC4Ch]
    02058AD0 E1D210BC ldrh    r1,[r2,0Ch]           //r1  = [r2,0Ch] (ex: 0x0002)
    02058AD4 E592C064 ldr     r12,[r2,64h]          //r12 = [r2,64h] (ex: 0x06400000)
    02058AD8 E1A02FA1 mov     r2,r1,lsr 1Fh         //r2  = r1 >> 31
    02058ADC E0621F01 rsb     r1,r2,r1,lsl 1Eh      //r1  = (r1 << 30) - r2
    02058AE0 E0821F61 add     r1,r2,r1,ror 1Eh      //r1  = r2 + (r1 ror 30)
    02058AE4 E1A01081 mov     r1,r1,lsl 1h          //r1  = r1 * 2
    02058AE8 E19310F1 ldrsh   r1,[r3,r1]            //r1  = [20AFC28h,r1]   (ex: 0x0080)
    02058AEC E100C180 smlabb  r0,r0,r1,r12          //r0  = (r0 * r1) + r12
    02058AF0 E12FFF1E bx      r14
    02058AF4 020AFC4C
    02058AF8 020AFC28
}
020588C8 E2851064 add     r1,r5,64h                 //r1 = ptrsir0datablkbeg + 0x64
020588CC E3A02C01 mov     r2,100h                   //r2 = 0x100
020588D0 E3A03009 mov     r3,9h                     //r3 = 9
020588D4 EBFF0DC5 bl      201BFF0h
{
    0201BFF0 E59FC004 ldr     r12,=1FF8000h
    0201BFF4 E1A020C2 mov     r2,r2,asr 1h          //r2 = r2 >> 1
    0201BFF8 E12FFF1C bx      r12
    {
        0201BFFC 01FF8000 mvneqs  r8,r0
        0201C000 E59F201C ldr     r2,=0FFFFh        //r2 = 0xFFFF    
        0201C004 E3A01000 mov     r1,0h             //r1 = 0
        0201C008 E1C020B0 strh    r2,[r0]           //[r0] = r2
        0201C00C E1C020B2 strh    r2,[r0,2h]
        0201C010 E1C020B4 strh    r2,[r0,4h]
        0201C014 E1C010B6 strh    r1,[r0,6h]
        0201C018 E1C010B8 strh    r1,[r0,8h]
        0201C01C E1C010BA strh    r1,[r0,0Ah]
        0201C020 E12FFF1E bx      r14
        0201C024 0000FFFF 
    }
    0201BFFC 01FF8000 
}
020588D8 E28D0014 add     r0,r13,14h                //r0 = r13 + 14h
020588DC EBFEC0BC bl      2008BD4h                  ///fun_02008BD4 Memfree wrapper    
020588E0 E3A0E000 mov     r14,0h                    //r14 = 0
020588E4 E59F31BC ldr     r3,=20A3468h              //r3 = 020A3468
020588E8 E59F51BC ldr     r5,=22B578Ch              //r5 = 022B578C
020588EC E083220E add     r2,r3,r14,lsl 4h			//r2 = 020A3468 + (r14 << 4)
020588F0 E5921008 ldr     r1,[r2,8h]				//r1 = [r2 + 0x8]
020588F4 E59F01B4 ldr     r0,=3FFh					//r0 = 0x3FF
020588F8 E0841001 add     r1,r4,r1					//r1 = param1 + r1
020588FC E085720E add     r7,r5,r14,lsl 4h			//r7 = 022B578C + (r14 << 4)
02058900 E3A09B01 mov     r9,400h					//r9 = 0x400
02058904 E793320E ldr     r3,[r3,r14,lsl 4h]		//r3 = [020A3468 + (r14 << 4)]
02058908 E0018000 and     r8,r1,r0					//r8 = r1 & 0x3FF
0205890C E5922004 ldr     r2,[r2,4h]				//r2 = [r2 + 0x4]
02058910 E1A01F03 mov     r1,r3,lsl 1Eh				//r1 = r3 << 0x1E
02058914 E3A05000 mov     r5,0h						//r5 = 0
02058918 E1A00F02 mov     r0,r2,lsl 1Eh				//r0 = r2 << 0x1E
0205891C E2699000 rsb     r9,r9,0h					//
02058920 E1A02005 mov     r2,r5
02058924 E3A03001 mov     r3,1h
02058928 E1A0C185 mov     r12,r5,lsl 3h
0205892C E197A0BC ldrh    r10,[r7,r12]
02058930 E0876185 add     r6,r7,r5,lsl 3h
02058934 E3550001 cmp     r5,1h
02058938 E3CAAC01 bic     r10,r10,100h
0205893C E187A0BC strh    r10,[r7,r12]
02058940 E197A0BC ldrh    r10,[r7,r12]
02058944 E2855001 add     r5,r5,1h
02058948 E3CAAC02 bic     r10,r10,200h
0205894C E187A0BC strh    r10,[r7,r12]
02058950 E1D6A0B0 ldrh    r10,[r6]
02058954 01A0C003 moveq   r12,r3
02058958 11A0C002 movne   r12,r2
0205895C E3CAAB03 bic     r10,r10,0C00h
02058960 E1C6A0B0 strh    r10,[r6]
02058964 E1D6A0B0 ldrh    r10,[r6]
02058968 E1A0CF0C mov     r12,r12,lsl 1Eh
0205896C E3550002 cmp     r5,2h
02058970 E18AAA2C orr     r10,r10,r12,lsr 14h
02058974 E1C6A0B0 strh    r10,[r6]
02058978 E1D6C0B0 ldrh    r12,[r6]
0205897C E3CCCA01 bic     r12,r12,1000h
02058980 E1C6C0B0 strh    r12,[r6]
02058984 E1D6C0B0 ldrh    r12,[r6]
02058988 E3CCCA02 bic     r12,r12,2000h
0205898C E1C6C0B0 strh    r12,[r6]
02058990 E1D6C0B0 ldrh    r12,[r6]
02058994 E38CCA02 orr     r12,r12,2000h
02058998 E1C6C0B0 strh    r12,[r6]
0205899C E1D6C0B0 ldrh    r12,[r6]
020589A0 E3CCC903 bic     r12,r12,0C000h
020589A4 E1C6C0B0 strh    r12,[r6]
020589A8 E1D6C0B0 ldrh    r12,[r6]
020589AC E18CC821 orr     r12,r12,r1,lsr 10h
020589B0 E1C6C0B0 strh    r12,[r6]
020589B4 E1D6C0B2 ldrh    r12,[r6,2h]
020589B8 E3CCCC3E bic     r12,r12,3E00h
020589BC E1C6C0B2 strh    r12,[r6,2h]
020589C0 E1D6C0B2 ldrh    r12,[r6,2h]
020589C4 E3CCC903 bic     r12,r12,0C000h
020589C8 E1C6C0B2 strh    r12,[r6,2h]
020589CC E1D6C0B2 ldrh    r12,[r6,2h]
020589D0 E18CC820 orr     r12,r12,r0,lsr 10h
020589D4 E1C6C0B2 strh    r12,[r6,2h]
020589D8 E1D6C0B4 ldrh    r12,[r6,4h]
020589DC E00CC009 and     r12,r12,r9
020589E0 E1C6C0B4 strh    r12,[r6,4h]
020589E4 E1D6C0B4 ldrh    r12,[r6,4h]
020589E8 E18CC008 orr     r12,r12,r8
020589EC E1C6C0B4 strh    r12,[r6,4h]
020589F0 E1D6C0B4 ldrh    r12,[r6,4h]
020589F4 E3CCCB03 bic     r12,r12,0C00h
020589F8 E1C6C0B4 strh    r12,[r6,4h]
020589FC E1D6C0B4 ldrh    r12,[r6,4h]
02058A00 E38CCB03 orr     r12,r12,0C00h
02058A04 E1C6C0B4 strh    r12,[r6,4h]
02058A08 E1D6C0B4 ldrh    r12,[r6,4h]
02058A0C E3CCCA0F bic     r12,r12,0F000h
02058A10 E1C6C0B4 strh    r12,[r6,4h]
02058A14 E1D6C0B6 ldrh    r12,[r6,6h]
02058A18 E3CCC001 bic     r12,r12,1h
02058A1C E1C6C0B6 strh    r12,[r6,6h]
02058A20 E1D6C0B6 ldrh    r12,[r6,6h]
02058A24 E3CCC002 bic     r12,r12,2h
02058A28 E1C6C0B6 strh    r12,[r6,6h]
02058A2C BAFFFFBD blt     2058928h
02058A30 E28EE001 add     r14,r14,1h
02058A34 E35E0003 cmp     r14,3h
02058A38 BAFFFFA9 blt     20588E4h
02058A3C E59F1070 ldr     r1,=3E5h
02058A40 E28D200C add     r2,r13,0Ch
02058A44 E3A00004 mov     r0,4h
02058A48 E3A03000 mov     r3,0h
02058A4C EBFECE4D bl      200C388h
02058A50 E59D100C ldr     r1,[r13,0Ch]
02058A54 E28D0008 add     r0,r13,8h
02058A58 EBFF1A95 bl      201F4B4h
02058A5C E3A00090 mov     r0,90h
02058A60 E58D0000 str     r0,[r13]
02058A64 E3A000FF mov     r0,0FFh
02058A68 E58D0004 str     r0,[r13,4h]
02058A6C E59D1008 ldr     r1,[r13,8h]
02058A70 E28D0020 add     r0,r13,20h
02058A74 E3A02001 mov     r2,1h
02058A78 E3A03020 mov     r3,20h
02058A7C EBFF1AC5 bl      201F598h
02058A80 E59F0030 ldr     r0,=20AFC4Ch
02058A84 E28D1020 add     r1,r13,20h
02058A88 E5900000 ldr     r0,[r0]
02058A8C E3A02000 mov     r2,0h
02058A90 EBFF0A69 bl      201B43Ch				//0201B43C fun_201B43Ch
{
	0201B43C E92D43F8 push    r3-r9,r14
	0201B440 E1A06001 mov     r6,r1
	0201B444 E5D6100A ldrb    r1,[r6,0Ah]
	0201B448 E1A07000 mov     r7,r0
	0201B44C E1D600B4 ldrh    r0,[r6,4h]
	0201B450 E1A05002 mov     r5,r2
	0201B454 E35100FF cmp     r1,0FFh
	0201B458 11A05001 movne   r5,r1
	0201B45C E3500000 cmp     r0,0h
	0201B460 1A000014 bne     201B4B8h
	0201B464 E3550010 cmp     r5,10h
	0201B468 2A000007 bcs     201B48Ch
	0201B46C E1D620F6 ldrsh   r2,[r6,6h]
	0201B470 E1A01205 mov     r1,r5,lsl 4h
	0201B474 E5970008 ldr     r0,[r7,8h]
	0201B478 E5963000 ldr     r3,[r6]
	0201B47C E20110FF and     r1,r1,0FFh
	0201B480 E20220FF and     r2,r2,0FFh
	0201B484 EBFFFE40 bl      201AD8Ch
	0201B488 EA000029 b       201B534h
	0201B48C E5961000 ldr     r1,[r6]
	0201B490 E2450010 sub     r0,r5,10h
	0201B494 E58D1000 str     r1,[r13]
	0201B498 E1A00800 mov     r0,r0,lsl 10h
	0201B49C E1A01820 mov     r1,r0,lsr 10h
	0201B4A0 E5970008 ldr     r0,[r7,8h]
	0201B4A4 E20110FF and     r1,r1,0FFh
	0201B4A8 E3A02000 mov     r2,0h
	0201B4AC E3A03010 mov     r3,10h
	0201B4B0 EBFFFE59 bl      201AE1Ch
	0201B4B4 EA00001E b       201B534h
	0201B4B8 E3500001 cmp     r0,1h
	0201B4BC 1A00001C bne     201B534h
	0201B4C0 E5D6000B ldrb    r0,[r6,0Bh]
	0201B4C4 E3500000 cmp     r0,0h
	0201B4C8 0A000012 beq     201B518h
	0201B4CC E1D610F6 ldrsh   r1,[r6,6h]
	0201B4D0 E3A04000 mov     r4,0h
	0201B4D4 E3A09010 mov     r9,10h
	0201B4D8 E1A001C1 mov     r0,r1,asr 3h
	0201B4DC E0818E20 add     r8,r1,r0,lsr 1Ch
	0201B4E0 EA000009 b       201B50Ch
	0201B4E4 E5960000 ldr     r0,[r6]
	0201B4E8 E0851004 add     r1,r5,r4
	0201B4EC E0800304 add     r0,r0,r4,lsl 6h
	0201B4F0 E58D0000 str     r0,[r13]
	0201B4F4 E1D620B8 ldrh    r2,[r6,8h]
	0201B4F8 E5970008 ldr     r0,[r7,8h]
	0201B4FC E1A03009 mov     r3,r9
	0201B500 E20110FF and     r1,r1,0FFh
	0201B504 EBFFFE44 bl      201AE1Ch
	0201B508 E2844001 add     r4,r4,1h
	0201B50C E1540248 cmp     r4,r8,asr 4h
	0201B510 BAFFFFF3 blt     201B4E4h
	0201B514 EA000006 b       201B534h
	0201B518 E5960000 ldr     r0,[r6]
	0201B51C E20510FF and     r1,r5,0FFh
	0201B520 E58D0000 str     r0,[r13]
	0201B524 E1D620B8 ldrh    r2,[r6,8h]
	0201B528 E1D630B6 ldrh    r3,[r6,6h]
	0201B52C E5970008 ldr     r0,[r7,8h]
	0201B530 EBFFFE39 bl      201AE1Ch
	0201B534 E3A00001 mov     r0,1h
	0201B538 E8BD83F8 pop     r3-r9,r15
}
02058A94 E28D000C add     r0,r13,0Ch
02058A98 EBFEC04D bl      2008BD4h				//02008BD4 fun_2008BD4h
{
	02008BD4 E92D4010 push    r4,r14
	02008BD8 E1A04000 mov     r4,r0
	02008BDC E5940000 ldr     r0,[r4]
	02008BE0 EBFFE168 bl      2001188h			//02001188 fun_2001188h
	{
		02001188 E59FC008 ldr     r12,=2001638h	//Function 02001638
		0200118C E1A01000 mov     r1,r0
		02001190 E3A00000 mov     r0,0h
		02001194 E12FFF1C bx      r12			//Branch and execute 02001638
		{
			02001638 E92D41F0 push    r4-r8,r14
			0200163C E1A07000 mov     r7,r0
			02001640 E59F0160 ldr     r0,=20AEF08h		//020AEF08
			02001644 E1A06001 mov     r6,r1
			02001648 EB000599 bl      2002CB4h
			{
				02002CB4 E92D4070 push    r4-r6,r14
				02002CB8 E59F1050 ldr     r1,=22B966Ch		//Struct_022B966C
				02002CBC E1A06000 mov     r6,r0				//R6 = 020AEF08
				02002CC0 E5915004 ldr     r5,[r1,4h]		//R5 = [Struct_022B966C + 4] //EX: 022B967C
				02002CC4 E1A00005 mov     r0,r5				//R0 = R5 //EX: 022B967C
				02002CC8 EB01DB8F bl      2079B0Ch			//02079B0C
				{
					02079B0C E5900070 ldr     r0,[r0,70h]	//R0 = [R0 + 0x70] //EX: 0x2
					02079B10 E12FFF1E bx      r14
				}
				02002CCC E5961020 ldr     r1,[r6,20h]		//R1 = [020AEF08 + 0x20] //EX: 0x7
				02002CD0 E1A04000 mov     r4,r0
				02002CD4 E3510000 cmp     r1,0h
				02002CD8 0A000005 beq     2002CF4h			///02002CF4
				02002CDC E1510004 cmp     r1,r4
				02002CE0 23A04000 movcs   r4,0h
				02002CE4 2A000003 bcs     2002CF8h
				02002CE8 E1A00005 mov     r0,r5
				02002CEC EB01DB5C bl      2079A64h
				02002CF0 EA000000 b       2002CF8h
				///02002CF4
				02002CF4 E3A04000 mov     r4,0h
				02002CF8 E1A00006 mov     r0,r6
				02002CFC EBFFFF00 bl      2002904h
				02002D00 E586401C str     r4,[r6,1Ch]
				02002D04 EBFFFE1D bl      2002580h
				02002D08 E5860018 str     r0,[r6,18h]
				02002D0C E8BD8070 pop     r4-r6,r15
			}
			0200164C E59F1158 ldr     r1,=20AEF00h
			02001650 E1A00007 mov     r0,r7
			02001654 E5912004 ldr     r2,[r1,4h]
			02001658 E1A01006 mov     r1,r6
			0200165C E12FFF32 blx     r2
			02001660 E1B07000 movs    r7,r0
			02001664 059F7144 ldreq   r7,=20B3384h
			02001668 E3560000 cmp     r6,0h
			0200166C 1A000002 bne     200167Ch
			02001670 E59F0130 ldr     r0,=20AEF08h
			02001674 EB000607 bl      2002E98h
			02001678 E8BD81F0 pop     r4-r8,r15
			0200167C E5974008 ldr     r4,[r7,8h]
			02001680 E597100C ldr     r1,[r7,0Ch]
			02001684 E3A05000 mov     r5,0h
			02001688 EA000041 b       2001794h
			0200168C E594000C ldr     r0,[r4,0Ch]
			02001690 E1500006 cmp     r0,r6
			02001694 1A00003C bne     200178Ch
			02001698 E3A00000 mov     r0,0h
			0200169C E5840000 str     r0,[r4]
			020016A0 E5840004 str     r0,[r4,4h]
			020016A4 E5840014 str     r0,[r4,14h]
			020016A8 E5840008 str     r0,[r4,8h]
			020016AC E597000C ldr     r0,[r7,0Ch]
			020016B0 E2400001 sub     r0,r0,1h
			020016B4 E1550000 cmp     r5,r0
			020016B8 AA000017 bge     200171Ch
			020016BC E284C018 add     r12,r4,18h
			020016C0 E59C0004 ldr     r0,[r12,4h]
			020016C4 E3500000 cmp     r0,0h
			020016C8 1A000013 bne     200171Ch
			020016CC E5941010 ldr     r1,[r4,10h]
			020016D0 E59C0010 ldr     r0,[r12,10h]
			020016D4 E285E001 add     r14,r5,1h
			020016D8 E0810000 add     r0,r1,r0
			020016DC E5840010 str     r0,[r4,10h]
			020016E0 E597000C ldr     r0,[r7,0Ch]
			020016E4 E2400001 sub     r0,r0,1h
			020016E8 E587000C str     r0,[r7,0Ch]
			020016EC EA000007 b       2001710h
			020016F0 E28C8018 add     r8,r12,18h
			020016F4 E1A0600C mov     r6,r12
			020016F8 E8B8000F ldmia   [r8]!,r0-r3
			020016FC E8A6000F stmia   [r6]!,r0-r3
			02001700 E8980003 ldmia   [r8],r0,r1
			02001704 E8860003 stmia   [r6],r0,r1
			02001708 E28EE001 add     r14,r14,1h
			0200170C E28CC018 add     r12,r12,18h
			02001710 E597000C ldr     r0,[r7,0Ch]
			02001714 E15E0000 cmp     r14,r0
			02001718 BAFFFFF4 blt     20016F0h
			0200171C E3550000 cmp     r5,0h
			02001720 DA000016 ble     2001780h
			02001724 E2442018 sub     r2,r4,18h
			02001728 E5920004 ldr     r0,[r2,4h]
			0200172C E3500000 cmp     r0,0h
			02001730 1A000012 bne     2001780h
			02001734 E5921010 ldr     r1,[r2,10h]
			02001738 E5940010 ldr     r0,[r4,10h]
			0200173C E0810000 add     r0,r1,r0
			02001740 E5820010 str     r0,[r2,10h]
			02001744 E597000C ldr     r0,[r7,0Ch]
			02001748 E2400001 sub     r0,r0,1h
			0200174C E587000C str     r0,[r7,0Ch]
			02001750 EA000007 b       2001774h
			02001754 E284C018 add     r12,r4,18h
			02001758 E1A06004 mov     r6,r4
			0200175C E8BC000F ldmia   [r12]!,r0-r3
			02001760 E8A6000F stmia   [r6]!,r0-r3
			02001764 E89C0003 ldmia   [r12],r0,r1
			02001768 E8860003 stmia   [r6],r0,r1
			0200176C E2855001 add     r5,r5,1h
			02001770 E2844018 add     r4,r4,18h
			02001774 E597000C ldr     r0,[r7,0Ch]
			02001778 E1550000 cmp     r5,r0
			0200177C BAFFFFF4 blt     2001754h
			02001780 E59F0020 ldr     r0,=20AEF08h
			02001784 EB0005C3 bl      2002E98h
			02001788 E8BD81F0 pop     r4-r8,r15
			0200178C E2855001 add     r5,r5,1h
			02001790 E2844018 add     r4,r4,18h
			02001794 E1550001 cmp     r5,r1
			02001798 BAFFFFBB blt     200168Ch
			0200179C E59F0004 ldr     r0,=20AEF08h
			020017A0 EB0005BC bl      2002E98h
			020017A4 E8BD81F0 pop     r4-r8,r15
			020017A8 020AEF08 andeq   r14,r10,20h
			020017AC 020AEF00 andeq   r14,r10,0h
			020017B0 020B3384 andeq   r3,r11,10000002h
			020017B4 E5902008 ldr     r2,[r0,8h]
			020017B8 E590C00C ldr     r12,[r0,0Ch]
			020017BC E3A03000 mov     r3,0h
			020017C0 EA000005 b       20017DCh
			020017C4 E592000C ldr     r0,[r2,0Ch]
			020017C8 E1500001 cmp     r0,r1
			020017CC 03A00001 moveq   r0,1h
			020017D0 012FFF1E bxeq    r14
			020017D4 E2833001 add     r3,r3,1h
			020017D8 E2822018 add     r2,r2,18h
			020017DC E153000C cmp     r3,r12
			020017E0 BAFFFFF7 blt     20017C4h
			020017E4 E3A00000 mov     r0,0h
			020017E8 E12FFF1E bx      r14
			020017EC E92D4008 push    r3,r14
			020017F0 E59F000C ldr     r0,=20AEF08h
			020017F4 EB00052E bl      2002CB4h
			020017F8 E59F0004 ldr     r0,=20AEF08h
			020017FC EB0005A5 bl      2002E98h
			02001800 E8BD8008 pop     r3,r15
		}
	}
	02008BE4 E3A00000 mov     r0,0h
	02008BE8 E5840000 str     r0,[r4]
	02008BEC E5840004 str     r0,[r4,4h]
	02008BF0 E8BD8010 pop     r4,r15
}
02058A9C E28DD030 add     r13,r13,30h
02058AA0 E8BD87F0 pop     r4-r10,r15
02058AA4 000003E3 andeq   r0,r0,r3,ror 7h
02058AA8 020A3468 andeq   r3,r10,68000000h
02058AAC 022B578C eoreq   r5,r11,2300000h
02058AB0 000003FF ????
02058AB4 000003E5 andeq   r0,r0,r5,ror 7h
02058AB8 020AFC4C andeq   r15,r10,4C00h


///
/// 0205379C  205379Ch
///
//r0 = ptr 
//r1 = 16bits word 
//r2 = counter maybe?
0205379C E92D4078 push    r3-r6,r14
020537A0 E24DD00C sub     r13,r13,0Ch
020537A4 E1A06001 mov     r6,r1
020537A8 E1A05000 mov     r5,r0
020537AC E1A00006 mov     r0,r6
020537B0 E3A01F96 mov     r1,258h           //r1 = 600 //Possibly nb pokemons? Since the total nb of entities is 1200
020537B4 E1A04002 mov     r4,r2             
020537B8 EB00F1B9 bl      208FEA4h          
{
    0208FEA4 E020C001 eor     r12,r0,r1             //r12 = r0 ^ r1 (XOR)
    0208FEA8 E20CC102 and     r12,r12,80000000h     //r12 = r12 & 0x80000000 //Isolate the sign bit into r12
    0208FEAC E3500000 cmp     r0,0h
    if( r0 < 0 )                                    //lt == signed <
        0208FEB0 B2600000 rsblt   r0,r0,0h          //r0 = 0 - r0
        0208FEB4 B28CC001 addlt   r12,r12,1h        //r12 = r12 + 1
    0208FEB8 E3510000 cmp     r1,0h
    if( r1 < 0 )
        0208FEBC B2611000 rsblt   r1,r1,0h          //r1 = 0 - r1
    else if( r1 == 0 )
        0208FEC0 0A000075 beq     209009Ch          ///0209009C v
    0208FEC4 E1500001 cmp     r0,r1                 
    if( r0 < r1 )                                   //unsigned <
        0208FEC8 31A01000 movcc   r1,r0             //r1 = r0
        0208FECC 33A00000 movcc   r0,0h             //r0 = 0
        0208FED0 3A000071 bcc     209009Ch          ///0209009C v
    0208FED4 E3A0201C mov     r2,1Ch                //r2 = 28 (0x1C) 
    0208FED8 E1A03220 mov     r3,r0,lsr 4h          //r3 = r0 / 16 
    0208FEDC E1510623 cmp     r1,r3,lsr 0Ch         //r3 >> 12 == r3 / 4096 (0x1000)
    if( r1 <= (r3 / 4096) )
        0208FEE0 D2422010 suble   r2,r2,10h         //r2 = r2 - 16    
        0208FEE4 D1A03823 movle   r3,r3,lsr 10h     //r3 = r3 >> 16
    0208FEE8 E1510223 cmp     r1,r3,lsr 4h          //r3 >> 4 == r3 / 16
    if( r1 <= (r3 / 16)  )
        0208FEEC D2422008 suble   r2,r2,8h          //r2 = r2 - 8
        0208FEF0 D1A03423 movle   r3,r3,lsr 8h      //r3 = r3 / 256
    0208FEF4 E1510003 cmp     r1,r3
    if( r1 <= r3 )
        0208FEF8 D2422004 suble   r2,r2,4h          //r2 = r2 - 4
        0208FEFC D1A03223 movle   r3,r3,lsr 4h      //r3 = r3 / 16
    0208FF00 E1A00210 mov     r0,r0,lsl r2          //r0 = r0 << r2
    0208FF04 E2611000 rsb     r1,r1,0h              //r1 = 0 - r1
    0208FF08 E0900000 adds    r0,r0,r0              //r0 = r0 + r0 //(r0 exponent 2)
    0208FF0C E0822082 add     r2,r2,r2,lsl 1h       //r2 = r2 + (r2 * 2)
    0208FF10 E08FF102 add     r15,r15,r2,lsl 2h     //r15 = r15 + (r2 * 4) //r15 = 0208FF14 + (r2 * 4) + 4
    
    0208FF14 E1A00000 nop                           //NOP
    0208FF18 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF1C 30433001 subcc   r3,r3,r1
    0208FF20 E0B00000 adcs    r0,r0,r0
    0208FF24 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF28 30433001 subcc   r3,r3,r1
    0208FF2C E0B00000 adcs    r0,r0,r0
    0208FF30 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF34 30433001 subcc   r3,r3,r1
    0208FF38 E0B00000 adcs    r0,r0,r0
    0208FF3C E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF40 30433001 subcc   r3,r3,r1
    0208FF44 E0B00000 adcs    r0,r0,r0
    0208FF48 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF4C 30433001 subcc   r3,r3,r1
    0208FF50 E0B00000 adcs    r0,r0,r0
    0208FF54 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF58 30433001 subcc   r3,r3,r1
    0208FF5C E0B00000 adcs    r0,r0,r0
    0208FF60 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF64 30433001 subcc   r3,r3,r1
    0208FF68 E0B00000 adcs    r0,r0,r0
    0208FF6C E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF70 30433001 subcc   r3,r3,r1
    0208FF74 E0B00000 adcs    r0,r0,r0
    0208FF78 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF7C 30433001 subcc   r3,r3,r1
    0208FF80 E0B00000 adcs    r0,r0,r0
    0208FF84 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF88 30433001 subcc   r3,r3,r1
    0208FF8C E0B00000 adcs    r0,r0,r0
    0208FF90 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FF94 30433001 subcc   r3,r3,r1
    0208FF98 E0B00000 adcs    r0,r0,r0
    0208FF9C E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FFA0 30433001 subcc   r3,r3,r1
    0208FFA4 E0B00000 adcs    r0,r0,r0
    0208FFA8 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FFAC 30433001 subcc   r3,r3,r1
    0208FFB0 E0B00000 adcs    r0,r0,r0
    0208FFB4 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FFB8 30433001 subcc   r3,r3,r1
    0208FFBC E0B00000 adcs    r0,r0,r0
    0208FFC0 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FFC4 30433001 subcc   r3,r3,r1
    0208FFC8 E0B00000 adcs    r0,r0,r0
    0208FFCC E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FFD0 30433001 subcc   r3,r3,r1
    0208FFD4 E0B00000 adcs    r0,r0,r0
    0208FFD8 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FFDC 30433001 subcc   r3,r3,r1
    0208FFE0 E0B00000 adcs    r0,r0,r0
    0208FFE4 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FFE8 30433001 subcc   r3,r3,r1
    0208FFEC E0B00000 adcs    r0,r0,r0
    0208FFF0 E0B13083 adcs    r3,r1,r3,lsl 1h
    0208FFF4 30433001 subcc   r3,r3,r1
    0208FFF8 E0B00000 adcs    r0,r0,r0
    0208FFFC E0B13083 adcs    r3,r1,r3,lsl 1h
    02090000 30433001 subcc   r3,r3,r1
    02090004 E0B00000 adcs    r0,r0,r0
    02090008 E0B13083 adcs    r3,r1,r3,lsl 1h
    0209000C 30433001 subcc   r3,r3,r1
    02090010 E0B00000 adcs    r0,r0,r0
    02090014 E0B13083 adcs    r3,r1,r3,lsl 1h
    02090018 30433001 subcc   r3,r3,r1
    0209001C E0B00000 adcs    r0,r0,r0
    02090020 E0B13083 adcs    r3,r1,r3,lsl 1h
    02090024 30433001 subcc   r3,r3,r1
    02090028 E0B00000 adcs    r0,r0,r0
    0209002C E0B13083 adcs    r3,r1,r3,lsl 1h
    02090030 30433001 subcc   r3,r3,r1
    02090034 E0B00000 adcs    r0,r0,r0
    02090038 E0B13083 adcs    r3,r1,r3,lsl 1h
    0209003C 30433001 subcc   r3,r3,r1
    02090040 E0B00000 adcs    r0,r0,r0
    02090044 E0B13083 adcs    r3,r1,r3,lsl 1h
    02090048 30433001 subcc   r3,r3,r1
    0209004C E0B00000 adcs    r0,r0,r0
    02090050 E0B13083 adcs    r3,r1,r3,lsl 1h
    02090054 30433001 subcc   r3,r3,r1
    02090058 E0B00000 adcs    r0,r0,r0
    0209005C E0B13083 adcs    r3,r1,r3,lsl 1h
    02090060 30433001 subcc   r3,r3,r1
    02090064 E0B00000 adcs    r0,r0,r0
    02090068 E0B13083 adcs    r3,r1,r3,lsl 1h
    0209006C 30433001 subcc   r3,r3,r1
    02090070 E0B00000 adcs    r0,r0,r0
    02090074 E0B13083 adcs    r3,r1,r3,lsl 1h
    02090078 30433001 subcc   r3,r3,r1
    0209007C E0B00000 adcs    r0,r0,r0
    02090080 E0B13083 adcs    r3,r1,r3,lsl 1h
    02090084 30433001 subcc   r3,r3,r1
    02090088 E0B00000 adcs    r0,r0,r0
    0209008C E0B13083 adcs    r3,r1,r3,lsl 1h
    02090090 30433001 subcc   r3,r3,r1
    02090094 E0B00000 adcs    r0,r0,r0
    02090098 E1A01003 mov     r1,r3
    ///0209009C
    0209009C E21C3102 ands    r3,r12,80000000h          
    020900A0 12600000 rsbne   r0,r0,0h
    020900A4 E21C3001 ands    r3,r12,1h
    020900A8 12611000 rsbne   r1,r1,0h
    020900AC E12FFF1E bx      r14
}
020537BC E59F0080 ldr     r0,=22AB92Ch
020537C0 E1D000F0 ldrsh   r0,[r0]
020537C4 E1500001 cmp     r0,r1
020537C8 0A000014 beq     2053820h
020537CC E1A00006 mov     r0,r6
020537D0 E3A01F96 mov     r1,258h           //r1 = 600
020537D4 EB00F1B2 bl      208FEA4h
020537D8 E2410001 sub     r0,r1,1h
020537DC E59F2060 ldr     r2,=22AB92Ch
020537E0 E1A00800 mov     r0,r0,lsl 10h
020537E4 E1C210B0 strh    r1,[r2]
020537E8 E1A01820 mov     r1,r0,lsr 10h
020537EC E28D2004 add     r2,r13,4h
020537F0 E3A00005 mov     r0,5h                 //5 aka "BALANCE/m_level.bin"
020537F4 E3A03000 mov     r3,0h                 //
020537F8 EBFEE2E2 bl      200C388h              ///fun_200C388h(r0 = packfileid, r1 = subfileindex, r2 = targetpair (ptrfbuffer,flen), r3 = nbelem??)
020537FC E59D1004 ldr     r1,[r13,4h]
02053800 E28D0000 add     r0,r13,0h
02053804 EBFF2F2A bl      201F4B4h
02053808 E59D2000 ldr     r2,[r13]
0205380C E59F0034 ldr     r0,=22AB930h
02053810 E3A01000 mov     r1,0h
02053814 EBFF2F69 bl      201F5C0h
02053818 E28D0004 add     r0,r13,4h
0205381C EBFED4EC bl      2008BD4h
02053820 E2544001 subs    r4,r4,1h
02053824 E59F101C ldr     r1,=22AB930h
02053828 43A04000 movmi   r4,0h
0205382C E3A0000C mov     r0,0Ch
02053830 E0201094 mla     r0,r4,r0,r1
02053834 E8900007 ldmia   [r0],r0-r2
02053838 E8850007 stmia   [r5],r0-r2
0205383C E28DD00C add     r13,r13,0Ch
02053840 E8BD8078 pop     r3-r6,r15
02053844 022AB92C eoreq   r11,r10,0B0000h
02053848 022AB930 eoreq   r11,r10,0C0000h



///
/// 0201D3DC
///
0201D3DC E92D40F8 push    r3-r7,r14
0201D3E0 E24DD008 sub     r13,r13,8h
0201D3E4 E1A05000 mov     r5,r0
0201D3E8 E1A06001 mov     r6,r1
0201D3EC E1A07002 mov     r7,r2
0201D3F0 EBFFFFB7 bl      201D2D4h
0201D3F4 E3E01000 mvn     r1,0h
0201D3F8 E1500001 cmp     r0,r1
if( r0 == 0xFFFFFFFF )
    0201D3FC 0A000005 beq     201D418h              ///0201D418 v
0201D400 E3A01038 mov     r1,38h
0201D404 E1025180 smlabb  r2,r0,r1,r5
0201D408 E1D212FC ldrsh   r1,[r2,2Ch]
0201D40C E2811001 add     r1,r1,1h
0201D410 E1C212BC strh    r1,[r2,2Ch]
0201D414 EA000018 b       201D47Ch                  ///0201D47C v
///0201D418
0201D418 E1A00005 mov     r0,r5
0201D41C EBFFFF88 bl      201D244h
0201D420 E1A04000 mov     r4,r0
0201D424 E3A00038 mov     r0,38h
0201D428 E1055084 smlabb  r5,r4,r0,r5
0201D42C E3A03001 mov     r3,1h
0201D430 E28D0000 add     r0,r13,0h
0201D434 E1A01006 mov     r1,r6
0201D438 E1A02007 mov     r2,r7
0201D43C E5C53021 strb    r3,[r5,21h]
0201D440 EBFFADFD bl      2008C3Ch                //LoadFileFromRom
0201D444 E59D1000 ldr     r1,[r13]
0201D448 E1A00005 mov     r0,r5
0201D44C E5851034 str     r1,[r5,34h]
0201D450 E59D2004 ldr     r2,[r13,4h]
0201D454 E1A01006 mov     r1,r6
0201D458 E5852028 str     r2,[r5,28h]
0201D45C EB01B08C bl      2089694h
0201D460 E1D512FC ldrsh   r1,[r5,2Ch]
0201D464 E2850030 add     r0,r5,30h
0201D468 E2811001 add     r1,r1,1h
0201D46C E1C512BC strh    r1,[r5,2Ch]
0201D470 E5951034 ldr     r1,[r5,34h]
0201D474 EB000844 bl      201F58Ch             //HandleSir0Translation(R0=OutPtrToSIR0Data, R1=filebuffer)
0201D478 E1A00004 mov     r0,r4
///0201D47C
0201D47C E28DD008 add     r13,r13,8h
0201D480 E8BD80F8 pop     r3-r7,r15

///
/// 0201D484  201D484h
///
//r0 =
//r1 = packfileid
//r2 =
//r3 = 
0201D484 E92D43F8 push    r3-r9,r14
0201D488 E1A09000 mov     r9,r0
0201D48C E1A08001 mov     r8,r1
0201D490 E1A07002 mov     r7,r2
0201D494 E1A06003 mov     r6,r3
0201D498 EBFFFFA5 bl      201D334h
{
    0201D334 E92D4038 push    r3-r5,r14
    0201D338 E2803C15 add     r3,r0,1500h
    0201D33C E1D350F8 ldrsh   r5,[r3,8h]
    0201D340 E3A04001 mov     r4,1h
    0201D344 E3A0C038 mov     r12,38h
    0201D348 EA00000B b       201D37Ch
    0201D34C E10E0C84 smlabb  r14,r4,r12,r0
    0201D350 E5DE3021 ldrb    r3,[r14,21h]
    0201D354 E3530002 cmp     r3,2h
    0201D358 01DE32B2 ldreqh  r3,[r14,22h]
    0201D35C 01510003 cmpeq   r1,r3
    0201D360 01DE32B4 ldreqh  r3,[r14,24h]
    0201D364 01520003 cmpeq   r2,r3
    0201D368 01A00004 moveq   r0,r4
    0201D36C 08BD8038 popeq   r3-r5,r15
    0201D370 E2843001 add     r3,r4,1h
    0201D374 E1A03803 mov     r3,r3,lsl 10h
    0201D378 E1A04843 mov     r4,r3,asr 10h
    0201D37C E1540005 cmp     r4,r5
    0201D380 BAFFFFF1 blt     201D34Ch
    0201D384 E3E00000 mvn     r0,0h
    0201D388 E8BD8038 pop     r3-r5,r15
}
0201D49C E3E01000 mvn     r1,0h
0201D4A0 E1500001 cmp     r0,r1
if( r0 == 0xFFFFFFFF )
    0201D4A4 0A000005 beq     201D4C0h              ///0201D4C0 v
0201D4A8 E3A01038 mov     r1,38h
0201D4AC E1029180 smlabb  r2,r0,r1,r9
0201D4B0 E1D212FC ldrsh   r1,[r2,2Ch]
0201D4B4 E2811001 add     r1,r1,1h
0201D4B8 E1C212BC strh    r1,[r2,2Ch]
0201D4BC E8BD83F8 pop     r3-r9,r15
///0201D4C0
0201D4C0 E1A00009 mov     r0,r9
0201D4C4 EBFFFF5E bl      201D244h
0201D4C8 E1A05000 mov     r5,r0
0201D4CC E3A00038 mov     r0,38h
0201D4D0 E1049085 smlabb  r4,r5,r0,r9
0201D4D4 E5DD0020 ldrb    r0,[r13,20h]
0201D4D8 E3A01002 mov     r1,2h
0201D4DC E5C41021 strb    r1,[r4,21h]
0201D4E0 E1C482B2 strh    r8,[r4,22h]
0201D4E4 E1C472B4 strh    r7,[r4,24h]
0201D4E8 E3500000 cmp     r0,0h
if( r0 == 0 )
    0201D4EC 0A000012 beq     201D53Ch              ///0201D53C v
0201D4F0 E2890A01 add     r0,r9,1000h
0201D4F4 E5909500 ldr     r9,[r0,500h]
0201D4F8 E1A00008 mov     r0,r8
0201D4FC E1A01007 mov     r1,r7
0201D500 EBFFBB8D bl      200C33Ch                /// 0200C33C LookupPackFileSubFiles 200C33Ch (r0 = fileid (0 to 5), r1 = subfileindex )
0201D504 E1A00008 mov     r0,r8
0201D508 E1A01007 mov     r1,r7
0201D50C E1A02009 mov     r2,r9
0201D510 EBFFBB91 bl      200C35Ch
0201D514 E1A00009 mov     r0,r9
0201D518 EB000BF3 bl      20204ECh
0201D51C E1A07000 mov     r7,r0
0201D520 E1A01006 mov     r1,r6
0201D524 EBFF8F11 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
0201D528 E1A01007 mov     r1,r7
0201D52C E1A02009 mov     r2,r9
0201D530 E1A06000 mov     r6,r0
0201D534 EB000821 bl      201F5C0h
0201D538 EA00000B b       201D56Ch              ///0201D56C v
///0201D53C
0201D53C E1A00008 mov     r0,r8
0201D540 E1A01007 mov     r1,r7
0201D544 EBFFBB7C bl      200C33Ch                /// 0200C33C LookupPackFileSubFiles 200C33Ch (r0 = fileid (0 to 5), r1 = subfileindex )
0201D548 E1A01006 mov     r1,r6
0201D54C E1A09000 mov     r9,r0
0201D550 EBFF8F06 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
0201D554 E1A06000 mov     r6,r0
0201D558 E1A00008 mov     r0,r8
0201D55C E1A01007 mov     r1,r7
0201D560 E1A02006 mov     r2,r6
0201D564 EBFFBB7C bl      200C35Ch              ///200C35Ch (r0 = packfileid, r1 = subfileid, r2 = SubFileOffset)
{
    0200C35C E92D4008 push    r3,r14
    0200C360 E59F301C ldr     r3,=20AF69Ch      //r3 = 20AF69Ch
    0200C364 E1A0C001 mov     r12,r1            //r12 = r1
    0200C368 E5933000 ldr     r3,[r3]           //r3 = [20AF69Ch]
    0200C36C E3A01054 mov     r1,54h            //r1 = 0x54
    0200C370 E0203091 mla     r0,r1,r0,r3       //r0 = (0x54 * r0) + [20AF69Ch]
    0200C374 E1A01002 mov     r1,r2             //r1 = r2
    0200C378 E1A0200C mov     r2,r12            //r2 = r12
    0200C37C EB000040 bl      200C484h          
    {
        0200C484 E92D4070 push    r4-r6,r14
        0200C488 E1A06000 mov     r6,r0
        0200C48C E1A05001 mov     r5,r1
        0200C490 E1A04002 mov     r4,r2
        0200C494 EBFFEF33 bl      2008168h
        0200C498 E5962000 ldr     r2,[r6]
        0200C49C E286000C add     r0,r6,0Ch
        0200C4A0 E7921184 ldr     r1,[r2,r4,lsl 3h]
        0200C4A4 E0824184 add     r4,r2,r4,lsl 3h
        0200C4A8 E3A02000 mov     r2,0h
        0200C4AC EBFFEF7D bl      20082A8h
        0200C4B0 E5942004 ldr     r2,[r4,4h]
        0200C4B4 E1A01005 mov     r1,r5
        0200C4B8 E286000C add     r0,r6,0Ch
        0200C4BC EBFFEF64 bl      2008254h
        0200C4C0 EBFFEF33 bl      2008194h
        0200C4C4 E5940004 ldr     r0,[r4,4h]
        0200C4C8 E8BD8070 pop     r4-r6,r15
    }
    0200C380 E8BD8008 pop     r3,r15
    0200C384 020AF69C 

}
0201D568 E5849028 str     r9,[r4,28h]
///0201D56C
0201D56C E5846034 str     r6,[r4,34h]
0201D570 E1D412FC ldrsh   r1,[r4,2Ch]
0201D574 E2840030 add     r0,r4,30h
0201D578 E2811001 add     r1,r1,1h
0201D57C E1C412BC strh    r1,[r4,2Ch]
0201D580 E5941034 ldr     r1,[r4,34h]
0201D584 EB000800 bl      201F58Ch             //HandleSir0Translation(R0=OutPtrToSIR0Data, R1=filebuffer)
0201D588 E1A00005 mov     r0,r5
0201D58C E8BD83F8 pop     r3-r9,r15


///
/// 201D590h Fun_201D590
///
//r1 = packfileid
0201D590 E92D43F8 push    r3-r9,r14
0201D594 E1A09000 mov     r9,r0
0201D598 E1A08001 mov     r8,r1
0201D59C E1A07002 mov     r7,r2
0201D5A0 E1A06003 mov     r6,r3
0201D5A4 EBFFFF62 bl      201D334h
0201D5A8 E3E01000 mvn     r1,0h
0201D5AC E1500001 cmp     r0,r1
0201D5B0 0A000005 beq     201D5CCh
0201D5B4 E3A01038 mov     r1,38h
0201D5B8 E1029180 smlabb  r2,r0,r1,r9
0201D5BC E1D212FC ldrsh   r1,[r2,2Ch]
0201D5C0 E2811001 add     r1,r1,1h
0201D5C4 E1C212BC strh    r1,[r2,2Ch]
0201D5C8 E8BD83F8 pop     r3-r9,r15
0201D5CC E1A00009 mov     r0,r9
0201D5D0 EBFFFF1B bl      201D244h
0201D5D4 E5DD1020 ldrb    r1,[r13,20h]
0201D5D8 E1A05000 mov     r5,r0
0201D5DC E3A00038 mov     r0,38h
0201D5E0 E1049085 smlabb  r4,r5,r0,r9
0201D5E4 E3510000 cmp     r1,0h
0201D5E8 0A00000F beq     201D62Ch
0201D5EC E2890A01 add     r0,r9,1000h
0201D5F0 E5909500 ldr     r9,[r0,500h]
0201D5F4 E1A00008 mov     r0,r8
0201D5F8 E1A01007 mov     r1,r7
0201D5FC EBFFBB4E bl      200C33Ch                /// 0200C33C LookupPackFileSubFiles 200C33Ch (r0 = fileid (0 to 5), r1 = subfileindex )
0201D600 E1A00008 mov     r0,r8
0201D604 E1A01007 mov     r1,r7
0201D608 E1A02009 mov     r2,r9
0201D60C EBFFBB52 bl      200C35Ch
0201D610 E1A00009 mov     r0,r9
0201D614 EB000BB4 bl      20204ECh
0201D618 E1A02009 mov     r2,r9
0201D61C E1A01000 mov     r1,r0
0201D620 E1A00006 mov     r0,r6
0201D624 EB0007E5 bl      201F5C0h
0201D628 EA000006 b       201D648h
0201D62C E1A00008 mov     r0,r8
0201D630 E1A01007 mov     r1,r7
0201D634 EBFFBB40 bl      200C33Ch                /// 0200C33C LookupPackFileSubFiles 200C33Ch (r0 = fileid (0 to 5), r1 = subfileindex )
0201D638 E1A00008 mov     r0,r8
0201D63C E1A01007 mov     r1,r7
0201D640 E1A02006 mov     r2,r6
0201D644 EBFFBB44 bl      200C35Ch
0201D648 E3A00002 mov     r0,2h
0201D64C E5C40021 strb    r0,[r4,21h]
0201D650 E1C482B2 strh    r8,[r4,22h]
0201D654 E1C472B4 strh    r7,[r4,24h]
0201D658 E5846034 str     r6,[r4,34h]
0201D65C E1D412FC ldrsh   r1,[r4,2Ch]
0201D660 E2840030 add     r0,r4,30h
0201D664 E2811001 add     r1,r1,1h
0201D668 E1C412BC strh    r1,[r4,2Ch]
0201D66C E5941034 ldr     r1,[r4,34h]
0201D670 EB0007C5 bl      201F58Ch             //HandleSir0Translation(R0=OutPtrToSIR0Data, R1=filebuffer)
0201D674 E3A01001 mov     r1,1h
0201D678 E1A00005 mov     r0,r5
0201D67C E5C41020 strb    r1,[r4,20h]
0201D680 E8BD83F8 pop     r3-r9,r15


///
///
///
//Called once on line 022F761C with (r0=, r1=, r2=0 ("MONSTER/monster.bin") )
//r2 = packfileid
0201D684 E92D41F0 push    r4-r8,r14
0201D688 E5DDC018 ldrb    r12,[r13,18h]
0201D68C E3A04038 mov     r4,38h
0201D690 E1060481 smlabb  r6,r1,r4,r0
0201D694 E1A05002 mov     r5,r2
0201D698 E1A04003 mov     r4,r3
0201D69C E35C0000 cmp     r12,0h
0201D6A0 0A000010 beq     201D6E8h
0201D6A4 E2800A01 add     r0,r0,1000h
0201D6A8 E5907500 ldr     r7,[r0,500h]
0201D6AC E1A00005 mov     r0,r5
0201D6B0 E1A01004 mov     r1,r4
0201D6B4 E5968034 ldr     r8,[r6,34h]
0201D6B8 EBFFBB1F bl      200C33Ch                /// 0200C33C LookupPackFileSubFiles 200C33Ch (r0 = fileid (0 to 5), r1 = subfileindex )
0201D6BC E1A00005 mov     r0,r5
0201D6C0 E1A01004 mov     r1,r4
0201D6C4 E1A02007 mov     r2,r7
0201D6C8 EBFFBB23 bl      200C35Ch
0201D6CC E1A00007 mov     r0,r7
0201D6D0 EB000B85 bl      20204ECh
0201D6D4 E1A02007 mov     r2,r7
0201D6D8 E1A01000 mov     r1,r0
0201D6DC E1A00008 mov     r0,r8
0201D6E0 EB0007B6 bl      201F5C0h
0201D6E4 EA000007 b       201D708h
0201D6E8 E5967034 ldr     r7,[r6,34h]
0201D6EC E1A00005 mov     r0,r5
0201D6F0 E1A01004 mov     r1,r4
0201D6F4 EBFFBB10 bl      200C33Ch                /// 0200C33C LookupPackFileSubFiles 200C33Ch (r0 = fileid (0 to 5), r1 = subfileindex )
0201D6F8 E1A00005 mov     r0,r5
0201D6FC E1A01004 mov     r1,r4
0201D700 E1A02007 mov     r2,r7
0201D704 EBFFBB14 bl      200C35Ch
0201D708 E5961034 ldr     r1,[r6,34h]
0201D70C E2860030 add     r0,r6,30h
0201D710 EB00079D bl      201F58Ch             //HandleSir0Translation(R0=OutPtrToSIR0Data, R1=filebuffer)
0201D714 E3A00002 mov     r0,2h
0201D718 E5C60021 strb    r0,[r6,21h]
0201D71C E1C652B2 strh    r5,[r6,22h]
0201D720 E1C642B4 strh    r4,[r6,24h]
0201D724 E3A00001 mov     r0,1h
0201D728 E8BD81F0 pop     r4-r8,r15


///
///
///
022BE780 E92D4FF0 push    r4-r11,r14
022BE784 E24DD05C sub     r13,r13,5Ch
022BE788 E59F3208 ldr     r3,=22DC1C0h
022BE78C E1A0A000 mov     r10,r0
022BE790 E5930000 ldr     r0,[r3]
022BE794 E1A09001 mov     r9,r1
022BE798 E2800A02 add     r0,r0,2000h
022BE79C E5900784 ldr     r0,[r0,784h]
022BE7A0 E1A08002 mov     r8,r2
022BE7A4 E3500000 cmp     r0,0h
022BE7A8 1A000074 bne     22BE980h
022BE7AC EBF50C15 bl      2001808h
022BE7B0 E1A04000 mov     r4,r0
022BE7B4 E5990000 ldr     r0,[r9]
022BE7B8 EB0005B8 bl      22BFEA0h
022BE7BC E59F11D4 ldr     r1,=22DC1C0h
022BE7C0 E1A05000 mov     r5,r0
022BE7C4 E5910000 ldr     r0,[r1]
022BE7C8 E5991000 ldr     r1,[r9]
022BE7CC E2800A02 add     r0,r0,2000h
022BE7D0 E5D007A0 ldrb    r0,[r0,7A0h]
022BE7D4 E3500000 cmp     r0,0h
022BE7D8 0A000008 beq     22BE800h
022BE7DC E35100EF cmp     r1,0EFh
022BE7E0 13510F53 cmpne   r1,14Ch
022BE7E4 159F01B0 ldrne   r0,=287h
022BE7E8 11510000 cmpne   r1,r0
022BE7EC 12400F4F subne   r0,r0,13Ch
022BE7F0 11510000 cmpne   r1,r0
022BE7F4 1A00000D bne     22BE830h
022BE7F8 E3E00000 mvn     r0,0h
022BE7FC EA000063 b       22BE990h
022BE800 E35100EF cmp     r1,0EFh
022BE804 13510F53 cmpne   r1,14Ch
022BE808 159F018C ldrne   r0,=287h
022BE80C 11510000 cmpne   r1,r0
022BE810 12400F4F subne   r0,r0,13Ch
022BE814 11510000 cmpne   r1,r0
022BE818 1A000004 bne     22BE830h
022BE81C E3A00B43 mov     r0,10C00h
022BE820 EB000591 bl      22BFE6Ch
022BE824 E3500000 cmp     r0,0h
022BE828 13E00000 mvnne   r0,0h
022BE82C 1A000057 bne     22BE990h
022BE830 E5950000 ldr     r0,[r5]
022BE834 E3500005 cmp     r0,5h
022BE838 1A000018 bne     22BE8A0h
022BE83C E5951004 ldr     r1,[r5,4h]
022BE840 E3A00003 mov     r0,3h                     //3 is "EFFECT/effect.bin"
022BE844 E1A01801 mov     r1,r1,lsl 10h
022BE848 E1A01841 mov     r1,r1,asr 10h
022BE84C E2811F43 add     r1,r1,10Ch
022BE850 E1A01801 mov     r1,r1,lsl 10h
022BE854 E1A01821 mov     r1,r1,lsr 10h
022BE858 EBF536B7 bl      200C33Ch                  /// 0200C33C LookupPackFileSubFiles 200C33Ch (r0 = fileid (0 to 5), r1 = subfileindex )
022BE85C E1540000 cmp     r4,r0
022BE860 AA000046 bge     22BE980h
022BE864 E28D5030 add     r5,r13,30h
022BE868 E3A04002 mov     r4,2h
022BE86C E8B9000F ldmia   [r9]!,r0-r3
022BE870 E8A5000F stmia   [r5]!,r0-r3
022BE874 E2544001 subs    r4,r4,1h
022BE878 1AFFFFFB bne     22BE86Ch
022BE87C E8990007 ldmia   [r9],r0-r2
022BE880 E8850007 stmia   [r5],r0-r2
022BE884 E3A03008 mov     r3,8h
022BE888 E28D1030 add     r1,r13,30h
022BE88C E1A0000A mov     r0,r10
022BE890 E1A02008 mov     r2,r8
022BE894 E58D3030 str     r3,[r13,30h]
022BE898 EBFFFFA4 bl      22BE730h
022BE89C EA00003B b       22BE990h
022BE8A0 E3500003 cmp     r0,3h
022BE8A4 1A000035 bne     22BE980h
022BE8A8 E59F00E8 ldr     r0,=22DC1C0h
022BE8AC E3A0B000 mov     r11,0h
022BE8B0 E5906000 ldr     r6,[r0]
022BE8B4 E3E00000 mvn     r0,0h
022BE8B8 E1A0700B mov     r7,r11
022BE8BC E58D0000 str     r0,[r13]
022BE8C0 EA000012 b       22BE910h
022BE8C4 E596100C ldr     r1,[r6,0Ch]
022BE8C8 E59D0000 ldr     r0,[r13]
022BE8CC E1510000 cmp     r1,r0
022BE8D0 0A00000C beq     22BE908h
022BE8D4 E5960008 ldr     r0,[r6,8h]
022BE8D8 E3500003 cmp     r0,3h
022BE8DC 1A000009 bne     22BE908h
022BE8E0 E5960014 ldr     r0,[r6,14h]
022BE8E4 E5991000 ldr     r1,[r9]
022BE8E8 E1500001 cmp     r0,r1
022BE8EC 0A000005 beq     22BE908h
022BE8F0 EB00056A bl      22BFEA0h
022BE8F4 E5951004 ldr     r1,[r5,4h]
022BE8F8 E5900004 ldr     r0,[r0,4h]
022BE8FC E1510000 cmp     r1,r0
022BE900 03A0B001 moveq   r11,1h
022BE904 0A000003 beq     22BE918h
022BE908 E2877001 add     r7,r7,1h
022BE90C E2866F4F add     r6,r6,13Ch
022BE910 E3570020 cmp     r7,20h
022BE914 BAFFFFEA blt     22BE8C4h
022BE918 E35B0000 cmp     r11,0h
022BE91C 1A000017 bne     22BE980h
022BE920 E5951004 ldr     r1,[r5,4h]
022BE924 E3A00003 mov     r0,3h                     //3 is "EFFECT/effect.bin"
022BE928 E1A01801 mov     r1,r1,lsl 10h
022BE92C E1A01841 mov     r1,r1,asr 10h
022BE930 E1A01801 mov     r1,r1,lsl 10h
022BE934 E1A01821 mov     r1,r1,lsr 10h
022BE938 EBF5367F bl      200C33Ch                  /// 0200C33C LookupPackFileSubFiles 200C33Ch (r0 = fileid (0 to 5), r1 = subfileindex )
022BE93C E1540000 cmp     r4,r0
022BE940 AA00000E bge     22BE980h
022BE944 E28D5004 add     r5,r13,4h
022BE948 E3A04002 mov     r4,2h
022BE94C E8B9000F ldmia   [r9]!,r0-r3
022BE950 E8A5000F stmia   [r5]!,r0-r3
022BE954 E2544001 subs    r4,r4,1h
022BE958 1AFFFFFB bne     22BE94Ch
022BE95C E8990007 ldmia   [r9],r0-r2
022BE960 E8850007 stmia   [r5],r0-r2
022BE964 E3A03008 mov     r3,8h
022BE968 E28D1004 add     r1,r13,4h
022BE96C E1A0000A mov     r0,r10
022BE970 E1A02008 mov     r2,r8
022BE974 E58D3004 str     r3,[r13,4h]
022BE978 EBFFFF6C bl      22BE730h
022BE97C EA000003 b       22BE990h
022BE980 E1A0000A mov     r0,r10
022BE984 E1A01009 mov     r1,r9
022BE988 E1A02008 mov     r2,r8
022BE98C EBFFFF67 bl      22BE730h
022BE990 E28DD05C add     r13,r13,5Ch
022BE994 E8BD8FF0 pop     r4-r11,r15
022BE998 022DC1C0 
022BE99C 00000287 

///
///
///
022F74D4 E92D4078 push    r3-r6,r14
022F74D8 E24DD004 sub     r13,r13,4h
022F74DC E59F10E0 ldr     r1,=2353538h
022F74E0 E1A06000 mov     r6,r0
022F74E4 E5912000 ldr     r2,[r1]
022F74E8 E59F10D8 ldr     r1,=19914h
022F74EC E0822086 add     r2,r2,r6,lsl 1h
022F74F0 E19210F1 ldrsh   r1,[r2,r1]
022F74F4 E3510000 cmp     r1,0h
022F74F8 1A00002F bne     22F75BCh
022F74FC EBF56C7A bl      20526ECh
022F7500 E1A04000 mov     r4,r0
022F7504 E1A00006 mov     r0,r6
022F7508 EBF57565 bl      2054AA4h
022F750C E3500000 cmp     r0,0h
022F7510 0A000015 beq     22F756Ch
022F7514 E1A00006 mov     r0,r6
022F7518 EBF57561 bl      2054AA4h
022F751C E3500000 cmp     r0,0h
022F7520 E1A00006 mov     r0,r6
022F7524 13A05000 movne   r5,0h
022F7528 EBF56CBB bl      205281Ch
022F752C E3A01000 mov     r1,0h
022F7530 EBF4270E bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022F7534 E59F2090 ldr     r2,=237C9B0h
022F7538 E3A01001 mov     r1,1h
022F753C E7820105 str     r0,[r2,r5,lsl 2h]
022F7540 E58D1000 str     r1,[r13]
022F7544 E59F0084 ldr     r0,=20AFC68h
022F7548 E7923105 ldr     r3,[r2,r5,lsl 2h]
022F754C E5900000 ldr     r0,[r0]
022F7550 E1A02004 mov     r2,r4
022F7554 E3A01000 mov     r1,0h                 //0 is "MONSTER/monster.bin"
022F7558 EBF4980C bl      201D590h              ///Fun_201D590
022F755C E1A01000 mov     r1,r0
022F7560 E1A00006 mov     r0,r6
022F7564 EBFFFFA8 bl      22F740Ch
022F7568 EA00000A b       22F7598h
022F756C E3A01001 mov     r1,1h
022F7570 E59F0058 ldr     r0,=20AFC68h
022F7574 E58D1000 str     r1,[r13]
022F7578 E3A01000 mov     r1,0h                 //0 is "MONSTER/monster.bin"
022F757C E5900000 ldr     r0,[r0]
022F7580 E1A02004 mov     r2,r4
022F7584 E1A03001 mov     r3,r1
022F7588 EBF497BD bl      201D484h              ///Fun_0201D484
022F758C E1A01000 mov     r1,r0
022F7590 E1A00006 mov     r0,r6
022F7594 EBFFFF9C bl      22F740Ch
022F7598 E1A01004 mov     r1,r4
022F759C E3A00000 mov     r0,0h                     //0 is "MONSTER/monster.bin"
022F75A0 EBF45365 bl      200C33Ch                  /// 0200C33C LookupPackFileSubFiles 200C33Ch (r0 = fileid (0 to 5), r1 = subfileindex )
022F75A4 E59F1018 ldr     r1,=2353538h
022F75A8 E5911000 ldr     r1,[r1]
022F75AC E281190B add     r1,r1,2C000h
022F75B0 E5912B0C ldr     r2,[r1,0B0Ch]
022F75B4 E0820000 add     r0,r2,r0
022F75B8 E5810B0C str     r0,[r1,0B0Ch]
022F75BC E28DD004 add     r13,r13,4h
022F75C0 E8BD8078 pop     r3-r6,r15
022F75C4 02353538 
022F75C8 00019914 
022F75CC 0237C9B0
022F75D0 020AFC68 