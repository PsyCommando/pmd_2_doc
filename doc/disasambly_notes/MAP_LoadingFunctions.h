//Details:
Overlay_0011 is loaded at 0x022DC240

//Interesting strings:
2320C44 - "MAP_BG/bg_list.dat"
2320C58 - "rom0:/MAP_BG/%s.bpl"
2320C6C - "rom0:/MAP_BG/%s.bpc"
2320C80 - "rom0:/MAP_BG/%s.bma"
2320C94 - "rom0:/MAP_BG/%s.bpa"
2320CC8 - "ground_map.c"


//Notable static memory locations:
2324CBCh Pointer to a sort of dynamically allocated struct? Or maybe the content of a file.


//
[0x2388F24..0x2388F26]!? //For r10,2BCh

[r10,2BCh] = Unknown
[r10,2C0h] = Unknown, probably the nb of BPC entries!
[r10,2C2h] = Unknown
[r10,2C4h] = Unknown (usually 0xE)

Struct at [r10,1E0h]
{
    [r10,1E0h] = BMA_MapWidth
    [r10,1E1h] = BMA_MapHeight
    [r10,1E2h] = BMA_unk3
    [r10,1E3h] = BMA_unk4
    [r10,1E4h] = BMA_unk5
    [r10,1E6h] = BMA_unk6
    [r10,1E8h] = BMA_unk7
}







/*
    fun_022EBB40 22EBB40h(r0 = targetfnamestr, r1 = lvlid )
        Refers to the string :
        "MAP_BG/bg_list.dat"
        
        Loads the BG list file and get the files to load for the level id!
*/
022EBB40 E92D4FF8 push    r3-r11,r14
022EBB44 E24DD048 sub     r13,r13,48h
022EBB48 E1A0A000 mov     r10,r0                    //R10 = targetfnamestr
022EBB4C E1A04001 mov     r4,r1                     //R4  = Param1
022EBB50 EBF47184 bl      2008168h                  //FStreamAlloc()
022EBB54 E28D0000 add     r0,r13,0h                 //R0 = PtrFileStream
022EBB58 EBF471A9 bl      2008204h                  //FStreamCtor(r0 = PtrFStreamStruct)
022EBB5C E59F10B0 ldr     r1,=2320C44h              //"MAP_BG/bg_list.dat"
022EBB60 E28D0000 add     r0,r13,0h                 //R0 = PtrFileStream
022EBB64 EBF471A9 bl      2008210h                  //FStreamFOpen(r0 = PtrFStreamStruct, r1 = PtrFPath)
022EBB68 E3A00058 mov     r0,58h                    //R0 = 0x58
022EBB6C E0010094 mul     r1,r4,r0                  //R1 = Param1 * 0x58
022EBB70 E28D0000 add     r0,r13,0h                 //R0 = PtrFileStream
022EBB74 E3A02000 mov     r2,0h                     //R2 = 0
022EBB78 EBF471CA bl      20082A8h                  ///020082A8 FStreamSeek( r0 = PtrFStreamStruct, r1 = OffsetToSeekTo, r2 = unknown?(usually 0)  ) 
022EBB7C E28D0000 add     r0,r13,0h                 //R0 = PtrFileStream
022EBB80 E1A0100A mov     r1,r10                    //R1 = targetfnamestr
022EBB84 E3A02008 mov     r2,8h                     //R2 = 8
022EBB88 EBF471B1 bl      2008254h                  ///02008254 FStreamRead(r0 = PtrFStreamStruct, r1 = PtrOutBuffer, r2 = NbBytesToRead )
022EBB8C E3A00000 mov     r0,0h                     //R0 = 0
022EBB90 E5CA0008 strb    r0,[r10,8h]               //[targetfnamestr + 8] = 0
022EBB94 E28D0000 add     r0,r13,0h                 //R0 = PtrFileStream
022EBB98 E28A100A add     r1,r10,0Ah                //R1 = targetfnamestr + 0xA
022EBB9C E3A02008 mov     r2,8h                     //R2 = 8
022EBBA0 EBF471AB bl      2008254h                  ///02008254 FStreamRead(r0 = PtrFStreamStruct, r1 = PtrOutBuffer, r2 = NbBytesToRead )
022EBBA4 E3A00000 mov     r0,0h                     //R0 = 0
022EBBA8 E5CA0012 strb    r0,[r10,12h]              //[targetfnamestr +0x12] = 0    
022EBBAC E28D0000 add     r0,r13,0h                 //R0 = PtrFileStream
022EBBB0 E28A1014 add     r1,r10,14h                //R1 = targetfnamestr + 0x14
022EBBB4 E3A02008 mov     r2,8h                     //R2 = 8
022EBBB8 EBF471A5 bl      2008254h                  ///02008254 FStreamRead(r0 = PtrFStreamStruct, r1 = PtrOutBuffer, r2 = NbBytesToRead )
022EBBBC E3A08000 mov     r8,0h                     //CntR8 = 0
022EBBC0 E5CA801C strb    r8,[r10,1Ch]              //[targetfnamestr + 0x1C] = 0
022EBBC4 E28A601E add     r6,r10,1Eh                //R6  = targetfnamestr + 0x1E
022EBBC8 E28D7000 add     r7,r13,0h                 //R7  = R13 + 0
022EBBCC E3A0B008 mov     r11,8h                    //R11 = 8
022EBBD0 E1A05008 mov     r5,r8                     //R5  = 0
022EBBD4 E3A0400A mov     r4,0Ah                    //R4  = 0xA

///022EBBD8 LOOP1_BEG
022EBBD8 E0090498 mul     r9,r8,r4                  //R9 = CntR8 * 0xA
022EBBDC E1A00007 mov     r0,r7                     //R0 = PtrFileStream
022EBBE0 E1A0200B mov     r2,r11                    //R2 = R2 = 8
022EBBE4 E0861009 add     r1,r6,r9                  //R1 = (targetfnamestr + 0x1E) + (CntR8 * 0xA)
022EBBE8 EBF47199 bl      2008254h                  ///02008254 FStreamRead(r0 = PtrFStreamStruct, r1 = PtrOutBuffer, r2 = NbBytesToRead )
022EBBEC E08A0009 add     r0,r10,r9                 //R0 = targetfnamestr + (CntR8 * 0xA)
022EBBF0 E2888001 add     r8,r8,1h                  //CntR8 = CntR8 + 1
022EBBF4 E5C05026 strb    r5,[r0,26h]               //[(targetfnamestr + (CntR8 * 0xA)) + 0x26] = 0
022EBBF8 E3580008 cmp     r8,8h 
if( CntR8 < 8 )
    022EBBFC BAFFFFF5 blt     22EBBD8h                      ///022EBBD8 LOOP1_BEG

022EBC00 E28D0000 add     r0,r13,0h                 //R0 = PtrFileStream
022EBC04 EBF471AE bl      20082C4h
022EBC08 EBF47161 bl      2008194h
022EBC0C E28DD048 add     r13,r13,48h
022EBC10 E8BD8FF8 pop     r3-r11,r15
022EBC14 02320C44

/*
    fun_022EBFC8 FreeMapFileBuffers
        R0 = Pointer to level struct? 
        

        NOTE: free and zero bma, bpc, bpl buffers ptrs
*/
022EBFC8 E92D41F0 push    r4-r8,r14
022EBFCC E1A08000 mov     r8,r0
022EBFD0 E1D810F0 ldrsh   r1,[r8]
022EBFD4 E3510003 cmp     r1,3h
if( R1 != 3 )
    022EBFD8 1A000001 bne     22EBFE4h                  ///022EBFE4 LBL1
022EBFDC EBFFFFDF bl      22EBF60h
022EBFE0 E8BD81F0 pop     r4-r8,r15
///022EBFE4 LBL1
022EBFE4 E3A06000 mov     r6,0h                 //R6 = CNT = 0
022EBFE8 E28850C4 add     r5,r8,0C4h            //R5 = PtrLvlStruct + 0xC4
022EBFEC E3A0402C mov     r4,2Ch

///022EBFF0 LOOP1_BEG
022EBFF0 E0275496 mla     r7,r6,r4,r5           //R7 = (CNT * 0x2C) + (PtrLvlStruct + 0xC4)
022EBFF4 E2870008 add     r0,r7,8h              //R0 = ((CNT * 0x2C) + (PtrLvlStruct + 0xC4)) + 8
022EBFF8 EBF45C8E bl      2003238h              //IsPointerNull(R0 = Ptr)
022EBFFC E3500000 cmp     r0,0h
if( R0 != 0 )
    022EC000 1A000001 bne     22EC00Ch          ///022EC00C LBL2
022EC004 E2870008 add     r0,r7,8h
022EC008 EBF472F1 bl      2008BD4h              //02008BD4
{
    02008BD4 E92D4010 push    r4,r14
    02008BD8 E1A04000 mov     r4,r0
    02008BDC E5940000 ldr     r0,[r4]
    02008BE0 EBFFE168 bl      2001188h          ///MemFree(r0 = ptrbufftofree)
    02008BE4 E3A00000 mov     r0,0h
    02008BE8 E5840000 str     r0,[r4]
    02008BEC E5840004 str     r0,[r4,4h]
    02008BF0 E8BD8010 pop     r4,r15
}
///022EC00C LBL2
022EC00C E2866001 add     r6,r6,1h              //R6 = CNT + 1
022EC010 E3560004 cmp     r6,4h
if(CNT < 4)
    022EC014 BAFFFFF5 blt     22EBFF0h              ///022EBFF0 LOOP1_BEG

022EC018 E2880E1A add     r0,r8,1A0h
022EC01C EBF5DD96 bl      206367Ch
022EC020 E2880F65 add     r0,r8,194h
022EC024 EBF5DD6B bl      20635D8h
022EC028 E2880F5D add     r0,r8,174h            //R0 = PTR_PtrBPLBuff
022EC02C EBF45C81 bl      2003238h              //IsPointerNull(R0 = Ptr)
022EC030 E3500000 cmp     r0,0h
022EC034 1A000001 bne     22EC040h
022EC038 E2880F5D add     r0,r8,174h            //R0 = PTR_PtrBPLBuff
022EC03C EBF472E4 bl      2008BD4h
{
    02008BD4 E92D4010 push    r4,r14
    02008BD8 E1A04000 mov     r4,r0
    02008BDC E5940000 ldr     r0,[r4]
    02008BE0 EBFFE168 bl      2001188h          ///MemFree(r0 = ptrbufftofree)
    02008BE4 E3A00000 mov     r0,0h
    02008BE8 E5840000 str     r0,[r4]
    02008BEC E5840004 str     r0,[r4,4h]
    02008BF0 E8BD8010 pop     r4,r15
}
022EC040 E2880F5F add     r0,r8,17Ch
022EC044 EBF45C7B bl      2003238h              //IsPointerNull(R0 = Ptr)
022EC048 E3500000 cmp     r0,0h
022EC04C 1A000001 bne     22EC058h
022EC050 E2880F5F add     r0,r8,17Ch
022EC054 EBF472DE bl      2008BD4h
{
    02008BD4 E92D4010 push    r4,r14
    02008BD8 E1A04000 mov     r4,r0
    02008BDC E5940000 ldr     r0,[r4]
    02008BE0 EBFFE168 bl      2001188h          ///MemFree(r0 = ptrbufftofree)
    02008BE4 E3A00000 mov     r0,0h
    02008BE8 E5840000 str     r0,[r4]
    02008BEC E5840004 str     r0,[r4,4h]
    02008BF0 E8BD8010 pop     r4,r15
}
022EC058 E2880F61 add     r0,r8,184h            //R0 = PTR_PtrBPCBuff
022EC05C EBF45C75 bl      2003238h              //IsPointerNull(R0 = Ptr)
022EC060 E3500000 cmp     r0,0h
022EC064 1A000001 bne     22EC070h
022EC068 E2880F61 add     r0,r8,184h            //R0 = PTR_PtrBPCBuff
022EC06C EBF472D8 bl      2008BD4h
{
    02008BD4 E92D4010 push    r4,r14
    02008BD8 E1A04000 mov     r4,r0
    02008BDC E5940000 ldr     r0,[r4]
    02008BE0 EBFFE168 bl      2001188h          ///MemFree(r0 = ptrbufftofree)
    02008BE4 E3A00000 mov     r0,0h
    02008BE8 E5840000 str     r0,[r4]
    02008BEC E5840004 str     r0,[r4,4h]
    02008BF0 E8BD8010 pop     r4,r15
}
022EC070 E2880F63 add     r0,r8,18Ch            //R0 = PTR_PtrBMABuff
022EC074 EBF45C6F bl      2003238h              //IsPointerNull(R0 = Ptr)
022EC078 E3500000 cmp     r0,0h
022EC07C 18BD81F0 popne   r4-r8,r15
022EC080 E2880F63 add     r0,r8,18Ch            //R0 = PTR_PtrBMABuff
022EC084 EBF472D2 bl      2008BD4h
{
    02008BD4 E92D4010 push    r4,r14
    02008BD8 E1A04000 mov     r4,r0
    02008BDC E5940000 ldr     r0,[r4]
    02008BE0 EBFFE168 bl      2001188h          ///MemFree(r0 = ptrbufftofree)    
    02008BE4 E3A00000 mov     r0,0h
    02008BE8 E5840000 str     r0,[r4]
    02008BEC E5840004 str     r0,[r4,4h]
    02008BF0 E8BD8010 pop     r4,r15
}
022EC088 E8BD81F0 pop     r4-r8,r15

/*
    fun_022EC08C( PtrStruct )
    
*/
022EC08C E92D4FF8 push    r3-r11,r14
022EC090 E1A0A000 mov     r10,r0
022EC094 EBFFFFCB bl      22EBFC8h
022EC098 E3A02000 mov     r2,0h
022EC09C E1CA20B0 strh    r2,[r10]
022EC0A0 E5CA21C0 strb    r2,[r10,1C0h]
022EC0A4 E2421001 sub     r1,r2,1h
022EC0A8 E28A0C01 add     r0,r10,100h
022EC0AC E1C01BBE strh    r1,[r0,0BEh]
022EC0B0 E28A0C02 add     r0,r10,200h
022EC0B4 E1C02BB8 strh    r2,[r0,0B8h]
022EC0B8 E5CA21E0 strb    r2,[r10,1E0h]
022EC0BC E5CA21E1 strb    r2,[r10,1E1h]
022EC0C0 E5CA21E2 strb    r2,[r10,1E2h]
022EC0C4 E5CA21E3 strb    r2,[r10,1E3h]
022EC0C8 E5CA21EE strb    r2,[r10,1EEh]
022EC0CC E58A21F4 str     r2,[r10,1F4h]
022EC0D0 E5CA21F8 strb    r2,[r10,1F8h]
022EC0D4 E5CA21F9 strb    r2,[r10,1F9h]
022EC0D8 E5CA21FA strb    r2,[r10,1FAh]
022EC0DC E5CA21FB strb    r2,[r10,1FBh]
022EC0E0 E5CA21BC strb    r2,[r10,1BCh]
022EC0E4 E28A0004 add     r0,r10,4h
022EC0E8 E1A05002 mov     r5,r2
022EC0EC E1C050B0 strh    r5,[r0]
022EC0F0 E1C050B2 strh    r5,[r0,2h]
022EC0F4 E2822001 add     r2,r2,1h
022EC0F8 E5805008 str     r5,[r0,8h]
022EC0FC E5805004 str     r5,[r0,4h]
022EC100 E3520010 cmp     r2,10h
022EC104 E280000C add     r0,r0,0Ch
022EC108 BAFFFFF7 blt     22EC0ECh
022EC10C E3A09000 mov     r9,0h             //r9 = 0
022EC110 E28A40C4 add     r4,r10,0C4h       //r4 = r10 + 0xC4
022EC114 E1A08009 mov     r8,r9             //r8 = 0
022EC118 E3A0702C mov     r7,2Ch            //r7 = 0x2C
022EC11C E0000795 mul     r0,r5,r7          //r0 = r5 * 0x2C
022EC120 E0846000 add     r6,r4,r0          //r6 = (r10 + 0xC4) + (r5 * 0x2C)
022EC124 E7C49000 strb    r9,[r4,r0]        
022EC128 E5C69001 strb    r9,[r6,1h]
022EC12C E1C690B2 strh    r9,[r6,2h]
022EC130 E1C690B4 strh    r9,[r6,4h]
022EC134 E2860008 add     r0,r6,8h
022EC138 EBF45C3A bl      2003228h
022EC13C E5868010 str     r8,[r6,10h]
022EC140 E5868018 str     r8,[r6,18h]
022EC144 E5868014 str     r8,[r6,14h]
022EC148 E5868020 str     r8,[r6,20h]
022EC14C E586801C str     r8,[r6,1Ch]
022EC150 E2855001 add     r5,r5,1h
022EC154 E5868024 str     r8,[r6,24h]
022EC158 E5868028 str     r8,[r6,28h]
022EC15C E3550004 cmp     r5,4h
if( r5 < 4 )
    022EC160 BAFFFFED blt     22EC11Ch
022EC164 E28A4C02 add     r4,r10,200h
022EC168 E1D41CF2 ldrsh   r1,[r4,0C2h]
022EC16C E59F00C0 ldr     r0,=2324CBCh
022EC170 E5DA22BC ldrb    r2,[r10,2BCh]
022EC174 E5903000 ldr     r3,[r0]
022EC178 E1A01A01 mov     r1,r1,lsl 14h
022EC17C E3A00028 mov     r0,28h
022EC180 E2836098 add     r6,r3,98h
022EC184 E1650082 smulbb  r5,r2,r0
022EC188 E1A07821 mov     r7,r1,lsr 10h
022EC18C E59FB0A4 ldr     r11,=2320BF4h
022EC190 EA000012 b       22EC1E0h
022EC194 E2870001 add     r0,r7,1h
022EC198 E1A03800 mov     r3,r0,lsl 10h
022EC19C E1A01007 mov     r1,r7
022EC1A0 E1A0200B mov     r2,r11
022EC1A4 E0860005 add     r0,r6,r5
022EC1A8 E1A07823 mov     r7,r3,lsr 10h
022EC1AC EBF478F7 bl      200A590h
022EC1B0 E3A09001 mov     r9,1h
022EC1B4 E2870001 add     r0,r7,1h
022EC1B8 E1A03800 mov     r3,r0,lsl 10h
022EC1BC E1A01007 mov     r1,r7
022EC1C0 E59F2074 ldr     r2,=2320BE8h
022EC1C4 E0860005 add     r0,r6,r5
022EC1C8 E1A07823 mov     r7,r3,lsr 10h
022EC1CC EBF478EF bl      200A590h
022EC1D0 E2899001 add     r9,r9,1h
022EC1D4 E3590010 cmp     r9,10h
022EC1D8 BAFFFFF5 blt     22EC1B4h
022EC1DC E2888001 add     r8,r8,1h
022EC1E0 E1D40CF4 ldrsh   r0,[r4,0C4h]
022EC1E4 E1580000 cmp     r8,r0
022EC1E8 BAFFFFE9 blt     22EC194h
022EC1EC E0860005 add     r0,r6,r5
022EC1F0 EBF478C3 bl      200A504h
022EC1F4 E28A0C02 add     r0,r10,200h
022EC1F8 E1D00CF0 ldrsh   r0,[r0,0C0h]
022EC1FC E3500000 cmp     r0,0h
022EC200 E1A0000A mov     r0,r10
022EC204 DA000002 ble     22EC214h
022EC208 E3A01001 mov     r1,1h
022EC20C EB000903 bl      22EE620h
022EC210 EA000001 b       22EC21Ch
022EC214 E3A01000 mov     r1,0h
022EC218 EB000900 bl      22EE620h
022EC21C E59A1228 ldr     r1,[r10,228h]
022EC220 E28A0E21 add     r0,r10,210h
022EC224 E12FFF31 blx     r1
022EC228 E3A00001 mov     r0,1h
022EC22C E5CA02BA strb    r0,[r10,2BAh]
022EC230 E8BD8FF8 pop     r3-r11,r15
022EC234 02324CBC 
022EC238 02320BF4 
022EC23C 02320BE8 


/*
    fun_02311074_GroundSubMapIndependent_Select 2311074h
*/
02311074 E92D4038 push    r3-r5,r14
02311078 E24DD008 sub     r13,r13,8h            //Alloc 8 bytes on stack
0231107C E59F11E8 ldr     r1,=2324DF8h          //ptr_tbl_2324DF8h
02311080 E5912000 ldr     r2,[r1]               //r2 = Ptr
02311084 E5920008 ldr     r0,[r2,8h]
02311088 E3500002 cmp     r0,2h
0231108C 1A000066 bne     231122Ch
02311090 E3A00001 mov     r0,1h
02311094 E5C2000C strb    r0,[r2,0Ch]
02311098 E5912000 ldr     r2,[r1]
0231109C E59F11CC ldr     r1,=2323974h          //"GroundSubMapIndependent Select %3d"
023110A0 E1D250F0 ldrsh   r5,[r2]
023110A4 E1A02005 mov     r2,r5
023110A8 EBF3EC64 bl      200C240h              //DebugPrint
023110AC EBFFFF08 bl      2310CD4h
023110B0 E3A00FBD mov     r0,2F4h               //Size buffer elem 0x2F4
023110B4 E3A0100F mov     r1,0Fh                //
023110B8 EBF3C02C bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
023110BC E59F21A8 ldr     r2,=2324DF8h          //ptr_tbl_2324DF8h
023110C0 E3E01000 mvn     r1,0h
023110C4 E5820004 str     r0,[r2,4h]
023110C8 E5920000 ldr     r0,[r2]
023110CC E1550001 cmp     r5,r1
023110D0 E1C050B0 strh    r5,[r0]                   //r5 = levelid
if( levelid != 0xFFFFFFFF )
    023110D4 1A00000C bne     231110Ch                  ///0231110C
023110D8 E5920004 ldr     r0,[r2,4h]
023110DC E59F1190 ldr     r1,=2323920h              //DefaultValueTable_2323920?
023110E0 EBFF6ACC bl      22EBC18h                  ///fun_022EBC18 Loads correct constants for the level id!
023110E4 E59F0180 ldr     r0,=2324DF8h              //ptr_tbl_2324DF8h
023110E8 E3E01000 mvn     r1,0h                     //r1 = 0xFFFFFFFF
023110EC E5900004 ldr     r0,[r0,4h]                //r0 = [ptr_tbl_2324DF8h,4h]
023110F0 EBFF6C61 bl      22EC27Ch                  /// LoadBGTileSets (r0 = struct ptr, r1 = level id)
023110F4 E59F0170 ldr     r0,=2324DF8h              //ptr_tbl_2324DF8h
023110F8 E5902000 ldr     r2,[r0]                   //r2 = [ptr_tbl_2324DF8h]
023110FC E5901004 ldr     r1,[r0,4h]                //r1 = [ptr_tbl_2324DF8h,4h]
02311100 E2820010 add     r0,r2,10h                 //r0 = [ptr_tbl_2324DF8h] + 10h
02311104 EBFF7B14 bl      22EFD5Ch                  ///fun_022EFD5C
02311108 EA00003F b       231120Ch                  ///0231120C v
///0231110C levelid != 0xFFFFFFFF
0231110C E3A0000C mov     r0,0Ch
02311110 E1610085 smulbb  r1,r5,r0                  //r1 = levelid * 12
02311114 E59F315C ldr     r3,=20A5488h              //leveltable
02311118 E19300F1 ldrsh   r0,[r3,r1]
0231111C E0834001 add     r4,r3,r1
02311120 E2800001 add     r0,r0,1h
02311124 E350000C cmp     r0,0Ch
02311128 908FF100 addls   r15,r15,r0,lsl 2h
0231112C EA000022 b       23111BCh                  ///023111BC //r0 > 12
02311130 EA000017 b       2311194h                  ///02311194 //r0 == 0
02311134 EA000020 b       23111BCh                  ///023111BC //r0 == 1
02311138 EA00001F b       23111BCh                  ///023111BC //r0 == 2
0231113C EA00001E b       23111BCh                  ///023111BC //r0 == 3
02311140 EA00001D b       23111BCh                  ///023111BC //r0 == 4
02311144 EA00001C b       23111BCh                  ///023111BC //r0 == 5
02311148 EA000005 b       2311164h                  ///02311164 //r0 == 6
0231114C EA000004 b       2311164h                  ///02311164 //r0 == 7
02311150 EA000003 b       2311164h                  ///02311164 //r0 == 8
02311154 EA000002 b       2311164h                  ///02311164 //r0 == 9
02311158 EA000001 b       2311164h                  ///02311164 //r0 == 10
0231115C EA000006 b       231117Ch                  ///0231117C //r0 == 11
02311160 EA000005 b       231117Ch                  ///0231117C //r0 == 12
///02311164 - R0 == 6,7,8,9,10
02311164 E5920004 ldr     r0,[r2,4h]
02311168 E59F110C ldr     r1,=232393Ch
0231116C EBFF6AA9 bl      22EBC18h                        ///fun_022EBC18 Loads correct constants for the level id!
02311170 E3A00001 mov     r0,1h
02311174 EBFF78F0 bl      22EF53Ch
02311178 EA000015 b       23111D4h
///0231117C - R0 == 11,12
0231117C E5920004 ldr     r0,[r2,4h]
02311180 E59F10F8 ldr     r1,=2323958h
02311184 EBFF6AA3 bl      22EBC18h                        ///fun_022EBC18 Loads correct constants for the level id!
02311188 E3A00001 mov     r0,1h
0231118C EB0005CB bl      23128C0h
02311190 EA00000F b       23111D4h
///02311194 - R0 == 0
02311194 E59F00E8 ldr     r0,=23238CCh
02311198 E1D430F0 ldrsh   r3,[r4]
0231119C E590C00C ldr     r12,[r0,0Ch]
023111A0 E590E008 ldr     r14,[r0,8h]
023111A4 E59F10DC ldr     r1,=2323998h
023111A8 E28D0000 add     r0,r13,0h
023111AC E1A02005 mov     r2,r5
023111B0 E58DE000 str     r14,[r13]
023111B4 E58DC004 str     r12,[r13,4h]
023111B8 EBF3EC27 bl      200C25Ch                      //Fatal error?
///023111BC - R0 == 1,2,3,4,5 || r0 > 12
023111BC E59F00A8 ldr     r0,=2324DF8h
023111C0 E59F10AC ldr     r1,=2323920h
023111C4 E5900004 ldr     r0,[r0,4h]
023111C8 EBFF6A92 bl      22EBC18h                        ///fun_022EBC18 Loads correct constants for the level id!
023111CC E3A00001 mov     r0,1h
023111D0 EB0005BA bl      23128C0h
023111D4 E59F0090 ldr     r0,=2324DF8h
023111D8 E1D410F4 ldrsh   r1,[r4,4h]
023111DC E5900004 ldr     r0,[r0,4h]
023111E0 EBFF6C25 bl      22EC27Ch              /// LoadBGTileSets (r0 = struct ptr, r1 = level id)
023111E4 E59F0080 ldr     r0,=2324DF8h
023111E8 E5902000 ldr     r2,[r0]
023111EC E5901004 ldr     r1,[r0,4h]
023111F0 E2820010 add     r0,r2,10h
023111F4 EBFF7AD8 bl      22EFD5Ch              ///fun_022EFD5C
023111F8 E59F006C ldr     r0,=2324DF8h
023111FC E59F2088 ldr     r2,=23238CCh
02311200 E5900004 ldr     r0,[r0,4h]
02311204 E3A01000 mov     r1,0h
02311208 EBFF7831 bl      22EF2D4h
///0231120C
0231120C E59F0058 ldr     r0,=2324DF8h
02311210 E3A03004 mov     r3,4h
02311214 E5902000 ldr     r2,[r0]
02311218 E3A01000 mov     r1,0h
0231121C E5823008 str     r3,[r2,8h]
02311220 E5900000 ldr     r0,[r0]
02311224 E5C0100C strb    r1,[r0,0Ch]
02311228 EA00000D b       2311264h              ///02311264 v

0231122C E3500003 cmp     r0,3h
02311230 1A00000B bne     2311264h
02311234 E3A00001 mov     r0,1h
02311238 E5C2000C strb    r0,[r2,0Ch]
0231123C E5910000 ldr     r0,[r1]
02311240 E1D000F2 ldrsh   r0,[r0,2h]
02311244 EB0005CC bl      231297Ch
02311248 E59F001C ldr     r0,=2324DF8h
0231124C E3A03004 mov     r3,4h
02311250 E5902000 ldr     r2,[r0]
02311254 E3A01000 mov     r1,0h
02311258 E5823008 str     r3,[r2,8h]
0231125C E5900000 ldr     r0,[r0]
02311260 E5C0100C strb    r1,[r0,0Ch]
///02311264
02311264 E28DD008 add     r13,r13,8h
02311268 E8BD8038 pop     r3-r5,r15
0231126C 02324DF8 
02311270 02323974 
02311274 02323920
02311278 020A5488 
0231127C 0232393C 
02311280 02323958 
02311284 023238CC 
02311288 02323998 
0231128C 023238CC 


/*
    fun_022F173C GroundMap_Change 22F173Ch
*/
022F173C E92D4010 push    r4,r14
022F1740 E1A04000 mov     r4,r0                 //r4 = LevelID
022F1744 E59F1040 ldr     r1,=2321018h          //"GroundMap Change %3d"
022F1748 E1A02004 mov     r2,r4
022F174C E3A00001 mov     r0,1h
022F1750 EBF46ABA bl      200C240h              //DebugPrint
022F1754 E59F2034 ldr     r2,=2324CC0h          //ptr_tbl_2324CC0h 
022F1758 E59F1034 ldr     r1,=20A5488h          //LevelTableBeg
022F175C E5923004 ldr     r3,[r2,4h]            //r3 [ptr_tbl_2324CC0h,4h]    
022F1760 E3A0000C mov     r0,0Ch                //r0 = 12
022F1764 E1C340B0 strh    r4,[r3]               //[r3] = LevelID
022F1768 E1001084 smlabb  r0,r4,r0,r1           //r0 = (LevelID * 12) + LevelTableBeg
022F176C E1D010F4 ldrsh   r1,[r0,4h]
022F1770 E5920000 ldr     r0,[r2]
022F1774 EBFFF169 bl      22EDD20h
022F1778 E59F0010 ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h 
022F177C E3A01001 mov     r1,1h
022F1780 E5900004 ldr     r0,[r0,4h]
022F1784 E5C01002 strb    r1,[r0,2h]
022F1788 E8BD8010 pop     r4,r15
022F178C 02321018 
022F1790 02324CC0 
022F1794 020A5488 

/*
    fun_022F12C0 GroundMap_Select 22F12C0h
    r0 = levelid
    
*/
022F12C0 E92D4070 push    r4-r6,r14
022F12C4 E24DD008 sub     r13,r13,8h            //Alloc 8 bytes on stack    
022F12C8 E1A05000 mov     r5,r0                 //r5 = levelid
022F12CC E59F1260 ldr     r1,=2320FB4h          //"GroundMap Select %3d"
022F12D0 E1A02005 mov     r2,r5                 //r2 = param2
022F12D4 E3A00001 mov     r0,1h                 //r0 = 1
022F12D8 EBF46BD8 bl      200C240h              //DebugPrint
022F12DC EBFFFFD8 bl      22F1244h              //022F1244 - Seems to initialise / prepare the level to switch!
{
    //022F1244
    022F1244 E92D4008 push    r3,r14
    022F1248 EB008083 bl      231145Ch          ///0231145C //IsPtr_2324E00hNull
    {
        ///0231145C
        0231145C E59F0014 ldr     r0,=2324E00h
        02311460 E5900000 ldr     r0,[r0]
        02311464 E3500000 cmp     r0,0h
        if( r0 != 0 )
            02311468 13A00001 movne   r0,1h             //r0 = 1
        else
            0231146C 03A00000 moveq   r0,0h             //r0 = 0
        02311470 E20000FF and     r0,r0,0FFh            //r0 = r0 & 0xFF
        02311474 E12FFF1E bx      r14
        02311478 02324E00 
    }
    022F124C E3500000 cmp     r0,0h
    if( r0 == 0 ) //if Ptr_2324E00h is not null!
        022F1250 0A000000 beq     22F1258h              ///022F1258
    else 
    {
        //Ptr_2324E00h is null!
        022F1254 EB008063 bl      23113E8h              ///023113E8
        {
            ///023113E8
            023113E8 E92D4008 push    r3,r14
            023113EC E59F0064 ldr     r0,=2324E00h      //
            023113F0 E3A03000 mov     r3,0h             //r3 = 0
            023113F4 E5901000 ldr     r1,[r0]           //r1 = [2324E00h] //ptr_2324E00h
            023113F8 E3A02001 mov     r2,1h             //r2 = 1
            023113FC E5C13000 strb    r3,[r1]           //[ptr_2324E00h] = 0
            02311400 E5901000 ldr     r1,[r0]           //r1 = [2324E00h] //ptr_2324E00h
            02311404 E5812004 str     r2,[r1,4h]        //[ptr_2324E00h,4h] = 1
            02311408 E5900000 ldr     r0,[r0]           //r0 = [2324E00h] //ptr_2324E00h
            0231140C E5C02008 strb    r2,[r0,8h]        //[ptr_2324E00h,8h] = 1
            02311410 EB0005F7 bl      2312BF4h
            {
                02312BF4 E92D4008 push    r3,r14
                02312BF8 EB0002E6 bl      2313798h
                02312BFC E59F0050 ldr     r0,=2324E28h
                02312C00 E5900004 ldr     r0,[r0,4h]
                02312C04 E5D00010 ldrb    r0,[r0,10h]
                02312C08 E3500000 cmp     r0,0h
                02312C0C 0A000004 beq     2312C24h
                02312C10 EB0006B8 bl      23146F8h
                02312C14 E59F0038 ldr     r0,=2324E28h
                02312C18 E3A01000 mov     r1,0h
                02312C1C E5900004 ldr     r0,[r0,4h]
                02312C20 E5C01010 strb    r1,[r0,10h]
                02312C24 E59F0028 ldr     r0,=2324E28h
                02312C28 E3A02000 mov     r2,0h
                02312C2C E5901004 ldr     r1,[r0,4h]
                02312C30 E5812000 str     r2,[r1]
                02312C34 E5901004 ldr     r1,[r0,4h]
                02312C38 E1C120B4 strh    r2,[r1,4h]
                02312C3C E5901004 ldr     r1,[r0,4h]
                02312C40 E1C120B6 strh    r2,[r1,6h]
                02312C44 E5900004 ldr     r0,[r0,4h]
                02312C48 E2800C05 add     r0,r0,500h
                02312C4C E1C027B8 strh    r2,[r0,78h]
                02312C50 E8BD8008 pop     r3,r15
                02312C54 02324E28 eoreqs  r4,r2,280h
            }
            02311414 E3A00000 mov     r0,0h
            02311418 EB000528 bl      23128C0h
            0231141C E3A00001 mov     r0,1h
            02311420 E3A01005 mov     r1,5h
            02311424 EBFF6725 bl      22EB0C0h
            02311428 E59F0028 ldr     r0,=2324E00h
            0231142C E5900004 ldr     r0,[r0,4h]
            02311430 E3500000 cmp     r0,0h
            02311434 08BD8008 popeq   r3,r15
            02311438 EBFF6A9B bl      22EBEACh
            0231143C E59F0014 ldr     r0,=2324E00h
            02311440 E5900004 ldr     r0,[r0,4h]
            02311444 EBF3BF4F bl      2001188h              ///MemFree(r0 = ptrbufftofree)
            02311448 E59F0008 ldr     r0,=2324E00h
            0231144C E3A01000 mov     r1,0h
            02311450 E5801004 str     r1,[r0,4h]
            02311454 E8BD8008 pop     r3,r15
            02311458 02324E00 
        }
    }
    ///022F1258 - (Ptr_2324E00h != 0)
    022F1258 EB0081BF bl      231195Ch          ///0231195C //IsPtr_2324E08hNull
    {
        ///0231195C
        0231195C E59F0014 ldr     r0,=2324E08h          //r0 = 0x2324E08 
        02311960 E5900000 ldr     r0,[r0]               //r0 = [0x2324E08] //Some pointer
        02311964 E3500000 cmp     r0,0h
        if( r0 != 0 )
            02311968 13A00001 movne   r0,1h             //r0 = 1
        else
            0231196C 03A00000 moveq   r0,0h             //r0 = 0
        02311970 E20000FF and     r0,r0,0FFh            //r0 = & 0xFF
        02311974 E12FFF1E bx      r14                   //Return
    }
    022F125C E3500000 cmp     r0,0h
    if( r0 == 0 )   //if( Ptr_2324E08h != 0 )
        022F1260 0A000000 beq     22F1268h              ///022F1268
    else
    {
        //if Ptr_2324E08h is null!
        022F1264 EB00819F bl      23118E8h              ///023118E8
        {
            ///023118E8
            023118E8 E92D4008 push    r3,r14
            023118EC E59F0064 ldr     r0,=2324E08h
            023118F0 E3A03000 mov     r3,0h
            023118F4 E5901000 ldr     r1,[r0]
            023118F8 E3A02001 mov     r2,1h
            023118FC E5C13000 strb    r3,[r1]
            02311900 E5901000 ldr     r1,[r0]
            02311904 E5812004 str     r2,[r1,4h]
            02311908 E5900000 ldr     r0,[r0]
            0231190C E5C02008 strb    r2,[r0,8h]
            02311910 EB0004B7 bl      2312BF4h
            02311914 E3A00000 mov     r0,0h
            02311918 EB0003E8 bl      23128C0h
            0231191C E3A00001 mov     r0,1h
            02311920 E3A01005 mov     r1,5h
            02311924 EBFF65E5 bl      22EB0C0h
            02311928 E59F0028 ldr     r0,=2324E08h
            0231192C E5900004 ldr     r0,[r0,4h]
            02311930 E3500000 cmp     r0,0h
            02311934 08BD8008 popeq   r3,r15
            02311938 EBFF695B bl      22EBEACh
            0231193C E59F0014 ldr     r0,=2324E08h
            02311940 E5900004 ldr     r0,[r0,4h]
            02311944 EBF3BE0F bl      2001188h              ///MemFree(r0 = ptrbufftofree)
            02311948 E59F0008 ldr     r0,=2324E08h
            0231194C E3A01000 mov     r1,0h
            02311950 E5801004 str     r1,[r0,4h]
            02311954 E8BD8008 pop     r3,r15
            02311958 02324E08 
        }
    }
    ///022F1268 - if( Ptr_2324E08h != 0 )
    022F1268 E59F004C ldr     r0,=2324CC0h
    022F126C E3A02000 mov     r2,0h
    022F1270 E5901004 ldr     r1,[r0,4h]
    022F1274 E5C12003 strb    r2,[r1,3h]
    022F1278 E5900004 ldr     r0,[r0,4h]
    022F127C E5C02002 strb    r2,[r0,2h]
    022F1280 EB000762 bl      22F3010h              ///022F3010            
    {
        ///022F3010
        022F3010 E92D4008 push    r3,r14
        022F3014 EB000519 bl      22F4480h          ///022F4480
        {
            ///022F4480
            022F4480 E92D4008 push    r3,r14
            022F4484 E59F0040 ldr     r0,=2324CE0h
            022F4488 E3A01000 mov     r1,0h
            022F448C E5900000 ldr     r0,[r0]
            022F4490 E1C010B6 strh    r1,[r0,6h]
            022F4494 EBF5DD70 bl      206BA5Ch
            022F4498 EBFF3CFC bl      22C3890h
            022F449C EBFFFFAE bl      22F435Ch
            022F44A0 E59F0024 ldr     r0,=2324CE0h
            022F44A4 E5900000 ldr     r0,[r0]
            022F44A8 E5D00010 ldrb    r0,[r0,10h]
            022F44AC E3500000 cmp     r0,0h
            022F44B0 08BD8008 popeq   r3,r15
            022F44B4 EB00808F bl      23146F8h
            022F44B8 E59F000C ldr     r0,=2324CE0h
            022F44BC E3A01000 mov     r1,0h
            022F44C0 E5900000 ldr     r0,[r0]
            022F44C4 E5C01010 strb    r1,[r0,10h]
            022F44C8 E8BD8008 pop     r3,r15
            022F44CC 02324CE0
        }
        022F3018 E59F2030 ldr     r2,=2324CE0h
        022F301C E3A03000 mov     r3,0h
        022F3020 E5920000 ldr     r0,[r2]
        022F3024 E3A01010 mov     r1,10h
        022F3028 E5803000 str     r3,[r0]
        022F302C E5920000 ldr     r0,[r2]
        022F3030 E1C030B4 strh    r3,[r0,4h]
        022F3034 E5920000 ldr     r0,[r2]
        022F3038 E2800C03 add     r0,r0,300h
        022F303C E1C033B8 strh    r3,[r0,38h]
        022F3040 E5920000 ldr     r0,[r2]
        022F3044 E2800FD1 add     r0,r0,344h
        022F3048 EBF44080 bl      2003250h          //FillBufferWithZeros( R0=buffaddress, R1=BuffLen )
        022F304C E8BD8008 pop     r3,r15
        022F3050 02324CE0 
    }
    022F1284 E3A00000 mov     r0,0h
    022F1288 EB000419 bl      22F22F4h
    022F128C E59F0028 ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h    
    022F1290 E5900000 ldr     r0,[r0]
    022F1294 E3500000 cmp     r0,0h
    022F1298 08BD8008 popeq   r3,r15
    022F129C EBFFEB02 bl      22EBEACh
    022F12A0 E59F0014 ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
    022F12A4 E5900000 ldr     r0,[r0]
    022F12A8 EBF43FB6 bl      2001188h              ///MemFree(r0 = ptrbufftofree)
    022F12AC E59F0008 ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
    022F12B0 E3A01000 mov     r1,0h
    022F12B4 E5801000 str     r1,[r0]
    022F12B8 E8BD8008 pop     r3,r15
    022F12BC 02324CC0
}
022F12E0 E3A00FBD mov     r0,2F4h                   //r0 = 0x2F4    //Elem len
022F12E4 E3A01006 mov     r1,6h                     //r1 = 6        //Nb elems
022F12E8 EBF43FA0 bl      2001170h                  ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022F12EC E59F2244 ldr     r2,=2324CC0h              //r2 = 02324CC0
022F12F0 E3E01000 mvn     r1,0h                     //r1 = 0xFFFFFFFF
022F12F4 E5820000 str     r0,[r2]                   //[02324CC0] = PtrAllocatedTable
022F12F8 E5920004 ldr     r0,[r2,4h]                //r0 = [0x2324CC0 + 4]
022F12FC E1550001 cmp     r5,r1                     //
022F1300 E1C050B0 strh    r5,[r0]                   //[r0] = levelid
if( levelid != 0xFFFFFFFF)
    022F1304 1A000010 bne     22F134Ch              ///022F134C v
022F1308 E5920000 ldr     r0,[r2]                   //r0 = [02324CC0]
022F130C E59F1228 ldr     r1,=2320CD8h              //r1 = Ptr Ground map select default values table
022F1310 EBFFEA40 bl      22EBC18h                  ///fun_022EBC18 Loads correct constants for the level id!
022F1314 E59F021C ldr     r0,=2324CC0h              //ptr_tbl_2324CC0h
022F1318 E3E01000 mvn     r1,0h                     //r1 = 0xFFFFFFFF
022F131C E5900000 ldr     r0,[r0]                   //r0 = [ptr_tbl_2324CC0h]
022F1320 EBFFEBD5 bl      22EC27Ch                  /// LoadBGTileSets (r0 = struct ptr, r1 = level id)
022F1324 E59F020C ldr     r0,=2324CC0h              //ptr_tbl_2324CC0h
022F1328 E8900006 ldmia   [r0],r1,r2                //r1 = [ptr_tbl_2324CC0h], r2 = [ptr_tbl_2324CC0h,4h]
022F132C E2820004 add     r0,r2,4h                  //r0 = [ptr_tbl_2324CC0h,4h] + 4
022F1330 EBFFFA89 bl      22EFD5Ch                  ///fun_022EFD5C
022F1334 E59F01FC ldr     r0,=2324CC0h              //ptr_tbl_2324CC0h
022F1338 E5901004 ldr     r1,[r0,4h]                //r1 = [ptr_tbl_2324CC0h,4h]
022F133C E2810020 add     r0,r1,20h                 //r0 = [ptr_tbl_2324CC0h,4h] + 32
022F1340 E2811004 add     r1,r1,4h                  //r1 = [ptr_tbl_2324CC0h,4h] + 4
022F1344 EBFFFC1E bl      22F03C4h                  ///fun_022F03C4 //Copy constructor maybe??
{
    ///fun_022F03C4
    022F03C4 E5913004 ldr     r3,[r1,4h]
    022F03C8 E5912008 ldr     r2,[r1,8h]
    022F03CC E5803004 str     r3,[r0,4h]
    022F03D0 E5802008 str     r2,[r0,8h]
    022F03D4 E591300C ldr     r3,[r1,0Ch]
    022F03D8 E5912010 ldr     r2,[r1,10h]
    022F03DC E580300C str     r3,[r0,0Ch]
    022F03E0 E5802010 str     r2,[r0,10h]
    022F03E4 E5912014 ldr     r2,[r1,14h]
    022F03E8 E5911018 ldr     r1,[r1,18h]
    022F03EC E5802014 str     r2,[r0,14h]
    022F03F0 E5801018 str     r1,[r0,18h]
    022F03F4 E12FFF1E bx      r14
}
022F1348 EA000066 b       22F14E8h              ///022F14E8 v
///022F134C -  levelid != 0xFFFFFFFF
022F134C E3A0000C mov     r0,0Ch                //r0 = 12
022F1350 E1630085 smulbb  r3,r5,r0              //r3 = levelid * 12
022F1354 E59F41E4 ldr     r4,=20A5488h          //Level list beg
022F1358 E19400F3 ldrsh   r0,[r4,r3]            //r0 = unk1 of levelentry
022F135C E0844003 add     r4,r4,r3              //r4 =  Levellistbeg + (levelid * 12)
022F1360 E2800001 add     r0,r0,1h              //r0 = unk1 of levelentry + 1
022F1364 E350000C cmp     r0,0Ch
if( (unk1 of levelentry + 1) <= 12 )
    022F1368 908FF100 addls   r15,r15,r0,lsl 2h
022F136C EA00003E b       22F146Ch              ///022F146C //level entry first value > 12
022F1370 EA000033 b       22F1444h              ///022F1444 //level entry first value == 0
022F1374 EA00003C b       22F146Ch              ///022F146C //level entry first value == 1
022F1378 EA00003B b       22F146Ch              ///022F146C //level entry first value == 2
022F137C EA00003A b       22F146Ch              ///022F146C //level entry first value == 3
022F1380 EA000039 b       22F146Ch              ///022F146C //level entry first value == 4
022F1384 EA000038 b       22F146Ch              ///022F146C //level entry first value == 5
022F1388 EA000005 b       22F13A4h              ///022F13A4 //level entry first value == 6
022F138C EA000004 b       22F13A4h              ///022F13A4 //level entry first value == 7
022F1390 EA000003 b       22F13A4h              ///022F13A4 //level entry first value == 8
022F1394 EA000002 b       22F13A4h              ///022F13A4 //level entry first value == 9
022F1398 EA000001 b       22F13A4h              ///022F13A4 //level entry first value == 10
022F139C EA000008 b       22F13C4h              ///022F13C4 //level entry first value == 11
022F13A0 EA000007 b       22F13C4h              ///022F13C4 //level entry first value == 12
///022F13A4 - level entry first value == 6,7,8,9,10
022F13A4 E5920000 ldr     r0,[r2]               //r0 = [02324CC0]
022F13A8 E59F1194 ldr     r1,=2320CF4h          //r1 = PtrSecEntry Ground map select default values
022F13AC EBFFEA19 bl      22EBC18h              ///fun_022EBC18 Loads correct constants for the level id!
022F13B0 E3A00000 mov     r0,0h                 //r0 = 0    
022F13B4 EBFFF860 bl      22EF53Ch              ///fun_022EF53C
{
    ///fun_022EF53C
    022EF53C E92D4010 push    r4,r14
    022EF540 E1A04000 mov     r4,r0
    022EF544 EBF466F5 bl      2009120h
    022EF548 E3A00002 mov     r0,2h
    022EF54C E1A01000 mov     r1,r0
    022EF550 E1A02004 mov     r2,r4
    022EF554 EBF467BE bl      2009454h
    022EF558 E1A02004 mov     r2,r4
    022EF55C E3A00003 mov     r0,3h
    022EF560 E3A01002 mov     r1,2h
    022EF564 EBF467BA bl      2009454h
    022EF568 E1A02004 mov     r2,r4
    022EF56C E3A00004 mov     r0,4h
    022EF570 E3A01002 mov     r1,2h
    022EF574 EBF467B6 bl      2009454h
    022EF578 E1A01004 mov     r1,r4
    022EF57C E3A00002 mov     r0,2h
    022EF580 EBF4670A bl      20091B0h
    022EF584 E1A01004 mov     r1,r4
    022EF588 E3A00003 mov     r0,3h
    022EF58C EBF46707 bl      20091B0h
    022EF590 E8BD8010 pop     r4,r15
}
022F13B8 E3A00002 mov     r0,2h                 //r0 = 2
022F13BC EB0003CC bl      22F22F4h              ///fun_022F22F4 - Seems to do some allocation and call the loading of the tileset??
022F13C0 EA000031 b       22F148Ch              ///022F148C v
///022F13C4 - level entry first value == 11,12
022F13C4 E59F617C ldr     r6,=2320D2Ch          //r6 = 02320D2C (ptr to beg of table with 81, 8 bytes data entries) //PtrTblCurEntry
022F13C8 EA000011 b       22F1414h              ///022F1414 v

///
///022F13CC - LOOP_Beg
///
022F13CC E1500005 cmp     r0,r5
if( curentry != levelid )
    022F13D0 1A00000E bne     22F1410h          ///022F1410 v
022F13D4 E59F015C ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
022F13D8 E5900000 ldr     r0,[r0]               //r0 = [ptr_tbl_2324CC0h]
022F13DC EBF43F69 bl      2001188h              ///MemFree(r0 = ptrbufftofree)
022F13E0 E1D410F0 ldrsh   r1,[r4]               //r1 = LevelEntryMapTy
022F13E4 E59F014C ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
022F13E8 E3A03000 mov     r3,0h                 //r3 = 0
022F13EC E5803000 str     r3,[r0]               //[ptr_tbl_2324CC0h] = 0
022F13F0 E351000B cmp     r1,0Bh
if( LevelEntryMapTy == 11 )
    022F13F4 03A03001 moveq   r3,1h             //r3 = 1
022F13F8 E5962004 ldr     r2,[r6,4h]            //r2 = [PtrTblCurEntry,4h] 
022F13FC E1A00005 mov     r0,r5                 //r0 = levelid
022F1400 E2861002 add     r1,r6,2h              //r1 = PtrTblCurEntry + 2
022F1404 E20330FF and     r3,r3,0FFh            //r3 = r3 & 0xFF
022F1408 EB000053 bl      22F155Ch              ///fun_022F155C GroundMap_SelectDungeon
{
    /*
        fun_022F155C GroundMap_SelectDungeon 22F155Ch
    */
    022F155C E92D41F0 push    r4-r8,r14
    022F1560 E24DD008 sub     r13,r13,8h
    022F1564 E1A08000 mov     r8,r0                 //r8 = levelid
    022F1568 E1A07001 mov     r7,r1                 //r7 = PtrTblCurEntry + 2
    022F156C E1A06002 mov     r6,r2                 //r6 = [PtrTblCurEntry,4h]
    022F1570 E59F1178 ldr     r1,=2320FE8h          //"GroundMap SelectDungeon %3d"    
    022F1574 E1A02008 mov     r2,r8                 //r2 = levelid
    022F1578 E3A00001 mov     r0,1h                 //r1 = 1
    022F157C E1A05003 mov     r5,r3                 //r5 = bIsLevelEntryUnk1_11
    022F1580 EBF46B2E bl      200C240h              //DebugPrint
    022F1584 EBFFFF2E bl      22F1244h              //022F1244 - Seems to initialise / prepare the level to switch!
    022F1588 E3A00FBD mov     r0,2F4h               //r0 = 756
    022F158C E3A01006 mov     r1,6h                 //r1 = 6
    022F1590 EBF43EF6 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
    022F1594 E59F2158 ldr     r2,=2324CC0h          //ptr_tbl_2324CC0h
    022F1598 E3E01000 mvn     r1,0h                 //r1 = 0xFFFFFFFF
    022F159C E5820000 str     r0,[r2]               //[ptr_tbl_2324CC0h] = ptrallocatedbuffer
    022F15A0 E5920004 ldr     r0,[r2,4h]            //r0 = [ptr_tbl_2324CC0h,4h]
    022F15A4 E1580001 cmp     r8,r1                 
    022F15A8 E1C080B0 strh    r8,[r0]               //[r0] = levelid
    if( levelid != 0xFFFFFFFF )
    {
        022F15AC 15D70000 ldrneb  r0,[r7]           //r0 = [PtrTblCurEntry + 2]
        022F15B0 135000FF cmpne   r0,0FFh
        if( r0 != 0xFF )
            022F15B4 1A000011 bne     22F1600h      //022F1600 v
    }
    022F15B8 E59F0134 ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
    022F15BC E59F1134 ldr     r1,=2320CD8h          //ptr_tbl_2320CD8h
    022F15C0 E5900000 ldr     r0,[r0]               //r0 = [ptr_tbl_2324CC0h]
    022F15C4 EBFFE993 bl      22EBC18h              ///fun_022EBC18 Loads correct constants for the level id!
    022F15C8 E59F0124 ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
    022F15CC E3E01000 mvn     r1,0h                 //r1 = 0xFFFFFFFF
    022F15D0 E5900000 ldr     r0,[r0]               //r0 = [ptr_tbl_2324CC0h]
    022F15D4 EBFFEB28 bl      22EC27Ch              ///LoadBGTileSets (r0 = struct ptr, r1 = level id)
    022F15D8 E59F0114 ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
    022F15DC E8900006 ldmia   [r0],r1,r2            //r1 = [ptr_tbl_2324CC0h], r2 = [ptr_tbl_2324CC0h,4h]
    022F15E0 E2820004 add     r0,r2,4h              //r0 = [ptr_tbl_2324CC0h,4h] + 4
    022F15E4 EBFFF9DC bl      22EFD5Ch              ///fun_022EFD5C Seems to do level specific stuff?
    022F15E8 E59F0104 ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
    022F15EC E5901004 ldr     r1,[r0,4h]            //r1 = [ptr_tbl_2324CC0h,4h]
    022F15F0 E2810020 add     r0,r1,20h             //r0 = [ptr_tbl_2324CC0h,4h] + 32
    022F15F4 E2811004 add     r1,r1,4h              //r1 = [ptr_tbl_2324CC0h,4h] + 4
    022F15F8 EBFFFB71 bl      22F03C4h              ///fun_022F03C4 //Copy constructor maybe??
    022F15FC EA000035 b       22F16D8h              ///022F16D8 v
    ///022F1600 - [PtrTblCurEntry + 2] != 0xFF
    022F1600 E3A0000C mov     r0,0Ch                //r0 = 12
    022F1604 E1610088 smulbb  r1,r8,r0              //r1 = levelid * 12
    022F1608 E59F20EC ldr     r2,=20A5488h          //Level list beg
    022F160C E19200F1 ldrsh   r0,[r2,r1]            //r0 = [20A5488h,(levelid * 12)]
    022F1610 E0824001 add     r4,r2,r1              //r4 = 20A5488h + (levelid * 12)
    022F1614 E350000A cmp     r0,0Ah                
    022F1618 1350000B cmpne   r0,0Bh
    if( LvlEntryUnk1 == 10 || LvlEntryUnk1 == 11 )
        022F161C 0A000008 beq     22F1644h          ///022F1644 v
    022F1620 E59F10D8 ldr     r1,=2320CA8h          //ptr_tbl_2320CA8h
    022F1624 E28D0000 add     r0,r13,0h             //r0  = r13
    022F1628 E591300C ldr     r3,[r1,0Ch]           //r3  = [ptr_tbl_2320CA8h,0Ch]
    022F162C E591C008 ldr     r12,[r1,8h]           //r12 = [ptr_tbl_2320CA8h,8h]
    022F1630 E59F10CC ldr     r1,=2321004h          //"map type error %d"
    022F1634 E1A02008 mov     r2,r8                 //r2  = levelid
    022F1638 E58DC000 str     r12,[r13]             //[r13] = r12 
    022F163C E58D3004 str     r3,[r13,4h]           //[r13,4h] = r3
    022F1640 EBF46B05 bl      200C25Ch              ///fun_0200C25C Fatal error?
    ///022F1644
    022F1644 E59F00A8 ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
    022F1648 E59F10B8 ldr     r1,=2320D10h          //Ground map select default values  When level entry first value == 11,12
    022F164C E5900000 ldr     r0,[r0]               //r0 = [ptr_tbl_2324CC0h]
    022F1650 EBFFE970 bl      22EBC18h              ///fun_022EBC18 Loads correct constants for the level id!
    022F1654 E3A00000 mov     r0,0h                 //r0 = 0
    022F1658 EBFFF7B7 bl      22EF53Ch              ///fun_022EF53C
    {
        ///fun_022EF53C
        022EF53C E92D4010 push    r4,r14            
        022EF540 E1A04000 mov     r4,r0             //r4 = 0
        022EF544 EBF466F5 bl      2009120h
        {
            02009120 E3500000 cmp     r0,0h
            if( r0 != 0 )
                02009124 1A000009 bne     2009150h          ///02009150 v
            02009128 E59F2054 ldr     r2,=22A37CCh          //ptr_struct_22A37CCh
            0200912C E3A03001 mov     r3,1h                 //r3 = 1
            02009130 E7823200 str     r3,[r2,r0,lsl 4h]     //[ptr_struct_22A37CCh, 0 * 16]  = 1  
            02009134 E59F104C ldr     r1,=22A37D0h          //ptr_struct_22A37D0h
            02009138 E3A03000 mov     r3,0h                 //r3 = 0
            0200913C E7813200 str     r3,[r1,r0,lsl 4h]     //[ptr_struct_22A37D0h, 0 * 16] = 0
            02009140 E59F2044 ldr     r2,=22A37D4h          //r2 = ptr_struct_22A37D4h
            02009144 E3A03002 mov     r3,2h                 //r3 = 2
            02009148 E59F1040 ldr     r1,=22A37D8h          //r1 = ptr_struct_22A37D8h
            0200914C EA000008 b       2009174h              ///02009174 v
            ///02009150
            02009150 E59F202C ldr     r2,=22A37CCh          //ptr_struct_22A37CCh
            02009154 E3A03000 mov     r3,0h                 //r3 = 0
            02009158 E7823200 str     r3,[r2,r0,lsl 4h]
            0200915C E59F1024 ldr     r1,=22A37D0h
            02009160 E3A03001 mov     r3,1h
            02009164 E7813200 str     r3,[r1,r0,lsl 4h]
            02009168 E59F201C ldr     r2,=22A37D4h
            0200916C E59F101C ldr     r1,=22A37D8h
            02009170 E3A03002 mov     r3,2h
            ///02009174
            02009174 E7823200 str     r3,[r2,r0,lsl 4h]     //[ptr_struct_22A37D4h,0 * 16] = r3
            02009178 E3A02003 mov     r2,3h                 //r2 = 3
            0200917C E7812200 str     r2,[r1,r0,lsl 4h]     //[ptr_struct_22A37D8h,0 * 16] = 3
            02009180 E12FFF1E bx      r14
            02009184 022A37CC 
            02009188 022A37D0
            0200918C 022A37D4 
            02009190 022A37D8
        }
        022EF548 E3A00002 mov     r0,2h             //r0 = 2
        022EF54C E1A01000 mov     r1,r0             //r1 = 2
        022EF550 E1A02004 mov     r2,r4             //r2 = 0
        022EF554 EBF467BE bl      2009454h
        {
            02009454 E59FC00C ldr     r12,=22A37B6h     //r12 = 022A37B6
            02009458 E3A0300A mov     r3,0Ah            //r3  = 10
            0200945C E023C392 mla     r3,r2,r3,r12      //r3 = 0 * 10 + 0x22A37B6
            02009460 E7C01003 strb    r1,[r0,r3]
            02009464 E12FFF1E bx      r14
            02009468 022A37B6 
        }
        022EF558 E1A02004 mov     r2,r4             //r2 = 0
        022EF55C E3A00003 mov     r0,3h             //r0 = 3
        022EF560 E3A01002 mov     r1,2h             //r1 = 1
        022EF564 EBF467BA bl      2009454h
        022EF568 E1A02004 mov     r2,r4
        022EF56C E3A00004 mov     r0,4h
        022EF570 E3A01002 mov     r1,2h
        022EF574 EBF467B6 bl      2009454h
        022EF578 E1A01004 mov     r1,r4
        022EF57C E3A00002 mov     r0,2h
        022EF580 EBF4670A bl      20091B0h
        022EF584 E1A01004 mov     r1,r4
        022EF588 E3A00003 mov     r0,3h
        022EF58C EBF46707 bl      20091B0h
        022EF590 E8BD8010 pop     r4,r15
    }
    022F165C E3A00001 mov     r0,1h             //r0 = 1
    022F1660 EB000323 bl      22F22F4h          ///fun_022F22F4 - Seems to do some allocation and call the loading of the tileset??
    022F1664 E3550000 cmp     r5,0h
    if( r5 == 0 )
        022F1668 0A000006 beq     22F1688h      ///022F1688 v
    022F166C E59F0080 ldr     r0,=2324CC0h
    022F1670 E1D410F4 ldrsh   r1,[r4,4h]
    022F1674 E5900000 ldr     r0,[r0]
    022F1678 E1A02007 mov     r2,r7             //r2 = PtrTblCurEntry + 2
    022F167C E1A03006 mov     r3,r6
    022F1680 EBFFEEEF bl      22ED244h
    022F1684 EA000005 b       22F16A0h
    ///022F1688
    022F1688 E59F0064 ldr     r0,=2324CC0h
    022F168C E1D410F4 ldrsh   r1,[r4,4h]
    022F1690 E5900000 ldr     r0,[r0]
    022F1694 E1A02007 mov     r2,r7             //r2 = PtrTblCurEntry + 2
    022F1698 E1A03006 mov     r3,r6
    022F169C EBFFEDA0 bl      22ECD24h
    022F16A0 E59F004C ldr     r0,=2324CC0h
    022F16A4 E8900006 ldmia   [r0],r1,r2
    022F16A8 E2820004 add     r0,r2,4h
    022F16AC EBFFF9AA bl      22EFD5Ch
    022F16B0 E59F003C ldr     r0,=2324CC0h
    022F16B4 E5901004 ldr     r1,[r0,4h]
    022F16B8 E2810020 add     r0,r1,20h
    022F16BC E2811004 add     r1,r1,4h
    022F16C0 EBFFFB3F bl      22F03C4h
    022F16C4 E59F0028 ldr     r0,=2324CC0h
    022F16C8 E59F203C ldr     r2,=2320CB8h
    022F16CC E5900000 ldr     r0,[r0]
    022F16D0 E3A01000 mov     r1,0h
    022F16D4 EBFFF6FE bl      22EF2D4h
    ///022F16D8 
    022F16D8 E59F0014 ldr     r0,=2324CC0h
    022F16DC E3A01001 mov     r1,1h
    022F16E0 E5900004 ldr     r0,[r0,4h]
    022F16E4 E5C01002 strb    r1,[r0,2h]
    022F16E8 E28DD008 add     r13,r13,8h
    022F16EC E8BD81F0 pop     r4-r8,r15
    022F16F0 02320FE8
    022F16F4 02324CC0
    022F16F8 02320CD8
    022F16FC 020A5488
    022F1700 02320CA8
    022F1704 02321004
    022F1708 02320D10
    022F170C 02320CB8
}
022F140C EA000046 b       22F152Ch              ///022F152C - Return v
///022F1410
022F1410 E2866008 add     r6,r6,8h              //r6 = PtrTblCurEntry + 8
///022F1414
022F1414 E1D600F0 ldrsh   r0,[r6]               //r0 = [PtrTblCurEntry] //curentry
022F1418 E1500001 cmp     r0,r1
if( curentry != 0 )
    022F141C 1AFFFFEA bne     22F13CCh          ///022F13CC - LOOP_Beg ^

022F1420 E59F0110 ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
022F1424 E59F1120 ldr     r1,=2320D10h          //ptr_tbl_2320D10h
022F1428 E5900000 ldr     r0,[r0]               //r0 = [ptr_tbl_2324CC0h] //
022F142C EBFFE9F9 bl      22EBC18h              ///fun_022EBC18 Loads correct constants for the level id!
022F1430 E3A00000 mov     r0,0h                 //r0 = 0
022F1434 EBFFF840 bl      22EF53Ch              ///fun_022EF53C
{
    ///fun_022EF53C
    022EF53C E92D4010 push    r4,r14
    022EF540 E1A04000 mov     r4,r0
    022EF544 EBF466F5 bl      2009120h
    022EF548 E3A00002 mov     r0,2h
    022EF54C E1A01000 mov     r1,r0
    022EF550 E1A02004 mov     r2,r4
    022EF554 EBF467BE bl      2009454h
    022EF558 E1A02004 mov     r2,r4
    022EF55C E3A00003 mov     r0,3h
    022EF560 E3A01002 mov     r1,2h
    022EF564 EBF467BA bl      2009454h
    022EF568 E1A02004 mov     r2,r4
    022EF56C E3A00004 mov     r0,4h
    022EF570 E3A01002 mov     r1,2h
    022EF574 EBF467B6 bl      2009454h
    022EF578 E1A01004 mov     r1,r4
    022EF57C E3A00002 mov     r0,2h
    022EF580 EBF4670A bl      20091B0h
    022EF584 E1A01004 mov     r1,r4
    022EF588 E3A00003 mov     r0,3h
    022EF58C EBF46707 bl      20091B0h
    022EF590 E8BD8010 pop     r4,r15
}
022F1438 E3A00001 mov     r0,1h
022F143C EB0003AC bl      22F22F4h
022F1440 EA000011 b       22F148Ch              ///022F148C v
///022F1444 - level entry first value == 0
022F1444 E59F0104 ldr     r0,=2320CA8h          //ptr_tbl_2320CA8h
022F1448 E1D430F0 ldrsh   r3,[r4]               //r3  = LevelEntryMapTy
022F144C E590C01C ldr     r12,[r0,1Ch]          //r12 = [ptr_tbl_2320CA8h,1Ch]
022F1450 E590E018 ldr     r14,[r0,18h]          //r14 = [ptr_tbl_2320CA8h,18h]
022F1454 E59F10F8 ldr     r1,=2320FCCh          //"select map type error %d %d"
022F1458 E28D0000 add     r0,r13,0h
022F145C E1A02005 mov     r2,r5
022F1460 E58DE000 str     r14,[r13]
022F1464 E58DC004 str     r12,[r13,4h]
022F1468 EBF46B7B bl      200C25Ch              ///fatal error?
///022F146C - level entry first value == 1,2,3,4,5 || level entry first value > 12
022F146C E59F00C4 ldr     r0,=2324CC0h
022F1470 E59F10C4 ldr     r1,=2320CD8h
022F1474 E5900000 ldr     r0,[r0]
022F1478 EBFFE9E6 bl      22EBC18h              ///fun_022EBC18 Loads correct constants for the level id!
022F147C E3A00000 mov     r0,0h                 //r0 = 0
022F1480 EBFFF82D bl      22EF53Ch              ///fun_022EF53C
{
    ///fun_022EF53C
    022EF53C E92D4010 push    r4,r14
    022EF540 E1A04000 mov     r4,r0
    022EF544 EBF466F5 bl      2009120h
    022EF548 E3A00002 mov     r0,2h
    022EF54C E1A01000 mov     r1,r0
    022EF550 E1A02004 mov     r2,r4
    022EF554 EBF467BE bl      2009454h
    022EF558 E1A02004 mov     r2,r4
    022EF55C E3A00003 mov     r0,3h
    022EF560 E3A01002 mov     r1,2h
    022EF564 EBF467BA bl      2009454h
    022EF568 E1A02004 mov     r2,r4
    022EF56C E3A00004 mov     r0,4h
    022EF570 E3A01002 mov     r1,2h
    022EF574 EBF467B6 bl      2009454h
    022EF578 E1A01004 mov     r1,r4
    022EF57C E3A00002 mov     r0,2h
    022EF580 EBF4670A bl      20091B0h
    022EF584 E1A01004 mov     r1,r4
    022EF588 E3A00003 mov     r0,3h
    022EF58C EBF46707 bl      20091B0h
    022EF590 E8BD8010 pop     r4,r15
}
022F1484 E3A00001 mov     r0,1h
022F1488 EB000399 bl      22F22F4h              ///fun_022F22F4 - Seems to do some allocation and call the loading of the tileset??
///022F148C
022F148C E59F00A4 ldr     r0,=2324CC0h
022F1490 E1D410F4 ldrsh   r1,[r4,4h]            //r1 = levelentry_mapid
022F1494 E5900000 ldr     r0,[r0]               //r0 = [ptr_tbl_2324CC0h]
022F1498 EBFFEB77 bl      22EC27Ch              ///022EC27C LoadBGTileSets
022F149C E59F0094 ldr     r0,=2324CC0h
022F14A0 E8900006 ldmia   [r0],r1,r2            //r1 = [r0], r2 = [r0,4h] 
022F14A4 E2820004 add     r0,r2,4h              //r0 = r2 + 4 (r0 = ptr to ptr to 0x210CA44)
022F14A8 EBFFFA2B bl      22EFD5Ch              ///fun_022EFD5C
022F14AC E59F0084 ldr     r0,=2324CC0h
022F14B0 E5901004 ldr     r1,[r0,4h]
022F14B4 E2810020 add     r0,r1,20h
022F14B8 E2811004 add     r1,r1,4h
022F14BC EBFFFBC0 bl      22F03C4h              ///fun_022F03C4 //Copy constructor maybe??
022F14C0 E1D400F6 ldrsh   r0,[r4,6h]
022F14C4 E3E01000 mvn     r1,0h                 //r1 = 0xFFFFFFFF
022F14C8 E1500001 cmp     r0,r1
if( r0 != 0xFFFFFFFF )
    022F14CC 0A000000 beq     22F14D4h          ///022F14D4 v
022F14D0 EB000433 bl      22F25A4h              ///fun_022F25A4 Ground Weather Select
///022F14D4
022F14D4 E59F005C ldr     r0,=2324CC0h          //ptr_tbl_2324CC0h
022F14D8 E59F2078 ldr     r2,=2320CA8h          //ptr_tbl_2320CA8h   
022F14DC E5900000 ldr     r0,[r0]               //r0 = [ptr_tbl_2324CC0h]
022F14E0 E3A01000 mov     r1,0h                 //r1 = 0
022F14E4 EBFFF77A bl      22EF2D4h
{
    022EF2D4 E5923000 ldr     r3,[r2]           //r3 = [ptr_tbl_2320CA8h]
    022EF2D8 E5922004 ldr     r2,[r2,4h]        //r2 = [ptr_tbl_2320CA8h,4h]    
    022EF2DC E0800181 add     r0,r0,r1,lsl 3h   //r0 = [ptr_tbl_2324CC0h] + (0 * 8)
    022EF2E0 E5803200 str     r3,[r0,200h]      //[r0,200h] = [ptr_tbl_2320CA8h]
    022EF2E4 E5802204 str     r2,[r0,204h]      //[r0,204h] = [ptr_tbl_2320CA8h,4h] 
    022EF2E8 E12FFF1E bx      r14
}
///022F14E8
022F14E8 E59F0048 ldr     r0,=2324CC0h
022F14EC E3A01001 mov     r1,1h
022F14F0 E5900004 ldr     r0,[r0,4h]
022F14F4 E5C01002 strb    r1,[r0,2h]
022F14F8 EB007FD7 bl      231145Ch
022F14FC E3500000 cmp     r0,0h
022F1500 0A000003 beq     22F1514h
022F1504 E3A00000 mov     r0,0h
022F1508 E1A01000 mov     r1,r0
022F150C EB006F81 bl      230D318h
022F1510 EA000005 b       22F152Ch
022F1514 EB008110 bl      231195Ch
022F1518 E3500000 cmp     r0,0h
022F151C 0A000002 beq     22F152Ch
022F1520 E3A00000 mov     r0,0h
022F1524 E1A01000 mov     r1,r0
022F1528 EB006F7A bl      230D318h
///022F152C - Return
022F152C E28DD008 add     r13,r13,8h
022F1530 E8BD8070 pop     r4-r6,r15
022F1534 02320FB4 
022F1538 02324CC0 
022F153C 02320CD8 
022F1540 020A5488 
022F1544 02320CF4 
022F1548 02320D2C 
022F154C 02320D10 
022F1550 02320CA8 
022F1554 02320FCC 
022F1558 02320CA8 


/*
    fun_022F22F4 22F22F4h
        This function seems to refer a lot of the map data, including the static memory location 2324CC8h
*/
022F22F4 E92D4010 push    r4,r14
022F22F8 E59F1298 ldr     r1,=2324CC8h              ///CurMapDataPtr?
022F22FC E1A04000 mov     r4,r0
022F2300 E5910000 ldr     r0,[r1]
022F2304 E5902004 ldr     r2,[r0,4h]
022F2308 E3520003 cmp     r2,3h
if( r2 != 3 )
    022F230C 1A000002 bne     22F231Ch              ///022F231C v
022F2310 E5910004 ldr     r0,[r1,4h]
022F2314 EBFFEF87 bl      22EE138h                  ///022EE138
{
    022EE138 E59FC000 ldr     r12,=20163E4h
    022EE13C E12FFF1C bx      r12
    {
        020163E4 E92D4008 push    r3,r14
        020163E8 E59F1024 ldr     r1,=20AF760h
        020163EC E3A00014 mov     r0,14h
        020163F0 E5912000 ldr     r2,[r1]
        020163F4 E59F101C ldr     r1,=209902Ch
        020163F8 E0000092 mul     r0,r2,r0
        020163FC E7910000 ldr     r0,[r1,r0]
        02016400 E3500000 cmp     r0,0h
        if( r0 == 0 )
            02016404 0A000000 beq     201640Ch
        02016408 E12FFF30 blx     r0
        0201640C EBFFFC42 bl      201551Ch
        {
            0201551C E92D4008 push    r3,r14
            02015520 E59F0044 ldr     r0,=20AF710h
            02015524 E5900000 ldr     r0,[r0]
            02015528 E3500000 cmp     r0,0h
            0201552C 08BD8008 popeq   r3,r15
            02015530 E5D0003C ldrb    r0,[r0,3Ch]
            02015534 E3500000 cmp     r0,0h
            02015538 0A000000 beq     2015540h
            0201553C EB00025C bl      2015EB4h
            {
                02015EB4 E92D4008 push    r3,r14
                02015EB8 E59F0024 ldr     r0,=20AF710h
                02015EBC E5900000 ldr     r0,[r0]
                02015EC0 E5900004 ldr     r0,[r0,4h]
                02015EC4 E3500000 cmp     r0,0h
                02015EC8 08BD8008 popeq   r3,r15
                02015ECC EBFFD540 bl      200B3D4h
                02015ED0 E59F000C ldr     r0,=20AF710h
                02015ED4 E5900000 ldr     r0,[r0]
                02015ED8 E5900004 ldr     r0,[r0,4h]
                02015EDC EBFFD513 bl      200B330h
                {
                    0200B330 E3A01001 mov     r1,1h
                    0200B334 E5C01001 strb    r1,[r0,1h]
                    0200B338 E12FFF1E bx      r14
                }
                02015EE0 E8BD8008 pop     r3,r15
                02015EE4 020AF710 andeq   r15,r10,400000h
            }
            02015540 E59F0024 ldr     r0,=20AF710h
            02015544 E5900000 ldr     r0,[r0]
            02015548 E280001C add     r0,r0,1Ch
            0201554C EBFFCDA0 bl      2008BD4h
            02015550 E59F0014 ldr     r0,=20AF710h
            02015554 E5900000 ldr     r0,[r0]
            02015558 EBFFAF0A bl      2001188h              ///MemFree(r0 = ptrbufftofree)
            0201555C E59F0008 ldr     r0,=20AF710h
            02015560 E3A01000 mov     r1,0h
            02015564 E5801000 str     r1,[r0]
            02015568 E8BD8008 pop     r3,r15
            0201556C 020AF710
        }
        02016410 E8BD8008 pop     r3,r15
        02016414 020AF760
        02016418 0209902C
    }
    022EE140 020163E4 
}
022F2318 EA000009 b       22F2344h                  ///022F2344 v
///022F231C
022F231C E2420004 sub     r0,r2,4h
022F2320 E3500003 cmp     r0,3h
if( r0 <= 3 )
    022F2324 8A000002 bhi     22F2334h              ///022F2334 v
022F2328 E5910004 ldr     r0,[r1,4h]
022F232C EBFFEFB7 bl      22EE210h
022F2330 EA000003 b       22F2344h                  ///022F2344 v
///022F2334
022F2334 E3520008 cmp     r2,8h
if( r2 != 8 )
    022F2338 1A000001 bne     22F2344h              ///022F2344 v
022F233C E5910004 ldr     r0,[r1,4h]
022F2340 EBFFEFE2 bl      22EE2D0h
///022F2344
022F2344 E59F024C ldr     r0,=2324CC8h              ///CurMapDataPtr?
022F2348 E3A03000 mov     r3,0h
022F234C E5901000 ldr     r1,[r0]
022F2350 E2432001 sub     r2,r3,1h
022F2354 E5C13009 strb    r3,[r1,9h]
022F2358 E5901000 ldr     r1,[r0]
022F235C E5C13008 strb    r3,[r1,8h]
022F2360 E5901000 ldr     r1,[r0]
022F2364 E5814004 str     r4,[r1,4h]
022F2368 E5901000 ldr     r1,[r0]
022F236C E1C120B0 strh    r2,[r1]
022F2370 E5900004 ldr     r0,[r0,4h]
022F2374 E3500000 cmp     r0,0h
022F2378 0A000007 beq     22F239Ch
022F237C EBFFE6CA bl      22EBEACh
022F2380 E59F0210 ldr     r0,=2324CC8h              ///CurMapDataPtr?
022F2384 E5900004 ldr     r0,[r0,4h]
022F2388 EBF43B7E bl      2001188h                  ///MemFree(r0 = ptrbufftofree)    
022F238C E59F0204 ldr     r0,=2324CC8h              ///CurMapDataPtr?
022F2390 E3A01000 mov     r1,0h
022F2394 E5801004 str     r1,[r0,4h]
022F2398 EB0001C9 bl      22F2AC4h
022F239C E3540000 cmp     r4,0h
022F23A0 08BD8010 popeq   r4,r15
022F23A4 E3A00FBD mov     r0,2F4h
022F23A8 E3A01006 mov     r1,6h
022F23AC EBF43B6F bl      2001170h                  ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022F23B0 E59F11E0 ldr     r1,=2324CC8h              ///CurMapDataPtr?
022F23B4 E3540003 cmp     r4,3h
022F23B8 E5810004 str     r0,[r1,4h]
022F23BC 1A00000D bne     22F23F8h
022F23C0 EBFFEF40 bl      22EE0C8h
022F23C4 E3A00002 mov     r0,2h
022F23C8 E1A01000 mov     r1,r0
022F23CC E3A02000 mov     r2,0h
022F23D0 EBF45C1F bl      2009454h
022F23D4 E3A00003 mov     r0,3h
022F23D8 E3A01002 mov     r1,2h
022F23DC E3A02000 mov     r2,0h
022F23E0 EBF45C1B bl      2009454h
022F23E4 E3A00004 mov     r0,4h
022F23E8 E3A01002 mov     r1,2h
022F23EC E3A02000 mov     r2,0h
022F23F0 EBF45C17 bl      2009454h
022F23F4 EA000062 b       22F2584h
022F23F8 E3540004 cmp     r4,4h
022F23FC 1A00000E bne     22F243Ch
022F2400 E3A01002 mov     r1,2h
022F2404 EBFFEF63 bl      22EE198h
022F2408 E3A00002 mov     r0,2h
022F240C E1A01000 mov     r1,r0
022F2410 E3A02000 mov     r2,0h
022F2414 EBF45C0E bl      2009454h
022F2418 E3A00003 mov     r0,3h
022F241C E3A01002 mov     r1,2h
022F2420 E3A02000 mov     r2,0h
022F2424 EBF45C0A bl      2009454h
022F2428 E3A00004 mov     r0,4h
022F242C E3A01002 mov     r1,2h
022F2430 E3A02000 mov     r2,0h
022F2434 EBF45C06 bl      2009454h
022F2438 EA000051 b       22F2584h
022F243C E3540005 cmp     r4,5h
022F2440 1A00000E bne     22F2480h
022F2444 E3A01003 mov     r1,3h
022F2448 EBFFEF52 bl      22EE198h
022F244C E3A00002 mov     r0,2h
022F2450 E1A01000 mov     r1,r0
022F2454 E3A02000 mov     r2,0h
022F2458 EBF45BFD bl      2009454h
022F245C E3A00003 mov     r0,3h
022F2460 E3A01002 mov     r1,2h
022F2464 E3A02000 mov     r2,0h
022F2468 EBF45BF9 bl      2009454h
022F246C E3A00004 mov     r0,4h
022F2470 E3A01002 mov     r1,2h
022F2474 E3A02000 mov     r2,0h
022F2478 EBF45BF5 bl      2009454h
022F247C EA000040 b       22F2584h
022F2480 E3540006 cmp     r4,6h
022F2484 1A00000E bne     22F24C4h
022F2488 E3A01004 mov     r1,4h
022F248C EBFFEF41 bl      22EE198h
022F2490 E3A00002 mov     r0,2h
022F2494 E1A01000 mov     r1,r0
022F2498 E3A02000 mov     r2,0h
022F249C EBF45BEC bl      2009454h
022F24A0 E3A00003 mov     r0,3h
022F24A4 E3A01002 mov     r1,2h
022F24A8 E3A02000 mov     r2,0h
022F24AC EBF45BE8 bl      2009454h
022F24B0 E3A00004 mov     r0,4h
022F24B4 E3A01002 mov     r1,2h
022F24B8 E3A02000 mov     r2,0h
022F24BC EBF45BE4 bl      2009454h
022F24C0 EA00002F b       22F2584h
022F24C4 E3540007 cmp     r4,7h
022F24C8 1A00000E bne     22F2508h
022F24CC E3A01001 mov     r1,1h
022F24D0 EBFFEF30 bl      22EE198h
022F24D4 E3A00002 mov     r0,2h
022F24D8 E1A01000 mov     r1,r0
022F24DC E3A02000 mov     r2,0h
022F24E0 EBF45BDB bl      2009454h
022F24E4 E3A00003 mov     r0,3h
022F24E8 E3A01002 mov     r1,2h
022F24EC E3A02000 mov     r2,0h
022F24F0 EBF45BD7 bl      2009454h
022F24F4 E3A00004 mov     r0,4h
022F24F8 E3A01002 mov     r1,2h
022F24FC E3A02000 mov     r2,0h
022F2500 EBF45BD3 bl      2009454h
022F2504 EA00001E b       22F2584h
022F2508 E3540008 cmp     r4,8h
022F250C 1A00000D bne     22F2548h
022F2510 EBFFEF54 bl      22EE268h
022F2514 E3A00002 mov     r0,2h
022F2518 E1A01000 mov     r1,r0
022F251C E3A02000 mov     r2,0h
022F2520 EBF45BCB bl      2009454h
022F2524 E3A00003 mov     r0,3h
022F2528 E3A01002 mov     r1,2h
022F252C E3A02000 mov     r2,0h
022F2530 EBF45BC7 bl      2009454h
022F2534 E3A00004 mov     r0,4h
022F2538 E3A01002 mov     r1,2h
022F253C E3A02000 mov     r2,0h
022F2540 EBF45BC3 bl      2009454h
022F2544 EA00000E b       22F2584h
022F2548 E3540001 cmp     r4,1h
022F254C 059F1048 ldreq   r1,=2321040h
022F2550 E59F0040 ldr     r0,=2324CC8h              ///CurMapDataPtr?
022F2554 159F1044 ldrne   r1,=232105Ch
022F2558 E5900004 ldr     r0,[r0,4h]
022F255C EBFFE5AD bl      22EBC18h                  ///fun_022EBC18 Loads correct constants for the level id!
022F2560 E59F0030 ldr     r0,=2324CC8h              ///CurMapDataPtr?
022F2564 E3E01000 mvn     r1,0h
022F2568 E5900004 ldr     r0,[r0,4h]
022F256C EBFFE742 bl      22EC27Ch                  /// LoadBGTileSets
022F2570 E59F0020 ldr     r0,=2324CC8h              ///CurMapDataPtr?
022F2574 E5902000 ldr     r2,[r0]
022F2578 E5901004 ldr     r1,[r0,4h]
022F257C E282000C add     r0,r2,0Ch
022F2580 EBFFF5F5 bl      22EFD5Ch                  ///fun_022EFD5C
022F2584 E59F000C ldr     r0,=2324CC8h              ///CurMapDataPtr?
022F2588 E3A01001 mov     r1,1h
022F258C E5900000 ldr     r0,[r0]
022F2590 E5C01008 strb    r1,[r0,8h]
022F2594 E8BD8010 pop     r4,r15
022F2598 02324CC8 
022F259C 02321040 
022F25A0 0232105C 


/*
     022EC27C LoadBGTileSets 22EC27Ch
        Refers to all the various filenames of the level data!
        -R0 : PtrStructLevel2 : usually the pointer at [2324CC8h,4h] 
        -R1 : LevelId
        
        Note: Called when the top screen bg changes, and when the lower screen bg changes to either a map, or an image (The krabby blowing bubbles)
*/
022EC27C E92D4FF0 push    r4-r11,r14
022EC280 E24DDF4F sub     r13,r13,13Ch
022EC284 E1A0A000 mov     r10,r0                        //R10 = PtrStructLevel2
022EC288 E5DA02BC ldrb    r0,[r10,2BCh]                 
022EC28C E1A04001 mov     r4,r1                         //R4 = Param1 (Possibly LevelId!)
022EC290 E3500000 cmp     r0,0h
if( R0 == 0 )
    022EC294 03A00006 moveq   r0,6h
    022EC298 058D0000 streq   r0,[r13]
else
    022EC29C 13A0000F movne   r0,0Fh
    022EC2A0 158D0000 strne   r0,[r13]
022EC2A4 E3E00000 mvn     r0,0h                         //r0 = 0xFFFFFFFF
022EC2A8 E1540000 cmp     r4,r0
022EC2AC E1A0000A mov     r0,r10                        //R0 = PtrStructLevel2
if( Param1 != 0xFFFFFFFF )
    022EC2B0 1A000001 bne     22EC2BCh                  ///022EC2BC LBL1 v
022EC2B4 EBFFFF74 bl      22EC08Ch                      ///fun_022EC08C 022EC08C
022EC2B8 EA00028D b       22ECCF4h                      ///022ECCF4 LBL2 Return v 
///022EC2BC LBL1
022EC2BC EBFFFF41 bl      22EBFC8h                      ///fun_022EBFC8 Seems to allocate level data buffer?
022EC2C0 E3A00001 mov     r0,1h
022EC2C4 E1CA00B0 strh    r0,[r10]
022EC2C8 E3A00000 mov     r0,0h
022EC2CC E5CA01C0 strb    r0,[r10,1C0h]
022EC2D0 E28A0C01 add     r0,r10,100h
022EC2D4 E1C04BBE strh    r4,[r0,0BEh]                  //[R0+0xBE] = LevelId
022EC2D8 E1D01BFE ldrsh   r1,[r0,0BEh]                  //R1 = LevelId
022EC2DC E28D00CC add     r0,r13,0CCh
022EC2E0 EBFFFE16 bl      22EBB40h                      ///fun_022EBB40(r0 = targetfnamestr, r1 = lvlid ) Looks up the filenames in bg_list.dat!
022EC2E4 E59F1A10 ldr     r1,=2320C58h                  //"rom0:/MAP_BG/%s.bpl"
022EC2E8 E28D004C add     r0,r13,4Ch
022EC2EC E28D20CC add     r2,r13,0CCh
022EC2F0 EBF674A3 bl      2089584h                      ///fun_02089584 ReplaceTokens
022EC2F4 E59D2000 ldr     r2,[r13]                      // 6 or 15
022EC2F8 E28D104C add     r1,r13,4Ch                    //r1 = r13 + 76
022EC2FC E28A0F5D add     r0,r10,174h                   //R0 = PtrBPLBuff  
022EC300 EBF4724D bl      2008C3Ch                      ///LoadFileFromRom
022EC304 E59F19F4 ldr     r1,=2320C6Ch                  //"rom0:/MAP_BG/%s.bpc"
022EC308 E28D004C add     r0,r13,4Ch
022EC30C E28D20D6 add     r2,r13,0D6h
022EC310 EBF6749B bl      2089584h                      ///fun_02089584 ReplaceTokens
022EC314 E59F29E8 ldr     r2,=30Fh
022EC318 E28A0F61 add     r0,r10,184h                   //R0 = PtrBPCBuff
022EC31C E28D104C add     r1,r13,4Ch
022EC320 EBF47245 bl      2008C3Ch                      ///LoadFileFromRom
022EC324 E59F19DC ldr     r1,=2320C80h                  //"rom0:/MAP_BG/%s.bma"
022EC328 E28D004C add     r0,r13,4Ch
022EC32C E28D20E0 add     r2,r13,0E0h
022EC330 EBF67493 bl      2089584h                      ///fun_02089584 ReplaceTokens
022EC334 E59F29C8 ldr     r2,=30Fh                      //r2 = 0x30F
022EC338 E28A0F63 add     r0,r10,18Ch                   //R0 = PtrBMABuff
022EC33C E28D104C add     r1,r13,4Ch                    //r1 = StackPtr + 0x4C
022EC340 EBF4723D bl      2008C3Ch                      ///LoadFileFromRom
022EC344 E59A1184 ldr     r1,[r10,184h]                 //R1 = PtrBPCBuff
022EC348 E59A0174 ldr     r0,[r10,174h]                 //R0 = PtrBPLBuff
022EC34C E1D120B0 ldrh    r2,[r1]                       //R2 = FirstWordBPC
022EC350 E58D0028 str     r0,[r13,28h]                  //[R13 + 28] = PtrBPLBuff
022EC354 E59A518C ldr     r5,[r10,18Ch]                 //R5 = PtrBMABuff
022EC358 E3520000 cmp     r2,0h                         //
if( R2 == 0 )                                           //FirstWordBPC == 0
    022EC35C 0A00000F beq     22EC3A0h                  ///022EC3A0 LBL3 v
022EC360 E1D100B2 ldrh    r0,[r1,2h]                    //R0 = SecondWordBPC
022EC364 E1A020A2 mov     r2,r2,lsr 1h                  //R2 = FirstWordBPC/2
022EC368 E0812082 add     r2,r1,r2,lsl 1h               //R2 = PtrBPCBuff + ((FirstWordBPC/2) * 2) //I think they're making sure the number is even by clearing the lowest bit.
022EC36C E3500000 cmp     r0,0h
022EC370 E58D2044 str     r2,[r13,44h]                  //[R13 + 44h] = PtrBPCBuff + ((FirstWordBPC/2) * 2)
if( R0 == 0 )
    022EC374 03A02000 moveq   r2,0h
    022EC378 03A00001 moveq   r0,1h
    022EC37C 058D2048 streq   r2,[r13,48h]
    022EC380 058A01C4 streq   r0,[r10,1C4h]
    022EC384 0A000009 beq     22EC3B0h                  ///022EC3B0 LBL4 v
022EC388 E1A000A0 mov     r0,r0,lsr 1h                  //R0 = SecondWordBPC/2
022EC38C E0812080 add     r2,r1,r0,lsl 1h               //R2 = PtrBPCBuff + ((SecondWordBPC/2) * 2)
022EC390 E3A00002 mov     r0,2h                         //R0 = 2
022EC394 E58D2048 str     r2,[r13,48h]                  //[R13 + 48h] = PtrBPCBuff + ((SecondWordBPC/2) * 2)
022EC398 E58A01C4 str     r0,[r10,1C4h]                 //[R10 + 1C4h] = 2
022EC39C EA000003 b       22EC3B0h                      ///022EC3B0 LBL4 v
///022EC3A0 LBL3
022EC3A0 E3A00000 mov     r0,0h
022EC3A4 E58D0044 str     r0,[r13,44h]                  //[R13 + 44h] = 0 //This is probably the BPC beginning ?
022EC3A8 E58D0048 str     r0,[r13,48h]                  //[R13 + 48h] = 0 //This is probably the BPC
022EC3AC E58A01C4 str     r0,[r10,1C4h]                 //[R10 + 1C4h] = 0
///022EC3B0 LBL4
022EC3B0 E59D0028 ldr     r0,[r13,28h]                  //R0 = PtrBPLBuff
022EC3B4 E28A20EA add     r2,r10,0EAh
022EC3B8 E5D03000 ldrb    r3,[r0]                       //R3 = FirstByteBPL
022EC3BC E28A0C01 add     r0,r10,100h
022EC3C0 E2811004 add     r1,r1,4h                      //R1 = PtrBPC_DBA
022EC3C4 E1C03EBA strh    r3,[r0,0EAh]                  //[r0,0EAh] = FirstByteBPL (Basically put at r10 + 0x1EA the first byte of the palette!)
022EC3C8 E59D0028 ldr     r0,[r13,28h]                  //R0 = PtrBPLBuff
022EC3CC E5D03002 ldrb    r3,[r0,2h]                    //R3 = Unk2BPL
022EC3D0 E2820C01 add     r0,r2,100h
022EC3D4 E58D0040 str     r0,[r13,40h]                  //[r13,40h] = PtrBPLHeaderCopy
022EC3D8 E1C030B2 strh    r3,[r0,2h]
022EC3DC E3A00000 mov     r0,0h                         //R0 = CNT ?

//
///022EC3E0 LOOP3_BEG - Copy BPC SizeTable
//
022EC3E0 E28A4F72 add     r4,r10,1C8h
022EC3E4 E3A0200C mov     r2,0Ch
022EC3E8 E28D3044 add     r3,r13,44h                    //PtrBPC_DBB
022EC3EC E7933100 ldr     r3,[r3,r0,lsl 2h]             //R3 = [PtrBPC_DBB + (CNT * 4)]
022EC3F0 E0244290 mla     r4,r0,r2,r4                   //R4 = (CNT * 12) + R4
022EC3F4 E3530000 cmp     r3,0h
022EC3F8 E3A06000 mov     r6,0h                         //R6 = CNT2 = 0
if( R3 == 0 )
    022EC3FC 0A00000A beq     22EC42Ch                      ///022EC42C LBL5 v
022EC400 E0D120B2 ldrh    r2,[r1],2h                    //R2 = [PtrBPC_DBA] then increment PtrBPC_DBA by 2! (This parses the words from the BPC SizeTable's ScreenEntry)
022EC404 E1C420B0 strh    r2,[r4]                       //    

///022EC408 LOOP1_BEG
022EC408 E0D130B2 ldrh    r3,[r1],2h                    //R2 = [PtrBPC_DBA] then increment PtrBPC_DBA by 2!
022EC40C E0842086 add     r2,r4,r6,lsl 1h               //R2 = R4 + (CNT2 * 2)
022EC410 E2866001 add     r6,r6,1h                      //R6 = CNT2 + 1
022EC414 E1C230B2 strh    r3,[r2,2h]
022EC418 E3560004 cmp     r6,4h
if( CNT2 < 4 )
    022EC41C BAFFFFF9 blt     22EC408h                      ///022EC408 LOOP1_BEG ^

022EC420 E0D120B2 ldrh    r2,[r1],2h                    //R2 = [PtrBPC_DBA], increment PtrBPC_DBA by 2!
022EC424 E1C420BA strh    r2,[r4,0Ah]
022EC428 EA000007 b       22EC44Ch                      ///022EC44C LBL6 v
///022EC42C LBL5
022EC42C E1C460B0 strh    r6,[r4]
022EC430 E1A03006 mov     r3,r6

///022EC434 LOOP2_BEG
022EC434 E0842086 add     r2,r4,r6,lsl 1h
022EC438 E2866001 add     r6,r6,1h
022EC43C E1C230B2 strh    r3,[r2,2h]
022EC440 E3560004 cmp     r6,4h
if( R6 < 4 )
    022EC444 BAFFFFFA blt     22EC434h                      ///022EC434 LOOP2_BEG ^

022EC448 E1C430BA strh    r3,[r4,0Ah]
///022EC44C LBL6
022EC44C E2800001 add     r0,r0,1h                      //R0 = CNT + 1
022EC450 E3500002 cmp     r0,2h                         //
if( CNT < 2 )
    022EC454 BAFFFFE1 blt     22EC3E0h                      ///022EC3E0 LOOP3_BEG ^

///
///022EC458 - Copy BMA stuff
///
022EC458 E5D51000 ldrb    r1,[r5]                       //R1 = [PtrBMABuff] = MapWidth
022EC45C E28A0C02 add     r0,r10,200h
022EC460 E58D0034 str     r0,[r13,34h]
022EC464 E5CA11E0 strb    r1,[r10,1E0h]                 //[R10 + 0x1E0] = MapWidth
022EC468 E5D50001 ldrb    r0,[r5,1h]                    //R0 = [PtrBMABuff+1] = MapHeight
022EC46C E28A4E1E add     r4,r10,1E0h
022EC470 E59F2894 ldr     r2,=2324CBCh                  //Pointer to Pointer, or pointer table?
022EC474 E5C40001 strb    r0,[r4,1h]                    //
022EC478 E5D53004 ldrb    r3,[r5,4h]                    //R3 = BMA_DBA_unk3
022EC47C E59D0028 ldr     r0,[r13,28h]                  //R0 = PtrBPLBuff
022EC480 E3A01028 mov     r1,28h                        //R1 = 0x28
022EC484 E2800004 add     r0,r0,4h                      //R0 = PtrBPL_Palettes + 4 //Skips header?
022EC488 E5C43002 strb    r3,[r4,2h]                    //[R4+2] = BMA_DBA_unk3   
022EC48C E58D001C str     r0,[r13,1Ch]                  //[r13,1Ch] = (PtrBPL_Palettes + 4)
022EC490 E5D50005 ldrb    r0,[r5,5h]                    //R0 = [PtrBMABuff+5] = BMA_unk4
022EC494 E3A09000 mov     r9,0h                         //R9 = CNTPalettes = 0
022EC498 E5C40003 strb    r0,[r4,3h]                    //[R4+3] = BMA_unk4
022EC49C E5D50006 ldrb    r0,[r5,6h]                    //R0 = BMA_unk5
022EC4A0 E1C400B4 strh    r0,[r4,4h]                    //[R4+4] = BMA_unk5
022EC4A4 E5D50008 ldrb    r0,[r5,8h]                    //R0 = BMA_unk6
022EC4A8 E1C400B6 strh    r0,[r4,6h]                    //[R4+6] = BMA_unk6    
022EC4AC E5D5000A ldrb    r0,[r5,0Ah]                   //R0 = BMA_unk7
022EC4B0 E1C400B8 strh    r0,[r4,8h]                    //[R4+8] = BMA_unk7
022EC4B4 E59D0034 ldr     r0,[r13,34h]                  //r0 = r10 + 0x200
022EC4B8 E5DA32BC ldrb    r3,[r10,2BCh]                 //r3 =      (is 0 only if constants were loaded by Ground map select, otherwise is always 1)
022EC4BC E1D00CF2 ldrsh   r0,[r0,0C2h]                  //r0 =     ([r10,2C2h], from default map constants. Is always 0) 
022EC4C0 E5922000 ldr     r2,[r2]                       //r2 = [2324CBCh]
022EC4C4 E1660183 smulbb  r6,r3,r1                      //r6 = r3 * 0x28
022EC4C8 E1A00A00 mov     r0,r0,lsl 14h                 //r0 = r0 << 20
022EC4CC E2827098 add     r7,r2,98h                     //r7 = r2 + 0x98
022EC4D0 E1A08820 mov     r8,r0,lsr 10h                 //r8 = (r0 << 20) >> 16
022EC4D4 EA000012 b       22EC524h                  ///022EC524 LBL7 v
///
///022EC4D8 LOOP4_BEG  - Copy color palettes!
///
022EC4D8 E2880001 add     r0,r8,1h                  //r0 = Cntr8 + 1
022EC4DC E1A00800 mov     r0,r0,lsl 10h
022EC4E0 E1A01008 mov     r1,r8
022EC4E4 E1A08820 mov     r8,r0,lsr 10h
022EC4E8 E59F2820 ldr     r2,=2320BF4h
022EC4EC E0870006 add     r0,r7,r6                  //r0 = ([2324CBCh] + 98h) + ([r10,2BCh] * 0x28)
022EC4F0 EBF47826 bl      200A590h                  //Copy color at r2, at buffer at r0 + 0x18
{
    0200A590 E5900018 ldr     r0,[r0,18h]
    0200A594 E3A03004 mov     r3,4h
    0200A598 E0801101 add     r1,r0,r1,lsl 2h       //r1 = PtrMemPalette?? + (CntColor * 4)
    ///0200A59C
    0200A59C E4D20001 ldrb    r0,[r2],1h
    0200A5A0 E2533001 subs    r3,r3,1h
    0200A5A4 E4C10001 strb    r0,[r1],1h
    if( r3 != 0 )
        0200A5A8 1AFFFFFB bne     200A59Ch              ///0200A59C ^
    0200A5AC E12FFF1E bx      r14
}
022EC4F4 E59D201C ldr     r2,[r13,1Ch]              //r2 = PtrBPL_Palettes
022EC4F8 E0870006 add     r0,r7,r6                  //r0 = A pointer in a pointer table?
022EC4FC E1A01008 mov     r1,r8                     //r1 = ColorIndex??
022EC500 E3A0300F mov     r3,0Fh                    //r3 = Colors to copy??
022EC504 EBF47829 bl      200A5B0h                  //Seems to copy colors bytes from the bpl
{
    0200A5B0 E92D4038 push    r3-r5,r14
    0200A5B4 E5900018 ldr     r0,[r0,18h]           //r0 = PtrMemPalette??
    0200A5B8 E3A0E000 mov     r14,0h                //r14 = CntColor
    0200A5BC E080C101 add     r12,r0,r1,lsl 2h      //r12 = PtrMemPalette?? + (ColorIndex?? * 4)
    0200A5C0 EA000009 b       200A5ECh              ///0200A5EC v
    ///0200A5C4
    0200A5C4 E1A04002 mov     r4,r2                 //r4 = PtrSourceBplPalette
    0200A5C8 E1A0500C mov     r5,r12                //r5 = PtrDestinationPalette
    0200A5CC E2822004 add     r2,r2,4h              //r2 = PtrBPL_Palettes + 4
    0200A5D0 E28CC004 add     r12,r12,4h            //r12 = PtrDestinationPalette + 4
    0200A5D4 E3A01004 mov     r1,4h                 //r1 = NbColorBytesLeft = 4
    ///0200A5D8
    0200A5D8 E4D40001 ldrb    r0,[r4],1h            //r0 = [PtrSourceBplPalette],  ++PtrSourceBplPalette
    0200A5DC E2511001 subs    r1,r1,1h              //r1 = NbColorBytesLeft - 1
    0200A5E0 E4C50001 strb    r0,[r5],1h            //[PtrDestinationPalette] = CurColorByte, ++PtrDestinationPalette
    if( r1 != 0 )
        0200A5E4 1AFFFFFB bne     200A5D8h              ///0200A5D8 ^
    0200A5E8 E28EE001 add     r14,r14,1h            //r14 = CntColor + 1
    ///0200A5EC
    0200A5EC E15E0003 cmp     r14,r3                    
    if( CntColor < NbColorsToCopy )
        0200A5F0 BAFFFFF3 blt     200A5C4h              ///0200A5C4 ^
    0200A5F4 E8BD8038 pop     r3-r5,r15
}
022EC508 E288000F add     r0,r8,0Fh                 //r0 = ColorIndex? + 15
022EC50C E1A00800 mov     r0,r0,lsl 10h             //r0 = r0 << 16
022EC510 E1A08820 mov     r8,r0,lsr 10h             //r8 = r0 >> 16
022EC514 E59D001C ldr     r0,[r13,1Ch]              //r0 = PtrBPL_Palettes
022EC518 E2899001 add     r9,r9,1h                  //r9 = CNTPalettes + 1
022EC51C E280003C add     r0,r0,3Ch                 //r0 = PtrBPL_Palettes + 60
022EC520 E58D001C str     r0,[r13,1Ch]              //[r13,1Ch] = (PtrBPL_Palettes + 60)
///022EC524 LBL7
022EC524 E59D0040 ldr     r0,[r13,40h]              //r0 = PtrBPLHeaderCopy
022EC528 E1D000F0 ldrsh   r0,[r0]                   //r0 = [PtrBPLHeaderCopy] //BPLNbPalettes
022EC52C E1590000 cmp     r9,r0
if( CNTPalettes >= BPLNbPalettes )
    022EC530 AA000003 bge     22EC544h              ///022EC544 LBL6 v
022EC534 E59D0034 ldr     r0,[r13,34h]
022EC538 E1D00CF4 ldrsh   r0,[r0,0C4h]              //r0 = Unknown?? (Was 0xE) (At 0x210CD08) (The value is actually a constant loaded from the table at 0x2320CD8, on line 022EBC2C)
022EC53C E1590000 cmp     r9,r0
if( CNTPalettes < r0 )
    022EC540 BAFFFFE4 blt     22EC4D8h              ///022EC4D8 LOOP4_BEG  We seem to copy part of the palette this way, and continue parsing the rest another way?

///022EC544 LBL6
022EC544 E28A0C02 add     r0,r10,200h
022EC548 E58D0038 str     r0,[r13,38h]
022EC54C EA000015 b       22EC5A8h                  ///022EC5A8 v 
///022EC550
022EC550 E2880001 add     r0,r8,1h                  //r0 = ColorIndex? + 1
022EC554 E1A00800 mov     r0,r0,lsl 10h             //r0 = (ColorIndex? + 1) << 16
022EC558 E1A01008 mov     r1,r8                     //r1 = ColorIndex?
022EC55C E1A08820 mov     r8,r0,lsr 10h             //r8 = ((ColorIndex? + 1) << 16) >> 16
022EC560 E59F27A8 ldr     r2,=2320BF4h              //r2 = 
022EC564 E0870006 add     r0,r7,r6
022EC568 EBF47808 bl      200A590h
022EC56C E3A00001 mov     r0,1h
022EC570 E58D0018 str     r0,[r13,18h]
///022EC574
022EC574 E2880001 add     r0,r8,1h
022EC578 E1A00800 mov     r0,r0,lsl 10h
022EC57C E1A01008 mov     r1,r8
022EC580 E1A08820 mov     r8,r0,lsr 10h
022EC584 E59F2788 ldr     r2,=2320BE8h
022EC588 E0870006 add     r0,r7,r6
022EC58C EBF477FF bl      200A590h
022EC590 E59D0018 ldr     r0,[r13,18h]
022EC594 E2800001 add     r0,r0,1h
022EC598 E58D0018 str     r0,[r13,18h]
022EC59C E3500010 cmp     r0,10h
022EC5A0 BAFFFFF3 blt     22EC574h                  ///022EC574 ^
022EC5A4 E2899001 add     r9,r9,1h
///022EC5A8
022EC5A8 E59D0038 ldr     r0,[r13,38h]              //r0 = AddressNextMemPalette?
022EC5AC E1D00CF4 ldrsh   r0,[r0,0C4h]              //r0 = Unknown?? (Was 0xE)
022EC5B0 E1590000 cmp     r9,r0
if( CNTPalettes < r0 )                                  //Seems like we parse the remaining palettes 
    022EC5B4 BAFFFFE5 blt     22EC550h                  ///022EC550 ^
022EC5B8 E0870006 add     r0,r7,r6
022EC5BC EBF477D0 bl      200A504h
{
    0200A504 E3A01001 mov     r1,1h
    0200A508 E5C01008 strb    r1,[r0,8h]
    0200A50C E12FFF1E bx      r14
}
022EC5C0 E28A0C02 add     r0,r10,200h               //r0 = 
022EC5C4 E1D00BFE ldrsh   r0,[r0,0BEh]              //r0 = 
022EC5C8 E3A08000 mov     r8,0h                     //r8 = CNT_4 = 0
022EC5CC E58D0020 str     r0,[r13,20h]
022EC5D0 EA0000F2 b       22EC9A0h                  ///022EC9A0 v

///
///022EC5D4 LOOP_BEG_ParseBPCIndexTable - We seem to begin reading the BPC Datablock B stuff here?
///
022EC5D4 E28D0044 add     r0,r13,44h                //r0 = PtrBPC_DBB
022EC5D8 E7900108 ldr     r0,[r0,r8,lsl 2h]         //r0 = [ PtrBPC_DBB + (CNT_4 * 4) ]
022EC5DC E58D0024 str     r0,[r13,24h]              //r0 = PtrBPCCurPos?
022EC5E0 E3500000 cmp     r0,0h
if( PtrBPCCurPos? == 0 )
    022EC5E4 0A0000E9 beq     22EC990h                  ///022EC990 v
022EC5E8 E3A0000C mov     r0,0Ch                    //r0 = 12
022EC5EC E0010098 mul     r1,r8,r0                  //r1 = CNT_4 * 12
022EC5F0 E28A0C02 add     r0,r10,200h               //r0 = r10 + 0x200
022EC5F4 E1D02CF6 ldrsh   r2,[r0,0C6h]              //r2 = [r0,0C6h]
022EC5F8 E28A6F72 add     r6,r10,1C8h               //r6 = r10 + 0x1C8 = PtrBPC_DBA_Copy (datablockA entries in-memory copy ptr)  
022EC5FC E0869001 add     r9,r6,r1                  //r9 = PtrBPC_DBA_Copy + (CNT_4 * 12) (This gets the entry at the current index CNT_4)
022EC600 E19610F1 ldrsh   r1,[r6,r1]                //r1 = [ PtrBPC_DBA_Copy + (CNT_4 * 12) ] = BPC_Entry_unk1 (First word of the DBA entry!)
022EC604 E5DA32BC ldrb    r3,[r10,2BCh]             //r3 = [r10,2BCh] (was 0)
022EC608 E59F0708 ldr     r0,=2320C18h              //Table_02320C18 VRAM addresses table!! (Table with 5, 8 bytes entries made of 2 int 32) 
022EC60C E3A07000 mov     r7,0h                     //r7 = 0
022EC610 E0803183 add     r3,r0,r3,lsl 3h           //r3 = 2320C18h + (r3 * 8)
022EC614 E59D0020 ldr     r0,[r13,20h]              //r0 = [r13,20h] (was 0)
022EC618 E59D6024 ldr     r6,[r13,24h]              //r6 = PtrBPC_DBB?
022EC61C E7930100 ldr     r0,[r3,r0,lsl 2h]         //r0 = Table_02320C18[r3][r0] 
022EC620 E58D7014 str     r7,[r13,14h]              //[r13,14h] = 0
022EC624 E080B282 add     r11,r0,r2,lsl 5h          //r11 = Table_02320C18[r3][r0] + (r2 * 32)
022EC628 E08B0281 add     r0,r11,r1,lsl 5h          //r0  = (Table_02320C18[r3][r0] + (r2 * 32)) + (BPC_Entry_unk1 * 32)
022EC62C E58D0010 str     r0,[r13,10h]              //[r13,10h] = r0
022EC630 E1A03007 mov     r3,r7                     //r3 = 0
022EC634 E1A02007 mov     r2,r7                     //r2 = 0
022EC638 E1A01007 mov     r1,r7                     //r1 = 0 = CNT_5
022EC63C E1A00007 mov     r0,r7                     //r0 = 0
///022EC640 - Zero out 16 bytes of the video ram!
022EC640 E0CB00B2 strh    r0,[r11],2h               //
022EC644 E2811001 add     r1,r1,1h                  //r1 = CNT_5 + 1
022EC648 E3510010 cmp     r1,10h                    
if( CNT_5 < 16 )
    022EC64C BAFFFFFB blt     22EC640h              ///022EC640 ^
022EC650 EA000047 b       22EC774h              ///022EC774 v

///
///022EC654 - This loop decodes the BPC image and write it straight to VRAM!!
///
022EC654 E5D60000 ldrb    r0,[r6]               //r0 = [PtrCurBPC_DBB] = cmdbyte
022EC658 E2861001 add     r1,r6,1h              //r1 = PtrBPC_DBB + 1
022EC65C E1A06001 mov     r6,r1                 //r6 = PtrCurBPC_DBB
022EC660 E3500080 cmp     r0,80h
if( cmdbyte >= 0x80 )                           //SIGNED! ( cmdbyte >= -128 )
    022EC664 AA00001B bge     22EC6D8h              ///022EC6D8 v
022EC668 E350007F cmp     r0,7Fh
if( cmdbyte == 0x7F )
    022EC66C 05D1C001 ldreqb  r12,[r1,1h]           //r12 = ThirdByteAfterCur
    022EC670 05D10000 ldreqb  r0,[r1]               //r0  = SecondByteAfterCur
    022EC674 02816002 addeq   r6,r1,2h              //r6  = PtrCurBPC_DBB + 2
    022EC678 0180040C orreq   r0,r0,r12,lsl 8h      //r0  = SecondByteAfterCur | (ThirdByteAfterCur << 8)
    022EC67C 0A000002 beq     22EC68Ch              ///022EC68C v
022EC680 E350007E cmp     r0,7Eh
if( cmdbyte == 0x7E )
    022EC684 05D10000 ldreqb  r0,[r1]
    022EC688 02816001 addeq   r6,r1,1h
///022EC68C
022EC68C E3530000 cmp     r3,0h                 //r3 is a boolean flag triggered by some command bytes
if( r3 != 0 )
    022EC690 14D61001 ldrneb  r1,[r6],1h            //r1 = [PtrCurBPC_DBB], ++PtrCurBPC_DBB
    022EC694 13A03000 movne   r3,0h                 //r3 = 0
    022EC698 12400001 subne   r0,r0,1h              //r0 = cmdbyte - 1 = nbtowrite
    022EC69C 11821401 orrne   r1,r2,r1,lsl 8h       //r1 = r2 | (r1 << 8)
    022EC6A0 10CB10B2 strneh  r1,[r11],2h           //[r11] = r1, r11 = r11 + 2
022EC6A4 E3A01000 mov     r1,0h                     //r1 = CntCopy = 0
022EC6A8 EA000005 b       22EC6C4h              ///022EC6C4 v
///022EC6AC - Copy the next 16bits word!
022EC6AC E5D6E001 ldrb    r14,[r6,1h]               //r14 = [PtrCurBPC_DBB + 1]
022EC6B0 E4D6C002 ldrb    r12,[r6],2h               //r12 = [PtrCurBPC_DBB], PtrCurBPC_DBB += 2
022EC6B4 E2811002 add     r1,r1,2h                  //r1 = CntCopy + 2
022EC6B8 E58DC02C str     r12,[r13,2Ch]             //[r13,2Ch] = r12
022EC6BC E18CC40E orr     r12,r12,r14,lsl 8h        //r12 = r12 | (r14 << 8)
022EC6C0 E0CBC0B2 strh    r12,[r11],2h              //[r11] = r12, r11 += 2 
///022EC6C4
022EC6C4 E1510000 cmp     r1,r0
if( CntCopy < nbtowrite )
    022EC6C8 BAFFFFF7 blt     22EC6ACh              ///022EC6AC ^
else if( CntCopy <= nbtowrite )
    022EC6CC D3A03001 movle   r3,1h             //r3 = 1
    022EC6D0 D4D62001 ldrleb  r2,[r6],1h        //r2 = [PtrCurBPC_DBB], ++PtrCurBPC_DBB   
022EC6D4 EA000026 b       22EC774h              ///022EC774 v
///
///022EC6D8 - cmdbyte >= 0x80
///
022EC6D8 E35000C0 cmp     r0,0C0h
if( cmdbyte >= 0xC0 )                           //SIGNED
    022EC6DC AA000006 bge     22EC6FCh              ///022EC6FC v
022EC6E0 E35000BF cmp     r0,0BFh
if( cmdbyte == 0xBF )
    022EC6E4 05D10000 ldreqb  r0,[r1]           //r0 = nbtowrite = PtrCurBPC_DBB (Load next byte!)
    022EC6E8 02816001 addeq   r6,r1,1h          //r6 = PtrCurBPC_DBB + 1
022EC6EC E58D7014 str     r7,[r13,14h]
if( cmdbyte != 0xBF )
    022EC6F0 12400080 subne   r0,r0,80h         //r0 = cmdbyte - 0x80 = nbtowrite
022EC6F4 E4D67001 ldrb    r7,[r6],1h            //r7 = [PtrCurBPC_DBB], ++PtrCurBPC_DBB (Load next byte and increment current byte ptr!)
022EC6F8 EA00000D b       22EC734h              ///022EC734 v
///
///022EC6FC - cmdbyte >= 0xC0
///
022EC6FC E35000E0 cmp     r0,0E0h
if( cmdbyte >= 0xE0 )
    022EC700 AA000004 bge     22EC718h              ///022EC718 v
022EC704 E35000DF cmp     r0,0DFh
if( cmdbyte == 0xDF )
    022EC708 05D10000 ldreqb  r0,[r1]               //r0 = nbtowrite = []
    022EC70C 02816001 addeq   r6,r1,1h
else
    022EC710 124000C0 subne   r0,r0,0C0h
022EC714 EA000006 b       22EC734h              ///022EC734 v
///
///022EC718 - curdbbbyte >= 0xE0
///
022EC718 E35000FF cmp     r0,0FFh
if( curdbbbyte == 0xFF )
    022EC71C 05D10000 ldreqb  r0,[r1]
    022EC720 02816001 addeq   r6,r1,1h
022EC724 E59D1014 ldr     r1,[r13,14h]
if( curdbbbyte != 0xFF )
    022EC728 124000E0 subne   r0,r0,0E0h
022EC72C E58D7014 str     r7,[r13,14h]
022EC730 E1A07001 mov     r7,r1
///022EC734
022EC734 E3530000 cmp     r3,0h
if( r3 != 0 )
    022EC738 11821407 orrne   r1,r2,r7,lsl 8h       //r1 = r2 | (nextbpcdbbbyte << 8)
    022EC73C 10CB10B2 strneh  r1,[r11],2h           //[PtrCurVRAM] = (r2 | (nextbpcdbbbyte << 8)), PtrCurVRAM = PtrCurVRAM + 2
022EC740 E1871407 orr     r1,r7,r7,lsl 8h           //r1 = nextbpcdbbbyte | (nextbpcdbbbyte << 8)
022EC744 E1A01801 mov     r1,r1,lsl 10h             //r1 = r1 << 16
022EC748 E1A02821 mov     r2,r1,lsr 10h             //r2 = r1 >> 16 (Make unsigned int16)
if( r3 != 0 )
    022EC74C 13A03000 movne   r3,0h                 //r3 = 0
    022EC750 12400001 subne   r0,r0,1h              //r0 = LastComparedValue - 1
022EC754 E3A01000 mov     r1,0h                     //r1 = CNT_6 = 0
022EC758 EA000001 b       22EC764h              ///022EC764 v
///022EC75C
022EC75C E0CB20B2 strh    r2,[r11],2h               //[r11] = r2, r11 = r11 + 2
022EC760 E2811002 add     r1,r1,2h                  //r1 = CNT_6 + 1
///022EC764
022EC764 E1510000 cmp     r1,r0
if( CNT_6 < (curdbbbyte - 0x80) )
    022EC768 BAFFFFFB blt     22EC75Ch              ///022EC75C ^ 
else if( CNT_6 <= (curdbbbyte - 0x80) )
    022EC76C D3A03001 movle   r3,1h                 //r3 = 1
    022EC770 D1A02007 movle   r2,r7                 //r2 = nextbpcdbbbyte
///022EC774 - Validate Loop
022EC774 E59D0010 ldr     r0,[r13,10h]          //r0 = PtrVRAMEnd
022EC778 E15B0000 cmp     r11,r0                
if( VRAM_BaseAddress < PtrVRAMEnd )
    022EC77C 3AFFFFB4 bcc     22EC654h              ///022EC654 ^

///
/// 022EC780 - skip padding at the end if tile size odd numbered
///
022EC780 E59D0024 ldr     r0,[r13,24h]              //r0 = PtrBPC_DBB
022EC784 E1D970F0 ldrsh   r7,[r9]                   //r7 = BPC_DBA_Unk1
022EC788 E0460000 sub     r0,r6,r0                  //r0 = PtrCurBPC_DBB - PtrBPC_DBB = BPC_ChunkCompressedLen
022EC78C E3100001 tst     r0,1h
if( (BPC_ChunkCompressedLen & 1) != 0 )
    022EC790 12866001 addne   r6,r6,1h              //r6 = PtrCurBPC_DBB +1
022EC794 E28A0C02 add     r0,r10,200h               //r0 = r10 + 0x200
022EC798 E59F257C ldr     r2,=0FFFFh                //r2 = 0xFFFF
022EC79C E3A03000 mov     r3,0h                     //r3 = 0    
022EC7A0 EA000005 b       22EC7BCh              ///022EC7BC v

//
///022EC7A4 VRAM_PADDING_LOOP - This fills in the missing tiles. (NbTilesCurDispMode - NbTiles)
//
022EC7A4 E1A01003 mov     r1,r3                     //r1 = 0
///022EC7A8
022EC7A8 E0CB20B2 strh    r2,[r11],2h               //[r11] = 0xFFFF, r11 += 2
022EC7AC E2811001 add     r1,r1,1h                  //r1 = r1 + 1
022EC7B0 E3510010 cmp     r1,10h
if(r1 < 16)
    022EC7B4 BAFFFFFB blt     22EC7A8h              ///022EC7A8 ^
022EC7B8 E2877001 add     r7,r7,1h
///022EC7BC
022EC7BC E1D01CF8 ldrsh   r1,[r0,0C8h]              //r1 = NbTilesCurDispMode //r10 + 2C8h
022EC7C0 E1570001 cmp     r7,r1
if( BPC_DBA_Unk1 < r1 )
    022EC7C4 BAFFFFF6 blt     22EC7A4h              ///022EC7A4 VRAM_PADDING_LOOP ^

022EC7C8 E28A0C02 add     r0,r10,200h
022EC7CC E08A1108 add     r1,r10,r8,lsl 2h          //r1 = r10 + (CNT_4 * 4)
022EC7D0 E591C2DC ldr     r12,[r1,2DCh]             //r12 = AddressOfBlockContainingTheCopiedHeaderData
022EC7D4 E1D07CF2 ldrsh   r7,[r0,0C2h]              //(was 0)
022EC7D8 E1D02CF6 ldrsh   r2,[r0,0C6h]              //(was 0)
022EC7DC E3A01001 mov     r1,1h                     //r1 = 1
022EC7E0 E1A0300C mov     r3,r12                    //r3 = r12 = BPC_SecDBBEntryCurCpAddress
022EC7E4 E1C01BB8 strh    r1,[r0,0B8h]              //
022EC7E8 E1820607 orr     r0,r2,r7,lsl 0Ch          //r0 = r2 | (r7 << 12)
022EC7EC E1A00800 mov     r0,r0,lsl 10h             //r0 = r0 << 16
022EC7F0 E1D910FA ldrsh   r1,[r9,0Ah]               //r1 = BPC_DBA_Unk6
022EC7F4 E58D003C str     r0,[r13,3Ch]              //[r13,3Ch] = r0
022EC7F8 E2410001 sub     r0,r1,1h                  //r0 = BPC_DBA_Unk6 - 1
022EC7FC E0800180 add     r0,r0,r0,lsl 3h           //r0 = (BPC_DBA_Unk6 - 1) + ((BPC_DBA_Unk6 - 1) * 8)
022EC800 E08C7080 add     r7,r12,r0,lsl 1h          //r7 = r12 + (r0 * 2) = BPC_SecDBBEntryEndCpAddress
022EC804 E3A00000 mov     r0,0h                     //r0 = 0
022EC808 EA000020 b       22EC890h                  ///022EC890 v

///
///022EC80C LOOP_BEG_TMDecompPass1 - This is the first pass of the decompression process for the tilemapping data that follows the image
///
022EC80C E5D61000 ldrb    r1,[r6]               //r1 = [PtrCurBPC_DBB] = curbyte
022EC810 E2862001 add     r2,r6,1h              //r2 = PtrCurBPC_DBB + 1
022EC814 E1A06002 mov     r6,r2                 //r6 = (PtrCurBPC_DBB + 1)
022EC818 E35100C0 cmp     r1,0C0h
if( curbyte < 0xC0 )
    022EC81C BA000008 blt     22EC844h              ///022EC844 v
022EC820 E3A020C0 mov     r2,0C0h               //r2 = 0xC0
022EC824 EA000003 b       22EC838h              ///022EC838 v
///022EC828
022EC828 E4D6B001 ldrb    r11,[r6],1h
022EC82C E2822001 add     r2,r2,1h
022EC830 E1A0B40B mov     r11,r11,lsl 8h
022EC834 E0C3B0B2 strh    r11,[r3],2h
///022EC838
022EC838 E1520001 cmp     r2,r1
if( r2 <= curbyte )
    022EC83C DAFFFFF9 ble     22EC828h          ///022EC828 ^
022EC840 EA000012 b       22EC890h              ///022EC890 v
///022EC844
022EC844 E3510080 cmp     r1,80h
if( curbyte < 0x80 )
    022EC848 BA00000A blt     22EC878h              ///022EC878 v
022EC84C E5D2B000 ldrb    r11,[r2]              //r11 = 
022EC850 E2826001 add     r6,r2,1h
022EC854 E3A02080 mov     r2,80h
022EC858 E1A0BC0B mov     r11,r11,lsl 18h
022EC85C E1A0B82B mov     r11,r11,lsr 10h
022EC860 EA000001 b       22EC86Ch              ///022EC86C v
///022EC864
022EC864 E0C3B0B2 strh    r11,[r3],2h           //[BPC_SecDBBEntryCurCpAddress] = r11, BPC_SecDBBEntryCurCpAddress += 2
022EC868 E2822001 add     r2,r2,1h              //r2 = r2 + 1
///022EC86C
022EC86C E1520001 cmp     r2,r1
if( r2 <= curbyte )
    022EC870 DAFFFFFB ble     22EC864h          ///022EC864 ^
022EC874 EA000005 b       22EC890h              ///022EC890 v
///022EC878
022EC878 E3A02000 mov     r2,0h                 //r2 = 0
022EC87C EA000001 b       22EC888h              ///022EC888 v
///022EC880
022EC880 E0C300B2 strh    r0,[r3],2h            //[BPC_SecDBBEntryCurCpAddress] = 0, BPC_SecDBBEntryCurCpAddress += 2
022EC884 E2822001 add     r2,r2,1h              //r2 = r2 + 1
///022EC888
022EC888 E1520001 cmp     r2,r1
if( r2 <= curbyte )
    022EC88C DAFFFFFB ble     22EC880h              ///022EC880 ^
///022EC890 
022EC890 E1530007 cmp     r3,r7
if( BPC_SecDBBEntryCurCpAddress < BPC_SecDBBEntryEndCpAddress ) //unsigned
    022EC894 3AFFFFDC bcc     22EC80Ch              ///022EC80C LOOP_BEG_TMDecompPass1 ^ 
    
022EC898 E1A0300C mov     r3,r12                //r3 = StartOfTheBufferWeJustWrote
022EC89C EA000021 b       22EC928h              ///022EC928 v

///
///022EC8A0 LOOP_BEG_TMDecompPass2 - Second pass for decompressing the tile mapping data
///
022EC8A0 E5D62000 ldrb    r2,[r6]
022EC8A4 E2860001 add     r0,r6,1h
022EC8A8 E1A06000 mov     r6,r0
022EC8AC E35200C0 cmp     r2,0C0h
if(r2 < 0xC0)
    022EC8B0 BA00000B blt     22EC8E4h              ///022EC8E4 v
022EC8B4 E3A000C0 mov     r0,0C0h
022EC8B8 EA000006 b       22EC8D8h              ///022EC8D8 v
///022EC8BC
022EC8BC E58D300C str     r3,[r13,0Ch]
022EC8C0 E0D3B0B2 ldrh    r11,[r3],2h
022EC8C4 E4D61001 ldrb    r1,[r6],1h
022EC8C8 E2800001 add     r0,r0,1h
022EC8CC E18BB001 orr     r11,r11,r1
022EC8D0 E59D100C ldr     r1,[r13,0Ch]
022EC8D4 E1C1B0B0 strh    r11,[r1]
///022EC8D8
022EC8D8 E1500002 cmp     r0,r2
if( r0 <= r2 )
    022EC8DC DAFFFFF6 ble     22EC8BCh              ///022EC8BC ^
022EC8E0 EA000010 b       22EC928h              ///022EC928 v
///022EC8E4
022EC8E4 E3520080 cmp     r2,80h
if(r2 < 0x80)
    022EC8E8 BA00000C blt     22EC920h              ///022EC920 v
022EC8EC E2806001 add     r6,r0,1h
022EC8F0 E5D0E000 ldrb    r14,[r0]
022EC8F4 E3A0B080 mov     r11,80h
022EC8F8 EA000005 b       22EC914h              ///022EC914 v
///022EC8FC
022EC8FC E58D3008 str     r3,[r13,8h]
022EC900 E0D300B2 ldrh    r0,[r3],2h
022EC904 E28BB001 add     r11,r11,1h
022EC908 E180100E orr     r1,r0,r14
022EC90C E59D0008 ldr     r0,[r13,8h]
022EC910 E1C010B0 strh    r1,[r0]
///022EC914
022EC914 E15B0002 cmp     r11,r2
if( r11 <= r2 )
    022EC918 DAFFFFF7 ble     22EC8FCh              ///022EC8FC ^
022EC91C EA000001 b       22EC928h              ///022EC928 v
///022EC920
022EC920 E2820001 add     r0,r2,1h
022EC924 E0833080 add     r3,r3,r0,lsl 1h
///022EC928
022EC928 E1530007 cmp     r3,r7
if( WritePos < BPC_SecDBBEntryEndCpAddress )
    022EC92C 3AFFFFDB bcc     22EC8A0h              ///022EC8A0 LOOP_BEG_TMDecompPass2 ^

022EC930 E1D900FA ldrsh   r0,[r9,0Ah]           //r0 = BPC_DBA_Unk6
022EC934 E3A03001 mov     r3,1h                 //r3 = 1
022EC938 E0800180 add     r0,r0,r0,lsl 3h       //r0 = BPC_DBA_Unk6 + (BPC_DBA_Unk6 * 8) 
022EC93C E08C1080 add     r1,r12,r0,lsl 1h      //r1 = StartOfTheBufferWeJustWrote + ((BPC_DBA_Unk6 + (BPC_DBA_Unk6 * 8)) * 2)
022EC940 E3A00000 mov     r0,0h                 //r0 = 0
022EC944 EA000008 b       22EC96Ch              ///022EC96C v

///
///022EC948
///
022EC948 E1A02000 mov     r2,r0                 //r2 = CNT_TileInf = 0
///022EC94C
022EC94C E177B0B2 ldrh    r11,[r7,-2h]!         
022EC950 E59D603C ldr     r6,[r13,3Ch]
022EC954 E2822001 add     r2,r2,1h              //r2 = CNT_TileInf + 1
022EC958 E08B6826 add     r6,r11,r6,lsr 10h
022EC95C E3520009 cmp     r2,9h
022EC960 E16160B2 strh    r6,[r1,-2h]!
if( CNT_TileInf < 9 )
    022EC964 BAFFFFF8 blt     22EC94Ch              ///022EC94C ^
022EC968 E2833001 add     r3,r3,1h
///022EC96C 
022EC96C E1D920FA ldrsh   r2,[r9,0Ah]           //r2 = BPC_DBA_Unk6
022EC970 E1530002 cmp     r3,r2
if( r3 < BPC_DBA_Unk6 )
    022EC974 BAFFFFF3 blt     22EC948h              ///022EC948 ^
022EC978 E3A01000 mov     r1,0h                 //r1 = 0
022EC97C E1A00001 mov     r0,r1                 //r0 = 0
///022EC980
022EC980 E2811001 add     r1,r1,1h              //r1 = r1 + 1
022EC984 E3510009 cmp     r1,9h                 
022EC988 E0CC00B2 strh    r0,[r12],2h           //[StartOfTheBufferWeJustWrote] = r0, StartOfTheBufferWeJustWrote += 2
if( r1 < 9)
    022EC98C BAFFFFFB blt     22EC980h              ///022EC980 ^

///022EC990
022EC990 E59D0020 ldr     r0,[r13,20h]          //r0 = [r13,20h] (was 0)
022EC994 E2888001 add     r8,r8,1h              //r8 = CNT_4 + 1
022EC998 E2800001 add     r0,r0,1h              //r0 = r0 + 1
022EC99C E58D0020 str     r0,[r13,20h]          //[r13,20h] = (r0 + 1)
///022EC9A0
022EC9A0 E28A0C02 add     r0,r10,200h           //
022EC9A4 E1D00CF0 ldrsh   r0,[r0,0C0h]          //r0 = [r0,0C0h] (was 2, at 0x210CD04)
022EC9A8 E1580000 cmp     r8,r0     
if( CNT_4 < r0 )
    022EC9AC BAFFFF08 blt     22EC5D4h              ///022EC5D4 LOOP_BEG_ParseBPCIndexTable ^

///
///022EC9B0 - After Parsing the BPC entry we end up here!
///
022EC9B0 E28A0FB9 add     r0,r10,2E4h               //r0 = PtrTablePtrMappingData? =r10 + 0x2E4 (Possibly destination struct?)
022EC9B4 E285100C add     r1,r5,0Ch                 //r1 = PtrBMAData = PtrBMABuff + 12
022EC9B8 E28A2FAF add     r2,r10,2BCh               //r2 = r10 + 0x2BC
022EC9BC E28A3E1E add     r3,r10,1E0h               //r3 = PtrBMAHeaderCopy = r10 + 0x1E0 (Map width from BMA!)
022EC9C0 EB000643 bl      22EE2D4h                  ///fun_022EE2D4
{
    022EE2D4 E92D4FF8 push    r3-r11,r14
    022EE2D8 E24DD010 sub     r13,r13,10h           //Alloc 16 bytes on stack
    022EE2DC E3A05000 mov     r5,0h                 //r5 = Cnt = 0
    022EE2E0 EA0000B8 b       22EE5C8h              ///022EE5C8 v
    ///022EE2E4 LOOP_BEG_BMA
    022EE2E4 E0444005 sub     r4,r4,r5              //r4  = r4 - Cnt
    022EE2E8 E0804104 add     r4,r0,r4,lsl 2h       //r4  = PtrTablePtrMappingData? + (r4 * 4)
    022EE2EC E514A004 ldr     r10,[r4,-4h]          //r10 = [r4 - 4]
    022EE2F0 E3A07000 mov     r7,0h                 //r7 = Cnt1 = 0
    022EE2F4 EA0000A3 b       22EE588h              ///022EE588 v
    ///022EE2F8 LOOP_BEG_1
    022EE2F8 E3570000 cmp     r7,0h
    022EE2FC E3A06000 mov     r6,0h                 //r6 = Cnt2 = 0
    if( r7 != 0 )
        022EE300 1A000042 bne     22EE410h          ///022EE410 v
    022EE304 EA00003D b       22EE400h              ///022EE400 v
    ///022EE308 LOOP_BEG_2
    022EE308 E4D19001 ldrb    r9,[r1],1h            //r9 = curbyte = [PtrReadBMADataPos], PtrReadBMADataPos += 1
    022EE30C E35900C0 cmp     r9,0C0h
    if( curbyte < 0xC0 )
        022EE310 BA000014 blt     22EE368h          ///022EE368 v
    022EE314 E3A080C0 mov     r8,0C0h               //r8 = 0xC0
    022EE318 EA00000D b       22EE354h              ///022EE354 v
    ///022EE31C
    022EE31C E5D1E000 ldrb    r14,[r1]              //r14 = [PtrReadBMADataPos]
    022EE320 E5D1C001 ldrb    r12,[r1,1h]           //r12 = [PtrReadBMADataPos + 1]
    022EE324 E5D14002 ldrb    r4,[r1,2h]            //r4  = [PtrReadBMADataPos + 2]
    022EE328 E2811003 add     r1,r1,3h              //r1  = PtrReadBMADataPos + 3
    022EE32C E18EC40C orr     r12,r14,r12,lsl 8h    //r12 = r14 | (r12 << 8)
    022EE330 E18C4804 orr     r4,r12,r4,lsl 10h     //r4  = r12 | (r4  << 16)
    022EE334 E59FC2A4 ldr     r12,=0FFFh            //r12 = 0x0FFF
    022EE338 E2888001 add     r8,r8,1h              //r8  = r8 + 1
    022EE33C E004C00C and     r12,r4,r12            //r12 = r4 & r12 
    022EE340 E1CAC0B0 strh    r12,[r10]             //[r10] = r12
    022EE344 E59FC294 ldr     r12,=0FFFh            //r12  = 0x0FFF
    022EE348 E00C4644 and     r4,r12,r4,asr 0Ch     //r4   = 0x0FFF & (r4 >> 12) (SIGNED Shift Right)
    022EE34C E1CA40B2 strh    r4,[r10,2h]           //[r10 + 2] = r4
    022EE350 E28AA004 add     r10,r10,4h            //r10 = r10 + 4
    ///022EE354
    022EE354 E1580009 cmp     r8,r9
    if( r8 <= curbyte )
        022EE358 DAFFFFEF ble     22EE31Ch          ///022EE31C ^ 
    022EE35C E24940BF sub     r4,r9,0BFh            //r4 = curbyte - 0xBF
    022EE360 E0866084 add     r6,r6,r4,lsl 1h       //r6 = Cnt2 + (r4 * 2)    
    022EE364 EA000025 b       22EE400h              ///022EE400 v
    ///022EE368 curbyte < 0xC0
    022EE368 E3590080 cmp     r9,80h
    if( curbyte < 0x80 )
        022EE36C BA000018 blt     22EE3D4h          //022EE3D4 curbyte < 0x80 v
    022EE370 E5D1C000 ldrb    r12,[r1]              //r12 = [PtrReadBMADataPos]
    022EE374 E5D1B001 ldrb    r11,[r1,1h]           //r11 = [PtrReadBMADataPos + 1]
    022EE378 E5D18002 ldrb    r8,[r1,2h]            //r8  = [PtrReadBMADataPos + 2]
    022EE37C E2811003 add     r1,r1,3h              //r1  = PtrReadBMADataPos + 3
    022EE380 E18CB40B orr     r11,r12,r11,lsl 8h    //r11 = r12 | (r11 << 8)    
    022EE384 E18BB808 orr     r11,r11,r8,lsl 10h    //r11 = r11 | (r8  << 16)
    022EE388 E59F8250 ldr     r8,=0FFFh             //r8  = 0x0FFF
    022EE38C E59FC24C ldr     r12,=0FFFh            //r12 = 0x0FFF
    022EE390 E00B8008 and     r8,r11,r8             //r8  = r11 & r8
    022EE394 E00CB64B and     r11,r12,r11,asr 0Ch   //r11 = r12 & (r11 >> 12) (SIGNED Shift Right)
    022EE398 E1A08808 mov     r8,r8,lsl 10h         //r8  = r8  << 16
    022EE39C E1A0B80B mov     r11,r11,lsl 10h       //r11 = r11 << 16
    022EE3A0 E3A04080 mov     r4,80h                //r4  = 0x80
    022EE3A4 E1A08828 mov     r8,r8,lsr 10h         //r8  = r8  >> 16
    022EE3A8 E1A0B82B mov     r11,r11,lsr 10h       //r11 = r11 >> 16
    022EE3AC EA000003 b       22EE3C0h              ///022EE3C0 v
    ///022EE3B0 LOOP_BEG_6
    022EE3B0 E1CA80B0 strh    r8,[r10]              //[r10]     = r8
    022EE3B4 E1CAB0B2 strh    r11,[r10,2h]          //[r10 + 2] = r11
    022EE3B8 E28AA004 add     r10,r10,4h            //r10 = r10 + 4
    022EE3BC E2844001 add     r4,r4,1h              //r4 = r4 + 1
    ///022EE3C0
    022EE3C0 E1540009 cmp     r4,r9
    if( r4 < r9 )
        022EE3C4 DAFFFFF9 ble     22EE3B0h          ///022EE3B0 LOOP_BEG_6 ^
    022EE3C8 E249407F sub     r4,r9,7Fh             //r4 = r9 - 0x7F
    022EE3CC E0866084 add     r6,r6,r4,lsl 1h       //r6 = r6 + (r4 * 2)
    022EE3D0 EA00000A b       22EE400h              ///022EE400 v
    ///022EE3D4 curbyte < 0x80
    022EE3D4 E3A0B000 mov     r11,0h                //r11 = CntWords = 0
    022EE3D8 EA000004 b       22EE3F0h              ///022EE3F0 v
    ///022EE3DC
    022EE3DC E3A04000 mov     r4,0h                 //r4       = 0
    022EE3E0 E1CA40B0 strh    r4,[r10]              //[r10]    = 0
    022EE3E4 E1CA40B2 strh    r4,[r10,2h]           //[r10,2h] = 0
    022EE3E8 E28AA004 add     r10,r10,4h            //r10      = r10 + 4
    022EE3EC E28BB001 add     r11,r11,1h            //r11      = CntWords + 1
    ///022EE3F0 
    022EE3F0 E15B0009 cmp     r11,r9
    if( CntWords <= curbyte )
        022EE3F4 DAFFFFF8 ble     22EE3DCh          ///022EE3DC ^
    022EE3F8 E2894001 add     r4,r9,1h              //r4 = curbyte + 1
    022EE3FC E0866084 add     r6,r6,r4,lsl 1h       //r6 = r6 + (curbyte * 2)
    ///022EE400
    022EE400 E5D34002 ldrb    r4,[r3,2h]            //r4 = BMA_unk3
    022EE404 E1560004 cmp     r6,r4
    if( r6 < BMA_unk3 )
        022EE408 BAFFFFBE blt     22EE308h          ///022EE308 LOOP_BEG_2
    022EE40C EA00005A b       22EE57Ch              ///022EE57C v
    ///022EE410
    022EE410 E24A9080 sub     r9,r10,80h            //r9 = r10 - 0x80
    022EE414 EA000051 b       22EE560h              ///022EE560 v
    ///022EE418 LOOP_BEG_7
    022EE418 E4D1B001 ldrb    r11,[r1],1h           //r11 = curbyte2 = [PtrReadBMADataPos], PtrReadBMADataPos += 1
    022EE41C E35B00C0 cmp     r11,0C0h
    if( curbyte2 < 0xC0 )
        022EE420 BA00001D blt     22EE49Ch          ///022EE49C
    022EE424 E3A0C0C0 mov     r12,0C0h              //r12 = 0xC0
    022EE428 EA000016 b       22EE488h              ///022EE488 v
    022EE42C E5D18001 ldrb    r8,[r1,1h]            //r8  = [PtrReadBMADataPos + 1]
    022EE430 E5D1E000 ldrb    r14,[r1]              //r14 = [PtrReadBMADataPos]
    022EE434 E5D14002 ldrb    r4,[r1,2h]            //r4  = [PtrReadBMADataPos + 2]
    022EE438 E58D800C str     r8,[r13,0Ch]          //[r13,0Ch] = r8
    022EE43C E1D980B0 ldrh    r8,[r9]               //r8  = [r9]
    022EE440 E2811003 add     r1,r1,3h              //r1  = r1 + 3
    022EE444 E28CC001 add     r12,r12,1h            //r12 = r12 + 1
    022EE448 E58D8004 str     r8,[r13,4h]           //[r13,4h] = r8
    022EE44C E59D800C ldr     r8,[r13,0Ch]          //r8  = [r13 + 0xC]
    022EE450 E18E8408 orr     r8,r14,r8,lsl 8h      //r8  = r14 | (r8 << 8)
    022EE454 E188E804 orr     r14,r8,r4,lsl 10h     //r14 = r8  | (r4 << 16)
    022EE458 E59F4180 ldr     r4,=0FFFh             //r4  = 0x0FFF
    022EE45C E00E8004 and     r8,r14,r4             //r8  = r14 & 0x0FFF
    022EE460 E59D4004 ldr     r4,[r13,4h]           //[r13,4h] = r4
    022EE464 E0244008 eor     r4,r4,r8              //r4  = r4 ^ r8
    022EE468 E1CA40B0 strh    r4,[r10]              //[r10] = r4
    022EE46C E59F416C ldr     r4,=0FFFh             //r4  = 0x0FFF
    022EE470 E004E64E and     r14,r4,r14,asr 0Ch    //r14 = r4 & (r14 >> 12) (SIGNED Shift Right)    
    022EE474 E1D940B2 ldrh    r4,[r9,2h]            //r4  = [r9 + 2]
    022EE478 E2899004 add     r9,r9,4h              //r9  = r9 + 4
    022EE47C E024400E eor     r4,r4,r14             //r4  = r4 ^ r14
    022EE480 E1CA40B2 strh    r4,[r10,2h]           //[r10,2h] = r4
    022EE484 E28AA004 add     r10,r10,4h            //r10 = r10 + 4
    ///022EE488
    022EE488 E15C000B cmp     r12,r11
    if( r12 <= curbyte2 )
        022EE48C DAFFFFE6 ble     22EE42Ch
    022EE490 E24B40BF sub     r4,r11,0BFh               //r4 = curbyte2 - 0xBF
    022EE494 E0866084 add     r6,r6,r4,lsl 1h           //r6 = r6 + (r4 * 2)
    022EE498 EA000030 b       22EE560h                  ///022EE560 v
    ///022EE49C
    022EE49C E35B0080 cmp     r11,80h
    if( curbyte2 < 0x80 )
        022EE4A0 BA00001F blt     22EE524h              ///022EE524 v
    022EE4A4 E5D14000 ldrb    r4,[r1]                   //r4  = [PtrReadBMADataPos]
    022EE4A8 E5D1E001 ldrb    r14,[r1,1h]               //r14 = [PtrReadBMADataPos + 1]   
    022EE4AC E5D18002 ldrb    r8,[r1,2h]                //r8  = [PtrReadBMADataPos + 2]    
    022EE4B0 E2811003 add     r1,r1,3h                  //r1  = PtrReadBMADataPos + 3
    022EE4B4 E184440E orr     r4,r4,r14,lsl 8h          //r4  = [PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)
    022EE4B8 E1844808 orr     r4,r4,r8,lsl 10h          //r4  = ([PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)) | ([PtrReadBMADataPos + 2]  << 16)
    022EE4BC E59F811C ldr     r8,=0FFFh                 //r8  = 0x0FFF
    022EE4C0 E3A0C080 mov     r12,80h                   //r12 = 0x80
    022EE4C4 E0048008 and     r8,r4,r8                  //r8  = (([PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)) | ([PtrReadBMADataPos + 2]  << 16)) & 0x0FFF
    022EE4C8 E58D8000 str     r8,[r13]                  //[r13] = AssembledDWord = ((([PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)) | ([PtrReadBMADataPos + 2]  << 16)) & 0x0FFF)
    022EE4CC E59F810C ldr     r8,=0FFFh                 //r8  = 0x0FFF    
    022EE4D0 E0084644 and     r4,r8,r4,asr 0Ch          //r4  = 0x0FFF &(([PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)) | ([PtrReadBMADataPos + 2]  << 16)) >> 12) (SIGNED Shift Right)
    022EE4D4 E58D4008 str     r4,[r13,8h]               //[r13,8h] = Assembled12BitsInt = (0x0FFF &(([PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)) | ([PtrReadBMADataPos + 2]  << 16)) >> 12) (SIGNED Shift Right))
    022EE4D8 EA00000C b       22EE510h                  ///022EE510 v
    ///022EE4DC
    022EE4DC E1D980B0 ldrh    r8,[r9]                   //r8  = [r9]
    022EE4E0 E59D4000 ldr     r4,[r13]                  //r4  = AssembledDWord
    022EE4E4 E289E002 add     r14,r9,2h                 //r14 = r9 + 2
    022EE4E8 E0284004 eor     r4,r8,r4                  //r4  = r8 ^ AssembledDWord
    022EE4EC E1CA40B0 strh    r4,[r10]                  //[r10] = r4
    022EE4F0 E1DEE0B0 ldrh    r14,[r14]                 //r14 = [r14]
    022EE4F4 E59D4008 ldr     r4,[r13,8h]               //r4  = Assembled12BitsInt
    022EE4F8 E28A8002 add     r8,r10,2h                 //r8  = r10 + 2
    022EE4FC E02E4004 eor     r4,r14,r4                 //r4  = r14 ^ Assembled12BitsInt
    022EE500 E2899004 add     r9,r9,4h                  //r9  = r9 + 4
    022EE504 E28AA004 add     r10,r10,4h                //r10 = r10 + 4    
    022EE508 E1C840B0 strh    r4,[r8]                   //[r8] = r4
    022EE50C E28CC001 add     r12,r12,1h                //r12 = r12 + 1
    ///022EE510
    022EE510 E15C000B cmp     r12,r11
    if( r12 <= r11 )
        022EE514 DAFFFFF0 ble     22EE4DCh              ///022EE4DC ^
    022EE518 E24B407F sub     r4,r11,7Fh                //r4 = r11 - 0x7F
    022EE51C E0866084 add     r6,r6,r4,lsl 1h           //r6 = r6 + (r4 * 2)
    022EE520 EA00000E b       22EE560h                  ///022EE560 v
    ///022EE524
    022EE524 E3A04000 mov     r4,0h                     //r4 = 0
    022EE528 EA000008 b       22EE550h                  ///022EE550 v
    ///022EE52C LOOP_BEG_8
    022EE52C E1D9C0B0 ldrh    r12,[r9]              //r12 = [r9]
    022EE530 E2898002 add     r8,r9,2h              //r8  = r9 + 2
    022EE534 E2899004 add     r9,r9,4h              //r9  = r9 + 4
    022EE538 E1CAC0B0 strh    r12,[r10]             //[r10] = r12
    022EE53C E1D880B0 ldrh    r8,[r8]               //r8  = [r8]
    022EE540 E28AC002 add     r12,r10,2h            //r12 = r10 + 2
    022EE544 E28AA004 add     r10,r10,4h            //r10 = r10 + 4
    022EE548 E1CC80B0 strh    r8,[r12]              //[r12] = r8
    022EE54C E2844001 add     r4,r4,1h              //r4 = r4 + 1
    ///022EE550
    022EE550 E154000B cmp     r4,r11
    if( r4 <= r11 )
        022EE554 DAFFFFF4 ble     22EE52Ch          ///022EE52C ^
    022EE558 E28B4001 add     r4,r11,1h
    022EE55C E0866084 add     r6,r6,r4,lsl 1h
    ///022EE560
    022EE560 E5D34002 ldrb    r4,[r3,2h]            //r4 = BMA_unk3
    022EE564 E1560004 cmp     r6,r4
    if( r6 < BMA_unk3 )
        022EE568 BAFFFFAA blt     22EE418h          ///022EE418 LOOP_BEG_7 ^
    022EE56C EA000002 b       22EE57Ch              ///022EE57C v
    ///022EE570 LOOP_BEG_3
    022EE570 E3A04000 mov     r4,0h                 //r4 = 0
    022EE574 E0CA40B2 strh    r4,[r10],2h           //[r10] = 0, r10 += 2
    022EE578 E2866001 add     r6,r6,1h              //r6 = r6 + 1
    ///022EE57C
    022EE57C E3560040 cmp     r6,40h
    if( r6 < 64 )
        022EE580 BAFFFFFA blt     22EE570h          ///022EE570 LOOP_BEG_3
    022EE584 E2877001 add     r7,r7,1h
    ///022EE588
    022EE588 E5D34003 ldrb    r4,[r3,3h]            //r4 = BMA_unk4
    022EE58C E1570004 cmp     r7,r4             
    if( r7 < BMA_unk4 )
        022EE590 BAFFFF58 blt     22EE2F8h          ///022EE2F8 LOOP_BEG_1
    022EE594 E3A06000 mov     r6,0h                 //r6 = 0
    022EE598 E1A08006 mov     r8,r6                 //r8 = 0
    022EE59C EA000005 b       22EE5B8h              ///022EE5B8 v
    ///022EE5A0 LOOP_BEG_4
    022EE5A0 E1A04008 mov     r4,r8                 //r4 = Cnt4 = 0
    ///022EE5A4 LOOP_BEG_5
    022EE5A4 E2844001 add     r4,r4,1h              //r4 = r4 + 1
    022EE5A8 E3540040 cmp     r4,40h
    022EE5AC E0CA60B2 strh    r6,[r10],2h           //[r10] = 0, r10 += 2
    if( r4 < 64 )
        022EE5B0 BAFFFFFB blt     22EE5A4h          ///022EE5A4 LOOP_BEG_5
    022EE5B4 E2877001 add     r7,r7,1h
    ///022EE5B8
    022EE5B8 E1D241F4 ldrsh   r4,[r2,14h]           //r4 = [r2,14h](was 0x3E)  
    022EE5BC E1570004 cmp     r7,r4
    if( r7 < r4 )
        022EE5C0 BAFFFFF6 blt     22EE5A0h          ///022EE5A0 LOOP_BEG_4 ^
    022EE5C4 E2855001 add     r5,r5,1h              //r5 = r5 + 1
    ///022EE5C8
    022EE5C8 E1D240F4 ldrsh   r4,[r2,4h]            //r4 = [r2,4h](was 2)
    022EE5CC E1550004 cmp     r5,r4
    if( Cnt < r4 )
        022EE5D0 BAFFFF43 blt     22EE2E4h          ///022EE2E4 LOOP_BEG_BMA
    022EE5D4 E1A00001 mov     r0,r1                 //r0 = PtrReadBMADataPos
    022EE5D8 E28DD010 add     r13,r13,10h
    022EE5DC E8BD8FF8 pop     r3-r11,r15
    022EE5E0 00000FFF 
}
022EC9C4 E1A01000 mov     r1,r0                 //r1 = PtrReadBMADataPos
022EC9C8 E58A11F0 str     r1,[r10,1F0h]         //[r10,1F0h] = PtrReadBMADataPos
022EC9CC E59A02D8 ldr     r0,[r10,2D8h]         //r0 = [r10,2D8h] (was 0x20F6644)
022EC9D0 E3500000 cmp     r0,0h
if( r0 == 0 )
    022EC9D4 0A000004 beq     22EC9ECh          ///022EC9EC v
022EC9D8 E28A2C02 add     r2,r10,200h           //r2 = r10 + 0x200
022EC9DC E1D23CFE ldrsh   r3,[r2,0CEh]          //r3 = [r2 + 0xCE] (was 0xBA)
022EC9E0 E59A52D4 ldr     r5,[r10,2D4h]         //r5 = [r10 + 0x2D4] (was 0x22F1800)
022EC9E4 E1A02004 mov     r2,r4                 //r2 = r4 (PtrBMABuff)
022EC9E8 E12FFF35 blx     r5                    ///Run r5 (Was 022F1800)
{
    
}
///022EC9EC
022EC9EC E3A01000 mov     r1,0h                 //r1 = 0
022EC9F0 E59D0040 ldr     r0,[r13,40h]          //r0 = PtrBPLHeaderCopy (r10 + 0x1EA)
022EC9F4 E5CA11EE strb    r1,[r10,1EEh]         //[r10,1EEh] = 0
022EC9F8 E1D020F2 ldrsh   r2,[r0,2h]            //r2 = BPL_Unk1
022EC9FC E28A0004 add     r0,r10,4h             //r0 = r10 + 4
022ECA00 E3520000 cmp     r2,0h
if(r2 == 0)
    022ECA04 0A000026 beq     22ECAA4h          ///022ECAA4 v
022ECA08 E59D2040 ldr     r2,[r13,40h]          //r2 = PtrBPLHeaderCopy (r10 + 0x1EA)
022ECA0C E3A03001 mov     r3,1h                 //r3 = 1
022ECA10 E1D250F0 ldrsh   r5,[r2]               //r5 = BPL_nbpal    
022ECA14 E59D2028 ldr     r2,[r13,28h]          //R2 = PtrBPLBuff
022ECA18 E1A07001 mov     r7,r1                 //r7 = r1
022ECA1C E2824004 add     r4,r2,4h              //R4 = PtrBPLBuff + 4
022ECA20 E0652205 rsb     r2,r5,r5,lsl 4h       //r2 = (r5 * 16) - r5
022ECA24 E0842102 add     r2,r4,r2,lsl 2h       //r2 = r4 + (((r5 * 16) - r5) * 4)
022ECA28 E58A21F4 str     r2,[r10,1F4h]         //[r10,1F4h] = r2
022ECA2C E5CA31F8 strb    r3,[r10,1F8h]         //[r10,1F8h] = 1
022ECA30 E5CA31F9 strb    r3,[r10,1F9h]         //[r10,1F9h] = 1
022ECA34 E5CA31FA strb    r3,[r10,1FAh]         //[r10,1FAh] = 1
022ECA38 E5CA31FB strb    r3,[r10,1FBh]         //[r10,1FBh] = 1
022ECA3C E0823105 add     r3,r2,r5,lsl 2h       //r3 = r2 + (r5 * 4)
022ECA40 E5CA11BC strb    r1,[r10,1BCh]         //[r10,1BCh] = r1
022ECA44 E28A5C02 add     r5,r10,200h           //r5 = r10 + 0x200
022ECA48 E1A06001 mov     r6,r1                 //r6 = r1
022ECA4C EA00000C b       22ECA84h              ///022ECA84 v
///022ECA50 
022ECA50 E1D240F2 ldrsh   r4,[r2,2h]            //r4 = [r2 + 2]
022ECA54 E2811001 add     r1,r1,1h              //r1 = r1 + 1
022ECA58 E3540000 cmp     r4,0h             
if( r4 > 0 )
    022ECA5C C5803004 strgt   r3,[r0,4h]        //[r0,4h] = r3
    022ECA60 C1D240F2 ldrgtsh r4,[r2,2h]        //r4 = [r2 + 2]
022ECA64 E2822004 add     r2,r2,4h              //r2 = r2 + 4
if( r4 <= 0 )
    022ECA68 D5807004 strle   r7,[r0,4h]        //[r0,4h] = r7
022ECA6C E1C060B0 strh    r6,[r0]               //[r0] = r6
if( r4 > 0 )
    022ECA70 C0644204 rsbgt   r4,r4,r4,lsl 4h   //r4 = (r4 * 16) - r4
022ECA74 E1C060B2 strh    r6,[r0,2h]            //[r0,2h] = r6
022ECA78 E5806008 str     r6,[r0,8h]            //[r0,8h] = r6
if( r4 > 0 )
    022ECA7C C0833104 addgt   r3,r3,r4,lsl 2h   //r3 = r3 + (r4 * 4)
022ECA80 E280000C add     r0,r0,0Ch             //r0 = r0 + 12
///022ECA84
022ECA84 E59D4040 ldr     r4,[r13,40h]          //r4 = PtrBPLHeaderCopy
022ECA88 E1D440F0 ldrsh   r4,[r4]               //r4 = [PtrBPLHeaderCopy] //BPL_nbpal
022ECA8C E1510004 cmp     r1,r4
if( r1 >= r4 )
    022ECA90 AA000009 bge     22ECABCh          ///022ECABC v
022ECA94 E1D54CF4 ldrsh   r4,[r5,0C4h]          //r4 = [r5 + 0xC4]
022ECA98 E1510004 cmp     r1,r4                 
if( r1 < r4 )
    022ECA9C BAFFFFEB blt     22ECA50h          ///022ECA50 ^
022ECAA0 EA000005 b       22ECABCh              ///022ECABC v
///022ECAA4
022ECAA4 E58A11F4 str     r1,[r10,1F4h]
022ECAA8 E5CA11F8 strb    r1,[r10,1F8h]
022ECAAC E5CA11F9 strb    r1,[r10,1F9h]
022ECAB0 E5CA11FA strb    r1,[r10,1FAh]
022ECAB4 E5CA11FB strb    r1,[r10,1FBh]
022ECAB8 E5CA11BC strb    r1,[r10,1BCh]
///022ECABC
022ECABC E3A02000 mov     r2,0h
022ECAC0 EA000005 b       22ECADCh              ///022ECADC v

///
///022ECAC4 LOOP_022ECAC4_BEG
///
022ECAC4 E1C020B0 strh    r2,[r0]
022ECAC8 E1C020B2 strh    r2,[r0,2h]
022ECACC E5802008 str     r2,[r0,8h]
022ECAD0 E5802004 str     r2,[r0,4h]
022ECAD4 E2811001 add     r1,r1,1h
022ECAD8 E280000C add     r0,r0,0Ch
///022ECADC
022ECADC E3510010 cmp     r1,10h
if( r1 < 16 )
    022ECAE0 BAFFFFF7 blt     22ECAC4h          ///022ECAC4 LOOP_022ECAC4_BEG ^
022ECAE4 E3A07000 mov     r7,0h
022ECAE8 E28A40C4 add     r4,r10,0C4h
022ECAEC E1A09007 mov     r9,r7
022ECAF0 E1A05007 mov     r5,r7
022ECAF4 E3A0802C mov     r8,2Ch

///
///022ECAF8 LOOP_022ECAF8_BEG
///
022ECAF8 E0000897 mul     r0,r7,r8
022ECAFC E0846000 add     r6,r4,r0
022ECB00 E7C49000 strb    r9,[r4,r0]
022ECB04 E5C69001 strb    r9,[r6,1h]
022ECB08 E1C690B4 strh    r9,[r6,4h]
022ECB0C E1C690B2 strh    r9,[r6,2h]
022ECB10 E2860008 add     r0,r6,8h
022ECB14 EBF459C3 bl      2003228h              ///fun_02003228 
{
    02003228 E3A01000 mov     r1,0h                 //r1 = 0
    0200322C E5801000 str     r1,[r0]               //[r0] = 0
    02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
    02003234 E12FFF1E bx      r14
}
022ECB18 E5865010 str     r5,[r6,10h]
022ECB1C E5865018 str     r5,[r6,18h]
022ECB20 E5865014 str     r5,[r6,14h]
022ECB24 E5865020 str     r5,[r6,20h]
022ECB28 E586501C str     r5,[r6,1Ch]
022ECB2C E2877001 add     r7,r7,1h
022ECB30 E5865024 str     r5,[r6,24h]
022ECB34 E5865028 str     r5,[r6,28h]           //PtrBPLBuff = R5
022ECB38 E3570004 cmp     r7,4h
if( r7 < 4 )
    022ECB3C BAFFFFED blt     22ECAF8h              ///022ECAF8 LOOP_022ECAF8_BEG ^
022ECB40 E28A0C02 add     r0,r10,200h
022ECB44 E1D07BFE ldrsh   r7,[r0,0BEh]
022ECB48 EA00004C b       22ECC80h              ///022ECC80 v

///
///022ECB4C LOOP_BPC_BPA_BEG
///
022ECB4C E3A0000C mov     r0,0Ch                //r0 = LenBPCSzTblEntry = 12
022ECB50 E0020095 mul     r2,r5,r0              //r2 = CNT_SzEntry * 12
022ECB54 E28A0C02 add     r0,r10,200h           //r0 = r10 + 0x200 (Address of the tileset data struct?)    
022ECB58 E28A3F72 add     r3,r10,1C8h           //r3 = BPCSizeTableBeg
022ECB5C E1D01CF6 ldrsh   r1,[r0,0C6h]          //was 0    
022ECB60 E19300F2 ldrsh   r0,[r3,r2]            //r0 = [BPCSizeTableBeg + (CNT_SzEntry * 12)]
022ECB64 E3550000 cmp     r5,0h
if( CNT_SzEntry == 0 )
    022ECB68 03A08000 moveq   r8,0h             //r8 = 0
022ECB6C E0811000 add     r1,r1,r0              //r1 = r1 + (r10 + 0x200)
022ECB70 E0830002 add     r0,r3,r2              //r0 = BPCSizeTableBeg + (CNT_SzEntry * 12)
if( CNT_SzEntry != 0 )
    022ECB74 13A08002 movne   r8,2h             //r8 = 2
022ECB78 E3550000 cmp     r5,0h
if( CNT_SzEntry == 0 )
    022ECB7C 03A09000 moveq   r9,0h             //r9 = 0
022ECB80 E58D0004 str     r0,[r13,4h]           //[r13 + 4h] = BPCSizeTableBeg + (CNT_SzEntry * 12)
022ECB84 E28A00C4 add     r0,r10,0C4h           //r0 = r10 + 0xC4
022ECB88 E5DA62BC ldrb    r6,[r10,2BCh]         //r6 = [r10 + 0x2BC]
022ECB8C E59F4184 ldr     r4,=2320C18h          //r4 = Table_02320C18 VRAM addresses table!! (Table with 5, 8 bytes entries made of 2 int 32) 
if( CNT_SzEntry != 0 )
    022ECB90 13A09004 movne   r9,4h             //r9 = 4
022ECB94 E0844186 add     r4,r4,r6,lsl 3h       //r4 = 0x2320C18 + ([r10 + 0x2BC] * 8)
022ECB98 E7944107 ldr     r4,[r4,r7,lsl 2h]     //r4 = (0x2320C18 + ([r10 + 0x2BC] * 8)) + (r7 * 4)
022ECB9C E3A06000 mov     r6,0h                 //r6  = CNT_BPASlot = 0    
022ECBA0 E084B281 add     r11,r4,r1,lsl 5h      //r11 = ((0x2320C18 + ([r10 + 0x2BC] * 8)) + (r7 * 4)) + (r1 << 5)
022ECBA4 E58D0030 str     r0,[r13,30h]          //[r13,30h] = (r10 + 0xC4)
///022ECBA8 LOOP_BPC_BPA2_BEG
022ECBA8 E59D0030 ldr     r0,[r13,30h]          //r0 = (r10 + 0xC4)
022ECBAC E3A0102C mov     r1,2Ch                //r1 = 0x2C
022ECBB0 E0240198 mla     r4,r8,r1,r0           //r4 = (r8 * 0x2C) + (r10 + 0xC4)
022ECBB4 E3A0000A mov     r0,0Ah                //r0 = 0xA    
022ECBB8 E0000099 mul     r0,r9,r0              //r0 = r9 * 0xA
022ECBBC E28D10CC add     r1,r13,0CCh           //r1 = r13 + 0xCC
022ECBC0 E0811000 add     r1,r1,r0              //r1 = (r13 + 0xCC) + (r9 * 0xA)
022ECBC4 E1D111DE ldrsb   r1,[r1,1Eh]           //r1 = [((r13 + 0xCC) + (r9 * 0xA))  0x1E]
022ECBC8 E3510000 cmp     r1,0h
if( r1 == 0 )
    022ECBCC 0A000024 beq     22ECC64h          ///022ECC64 v
022ECBD0 E5DA12BC ldrb    r1,[r10,2BCh]         //r1 = [r10 + 0x2BC]
022ECBD4 E3510000 cmp     r1,0h                 
if( r1 != 0 )
    022ECBD8 1A000021 bne     22ECC64h          ///022ECC64 v
022ECBDC E28D10EA add     r1,r13,0EAh           //r1 = r13 + 0xEA
022ECBE0 E0812000 add     r2,r1,r0              //r2 = (r13 + 0xEA) + r0
022ECBE4 E59F1134 ldr     r1,=2320C94h          //"rom0:/MAP_BG/%s.bpa"
022ECBE8 E28D004C add     r0,r13,4Ch            //r0 = r13 + 0x4C
022ECBEC EBF67264 bl      2089584h              ///fun_02089584 ReplaceTokens
022ECBF0 E59D2000 ldr     r2,[r13]              //r2 = [r13]
022ECBF4 E2840008 add     r0,r4,8h              //r0 = r4 + 8
022ECBF8 E28D104C add     r1,r13,4Ch            //r1 = r13 + 0x4C
022ECBFC EBF4700E bl      2008C3Ch              //LoadFileFromRom
022ECC00 E3A00001 mov     r0,1h                 //r0 = 1
022ECC04 E5C40000 strb    r0,[r4]               //[r4] = 1
022ECC08 E5C40001 strb    r0,[r4,1h]            //[r4,1h] = 1
022ECC0C E5943008 ldr     r3,[r4,8h]            //r3 = PtrBPABuff
022ECC10 E59D0004 ldr     r0,[r13,4h]           //r0 = (BPCSizeTableBeg + (CNT_SzEntry * 12)) //PtrBPCIndexEntry
022ECC14 E5843010 str     r3,[r4,10h]           //[r4,10h] = PtrBPABuff
022ECC18 E0801086 add     r1,r0,r6,lsl 1h       //r1 = PtrBPCIndexEntry + (CNT_BPASlot * 2)
022ECC1C E1D300F2 ldrsh   r0,[r3,2h]            //r0 = BPA_unk2
022ECC20 E2832004 add     r2,r3,4h              //r2 = PtrBPABuff + 4 //Possibly beginning of DatablockA
022ECC24 E5842018 str     r2,[r4,18h]           //[r4,18h] = (PtrBPABuff + 4)
022ECC28 E0820100 add     r0,r2,r0,lsl 2h       //r0 = (PtrBPABuff + 4) + (BPA_unk2 * 4)
022ECC2C E5842014 str     r2,[r4,14h]           //[r4,14h] = (PtrBPABuff + 4)
022ECC30 E5840020 str     r0,[r4,20h]           //[r4,20h] = ((PtrBPABuff + 4) + (BPA_unk2 * 4))
022ECC34 E584001C str     r0,[r4,1Ch]           //[r4,1Ch] = ((PtrBPABuff + 4) + (BPA_unk2 * 4))
022ECC38 E3A00000 mov     r0,0h                 //r0 = 0
022ECC3C E1C400B2 strh    r0,[r4,2h]            //[r4,2h] = 0
022ECC40 E5940018 ldr     r0,[r4,18h]           //r0 = (PtrBPABuff + 4)
022ECC44 E5900000 ldr     r0,[r0]               //r0 = [r0]
022ECC48 E1C400B4 strh    r0,[r4,4h]            //[r4,4h] = r0
022ECC4C E584B024 str     r11,[r4,24h]          //[r4,24h] = r11
022ECC50 E1D100F2 ldrsh   r0,[r1,2h]            //r0 = [r1,2h]
022ECC54 E1A00280 mov     r0,r0,lsl 5h
022ECC58 E5840028 str     r0,[r4,28h]           //PtrBPLBuff = R0 
022ECC5C E1D100F2 ldrsh   r0,[r1,2h]
022ECC60 E08BB280 add     r11,r11,r0,lsl 5h     //r11 = r11 + (r0 * 32)
///022ECC64
022ECC64 E2866001 add     r6,r6,1h              //r6 = CNT_BPASlot + 1
022ECC68 E3560002 cmp     r6,2h
022ECC6C E2899001 add     r9,r9,1h              //r9 = r9 + 1
022ECC70 E2888001 add     r8,r8,1h              //r8 = r8 + 1    
if( CNT_BPASlot < 2 )
    022ECC74 BAFFFFCB blt     22ECBA8h          ///022ECBA8 LOOP_BPC_BPA2_BEG ^
022ECC78 E2855001 add     r5,r5,1h              //r5 = CNT_SzEntry + 1
022ECC7C E2877001 add     r7,r7,1h
///022ECC80
022ECC80 E28A0C02 add     r0,r10,200h
022ECC84 E1D00CF0 ldrsh   r0,[r0,0C0h]
022ECC88 E1550000 cmp     r5,r0
if( CNT_SzEntry < r0 )
    022ECC8C BAFFFFAE blt     22ECB4Ch              ///022ECB4C LOOP_BPC_BPA_BEG ^ 

022ECC90 E3500000 cmp     r0,0h                 
022ECC94 E1A0000A mov     r0,r10                //r0 = r10
if( r0 <= 0 )
    022ECC98 DA000002 ble     22ECCA8h          ///022ECCA8 v
022ECC9C E3A01001 mov     r1,1h                 //r1 = 1
022ECCA0 EB00065E bl      22EE620h              ///fun_022EE620
022ECCA4 EA000001 b       22ECCB0h              ///022ECCB0 v
///022ECCA8
022ECCA8 E3A01000 mov     r1,0h                 //r1 = 0
022ECCAC EB00065B bl      22EE620h              ///fun_022EE620
///022ECCB0
022ECCB0 E59A1228 ldr     r1,[r10,228h]
022ECCB4 E28A0E21 add     r0,r10,210h
022ECCB8 E12FFF31 blx     r1
022ECCBC E3A01001 mov     r1,1h                 //r1 = 1
022ECCC0 E28A0F61 add     r0,r10,184h           //R0 = PTR_PtrBPCBuff
022ECCC4 E5CA12BA strb    r1,[r10,2BAh]
022ECCC8 EBF4595A bl      2003238h              //IsPointerNull(R0 = Ptr)
022ECCCC E3500000 cmp     r0,0h
if( r0 != 0 )
    022ECCD0 1A000001 bne     22ECCDCh          ///022ECCDC v
022ECCD4 E28A0F61 add     r0,r10,184h           //R0 = PTR_PtrBPCBuff
022ECCD8 EBF46FBD bl      2008BD4h              ///fun_02008BD4
///022ECCDC
022ECCDC E28A0F63 add     r0,r10,18Ch           //R0 = PTR_PtrBMABuff    
022ECCE0 EBF45954 bl      2003238h              //IsPointerNull(R0 = Ptr)
022ECCE4 E3500000 cmp     r0,0h
if( r0 != 0 )
    022ECCE8 1A000001 bne     22ECCF4h          ///022ECCF4 v  
022ECCEC E28A0F63 add     r0,r10,18Ch           //R0 = PTR_PtrBMABuff
022ECCF0 EBF46FB7 bl      2008BD4h              ///fun_02008BD4
///022ECCF4 LBL2
022ECCF4 E28DDF4F add     r13,r13,13Ch
022ECCF8 E8BD8FF0 pop     r4-r11,r15
022ECCFC 02320C58 
022ECD00 02320C6C 
022ECD04 0000030F 
022ECD08 02320C80 
022ECD0C 02324CBC 
022ECD10 02320BF4 
022ECD14 02320BE8 
022ECD18 02320C18
022ECD1C 0000FFFF 
022ECD20 02320C94 






/*
    fun_022ECD24 22ECD24h
        References to strings for loading map files
*/
022ECD24 E92D4FF8 push    r3-r11,r14
022ECD28 E24DDF4A sub     r13,r13,128h
022ECD2C E1A06001 mov     r6,r1                     //r6 = r1
022ECD30 E3E07000 mvn     r7,0h                     //r7 = 0xFFFFFFFF
022ECD34 E1A05002 mov     r5,r2                     //r5 = r2
022ECD38 E1560007 cmp     r6,r7                     
if( r6 != r7 )
    022ECD3C 15D52000 ldrneb  r2,[r5]               //r2 = 
022ECD40 E1A0A000 mov     r10,r0                    //r0 = r10
022ECD44 E1A04003 mov     r4,r3                     //r4 = r3
022ECD48 135200FF cmpne   r2,0FFh
022ECD4C 1A000002 bne     22ECD5Ch
022ECD50 E1A0000A mov     r0,r10
022ECD54 EBFFFCCC bl      22EC08Ch
022ECD58 EA00012D b       22ED214h
022ECD5C EBFFFD46 bl      22EC27Ch                  /// LoadBGTileSets
022ECD60 E1A0000A mov     r0,r10
022ECD64 EBFFFC97 bl      22EBFC8h
022ECD68 E3A00002 mov     r0,2h
022ECD6C E1CA00B0 strh    r0,[r10]
022ECD70 E3A00000 mov     r0,0h
022ECD74 E5CA01C0 strb    r0,[r10,1C0h]
022ECD78 E28A0C01 add     r0,r10,100h
022ECD7C E1C06BBE strh    r6,[r0,0BEh]
022ECD80 E1D01BFE ldrsh   r1,[r0,0BEh]
022ECD84 E28D00B8 add     r0,r13,0B8h
022ECD88 EBFFFB6C bl      22EBB40h
022ECD8C E59F1488 ldr     r1,=2320C58h              //"rom0:/MAP_BG/%s.bpl"
022ECD90 E28D0038 add     r0,r13,38h
022ECD94 E28D20B8 add     r2,r13,0B8h
022ECD98 EBF671F9 bl      2089584h                  ///fun_02089584 ReplaceTokens
022ECD9C E28A0F5D add     r0,r10,174h               //R0 = PTR_PtrBPLBuff
022ECDA0 E28D1038 add     r1,r13,38h
022ECDA4 E3A02006 mov     r2,6h                     //r2 = 6
022ECDA8 EBF46FA3 bl      2008C3Ch                  //LoadFileFromRom
022ECDAC E59F146C ldr     r1,=2320C6Ch              //"rom0:/MAP_BG/%s.bpc"
022ECDB0 E28D0038 add     r0,r13,38h
022ECDB4 E28D20C2 add     r2,r13,0C2h
022ECDB8 EBF671F1 bl      2089584h                  ///fun_02089584 ReplaceTokens
022ECDBC E59F2460 ldr     r2,=30Fh                  //r2 = 0x30F
022ECDC0 E28A0F61 add     r0,r10,184h               //R0 = PTR_PtrBPCBuff
022ECDC4 E28D1038 add     r1,r13,38h
022ECDC8 EBF46F9B bl      2008C3Ch                  //LoadFileFromRom
022ECDCC E59F1454 ldr     r1,=2320C80h              //"rom0:/MAP_BG/%s.bma"
022ECDD0 E28D0038 add     r0,r13,38h
022ECDD4 E28D20CC add     r2,r13,0CCh
022ECDD8 EBF671E9 bl      2089584h                  ///fun_02089584 ReplaceTokens
022ECDDC E28A0F63 add     r0,r10,18Ch               //R0 = PTR_PtrBMABuff
022ECDE0 E28D1038 add     r1,r13,38h
022ECDE4 E3A02006 mov     r2,6h                     //r2 = 6
022ECDE8 EBF46F93 bl      2008C3Ch                  //LoadFileFromRom
022ECDEC E28A10EA add     r1,r10,0EAh
022ECDF0 E59A0184 ldr     r0,[r10,184h]             //R0 = PtrBPCBuff
022ECDF4 E59A9174 ldr     r9,[r10,174h]             //R9 = PtrBPLBuff
022ECDF8 E281BC01 add     r11,r1,100h
022ECDFC E5D91000 ldrb    r1,[r9]                   //r1 = BPLNbPalettes?
022ECE00 E59A618C ldr     r6,[r10,18Ch]             //R6 = PtrBMABuff
022ECE04 E28A3C01 add     r3,r10,100h
022ECE08 E1C31EBA strh    r1,[r3,0EAh]              //[r3,0EAh] = BPLNbPalettes
022ECE0C E5D99002 ldrb    r9,[r9,2h]                //r9 = BPLUnk2
022ECE10 E2802002 add     r2,r0,2h                  //r2 = PtrBPCBuff + 2
022ECE14 E28A8F72 add     r8,r10,1C8h               //r8 = r10 + 0x1C8
022ECE18 E1CB90B2 strh    r9,[r11,2h]               //[r11,2h] = BPLUnk2
022ECE1C E1D000F0 ldrsh   r0,[r0]                   //r0 = BPC_offsetfirstimg
022ECE20 E28A7E1E add     r7,r10,1E0h               //r7 = r10 + 0x1E0 (Map width BMA)
022ECE24 E3A01000 mov     r1,0h                     //r1 = 0
022ECE28 E1C30CB8 strh    r0,[r3,0C8h]              //[r3,0C8h] = BPC_offsetfirstimg //[r10 + 0x1C8]
///022ECE2C
022ECE2C E0D230B2 ldrh    r3,[r2],2h                //r3 = CurrentBPCIndexTblEntryWord
022ECE30 E0880081 add     r0,r8,r1,lsl 1h           //r0 = r8 + (Cnt * 2)
022ECE34 E2811001 add     r1,r1,1h                  //r1 = Cnt + 1
022ECE38 E1C030B2 strh    r3,[r0,2h]                //[r0 + 2] =  r3
022ECE3C E3510004 cmp     r1,4h
if( Cnt < 4 )
    022ECE40 BAFFFFF9 blt     22ECE2Ch              ///022ECE2C
022ECE44 E1D210B0 ldrh    r1,[r2]
022ECE48 E59F93DC ldr     r9,=2320BE4h
022ECE4C E28D0024 add     r0,r13,24h
022ECE50 E1C810BA strh    r1,[r8,0Ah]
022ECE54 E5D63000 ldrb    r3,[r6]
022ECE58 E286100C add     r1,r6,0Ch
022ECE5C E28A2FAF add     r2,r10,2BCh
022ECE60 E5C73000 strb    r3,[r7]
022ECE64 E5D6B001 ldrb    r11,[r6,1h]
022ECE68 E28A3E1E add     r3,r10,1E0h
022ECE6C E5C7B001 strb    r11,[r7,1h]
022ECE70 E5D6B004 ldrb    r11,[r6,4h]
022ECE74 E5C7B002 strb    r11,[r7,2h]
022ECE78 E5D6B005 ldrb    r11,[r6,5h]
022ECE7C E5C7B003 strb    r11,[r7,3h]
022ECE80 E5D6B006 ldrb    r11,[r6,6h]
022ECE84 E1C7B0B4 strh    r11,[r7,4h]
022ECE88 E5D6B008 ldrb    r11,[r6,8h]
022ECE8C E1C7B0B6 strh    r11,[r7,6h]
022ECE90 E5D6600A ldrb    r6,[r6,0Ah]
022ECE94 E1C760B8 strh    r6,[r7,8h]
022ECE98 E5996020 ldr     r6,[r9,20h]
022ECE9C E599901C ldr     r9,[r9,1Ch]
022ECEA0 E58D6028 str     r6,[r13,28h]
022ECEA4 E58D9024 str     r9,[r13,24h]
022ECEA8 E59A62D8 ldr     r6,[r10,2D8h]
022ECEAC E58D6024 str     r6,[r13,24h]
022ECEB0 EB000507 bl      22EE2D4h
022ECEB4 E1A06000 mov     r6,r0
022ECEB8 E58A61F0 str     r6,[r10,1F0h]
022ECEBC E59A12E4 ldr     r1,[r10,2E4h]
022ECEC0 E3A00040 mov     r0,40h
022ECEC4 E88D0022 stmea   [r13],r1,r5
022ECEC8 E58D4008 str     r4,[r13,8h]
022ECECC E58D000C str     r0,[r13,0Ch]
022ECED0 E5D71003 ldrb    r1,[r7,3h]
022ECED4 E59F2354 ldr     r2,=6018000h
022ECED8 E28D002C add     r0,r13,2Ch
022ECEDC E58D1010 str     r1,[r13,10h]
022ECEE0 E59A32D8 ldr     r3,[r10,2D8h]
022ECEE4 E28A1F65 add     r1,r10,194h
022ECEE8 E58D3014 str     r3,[r13,14h]
022ECEEC E59A32DC ldr     r3,[r10,2DCh]
022ECEF0 EBFF5A90 bl      22C3938h
022ECEF4 E59D202C ldr     r2,[r13,2Ch]
022ECEF8 E59D0030 ldr     r0,[r13,30h]
022ECEFC E1A01006 mov     r1,r6
022ECF00 E58A217C str     r2,[r10,17Ch]
022ECF04 E58A0180 str     r0,[r10,180h]
022ECF08 E59D2034 ldr     r2,[r13,34h]
022ECF0C E28A0C02 add     r0,r10,200h
022ECF10 E58A21B8 str     r2,[r10,1B8h]
022ECF14 E1D03CFE ldrsh   r3,[r0,0CEh]
022ECF18 E59A02D8 ldr     r0,[r10,2D8h]
022ECF1C E59A42D4 ldr     r4,[r10,2D4h]
022ECF20 E1A02007 mov     r2,r7
022ECF24 E12FFF34 blx     r4
022ECF28 E3A00C02 mov     r0,200h
022ECF2C E3A01000 mov     r1,0h
022ECF30 E1C800B0 strh    r0,[r8]
022ECF34 E1A07001 mov     r7,r1
022ECF38 E0880081 add     r0,r8,r1,lsl 1h
022ECF3C E2811001 add     r1,r1,1h
022ECF40 E1C070B2 strh    r7,[r0,2h]
022ECF44 E3510004 cmp     r1,4h
022ECF48 BAFFFFFA blt     22ECF38h
022ECF4C E3A00E19 mov     r0,190h
022ECF50 E1C800BA strh    r0,[r8,0Ah]
022ECF54 E28A0C01 add     r0,r10,100h
022ECF58 E3A0100C mov     r1,0Ch
022ECF5C E1C01EBA strh    r1,[r0,0EAh]
022ECF60 E1C07EBC strh    r7,[r0,0ECh]
022ECF64 E59A61B8 ldr     r6,[r10,1B8h]
022ECF68 E3560000 cmp     r6,0h
022ECF6C 0A00005B beq     22ED0E0h
022ECF70 E59F02BC ldr     r0,=2324CBCh                  //Pointer to Pointer, or pointer table?
022ECF74 E5DA12BC ldrb    r1,[r10,2BCh]
022ECF78 E5905000 ldr     r5,[r0]
022ECF7C E3A00028 mov     r0,28h
022ECF80 E1640081 smulbb  r4,r1,r0
022ECF84 E59F32AC ldr     r3,=2320BE4h
022ECF88 E28D2018 add     r2,r13,18h
022ECF8C E1A08007 mov     r8,r7
022ECF90 E2855098 add     r5,r5,98h
022ECF94 E3A01004 mov     r1,4h
022ECF98 E4D30001 ldrb    r0,[r3],1h
022ECF9C E2511001 subs    r1,r1,1h
022ECFA0 E4C20001 strb    r0,[r2],1h
022ECFA4 1AFFFFFB bne     22ECF98h
022ECFA8 EA00002B b       22ED05Ch
022ECFAC E2870001 add     r0,r7,1h
022ECFB0 E1A03800 mov     r3,r0,lsl 10h
022ECFB4 E1A01007 mov     r1,r7
022ECFB8 E59F227C ldr     r2,=2320BF4h
022ECFBC E0850004 add     r0,r5,r4
022ECFC0 E1A07823 mov     r7,r3,lsr 10h
022ECFC4 EBF47571 bl      200A590h
022ECFC8 E2866004 add     r6,r6,4h
022ECFCC E3A09001 mov     r9,1h
022ECFD0 E28DB020 add     r11,r13,20h
022ECFD4 E28D3018 add     r3,r13,18h
022ECFD8 E28D201C add     r2,r13,1Ch
022ECFDC E3A01004 mov     r1,4h
022ECFE0 E4D30001 ldrb    r0,[r3],1h
022ECFE4 E2511001 subs    r1,r1,1h
022ECFE8 E4C20001 strb    r0,[r2],1h
022ECFEC 1AFFFFFB bne     22ECFE0h
022ECFF0 E5D60000 ldrb    r0,[r6]
022ECFF4 E28D301C add     r3,r13,1Ch
022ECFF8 E28D2020 add     r2,r13,20h
022ECFFC E5CD001C strb    r0,[r13,1Ch]
022ED000 E5D60001 ldrb    r0,[r6,1h]
022ED004 E3A01004 mov     r1,4h
022ED008 E5CD001D strb    r0,[r13,1Dh]
022ED00C E5D60002 ldrb    r0,[r6,2h]
022ED010 E5CD001E strb    r0,[r13,1Eh]
022ED014 E5D60003 ldrb    r0,[r6,3h]
022ED018 E5CD001F strb    r0,[r13,1Fh]
022ED01C E4D30001 ldrb    r0,[r3],1h
022ED020 E2511001 subs    r1,r1,1h
022ED024 E4C20001 strb    r0,[r2],1h
022ED028 1AFFFFFB bne     22ED01Ch
022ED02C E2870001 add     r0,r7,1h
022ED030 E1A01007 mov     r1,r7
022ED034 E1A03800 mov     r3,r0,lsl 10h
022ED038 E1A0200B mov     r2,r11
022ED03C E0850004 add     r0,r5,r4
022ED040 E1A07823 mov     r7,r3,lsr 10h
022ED044 EBF47551 bl      200A590h
022ED048 E2899001 add     r9,r9,1h
022ED04C E3590010 cmp     r9,10h
022ED050 E2866004 add     r6,r6,4h
022ED054 BAFFFFDE blt     22ECFD4h
022ED058 E2888001 add     r8,r8,1h
022ED05C E358000C cmp     r8,0Ch
022ED060 AA000003 bge     22ED074h
022ED064 E28A0C02 add     r0,r10,200h
022ED068 E1D00CF4 ldrsh   r0,[r0,0C4h]
022ED06C E1580000 cmp     r8,r0
022ED070 BAFFFFCD blt     22ECFACh
022ED074 E28A6C02 add     r6,r10,200h
022ED078 E59FB1BC ldr     r11,=2320BF4h
022ED07C EA000012 b       22ED0CCh
022ED080 E2870001 add     r0,r7,1h
022ED084 E1A03800 mov     r3,r0,lsl 10h
022ED088 E1A01007 mov     r1,r7
022ED08C E1A0200B mov     r2,r11
022ED090 E0850004 add     r0,r5,r4
022ED094 E1A07823 mov     r7,r3,lsr 10h
022ED098 EBF4753C bl      200A590h
022ED09C E3A09001 mov     r9,1h
022ED0A0 E2870001 add     r0,r7,1h
022ED0A4 E1A03800 mov     r3,r0,lsl 10h
022ED0A8 E1A01007 mov     r1,r7
022ED0AC E59F218C ldr     r2,=2320BE8h
022ED0B0 E0850004 add     r0,r5,r4
022ED0B4 E1A07823 mov     r7,r3,lsr 10h
022ED0B8 EBF47534 bl      200A590h
022ED0BC E2899001 add     r9,r9,1h
022ED0C0 E3590010 cmp     r9,10h
022ED0C4 BAFFFFF5 blt     22ED0A0h
022ED0C8 E2888001 add     r8,r8,1h
022ED0CC E1D60CF4 ldrsh   r0,[r6,0C4h]
022ED0D0 E1580000 cmp     r8,r0
022ED0D4 BAFFFFE9 blt     22ED080h
022ED0D8 E0850004 add     r0,r5,r4
022ED0DC EBF47508 bl      200A504h
022ED0E0 E3A01000 mov     r1,0h
022ED0E4 E5CA11EE strb    r1,[r10,1EEh]
022ED0E8 E59F0144 ldr     r0,=2324CBCh
022ED0EC E5DA22BC ldrb    r2,[r10,2BCh]
022ED0F0 E5903000 ldr     r3,[r0]
022ED0F4 E3A00028 mov     r0,28h
022ED0F8 E5CA11BC strb    r1,[r10,1BCh]
022ED0FC E59A119C ldr     r1,[r10,19Ch]
022ED100 E28A4004 add     r4,r10,4h
022ED104 E3510000 cmp     r1,0h
022ED108 E2833098 add     r3,r3,98h
022ED10C E1610082 smulbb  r1,r2,r0
022ED110 0A000009 beq     22ED13Ch
022ED114 E28A0E1A add     r0,r10,1A0h
022ED118 E0831001 add     r1,r3,r1
022ED11C E3A02020 mov     r2,20h
022ED120 EBF5D940 bl      2063628h
022ED124 E59A119C ldr     r1,[r10,19Ch]
022ED128 E28A0E1A add     r0,r10,1A0h
022ED12C E3A02020 mov     r2,20h
022ED130 EBF5D97F bl      2063734h
022ED134 E3A00001 mov     r0,1h
022ED138 E5CA01BC strb    r0,[r10,1BCh]
022ED13C E3A00000 mov     r0,0h
022ED140 E58A01F4 str     r0,[r10,1F4h]
022ED144 E5CA01F8 strb    r0,[r10,1F8h]
022ED148 E5CA01F9 strb    r0,[r10,1F9h]
022ED14C E5CA01FA strb    r0,[r10,1FAh]
022ED150 E5CA01FB strb    r0,[r10,1FBh]
022ED154 E1A05000 mov     r5,r0
022ED158 E1C450B0 strh    r5,[r4]
022ED15C E1C450B2 strh    r5,[r4,2h]
022ED160 E2800001 add     r0,r0,1h
022ED164 E5845008 str     r5,[r4,8h]
022ED168 E5845004 str     r5,[r4,4h]
022ED16C E3500010 cmp     r0,10h
022ED170 E284400C add     r4,r4,0Ch
022ED174 BAFFFFF7 blt     22ED158h
022ED178 E28A30C4 add     r3,r10,0C4h
022ED17C E3A01000 mov     r1,0h
022ED180 E3A0002C mov     r0,2Ch
022ED184 E0020095 mul     r2,r5,r0
022ED188 E7C31002 strb    r1,[r3,r2]
022ED18C E0832002 add     r2,r3,r2
022ED190 E5C21001 strb    r1,[r2,1h]
022ED194 E1C210B4 strh    r1,[r2,4h]
022ED198 E1C210B2 strh    r1,[r2,2h]
022ED19C E5821010 str     r1,[r2,10h]
022ED1A0 E5821018 str     r1,[r2,18h]
022ED1A4 E5821014 str     r1,[r2,14h]
022ED1A8 E5821020 str     r1,[r2,20h]
022ED1AC E582101C str     r1,[r2,1Ch]
022ED1B0 E2855001 add     r5,r5,1h
022ED1B4 E5821024 str     r1,[r2,24h]
022ED1B8 E5821028 str     r1,[r2,28h]
022ED1BC E3550004 cmp     r5,4h
022ED1C0 BAFFFFEF blt     22ED184h
022ED1C4 E1A0000A mov     r0,r10
022ED1C8 E3A01001 mov     r1,1h
022ED1CC EB000513 bl      22EE620h
022ED1D0 E59A1228 ldr     r1,[r10,228h]
022ED1D4 E28A0E21 add     r0,r10,210h
022ED1D8 E12FFF31 blx     r1
022ED1DC E3A01001 mov     r1,1h
022ED1E0 E28A0F61 add     r0,r10,184h           //R0 = PtrBPCBuff
022ED1E4 E5CA12BA strb    r1,[r10,2BAh]
022ED1E8 EBF45812 bl      2003238h              //IsPointerNull(R0 = Ptr)
022ED1EC E3500000 cmp     r0,0h
022ED1F0 1A000001 bne     22ED1FCh
022ED1F4 E28A0F61 add     r0,r10,184h           //R0 = PtrBPCBuff
022ED1F8 EBF46E75 bl      2008BD4h
022ED1FC E28A0F63 add     r0,r10,18Ch           //R0 = PTR_PtrBMABuff
022ED200 EBF4580C bl      2003238h              //IsPointerNull(R0 = Ptr)
022ED204 E3500000 cmp     r0,0h
022ED208 1A000001 bne     22ED214h
022ED20C E28A0F63 add     r0,r10,18Ch           //R0 = PTR_PtrBMABuff
022ED210 EBF46E6F bl      2008BD4h
022ED214 E28DDF4A add     r13,r13,128h
022ED218 E8BD8FF8 pop     r3-r11,r15
022ED21C 02320C58 
022ED220 02320C6C 
022ED224 0000030F 
022ED228 02320C80 
022ED22C 02320BE4 
022ED230 06018000 
022ED234 02324CBC 
022ED238 02320BE4 
022ED23C 02320BF4 
022ED240 02320BE8 




/*
022F2570 E59F0020 ldr     r0,=2324CC8h              ///CurMapDataPtr?
022F2574 E5902000 ldr     r2,[r0]
022F2578 E5901004 ldr     r1,[r0,4h]
022F257C E282000C add     r0,r2,0Ch


    fun_022EFD5C 22EFD5Ch
    r0=PtrToPtrLvlData?, r1=PtrLvlData?
    
        Seems to do level specific stuff?
*/
022EFD5C E92D4038 push    r3-r5,r14
022EFD60 E1A05001 mov     r5,r1             //r5 = PtrLvlData?
022EFD64 E5D512BC ldrb    r1,[r5,2BCh]      //r1 = (Value a [r10,2BCh] )
022EFD68 E3A02000 mov     r2,0h             //r2 = 0
022EFD6C E5805000 str     r5,[r0]           //[r0] = PtrCurLevelData
022EFD70 E5802004 str     r2,[r0,4h]        //Zero level data
022EFD74 E5802008 str     r2,[r0,8h]
022EFD78 E580200C str     r2,[r0,0Ch]
022EFD7C E5802010 str     r2,[r0,10h]
022EFD80 E5802014 str     r2,[r0,14h]
022EFD84 E5802018 str     r2,[r0,18h]
022EFD88 E2850C01 add     r0,r5,100h        //(basically r10 + 100h)
022EFD8C E3510000 cmp     r1,0h
if( r1 == 0 )
    022EFD90 03A01000 moveq   r1,0h
else
    022EFD94 13A01001 movne   r1,1h

//
// 022EFD98 - Check level ID Pass 1
//
022EFD98 E1D00BFE ldrsh   r0,[r0,0BEh]      //r0 = levelid  [r10,1BEh]  (Was 0x11F)
022EFD9C E20140FF and     r4,r1,0FFh        
022EFDA0 E3500E16 cmp     r0,160h
if( levelid > 0x160 )
    022EFDA4 CA000051 bgt     22EFEF0h      ///022EFEF0 v
else if( levelid >= 0x160 )
    022EFDA8 AA0000D9 bge     22F0114h      ///022F0114 v
022EFDAC E59F3604 ldr     r3,=113h
022EFDB0 E1500003 cmp     r0,r3
if( levelid > 0x113 )
    022EFDB4 CA000022 bgt     22EFE44h      ///022EFE44 v
022EFDB8 E2431001 sub     r1,r3,1h          //r1 = 113h - 1
022EFDBC E1500001 cmp     r0,r1
if( levelid < 0x112 )
    022EFDC0 BA000003 blt     22EFDD4h      ///022EFDD4 v
else if( levelid == 0x112 )
    022EFDC4 0A00008A beq     22EFFF4h      ///022EFFF4 v
022EFDC8 E1500003 cmp     r0,r3
if( levelid == 0x113 )
    022EFDCC 0A00016E beq     22F038Ch      ///022F038C v
022EFDD0 EA000174 b       22F03A8h          ///022F03A8 - Return 1 v

//
// 022EFD98 - Check level ID ( levelid < 0x112 )
//
022EFDD4 E35000DC cmp     r0,0DCh
if( levelid > 0xDC )
    022EFDD8 CA000009 bgt     22EFE04h      ///022EFE04 v
else if( levelid >= 0xDC )
    022EFDDC AA000084 bge     22EFFF4h      ///022EFFF4 v
022EFDE0 E3500085 cmp     r0,85h
if( levelid > 0x85 )
    022EFDE4 CA000003 bgt     22EFDF8h      ///022EFDF8 v
else if( levelid >= 0x85 )
    022EFDE8 AA000081 bge     22EFFF4h      ///022EFFF4 v
022EFDEC E3500008 cmp     r0,8h
if( levelid == 8 )
    022EFDF0 0A000084 beq     22F0008h      ///022F0008 v
022EFDF4 EA00016B b       22F03A8h          ///022F03A8 - Return 1 v
///022EFDF8 - ( levelid > 0x85 )
022EFDF8 E35000D4 cmp     r0,0D4h
if( levelid == 0xD4 )
    022EFDFC 0A00007C beq     22EFFF4h      ///022EFFF4 v
022EFE00 EA000168 b       22F03A8h          ///022F03A8 - Return 1 v
///022EFE04 - ( levelid > 0xDC )
022EFE04 E3500F43 cmp     r0,10Ch
if( levelid > 0x10C )
    022EFE08 CA000009 bgt     22EFE34h      ///022EFE34 v
else if( levelid >= 0x10C )
    022EFE0C AA000167 bge     22F03B0h      ///022F03B0 - Return 0 v
022EFE10 E35000F5 cmp     r0,0F5h
if( levelid > 0xF5 )
    022EFE14 CA000163 bgt     22F03A8h      ///022F03A8 - Return 1 v
022EFE18 E35000F3 cmp     r0,0F3h
if( levelid < 0xF3 )
    022EFE1C BA000161 blt     22F03A8h      ///022F03A8 - Return 1 v
else if( levelid == 0xF3 )
    022EFE20 0A00008F beq     22F0064h      ///022F0064 v
022EFE24 E35000F4 cmp     r0,0F4h
022EFE28 135000F5 cmpne   r0,0F5h
if( levelid == 0xF4 || levelid == 0xF5 )
    022EFE2C 0A00007D beq     22F0028h      ///022F0028 v
022EFE30 EA00015C b       22F03A8h          ///022F03A8 - Return 1 v
///022EFE34 - ( levelid > 0x10C )
022EFE34 E2431002 sub     r1,r3,2h          //r1 = 113h - 2
022EFE38 E1500001 cmp     r0,r1
if( levelid == 111h )
    022EFE3C 0A0000B4 beq     22F0114h      ///022F0114 v
022EFE40 EA000158 b       22F03A8h          ///022F03A8 - Return 1 v


//
// 022EFE44 - levelid > 0x113
//
022EFE44 E283101E add     r1,r3,1Eh             //r1 = 0x113 + 0x1E
022EFE48 E1500001 cmp     r0,r1
if( LevelId < 0x131 )
    022EFE4C CA00001B bgt     22EFEC0h          ///022EFEC0
022EFE50 E2831012 add     r1,r3,12h             //r1 = 0x131 + 0x12
022EFE54 E0501001 subs    r1,r0,r1              //r1 = LevelId - 0x143
if( (LevelId - 0x143) > 0 )                     //the pl suffix means negative flag cleared!
    022EFE58 508FF101 addpl   r15,r15,r1,lsl 2h //r15 = 022EFE5C + ((LevelId - 0x143) * 4) + 4
022EFE5C EA00000C b       22EFE94h              ///022EFE94
022EFE60 EA000078 b       22F0048h              ///022F0048 v
022EFE64 EA000073 b       22F0038h              ///022F0038 v
022EFE68 EA00014E b       22F03A8h              ///022F03A8 - Return 1 v
022EFE6C EA00014D b       22F03A8h              ///022F03A8 - Return 1 v
022EFE70 EA000077 b       22F0054h              ///022F0054 v
022EFE74 EA00014B b       22F03A8h              ///022F03A8 - Return 1 v
022EFE78 EA00014A b       22F03A8h              ///022F03A8 - Return 1 v
022EFE7C EA000149 b       22F03A8h              ///022F03A8 - Return 1 v
022EFE80 EA000148 b       22F03A8h              ///022F03A8 - Return 1 v
022EFE84 EA000147 b       22F03A8h              ///022F03A8 - Return 1 v
022EFE88 EA000146 b       22F03A8h              ///022F03A8 - Return 1 v
022EFE8C EA000145 b       22F03A8h              ///022F03A8 - Return 1 v
022EFE90 EA0000B5 b       22F016Ch

///022EFE94
022EFE94 E59F1520 ldr     r1,=123h              //r1 = 0x123
022EFE98 E1500001 cmp     r0,r1
if( LevelId > 0x123 )
    022EFE9C CA000004 bgt     22EFEB4h          ///022EFEB4
if( LevelId >= 0x123 )
    022EFEA0 AA00006B bge     22F0054h
022EFEA4 E2411001 sub     r1,r1,1h              //r1 = 0x123 - 1
022EFEA8 E1500001 cmp     r0,r1
if( LevelId == 0x122 )
    022EFEAC 0A000061 beq     22F0038h
022EFEB0 EA00013C b       22F03A8h              ///022F03A8 Return 1

///022EFEB4 LevelId > 0x123
022EFEB4 E3500F49 cmp     r0,124h
022EFEB8 0A000065 beq     22F0054h
022EFEBC EA000139 b       22F03A8h              ///022F03A8 - Return 1 v

///022EFEC0 LevelId < 0x131
022EFEC0 E2831044 add     r1,r3,44h
022EFEC4 E1500001 cmp     r0,r1
022EFEC8 CA000004 bgt     22EFEE0h
022EFECC AA00004D bge     22F0008h
022EFED0 E2831043 add     r1,r3,43h
022EFED4 E1500001 cmp     r0,r1
022EFED8 0A000061 beq     22F0064h
022EFEDC EA000131 b       22F03A8h              ///022F03A8 - Return 1 v
022EFEE0 E283104C add     r1,r3,4Ch
022EFEE4 E1500001 cmp     r0,r1
022EFEE8 0A000073 beq     22F00BCh
022EFEEC EA00012D b       22F03A8h              ///022F03A8 - Return 1 v

//
// 022EFEF0 - ( levelid > 0x160 )
//
022EFEF0 E59F34C8 ldr     r3,=195h
022EFEF4 E1500003 cmp     r0,r3
022EFEF8 CA000019 bgt     22EFF64h
022EFEFC AA00003C bge     22EFFF4h
022EFF00 E3500F5E cmp     r0,178h
022EFF04 CA00000B bgt     22EFF38h
022EFF08 AA000039 bge     22EFFF4h
022EFF0C E2431026 sub     r1,r3,26h
022EFF10 E1500001 cmp     r0,r1
022EFF14 CA000004 bgt     22EFF2Ch
022EFF18 AA000042 bge     22F0028h
022EFF1C E243102B sub     r1,r3,2Bh
022EFF20 E1500001 cmp     r0,r1
022EFF24 0A00003F beq     22F0028h
022EFF28 EA00011E b       22F03A8h              ///022F03A8 - Return 1 v
022EFF2C E3500E17 cmp     r0,170h
022EFF30 0A00003C beq     22F0028h
022EFF34 EA00011B b       22F03A8h              ///022F03A8 - Return 1 v
022EFF38 E243100E sub     r1,r3,0Eh
022EFF3C E1500001 cmp     r0,r1
022EFF40 CA000004 bgt     22EFF58h
022EFF44 AA00002A bge     22EFFF4h
022EFF48 E2431013 sub     r1,r3,13h
022EFF4C E1500001 cmp     r0,r1
022EFF50 0A000034 beq     22F0028h
022EFF54 EA000113 b       22F03A8h              ///022F03A8 - Return 1 v
022EFF58 E3500E19 cmp     r0,190h
022EFF5C 0A00002D beq     22F0018h
022EFF60 EA000110 b       22F03A8h              ///022F03A8 - Return 1 v
022EFF64 E2831016 add     r1,r3,16h
022EFF68 E1500001 cmp     r0,r1
022EFF6C CA00000B bgt     22EFFA0h
022EFF70 AA000030 bge     22F0038h
022EFF74 E283100C add     r1,r3,0Ch
022EFF78 E1500001 cmp     r0,r1
022EFF7C CA000003 bgt     22EFF90h
022EFF80 AA00002C bge     22F0038h
022EFF84 E3500E1A cmp     r0,1A0h
022EFF88 0A00002A beq     22F0038h
022EFF8C EA000105 b       22F03A8h              ///022F03A8 - Return 1 v
022EFF90 E2831010 add     r1,r3,10h
022EFF94 E1500001 cmp     r0,r1
022EFF98 0A000015 beq     22EFFF4h
022EFF9C EA000101 b       22F03A8h              ///022F03A8 - Return 1 v
022EFFA0 E3500E1B cmp     r0,1B0h
022EFFA4 CA000003 bgt     22EFFB8h
022EFFA8 AA000088 bge     22F01D0h
022EFFAC E3500F6B cmp     r0,1ACh
022EFFB0 0A000020 beq     22F0038h  
022EFFB4 EA0000FB b       22F03A8h              ///022F03A8 - Return 1 v
022EFFB8 E283101C add     r1,r3,1Ch
022EFFBC E0400001 sub     r0,r0,r1
022EFFC0 E3500009 cmp     r0,9h
022EFFC4 908FF100 addls   r15,r15,r0,lsl 2h
022EFFC8 EA0000F6 b       22F03A8h              ///022F03A8 - Return 1 v
022EFFCC EA00007F b       22F01D0h              ///022F01D0 
022EFFD0 EA00009B b       22F0244h
022EFFD4 EA000093 b       22F0228h
022EFFD8 EA000099 b       22F0244h
022EFFDC EA0000F1 b       22F03A8h              ///022F03A8 - Return 1 v
022EFFE0 EA0000AF b       22F02A4h
022EFFE4 EA0000C6 b       22F0304h
022EFFE8 EA0000C9 b       22F0314h
022EFFEC EA0000ED b       22F03A8h              ///022F03A8 - Return 1 v
022EFFF0 EA0000CD b       22F032Ch
///022EFFF4 - ( levelid == 0x112 ) OR ( levelid >= 0xDC ) OR ( levelid >= 0x85 )
022EFFF4 E3A01000 mov     r1,0h
022EFFF8 E1A00005 mov     r0,r5
022EFFFC E1A02001 mov     r2,r1
022F0000 EBFFF979 bl      22EE5ECh
022F0004 EA0000E9 b       22F03B0h              ///022F03B0 - Return 0 v
///022F0008 - (levelid == 8)
022F0008 E1A00005 mov     r0,r5                 //r0 = (PtrLevelStruct + 0x100)
022F000C E3A01002 mov     r1,2h                 //r1 = 2
022F0010 EBFFF982 bl      22EE620h              ///022EE620
{
    ///fun_022EE620
    022EE620 E92D4FF8 push    r3-r11,r14
    022EE624 E24DD018 sub     r13,r13,18h
    022EE628 E1A04000 mov     r4,r0                 //r4 = (PtrLevelStruct + 0x100)
    022EE62C E1A05001 mov     r5,r1                 //r5 = r1
    022EE630 E3550006 cmp     r5,6h
    022EE634 E2849E21 add     r9,r4,210h            //r9 = (PtrLevelStruct + 0x100) + 0x210
    if( r5 <= 6 )
        022EE638 908FF105 addls   r15,r15,r5,lsl 2h
    022EE63C EA000011 b       22EE688h              ///022EE688 r5 > 6
    022EE640 EA000005 b       22EE65Ch              ///022EE65C 0
    022EE644 EA00000F b       22EE688h              ///022EE688 1
    022EE648 EA00000E b       22EE688h              ///022EE688 2
    022EE64C EA00001C b       22EE6C4h              ///022EE6C4 3
    022EE650 EA000027 b       22EE6F4h              ///022EE6F4 4
    022EE654 EA000026 b       22EE6F4h              ///022EE6F4 5
    022EE658 EA000025 b       22EE6F4h              ///022EE6F4 6   
    ///022EE65C 0
    022EE65C E3A02000 mov     r2,0h
    022EE660 E58421FC str     r2,[r4,1FCh]
    022EE664 E58D2000 str     r2,[r13]
    022EE668 E1A00009 mov     r0,r9                 //r0 = (PtrLevelStruct + 0x100) + 0x210
    022EE66C E1A01004 mov     r1,r4                 //r1 = (PtrLevelStruct + 0x100)
    022EE670 E1A03002 mov     r3,r2                 //r3 = 0
    022EE674 E58D2004 str     r2,[r13,4h]
    022EE678 EB000093 bl      22EE8CCh
    {
        022EE8CC E92D4070 push    r4-r6,r14
        022EE8D0 E2812C02 add     r2,r1,200h
        022EE8D4 E1D22BF8 ldrsh   r2,[r2,0B8h]              // [r1 + 2B8h] (Should be 1 after the padding was applied after the image)
        022EE8D8 E5DD4014 ldrb    r4,[r13,14h]
        022EE8DC E59DC010 ldr     r12,[r13,10h]
        022EE8E0 E1C020B0 strh    r2,[r0]
        022EE8E4 E1C030B2 strh    r3,[r0,2h]                //[r0,2h] = r3
        022EE8E8 E1C0C0B4 strh    r12,[r0,4h]
        022EE8EC E5C04006 strb    r4,[r0,6h]
        022EE8F0 E5D121E2 ldrb    r2,[r1,1E2h]              //r2 = BMA_unk1
        022EE8F4 E3540000 cmp     r4,0h
        022EE8F8 E5802008 str     r2,[r0,8h]
        022EE8FC E5D121E3 ldrb    r2,[r1,1E3h]              //r2 = BMA_unk2
        022EE900 E580200C str     r2,[r0,0Ch]
        022EE904 E5D121E0 ldrb    r2,[r1,1E0h]              //r2 = BMA_Width
        if( r4 != 0 )
            022EE908 11A02182 movne   r2,r2,lsl 3h              //r2 = BMA_Width * 8
            022EE90C 15802010 strne   r2,[r0,10h]               
            022EE910 15D121E1 ldrneb  r2,[r1,1E1h]              //r2 = BMA_Height
            022EE914 11A02182 movne   r2,r2,lsl 3h              //r2 = BMA_Height * 8
            022EE918 1A000005 bne     22EE934h              ///022EE934
        022EE91C E1A02182 mov     r2,r2,lsl 3h              //r2 = BMA_Width * 8
        022EE920 E2422C01 sub     r2,r2,100h                //r2 = (BMA_Width * 8) - 256
        022EE924 E5802010 str     r2,[r0,10h]
        022EE928 E5D121E1 ldrb    r2,[r1,1E1h]              //r2 = BMA_Height
        022EE92C E1A02182 mov     r2,r2,lsl 3h              //r2 = BMA_Height * 8
        022EE930 E24220C0 sub     r2,r2,0C0h                //r2 = (BMA_Height * 8) - 192
        ///022EE934
        022EE934 E5802014 str     r2,[r0,14h]
        022EE938 E3A02000 mov     r2,0h                     //r2 = 0
        022EE93C E281EC02 add     r14,r1,200h               //r14 = (PtrLevelStruct + 0x100) + 0x200
        022EE940 EA000009 b       22EE96Ch                  ///022EE96C
        ///022EE944
        022EE944 E0816103 add     r6,r1,r3,lsl 2h           //r6 = (PtrLevelStruct + 0x100) + (r3 * 4)
        022EE948 E59642DC ldr     r4,[r6,2DCh]              //r4 = PtrScreenMappingData
        022EE94C E0805102 add     r5,r0,r2,lsl 2h           //r5 = r0 + (r2 * 4)
        022EE950 E585401C str     r4,[r5,1Ch]               //[r5,1Ch] = r4
        022EE954 E59642E4 ldr     r4,[r6,2E4h]              //
        022EE958 E2822001 add     r2,r2,1h
        022EE95C E5854024 str     r4,[r5,24h]
        022EE960 E59642EC ldr     r4,[r6,2ECh]
        022EE964 E2833001 add     r3,r3,1h
        022EE968 E585402C str     r4,[r5,2Ch]
        ///022EE96C
        022EE96C E152000C cmp     r2,r12
        022EE970 AA000002 bge     22EE980h
        022EE974 E1DE4CF0 ldrsh   r4,[r14,0C0h]             //Probably Nb of screens?(was 2, at 0x210CD04)
        022EE978 E1530004 cmp     r3,r4
        if( r3 < r4 )
            022EE97C BAFFFFF0 blt     22EE944h              ///022EE944
        022EE980 E3A0C000 mov     r12,0h
        022EE984 EA000004 b       22EE99Ch
        022EE988 E0803102 add     r3,r0,r2,lsl 2h
        022EE98C E583C01C str     r12,[r3,1Ch]
        022EE990 E583C024 str     r12,[r3,24h]
        022EE994 E583C02C str     r12,[r3,2Ch]
        022EE998 E2822001 add     r2,r2,1h
        022EE99C E3520002 cmp     r2,2h
        022EE9A0 BAFFFFF8 blt     22EE988h
        022EE9A4 E2811C02 add     r1,r1,200h
        022EE9A8 EB000000 bl      22EE9B0h
        022EE9AC E8BD8070 pop     r4-r6,r15
    }
    022EE67C E59F0228 ldr     r0,=22EEA60h
    022EE680 E5890018 str     r0,[r9,18h]
    022EE684 EA000035 b       22EE760h
    ///022EE688 - if (r5 > 6), 1 or 2
    022EE688 E3A07001 mov     r7,1h
    022EE68C E58471FC str     r7,[r4,1FCh]
    022EE690 E2840C02 add     r0,r4,200h
    022EE694 E1D06CF0 ldrsh   r6,[r0,0C0h]
    022EE698 E3550002 cmp     r5,2h
    022EE69C E3A02000 mov     r2,0h
    022EE6A0 13A07000 movne   r7,0h
    022EE6A4 E58D6000 str     r6,[r13]
    022EE6A8 E20760FF and     r6,r7,0FFh
    022EE6AC E1A00009 mov     r0,r9
    022EE6B0 E1A01004 mov     r1,r4
    022EE6B4 E1A03002 mov     r3,r2
    022EE6B8 E58D6004 str     r6,[r13,4h]
    022EE6BC EB000082 bl      22EE8CCh
    022EE6C0 EA000026 b       22EE760h
    ///022EE6C4 3
    022EE6C4 E3A00001 mov     r0,1h
    022EE6C8 E58401FC str     r0,[r4,1FCh]
    022EE6CC E2840C02 add     r0,r4,200h
    022EE6D0 E1D01CF0 ldrsh   r1,[r0,0C0h]
    022EE6D4 E3A02000 mov     r2,0h
    022EE6D8 E1A00009 mov     r0,r9
    022EE6DC E58D1000 str     r1,[r13]
    022EE6E0 E1A01004 mov     r1,r4
    022EE6E4 E1A03002 mov     r3,r2
    022EE6E8 E58D2004 str     r2,[r13,4h]
    022EE6EC EB000076 bl      22EE8CCh
    022EE6F0 EA00001A b       22EE760h
    ///022EE6F4 4, 5, 6
    022EE6F4 E3A00002 mov     r0,2h
    022EE6F8 E3550006 cmp     r5,6h
    022EE6FC 03A03001 moveq   r3,1h
    022EE700 E58401FC str     r0,[r4,1FCh]
    022EE704 13A03000 movne   r3,0h
    022EE708 E3A02001 mov     r2,1h
    022EE70C E20360FF and     r6,r3,0FFh
    022EE710 E58D2000 str     r2,[r13]
    022EE714 E1A00009 mov     r0,r9
    022EE718 E1A01004 mov     r1,r4
    022EE71C E3A03000 mov     r3,0h
    022EE720 E58D6004 str     r6,[r13,4h]
    022EE724 EB000068 bl      22EE8CCh
    022EE728 E2840C02 add     r0,r4,200h
    022EE72C E1D00CF0 ldrsh   r0,[r0,0C0h]
    022EE730 E3550004 cmp     r5,4h
    022EE734 13A02001 movne   r2,1h
    022EE738 03A02000 moveq   r2,0h
    022EE73C E2400001 sub     r0,r0,1h
    022EE740 E20260FF and     r6,r2,0FFh
    022EE744 E58D0000 str     r0,[r13]
    022EE748 E1A01004 mov     r1,r4
    022EE74C E2890054 add     r0,r9,54h
    022EE750 E3A02000 mov     r2,0h
    022EE754 E3A03001 mov     r3,1h
    022EE758 E58D6004 str     r6,[r13,4h]
    022EE75C EB00005A bl      22EE8CCh
    022EE760 E3550003 cmp     r5,3h
    022EE764 1A000005 bne     22EE780h
    022EE768 E59F2140 ldr     r2,=22EEA64h
    022EE76C E1A00009 mov     r0,r9
    022EE770 E2841C02 add     r1,r4,200h
    022EE774 E5892018 str     r2,[r9,18h]
    022EE778 EB00008C bl      22EE9B0h
    022EE77C EA000032 b       22EE84Ch
    022EE780 E59F012C ldr     r0,=2320BE4h
    022EE784 E3A0A000 mov     r10,0h
    022EE788 E5908024 ldr     r8,[r0,24h]
    022EE78C E5907028 ldr     r7,[r0,28h]
    022EE790 E58D8008 str     r8,[r13,8h]
    022EE794 E58D700C str     r7,[r13,0Ch]
    022EE798 E2845C02 add     r5,r4,200h
    022EE79C E59FB114 ldr     r11,=22EEA84h
    022EE7A0 E59F6108 ldr     r6,=22EEA64h
    022EE7A4 EA000025 b       22EE840h
    022EE7A8 E3A00054 mov     r0,54h
    022EE7AC E020909A mla     r0,r10,r0,r9
    022EE7B0 E084118A add     r1,r4,r10,lsl 3h
    022EE7B4 E5818200 str     r8,[r1,200h]
    022EE7B8 E5817204 str     r7,[r1,204h]
    022EE7BC E1D010F4 ldrsh   r1,[r0,4h]
    022EE7C0 E3510001 cmp     r1,1h
    022EE7C4 0A000001 beq     22EE7D0h
    022EE7C8 E3510002 cmp     r1,2h
    022EE7CC 0A00000C beq     22EE804h
    022EE7D0 E1D010F0 ldrsh   r1,[r0]
    022EE7D4 E3510000 cmp     r1,0h
    022EE7D8 0A000001 beq     22EE7E4h
    022EE7DC E3510001 cmp     r1,1h
    022EE7E0 0A000001 beq     22EE7ECh
    022EE7E4 E5806018 str     r6,[r0,18h]
    022EE7E8 EA000011 b       22EE834h
    022EE7EC E5D01006 ldrb    r1,[r0,6h]
    022EE7F0 E3510000 cmp     r1,0h
    022EE7F4 159F10C0 ldrne   r1,=22EEBF8h
    022EE7F8 059F10C0 ldreq   r1,=22EEAACh
    022EE7FC E5801018 str     r1,[r0,18h]
    022EE800 EA00000B b       22EE834h
    022EE804 E1D010F0 ldrsh   r1,[r0]
    022EE808 E3510000 cmp     r1,0h
    022EE80C 0A000001 beq     22EE818h
    022EE810 E3510001 cmp     r1,1h
    022EE814 0A000001 beq     22EE820h
    022EE818 E580B018 str     r11,[r0,18h]
    022EE81C EA000004 b       22EE834h
    022EE820 E5D01006 ldrb    r1,[r0,6h]
    022EE824 E3510000 cmp     r1,0h
    022EE828 159F1094 ldrne   r1,=22EEFC0h
    022EE82C 059F1094 ldreq   r1,=22EED98h
    022EE830 E5801018 str     r1,[r0,18h]
    022EE834 E085118A add     r1,r5,r10,lsl 3h
    022EE838 EB00005C bl      22EE9B0h
    022EE83C E28AA001 add     r10,r10,1h
    022EE840 E59401FC ldr     r0,[r4,1FCh]
    022EE844 E15A0000 cmp     r10,r0
    022EE848 BAFFFFD6 blt     22EE7A8h
    022EE84C E59F0060 ldr     r0,=2320BE4h
    022EE850 E59471FC ldr     r7,[r4,1FCh]
    022EE854 E5906024 ldr     r6,[r0,24h]
    022EE858 E5905028 ldr     r5,[r0,28h]
    022EE85C E58D6010 str     r6,[r13,10h]
    022EE860 E58D5014 str     r5,[r13,14h]
    022EE864 E3A0A000 mov     r10,0h
    022EE868 E3A08054 mov     r8,54h
    022EE86C EA00000A b       22EE89Ch
    022EE870 E0209897 mla     r0,r7,r8,r9
    022EE874 E0841187 add     r1,r4,r7,lsl 3h
    022EE878 E5816200 str     r6,[r1,200h]
    022EE87C E5815204 str     r5,[r1,204h]
    022EE880 E58DA000 str     r10,[r13]
    022EE884 E1A01004 mov     r1,r4
    022EE888 E1A0200A mov     r2,r10
    022EE88C E1A0300A mov     r3,r10
    022EE890 E58DA004 str     r10,[r13,4h]
    022EE894 EB00000C bl      22EE8CCh
    022EE898 E2877001 add     r7,r7,1h
    022EE89C E3570002 cmp     r7,2h
    022EE8A0 BAFFFFF2 blt     22EE870h
    022EE8A4 E28DD018 add     r13,r13,18h
    022EE8A8 E8BD8FF8 pop     r3-r11,r15
    022EE8AC 022EEA60 eoreq   r14,r14,60000h
    022EE8B0 022EEA64 eoreq   r14,r14,64000h
    022EE8B4 02320BE4 eoreqs  r0,r2,39000h
    022EE8B8 022EEA84 eoreq   r14,r14,84000h
    022EE8BC 022EEBF8 eoreq   r14,r14,3E000h
    022EE8C0 022EEAAC eoreq   r14,r14,0AC000h
    022EE8C4 022EEFC0 eoreq   r14,r14,300h
    022EE8C8 022EED98 eoreq   r14,r14,2600h
}
022F0014 EA0000E5 b       22F03B0h              ///022F03B0 - Return 0 v
022F0018 E1A00005 mov     r0,r5
022F001C E3A01004 mov     r1,4h
022F0020 EBFFF97E bl      22EE620h
022F0024 EA0000E1 b       22F03B0h              ///022F03B0 - Return 0 v
///022F0028
022F0028 E1A00005 mov     r0,r5
022F002C E3A01006 mov     r1,6h
022F0030 EBFFF97A bl      22EE620h
022F0034 EA0000DD b       22F03B0h              ///022F03B0 - Return 0 v
///022F0038
022F0038 E1A00005 mov     r0,r5
022F003C E3A01006 mov     r1,6h
022F0040 EBFFF976 bl      22EE620h
022F0044 EA0000D9 b       22F03B0h              ///022F03B0 - Return 0 v
///022F0048
022F0048 E1A00005 mov     r0,r5
022F004C E1A01002 mov     r1,r2
022F0050 EBFFF965 bl      22EE5ECh
///022F0054
022F0054 E1A00005 mov     r0,r5
022F0058 E3A01004 mov     r1,4h
022F005C EBFFF96F bl      22EE620h
022F0060 EA0000D2 b       22F03B0h              ///022F03B0 - Return 0 v
///022F0064 - 
022F0064 E1A00005 mov     r0,r5
022F0068 E3A01006 mov     r1,6h
022F006C EBFFF96B bl      22EE620h
022F0070 E1A00004 mov     r0,r4                 //r0 = r4 //AKA [r10,2BCh] 
022F0074 EBF46429 bl      2009120h
022F0078 E1A02004 mov     r2,r4
022F007C E3A00002 mov     r0,2h
022F0080 E3A01001 mov     r1,1h
022F0084 EBF464F2 bl      2009454h
022F0088 E1A02004 mov     r2,r4
022F008C E3A00003 mov     r0,3h
022F0090 E3A01002 mov     r1,2h
022F0094 EBF464EE bl      2009454h
022F0098 E1A02004 mov     r2,r4
022F009C E3A00004 mov     r0,4h
022F00A0 E3A01002 mov     r1,2h
022F00A4 EBF464EA bl      2009454h
022F00A8 E1A02004 mov     r2,r4
022F00AC E3A00080 mov     r0,80h
022F00B0 E1A01000 mov     r1,r0
022F00B4 EBF46502 bl      20094C4h
022F00B8 EA0000BC b       22F03B0h              ///022F03B0 - Return 0 v
022F00BC E1A00005 mov     r0,r5
022F00C0 E3A01006 mov     r1,6h
022F00C4 EBFFF955 bl      22EE620h
022F00C8 E1A00004 mov     r0,r4
022F00CC EBF46413 bl      2009120h
022F00D0 E1A02004 mov     r2,r4
022F00D4 E3A00002 mov     r0,2h
022F00D8 E3A01001 mov     r1,1h
022F00DC EBF464DC bl      2009454h
022F00E0 E1A02004 mov     r2,r4
022F00E4 E3A00003 mov     r0,3h
022F00E8 E3A01002 mov     r1,2h
022F00EC EBF464D8 bl      2009454h
022F00F0 E1A02004 mov     r2,r4
022F00F4 E3A00004 mov     r0,4h
022F00F8 E3A01002 mov     r1,2h
022F00FC EBF464D4 bl      2009454h
022F0100 E1A02004 mov     r2,r4
022F0104 E3A00040 mov     r0,40h
022F0108 E1A01000 mov     r1,r0
022F010C EBF464EC bl      20094C4h
022F0110 EA0000A6 b       22F03B0h              ///022F03B0 - Return 0 v

//
// 022F0114 - ( levelid >= 0x160 )
//
022F0114 E1A00005 mov     r0,r5
022F0118 E3A01006 mov     r1,6h
022F011C EBFFF93F bl      22EE620h
022F0120 E1A00004 mov     r0,r4
022F0124 EBF463FD bl      2009120h
022F0128 E1A02004 mov     r2,r4
022F012C E3A00002 mov     r0,2h
022F0130 E3A01001 mov     r1,1h
022F0134 EBF464C6 bl      2009454h
022F0138 E1A02004 mov     r2,r4
022F013C E3A00003 mov     r0,3h
022F0140 E3A01002 mov     r1,2h
022F0144 EBF464C2 bl      2009454h
022F0148 E1A02004 mov     r2,r4
022F014C E3A00004 mov     r0,4h
022F0150 E3A01002 mov     r1,2h
022F0154 EBF464BE bl      2009454h
022F0158 E1A02004 mov     r2,r4
022F015C E3A00040 mov     r0,40h
022F0160 E1A01000 mov     r1,r0
022F0164 EBF464D6 bl      20094C4h
022F0168 EA000090 b       22F03B0h              ///022F03B0 - Return 0 v
022F016C E1A00005 mov     r0,r5
022F0170 E3A01001 mov     r1,1h
022F0174 EBFFF91C bl      22EE5ECh
022F0178 E1A00005 mov     r0,r5
022F017C E3A01001 mov     r1,1h
022F0180 EBFFF926 bl      22EE620h
022F0184 E1A00004 mov     r0,r4
022F0188 EBF463E4 bl      2009120h
022F018C E1A02004 mov     r2,r4
022F0190 E3A00002 mov     r0,2h
022F0194 E3A01001 mov     r1,1h
022F0198 EBF464AD bl      2009454h
022F019C E1A02004 mov     r2,r4
022F01A0 E3A00003 mov     r0,3h
022F01A4 E3A01002 mov     r1,2h
022F01A8 EBF464A9 bl      2009454h
022F01AC E3A00004 mov     r0,4h
022F01B0 E3A01002 mov     r1,2h
022F01B4 E1A02004 mov     r2,r4
022F01B8 EBF464A5 bl      2009454h
022F01BC E1A02004 mov     r2,r4
022F01C0 E3A00000 mov     r0,0h
022F01C4 E3A01080 mov     r1,80h
022F01C8 EBF464BD bl      20094C4h
022F01CC EA000077 b       22F03B0h              ///022F03B0 - Return 0 v
022F01D0 E1A00005 mov     r0,r5
022F01D4 E3A01002 mov     r1,2h
022F01D8 EBFFF910 bl      22EE620h
022F01DC E1A00004 mov     r0,r4
022F01E0 EBF463CE bl      2009120h
022F01E4 E1A02004 mov     r2,r4
022F01E8 E3A00002 mov     r0,2h
022F01EC E3A01001 mov     r1,1h
022F01F0 EBF46497 bl      2009454h
022F01F4 E1A02004 mov     r2,r4
022F01F8 E3A00003 mov     r0,3h
022F01FC E3A01002 mov     r1,2h
022F0200 EBF46493 bl      2009454h
022F0204 E1A02004 mov     r2,r4
022F0208 E3A00004 mov     r0,4h
022F020C E3A01002 mov     r1,2h
022F0210 EBF4648F bl      2009454h
022F0214 E1A02004 mov     r2,r4
022F0218 E3A00060 mov     r0,60h
022F021C E3A01018 mov     r1,18h
022F0220 EBF464A7 bl      20094C4h
022F0224 EA000061 b       22F03B0h              ///022F03B0 - Return 0 v
022F0228 E1A00005 mov     r0,r5
022F022C E1A01002 mov     r1,r2
022F0230 EBFFF8ED bl      22EE5ECh
022F0234 E1A01004 mov     r1,r4
022F0238 E3A00002 mov     r0,2h
022F023C EBF463D4 bl      2009194h
022F0240 EA00005A b       22F03B0h              ///022F03B0 - Return 0 v
022F0244 E1A00005 mov     r0,r5
022F0248 E1A01002 mov     r1,r2
022F024C EBFFF8F3 bl      22EE620h
022F0250 E1A00004 mov     r0,r4
022F0254 EBF463B1 bl      2009120h
022F0258 E3A00002 mov     r0,2h
022F025C E1A01000 mov     r1,r0
022F0260 E1A02004 mov     r2,r4
022F0264 EBF4647A bl      2009454h
022F0268 E1A02004 mov     r2,r4
022F026C E3A00003 mov     r0,3h
022F0270 E3A01002 mov     r1,2h
022F0274 EBF46476 bl      2009454h
022F0278 E3A00004 mov     r0,4h
022F027C E3A01002 mov     r1,2h
022F0280 E1A02004 mov     r2,r4
022F0284 EBF46472 bl      2009454h
022F0288 E1A02004 mov     r2,r4
022F028C E3A00060 mov     r0,60h
022F0290 E3A01018 mov     r1,18h
022F0294 EBF4648A bl      20094C4h
022F0298 E3A00001 mov     r0,1h
022F029C EB0009AA bl      22F294Ch
022F02A0 EA000042 b       22F03B0h              ///022F03B0 - Return 0 v
022F02A4 E1A00005 mov     r0,r5
022F02A8 E1A01002 mov     r1,r2
022F02AC EBFFF8DB bl      22EE620h
022F02B0 E1A00004 mov     r0,r4
022F02B4 EBF46399 bl      2009120h
022F02B8 E3A00002 mov     r0,2h
022F02BC E1A01000 mov     r1,r0
022F02C0 E1A02004 mov     r2,r4
022F02C4 EBF46462 bl      2009454h
022F02C8 E1A02004 mov     r2,r4
022F02CC E3A00003 mov     r0,3h
022F02D0 E3A01002 mov     r1,2h
022F02D4 EBF4645E bl      2009454h
022F02D8 E3A00004 mov     r0,4h
022F02DC E3A01002 mov     r1,2h
022F02E0 E1A02004 mov     r2,r4
022F02E4 EBF4645A bl      2009454h
022F02E8 E1A02004 mov     r2,r4
022F02EC E3A00060 mov     r0,60h
022F02F0 E3A01018 mov     r1,18h
022F02F4 EBF46472 bl      20094C4h
022F02F8 E3A00002 mov     r0,2h
022F02FC EB000992 bl      22F294Ch
022F0300 EA00002A b       22F03B0h              ///022F03B0 - Return 0 v
022F0304 E1A00005 mov     r0,r5
022F0308 E3A01003 mov     r1,3h
022F030C EBFFF8C3 bl      22EE620h
022F0310 EA000026 b       22F03B0h              ///022F03B0 - Return 0 v
022F0314 E1A00005 mov     r0,r5
022F0318 E1A01002 mov     r1,r2
022F031C EBFFF8BF bl      22EE620h
022F0320 E3A00000 mov     r0,0h
022F0324 EB000988 bl      22F294Ch
022F0328 EA000020 b       22F03B0h              ///022F03B0 - Return 0 v
022F032C E1A00005 mov     r0,r5
022F0330 E1A01002 mov     r1,r2
022F0334 EBFFF8B9 bl      22EE620h
022F0338 E1A00004 mov     r0,r4
022F033C EBF46377 bl      2009120h
022F0340 E3A00002 mov     r0,2h
022F0344 E1A01000 mov     r1,r0
022F0348 E1A02004 mov     r2,r4
022F034C EBF46440 bl      2009454h
022F0350 E1A02004 mov     r2,r4
022F0354 E3A00003 mov     r0,3h
022F0358 E3A01002 mov     r1,2h
022F035C EBF4643C bl      2009454h
022F0360 E3A00004 mov     r0,4h
022F0364 E3A01002 mov     r1,2h
022F0368 E1A02004 mov     r2,r4
022F036C EBF46438 bl      2009454h
022F0370 E1A02004 mov     r2,r4
022F0374 E3A0005A mov     r0,5Ah
022F0378 E3A0101E mov     r1,1Eh
022F037C EBF46450 bl      20094C4h
022F0380 E3A00003 mov     r0,3h
022F0384 EB000970 bl      22F294Ch
022F0388 EA000008 b       22F03B0h              ///022F03B0 - Return 0 v
///022F038C - ( levelid == 0x113 )
022F038C E1A00005 mov     r0,r5
022F0390 E1A01002 mov     r1,r2
022F0394 EBFFF894 bl      22EE5ECh
022F0398 E1A01004 mov     r1,r4
022F039C E3A00002 mov     r0,2h
022F03A0 EBF4637B bl      2009194h
{
    02009194 E59F2010 ldr     r2,=22A37ACh
    02009198 E0811101 add     r1,r1,r1,lsl 2h
    0200919C E0821001 add     r1,r2,r1
    020091A0 E3A02001 mov     r2,1h
    020091A4 E7C02001 strb    r2,[r0,r1]
    020091A8 E12FFF1E bx      r14
    020091AC 022A37AC 
}
022F03A4 EA000001 b       22F03B0h              ///022F03B0 - Return 0 v
///022F03A8 - Return 1
022F03A8 E3A00001 mov     r0,1h
022F03AC E8BD8038 pop     r3-r5,r15
///022F03B0 - Return 0
022F03B0 E3A00000 mov     r0,0h
022F03B4 E8BD8038 pop     r3-r5,r15
022F03B8 00000113 
022F03BC 00000123 
022F03C0 00000195 



//
//  022F1800 22F1800h
//      r0 = Ptr?
//      r1 = Ptr?
//      r2 = PtrBMAHeaderBeg
//      r3 = value?
//
022F1800 E92D4FF8 push    r3-r11,r14
022F1804 E1D240F6 ldrsh   r4,[r2,6h]        //r4 = BMA_unk6
022F1808 E3A05000 mov     r5,0h             //r5 = 0 //Cnt1
022F180C E3540000 cmp     r4,0h
022F1810 E3A040FF mov     r4,0FFh           //r4 = 0xFF
if( BMA_unk6 <= 0 )
    022F1814 DA000088 ble     22F1A3Ch      ///022F1A3C BMA_unk6 <= 0 v
022F1818 E1A07000 mov     r7,r0             //r7 = r0 
022F181C E1A06005 mov     r6,r5             //r6 = 0
///022F1820
022F1820 E1A08006 mov     r8,r6             //r8 = 0 //Cnt2
///022F1824
022F1824 E2888001 add     r8,r8,1h          //r8 = Cnt2 + 1
022F1828 E3580C01 cmp     r8,100h
022F182C E4C74001 strb    r4,[r7],1h        //[r7] = BMA_unk6, r7 += 1
if( Cnt2 < 256 )
    022F1830 BAFFFFFB blt     22F1824h          ///022F1824 ^
022F1834 E2855001 add     r5,r5,1h          //r5 = Cnt1 + 1
022F1838 E3550004 cmp     r5,4h
if( Cnt1 < 4 )
    022F183C BAFFFFF7 blt     22F1820h      ///022F1820
022F1840 E5D26001 ldrb    r6,[r2,1h]        //r6 = BMA_MapHeight
022F1844 E3A04000 mov     r4,0h             //r4 = 0 //Cnt3
022F1848 E3560000 cmp     r6,0h
if( BMA_MapHeight <= 0 )
    022F184C DA00006E ble     22F1A0Ch      ///022F1A0C v
022F1850 EA00006A b       22F1A00h          ///022F1A00 v
///022F1854
022F1854 E3A09000 mov     r9,0h             //r9 = 0 //Cnt5
022F1858 E3A060FF mov     r6,0FFh           //r6 = 0xFF
///022F185C
022F185C E2899001 add     r9,r9,1h          //r9 = Cnt5 + 1
022F1860 E3590005 cmp     r9,5h
022F1864 E4C76001 strb    r6,[r7],1h        //[r7] = 0xFF, r7 += 1
if( Cnt5 < 5 )
    022F1868 BAFFFFFB blt     22F185Ch      ///022F185C ^
022F186C E3540000 cmp     r4,0h
022F1870 E3A0E000 mov     r14,0h            //r14 = 0
if( Cnt3 !=  0 )
    022F1874 1A00002A bne     22F1924h      ///022F1924 v
022F1878 E1A0800E mov     r8,r14            //r8 = 0
022F187C E3A0C0C0 mov     r12,0C0h          //r12 = 0xC0
022F1880 EA000023 b       22F1914h          ///022F1914 v
///022F1884
022F1884 E5D16000 ldrb    r6,[r1]           //r6 = 
022F1888 E281A001 add     r10,r1,1h         //r10 = r1 + 1
022F188C E1A0100A mov     r1,r10            //r1  = r1 + 1 
022F1890 E35600C0 cmp     r6,0C0h
if( r6 < 0xC0 )
    022F1894 BA000009 blt     22F18C0h      ///022F18C0 v
022F1898 E1A0B00C mov     r11,r12           //r11 = r12
022F189C EA000002 b       22F18ACh          ///022F18AC v
///022F18A0
022F18A0 E4D1A001 ldrb    r10,[r1],1h       //r10 = [r1], r1 += 1
022F18A4 E28BB001 add     r11,r11,1h        //r11 = r11 + 1
022F18A8 E4C7A001 strb    r10,[r7],1h       //[r7] = r10, r7 += 1
///022F18AC
022F18AC E15B0006 cmp     r11,r6
if( r11 <= r6 )
    022F18B0 DAFFFFFA ble     22F18A0h      ///022F18A0 ^
022F18B4 E24660BF sub     r6,r6,0BFh        //r6 = r6 - 0xBF
022F18B8 E08EE006 add     r14,r14,r6        //r14 = r14 + r6
022F18BC EA000014 b       22F1914h          ///022F1914 v
///022F18C0
022F18C0 E3560080 cmp     r6,80h
if( r6 < 0x80 )
    022F18C4 BA00000A blt     22F18F4h      ///022F18F4 v
022F18C8 E28A1001 add     r1,r10,1h         //r1  = r10 + 1
022F18CC E5DAA000 ldrb    r10,[r10]         //r10 = [r10]
022F18D0 E3A0B080 mov     r11,80h           //r11 = 0x80
022F18D4 EA000001 b       22F18E0h          ///022F18E0 v
///022F18D8
022F18D8 E4C7A001 strb    r10,[r7],1h       //[r7] = r10, r7 += 1
022F18DC E28BB001 add     r11,r11,1h        //r11  = r11 + 1
///022F18E0
022F18E0 E15B0006 cmp     r11,r6
if( r11 <= r6 )
    022F18E4 DAFFFFFB ble     22F18D8h      ///022F18D8 ^
022F18E8 E246607F sub     r6,r6,7Fh         //r6 = r6 - 0x7F
022F18EC E08EE006 add     r14,r14,r6        //r14 = r14 + r6
022F18F0 EA000007 b       22F1914h          ///022F1914 v
///022F18F4
022F18F4 E3A0A000 mov     r10,0h            //r10 = 0
022F18F8 EA000001 b       22F1904h          ///022F1904 v
///022F18FC
022F18FC E4C78001 strb    r8,[r7],1h        //[r7] = r8, r7 += 1
022F1900 E28AA001 add     r10,r10,1h        //r10  = r10 + 1 
///022F1904
022F1904 E15A0006 cmp     r10,r6
if( r10 <= r6 )
    022F1908 DAFFFFFB ble     22F18FCh      ///022F18FC
022F190C E2866001 add     r6,r6,1h          //r6 = r6 + 1
022F1910 E08EE006 add     r14,r14,r6        //r14 = r14 + r6
///022F1914
022F1914 E5D26000 ldrb    r6,[r2]               //r6 = BMA_MapWidth
022F1918 E15E0006 cmp     r14,r6
if( r14 < BMA_MapWidth )
    022F191C BAFFFFD8 blt     22F1884h          ///022F1884 ^
022F1920 EA00002D b       22F19DCh              ///022F19DC v
///022F1924
022F1924 E247CC01 sub     r12,r7,100h           //r12 = r7 - 256
022F1928 EA000028 b       22F19D0h              ///022F19D0 v
///022F192C
022F192C E5D16000 ldrb    r6,[r1]               //r6 = 
022F1930 E2818001 add     r8,r1,1h              //r8 = r1 + 1
022F1934 E1A01008 mov     r1,r8                 //r1 = r8
022F1938 E35600C0 cmp     r6,0C0h   
if( r6 < 0xC0 )
    022F193C BA00000B blt     22F1970h          ///022F1970 v
022F1940 E3A080C0 mov     r8,0C0h               //r8 = 0xC0
022F1944 EA000004 b       22F195Ch              ///022F195C v
///022F1948
022F1948 E4D1B001 ldrb    r11,[r1],1h           //r11 = [r1], r1 += 1
022F194C E4DCA001 ldrb    r10,[r12],1h          //r10 = [r12], r12 += 1
022F1950 E2888001 add     r8,r8,1h              //r8  = r8 + 1
022F1954 E02BA00A eor     r10,r11,r10           //r10 = r11 ^ r10
022F1958 E4C7A001 strb    r10,[r7],1h           //[r7] = r10, r7 += 1
///022F195C
022F195C E1580006 cmp     r8,r6
if( r8 <= r6 )
    022F1960 DAFFFFF8 ble     22F1948h          ///022F1948 ^
022F1964 E24660BF sub     r6,r6,0BFh            //r6 = r6 - 0xBF
022F1968 E08EE006 add     r14,r14,r6            //r14 = r14 + r6
022F196C EA000017 b       22F19D0h              ///022F19D0 v
///022F1970
022F1970 E3560080 cmp     r6,80h
if( r6 < 0x80 )
    022F1974 BA00000C blt     22F19ACh          ///022F19AC v
022F1978 E2881001 add     r1,r8,1h              //r1 = r8 + 1
022F197C E5D88000 ldrb    r8,[r8]               //r8 = [r8]
022F1980 E3A0A080 mov     r10,80h               //r10 = 0x80
022F1984 EA000003 b       22F1998h              ///022F1998 v
///022F1988
022F1988 E4DCB001 ldrb    r11,[r12],1h          //r11 = [r12], r12 += 1
022F198C E28AA001 add     r10,r10,1h            //r10 = r10 + 1
022F1990 E028B00B eor     r11,r8,r11            //r11 = r8 | r11
022F1994 E4C7B001 strb    r11,[r7],1h           //[r7] = r11, r7 += 1
///022F1998
022F1998 E15A0006 cmp     r10,r6
if( r10 <= r6 )
    022F199C DAFFFFF9 ble     22F1988h          ///022F1988
022F19A0 E246607F sub     r6,r6,7Fh             //r6 = r6 - 0x7F
022F19A4 E08EE006 add     r14,r14,r6            //r14 = r14 + r6
022F19A8 EA000008 b       22F19D0h              ///022F19D0 v
///022F19AC
022F19AC E3A0A000 mov     r10,0h                //r10 = 0
022F19B0 EA000002 b       22F19C0h              ///022F19C0 v
///022F19B4
022F19B4 E4DC8001 ldrb    r8,[r12],1h           //r8 = [r12], r12 += 1
022F19B8 E28AA001 add     r10,r10,1h            //r10 = r10 + 1
022F19BC E4C78001 strb    r8,[r7],1h            //[r7] = r8, r7 += 1
///022F19C0
022F19C0 E15A0006 cmp     r10,r6
if( r10 <= r6 )
    022F19C4 DAFFFFFA ble     22F19B4h          ///022F19B4 ^
022F19C8 E2866001 add     r6,r6,1h              //r6 = r6 + 1
022F19CC E08EE006 add     r14,r14,r6            //r14 = r14 + r6
///022F19D0
022F19D0 E5D26000 ldrb    r6,[r2]               //r6 = BMA_MapWidth
022F19D4 E15E0006 cmp     r14,r6
if( r14 < r6 )
    022F19D8 BAFFFFD3 blt     22F192Ch          ///022F192C ^
///022F19DC
022F19DC E0899006 add     r9,r9,r6              //r9 = r9 + BMA_MapWidth
022F19E0 E3A060FF mov     r6,0FFh               //r6 = 0xFF
022F19E4 EA000001 b       22F19F0h              ///022F19F0 v
///022F19E8
022F19E8 E4C76001 strb    r6,[r7],1h            //[r7] = 0xFF
022F19EC E2899001 add     r9,r9,1h              //r9 = r9 + 1
///022F19F0
022F19F0 E3590C01 cmp     r9,100h
if( r9 < 256 )
    022F19F4 BAFFFFFB blt     22F19E8h          ///022F19E8 ^
022F19F8 E2844001 add     r4,r4,1h              //r4 = Cnt3 + 1
022F19FC E2855001 add     r5,r5,1h              //r5 = r5 + 1
///022F1A00
022F1A00 E5D26001 ldrb    r6,[r2,1h]            //r6 = BMA_MapHeight
022F1A04 E1540006 cmp     r4,r6
if( Cnt3 < BMA_MapHeight )
    022F1A08 BAFFFF91 blt     22F1854h          ///022F1854 ^
///022F1A0C 
022F1A0C E3A040FF mov     r4,0FFh               //r4 = 0xFF
022F1A10 E3A06000 mov     r6,0h                 //r6 = 0
022F1A14 EA000005 b       22F1A30h              ///022F1A30 v
///022F1A18
022F1A18 E1A08006 mov     r8,r6                 //r8 = 0
022F1A1C E2888001 add     r8,r8,1h              //r8 = r8 + 1
022F1A20 E3580C01 cmp     r8,100h               
022F1A24 E4C74001 strb    r4,[r7],1h            //[r7] = 0xFF, r7 += 1
if( r8 < 256 )
    022F1A28 BAFFFFFB blt     22F1A1Ch
022F1A2C E2855001 add     r5,r5,1h              //r5 = r5 + 1
///022F1A30
022F1A30 E1550003 cmp     r5,r3
if( r5 < r3 )
    022F1A34 BAFFFFF7 blt     22F1A18h          ///022F1A18 ^
022F1A38 EA000034 b       22F1B10h              ///022F1B10 v
///022F1A3C (BMA_unk6 <= 0)
022F1A3C E1A06000 mov     r6,r0             //r6 = 0
022F1A40 E1A07005 mov     r7,r5             //r7 = r5
///022F1A44
022F1A44 E1A08007 mov     r8,r7             //r8 = r7
///022F1A48
022F1A48 E2888001 add     r8,r8,1h          //r8 = r8 + 1
022F1A4C E3580C01 cmp     r8,100h
022F1A50 E4C64001 strb    r4,[r6],1h        //[r6] = r4, r6 += 1
if( r8 < 256 )
    022F1A54 BAFFFFFB blt     22F1A48h      ///022F1A48 ^
022F1A58 E2855001 add     r5,r5,1h          //r5 = r5 + 1
022F1A5C E3550004 cmp     r5,4h
if(r5 < 4)
    022F1A60 BAFFFFF7 blt     22F1A44h      ///022F1A44 ^
022F1A64 E5D27001 ldrb    r7,[r2,1h]        //r7 = BMA_MapHeight
022F1A68 E3A04000 mov     r4,0h             //r4 = 0
022F1A6C E3570000 cmp     r7,0h
if( r7 <= 0 )
    022F1A70 DA00001B ble     22F1AE4h      ///022F1AE4 v
022F1A74 E3A080FF mov     r8,0FFh           //r8  = 0xFF
022F1A78 E1A09004 mov     r9,r4             //r9  = r4
022F1A7C E1A0A008 mov     r10,r8            //r10 = r8
022F1A80 E1A0C004 mov     r12,r4            //r12 = r4
022F1A84 EA000013 b       22F1AD8h          ///022F1AD8 v
///022F1A88
022F1A88 E1A0E00C mov     r14,r12           //r14 = r12
///022F1A8C
022F1A8C E28EE001 add     r14,r14,1h        //r14 = r14 + 1
022F1A90 E4C6A001 strb    r10,[r6],1h       //[r6] = r10, r6 += 1
022F1A94 E35E0005 cmp     r14,5h
if( r14 < 5 )
    022F1A98 BAFFFFFB blt     22F1A8Ch      ///022F1A8C ^
022F1A9C E3A0B000 mov     r11,0h            //r11 = 0
022F1AA0 EA000001 b       22F1AACh          ///022F1AAC v
///022F1AA4
022F1AA4 E4C69001 strb    r9,[r6],1h        //[r6] = r9, r6 += 1
022F1AA8 E28BB001 add     r11,r11,1h        //r11 = r11 + 1
///022F1AAC
022F1AAC E5D27000 ldrb    r7,[r2]           //r7 = BMA_MapWidth
022F1AB0 E15B0007 cmp     r11,r7
if( r11 < BMA_MapWidth )
    022F1AB4 BAFFFFFA blt     22F1AA4h      ///022F1AA4 ^
022F1AB8 E08EE007 add     r14,r14,r7        //r14 = r14 + r7    
022F1ABC EA000001 b       22F1AC8h          ///022F1AC8 v
///022F1AC0
022F1AC0 E4C68001 strb    r8,[r6],1h        //[r6] = r8, r6 += 1
022F1AC4 E28EE001 add     r14,r14,1h        //r14 = r14 + 1
///022F1AC8
022F1AC8 E35E0C01 cmp     r14,100h
if( r14 < 256 )
    022F1ACC BAFFFFFB blt     22F1AC0h      ///022F1AC0 ^
022F1AD0 E2844001 add     r4,r4,1h          //r4 = r4 + 1
022F1AD4 E2855001 add     r5,r5,1h          //r5 = r5 + 1
///022F1AD8
022F1AD8 E5D27001 ldrb    r7,[r2,1h]            ///r7 = BMA_MapHeight
022F1ADC E1540007 cmp     r4,r7
if( r4 < BMA_MapHeight )
    022F1AE0 BAFFFFE8 blt     22F1A88h          ///022F1A88
///022F1AE4
022F1AE4 E3A040FF mov     r4,0FFh           //r4 = 0xFF
022F1AE8 E3A07000 mov     r7,0h             //r7 = 0
022F1AEC EA000005 b       22F1B08h          ///022F1B08
022F1AF0 E1A08007 mov     r8,r7             //r8 = r7
///022F1AF4
022F1AF4 E2888001 add     r8,r8,1h          //r8 = r8 + 1
022F1AF8 E3580C01 cmp     r8,100h           
022F1AFC E4C64001 strb    r4,[r6],1h        //[r6] = r4, r6 += 1
if( r8 < 256 )
    022F1B00 BAFFFFFB blt     22F1AF4h      ///022F1AF4 ^ 
022F1B04 E2855001 add     r5,r5,1h          //r5 = r5 + 1
///022F1B08
022F1B08 E1550003 cmp     r5,r3
if(r5 < r3)
    022F1B0C BAFFFFF7 blt     22F1AF0h
///022F1B10
022F1B10 E1D230F8 ldrsh   r3,[r2,8h]        //r3 = BMA_unk7
022F1B14 E3530000 cmp     r3,0h
if( BMA_unk7 > 0 )
    022F1B18 C5D23001 ldrgtb  r3,[r2,1h]    //r3 = BMA_MapHeight
    022F1B1C C2804B01 addgt   r4,r0,400h    //r4 = r0 + 1024
    022F1B20 C3A06000 movgt   r6,0h         //r6 = 0
    022F1B24 C3530000 cmpgt   r3,0h
if(r3 <= 0 )                                //This condition can be triggered either by (BMA_unk7 <= 0) or (BMA_MapHeight <= 0)
    022F1B28 DA00004A ble     22F1C58h      ///022F1C58 v
022F1B2C EA000046 b       22F1C4Ch          ///022F1C4C v
///022F1B30
022F1B30 E3560000 cmp     r6,0h
022F1B34 E2844005 add     r4,r4,5h          //r4 = r4 + 5
022F1B38 E3A05000 mov     r5,0h             //r5 = 0
if( r6 != 0 )
    022F1B3C 1A000017 bne     22F1BA0h      ///022F1BA0 v
022F1B40 E3A07080 mov     r7,80h            //r7 = 0x80
022F1B44 EA000011 b       22F1B90h          ///022F1B90 v
///022F1B48
022F1B48 E4D19001 ldrb    r9,[r1],1h        //r9 = [r1], r1 += 1
022F1B4C E3590080 cmp     r9,80h
if(r9 < 0x80)
    022F1B50 BA00000B blt     22F1B84h      ///022F1B84 v
022F1B54 E1A08007 mov     r8,r7             //r8 = r7
022F1B58 EA000004 b       22F1B70h          ///022F1B70 v
///022F1B5C
022F1B5C E1A0A004 mov     r10,r4            //r10 = r4
022F1B60 E4D43001 ldrb    r3,[r4],1h        //r3 = [r4], r4 += 1
022F1B64 E2888001 add     r8,r8,1h          //r8 = r8 + 1
022F1B68 E3833080 orr     r3,r3,80h         //r3 = r3 | 0x80
022F1B6C E5CA3000 strb    r3,[r10]          //[r10] = r3
///022F1B70
022F1B70 E1580009 cmp     r8,r9
if( r8 <= r9 )
    022F1B74 DAFFFFF8 ble     22F1B5Ch      ///022F1B5C ^
022F1B78 E249307F sub     r3,r9,7Fh         //r3 = r9 - 0x7F
022F1B7C E0855003 add     r5,r5,r3          //r5 = r5 + r3
022F1B80 EA000002 b       22F1B90h          ///022F1B90 v
///022F1B84
022F1B84 E2893001 add     r3,r9,1h          //r3 = r9 + 1
022F1B88 E0844003 add     r4,r4,r3          //r4 = r4 + r3
022F1B8C E0855003 add     r5,r5,r3          //r5 = r5 + r3
///022F1B90
022F1B90 E5D28000 ldrb    r8,[r2]           //r8 = BMA_MapWidth
022F1B94 E1550008 cmp     r5,r8
if( r5 < BMA_MapWidth )
    022F1B98 BAFFFFEA blt     22F1B48h      ///022F1B48 ^
022F1B9C EA000026 b       22F1C3Ch          ///022F1C3C v
///022F1BA0
022F1BA0 E2447C01 sub     r7,r4,100h        //r7 = r4 - 256    
022F1BA4 E3A0C080 mov     r12,80h           //r12 = 0x80
022F1BA8 E1A03005 mov     r3,r5             //r3 = r5
022F1BAC EA00001F b       22F1C30h          ///022F1C30 v
///022F1BB0
022F1BB0 E4D1E001 ldrb    r14,[r1],1h       //r14 = [r1], r1 += 1
022F1BB4 E35E0080 cmp     r14,80h   
if( r14 < 0x80 )
    022F1BB8 BA00000E blt     22F1BF8h      ///022F1BF8 v
022F1BBC E1A0800C mov     r8,r12            //r8 = r12
022F1BC0 EA000007 b       22F1BE4h          ///022F1BE4 v
///022F1BC4
022F1BC4 E4D79001 ldrb    r9,[r7],1h        //r9 = [r7], r7+= 1
022F1BC8 E2888001 add     r8,r8,1h          //r8 = r8 + 1
022F1BCC E3190080 tst     r9,80h
if( r9 & 0x80 == 0 )
    022F1BD0 01A0A004 moveq   r10,r4        //r10 = r4
    022F1BD4 04D49001 ldreqb  r9,[r4],1h    //r9 = [r4], r4 += 1    
if(r9 & 0x80 != 0)
    022F1BD8 12844001 addne   r4,r4,1h      //r4 = r4 + 1
if( r9 & 0x80 == 0 )
    022F1BDC 03899080 orreq   r9,r9,80h     //r9 = r9 | 0x80
    022F1BE0 05CA9000 streqb  r9,[r10]      //[r10] = r9
///022F1BE4
022F1BE4 E158000E cmp     r8,r14
if( r8 <= r14 )
    022F1BE8 DAFFFFF5 ble     22F1BC4h      ///022F1BC4 ^
022F1BEC E24E807F sub     r8,r14,7Fh        //r8 = r14 + 0x7F    
022F1BF0 E0855008 add     r5,r5,r8          //r5 = r5 + r8
022F1BF4 EA00000D b       22F1C30h          ///022F1C30 v
///022F1BF8
022F1BF8 E1A08003 mov     r8,r3             //r8 = r3
022F1BFC EA000007 b       22F1C20h          ///022F1C20
///022F1C00
022F1C00 E4D79001 ldrb    r9,[r7],1h        //r9 = [r7], r7 += 1
022F1C04 E2888001 add     r8,r8,1h          //r8 = r8 + 1
022F1C08 E3190080 tst     r9,80h    
if( r9 & 0x80 != 0 )
    022F1C0C 11A0A004 movne   r10,r4        //r10 = r4
    022F1C10 14D49001 ldrneb  r9,[r4],1h    //r9 = [r4], r4 += 1
if( r9 & 0x80 == 0 )
    022F1C14 02844001 addeq   r4,r4,1h      //r4 = r4 + 1
if( r9 & 0x80 != 0 )
    022F1C18 13899080 orrne   r9,r9,80h     //r9 = r9 | 0x80
    022F1C1C 15CA9000 strneb  r9,[r10]      //[r10] = r9
///022F1C20
022F1C20 E158000E cmp     r8,r14
if( r8 <= r14 )
    022F1C24 DAFFFFF5 ble     22F1C00h      ///022F1C00 ^
022F1C28 E28E8001 add     r8,r14,1h         //r8 = r14 + 1
022F1C2C E0855008 add     r5,r5,r8          //r5 = r5 + r8
///022F1C30
022F1C30 E5D28000 ldrb    r8,[r2]           //r8 = BMA_MapWidth
022F1C34 E1550008 cmp     r5,r8             
if( r5 < BMA_MapWidth )
    022F1C38 BAFFFFDC blt     22F1BB0h      ///022F1BB0 ^
///022F1C3C
022F1C3C E2683C01 rsb     r3,r8,100h        //r3 = 256 - r8
022F1C40 E2433005 sub     r3,r3,5h          //r3 = r3 - 5
022F1C44 E0844003 add     r4,r4,r3          //r4 = r4 + r3
022F1C48 E2866001 add     r6,r6,1h          //r6 = r6 + 1    
///022F1C4C
022F1C4C E5D23001 ldrb    r3,[r2,1h]        //r3 = BMA_MapHeight
022F1C50 E1560003 cmp     r6,r3
if( r6 < BMA_MapHeight )
    022F1C54 BAFFFFB5 blt     22F1B30h      ///022F1B30 ^
///022F1C58
022F1C58 E1D230F8 ldrsh   r3,[r2,8h]        //r3 = BMA_unk7
022F1C5C E3530001 cmp     r3,1h 
if( BMA_unk7 > 1 )
    022F1C60 C5D24001 ldrgtb  r4,[r2,1h]    //r4 = BMA_MapHeight
    022F1C64 C2803B01 addgt   r3,r0,400h    //r3 = r0 + 1024
    022F1C68 C3A05000 movgt   r5,0h         //r5 = 0
    022F1C6C C3540000 cmpgt   r4,0h
if( BMA_unk7 <= 1 || ((BMA_unk7 > 1) && r(4 > BMA_MapHeight)) )
    022F1C70 DA00004A ble     22F1DA0h      ///022F1DA0 v
022F1C74 EA000046 b       22F1D94h          ///022F1D94 v
///022F1C78
022F1C78 E3550000 cmp     r5,0h
022F1C7C E2833005 add     r3,r3,5h          //r3 = r3 + 5
022F1C80 E3A04000 mov     r4,0h             //r4 = 0
if( r5 != 0 )
    022F1C84 1A000017 bne     22F1CE8h      ///022F1CE8 v
022F1C88 E3A06080 mov     r6,80h            //r6 = 0x80 
022F1C8C EA000011 b       22F1CD8h          //022F1CD8 v
022F1C90 E4D18001 ldrb    r8,[r1],1h        //r8 = [r1], r1 += 1
022F1C94 E3580080 cmp     r8,80h    
if( r8 < 0x80  )
    022F1C98 BA00000B blt     22F1CCCh      ///022F1CCC v
022F1C9C E1A07006 mov     r7,r6             //r7 = r6
022F1CA0 EA000004 b       22F1CB8h          ///022F1CB8 v
///022F1CA4
022F1CA4 E1A09003 mov     r9,r3             //r9 = r3
022F1CA8 E4D30001 ldrb    r0,[r3],1h        //r0 = [r3], r3 += 1
022F1CAC E2877001 add     r7,r7,1h          //r7 = r7 + 1
022F1CB0 E3800040 orr     r0,r0,40h         //r0 = r0 | 0x40    
022F1CB4 E5C90000 strb    r0,[r9]           //[r9] = r0
///022F1CB8
022F1CB8 E1570008 cmp     r7,r8
if( r7 <= r8 )
    022F1CBC DAFFFFF8 ble     22F1CA4h      ///022F1CA4 ^
022F1CC0 E248007F sub     r0,r8,7Fh         //r0 = r8 - 0x7F
022F1CC4 E0844000 add     r4,r4,r0          //r4 = r4 + r0
022F1CC8 EA000002 b       22F1CD8h          ///022F1CD8 v
///022F1CCC
022F1CCC E2880001 add     r0,r8,1h          //r0 = r8 + 1
022F1CD0 E0833000 add     r3,r3,r0          //r3 = r3 + r0
022F1CD4 E0844000 add     r4,r4,r0          //r4 = r4 + r0
///022F1CD8
022F1CD8 E5D28000 ldrb    r8,[r2]           //r8 = BMA_MapWidth
022F1CDC E1540008 cmp     r4,r8
if( r4 < BMA_MapWidth )
    022F1CE0 BAFFFFEA blt     22F1C90h      ///022F1C90 ^
022F1CE4 EA000026 b       22F1D84h          ///022F1D84 v
///022F1CE8
022F1CE8 E2437C01 sub     r7,r3,100h        //r7 = r3 + 256
022F1CEC E3A00080 mov     r0,80h            //r0 = 0x80 
022F1CF0 E1A09004 mov     r9,r4             //r9 = r4
022F1CF4 EA00001F b       22F1D78h          ///022F1D78
///022F1CF8
022F1CF8 E4D16001 ldrb    r6,[r1],1h        //r6 = [r1], r1 += 1    
022F1CFC E3560080 cmp     r6,80h
if( r6 < 0x80 )
    022F1D00 BA00000E blt     22F1D40h      ///022F1D40 v
022F1D04 E1A0B000 mov     r11,r0            //r11 = r0
022F1D08 EA000007 b       22F1D2Ch          ///022F1D2C v
///022F1D0C
022F1D0C E4D78001 ldrb    r8,[r7],1h        //r8 = [r7], r7 += 1
022F1D10 E28BB001 add     r11,r11,1h        //r11 = r11 + 1
022F1D14 E3180040 tst     r8,40h
if( r8 & 0x40 == 0 )
    022F1D18 01A0A003 moveq   r10,r3        //r10 = r3
    022F1D1C 04D38001 ldreqb  r8,[r3],1h    //r8 = [r3], r3 += 1    
if( r8 & 0x40 != 0 )
    022F1D20 12833001 addne   r3,r3,1h      //r3 = r3 + 1
if( r8 & 0x40 == 0 )
    022F1D24 03888040 orreq   r8,r8,40h     //r8 = r8 | 0x40
    022F1D28 05CA8000 streqb  r8,[r10]      //[r10] = r8 
///022F1D2C
022F1D2C E15B0006 cmp     r11,r6
if( r11 <= r6 )
    022F1D30 DAFFFFF5 ble     22F1D0Ch      ///022F1D0C ^  
022F1D34 E246607F sub     r6,r6,7Fh         //r6 = r6 - 0x7F
022F1D38 E0844006 add     r4,r4,r6          //r4 = r4 + r6
022F1D3C EA00000D b       22F1D78h          ///022F1D78 v
///022F1D40
022F1D40 E1A0B009 mov     r11,r9            //r11 = r9 
022F1D44 EA000007 b       22F1D68h          ///022F1D68 v
///022F1D48
022F1D48 E4D78001 ldrb    r8,[r7],1h        //r8 = [r7], r7 += 1
022F1D4C E28BB001 add     r11,r11,1h        //r11 = r11 + 1
022F1D50 E3180040 tst     r8,40h
if( r8 & 0x40 != 0 )
    022F1D54 11A0A003 movne   r10,r3        //r10 = r3
    022F1D58 14D38001 ldrneb  r8,[r3],1h    //r8 = [r3], r3 += 1
if(r8 & 0x40 == 0)
    022F1D5C 02833001 addeq   r3,r3,1h      //r3 = r3 + 1
if( r8 & 0x40 != 0 )
    022F1D60 13888040 orrne   r8,r8,40h     //r8 = r8 + 0x40
    022F1D64 15CA8000 strneb  r8,[r10]      //[r10] = r8
///022F1D68
022F1D68 E15B0006 cmp     r11,r6
if( r11 <= r6 )
    022F1D6C DAFFFFF5 ble     22F1D48h      ///022F1D48 ^
022F1D70 E2866001 add     r6,r6,1h          //r6 = r6 + 1
022F1D74 E0844006 add     r4,r4,r6          //r4 = r4 + r6
///022F1D78
022F1D78 E5D28000 ldrb    r8,[r2]           //r8 = BMA_MapWidth
022F1D7C E1540008 cmp     r4,r8     
if( r4 < BMA_MapWidth )
    022F1D80 BAFFFFDC blt     22F1CF8h      ///022F1CF8 ^
///022F1D84
022F1D84 E2680C01 rsb     r0,r8,100h        //r0 = 256 - r8
022F1D88 E2400005 sub     r0,r0,5h          //r0 = r0 - 5
022F1D8C E0833000 add     r3,r3,r0          //r3 = r3 + r0
022F1D90 E2855001 add     r5,r5,1h          //r5 = r5 + 1
///022F1D94
022F1D94 E5D20001 ldrb    r0,[r2,1h]        //r0 = BMA_MapHeight
022F1D98 E1550000 cmp     r5,r0
if( r5 < BMA_MapHeight )
    022F1D9C BAFFFFB5 blt     22F1C78h      ///022F1C78 ^
///022F1DA0
022F1DA0 E1A00001 mov     r0,r1             //r0 = r1
022F1DA4 E8BD8FF8 pop     r3-r11,r15        //return


022F1DA8 E92D4008 push    r3,r14
022F1DAC E3500005 cmp     r0,5h
022F1DB0 908FF100 addls   r15,r15,r0,lsl 2h
022F1DB4 E8BD8008 pop     r3,r15
022F1DB8 E8BD8008 pop     r3,r15
022F1DBC EA000003 b       22F1DD0h
022F1DC0 EA000008 b       22F1DE8h
022F1DC4 EA00000D b       22F1E00h
022F1DC8 EA000012 b       22F1E18h
022F1DCC EA000016 b       22F1E2Ch
022F1DD0 E59F0068 ldr     r0,=2324CC0h
022F1DD4 E3A01000 mov     r1,0h
022F1DD8 E5900000 ldr     r0,[r0]
022F1DDC E1A02001 mov     r2,r1
022F1DE0 EBFFF201 bl      22EE5ECh
022F1DE4 E8BD8008 pop     r3,r15
022F1DE8 E59F0050 ldr     r0,=2324CC0h
022F1DEC E3A01001 mov     r1,1h
022F1DF0 E5900000 ldr     r0,[r0]
022F1DF4 E1A02001 mov     r2,r1
022F1DF8 EBFFF1FB bl      22EE5ECh
022F1DFC E8BD8008 pop     r3,r15
022F1E00 E59F0038 ldr     r0,=2324CC0h
022F1E04 E3A01000 mov     r1,0h
022F1E08 E5900000 ldr     r0,[r0]
022F1E0C E3A02001 mov     r2,1h
022F1E10 EBFFF1F5 bl      22EE5ECh
022F1E14 E8BD8008 pop     r3,r15
022F1E18 E59F0020 ldr     r0,=2324CC0h
022F1E1C E3A01001 mov     r1,1h
022F1E20 E5900000 ldr     r0,[r0]
022F1E24 EBFFF1EE bl      22EE5E4h
022F1E28 E8BD8008 pop     r3,r15
022F1E2C E59F000C ldr     r0,=2324CC0h
022F1E30 E3A01000 mov     r1,0h
022F1E34 E5900000 ldr     r0,[r0]
022F1E38 EBFFF1E9 bl      22EE5E4h
022F1E3C E8BD8008 pop     r3,r15
022F1E40 02324CC0 eoreqs  r4,r2,0C000h


/*
    022EBC18 fun_022EBC18 22EBC18h 
        This function seems to assign a few variables that are used throughout the map loading process!
        (r0 = PtrLevelStruct, r1 = Defvaltbl? )
        
        Default value entry is 28(0x1C) bytes long
        {
            0x00 [r10,2BCh] 
            0x02 [r10,2BEh] 
            0x04 [r10,2C0h] 
            
            0x06 [r10,2C2h] 
            0x08 [r10,2C4h] NbPalettesToParse
            
            0x0E [r10,2CAh] 
            
            0x14 [r10,2D0h]
            
        }
*/
022EBC18 E92D4FF8 push    r3-r11,r14
022EBC1C E24DD008 sub     r13,r13,8h            
022EBC20 E1A0A000 mov     r10,r0                //r10 = PtrLevelstruct
022EBC24 E1A05001 mov     r5,r1                 //r5  = DefaultValTbl
022EBC28 E28A4FAF add     r4,r10,2BCh           //r4  = PtrStructWithDefaultConstants
022EBC2C E8B5000F ldmia   [r5]!,r0-r3           // *
022EBC30 E8A4000F stmia   [r4]!,r0-r3           // *
022EBC34 E8950007 ldmia   [r5],r0-r2            // COPY DEFAULT TABLE VALUES!!
022EBC38 E8840007 stmia   [r4],r0-r2            // *
022EBC3C E5DA02BC ldrb    r0,[r10,2BCh]         //r0 = [r10,2BCh] (Is 0 when default for GroundMap_Select are loaded, otherwise always 1!)
022EBC40 E28A4C02 add     r4,r10,200h           //r4 = r10 + 200h
022EBC44 E3A06000 mov     r6,0h                 //r6 = CNT_Layer = 0
022EBC48 E3500000 cmp     r0,0h
if( [r10,2BCh] == 0 )
    022EBC4C 03A09006 moveq   r9,6h             //r9 = 6
else
    022EBC50 13A0900F movne   r9,0Fh            //r9 = 15
022EBC54 E1D47BFE ldrsh   r7,[r4,0BEh]          //r7 = [r10,2BEh]
022EBC58 E1A05006 mov     r5,r6                 //r5 = 0
022EBC5C E3A08012 mov     r8,12h                //r8 = 18
022EBC60 EA00000D b       22EBC9Ch              ///022EBC9C v

///
///022EBC64 - LOOP_BEG - Allocates layer buffers!
///
022EBC64 E08A0086 add     r0,r10,r6,lsl 1h      //r0 = r10 + (CNT_Layer * 2)
022EBC68 E2800C02 add     r0,r0,200h            //r0 = r0 + 200h
022EBC6C E1D01CFA ldrsh   r1,[r0,0CAh]          //r1 = [r10 + 200h + (CNT_Layer * 2)] //LayerSize?
022EBC70 E3510000 cmp     r1,0h
if( LayerSize <= 0 )
    022EBC74 D08A0106 addle   r0,r10,r6,lsl 2h  //r0 = r10 + (CNT_Layer * 4)
    022EBC78 D58052DC strle   r5,[r0,2DCh]      //[(r10 + (CNT_Layer * 4)) + 0x2DC] = 0
    022EBC7C DA000004 ble     22EBC94h          ///022EBC94 v
022EBC80 E0000891 mul     r0,r1,r8              //r0 = LayerSize * 18
022EBC84 E1A01009 mov     r1,r9                 //r1 =( 6 if [r10,2BCh] == 0, or 15 )
022EBC88 EBF45538 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022EBC8C E08A1106 add     r1,r10,r6,lsl 2h      //r1 = r10 + (CNT_Layer * 4)
022EBC90 E58102DC str     r0,[r1,2DCh]          //[(r10 + (CNT_Layer * 4)),2DCh] = AllocatedBuffer
///022EBC94
022EBC94 E2866001 add     r6,r6,1h              //r6 = CNT_Layer + 1
022EBC98 E2877001 add     r7,r7,1h              //r7 = r7 + 1
///022EBC9C
022EBC9C E1D40CF0 ldrsh   r0,[r4,0C0h]          //r0 = [r10,2C0h] //NbLayers in bpc!
022EBCA0 E1560000 cmp     r6,r0
if( CNT_Layer < NbLayers )
    022EBCA4 BAFFFFEE blt     22EBC64h          ///022EBC64 - LOOP_BEG ^

022EBCA8 E3A01000 mov     r1,0h                 //r1 = 0
022EBCAC EA000003 b       22EBCC0h              ///022EBCC0 v

///
///022EBCB0 - LOOP_BEG - Zero pointer to unused layers!
///
022EBCB0 E08A0106 add     r0,r10,r6,lsl 2h      //r0 = r10 + (CNT_Layer * 4)
022EBCB4 E58012DC str     r1,[r0,2DCh]          //[(r10 + (CNT_Layer * 4)),2DCh] = 0
022EBCB8 E2866001 add     r6,r6,1h              //r6 = CNT_Layer + 1
022EBCBC E2877001 add     r7,r7,1h              //r7 = r7 + 1
///022EBCC0
022EBCC0 E3560002 cmp     r6,2h
if( CNT_Layer < 2 )
    022EBCC4 BAFFFFF9 blt     22EBCB0h          ///022EBCB0 - LOOP_BEG ^

022EBCC8 E28A4C02 add     r4,r10,200h           //r4 = r10 + 200h
022EBCCC E3A07000 mov     r7,0h                 //r7 = CNT_r7 0
022EBCD0 E1D48BFE ldrsh   r8,[r4,0BEh]          //r8 = ( [r10,2BEh] )
022EBCD4 E1A06007 mov     r6,r7                 //r6 = 0
022EBCD8 E59F51C4 ldr     r5,=2324CBCh          //r5 = ptr_ptr_2324CBCh
022EBCDC E3A0B01C mov     r11,1Ch               //r11 = 28 (layer entry size in bytes!)
022EBCE0 EA00001B b       22EBD54h              ///022EBD54 v

///
///022EBCE4 - LOOP_Beg - Alloc BMA layer entries buffers!
///
022EBCE4 E5DA02BC ldrb    r0,[r10,2BCh]         //r0 = [r10,2BCh]
022EBCE8 E3500000 cmp     r0,0h
022EBCEC E5950000 ldr     r0,[r5]               //r0 = [ptr_ptr_2324CBCh]
if( [r10,2BCh] != 0 )
    022EBCF0 1A000002 bne     22EBD00h          ///022EBD00 v
022EBCF4 E0200B98 mla     r0,r8,r11,r0          //r0 = ([r10,2BEh] * 28) + [ptr_ptr_2324CBCh]
022EBCF8 EBF47E00 bl      200B500h              ///0200B500 (Returns the pointer at 0x18 in the entry we put the pointer to in R0)
{
    ///Fun 0200B500
    0200B500 E5900018 ldr     r0,[r0,18h]       //r0 = [r0,18h] = some pointer in the entry
    0200B504 E12FFF1E bx      r14
}
022EBCFC EA000003 b       22EBD10h              ///022EBD10 v
///022EBD00 - ( [r10,2BCh] != 0 )
022EBD00 E2801038 add     r1,r0,38h             //r1 = [ptr_ptr_2324CBCh] + 56 //(Skips first 2 layer entries)
022EBD04 E3A0001C mov     r0,1Ch                //r0 = 28 //Layer entry size in bytes
022EBD08 E0201098 mla     r0,r8,r0,r1           //r0 = ([r10,2BEh] * 28) + ([ptr_ptr_2324CBCh] + 56)
022EBD0C EBF47DFB bl      200B500h              ///0200B500 (Returns the pointer at 0x18 in the entry we put the pointer to in R0)
///022EBD10
022EBD10 E08A1107 add     r1,r10,r7,lsl 2h      //r1 = r10 + (CNT_r7 * 4)
022EBD14 E58102EC str     r0,[r1,2ECh]          //[(r10 + (CNT_r7 * 4)) + 2ECh] = r0 //PointerFrom default const table!
022EBD18 E08A0087 add     r0,r10,r7,lsl 1h      //r0 = r10 + (CNT_r7 * 2)
022EBD1C E2800C02 add     r0,r0,200h            //r0 = r10 + (CNT_r7 * 2) + 200h
022EBD20 E1D00CFA ldrsh   r0,[r0,0CAh]          //r0 = [(r10 + (CNT_r7 * 2) + 200h),CAh] //[r10,2CAh] AKA LayerSize!
022EBD24 E3500000 cmp     r0,0h
if( LayerSize <= 0 )
    022EBD28 D08A0107 addle   r0,r10,r7,lsl 2h  //r0 = r10 + (CNT_r7 * 4)
    022EBD2C D58062E4 strle   r6,[r0,2E4h]      //[(r10 + (CNT_r7 * 4)), 2E4h] = 0
    022EBD30 DA000005 ble     22EBD4Ch          ///022EBD4C v
022EBD34 E1D40DF0 ldrsh   r0,[r4,0D0h]          //r0 = [r10,2D0h] //usually 0x3E
022EBD38 E1A01009 mov     r1,r9                 //r1 =  (6 if [r10,2BCh] == 0, or 15 )
022EBD3C E1A00380 mov     r0,r0,lsl 7h          //r0 = [r10,2D0h] * 128
022EBD40 EBF4550A bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements )     
022EBD44 E08A1107 add     r1,r10,r7,lsl 2h      //r1 = r10 + (CNT_r7 * 4)
022EBD48 E58102E4 str     r0,[r1,2E4h]          //[(r10 + (CNT_r7 * 4)),2E4h] = PtrAllocatedBuffer
///022EBD4C
022EBD4C E2877001 add     r7,r7,1h              //r7 = CNT_r7 + 1
022EBD50 E2888001 add     r8,r8,1h              //r8 = r8 + 1
///022EBD54
022EBD54 E1D40CF0 ldrsh   r0,[r4,0C0h]          //r0 = ( [r10,2C0h] ) //NbLayers
022EBD58 E1570000 cmp     r7,r0
if( CNT_r7 < NbLayers )
    022EBD5C BAFFFFE0 blt     22EBCE4h          ///022EBCE4 - LOOP_Beg ^

022EBD60 E3A01000 mov     r1,0h                 //r1 = 0
022EBD64 EA000003 b       22EBD78h              ///022EBD78 v

///
///022EBD68 - LOOP_Beg - Zero pointers of unused layers!
///
022EBD68 E08A0107 add     r0,r10,r7,lsl 2h      //r0 = r10 + (CNT_r7 * 4)
022EBD6C E58012EC str     r1,[r0,2ECh]          //[(r10 + (CNT_r7 * 4)),2ECh] = 0
022EBD70 E58012E4 str     r1,[r0,2E4h]          //[(r10 + (CNT_r7 * 4)),2E4h] = 0
022EBD74 E2877001 add     r7,r7,1h              //r7 = CNT_r7 + 1
///022EBD78
022EBD78 E3570002 cmp     r7,2h
if( CNT_r7 < 2 )
    022EBD7C BAFFFFF9 blt     22EBD68h          ///022EBD68 - LOOP_Beg ^

//
//022EBD80 - Alloc BMA last table
//
022EBD80 E59A02D4 ldr     r0,[r10,2D4h]         //r0 = [r10,2D4h] //Function pointer, usually 0 or 0x22F1800
022EBD84 E3500000 cmp     r0,0h
if( [r10,2D4h] == 0 )
    022EBD88 03A00000 moveq   r0,0h             //r0 = 0
    022EBD8C 0A000004 beq     22EBDA4h          ///022EBDA4 v
022EBD90 E28A0C02 add     r0,r10,200h           //r0 = r10 + 200h
022EBD94 E1D00CFE ldrsh   r0,[r0,0CEh]          //r0 = [r10,2CEh] //Usually 0xBA
022EBD98 E1A01009 mov     r1,r9                 //r1 = (6 if [r10,2BCh] == 0, or 15 )
022EBD9C E1A00400 mov     r0,r0,lsl 8h          //r0 = r0 * 256 //0xBA * 256 = 0xBA00 //Size of the BMA last table??
022EBDA0 EBF454F2 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
///
///022EBDA4 - ([r10,2D4h] == 0) 
///
022EBDA4 E58A02D8 str     r0,[r10,2D8h]         //[r10,2D8h] = r0
022EBDA8 E1A0000A mov     r0,r10                //r0 = r10
022EBDAC EB00006B bl      22EBF60h              ///022EBF60 ZeroOutserveral buffers?
{
    022EBF60 E92D40F8 push    r3-r7,r14         
    022EBF64 E1A07000 mov     r7,r0             //r0 = PtrLevelStruct
    022EBF68 E3A06000 mov     r6,0h             //r6 = Cntr6 = 0
    022EBF6C E28750C4 add     r5,r7,0C4h        //r5 = PtrLevelStruct + 0xC4
    022EBF70 E3A0402C mov     r4,2Ch            //r4 = 44
    
    ///
    ///022EBF74 - LOOP_Beg -
    ///
    022EBF74 E0205496 mla     r0,r6,r4,r5       //r0 = Cntr6 * 44 + (PtrLevelStruct + 0xC4)
    022EBF78 E2800008 add     r0,r0,8h          //r0 = (Cntr6 * 44 + (PtrLevelStruct + 0xC4)) + 8
    022EBF7C EBF45CA9 bl      2003228h          ///fun_02003228
    {
        ///fun_02003228
        02003228 E3A01000 mov     r1,0h                 //r1 = 0
        0200322C E5801000 str     r1,[r0]               //[r0] = 0
        02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
        02003234 E12FFF1E bx      r14
    }
    022EBF80 E2866001 add     r6,r6,1h          //r6 = Cntr6 + 1
    022EBF84 E3560004 cmp     r6,4h
    if( Cntr6 < 4 )
        022EBF88 BAFFFFF9 blt     22EBF74h      ///022EBF74 - LOOP_Beg ^
    
    //Zero out file buffers and tables??
    022EBF8C E2870E1A add     r0,r7,1A0h        //r0 = PtrLevelStruct + 0x1A0
    022EBF90 EBF5DD9A bl      2063600h
    022EBF94 E2870F65 add     r0,r7,194h        //r0 = PtrLevelStruct + 0x194
    022EBF98 EBF5DD8A bl      20635C8h
    022EBF9C E3A01000 mov     r1,0h             //r1 = 0
    022EBFA0 E2870F5D add     r0,r7,174h        //r0 = PtrBPLBuff
    022EBFA4 E5C711BC strb    r1,[r7,1BCh]      //[PtrLevelStruct,1BCh] = 0
    022EBFA8 EBF45C9E bl      2003228h
    {
        ///fun_02003228
        02003228 E3A01000 mov     r1,0h                 //r1 = 0
        0200322C E5801000 str     r1,[r0]               //[r0] = 0
        02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
        02003234 E12FFF1E bx      r14
    }
    022EBFAC E2870F5F add     r0,r7,17Ch        //r0 = PtrBMA1Buff
    022EBFB0 EBF45C9C bl      2003228h
    {
        ///fun_02003228
        02003228 E3A01000 mov     r1,0h                 //r1 = 0
        0200322C E5801000 str     r1,[r0]               //[r0] = 0
        02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
        02003234 E12FFF1E bx      r14
    }
    022EBFB4 E2870F61 add     r0,r7,184h        //r0 = PtrBPCBuff
    022EBFB8 EBF45C9A bl      2003228h
    {
        ///fun_02003228
        02003228 E3A01000 mov     r1,0h                 //r1 = 0
        0200322C E5801000 str     r1,[r0]               //[r0] = 0
        02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
        02003234 E12FFF1E bx      r14
    }
    022EBFBC E2870F63 add     r0,r7,18Ch        //r0 = PtrBMABuff
    022EBFC0 EBF45C98 bl      2003228h
    {
        ///fun_02003228
        02003228 E3A01000 mov     r1,0h                 //r1 = 0
        0200322C E5801000 str     r1,[r0]               //[r0] = 0
        02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
        02003234 E12FFF1E bx      r14
    }
    022EBFC4 E8BD80F8 pop     r3-r7,r15
}
022EBDB0 E3A02000 mov     r2,0h             //r2 = 0
022EBDB4 E1CA20B0 strh    r2,[r10]          //
022EBDB8 E5CA21C0 strb    r2,[r10,1C0h]     //Zero
022EBDBC E5CA22BA strb    r2,[r10,2BAh]     //Zero
022EBDC0 E2421001 sub     r1,r2,1h          //r1 = -1
022EBDC4 E28A0C01 add     r0,r10,100h       //r0 = r10 + 100h
022EBDC8 E1C01BBE strh    r1,[r0,0BEh]      //Set LevelId to -1
022EBDCC E58A21F0 str     r2,[r10,1F0h]     //
022EBDD0 E5CA21E0 strb    r2,[r10,1E0h]     //
022EBDD4 E5CA21E1 strb    r2,[r10,1E1h]     //
022EBDD8 E5CA21E2 strb    r2,[r10,1E2h]     //
022EBDDC E5CA21E3 strb    r2,[r10,1E3h]     //
022EBDE0 E5CA21EE strb    r2,[r10,1EEh]     // Zero Everything
022EBDE4 E58A21F4 str     r2,[r10,1F4h]     //
022EBDE8 E5CA21F8 strb    r2,[r10,1F8h]     //    
022EBDEC E5CA21F9 strb    r2,[r10,1F9h]     //
022EBDF0 E5CA21FA strb    r2,[r10,1FAh]     //
022EBDF4 E5CA21FB strb    r2,[r10,1FBh]     //
022EBDF8 E5CA21BC strb    r2,[r10,1BCh]     //
022EBDFC E28A0004 add     r0,r10,4h         //r0 = r10 + 4
022EBE00 E1A03002 mov     r3,r2             //r3 = 0

///
///022EBE04 - LOOP_Beg 
///
022EBE04 E1C030B0 strh    r3,[r0]           //[r10+4] = 0
022EBE08 E1C030B2 strh    r3,[r0,2h]        //[(r10+4),2h] = 0 
022EBE0C E2822001 add     r2,r2,1h          //r2 = 0 + 1
022EBE10 E5803008 str     r3,[r0,8h]        //[(r10+4),8h] = 0
022EBE14 E5803004 str     r3,[r0,4h]        //[(r10+4),4h] = 0
022EBE18 E3520010 cmp     r2,10h
022EBE1C E280000C add     r0,r0,0Ch         //r0 = r0 + 12
if( r2 < 16 )
    022EBE20 BAFFFFF7 blt     22EBE04h      ///022EBE04 - LOOP_Beg ^

///
022EBE24 E28A20C4 add     r2,r10,0C4h       //r2 = r10 + 0xC4
022EBE28 E3A04000 mov     r4,0h             //r4 = 0
022EBE2C E3A0002C mov     r0,2Ch            //r0 = 44

///
///022EBE30 - LOOP_Beg
///
022EBE30 E0010093 mul     r1,r3,r0          //r1 = cntr3 * 44
022EBE34 E7C24001 strb    r4,[r2,r1]        //[(r10 + 0xC4),(cntr3 * 44)] = 0
022EBE38 E0821001 add     r1,r2,r1          //r1 = (r10 + 0xC4) + (cntr3 * 44)
022EBE3C E5C14001 strb    r4,[r1,1h]        //[r1,1h] = 0
022EBE40 E1C140B2 strh    r4,[r1,2h]        //[r1,2h] = 0
022EBE44 E1C140B4 strh    r4,[r1,4h]        //[r1,4h] = 0
022EBE48 E5814010 str     r4,[r1,10h]       //[r1,10h] = 0
022EBE4C E5814018 str     r4,[r1,18h]       //[r1,18h] = 0
022EBE50 E5814014 str     r4,[r1,14h]       //[r1,14h] = 0
022EBE54 E5814020 str     r4,[r1,20h]       //[r1,20h] = 0
022EBE58 E581401C str     r4,[r1,1Ch]       //[r1,1Ch] = 0
022EBE5C E2833001 add     r3,r3,1h          //r3 = cntr3 + 1
022EBE60 E5814024 str     r4,[r1,24h]       //[r1,24h] = 0
022EBE64 E5814028 str     r4,[r1,28h]       //[r1,28h] = 0
022EBE68 E3530004 cmp     r3,4h
if( cntr3 < 4 )
    022EBE6C BAFFFFEF blt     22EBE30h      ///022EBE30 - LOOP_Beg ^

022EBE70 E59F0030 ldr     r0,=2320BE4h      //r0 = ptr_tbl_2320BE4h
022EBE74 E5902014 ldr     r2,[r0,14h]       //r2 = [ptr_tbl_2320BE4h,14h]
022EBE78 E5901018 ldr     r1,[r0,18h]       //r1 = [ptr_tbl_2320BE4h,18h]
022EBE7C E58D2000 str     r2,[r13]          //[r13] = r2
022EBE80 E58D1004 str     r1,[r13,4h]       //[r13,4h] = r1

///
///022EBE84 - LOOP_Beg
///
022EBE84 E08A0184 add     r0,r10,r4,lsl 3h  //r0 = r10 + (cntr4 * 8)
022EBE88 E5802200 str     r2,[r0,200h]      //[(r10 + (cntr4 * 8)),200h] = [ptr_tbl_2320BE4h,14h]
022EBE8C E2844001 add     r4,r4,1h          //r4 = cntr4 + 1
022EBE90 E5801204 str     r1,[r0,204h]      //[(r10 + (cntr4 * 8)),204h] = [ptr_tbl_2320BE4h,18h]
022EBE94 E3540002 cmp     r4,2h 
if( cntr4 < 2 )    
    022EBE98 BAFFFFF9 blt     22EBE84h      ///022EBE84 - LOOP_Beg ^

022EBE9C E28DD008 add     r13,r13,8h
022EBEA0 E8BD8FF8 pop     r3-r11,r15
022EBEA4 02324CBC 
022EBEA8 02320BE4 



/*
    022EF40C 22EF40Ch
*/
022EF40C E92D43F8 push    r3-r9,r14
022EF410 E24DD008 sub     r13,r13,8h
022EF414 E1A09000 mov     r9,r0
022EF418 E1A08001 mov     r8,r1
022EF41C E1A07002 mov     r7,r2
022EF420 E3A06000 mov     r6,0h
022EF424 E28D5000 add     r5,r13,0h
022EF428 E3A04001 mov     r4,1h
022EF42C EA000014 b       22EF484h
022EF430 E0880186 add     r0,r8,r6,lsl 3h
022EF434 E5903200 ldr     r3,[r0,200h]
022EF438 E590C204 ldr     r12,[r0,204h]
022EF43C E1A00009 mov     r0,r9
022EF440 E58D3000 str     r3,[r13]
022EF444 E58DC004 str     r12,[r13,4h]
022EF448 E5972000 ldr     r2,[r7]
022EF44C E1A01004 mov     r1,r4
022EF450 E0832002 add     r2,r3,r2
022EF454 E58D2000 str     r2,[r13]
022EF458 E5973004 ldr     r3,[r7,4h]
022EF45C E1A02005 mov     r2,r5
022EF460 E08C3003 add     r3,r12,r3
022EF464 E58D3004 str     r3,[r13,4h]
022EF468 EBFFFF9F bl      22EF2ECh                  ///022EF2EC
022EF46C E0892186 add     r2,r9,r6,lsl 3h
022EF470 E59D1000 ldr     r1,[r13]
022EF474 E59D0004 ldr     r0,[r13,4h]
022EF478 E2866001 add     r6,r6,1h
022EF47C E5821200 str     r1,[r2,200h]
022EF480 E5820204 str     r0,[r2,204h]
022EF484 E59901FC ldr     r0,[r9,1FCh]
022EF488 E1560000 cmp     r6,r0
022EF48C BAFFFFE7 blt     22EF430h
022EF490 E28DD008 add     r13,r13,8h
022EF494 E8BD83F8 pop     r3-r9,r15

/*
    022EF498 22EF498h HandleMapResolution?? 
    r0 = PtrLevelBgData
    
    This could be multiplying the BMA tile values into pixels, 
    and shifting their value into 8.8 fixed point numbers.
    Because the 11 bits shift left gives the same result as multiplying by 8 and shifting 
    left by 8 bits, so the integral part of the result is in the high byte.
*/
022EF498 E3A03000 mov     r3,0h                     //r3 = 0
022EF49C E5813000 str     r3,[r1]                   //
022EF4A0 E5813004 str     r3,[r1,4h]                //
022EF4A4 E5D011E0 ldrb    r1,[r0,1E0h]              //r1 = BMA_Width
022EF4A8 E1A01581 mov     r1,r1,lsl 0Bh             //r1 = BMA_Width * 2048
022EF4AC E5821000 str     r1,[r2]                   //(r2 was 027E3690)
022EF4B0 E5D001E1 ldrb    r0,[r0,1E1h]              //r0 = BMA_Height
022EF4B4 E1A00580 mov     r0,r0,lsl 0Bh             //r0 = BMA_Height * 2048
022EF4B8 E5820004 str     r0,[r2,4h]                //(r2 + 4 was 027E3694)
022EF4BC E12FFF1E bx      r14

/*
    022F1F3C 22F1F3Ch
        Seems to handle the 2 first values in the BMA file? 
        Is called on each redraw of the background.
        r0 = PtrBuff2
        r1 = PtrBuff1
*/
022F1F3C E92D40F8 push    r3-r7,r14
022F1F40 E59F20FC ldr     r2,=2324CC0h              //r2 = AddressUnkPointerTbl_2324CC0
022F1F44 E1A07000 mov     r7,r0                     //r7 = r0 (was 0x27E3698)
022F1F48 E5920000 ldr     r0,[r2]                   //r0 = [r2] (was 0x210CA44) 
022F1F4C E1A06001 mov     r6,r1                     //r6 = r1 (was 0x27E3690)
022F1F50 E3500000 cmp     r0,0h
if( r0 != 0  )
    022F1F54 15922004 ldrne   r2,[r2,4h]            //r2 = [r2,4h] (was 0x02115800)
    022F1F58 13E01000 mvnne   r1,0h                 //r1 = 0xFFFFFFFF
    022F1F5C 11D240F0 ldrnesh r4,[r2]               //r4 = lvlid (was 0x120)
    022F1F60 11540001 cmpne   r4,r1
if( r0 == 0 || r4 == 0xFFFFFFFF )
    022F1F64 0A000034 beq     22F203Ch              ///022F203C
022F1F68 E3A0300C mov     r3,0Ch                    //r3 = 12
022F1F6C E1A01007 mov     r1,r7                     //r1 = r7
022F1F70 E1A02006 mov     r2,r6                     //r2 = r6
022F1F74 E1640384 smulbb  r4,r4,r3                  //r4 = lvlid * 12
022F1F78 E59F50C8 ldr     r5,=20A5488h              //r5 = LevelTableBeg
022F1F7C EBFFF545 bl      22EF498h                  ///022EF498 HandleMapResolution?? Shift them by 11 left!
{
    022EF498 E3A03000 mov     r3,0h                     //r3 = 0
    022EF49C E5813000 str     r3,[r1]                   //
    022EF4A0 E5813004 str     r3,[r1,4h]                //
    022EF4A4 E5D011E0 ldrb    r1,[r0,1E0h]              //r1 = BMA_Width
    022EF4A8 E1A01581 mov     r1,r1,lsl 0Bh             //r1 = BMA_Width * 2048
    022EF4AC E5821000 str     r1,[r2]                   //(r2 was 027E3690)
    022EF4B0 E5D001E1 ldrb    r0,[r0,1E1h]              //r0 = BMA_Height
    022EF4B4 E1A00580 mov     r0,r0,lsl 0Bh             //r0 = BMA_Height * 2048
    022EF4B8 E5820004 str     r0,[r2,4h]                //(r2 + 4 was 027E3694)
    022EF4BC E12FFF1E bx      r14
}
022F1F80 E5970000 ldr     r0,[r7]                   //r0 = [r7] (Will always be 0 here)
022F1F84 E2800902 add     r0,r0,8000h               //r0 = r0 + 0x8000
022F1F88 E5870000 str     r0,[r7]                   //[r7] =  (r0 + 0x8000)
022F1F8C EB007D32 bl      231145Ch                  ///0231145C
{
    0231145C E59F0014 ldr     r0,=2324E00h
    02311460 E5900000 ldr     r0,[r0]
    02311464 E3500000 cmp     r0,0h
    if( r0 != 0 )
        02311468 13A00001 movne   r0,1h             //r0 = 1
    else
        0231146C 03A00000 moveq   r0,0h             //r0 = 0
    02311470 E20000FF and     r0,r0,0FFh            //r0 = r0 & 0xFF
    02311474 E12FFF1E bx      r14
    02311478 02324E00 
}
022F1F90 E3500000 cmp     r0,0h
if( r0 != 0 )
    022F1F94 15970004 ldrne   r0,[r7,4h]            //r0 = [r7,4h] (Will always be 0 here)
    022F1F98 12800A12 addne   r0,r0,12000h          //r0 = r0 + 0x12000
    022F1F9C 15870004 strne   r0,[r7,4h]            //[r7,4h] = r0
    022F1FA0 1A000006 bne     22F1FC0h              ///022F1FC0
022F1FA4 EB007E6C bl      231195Ch                  ///0231195C 
{
    0231195C E59F0014 ldr     r0,=2324E08h          //r0 = 0x2324E08 
    02311960 E5900000 ldr     r0,[r0]               //r0 = [0x2324E08] //Some pointer
    02311964 E3500000 cmp     r0,0h
    if( r0 != 0 )
        02311968 13A00001 movne   r0,1h             //r0 = 1
    else
        0231196C 03A00000 moveq   r0,0h             //r0 = 0
    02311970 E20000FF and     r0,r0,0FFh            //r0 = & 0xFF
    02311974 E12FFF1E bx      r14                   //Return
}
022F1FA8 E3500000 cmp     r0,0h
022F1FAC E5970004 ldr     r0,[r7,4h]
if( r0_at_022F1FA8 != 0 )
    022F1FB0 12800A16 addne   r0,r0,16000h
    022F1FB4 15870004 strne   r0,[r7,4h]
else
    022F1FB8 02800A06 addeq   r0,r0,6000h
    022F1FBC 05870004 streq   r0,[r7,4h]
///022F1FC0
022F1FC0 E5960000 ldr     r0,[r6]                   //r0 = (BMA_Width << 11)
022F1FC4 E2400902 sub     r0,r0,8000h               //r0 = (BMA_Width << 11) - 0x8000
022F1FC8 E5860000 str     r0,[r6]                   //[r6] = (BMA_Width << 11) - 0x8000
022F1FCC E5960004 ldr     r0,[r6,4h]                //r0 = (BMA_Height << 11)
022F1FD0 E2400A06 sub     r0,r0,6000h               //r0 = (BMA_Height << 11) - 0x6000
022F1FD4 E5860004 str     r0,[r6,4h]                //[r6,4h] = (BMA_Height << 11) - 0x6000
022F1FD8 E19500F4 ldrsh   r0,[r5,r4]                //r0 = [LevelTableBeg + (lvlid * 12)] (unkcategory?) (was 0x9)
022F1FDC E350000A cmp     r0,0Ah                    
if( unkcategory? != 0xA )
    022F1FE0 1A00000B bne     22F2014h              ///022F2014
022F1FE4 E5970000 ldr     r0,[r7]                   //r0 = [r7] (was 0x8000)
022F1FE8 E2800B06 add     r0,r0,1800h               //r0 = r0 + 0x1800
022F1FEC E5870000 str     r0,[r7]                   //[r7] = (r0 + 0x1800)
022F1FF0 E5970004 ldr     r0,[r7,4h]                //r0 = [r7,4h] (was )
022F1FF4 E2800B06 add     r0,r0,1800h               //
022F1FF8 E5870004 str     r0,[r7,4h]
022F1FFC E5960000 ldr     r0,[r6]                   //r0 = (BMA_Width << 11)
022F2000 E2400B06 sub     r0,r0,1800h
022F2004 E5860000 str     r0,[r6]
022F2008 E5960004 ldr     r0,[r6,4h]                //r0 = (BMA_Height << 11)
022F200C E2400B06 sub     r0,r0,1800h
022F2010 E5860004 str     r0,[r6,4h]
///022F2014
022F2014 E5971000 ldr     r1,[r7]                   //r1 = [r7] (was 0x8000)
022F2018 E5960000 ldr     r0,[r6]                   //r0 = ((BMA_Width << 11) - 0x8000)
022F201C E1510000 cmp     r1,r0                     //If the value at [r7] is bigger than the one at [r6], we clamp the value at [r6] to [r7]
if( r1 > r0 )
    022F2020 C5861000 strgt   r1,[r6]               //[r6] = ([r7])
022F2024 E5971004 ldr     r1,[r7,4h]                //r1 = [r7,4h] (was 0x12000)
022F2028 E5960004 ldr     r0,[r6,4h]                //r0 = ((BMA_Height << 11) - 0x6000) //(was 0x1E000)
022F202C E1510000 cmp     r1,r0                     //If the value at [r7,4h] is bigger than the one at [r6,4h], we clamp the value at [r6,4h] to [r7,4h]
if( r1 > r0 )
    022F2030 C5861004 strgt   r1,[r6,4h]            //[r6,4h] = r1
022F2034 E3A00001 mov     r0,1h                     //r0 = 1
022F2038 E8BD80F8 pop     r3-r7,r15
///022F203C
022F203C E3A00000 mov     r0,0h                     //r0 = 0
022F2040 E8BD80F8 pop     r3-r7,r15
022F2044 02324CC0 
022F2048 020A5488 


/*
    022EEBF8 22EEBF8h tilemapstuff????
*/
022EEBF8 E92D4FF8 push    r3-r11,r14
022EEBFC E24DD010 sub     r13,r13,10h
022EEC00 E5906048 ldr     r6,[r0,48h]
022EEC04 E5905044 ldr     r5,[r0,44h]
022EEC08 E5902008 ldr     r2,[r0,8h]
022EEC0C E590300C ldr     r3,[r0,0Ch]
022EEC10 E0422005 sub     r2,r2,r5
022EEC14 E58D2000 str     r2,[r13]
022EEC18 E5904024 ldr     r4,[r0,24h]
022EEC1C E0851306 add     r1,r5,r6,lsl 6h
022EEC20 E0841081 add     r1,r4,r1,lsl 1h
022EEC24 E043C006 sub     r12,r3,r6
022EEC28 E3A02002 mov     r2,2h
022EEC2C E3A0B000 mov     r11,0h
022EEC30 E35C0000 cmp     r12,0h
022EEC34 C1A04001 movgt   r4,r1
022EEC38 C24CC001 subgt   r12,r12,1h
022EEC3C D5903044 ldrle   r3,[r0,44h]
022EEC40 D5901024 ldrle   r1,[r0,24h]
022EEC44 E59DE000 ldr     r14,[r13]
022EEC48 D0811083 addle   r1,r1,r3,lsl 1h
022EEC4C D590C00C ldrle   r12,[r0,0Ch]                  //r12 = BMA_unk4
022EEC50 E590702C ldr     r7,[r0,2Ch]                   
022EEC54 D1A04001 movle   r4,r1
022EEC58 E3A06000 mov     r6,0h
022EEC5C E28D3004 add     r3,r13,4h
022EEC60 E0875082 add     r5,r7,r2,lsl 1h               //Address tilemap info???
022EEC64 E7835106 str     r5,[r3,r6,lsl 2h]
022EEC68 E2866001 add     r6,r6,1h
022EEC6C E3560003 cmp     r6,3h
022EEC70 E2822020 add     r2,r2,20h
022EEC74 BAFFFFF9 blt     22EEC60h
022EEC78 E3A03000 mov     r3,0h
022EEC7C E0D450B2 ldrh    r5,[r4],2h
022EEC80 E590601C ldr     r6,[r0,1Ch]
022EEC84 E3A0A000 mov     r10,0h
022EEC88 E0855185 add     r5,r5,r5,lsl 3h
022EEC8C E0869085 add     r9,r6,r5,lsl 1h
022EEC90 E28D6004 add     r6,r13,4h
022EEC94 E796810A ldr     r8,[r6,r10,lsl 2h]
022EEC98 E1D950B0 ldrh    r5,[r9]                   //(r9 is a pointer the tilemapping data for the current screen)
022EEC9C E2896004 add     r6,r9,4h
022EECA0 E1C850B0 strh    r5,[r8]
022EECA4 E1D970B2 ldrh    r7,[r9,2h]
022EECA8 E2885006 add     r5,r8,6h
022EECAC E2899006 add     r9,r9,6h
022EECB0 E1C870B2 strh    r7,[r8,2h]
022EECB4 E1D660B0 ldrh    r6,[r6]
022EECB8 E1C860B4 strh    r6,[r8,4h]
022EECBC E28D6004 add     r6,r13,4h
022EECC0 E786510A str     r5,[r6,r10,lsl 2h]
022EECC4 E28AA001 add     r10,r10,1h
022EECC8 E35A0003 cmp     r10,3h
022EECCC BAFFFFEF blt     22EEC90h
022EECD0 E24EE001 sub     r14,r14,1h
022EECD4 E35E0000 cmp     r14,0h
022EECD8 D590E008 ldrle   r14,[r0,8h]
022EECDC E2833001 add     r3,r3,1h
022EECE0 D044408E suble   r4,r4,r14,lsl 1h
022EECE4 E353000A cmp     r3,0Ah
022EECE8 BAFFFFE3 blt     22EEC7Ch
022EECEC E3A07000 mov     r7,0h
022EECF0 E28D6004 add     r6,r13,4h
022EECF4 E7965107 ldr     r5,[r6,r7,lsl 2h]
022EECF8 E2855D1F add     r5,r5,7C0h
022EECFC E7865107 str     r5,[r6,r7,lsl 2h]
022EED00 E2877001 add     r7,r7,1h
022EED04 E3570003 cmp     r7,3h
022EED08 BAFFFFF9 blt     22EECF4h
022EED0C EA000019 b       22EED78h
022EED10 E0D450B2 ldrh    r5,[r4],2h
022EED14 E590601C ldr     r6,[r0,1Ch]
022EED18 E3A0A000 mov     r10,0h
022EED1C E0855185 add     r5,r5,r5,lsl 3h
022EED20 E0869085 add     r9,r6,r5,lsl 1h
022EED24 E28D6004 add     r6,r13,4h
022EED28 E796810A ldr     r8,[r6,r10,lsl 2h]
022EED2C E1D950B0 ldrh    r5,[r9]
022EED30 E2896004 add     r6,r9,4h
022EED34 E1C850B0 strh    r5,[r8]
022EED38 E1D970B2 ldrh    r7,[r9,2h]
022EED3C E2885006 add     r5,r8,6h
022EED40 E2899006 add     r9,r9,6h
022EED44 E1C870B2 strh    r7,[r8,2h]
022EED48 E1D660B0 ldrh    r6,[r6]
022EED4C E1C860B4 strh    r6,[r8,4h]
022EED50 E28D6004 add     r6,r13,4h
022EED54 E786510A str     r5,[r6,r10,lsl 2h]
022EED58 E28AA001 add     r10,r10,1h
022EED5C E35A0003 cmp     r10,3h
022EED60 BAFFFFEF blt     22EED24h
022EED64 E24EE001 sub     r14,r14,1h
022EED68 E35E0000 cmp     r14,0h
022EED6C D590E008 ldrle   r14,[r0,8h]                   //r14 = BMA_unk3
022EED70 E2833001 add     r3,r3,1h
022EED74 D044408E suble   r4,r4,r14,lsl 1h
022EED78 E353000C cmp     r3,0Ch
022EED7C BAFFFFE3 blt     22EED10h
022EED80 E28BB001 add     r11,r11,1h
022EED84 E35B0009 cmp     r11,9h
022EED88 E2811080 add     r1,r1,80h
022EED8C BAFFFFA7 blt     22EEC30h
022EED90 E28DD010 add     r13,r13,10h
022EED94 E8BD8FF8 pop     r3-r11,r15


/*
*/
0207A0CC E92D4038 push    r3-r5,r14
0207A0D0 E1A05000 mov     r5,r0
0207A0D4 EB0005C2 bl      207B7E4h
0207A0D8 E59F1044 ldr     r1,=22B966Ch
0207A0DC E1A04000 mov     r4,r0
0207A0E0 E5910004 ldr     r0,[r1,4h]
0207A0E4 E5951008 ldr     r1,[r5,8h]
0207A0E8 E1510000 cmp     r1,r0
0207A0EC 1A000009 bne     207A118h
0207A0F0 E595100C ldr     r1,[r5,0Ch]
0207A0F4 E2511001 subs    r1,r1,1h
0207A0F8 E585100C str     r1,[r5,0Ch]
0207A0FC 1A000005 bne     207A118h
0207A100 E1A01005 mov     r1,r5
0207A104 EB00003C bl      207A1FCh
0207A108 E3A01000 mov     r1,0h
0207A10C E1A00005 mov     r0,r5
0207A110 E5851008 str     r1,[r5,8h]
0207A114 EBFFFDEF bl      20798D8h
0207A118 E1A00004 mov     r0,r4
0207A11C EB0005B5 bl      207B7F8h
0207A120 E8BD8038 pop     r3-r5,r15
0207A124 022B966C eoreq   r9,r11,6C00000h

/*
    02311290 2311290h
*/
02311290 E92D4008 push    r3,r14
02311294 E24DD008 sub     r13,r13,8h
02311298 E59F0088 ldr     r0,=2324DF8h
0231129C E5900000 ldr     r0,[r0]
023112A0 E5900008 ldr     r0,[r0,8h]
023112A4 E3500004 cmp     r0,4h
if( r0 != 4 )
    023112A8 1A00001A bne     2311318h      ///02311318 v
023112AC E28D1000 add     r1,r13,0h
023112B0 E3A00001 mov     r0,1h
023112B4 EBFF6843 bl      22EB3C8h
023112B8 E3500000 cmp     r0,0h
if( r0 == 0 )
    023112BC 03A00000 moveq   r0,0h
    023112C0 058D0000 streq   r0,[r13]
    023112C4 058D0004 streq   r0,[r13,4h]
023112C8 E59F0058 ldr     r0,=2324DF8h
023112CC E28D1000 add     r1,r13,0h
023112D0 E5900000 ldr     r0,[r0]
023112D4 E3A02000 mov     r2,0h
023112D8 E2800010 add     r0,r0,10h
023112DC EBFF7C52 bl      22F042Ch
023112E0 E3500000 cmp     r0,0h
if(r0 == 0)
    023112E4 0A000004 beq     23112FCh
023112E8 E59F0038 ldr     r0,=2324DF8h
023112EC E28D2000 add     r2,r13,0h
023112F0 E5900004 ldr     r0,[r0,4h]
023112F4 E3A01000 mov     r1,0h
023112F8 EBFF77F5 bl      22EF2D4h
023112FC E59F0024 ldr     r0,=2324DF8h
02311300 E5900004 ldr     r0,[r0,4h]
02311304 EBFF7900 bl      22EF70Ch              ///022EF70C
02311308 E59F0018 ldr     r0,=2324DF8h
0231130C E3A01001 mov     r1,1h
02311310 E5900000 ldr     r0,[r0]
02311314 E5C01004 strb    r1,[r0,4h]
///02311318
02311318 EB0005D5 bl      2312A74h
0231131C E3A00000 mov     r0,0h
02311320 E28DD008 add     r13,r13,8h
02311324 E8BD8008 pop     r3,r15
02311328 02324DF8 


/*
    022EF70C 22EF70Ch
        Does something with the background scrolling and beginning offset!
*/
022EF70C E92D4FF8 push    r3-r11,r14
022EF710 E24DD010 sub     r13,r13,10h
022EF714 E1A0A000 mov     r10,r0            //r10 = r0
022EF718 E1DA00F0 ldrsh   r0,[r10]          //This is the value inside [r10] elsewhere, AKA the level struct ptr
022EF71C E2400004 sub     r0,r0,4h          //r0 = r0 - 4
022EF720 E1A00800 mov     r0,r0,lsl 10h     //r0 = r0 << 16    
022EF724 E1A00840 mov     r0,r0,asr 10h     //r0 = (r0 << 16) >> 16
022EF728 E1A00800 mov     r0,r0,lsl 10h     //r0 = ((r0 << 16) >> 16) << 16
022EF72C E1A00820 mov     r0,r0,lsr 10h     //r0 = (((r0 << 16) >> 16) << 16) >> 16
022EF730 E3500002 cmp     r0,2h
if( r0 <= 2 )
    022EF734 8A000031 bhi     22EF800h      ///022EF800
022EF738 E28A6E21 add     r6,r10,210h               //r6 = r10 + 0x210
022EF73C E3A08000 mov     r8,0h                     //r8 = 0
022EF740 E28A4C02 add     r4,r10,200h               //r4 = r10 + 0x200    
022EF744 E59F5418 ldr     r5,=2324CBCh              //r5 = 0x2324CBC
022EF748 E3A0B01C mov     r11,1Ch                   //r11 = 0x1C
022EF74C EA00001A b       22EF7BCh                  ///022EF7BC 
///022EF750
022EF750 E59F1410 ldr     r1,=2320C10h              //r1 = 0x2320C10
022EF754 E1A00006 mov     r0,r6                     //r0 = (r10 + 0x210)
022EF758 EBFFFC94 bl      22EE9B0h                  ///022EE9B0
{
    022EE9B0 E92D4010 push    r4,r14
    022EE9B4 E5912000 ldr     r2,[r1]               //r2 = [2320C10h]
    022EE9B8 E1A04000 mov     r4,r0                 //r4 = r10 + 210h
    022EE9BC E1A00142 mov     r0,r2,asr 2h          //r0 = [2320C10h] >> 2
    022EE9C0 E0820EA0 add     r0,r2,r0,lsr 1Dh      //r0 = [2320C10h] + (([2320C10h] >> 2) >> 29)
    022EE9C4 E5842034 str     r2,[r4,34h]           //[r4,34h] = r2
    022EE9C8 E1A001C0 mov     r0,r0,asr 3h          //r0 = r0 * 8 
    022EE9CC E584003C str     r0,[r4,3Ch]           //[r4,3C] = r0
    022EE9D0 E5911004 ldr     r1,[r1,4h]            //r1 = [0x2320C10 + 4h]
    022EE9D4 E1A00141 mov     r0,r1,asr 2h
    022EE9D8 E0810EA0 add     r0,r1,r0,lsr 1Dh
    022EE9DC E5841038 str     r1,[r4,38h]
    022EE9E0 E1A001C0 mov     r0,r0,asr 3h
    022EE9E4 E5840040 str     r0,[r4,40h]
    022EE9E8 E1D400F0 ldrsh   r0,[r4]
    022EE9EC E3500000 cmp     r0,0h
    022EE9F0 0A000002 beq     22EEA00h
    022EE9F4 E3500001 cmp     r0,1h
    022EE9F8 0A000006 beq     22EEA18h
    022EE9FC E8BD8010 pop     r4,r15
    022EEA00 E3A00000 mov     r0,0h
    022EEA04 E5840044 str     r0,[r4,44h]
    022EEA08 E5840048 str     r0,[r4,48h]
    022EEA0C E584004C str     r0,[r4,4Ch]
    022EEA10 E5840050 str     r0,[r4,50h]
    022EEA14 E8BD8010 pop     r4,r15
    022EEA18 E594003C ldr     r0,[r4,3Ch]
    022EEA1C E3A01003 mov     r1,3h
    022EEA20 EBF6851F bl      208FEA4h
    022EEA24 E5840044 str     r0,[r4,44h]
    022EEA28 E5940040 ldr     r0,[r4,40h]
    022EEA2C E3A01003 mov     r1,3h
    022EEA30 EBF6851B bl      208FEA4h
    022EEA34 E5840048 str     r0,[r4,48h]
    022EEA38 E5940034 ldr     r0,[r4,34h]
    022EEA3C E3A01018 mov     r1,18h
    022EEA40 EBF68517 bl      208FEA4h
    022EEA44 E2810010 add     r0,r1,10h
    022EEA48 E584004C str     r0,[r4,4Ch]
    022EEA4C E5940038 ldr     r0,[r4,38h]
    022EEA50 E3A01018 mov     r1,18h
    022EEA54 EBF68512 bl      208FEA4h
    022EEA58 E5841050 str     r1,[r4,50h]
    022EEA5C E8BD8010 pop     r4,r15
}
022EF75C E1D610F2 ldrsh   r1,[r6,2h]                //r1 = [(r10 + 0x210) + 2]
022EF760 E1D40BFE ldrsh   r0,[r4,0BEh]              //r0 = [(r10 + 0x200) + 0xBE]
022EF764 E3A09000 mov     r9,0h                     //r9 = 0
022EF768 E0817000 add     r7,r1,r0                  //r7 = [(r10 + 0x210) + 0x2] + [(r10 + 0x200) + 0xBE]
022EF76C EA00000D b       22EF7A8h                  ///022EF7A8
///022EF770
022EF770 E5DA02BC ldrb    r0,[r10,2BCh]             //r0 = [r10 + 0x2BC]
022EF774 E286104C add     r1,r6,4Ch                 //r1 = (r10 + 0x210) + 0x4C
022EF778 E3500000 cmp     r0,0h
022EF77C E5950000 ldr     r0,[r5]                   //r0 = [0x2324CBC]
if( r0 != 0 )
    022EF780 1A000002 bne     22EF790h              ///022EF790
022EF784 E0200B97 mla     r0,r7,r11,r0              //r0 = (r7 * r11) + r0
022EF788 EBF46F0C bl      200B3C0h                  //<- This write the value used for the BG scrolling top left pixel register! 400001Ch - BG3HOFS - BG3 X-Offset (W) 400001Eh - BG3VOFS - BG3 Y-Offset (W)
{
    0200B3C0 E5912000 ldr     r2,[r1]
    0200B3C4 E5911004 ldr     r1,[r1,4h]
    0200B3C8 E5802004 str     r2,[r0,4h]
    0200B3CC E5801008 str     r1,[r0,8h]
    0200B3D0 E12FFF1E bx      r14    
}
022EF78C EA000003 b       22EF7A0h                  ///022EF7A0
///022EF790
022EF790 E2802038 add     r2,r0,38h                 //r2 = r0 + 0x38
022EF794 E3A0001C mov     r0,1Ch                    //r0 = 0x1C
022EF798 E0202097 mla     r0,r7,r0,r2               //r0 = (r7 * r0) + r2
022EF79C EBF46F07 bl      200B3C0h                  //<- This write the value used for the BG scrolling top left pixel register! 400001Ch - BG3HOFS - BG3 X-Offset (W) 400001Eh - BG3VOFS - BG3 Y-Offset (W)
///022EF7A0
022EF7A0 E2899001 add     r9,r9,1h                  //r9 = r9 + 1
022EF7A4 E2877001 add     r7,r7,1h                  //r7 = r7 + 1
///022EF7A8
022EF7A8 E1D600F4 ldrsh   r0,[r6,4h]                //r0 = [r6 + 4]
022EF7AC E1590000 cmp     r9,r0
if( r9 < r0 )
    022EF7B0 BAFFFFEE blt     22EF770h              ///022EF770
022EF7B4 E2888001 add     r8,r8,1h                  //r8 = r8 + 1
022EF7B8 E2866054 add     r6,r6,54h                 //r6 = r6 + 0x54
///022EF7BC
022EF7BC E59A01FC ldr     r0,[r10,1FCh]             //r0 = [r10 + 0x1FC]
022EF7C0 E1580000 cmp     r8,r0
if( r8 < r0 )
    022EF7C4 BAFFFFE1 blt     22EF750h              ///022EF750 ^
022EF7C8 E3A00000 mov     r0,0h                     //r0 = 0
022EF7CC E5CA01C1 strb    r0,[r10,1C1h]             //[r10 + 0x1C1] = r0
022EF7D0 E1DA00F0 ldrsh   r0,[r10]                  //r0 = [r10]
022EF7D4 E3500006 cmp     r0,6h
if( r0 == 6 )
    022EF7D8 05DA01C0 ldreqb  r0,[r10,1C0h]
    022EF7DC 03500000 cmpeq   r0,0h
if( r0 != 6 || (r0 == 6 && r0 != 0) )
    022EF7E0 1A000003 bne     22EF7F4h              ///022EF7F4 v
022EF7E4 EBF49AF0 bl      20163ACh                  ///020163AC
{
    020163AC E92D4008 push    r3,r14
    020163B0 E59F1024 ldr     r1,=20AF760h
    020163B4 E3A00014 mov     r0,14h
    020163B8 E5912000 ldr     r2,[r1]
    020163BC E59F101C ldr     r1,=2099028h
    020163C0 E0000092 mul     r0,r2,r0
    020163C4 E7910000 ldr     r0,[r1,r0]
    020163C8 E3500000 cmp     r0,0h
    020163CC 03A00002 moveq   r0,2h
    020163D0 08BD8008 popeq   r3,r15
    020163D4 E12FFF30 blx     r0
    020163D8 E8BD8008 pop     r3,r15
    020163DC 020AF760 
    020163E0 02099028 
}
022EF7E8 E3500001 cmp     r0,1h
if( r0 == 1 )
    022EF7EC 03A00001 moveq   r0,1h                 //r0 = 1
    022EF7F0 05CA01C1 streqb  r0,[r10,1C1h]         //[r10 + 0x1C1] = 1
///022EF7F4
022EF7F4 E3A00001 mov     r0,1h                     //r0 = 1
022EF7F8 E5CA02BA strb    r0,[r10,2BAh]             //
022EF7FC EA0000D6 b       22EFB5Ch                  ///022EFB5C v
///022EF800
022EF800 E28ABC01 add     r11,r10,100h
022EF804 E1DB1BFE ldrsh   r1,[r11,0BEh]
022EF808 E3E00000 mvn     r0,0h
022EF80C E1510000 cmp     r1,r0
022EF810 0A0000D1 beq     22EFB5Ch
022EF814 E1DB0EFC ldrsh   r0,[r11,0ECh]
022EF818 E3500000 cmp     r0,0h
022EF81C 0A000068 beq     22EF9C4h
022EF820 E5DA01EE ldrb    r0,[r10,1EEh]
022EF824 E3500000 cmp     r0,0h
022EF828 1A000065 bne     22EF9C4h
022EF82C E28A0C02 add     r0,r10,200h
022EF830 E1D01CF2 ldrsh   r1,[r0,0C2h]
022EF834 E58D000C str     r0,[r13,0Ch]
022EF838 E59F0324 ldr     r0,=2324CBCh
022EF83C E5DA22BC ldrb    r2,[r10,2BCh]
022EF840 E5903000 ldr     r3,[r0]
022EF844 E3A09000 mov     r9,0h
022EF848 E3A00028 mov     r0,28h
022EF84C E58D9008 str     r9,[r13,8h]
022EF850 E1640082 smulbb  r4,r2,r0
022EF854 E1A01A01 mov     r1,r1,lsl 14h
022EF858 E5DA01F9 ldrb    r0,[r10,1F9h]             
022EF85C E59A71F4 ldr     r7,[r10,1F4h]             //r7 = PtrSecPartBPL
022EF860 E2835098 add     r5,r3,98h
022EF864 E28A6004 add     r6,r10,4h
022EF868 E1A08821 mov     r8,r1,lsr 10h
022EF86C E58D0004 str     r0,[r13,4h]
022EF870 EA00003F b       22EF974h
022EF874 E5960004 ldr     r0,[r6,4h]
022EF878 E3500000 cmp     r0,0h
022EF87C 0A000036 beq     22EF95Ch
022EF880 E1D600F2 ldrsh   r0,[r6,2h]
022EF884 E2400001 sub     r0,r0,1h
022EF888 E1C600B2 strh    r0,[r6,2h]
022EF88C E1D600F2 ldrsh   r0,[r6,2h]
022EF890 E3500000 cmp     r0,0h
022EF894 CA00002E bgt     22EF954h
022EF898 E1D600F0 ldrsh   r0,[r6]
022EF89C E2400001 sub     r0,r0,1h
022EF8A0 E1C600B0 strh    r0,[r6]
022EF8A4 E1D600F0 ldrsh   r0,[r6]
022EF8A8 E3500000 cmp     r0,0h
022EF8AC CA000010 bgt     22EF8F4h
022EF8B0 E5DA01F9 ldrb    r0,[r10,1F9h]
022EF8B4 E3500000 cmp     r0,0h
022EF8B8 0A000006 beq     22EF8D8h
022EF8BC E1D700F0 ldrsh   r0,[r7]
022EF8C0 E1C600B2 strh    r0,[r6,2h]
022EF8C4 E1D700F2 ldrsh   r0,[r7,2h]
022EF8C8 E1C600B0 strh    r0,[r6]
022EF8CC E5960004 ldr     r0,[r6,4h]
022EF8D0 E5860008 str     r0,[r6,8h]
022EF8D4 EA00000A b       22EF904h
022EF8D8 E3A00000 mov     r0,0h
022EF8DC E1C600B0 strh    r0,[r6]
022EF8E0 E1A00000 nop
022EF8E4 E1C600B2 strh    r0,[r6,2h]
022EF8E8 E3A00000 mov     r0,0h
022EF8EC E5860008 str     r0,[r6,8h]
022EF8F0 EA000003 b       22EF904h
022EF8F4 E1D710F0 ldrsh   r1,[r7]
022EF8F8 E3A00001 mov     r0,1h
022EF8FC E58D0004 str     r0,[r13,4h]
022EF900 E1C610B2 strh    r1,[r6,2h]
022EF904 E5960008 ldr     r0,[r6,8h]
022EF908 E3500000 cmp     r0,0h
022EF90C 0A000012 beq     22EF95Ch
022EF910 E3A00001 mov     r0,1h
022EF914 E58D0008 str     r0,[r13,8h]
022EF918 E59F224C ldr     r2,=2320BF4h
022EF91C E0850004 add     r0,r5,r4
022EF920 E1A01008 mov     r1,r8
022EF924 EBF46B19 bl      200A590h
022EF928 E2881001 add     r1,r8,1h
022EF92C E1A01801 mov     r1,r1,lsl 10h
022EF930 E5962008 ldr     r2,[r6,8h]
022EF934 E0850004 add     r0,r5,r4
022EF938 E1A01821 mov     r1,r1,lsr 10h
022EF93C E3A0300F mov     r3,0Fh
022EF940 EBF46B1A bl      200A5B0h
022EF944 E5960008 ldr     r0,[r6,8h]
022EF948 E280003C add     r0,r0,3Ch
022EF94C E5860008 str     r0,[r6,8h]
022EF950 EA000001 b       22EF95Ch
022EF954 E3A00001 mov     r0,1h
022EF958 E58D0004 str     r0,[r13,4h]
022EF95C E2899001 add     r9,r9,1h
022EF960 E286600C add     r6,r6,0Ch
022EF964 E2877004 add     r7,r7,4h
022EF968 E2880010 add     r0,r8,10h
022EF96C E1A00800 mov     r0,r0,lsl 10h
022EF970 E1A08820 mov     r8,r0,lsr 10h
022EF974 E1DB0EFA ldrsh   r0,[r11,0EAh]
022EF978 E1590000 cmp     r9,r0
022EF97C B59D000C ldrlt   r0,[r13,0Ch]
022EF980 B1D00CF4 ldrltsh r0,[r0,0C4h]
022EF984 B1590000 cmplt   r9,r0
022EF988 BAFFFFB9 blt     22EF874h
022EF98C E59D0008 ldr     r0,[r13,8h]
022EF990 E3500000 cmp     r0,0h
022EF994 0A000001 beq     22EF9A0h
022EF998 E0850004 add     r0,r5,r4
022EF99C EBF46AD8 bl      200A504h
022EF9A0 E5DA01F8 ldrb    r0,[r10,1F8h]
022EF9A4 E5CA01F9 strb    r0,[r10,1F9h]
022EF9A8 E5DA01F8 ldrb    r0,[r10,1F8h]
022EF9AC E3500000 cmp     r0,0h
022EF9B0 13A00000 movne   r0,0h
022EF9B4 15CA01FB strneb  r0,[r10,1FBh]
022EF9B8 059D0004 ldreq   r0,[r13,4h]
022EF9BC 05CA01FB streqb  r0,[r10,1FBh]
022EF9C0 EA000001 b       22EF9CCh
022EF9C4 E3A00000 mov     r0,0h
022EF9C8 E5CA01FB strb    r0,[r10,1FBh]
022EF9CC E5DA01BC ldrb    r0,[r10,1BCh]
022EF9D0 E3500000 cmp     r0,0h
022EF9D4 0A000005 beq     22EF9F0h
022EF9D8 E3A01000 mov     r1,0h
022EF9DC E28A0E1A add     r0,r10,1A0h
022EF9E0 E3A020A0 mov     r2,0A0h
022EF9E4 E3A03020 mov     r3,20h
022EF9E8 E58D1000 str     r1,[r13]
022EF9EC EBF5CF8F bl      2063830h
022EF9F0 E5DA01EE ldrb    r0,[r10,1EEh]
022EF9F4 E3500000 cmp     r0,0h
022EF9F8 1A000029 bne     22EFAA4h
022EF9FC E3A01000 mov     r1,0h
022EFA00 E28A00C4 add     r0,r10,0C4h
022EFA04 E1A06001 mov     r6,r1
022EFA08 E3A05001 mov     r5,1h
022EFA0C E3A0302C mov     r3,2Ch
022EFA10 E0020391 mul     r2,r1,r3
022EFA14 E7D04002 ldrb    r4,[r0,r2]
022EFA18 E0802002 add     r2,r0,r2
022EFA1C E3540000 cmp     r4,0h
022EFA20 0A00001C beq     22EFA98h
022EFA24 E1D270F4 ldrsh   r7,[r2,4h]
022EFA28 E2474001 sub     r4,r7,1h
022EFA2C E1C240B4 strh    r4,[r2,4h]
022EFA30 E3570000 cmp     r7,0h
022EFA34 CA000017 bgt     22EFA98h
022EFA38 E5924018 ldr     r4,[r2,18h]
022EFA3C E2844004 add     r4,r4,4h
022EFA40 E5824018 str     r4,[r2,18h]
022EFA44 E5924028 ldr     r4,[r2,28h]
022EFA48 E5927020 ldr     r7,[r2,20h]
022EFA4C E3C44001 bic     r4,r4,1h
022EFA50 E0874004 add     r4,r7,r4
022EFA54 E5824020 str     r4,[r2,20h]
022EFA58 E1D240F2 ldrsh   r4,[r2,2h]
022EFA5C E2844001 add     r4,r4,1h
022EFA60 E1C240B2 strh    r4,[r2,2h]
022EFA64 E5924010 ldr     r4,[r2,10h]
022EFA68 E1D270F2 ldrsh   r7,[r2,2h]
022EFA6C E1D440F2 ldrsh   r4,[r4,2h]
022EFA70 E1570004 cmp     r7,r4
022EFA74 A5924014 ldrge   r4,[r2,14h]
022EFA78 A5824018 strge   r4,[r2,18h]
022EFA7C A592401C ldrge   r4,[r2,1Ch]
022EFA80 A5824020 strge   r4,[r2,20h]
022EFA84 A1C260B2 strgeh  r6,[r2,2h]
022EFA88 E5C25001 strb    r5,[r2,1h]
022EFA8C E5924018 ldr     r4,[r2,18h]
022EFA90 E5944000 ldr     r4,[r4]
022EFA94 E1C240B4 strh    r4,[r2,4h]
022EFA98 E2811001 add     r1,r1,1h
022EFA9C E3510004 cmp     r1,4h
022EFAA0 BAFFFFDA blt     22EFA10h
022EFAA4 E5DA01C0 ldrb    r0,[r10,1C0h]
022EFAA8 E3500000 cmp     r0,0h
022EFAAC 1A00002A bne     22EFB5Ch
022EFAB0 E28A5E21 add     r5,r10,210h
022EFAB4 E28A6C02 add     r6,r10,200h
022EFAB8 E3A08000 mov     r8,0h
022EFABC EA000021 b       22EFB48h                  ///022EFB48 v
///022EFAC0
022EFAC0 E1A00005 mov     r0,r5
022EFAC4 E1A01006 mov     r1,r6
022EFAC8 EBFFFBB8 bl      22EE9B0h
022EFACC E5951018 ldr     r1,[r5,18h]
022EFAD0 E1A00005 mov     r0,r5
022EFAD4 E12FFF31 blx     r1
022EFAD8 E28A0C02 add     r0,r10,200h
022EFADC E1D510F2 ldrsh   r1,[r5,2h]
022EFAE0 E1D00BFE ldrsh   r0,[r0,0BEh]
022EFAE4 E3A09000 mov     r9,0h
022EFAE8 E59F4074 ldr     r4,=2324CBCh
022EFAEC E0817000 add     r7,r1,r0
022EFAF0 E3A0B01C mov     r11,1Ch
022EFAF4 EA00000D b       22EFB30h                  ///022EFB30 v
///022EFAF8
022EFAF8 E5DA02BC ldrb    r0,[r10,2BCh]
022EFAFC E285104C add     r1,r5,4Ch
022EFB00 E3500000 cmp     r0,0h
022EFB04 E5940000 ldr     r0,[r4]
if( r0 != 0 )
    022EFB08 1A000002 bne     22EFB18h              ///022EFB18 v
022EFB0C E0200B97 mla     r0,r7,r11,r0
022EFB10 EBF46E2A bl      200B3C0h                  //<- This write the value used for the BG scrolling top left pixel register! 400001Ch - BG3HOFS - BG3 X-Offset (W) 400001Eh - BG3VOFS - BG3 Y-Offset (W)
022EFB14 EA000003 b       22EFB28h
///022EFB18
022EFB18 E2802038 add     r2,r0,38h
022EFB1C E3A0001C mov     r0,1Ch
022EFB20 E0202097 mla     r0,r7,r0,r2
022EFB24 EBF46E25 bl      200B3C0h                  //<- This write the value used for the BG scrolling top left pixel register! 400001Ch - BG3HOFS - BG3 X-Offset (W) 400001Eh - BG3VOFS - BG3 Y-Offset (W)
022EFB28 E2899001 add     r9,r9,1h
022EFB2C E2877001 add     r7,r7,1h
///022EFB30
022EFB30 E1D500F4 ldrsh   r0,[r5,4h]
022EFB34 E1590000 cmp     r9,r0
if( r9 < r0 )
    022EFB38 BAFFFFEE blt     22EFAF8h              ///022EFAF8 ^
022EFB3C E2888001 add     r8,r8,1h
022EFB40 E2855054 add     r5,r5,54h
022EFB44 E2866008 add     r6,r6,8h
///022EFB48
022EFB48 E59A01FC ldr     r0,[r10,1FCh]
022EFB4C E1580000 cmp     r8,r0
if( r8 < r0 )
    022EFB50 BAFFFFDA blt     22EFAC0h              ///022EFAC0 ^
022EFB54 E3A00001 mov     r0,1h
022EFB58 E5CA02BA strb    r0,[r10,2BAh]
///022EFB5C
022EFB5C E28DD010 add     r13,r13,10h
022EFB60 E8BD8FF8 pop     r3-r11,r15
022EFB64 02324CBC 
022EFB68 02320C10 
022EFB6C 02320BF4 



/*
    022EF2EC 22EF2ECh
*/
022EF2EC E2803E21 add     r3,r0,210h                //r3 = r0 + 0x210 
022EF2F0 E3A00054 mov     r0,54h                    //r0 = 0x54
022EF2F4 E592C000 ldr     r12,[r2]                  //r12 = [r2]
022EF2F8 E0203091 mla     r0,r1,r0,r3               //r0 = (r1 * r0) + r3
022EF2FC E35C0000 cmp     r12,0h
if( r12 >= 0 )
    022EF300 AA00000B bge     22EF334h              ///022EF334
022EF304 E5D01006 ldrb    r1,[r0,6h]
022EF308 E3510000 cmp     r1,0h
022EF30C 0A000005 beq     22EF328h                  ///022EF328
///022EF310
022EF310 E5923000 ldr     r3,[r2]
022EF314 E5901010 ldr     r1,[r0,10h]
022EF318 E0931001 adds    r1,r3,r1
022EF31C E5821000 str     r1,[r2]
022EF320 4AFFFFFA bmi     22EF310h                  ///022EF310
022EF324 EA000015 b       22EF380h                  ///022EF380
///022EF328
022EF328 E3A01000 mov     r1,0h
022EF32C E5821000 str     r1,[r2]
022EF330 EA000012 b       22EF380h                  ///022EF380
///022EF334
022EF334 E5903010 ldr     r3,[r0,10h]
022EF338 E15C0003 cmp     r12,r3
022EF33C BA00000F blt     22EF380h
022EF340 E5D01006 ldrb    r1,[r0,6h]
022EF344 E3510000 cmp     r1,0h
022EF348 0A000007 beq     22EF36Ch                  ///022EF36C
///022EF34C
022EF34C E5923000 ldr     r3,[r2]
022EF350 E5901010 ldr     r1,[r0,10h]               //r1 = (BMA_Width * 8)
022EF354 E0433001 sub     r3,r3,r1                  //
022EF358 E5823000 str     r3,[r2]
022EF35C E5901010 ldr     r1,[r0,10h]               //r1 = (BMA_Width * 8)
022EF360 E1530001 cmp     r3,r1
022EF364 AAFFFFF8 bge     22EF34Ch                  ///022EF34C
022EF368 EA000004 b       22EF380h                  ///022EF380
///022EF36C
022EF36C E3530000 cmp     r3,0h
022EF370 C2431001 subgt   r1,r3,1h
022EF374 C5821000 strgt   r1,[r2]
022EF378 D3A01000 movle   r1,0h
022EF37C D5821000 strle   r1,[r2]
///022EF380
022EF380 E5921004 ldr     r1,[r2,4h]                //r1 = ((BMA_Height * 8) - 0xC0)
022EF384 E3510000 cmp     r1,0h
022EF388 AA00000B bge     22EF3BCh                  ///022EF3BC
022EF38C E5D01006 ldrb    r1,[r0,6h]
022EF390 E3510000 cmp     r1,0h
022EF394 0A000005 beq     22EF3B0h                  ///022EF3B0
///022EF398
022EF398 E5923004 ldr     r3,[r2,4h]
022EF39C E5901014 ldr     r1,[r0,14h]               //r1 = BMA_Height * 8)
022EF3A0 E0931001 adds    r1,r3,r1
022EF3A4 E5821004 str     r1,[r2,4h]
022EF3A8 4AFFFFFA bmi     22EF398h                  ///022EF398
022EF3AC E12FFF1E bx      r14
///022EF3B0
022EF3B0 E3A00000 mov     r0,0h
022EF3B4 E5820004 str     r0,[r2,4h]                //r1 = ((BMA_Height * 8) - 0xC0)
022EF3B8 E12FFF1E bx      r14
///022EF3BC
022EF3BC E5903014 ldr     r3,[r0,14h]               //r3 = (BMA_Height * 8)
022EF3C0 E1510003 cmp     r1,r3
022EF3C4 B12FFF1E bxlt    r14
022EF3C8 E5D01006 ldrb    r1,[r0,6h]
022EF3CC E3510000 cmp     r1,0h
022EF3D0 0A000007 beq     22EF3F4h                  ///022EF3F4
///022EF3D4
022EF3D4 E5923004 ldr     r3,[r2,4h]
022EF3D8 E5901014 ldr     r1,[r0,14h]
022EF3DC E0433001 sub     r3,r3,r1
022EF3E0 E5823004 str     r3,[r2,4h]
022EF3E4 E5901014 ldr     r1,[r0,14h]
022EF3E8 E1530001 cmp     r3,r1
022EF3EC AAFFFFF8 bge     22EF3D4h                  ///022EF3D4
022EF3F0 E12FFF1E bx      r14
///022EF3F4
022EF3F4 E3530000 cmp     r3,0h
022EF3F8 C2430001 subgt   r0,r3,1h
022EF3FC C5820004 strgt   r0,[r2,4h]
022EF400 D3A00000 movle   r0,0h
022EF404 D5820004 strle   r0,[r2,4h]
022EF408 E12FFF1E bx      r14



/*
    022F042C 22F042Ch
*/
022F042C E92D4070 push    r4-r6,r14
022F0430 E24DD080 sub     r13,r13,80h
022F0434 E1A06000 mov     r6,r0
022F0438 E5964000 ldr     r4,[r6]
022F043C E59F0D90 ldr     r0,=16Ah
022F0440 E2842C01 add     r2,r4,100h
022F0444 E1D22BFE ldrsh   r2,[r2,0BEh]
022F0448 E1A05001 mov     r5,r1
022F044C E1520000 cmp     r2,r0
022F0450 CA00003D bgt     22F054Ch
022F0454 AA0000E8 bge     22F07FCh
022F0458 E2401047 sub     r1,r0,47h
022F045C E1520001 cmp     r2,r1
022F0460 CA00001B bgt     22F04D4h
022F0464 AA0001F6 bge     22F0C44h
022F0468 E3520F43 cmp     r2,10Ch
022F046C CA00000D bgt     22F04A8h
022F0470 AA0002CF bge     22F0FB4h
022F0474 E3520008 cmp     r2,8h
022F0478 CA000001 bgt     22F0484h
022F047C 0A00006C beq     22F0634h
022F0480 EA00034E b       22F11C0h
022F0484 E35200F5 cmp     r2,0F5h
022F0488 CA00034C bgt     22F11C0h
022F048C E35200F3 cmp     r2,0F3h
022F0490 BA00034A blt     22F11C0h
022F0494 135200F4 cmpne   r2,0F4h
022F0498 0A0000D7 beq     22F07FCh
022F049C E35200F5 cmp     r2,0F5h
022F04A0 0A000192 beq     22F0AF0h
022F04A4 EA000345 b       22F11C0h
022F04A8 E2401059 sub     r1,r0,59h
022F04AC E1520001 cmp     r2,r1
022F04B0 CA000003 bgt     22F04C4h
022F04B4 E2400059 sub     r0,r0,59h
022F04B8 E1520000 cmp     r2,r0
022F04BC 0A000290 beq     22F0F04h
022F04C0 EA00033E b       22F11C0h
022F04C4 E2400048 sub     r0,r0,48h
022F04C8 E1520000 cmp     r2,r0
022F04CC 0A0001DC beq     22F0C44h
022F04D0 EA00033A b       22F11C0h
022F04D4 E2401013 sub     r1,r0,13h
022F04D8 E1520001 cmp     r2,r1
022F04DC CA000010 bgt     22F0524h
022F04E0 AA000053 bge     22F0634h
022F04E4 E2400041 sub     r0,r0,41h
022F04E8 E1520000 cmp     r2,r0
022F04EC CA000008 bgt     22F0514h
022F04F0 E2520F49 subs    r0,r2,124h
022F04F4 508FF100 addpl   r15,r15,r0,lsl 2h
022F04F8 EA000330 b       22F11C0h
022F04FC EA0001D0 b       22F0C44h
022F0500 EA000227 b       22F0DA4h
022F0504 EA0001CE b       22F0C44h
022F0508 EA00032C b       22F11C0h
022F050C EA00032B b       22F11C0h
022F0510 EA000223 b       22F0DA4h
022F0514 E59F0CBC ldr     r0,=156h
022F0518 E1520000 cmp     r2,r0
022F051C 0A000077 beq     22F0700h
022F0520 EA000326 b       22F11C0h
022F0524 E240100B sub     r1,r0,0Bh
022F0528 E1520001 cmp     r2,r1
022F052C CA000003 bgt     22F0540h
022F0530 E240000B sub     r0,r0,0Bh
022F0534 E1520000 cmp     r2,r0
022F0538 0A000070 beq     22F0700h
022F053C EA00031F b       22F11C0h
022F0540 E3520E16 cmp     r2,160h
022F0544 0A00026E beq     22F0F04h
022F0548 EA00031C b       22F11C0h
022F054C E2801037 add     r1,r0,37h
022F0550 E1520001 cmp     r2,r1
022F0554 CA000015 bgt     22F05B0h
022F0558 AA0001B9 bge     22F0C44h
022F055C E2801018 add     r1,r0,18h
022F0560 E1520001 cmp     r2,r1
022F0564 CA00000A bgt     22F0594h
022F0568 AA000121 bge     22F09F4h
022F056C E2801005 add     r1,r0,5h
022F0570 E1520001 cmp     r2,r1
022F0574 CA000003 bgt     22F0588h
022F0578 E2800005 add     r0,r0,5h
022F057C E1520000 cmp     r2,r0
022F0580 0A00009D beq     22F07FCh
022F0584 EA00030D b       22F11C0h
022F0588 E3520E17 cmp     r2,170h
022F058C 0A00005B beq     22F0700h
022F0590 EA00030A b       22F11C0h
022F0594 E3520E19 cmp     r2,190h
022F0598 CA000001 bgt     22F05A4h
022F059C 0A0000D5 beq     22F08F8h
022F05A0 EA000306 b       22F11C0h
022F05A4 E3520E1A cmp     r2,1A0h
022F05A8 0A0001A5 beq     22F0C44h
022F05AC EA000303 b       22F11C0h
022F05B0 E3520E1B cmp     r2,1B0h
022F05B4 CA00000A bgt     22F05E4h
022F05B8 AA0002B4 bge     22F1090h
022F05BC E2801041 add     r1,r0,41h
022F05C0 E1520001 cmp     r2,r1
022F05C4 CA000003 bgt     22F05D8h
022F05C8 E2800041 add     r0,r0,41h
022F05CC E1520000 cmp     r2,r0
022F05D0 0A00019B beq     22F0C44h
022F05D4 EA0002F9 b       22F11C0h
022F05D8 E3520F6B cmp     r2,1ACh
022F05DC 0A000198 beq     22F0C44h
022F05E0 EA0002F6 b       22F11C0h
022F05E4 E2801048 add     r1,r0,48h
022F05E8 E1520001 cmp     r2,r1
022F05EC CA000004 bgt     22F0604h
022F05F0 AA0002CC bge     22F1128h
022F05F4 E2800047 add     r0,r0,47h
022F05F8 E1520000 cmp     r2,r0
022F05FC 0A0002A3 beq     22F1090h
022F0600 EA0002EE b       22F11C0h
022F0604 E2801050 add     r1,r0,50h
022F0608 E1520001 cmp     r2,r1
022F060C CA0002EB bgt     22F11C0h
022F0610 E3520F6D cmp     r2,1B4h
022F0614 BA0002E9 blt     22F11C0h
022F0618 0A0002C2 beq     22F1128h
022F061C E280104C add     r1,r0,4Ch
022F0620 E1520001 cmp     r2,r1
022F0624 12800050 addne   r0,r0,50h
022F0628 11520000 cmpne   r2,r0
022F062C 0A000271 beq     22F0FF8h
022F0630 EA0002E2 b       22F11C0h
022F0634 E596200C ldr     r2,[r6,0Ch]
022F0638 E5961004 ldr     r1,[r6,4h]
022F063C E1A00004 mov     r0,r4
022F0640 E0821001 add     r1,r2,r1
022F0644 E586100C str     r1,[r6,0Ch]
022F0648 E5963010 ldr     r3,[r6,10h]
022F064C E5961008 ldr     r1,[r6,8h]
022F0650 E2862014 add     r2,r6,14h
022F0654 E0831001 add     r1,r3,r1
022F0658 E5861010 str     r1,[r6,10h]
022F065C E596300C ldr     r3,[r6,0Ch]
022F0660 E596C014 ldr     r12,[r6,14h]
022F0664 E1A01143 mov     r1,r3,asr 2h
022F0668 E0831EA1 add     r1,r3,r1,lsr 1Dh
022F066C E08C11C1 add     r1,r12,r1,asr 3h
022F0670 E5861014 str     r1,[r6,14h]
022F0674 E5963010 ldr     r3,[r6,10h]
022F0678 E596C018 ldr     r12,[r6,18h]
022F067C E1A01143 mov     r1,r3,asr 2h
022F0680 E0831EA1 add     r1,r3,r1,lsr 1Dh
022F0684 E08C11C1 add     r1,r12,r1,asr 3h
022F0688 E5861018 str     r1,[r6,18h]
022F068C E596300C ldr     r3,[r6,0Ch]
022F0690 E3A01000 mov     r1,0h
022F0694 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F0698 E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F069C E08C3EE3 add     r3,r12,r3,ror 1Dh
022F06A0 E586300C str     r3,[r6,0Ch]
022F06A4 E5963010 ldr     r3,[r6,10h]
022F06A8 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F06AC E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F06B0 E08C3EE3 add     r3,r12,r3,ror 1Dh
022F06B4 E5863010 str     r3,[r6,10h]
022F06B8 EBFFFB0B bl      22EF2ECh
022F06BC E5951000 ldr     r1,[r5]
022F06C0 E5960014 ldr     r0,[r6,14h]
022F06C4 E28D2078 add     r2,r13,78h
022F06C8 E0810000 add     r0,r1,r0
022F06CC E58D0078 str     r0,[r13,78h]
022F06D0 E5953004 ldr     r3,[r5,4h]
022F06D4 E5961018 ldr     r1,[r6,18h]
022F06D8 E1A00004 mov     r0,r4
022F06DC E0833001 add     r3,r3,r1
022F06E0 E3A01000 mov     r1,0h
022F06E4 E58D307C str     r3,[r13,7Ch]
022F06E8 EBFFFAFF bl      22EF2ECh
022F06EC E28D2078 add     r2,r13,78h
022F06F0 E1A00004 mov     r0,r4
022F06F4 E3A01000 mov     r1,0h
022F06F8 EBFFFAF5 bl      22EF2D4h
022F06FC EA0002B1 b       22F11C8h
022F0700 E5950000 ldr     r0,[r5]
022F0704 E28D2070 add     r2,r13,70h
022F0708 E58D0070 str     r0,[r13,70h]
022F070C E5953004 ldr     r3,[r5,4h]
022F0710 E1A00004 mov     r0,r4
022F0714 E3A01001 mov     r1,1h
022F0718 E58D3074 str     r3,[r13,74h]
022F071C EBFFFAF2 bl      22EF2ECh
022F0720 E28D2070 add     r2,r13,70h
022F0724 E1A00004 mov     r0,r4
022F0728 E3A01001 mov     r1,1h
022F072C EBFFFAE8 bl      22EF2D4h
022F0730 E596200C ldr     r2,[r6,0Ch]
022F0734 E5961004 ldr     r1,[r6,4h]
022F0738 E1A00004 mov     r0,r4
022F073C E0821001 add     r1,r2,r1
022F0740 E586100C str     r1,[r6,0Ch]
022F0744 E5963010 ldr     r3,[r6,10h]
022F0748 E5961008 ldr     r1,[r6,8h]
022F074C E2862014 add     r2,r6,14h
022F0750 E0831001 add     r1,r3,r1
022F0754 E5861010 str     r1,[r6,10h]
022F0758 E596300C ldr     r3,[r6,0Ch]
022F075C E596C014 ldr     r12,[r6,14h]
022F0760 E1A01143 mov     r1,r3,asr 2h
022F0764 E0831EA1 add     r1,r3,r1,lsr 1Dh
022F0768 E08C11C1 add     r1,r12,r1,asr 3h
022F076C E5861014 str     r1,[r6,14h]
022F0770 E5963010 ldr     r3,[r6,10h]
022F0774 E596C018 ldr     r12,[r6,18h]
022F0778 E1A01143 mov     r1,r3,asr 2h
022F077C E0831EA1 add     r1,r3,r1,lsr 1Dh
022F0780 E08C11C1 add     r1,r12,r1,asr 3h
022F0784 E5861018 str     r1,[r6,18h]
022F0788 E596300C ldr     r3,[r6,0Ch]
022F078C E3A01001 mov     r1,1h
022F0790 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F0794 E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F0798 E08C3EE3 add     r3,r12,r3,ror 1Dh
022F079C E586300C str     r3,[r6,0Ch]
022F07A0 E5963010 ldr     r3,[r6,10h]
022F07A4 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F07A8 E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F07AC E08C3EE3 add     r3,r12,r3,ror 1Dh
022F07B0 E5863010 str     r3,[r6,10h]
022F07B4 EBFFFACC bl      22EF2ECh
022F07B8 E5952000 ldr     r2,[r5]
022F07BC E5961014 ldr     r1,[r6,14h]
022F07C0 E1A00004 mov     r0,r4
022F07C4 E0821001 add     r1,r2,r1
022F07C8 E58D1070 str     r1,[r13,70h]
022F07CC E5953004 ldr     r3,[r5,4h]
022F07D0 E5961018 ldr     r1,[r6,18h]
022F07D4 E28D2070 add     r2,r13,70h
022F07D8 E0833001 add     r3,r3,r1
022F07DC E3A01000 mov     r1,0h
022F07E0 E58D3074 str     r3,[r13,74h]
022F07E4 EBFFFAC0 bl      22EF2ECh
022F07E8 E28D2070 add     r2,r13,70h
022F07EC E1A00004 mov     r0,r4
022F07F0 E3A01000 mov     r1,0h
022F07F4 EBFFFAB6 bl      22EF2D4h
022F07F8 EA000272 b       22F11C8h
022F07FC E5950000 ldr     r0,[r5]
022F0800 E28D2068 add     r2,r13,68h
022F0804 E58D0068 str     r0,[r13,68h]
022F0808 E5953004 ldr     r3,[r5,4h]
022F080C E1A00004 mov     r0,r4
022F0810 E3A01000 mov     r1,0h
022F0814 E58D306C str     r3,[r13,6Ch]
022F0818 EBFFFAB3 bl      22EF2ECh
022F081C E28D2068 add     r2,r13,68h
022F0820 E1A00004 mov     r0,r4
022F0824 E3A01000 mov     r1,0h
022F0828 EBFFFAA9 bl      22EF2D4h
022F082C E596200C ldr     r2,[r6,0Ch]
022F0830 E5961004 ldr     r1,[r6,4h]
022F0834 E1A00004 mov     r0,r4
022F0838 E0821001 add     r1,r2,r1
022F083C E586100C str     r1,[r6,0Ch]
022F0840 E5963010 ldr     r3,[r6,10h]
022F0844 E5961008 ldr     r1,[r6,8h]
022F0848 E2862014 add     r2,r6,14h
022F084C E0831001 add     r1,r3,r1
022F0850 E5861010 str     r1,[r6,10h]
022F0854 E596300C ldr     r3,[r6,0Ch]
022F0858 E596C014 ldr     r12,[r6,14h]
022F085C E1A01143 mov     r1,r3,asr 2h
022F0860 E0831EA1 add     r1,r3,r1,lsr 1Dh
022F0864 E08C11C1 add     r1,r12,r1,asr 3h
022F0868 E5861014 str     r1,[r6,14h]
022F086C E5963010 ldr     r3,[r6,10h]
022F0870 E596C018 ldr     r12,[r6,18h]
022F0874 E1A01143 mov     r1,r3,asr 2h
022F0878 E0831EA1 add     r1,r3,r1,lsr 1Dh
022F087C E08C11C1 add     r1,r12,r1,asr 3h
022F0880 E5861018 str     r1,[r6,18h]
022F0884 E596300C ldr     r3,[r6,0Ch]
022F0888 E3A01001 mov     r1,1h
022F088C E1A0CFA3 mov     r12,r3,lsr 1Fh
022F0890 E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F0894 E08C3EE3 add     r3,r12,r3,ror 1Dh
022F0898 E586300C str     r3,[r6,0Ch]
022F089C E5963010 ldr     r3,[r6,10h]
022F08A0 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F08A4 E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F08A8 E08C3EE3 add     r3,r12,r3,ror 1Dh
022F08AC E5863010 str     r3,[r6,10h]
022F08B0 EBFFFA8D bl      22EF2ECh
022F08B4 E5952000 ldr     r2,[r5]
022F08B8 E5961014 ldr     r1,[r6,14h]
022F08BC E1A00004 mov     r0,r4
022F08C0 E0821001 add     r1,r2,r1
022F08C4 E58D1068 str     r1,[r13,68h]
022F08C8 E5953004 ldr     r3,[r5,4h]
022F08CC E5961018 ldr     r1,[r6,18h]
022F08D0 E28D2068 add     r2,r13,68h
022F08D4 E0833001 add     r3,r3,r1
022F08D8 E3A01001 mov     r1,1h
022F08DC E58D306C str     r3,[r13,6Ch]
022F08E0 EBFFFA81 bl      22EF2ECh
022F08E4 E28D2068 add     r2,r13,68h
022F08E8 E1A00004 mov     r0,r4
022F08EC E3A01001 mov     r1,1h
022F08F0 EBFFFA77 bl      22EF2D4h
022F08F4 EA000233 b       22F11C8h
022F08F8 E5950000 ldr     r0,[r5]
022F08FC E28D2060 add     r2,r13,60h
022F0900 E58D0060 str     r0,[r13,60h]
022F0904 E5953004 ldr     r3,[r5,4h]
022F0908 E1A00004 mov     r0,r4
022F090C E3A01001 mov     r1,1h
022F0910 E58D3064 str     r3,[r13,64h]
022F0914 EBFFFA74 bl      22EF2ECh
022F0918 E28D2060 add     r2,r13,60h
022F091C E1A00004 mov     r0,r4
022F0920 E3A01001 mov     r1,1h
022F0924 EBFFFA6A bl      22EF2D4h
022F0928 E596200C ldr     r2,[r6,0Ch]
022F092C E5961004 ldr     r1,[r6,4h]
022F0930 E1A00004 mov     r0,r4
022F0934 E0821001 add     r1,r2,r1
022F0938 E586100C str     r1,[r6,0Ch]
022F093C E5963010 ldr     r3,[r6,10h]
022F0940 E5961008 ldr     r1,[r6,8h]
022F0944 E2862014 add     r2,r6,14h
022F0948 E0831001 add     r1,r3,r1
022F094C E5861010 str     r1,[r6,10h]
022F0950 E596300C ldr     r3,[r6,0Ch]
022F0954 E596C014 ldr     r12,[r6,14h]
022F0958 E1A013C3 mov     r1,r3,asr 7h
022F095C E0831C21 add     r1,r3,r1,lsr 18h
022F0960 E08C1441 add     r1,r12,r1,asr 8h
022F0964 E5861014 str     r1,[r6,14h]
022F0968 E5963010 ldr     r3,[r6,10h]
022F096C E596C018 ldr     r12,[r6,18h]
022F0970 E1A013C3 mov     r1,r3,asr 7h
022F0974 E0831C21 add     r1,r3,r1,lsr 18h
022F0978 E08C1441 add     r1,r12,r1,asr 8h
022F097C E5861018 str     r1,[r6,18h]
022F0980 E596300C ldr     r3,[r6,0Ch]
022F0984 E3A01001 mov     r1,1h
022F0988 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F098C E06C3C03 rsb     r3,r12,r3,lsl 18h
022F0990 E08C3C63 add     r3,r12,r3,ror 18h
022F0994 E586300C str     r3,[r6,0Ch]
022F0998 E5963010 ldr     r3,[r6,10h]
022F099C E1A0CFA3 mov     r12,r3,lsr 1Fh
022F09A0 E06C3C03 rsb     r3,r12,r3,lsl 18h
022F09A4 E08C3C63 add     r3,r12,r3,ror 18h
022F09A8 E5863010 str     r3,[r6,10h]
022F09AC EBFFFA4E bl      22EF2ECh
022F09B0 E5952000 ldr     r2,[r5]
022F09B4 E5961014 ldr     r1,[r6,14h]
022F09B8 E1A00004 mov     r0,r4
022F09BC E0821001 add     r1,r2,r1
022F09C0 E58D1060 str     r1,[r13,60h]
022F09C4 E5953004 ldr     r3,[r5,4h]
022F09C8 E5961018 ldr     r1,[r6,18h]
022F09CC E28D2060 add     r2,r13,60h
022F09D0 E0833001 add     r3,r3,r1
022F09D4 E3A01000 mov     r1,0h
022F09D8 E58D3064 str     r3,[r13,64h]
022F09DC EBFFFA42 bl      22EF2ECh
022F09E0 E28D2060 add     r2,r13,60h
022F09E4 E1A00004 mov     r0,r4
022F09E8 E3A01000 mov     r1,0h
022F09EC EBFFFA38 bl      22EF2D4h
022F09F0 EA0001F4 b       22F11C8h
022F09F4 E5950000 ldr     r0,[r5]
022F09F8 E28D2058 add     r2,r13,58h
022F09FC E58D0058 str     r0,[r13,58h]
022F0A00 E5953004 ldr     r3,[r5,4h]
022F0A04 E1A00004 mov     r0,r4
022F0A08 E3A01000 mov     r1,0h
022F0A0C E58D305C str     r3,[r13,5Ch]
022F0A10 EBFFFA35 bl      22EF2ECh
022F0A14 E28D2058 add     r2,r13,58h
022F0A18 E1A00004 mov     r0,r4
022F0A1C E3A01000 mov     r1,0h
022F0A20 EBFFFA2B bl      22EF2D4h
022F0A24 E596200C ldr     r2,[r6,0Ch]
022F0A28 E5961004 ldr     r1,[r6,4h]
022F0A2C E1A00004 mov     r0,r4
022F0A30 E0821001 add     r1,r2,r1
022F0A34 E586100C str     r1,[r6,0Ch]
022F0A38 E5963010 ldr     r3,[r6,10h]
022F0A3C E5961008 ldr     r1,[r6,8h]
022F0A40 E2862014 add     r2,r6,14h
022F0A44 E0831001 add     r1,r3,r1
022F0A48 E5861010 str     r1,[r6,10h]
022F0A4C E596300C ldr     r3,[r6,0Ch]
022F0A50 E596C014 ldr     r12,[r6,14h]
022F0A54 E1A01143 mov     r1,r3,asr 2h
022F0A58 E0831EA1 add     r1,r3,r1,lsr 1Dh
022F0A5C E08C11C1 add     r1,r12,r1,asr 3h
022F0A60 E5861014 str     r1,[r6,14h]
022F0A64 E5963010 ldr     r3,[r6,10h]
022F0A68 E596C018 ldr     r12,[r6,18h]
022F0A6C E1A01143 mov     r1,r3,asr 2h
022F0A70 E0831EA1 add     r1,r3,r1,lsr 1Dh
022F0A74 E08C11C1 add     r1,r12,r1,asr 3h
022F0A78 E5861018 str     r1,[r6,18h]
022F0A7C E596300C ldr     r3,[r6,0Ch]
022F0A80 E3A01001 mov     r1,1h
022F0A84 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F0A88 E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F0A8C E08C3EE3 add     r3,r12,r3,ror 1Dh
022F0A90 E586300C str     r3,[r6,0Ch]
022F0A94 E5963010 ldr     r3,[r6,10h]
022F0A98 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F0A9C E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F0AA0 E08C3EE3 add     r3,r12,r3,ror 1Dh
022F0AA4 E5863010 str     r3,[r6,10h]
022F0AA8 EBFFFA0F bl      22EF2ECh
022F0AAC E5952000 ldr     r2,[r5]
022F0AB0 E5961014 ldr     r1,[r6,14h]
022F0AB4 E1A00004 mov     r0,r4
022F0AB8 E0821001 add     r1,r2,r1
022F0ABC E58D1058 str     r1,[r13,58h]
022F0AC0 E5953004 ldr     r3,[r5,4h]
022F0AC4 E5961018 ldr     r1,[r6,18h]
022F0AC8 E28D2058 add     r2,r13,58h
022F0ACC E0833001 add     r3,r3,r1
022F0AD0 E3A01001 mov     r1,1h
022F0AD4 E58D305C str     r3,[r13,5Ch]
022F0AD8 EBFFFA03 bl      22EF2ECh
022F0ADC E28D2058 add     r2,r13,58h
022F0AE0 E1A00004 mov     r0,r4
022F0AE4 E3A01001 mov     r1,1h
022F0AE8 EBFFF9F9 bl      22EF2D4h
022F0AEC EA0001B5 b       22F11C8h
022F0AF0 E5950000 ldr     r0,[r5]
022F0AF4 E28D2050 add     r2,r13,50h
022F0AF8 E58D0050 str     r0,[r13,50h]
022F0AFC E5953004 ldr     r3,[r5,4h]
022F0B00 E1A00004 mov     r0,r4
022F0B04 E3A01000 mov     r1,0h
022F0B08 E58D3054 str     r3,[r13,54h]
022F0B0C EBFFF9F6 bl      22EF2ECh
022F0B10 E28D2050 add     r2,r13,50h
022F0B14 E1A00004 mov     r0,r4
022F0B18 E3A01000 mov     r1,0h
022F0B1C EBFFF9EC bl      22EF2D4h
022F0B20 E596100C ldr     r1,[r6,0Ch]
022F0B24 E5960004 ldr     r0,[r6,4h]
022F0B28 E0812000 add     r2,r1,r0
022F0B2C E586200C str     r2,[r6,0Ch]
022F0B30 E1A00142 mov     r0,r2,asr 2h
022F0B34 E5961014 ldr     r1,[r6,14h]
022F0B38 E0820EA0 add     r0,r2,r0,lsr 1Dh
022F0B3C E08101C0 add     r0,r1,r0,asr 3h
022F0B40 E5860014 str     r0,[r6,14h]
022F0B44 E596000C ldr     r0,[r6,0Ch]
022F0B48 E1A01FA0 mov     r1,r0,lsr 1Fh
022F0B4C E0610E80 rsb     r0,r1,r0,lsl 1Dh
022F0B50 E0810EE0 add     r0,r1,r0,ror 1Dh
022F0B54 E586000C str     r0,[r6,0Ch]
022F0B58 E5951000 ldr     r1,[r5]
022F0B5C E5960014 ldr     r0,[r6,14h]
022F0B60 E0911000 adds    r1,r1,r0
022F0B64 E58D1050 str     r1,[r13,50h]
022F0B68 E5962014 ldr     r2,[r6,14h]
022F0B6C E5953004 ldr     r3,[r5,4h]
022F0B70 E1A00142 mov     r0,r2,asr 2h
022F0B74 E0820EA0 add     r0,r2,r0,lsr 1Dh
022F0B78 E08301C0 add     r0,r3,r0,asr 3h
022F0B7C E58D0054 str     r0,[r13,54h]
022F0B80 5A00000B bpl     22F0BB4h
022F0B84 E1A023C1 mov     r2,r1,asr 7h
022F0B88 E0812C22 add     r2,r1,r2,lsr 18h
022F0B8C E1A03FA1 mov     r3,r1,lsr 1Fh
022F0B90 E1A02442 mov     r2,r2,asr 8h
022F0B94 E0631C01 rsb     r1,r3,r1,lsl 18h
022F0B98 E2422001 sub     r2,r2,1h
022F0B9C E0831C61 add     r1,r3,r1,ror 18h
022F0BA0 E0402282 sub     r2,r0,r2,lsl 5h
022F0BA4 E2810C01 add     r0,r1,100h
022F0BA8 E58D2054 str     r2,[r13,54h]
022F0BAC E58D0050 str     r0,[r13,50h]
022F0BB0 EA00000A b       22F0BE0h
022F0BB4 E3510C01 cmp     r1,100h
022F0BB8 BA000008 blt     22F0BE0h
022F0BBC E1A023C1 mov     r2,r1,asr 7h
022F0BC0 E0812C22 add     r2,r1,r2,lsr 18h
022F0BC4 E1A03FA1 mov     r3,r1,lsr 1Fh
022F0BC8 E1A02442 mov     r2,r2,asr 8h
022F0BCC E0631C01 rsb     r1,r3,r1,lsl 18h
022F0BD0 E0402282 sub     r2,r0,r2,lsl 5h
022F0BD4 E0830C61 add     r0,r3,r1,ror 18h
022F0BD8 E58D2054 str     r2,[r13,54h]
022F0BDC E58D0050 str     r0,[r13,50h]
022F0BE0 E28D2050 add     r2,r13,50h
022F0BE4 E1A00004 mov     r0,r4
022F0BE8 E3A01000 mov     r1,0h
022F0BEC EBFFF9B8 bl      22EF2D4h
022F0BF0 E5951000 ldr     r1,[r5]
022F0BF4 E5960014 ldr     r0,[r6,14h]
022F0BF8 E28D2050 add     r2,r13,50h
022F0BFC E0810000 add     r0,r1,r0
022F0C00 E0800FA0 add     r0,r0,r0,lsr 1Fh
022F0C04 E1A000C0 mov     r0,r0,asr 1h
022F0C08 E58D0050 str     r0,[r13,50h]
022F0C0C E5961014 ldr     r1,[r6,14h]
022F0C10 E5953004 ldr     r3,[r5,4h]
022F0C14 E1A001C1 mov     r0,r1,asr 3h
022F0C18 E0810E20 add     r0,r1,r0,lsr 1Ch
022F0C1C E0833240 add     r3,r3,r0,asr 4h
022F0C20 E1A00004 mov     r0,r4
022F0C24 E3A01001 mov     r1,1h
022F0C28 E58D3054 str     r3,[r13,54h]
022F0C2C EBFFF9AE bl      22EF2ECh
022F0C30 E28D2050 add     r2,r13,50h
022F0C34 E1A00004 mov     r0,r4
022F0C38 E3A01001 mov     r1,1h
022F0C3C EBFFF9A4 bl      22EF2D4h
022F0C40 EA000160 b       22F11C8h
022F0C44 E5950000 ldr     r0,[r5]
022F0C48 E28D2048 add     r2,r13,48h
022F0C4C E58D0048 str     r0,[r13,48h]
022F0C50 E5953004 ldr     r3,[r5,4h]
022F0C54 E1A00004 mov     r0,r4
022F0C58 E3A01000 mov     r1,0h
022F0C5C E58D304C str     r3,[r13,4Ch]
022F0C60 EBFFF9A1 bl      22EF2ECh
022F0C64 E28D2048 add     r2,r13,48h
022F0C68 E1A00004 mov     r0,r4
022F0C6C E3A01000 mov     r1,0h
022F0C70 EBFFF997 bl      22EF2D4h
022F0C74 E596100C ldr     r1,[r6,0Ch]
022F0C78 E5960004 ldr     r0,[r6,4h]
022F0C7C E28D2048 add     r2,r13,48h
022F0C80 E0810000 add     r0,r1,r0
022F0C84 E586000C str     r0,[r6,0Ch]
022F0C88 E5963010 ldr     r3,[r6,10h]
022F0C8C E5961008 ldr     r1,[r6,8h]
022F0C90 E1A00004 mov     r0,r4
022F0C94 E0831001 add     r1,r3,r1
022F0C98 E5861010 str     r1,[r6,10h]
022F0C9C E596300C ldr     r3,[r6,0Ch]
022F0CA0 E596C014 ldr     r12,[r6,14h]
022F0CA4 E1A01143 mov     r1,r3,asr 2h
022F0CA8 E0831EA1 add     r1,r3,r1,lsr 1Dh
022F0CAC E08C11C1 add     r1,r12,r1,asr 3h
022F0CB0 E5861014 str     r1,[r6,14h]
022F0CB4 E5963010 ldr     r3,[r6,10h]
022F0CB8 E596C018 ldr     r12,[r6,18h]
022F0CBC E1A01143 mov     r1,r3,asr 2h
022F0CC0 E0831EA1 add     r1,r3,r1,lsr 1Dh
022F0CC4 E08C11C1 add     r1,r12,r1,asr 3h
022F0CC8 E5861018 str     r1,[r6,18h]
022F0CCC E596300C ldr     r3,[r6,0Ch]
022F0CD0 E3A01000 mov     r1,0h
022F0CD4 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F0CD8 E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F0CDC E08C3EE3 add     r3,r12,r3,ror 1Dh
022F0CE0 E586300C str     r3,[r6,0Ch]
022F0CE4 E5963010 ldr     r3,[r6,10h]
022F0CE8 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F0CEC E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F0CF0 E08C3EE3 add     r3,r12,r3,ror 1Dh
022F0CF4 E5863010 str     r3,[r6,10h]
022F0CF8 E5963014 ldr     r3,[r6,14h]
022F0CFC E58D3048 str     r3,[r13,48h]
022F0D00 E58D104C str     r1,[r13,4Ch]
022F0D04 EBFFF978 bl      22EF2ECh
022F0D08 E59D0048 ldr     r0,[r13,48h]
022F0D0C E28D2048 add     r2,r13,48h
022F0D10 E5860014 str     r0,[r6,14h]
022F0D14 E5961018 ldr     r1,[r6,18h]
022F0D18 E1A00004 mov     r0,r4
022F0D1C E58D1048 str     r1,[r13,48h]
022F0D20 E3A01000 mov     r1,0h
022F0D24 EBFFF970 bl      22EF2ECh
022F0D28 E59D1048 ldr     r1,[r13,48h]
022F0D2C E1A00004 mov     r0,r4
022F0D30 E5861018 str     r1,[r6,18h]
022F0D34 E5953000 ldr     r3,[r5]
022F0D38 E5962014 ldr     r2,[r6,14h]
022F0D3C E3A01000 mov     r1,0h
022F0D40 E0832002 add     r2,r3,r2
022F0D44 E58D2048 str     r2,[r13,48h]
022F0D48 E5953004 ldr     r3,[r5,4h]
022F0D4C E28D2048 add     r2,r13,48h
022F0D50 E58D304C str     r3,[r13,4Ch]
022F0D54 EBFFF964 bl      22EF2ECh
022F0D58 E1A00004 mov     r0,r4
022F0D5C E3A01000 mov     r1,0h
022F0D60 E28D2048 add     r2,r13,48h
022F0D64 EBFFF95A bl      22EF2D4h
022F0D68 E5961018 ldr     r1,[r6,18h]
022F0D6C E5952000 ldr     r2,[r5]
022F0D70 E1A00004 mov     r0,r4
022F0D74 E0821001 add     r1,r2,r1
022F0D78 E58D1048 str     r1,[r13,48h]
022F0D7C E5952004 ldr     r2,[r5,4h]
022F0D80 E3A01001 mov     r1,1h
022F0D84 E58D204C str     r2,[r13,4Ch]
022F0D88 E28D2048 add     r2,r13,48h
022F0D8C EBFFF956 bl      22EF2ECh
022F0D90 E28D2048 add     r2,r13,48h
022F0D94 E1A00004 mov     r0,r4
022F0D98 E3A01001 mov     r1,1h
022F0D9C EBFFF94C bl      22EF2D4h
022F0DA0 EA000108 b       22F11C8h
022F0DA4 E5950000 ldr     r0,[r5]
022F0DA8 E28D2040 add     r2,r13,40h
022F0DAC E58D0040 str     r0,[r13,40h]
022F0DB0 E5953004 ldr     r3,[r5,4h]
022F0DB4 E1A00004 mov     r0,r4
022F0DB8 E3A01000 mov     r1,0h
022F0DBC E58D3044 str     r3,[r13,44h]
022F0DC0 EBFFF949 bl      22EF2ECh
022F0DC4 E28D2040 add     r2,r13,40h
022F0DC8 E1A00004 mov     r0,r4
022F0DCC E3A01000 mov     r1,0h
022F0DD0 EBFFF93F bl      22EF2D4h
022F0DD4 E596200C ldr     r2,[r6,0Ch]
022F0DD8 E5960004 ldr     r0,[r6,4h]
022F0DDC E3A01000 mov     r1,0h
022F0DE0 E0820000 add     r0,r2,r0
022F0DE4 E586000C str     r0,[r6,0Ch]
022F0DE8 E5963010 ldr     r3,[r6,10h]
022F0DEC E5960008 ldr     r0,[r6,8h]
022F0DF0 E28D2040 add     r2,r13,40h
022F0DF4 E0830000 add     r0,r3,r0
022F0DF8 E5860010 str     r0,[r6,10h]
022F0DFC E596300C ldr     r3,[r6,0Ch]
022F0E00 E596C014 ldr     r12,[r6,14h]
022F0E04 E1A00143 mov     r0,r3,asr 2h
022F0E08 E0830EA0 add     r0,r3,r0,lsr 1Dh
022F0E0C E08C01C0 add     r0,r12,r0,asr 3h
022F0E10 E5860014 str     r0,[r6,14h]
022F0E14 E5963010 ldr     r3,[r6,10h]
022F0E18 E596C018 ldr     r12,[r6,18h]
022F0E1C E1A00143 mov     r0,r3,asr 2h
022F0E20 E0830EA0 add     r0,r3,r0,lsr 1Dh
022F0E24 E08C01C0 add     r0,r12,r0,asr 3h
022F0E28 E5860018 str     r0,[r6,18h]
022F0E2C E596300C ldr     r3,[r6,0Ch]
022F0E30 E1A00004 mov     r0,r4
022F0E34 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F0E38 E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F0E3C E08C3EE3 add     r3,r12,r3,ror 1Dh
022F0E40 E586300C str     r3,[r6,0Ch]
022F0E44 E5963010 ldr     r3,[r6,10h]
022F0E48 E1A0CFA3 mov     r12,r3,lsr 1Fh
022F0E4C E06C3E83 rsb     r3,r12,r3,lsl 1Dh
022F0E50 E08C3EE3 add     r3,r12,r3,ror 1Dh
022F0E54 E5863010 str     r3,[r6,10h]
022F0E58 E58D1040 str     r1,[r13,40h]
022F0E5C E5963014 ldr     r3,[r6,14h]
022F0E60 E58D3044 str     r3,[r13,44h]
022F0E64 EBFFF920 bl      22EF2ECh
022F0E68 E59D0044 ldr     r0,[r13,44h]
022F0E6C E28D2040 add     r2,r13,40h
022F0E70 E5860014 str     r0,[r6,14h]
022F0E74 E5961018 ldr     r1,[r6,18h]
022F0E78 E1A00004 mov     r0,r4
022F0E7C E58D1044 str     r1,[r13,44h]
022F0E80 E3A01001 mov     r1,1h
022F0E84 EBFFF918 bl      22EF2ECh
022F0E88 E59D1044 ldr     r1,[r13,44h]
022F0E8C E1A00004 mov     r0,r4
022F0E90 E5861018 str     r1,[r6,18h]
022F0E94 E5952000 ldr     r2,[r5]
022F0E98 E3A01000 mov     r1,0h
022F0E9C E58D2040 str     r2,[r13,40h]
022F0EA0 E595C004 ldr     r12,[r5,4h]
022F0EA4 E5963014 ldr     r3,[r6,14h]
022F0EA8 E28D2040 add     r2,r13,40h
022F0EAC E08C3003 add     r3,r12,r3
022F0EB0 E58D3044 str     r3,[r13,44h]
022F0EB4 EBFFF90C bl      22EF2ECh
022F0EB8 E1A00004 mov     r0,r4
022F0EBC E3A01000 mov     r1,0h
022F0EC0 E28D2040 add     r2,r13,40h
022F0EC4 EBFFF902 bl      22EF2D4h
022F0EC8 E5951000 ldr     r1,[r5]
022F0ECC E1A00004 mov     r0,r4
022F0ED0 E58D1040 str     r1,[r13,40h]
022F0ED4 E5953004 ldr     r3,[r5,4h]
022F0ED8 E5962018 ldr     r2,[r6,18h]
022F0EDC E3A01001 mov     r1,1h
022F0EE0 E0832002 add     r2,r3,r2
022F0EE4 E58D2044 str     r2,[r13,44h]
022F0EE8 E28D2040 add     r2,r13,40h
022F0EEC EBFFF8FE bl      22EF2ECh
022F0EF0 E28D2040 add     r2,r13,40h
022F0EF4 E1A00004 mov     r0,r4
022F0EF8 E3A01001 mov     r1,1h
022F0EFC EBFFF8F4 bl      22EF2D4h
022F0F00 EA0000B0 b       22F11C8h
022F0F04 E596100C ldr     r1,[r6,0Ch]
022F0F08 E1A00004 mov     r0,r4
022F0F0C E2813002 add     r3,r1,2h
022F0F10 E586300C str     r3,[r6,0Ch]
022F0F14 E1A010C3 mov     r1,r3,asr 1h
022F0F18 E5962014 ldr     r2,[r6,14h]
022F0F1C E0831F21 add     r1,r3,r1,lsr 1Eh
022F0F20 E0821141 add     r1,r2,r1,asr 2h
022F0F24 E5861014 str     r1,[r6,14h]
022F0F28 E596100C ldr     r1,[r6,0Ch]
022F0F2C E2862014 add     r2,r6,14h
022F0F30 E2011003 and     r1,r1,3h
022F0F34 E586100C str     r1,[r6,0Ch]
022F0F38 E3A01000 mov     r1,0h
022F0F3C EBFFF8EA bl      22EF2ECh
022F0F40 E5951000 ldr     r1,[r5]
022F0F44 E5960014 ldr     r0,[r6,14h]
022F0F48 E28D2038 add     r2,r13,38h
022F0F4C E0810000 add     r0,r1,r0
022F0F50 E58D0038 str     r0,[r13,38h]
022F0F54 E5953004 ldr     r3,[r5,4h]
022F0F58 E1A00004 mov     r0,r4
022F0F5C E3A01000 mov     r1,0h
022F0F60 E58D303C str     r3,[r13,3Ch]
022F0F64 EBFFF8E0 bl      22EF2ECh
022F0F68 E28D2038 add     r2,r13,38h
022F0F6C E1A00004 mov     r0,r4
022F0F70 E3A01000 mov     r1,0h
022F0F74 EBFFF8D6 bl      22EF2D4h
022F0F78 E5951000 ldr     r1,[r5]
022F0F7C E5960014 ldr     r0,[r6,14h]
022F0F80 E28D2038 add     r2,r13,38h
022F0F84 E0410000 sub     r0,r1,r0
022F0F88 E58D0038 str     r0,[r13,38h]
022F0F8C E5953004 ldr     r3,[r5,4h]
022F0F90 E1A00004 mov     r0,r4
022F0F94 E3A01001 mov     r1,1h
022F0F98 E58D303C str     r3,[r13,3Ch]
022F0F9C EBFFF8D2 bl      22EF2ECh
022F0FA0 E28D2038 add     r2,r13,38h
022F0FA4 E1A00004 mov     r0,r4
022F0FA8 E3A01001 mov     r1,1h
022F0FAC EBFFF8C8 bl      22EF2D4h
022F0FB0 EA000084 b       22F11C8h
022F0FB4 E5951000 ldr     r1,[r5]
022F0FB8 E5960014 ldr     r0,[r6,14h]
022F0FBC E28D2030 add     r2,r13,30h
022F0FC0 E0810000 add     r0,r1,r0
022F0FC4 E58D0030 str     r0,[r13,30h]
022F0FC8 E5953004 ldr     r3,[r5,4h]
022F0FCC E5961018 ldr     r1,[r6,18h]
022F0FD0 E1A00004 mov     r0,r4
022F0FD4 E0833001 add     r3,r3,r1
022F0FD8 E3A01000 mov     r1,0h
022F0FDC E58D3034 str     r3,[r13,34h]
022F0FE0 EBFFF8C1 bl      22EF2ECh
022F0FE4 E28D2030 add     r2,r13,30h
022F0FE8 E1A00004 mov     r0,r4
022F0FEC E3A01000 mov     r1,0h
022F0FF0 EBFFF8B7 bl      22EF2D4h
022F0FF4 EA000073 b       22F11C8h
022F0FF8 E596000C ldr     r0,[r6,0Ch]
022F0FFC E5953000 ldr     r3,[r5]
022F1000 E5952004 ldr     r2,[r5,4h]
022F1004 E2800002 add     r0,r0,2h
022F1008 E586000C str     r0,[r6,0Ch]
022F100C E5960010 ldr     r0,[r6,10h]
022F1010 E58D3028 str     r3,[r13,28h]
022F1014 E2800001 add     r0,r0,1h
022F1018 E5860010 str     r0,[r6,10h]
022F101C E596500C ldr     r5,[r6,0Ch]
022F1020 E596C014 ldr     r12,[r6,14h]
022F1024 E1A00145 mov     r0,r5,asr 2h
022F1028 E0850EA0 add     r0,r5,r0,lsr 1Dh
022F102C E08C01C0 add     r0,r12,r0,asr 3h
022F1030 E5860014 str     r0,[r6,14h]
022F1034 E5965010 ldr     r5,[r6,10h]
022F1038 E596C018 ldr     r12,[r6,18h]
022F103C E1A00145 mov     r0,r5,asr 2h
022F1040 E0850EA0 add     r0,r5,r0,lsr 1Dh
022F1044 E08C01C0 add     r0,r12,r0,asr 3h
022F1048 E5860018 str     r0,[r6,18h]
022F104C E596500C ldr     r5,[r6,0Ch]
022F1050 E1A00004 mov     r0,r4
022F1054 E2054007 and     r4,r5,7h
022F1058 E586400C str     r4,[r6,0Ch]
022F105C E5964010 ldr     r4,[r6,10h]
022F1060 E28D1020 add     r1,r13,20h
022F1064 E2044007 and     r4,r4,7h
022F1068 E5864010 str     r4,[r6,10h]
022F106C E5964014 ldr     r4,[r6,14h]
022F1070 E58D202C str     r2,[r13,2Ch]
022F1074 E0433004 sub     r3,r3,r4
022F1078 E58D3020 str     r3,[r13,20h]
022F107C E5963018 ldr     r3,[r6,18h]
022F1080 E0822003 add     r2,r2,r3
022F1084 E58D2024 str     r2,[r13,24h]
022F1088 EB0006A6 bl      22F2B28h
022F108C EA00004D b       22F11C8h
022F1090 E596000C ldr     r0,[r6,0Ch]
022F1094 E5953000 ldr     r3,[r5]
022F1098 E5952004 ldr     r2,[r5,4h]
022F109C E2800002 add     r0,r0,2h
022F10A0 E586000C str     r0,[r6,0Ch]
022F10A4 E5960010 ldr     r0,[r6,10h]
022F10A8 E58D3018 str     r3,[r13,18h]
022F10AC E2800001 add     r0,r0,1h
022F10B0 E5860010 str     r0,[r6,10h]
022F10B4 E596500C ldr     r5,[r6,0Ch]
022F10B8 E596C014 ldr     r12,[r6,14h]
022F10BC E1A00145 mov     r0,r5,asr 2h
022F10C0 E0850EA0 add     r0,r5,r0,lsr 1Dh
022F10C4 E08C01C0 add     r0,r12,r0,asr 3h
022F10C8 E5860014 str     r0,[r6,14h]
022F10CC E5965010 ldr     r5,[r6,10h]
022F10D0 E596C018 ldr     r12,[r6,18h]
022F10D4 E1A00145 mov     r0,r5,asr 2h
022F10D8 E0850EA0 add     r0,r5,r0,lsr 1Dh
022F10DC E08C01C0 add     r0,r12,r0,asr 3h
022F10E0 E5860018 str     r0,[r6,18h]
022F10E4 E596500C ldr     r5,[r6,0Ch]
022F10E8 E1A00004 mov     r0,r4
022F10EC E2054007 and     r4,r5,7h
022F10F0 E586400C str     r4,[r6,0Ch]
022F10F4 E5964010 ldr     r4,[r6,10h]
022F10F8 E28D1010 add     r1,r13,10h
022F10FC E2044007 and     r4,r4,7h
022F1100 E5864010 str     r4,[r6,10h]
022F1104 E5964014 ldr     r4,[r6,14h]
022F1108 E58D201C str     r2,[r13,1Ch]
022F110C E0433004 sub     r3,r3,r4
022F1110 E58D3010 str     r3,[r13,10h]
022F1114 E5963018 ldr     r3,[r6,18h]
022F1118 E0822003 add     r2,r2,r3
022F111C E58D2014 str     r2,[r13,14h]
022F1120 EB000680 bl      22F2B28h
022F1124 EA000027 b       22F11C8h
022F1128 E596000C ldr     r0,[r6,0Ch]
022F112C E5953000 ldr     r3,[r5]
022F1130 E5952004 ldr     r2,[r5,4h]
022F1134 E2800002 add     r0,r0,2h
022F1138 E586000C str     r0,[r6,0Ch]
022F113C E5960010 ldr     r0,[r6,10h]
022F1140 E58D3008 str     r3,[r13,8h]
022F1144 E2800001 add     r0,r0,1h
022F1148 E5860010 str     r0,[r6,10h]
022F114C E596500C ldr     r5,[r6,0Ch]
022F1150 E596C014 ldr     r12,[r6,14h]
022F1154 E1A000C5 mov     r0,r5,asr 1h
022F1158 E0850F20 add     r0,r5,r0,lsr 1Eh
022F115C E08C0140 add     r0,r12,r0,asr 2h
022F1160 E5860014 str     r0,[r6,14h]
022F1164 E5965010 ldr     r5,[r6,10h]
022F1168 E596C018 ldr     r12,[r6,18h]
022F116C E1A000C5 mov     r0,r5,asr 1h
022F1170 E0850F20 add     r0,r5,r0,lsr 1Eh
022F1174 E08C0140 add     r0,r12,r0,asr 2h
022F1178 E5860018 str     r0,[r6,18h]
022F117C E596500C ldr     r5,[r6,0Ch]
022F1180 E1A00004 mov     r0,r4
022F1184 E2054003 and     r4,r5,3h
022F1188 E586400C str     r4,[r6,0Ch]
022F118C E5964010 ldr     r4,[r6,10h]
022F1190 E28D1000 add     r1,r13,0h
022F1194 E2044003 and     r4,r4,3h
022F1198 E5864010 str     r4,[r6,10h]
022F119C E5964014 ldr     r4,[r6,14h]
022F11A0 E58D200C str     r2,[r13,0Ch]
022F11A4 E0433004 sub     r3,r3,r4
022F11A8 E58D3000 str     r3,[r13]
022F11AC E5963018 ldr     r3,[r6,18h]
022F11B0 E0822003 add     r2,r2,r3
022F11B4 E58D2004 str     r2,[r13,4h]
022F11B8 EB00065A bl      22F2B28h
022F11BC EA000001 b       22F11C8h
022F11C0 E3A00001 mov     r0,1h
022F11C4 EA000000 b       22F11CCh
022F11C8 E3A00000 mov     r0,0h
022F11CC E28DD080 add     r13,r13,80h
022F11D0 E8BD8070 pop     r4-r6,r15
022F11D4 0000016A 
022F11D8 00000156 



/*
*/
0231195C E59F0014 ldr     r0,=2324E08h
02311960 E5900000 ldr     r0,[r0]
02311964 E3500000 cmp     r0,0h
02311968 13A00001 movne   r0,1h
0231196C 03A00000 moveq   r0,0h
02311970 E20000FF and     r0,r0,0FFh
02311974 E12FFF1E bx      r14


/*
    022EB2F4 22EB2F4h
*/
022EB2F4 E92D4038 push    r3-r5,r14
022EB2F8 E24DD010 sub     r13,r13,10h
022EB2FC E1A04001 mov     r4,r1
022EB300 E3500001 cmp     r0,1h
022EB304 E3A05000 mov     r5,0h
if( r0 != 1 )
    022EB308 1A000010 bne     22EB350h              ///022EB350
022EB30C EB00968C bl      2310D44h                  ///fun 02310D44 Check if a ptr is null
{
    02310D44 E59F0014 ldr     r0,=2324DF8h
    02310D48 E5900000 ldr     r0,[r0]
    02310D4C E3500000 cmp     r0,0h
    if( r0 != 0 )
        02310D50 13A00001 movne   r0,1h
    else
        02310D54 03A00000 moveq   r0,0h
    02310D58 E20000FF and     r0,r0,0FFh
    02310D5C E12FFF1E bx      r14
    02310D60 02324DF8
}
022EB310 E3500000 cmp     r0,0h
if( r0 == 0 )
    022EB314 0A000005 beq     22EB330h              ///022EB330
022EB318 E28D0008 add     r0,r13,8h
022EB31C E28D1000 add     r1,r13,0h
022EB320 EB0096AE bl      2310DE0h                  ///fun 02310DE0
022EB324 E3500000 cmp     r0,0h
if( r0 != 0 )
    022EB328 13A05001 movne   r5,1h                 //r5 = 1
022EB32C EA000007 b       22EB350h                  ///022EB350
///022EB330
022EB330 EB008E7C bl      230ED28h                  ///fun 0230ED28 Check if a ptr is null
{
    0230ED28 E59F0014 ldr     r0,=2324DE4h
    0230ED2C E5900008 ldr     r0,[r0,8h]
    0230ED30 E3500000 cmp     r0,0h
    if( r0 != 0 ) 
        0230ED34 13A00001 movne   r0,1h
    else
        0230ED38 03A00000 moveq   r0,0h
    0230ED3C E20000FF and     r0,r0,0FFh
    0230ED40 E12FFF1E bx      r14
    0230ED44 02324DE4 
}    
022EB334 E3500000 cmp     r0,0h
if( r0 == 0 )
    022EB338 0A000004 beq     22EB350h              ///022EB350
022EB33C E28D0008 add     r0,r13,8h
022EB340 E28D1000 add     r1,r13,0h
022EB344 EB008FD0 bl      230F28Ch                  ///fun 0230F28C
{
    0230F28C E92D4038 push    r3-r5,r14
    0230F290 E59F20C0 ldr     r2,=2324DE4h          //r2 = 0x2324DE4
    0230F294 E1A05000 mov     r5,r0                 //r5 = r0
    0230F298 E5920004 ldr     r0,[r2,4h]            //r0 = [r2,4h]
    0230F29C E1A04001 mov     r4,r1                 //r4 = r1
    0230F2A0 E3500000 cmp     r0,0h
    if( r0 == 0 )
        0230F2A4 0A000029 beq     230F350h          ///0230F350
    0230F2A8 E5921008 ldr     r1,[r2,8h]
    0230F2AC E1D120F2 ldrsh   r2,[r1,2h]
    0230F2B0 E3520F43 cmp     r2,10Ch
    if( r2 != 0x10C )
        0230F2B4 1A000009 bne     230F2E0h          ///0230F2E0
    0230F2B8 E3A00902 mov     r0,8000h
    0230F2BC E5850000 str     r0,[r5]
    0230F2C0 E3A00A06 mov     r0,6000h
    0230F2C4 E5850004 str     r0,[r5,4h]
    0230F2C8 E3A00B5E mov     r0,17800h
    0230F2CC E5840000 str     r0,[r4]
    0230F2D0 E3A00A0F mov     r0,0F000h
    0230F2D4 E5840004 str     r0,[r4,4h]
    0230F2D8 E3A00001 mov     r0,1h
    0230F2DC E8BD8038 pop     r3-r5,r15
    ///0230F2E0
    0230F2E0 E3E01000 mvn     r1,0h
    0230F2E4 E1520001 cmp     r2,r1
    0230F2E8 0A000018 beq     230F350h
    0230F2EC E1A01005 mov     r1,r5
    0230F2F0 E1A02004 mov     r2,r4
    0230F2F4 EBFF8067 bl      22EF498h
    0230F2F8 E5950000 ldr     r0,[r5]
    0230F2FC E2800902 add     r0,r0,8000h
    0230F300 E5850000 str     r0,[r5]
    0230F304 E5950004 ldr     r0,[r5,4h]
    0230F308 E2800A06 add     r0,r0,6000h
    0230F30C E5850004 str     r0,[r5,4h]
    0230F310 E5940000 ldr     r0,[r4]
    0230F314 E2400902 sub     r0,r0,8000h
    0230F318 E5840000 str     r0,[r4]
    0230F31C E5940004 ldr     r0,[r4,4h]
    0230F320 E2400A06 sub     r0,r0,6000h
    0230F324 E5840004 str     r0,[r4,4h]
    0230F328 E5951000 ldr     r1,[r5]
    0230F32C E5940000 ldr     r0,[r4]
    0230F330 E1510000 cmp     r1,r0
    0230F334 C5841000 strgt   r1,[r4]
    0230F338 E5951004 ldr     r1,[r5,4h]
    0230F33C E5940004 ldr     r0,[r4,4h]
    0230F340 E1510000 cmp     r1,r0
    0230F344 C5841004 strgt   r1,[r4,4h]
    0230F348 E3A00001 mov     r0,1h
    0230F34C E8BD8038 pop     r3-r5,r15
    ///0230F350
    0230F350 E3A00000 mov     r0,0h
    0230F354 E8BD8038 pop     r3-r5,r15
    0230F358 02324DE4 
}
022EB348 E3500000 cmp     r0,0h
if( r0 != 0 )
    022EB34C 13A05001 movne   r5,1h                 //r5 = 1
///022EB350
022EB350 E3550000 cmp     r5,0h
if( r5 != 0 )
    022EB354 1A000004 bne     22EB36Ch              ///022EB36C
022EB358 E28D0008 add     r0,r13,8h
022EB35C E28D1000 add     r1,r13,0h
022EB360 EB001AF5 bl      22F1F3Ch                  ///022F1F3C
022EB364 E3500000 cmp     r0,0h
if( returnval != 0 )
    022EB368 13A05001 movne   r5,1h                 //r5 = 1
///022EB36C
022EB36C E3550000 cmp     r5,0h
if( r5 == 0 )
    022EB370 0A000011 beq     22EB3BCh              ///022EB3BC
022EB374 E59D1008 ldr     r1,[r13,8h]               //r1 = [r13,8h] (was 0x8000)
022EB378 E5940000 ldr     r0,[r4]                   //r0 = [r4] (was 0xB080)
022EB37C E1500001 cmp     r0,r1
if( r0 < r1 )
    022EB380 B5841000 strlt   r1,[r4]               //[r4] = r1
022EB384 E59D1000 ldr     r1,[r13]                  //r1 = [r13] (was 0xE800)
022EB388 E5940000 ldr     r0,[r4]                   //r0 = [r4] (was 0xB080)
022EB38C E1500001 cmp     r0,r1
if( r0 > r1 )
    022EB390 C5841000 strgt   r1,[r4]               //[r4] = r1
022EB394 E59D100C ldr     r1,[r13,0Ch]              //r1 = [r13,0Ch] (was 0x12000)
022EB398 E5940004 ldr     r0,[r4,4h]                //r0 = [r4,4h] (was 0x11C80)
022EB39C E1500001 cmp     r0,r1
if( r0 < r1 )
    022EB3A0 B5841004 strlt   r1,[r4,4h]            //[r4,4h] = r1
022EB3A4 E59D1004 ldr     r1,[r13,4h]               //r1 = [r13,4h] (was 0x1E000)
022EB3A8 E5940004 ldr     r0,[r4,4h]                //r0 = [r4,4h]    
022EB3AC E1500001 cmp     r0,r1
if( r0 > r1 )
    022EB3B0 C5841004 strgt   r1,[r4,4h]            //[r4,4h] = r1
022EB3B4 E3A00001 mov     r0,1h                     //r0 = 1
022EB3B8 EA000000 b       22EB3C0h                  ///022EB3C0
///022EB3BC
022EB3BC E3A00000 mov     r0,0h                     //r0 = 0
///022EB3C0
022EB3C0 E28DD010 add     r13,r13,10h
022EB3C4 E8BD8038 pop     r3-r5,r15



/*
*/
022EB3F0 E92D4FF0 push    r4-r11,r14
022EB3F4 E24DD05C sub     r13,r13,5Ch
022EB3F8 E59F0440 ldr     r0,=2320BBCh
022EB3FC E59F8440 ldr     r8,=23891F8h
022EB400 E5907010 ldr     r7,[r0,10h]
022EB404 E5906014 ldr     r6,[r0,14h]
022EB408 E5905008 ldr     r5,[r0,8h]
022EB40C E590A00C ldr     r10,[r0,0Ch]
022EB410 E590B000 ldr     r11,[r0]
022EB414 E5900004 ldr     r0,[r0,4h]
022EB418 E3E04001 mvn     r4,1h
022EB41C E58D0004 str     r0,[r13,4h]
022EB420 E58D0028 str     r0,[r13,28h]
022EB424 E2840001 add     r0,r4,1h
022EB428 E58D7034 str     r7,[r13,34h]
022EB42C E58D6038 str     r6,[r13,38h]
022EB430 E58D502C str     r5,[r13,2Ch]
022EB434 E58DA030 str     r10,[r13,30h]
022EB438 E58DB024 str     r11,[r13,24h]
022EB43C E3A09000 mov     r9,0h
022EB440 E58D800C str     r8,[r13,0Ch]
022EB444 E58D8010 str     r8,[r13,10h]
022EB448 E58D8014 str     r8,[r13,14h]
022EB44C E58D0020 str     r0,[r13,20h]
022EB450 E58D001C str     r0,[r13,1Ch]
022EB454 E58D0018 str     r0,[r13,18h]
022EB458 E5980000 ldr     r0,[r8]
022EB45C E3500005 cmp     r0,5h
022EB460 1A000010 bne     22EB4A8h
022EB464 E3590001 cmp     r9,1h
022EB468 1A00000C bne     22EB4A0h
022EB46C E59D000C ldr     r0,[r13,0Ch]
022EB470 E5D00050 ldrb    r0,[r0,50h]
022EB474 E3500000 cmp     r0,0h
022EB478 0A00000A beq     22EB4A8h
022EB47C E59D0010 ldr     r0,[r13,10h]
022EB480 E5901048 ldr     r1,[r0,48h]
022EB484 E59D0014 ldr     r0,[r13,14h]
022EB488 E590004C ldr     r0,[r0,4Ch]
022EB48C E5881048 str     r1,[r8,48h]
022EB490 E588004C str     r0,[r8,4Ch]
022EB494 E3A00001 mov     r0,1h
022EB498 E5C80050 strb    r0,[r8,50h]
022EB49C EA0000E1 b       22EB828h
022EB4A0 E3A00001 mov     r0,1h
022EB4A4 E5880000 str     r0,[r8]
022EB4A8 E5980000 ldr     r0,[r8]
022EB4AC E3500006 cmp     r0,6h
022EB4B0 908FF100 addls   r15,r15,r0,lsl 2h
022EB4B4 EA000029 b       22EB560h
022EB4B8 EA000028 b       22EB560h
022EB4BC EA000027 b       22EB560h
022EB4C0 EA000003 b       22EB4D4h
022EB4C4 EA00000C b       22EB4FCh
022EB4C8 EA000015 b       22EB524h
022EB4CC EA000023 b       22EB560h
022EB4D0 EA00001D b       22EB54Ch
022EB4D4 E5980004 ldr     r0,[r8,4h]
022EB4D8 E2881028 add     r1,r8,28h
022EB4DC E1A00800 mov     r0,r0,lsl 10h
022EB4E0 E1A00840 mov     r0,r0,asr 10h
022EB4E4 EB003702 bl      22F90F4h
022EB4E8 E59D1018 ldr     r1,[r13,18h]
022EB4EC E1500001 cmp     r0,r1
022EB4F0 03A00001 moveq   r0,1h
022EB4F4 05880000 streq   r0,[r8]
022EB4F8 EA000018 b       22EB560h
022EB4FC E5980004 ldr     r0,[r8,4h]
022EB500 E2881028 add     r1,r8,28h
022EB504 E1A00800 mov     r0,r0,lsl 10h
022EB508 E1A00840 mov     r0,r0,asr 10h
022EB50C EB0045EC bl      22FCCC4h
022EB510 E59D101C ldr     r1,[r13,1Ch]
022EB514 E1500001 cmp     r0,r1
022EB518 03A00001 moveq   r0,1h
022EB51C 05880000 streq   r0,[r8]
022EB520 EA00000E b       22EB560h
022EB524 E5980004 ldr     r0,[r8,4h]
022EB528 E2881028 add     r1,r8,28h
022EB52C E1A00800 mov     r0,r0,lsl 10h
022EB530 E1A00840 mov     r0,r0,asr 10h
022EB534 EB004B9D bl      22FE3B0h
022EB538 E59D1020 ldr     r1,[r13,20h]
022EB53C E1500001 cmp     r0,r1
022EB540 03A00001 moveq   r0,1h
022EB544 05880000 streq   r0,[r8]
022EB548 EA000004 b       22EB560h
022EB54C E2880028 add     r0,r8,28h
022EB550 EB008F41 bl      230F25Ch
022EB554 E3500000 cmp     r0,0h
022EB558 03A00001 moveq   r0,1h
022EB55C 05880000 streq   r0,[r8]
022EB560 E5980008 ldr     r0,[r8,8h]
022EB564 E3500004 cmp     r0,4h
022EB568 908FF100 addls   r15,r15,r0,lsl 2h
022EB56C EA000054 b       22EB6C4h
022EB570 EA000053 b       22EB6C4h
022EB574 EA000002 b       22EB584h
022EB578 EA00001D b       22EB5F4h
022EB57C EA000033 b       22EB650h
022EB580 EA00003A b       22EB670h
022EB584 E58D7054 str     r7,[r13,54h]
022EB588 E58D6058 str     r6,[r13,58h]
022EB58C E5982028 ldr     r2,[r8,28h]
022EB590 E5981018 ldr     r1,[r8,18h]
022EB594 E28D0054 add     r0,r13,54h
022EB598 E0421001 sub     r1,r2,r1
022EB59C E58D1054 str     r1,[r13,54h]
022EB5A0 E598202C ldr     r2,[r8,2Ch]
022EB5A4 E598101C ldr     r1,[r8,1Ch]
022EB5A8 E0421001 sub     r1,r2,r1
022EB5AC E58D1058 str     r1,[r13,58h]
022EB5B0 E598100C ldr     r1,[r8,0Ch]
022EB5B4 EBF466D5 bl      2005110h
022EB5B8 E59D1054 ldr     r1,[r13,54h]
022EB5BC E3510000 cmp     r1,0h
022EB5C0 059D0058 ldreq   r0,[r13,58h]
022EB5C4 03500000 cmpeq   r0,0h
022EB5C8 03A00000 moveq   r0,0h
022EB5CC 05880008 streq   r0,[r8,8h]
022EB5D0 0A00003F beq     22EB6D4h
022EB5D4 E5980018 ldr     r0,[r8,18h]
022EB5D8 E0800001 add     r0,r0,r1
022EB5DC E5880018 str     r0,[r8,18h]
022EB5E0 E598101C ldr     r1,[r8,1Ch]
022EB5E4 E59D0058 ldr     r0,[r13,58h]
022EB5E8 E0810000 add     r0,r1,r0
022EB5EC E588001C str     r0,[r8,1Ch]
022EB5F0 EA000037 b       22EB6D4h
022EB5F4 E58D504C str     r5,[r13,4Ch]
022EB5F8 E58DA050 str     r10,[r13,50h]
022EB5FC E5982028 ldr     r2,[r8,28h]
022EB600 E5981018 ldr     r1,[r8,18h]
022EB604 E28D004C add     r0,r13,4Ch
022EB608 E0421001 sub     r1,r2,r1
022EB60C E58D104C str     r1,[r13,4Ch]
022EB610 E598202C ldr     r2,[r8,2Ch]
022EB614 E598101C ldr     r1,[r8,1Ch]
022EB618 E0421001 sub     r1,r2,r1
022EB61C E58D1050 str     r1,[r13,50h]
022EB620 EBF466A2 bl      20050B0h
022EB624 E58D0008 str     r0,[r13,8h]
022EB628 E598000C ldr     r0,[r8,0Ch]
022EB62C EBF6901C bl      208F6A4h
022EB630 E1A01000 mov     r1,r0
022EB634 E59D0008 ldr     r0,[r13,8h]
022EB638 EBF68EFD bl      208F234h
022EB63C EBF6900B bl      208F670h
022EB640 E5880010 str     r0,[r8,10h]
022EB644 E3500000 cmp     r0,0h
022EB648 D3A00001 movle   r0,1h
022EB64C D5880010 strle   r0,[r8,10h]
022EB650 E5981018 ldr     r1,[r8,18h]
022EB654 E598001C ldr     r0,[r8,1Ch]
022EB658 E5881020 str     r1,[r8,20h]
022EB65C E5880024 str     r0,[r8,24h]
022EB660 E3A00000 mov     r0,0h
022EB664 E5880014 str     r0,[r8,14h]
022EB668 E3A00004 mov     r0,4h
022EB66C E5880008 str     r0,[r8,8h]
022EB670 E5981010 ldr     r1,[r8,10h]
022EB674 E28D0044 add     r0,r13,44h
022EB678 E2411001 sub     r1,r1,1h
022EB67C E5881010 str     r1,[r8,10h]
022EB680 E5982014 ldr     r2,[r8,14h]
022EB684 E2881020 add     r1,r8,20h
022EB688 E2822001 add     r2,r2,1h
022EB68C E5882014 str     r2,[r8,14h]
022EB690 E58D2000 str     r2,[r13]
022EB694 E5983010 ldr     r3,[r8,10h]
022EB698 E2882028 add     r2,r8,28h
022EB69C EBF466AA bl      200514Ch
022EB6A0 E59D1044 ldr     r1,[r13,44h]
022EB6A4 E59D0048 ldr     r0,[r13,48h]
022EB6A8 E5881018 str     r1,[r8,18h]
022EB6AC E588001C str     r0,[r8,1Ch]
022EB6B0 E5980010 ldr     r0,[r8,10h]
022EB6B4 E3500000 cmp     r0,0h
022EB6B8 D3A00000 movle   r0,0h
022EB6BC D5880008 strle   r0,[r8,8h]
022EB6C0 EA000003 b       22EB6D4h
022EB6C4 E5981028 ldr     r1,[r8,28h]
022EB6C8 E598002C ldr     r0,[r8,2Ch]
022EB6CC E5881018 str     r1,[r8,18h]
022EB6D0 E588001C str     r0,[r8,1Ch]
022EB6D4 E5980030 ldr     r0,[r8,30h]
022EB6D8 E3500001 cmp     r0,1h
022EB6DC 0A000002 beq     22EB6ECh
022EB6E0 E3500002 cmp     r0,2h
022EB6E4 0A000012 beq     22EB734h
022EB6E8 EA00002B b       22EB79Ch
022EB6EC E598003C ldr     r0,[r8,3Ch]
022EB6F0 E2400001 sub     r0,r0,1h
022EB6F4 E588003C str     r0,[r8,3Ch]
022EB6F8 E3500000 cmp     r0,0h
022EB6FC CA000029 bgt     22EB7A8h
022EB700 E3A00004 mov     r0,4h
022EB704 E588003C str     r0,[r8,3Ch]
022EB708 E1A00004 mov     r0,r4
022EB70C E3A01002 mov     r1,2h
022EB710 EBF45ADD bl      200228Ch
022EB714 E1A00500 mov     r0,r0,lsl 0Ah
022EB718 E5880040 str     r0,[r8,40h]
022EB71C E1A00004 mov     r0,r4
022EB720 E3A01002 mov     r1,2h
022EB724 EBF45AD8 bl      200228Ch
022EB728 E1A00500 mov     r0,r0,lsl 0Ah
022EB72C E5880044 str     r0,[r8,44h]
022EB730 EA00001C b       22EB7A8h
022EB734 E598003C ldr     r0,[r8,3Ch]
022EB738 E2400001 sub     r0,r0,1h
022EB73C E588003C str     r0,[r8,3Ch]
022EB740 E3500000 cmp     r0,0h
022EB744 CA000017 bgt     22EB7A8h
022EB748 E5981038 ldr     r1,[r8,38h]
022EB74C E1A00004 mov     r0,r4
022EB750 E588103C str     r1,[r8,3Ch]
022EB754 E3A01002 mov     r1,2h
022EB758 EBF45ACB bl      200228Ch
022EB75C E5982034 ldr     r2,[r8,34h]
022EB760 E3A01002 mov     r1,2h
022EB764 E0000092 mul     r0,r2,r0
022EB768 E1A00400 mov     r0,r0,lsl 8h
022EB76C E0800FA0 add     r0,r0,r0,lsr 1Fh
022EB770 E1A000C0 mov     r0,r0,asr 1h
022EB774 E5880040 str     r0,[r8,40h]
022EB778 E1A00004 mov     r0,r4
022EB77C EBF45AC2 bl      200228Ch
022EB780 E5981034 ldr     r1,[r8,34h]
022EB784 E0000091 mul     r0,r1,r0
022EB788 E1A00400 mov     r0,r0,lsl 8h
022EB78C E0800FA0 add     r0,r0,r0,lsr 1Fh
022EB790 E1A000C0 mov     r0,r0,asr 1h
022EB794 E5880044 str     r0,[r8,44h]
022EB798 EA000002 b       22EB7A8h
022EB79C E3A00000 mov     r0,0h
022EB7A0 E5880040 str     r0,[r8,40h]
022EB7A4 E5880044 str     r0,[r8,44h]
022EB7A8 E59D0004 ldr     r0,[r13,4h]
022EB7AC E58DB03C str     r11,[r13,3Ch]
022EB7B0 E58D0040 str     r0,[r13,40h]
022EB7B4 E5982018 ldr     r2,[r8,18h]
022EB7B8 E5981040 ldr     r1,[r8,40h]
022EB7BC E1A00009 mov     r0,r9
022EB7C0 E0821001 add     r1,r2,r1
022EB7C4 E2411080 sub     r1,r1,80h
022EB7C8 E58D103C str     r1,[r13,3Ch]
022EB7CC E598301C ldr     r3,[r8,1Ch]
022EB7D0 E5982044 ldr     r2,[r8,44h]
022EB7D4 E28D103C add     r1,r13,3Ch                    
022EB7D8 E0832002 add     r2,r3,r2
022EB7DC E2422D2E sub     r2,r2,0B80h
022EB7E0 E58D2040 str     r2,[r13,40h]
022EB7E4 EBFFFEC2 bl      22EB2F4h                      ///022EB2F4
022EB7E8 E3500000 cmp     r0,0h
022EB7EC 0A00000D beq     22EB828h
022EB7F0 E59D003C ldr     r0,[r13,3Ch]
022EB7F4 E2401902 sub     r1,r0,8000h
022EB7F8 E1A003C1 mov     r0,r1,asr 7h
022EB7FC E0810C20 add     r0,r1,r0,lsr 18h
022EB800 E1A00440 mov     r0,r0,asr 8h
022EB804 E5880048 str     r0,[r8,48h]
022EB808 E59D0040 ldr     r0,[r13,40h]                  //r0 = (BMA_Height << 11)
022EB80C E2401A06 sub     r1,r0,6000h
022EB810 E1A003C1 mov     r0,r1,asr 7h
022EB814 E0810C20 add     r0,r1,r0,lsr 18h
022EB818 E1A00440 mov     r0,r0,asr 8h
022EB81C E588004C str     r0,[r8,4Ch]
022EB820 E3A00001 mov     r0,1h
022EB824 E5C80050 strb    r0,[r8,50h]
022EB828 E2899001 add     r9,r9,1h
022EB82C E3590002 cmp     r9,2h
022EB830 E2888054 add     r8,r8,54h
022EB834 BAFFFF07 blt     22EB458h
022EB838 E28DD05C add     r13,r13,5Ch
022EB83C E8BD8FF0 pop     r4-r11,r15
022EB840 02320BBC eoreqs  r0,r2,2F000h
022EB844 023891F8 eoreqs  r9,r8,3Eh



/*
*/
0200B340 E92D4078 push    r3-r6,r14
0200B344 E24DD004 sub     r13,r13,4h
0200B348 E1A04000 mov     r4,r0
0200B34C E59F0068 ldr     r0,=1FFh
0200B350 E9940006 ldmib   [r4],r1,r2
0200B354 E0011000 and     r1,r1,r0
0200B358 E1A02B82 mov     r2,r2,lsl 17h
0200B35C E594000C ldr     r0,[r4,0Ch]
0200B360 E18113A2 orr     r1,r1,r2,lsr 7h
0200B364 E5801000 str     r1,[r0]
0200B368 E5D40001 ldrb    r0,[r4,1h]
0200B36C E3500000 cmp     r0,0h
if( r0 == 0 )
    0200B370 0A00000F beq     200B3B4h          ///0200B3B4
0200B374 E5945018 ldr     r5,[r4,18h]
0200B378 E5941014 ldr     r1,[r4,14h]
0200B37C E1A00005 mov     r0,r5
0200B380 E5946010 ldr     r6,[r4,10h]
0200B384 EB01BBD4 bl      207A2DCh
0200B388 E594C014 ldr     r12,[r4,14h]
0200B38C E1A02006 mov     r2,r6
0200B390 E1A03005 mov     r3,r5
0200B394 E3A00003 mov     r0,3h
0200B398 E3A01001 mov     r1,1h
0200B39C E58DC000 str     r12,[r13]
0200B3A0 EBFFEA9A bl      2005E10h
0200B3A4 E3A00003 mov     r0,3h
0200B3A8 EBFFEA60 bl      2005D30h
0200B3AC E3A00000 mov     r0,0h
0200B3B0 E5C40001 strb    r0,[r4,1h]
///0200B3B4
0200B3B4 E28DD004 add     r13,r13,4h
0200B3B8 E8BD8078 pop     r3-r6,r15
0200B3BC 000001FF 