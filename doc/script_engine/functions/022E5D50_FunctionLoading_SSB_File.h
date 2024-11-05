//Breakpoints:
0x022E5D94 : Break on read SSB header.

/*
    fun_02008210 2008210h 02008210
*/
02008210 E92D4070 push    r4-r6,r14
02008214 E1A06000 mov     r6,r0
02008218 E1A05001 mov     r5,r1
0200821C E3A04001 mov     r4,1h
02008220 E1A00006 mov     r0,r6
02008224 E1A01005 mov     r1,r5
02008228 EB01DD25 bl      207F6C4h
0200822C E3500000 cmp     r0,0h
02008230 18BD8070 popne   r4-r6,r15
02008234 E1A00004 mov     r0,r4
02008238 EBFFEEBB bl      2003D2Ch
0200823C EAFFFFF7 b       2008220h
02008240 E8BD8070 pop     r4-r6,r15


//------------------------------
// Function 022E5D50
// Params 
//      - R0: Pointer to Struct? (Possibly fstream object instance)
//      - R1: Pointer to string path to script file.(ex: SCRIPT/V01P07B/m01d0112.ssb)
//      - R3: Pointer to script name (ex: m01d0112)
//------------------------------
022E5D50 E92D4030 push    r4,r5,r14
022E5D54 E24DD054 sub     r13,r13,54h
022E5D58 E1A05001 mov     r5,r1         //R5 = ScriptFilePath
022E5D5C E1A04000 mov     r4,r0         //R4 = 
022E5D60 E59F118C ldr     r1,=2319908h  //"script load  file name [%s]"
022E5D64 E1A02005 mov     r2,r5         //R2 = ScriptFilePath
022E5D68 E3A00002 mov     r0,2h
022E5D6C EBF49933 bl      200C240h      ///DebugPrint( 2, "script load  file name [%s]", )
022E5D70 EBF488FC bl      2008168h      ///EXEC 02008168
022E5D74 E28D000C add     r0,r13,0Ch    // (0x1E)
022E5D78 EBF48921 bl      2008204h      ///EXEC 02008204 //This loads the address of the function at 0207F3E4 into R12, and bx to there.
022E5D7C E28D000C add     r0,r13,0Ch
022E5D80 E1A01005 mov     r1,r5         //Put pointer to ScriptPath into R1
022E5D84 EBF48921 bl      2008210h      ///EXEC 02008210 fun_02008210
022E5D88 E28D000C add     r0,r13,0Ch
022E5D8C E28D1000 add     r1,r13,0h
022E5D90 E3A0200C mov     r2,0Ch
022E5D94 EBF4892E bl      2008254h      ///EXEC 02008254 //This loads the SSB file's header. 
//-- SSB Header is read here
022E5D98 E1DD30B4 ldrh    r3,[r13,4h]   //R3 = SSBStrContTableOffset
022E5D9C E1DD20B6 ldrh    r2,[r13,6h]   //R2 = SSBConstTblSize.
022E5DA0 E59F0150 ldr     r0,=2324F74h      //StructWithScriptData
022E5DA4 E28D1000 add     r1,r13,0h     //R1 = PtrSSBHeaderBuff
022E5DA8 E0833002 add     r3,r3,r2      //R3 = SSBStrContTableOffset + SSBConstTblSize (OffsetStringTable?)
022E5DAC E1D020D1 ldrsb   r2,[r0,1h]    //R2 = MysteryHeaderOffsetInt16 (0) (The same position is written to on line 022E4A5C )
022E5DB0 E5940000 ldr     r0,[r4]       //R0 = SSBDestBufferPtr (Read from 0x2324C84. A pointer to where the script's content will be written, 0x2120380. (SSBDestBufferPtr))
022E5DB4 E3500000 cmp     r0,0h         //Check if SSBDestBufferPtr is null (result handled at line 022E5DCC)
022E5DB8 E2820004 add     r0,r2,4h      //R0 = MysteryHeaderOffsetInt16 + 4
022E5DBC E1A00080 mov     r0,r0,lsl 1h  //R0 = (MysteryHeaderOffsetInt16 + 4) * 2
022E5DC0 E19100B0 ldrh    r0,[r1,r0]    //R0 = SizeStringTable
022E5DC4 E0830000 add     r0,r3,r0      //R0 = (SSBStrContTableOffset + SSBConstTblSize) + SizeStringTable
022E5DC8 E1A00080 mov     r0,r0,lsl 1h  //R0 = ((SSBStrContTableOffset + SSBConstTblSize) + SizeStringTable) * 2
if( SSBDestBufferPtr != null )
    022E5DCC 1A000003 bne     22E5DE0h  ///GOTO LBL1
022E5DD0 E3A01001 mov     r1,1h         
022E5DD4 E5840004 str     r0,[r4,4h]
022E5DD8 EBF46CE4 bl      2001170h      ///EXEC 02001170
022E5DDC E5840000 str     r0,[r4]
///LBL1
022E5DE0 E1DD20B4 ldrh    r2,[r13,4h]   //R2 = SSBStrContTableOffset
022E5DE4 E5941000 ldr     r1,[r4]       //R1 = SSBDestBufferPtr
022E5DE8 E28D000C add     r0,r13,0Ch    //R0 = PtrSSBHeaderBuff + 12 (Points past the header)
022E5DEC E1A02082 mov     r2,r2,lsl 1h  //R2 = SSBStrContTableOffset * 2
022E5DF0 EBF48917 bl      2008254h      ///EXEC 02008254 (R0 = (PtrSSBHeaderBuff + 12), R1 = SSBDestBufferPtr, R2 = (SSBStrContTableOffset *2), R3 = OffsetStringTable  )
022E5DF4 E1DD00B0 ldrh    r0,[r13]      //R0 = SSBNbConsts
022E5DF8 E3500000 cmp     r0,0h
if( SSBNbConsts == 0 )
    022E5DFC 0A000008 beq     22E5E24h  ///GOTO LBL2
022E5E00 E1DD10B4 ldrh    r1,[r13,4h]   //R1 = SSBStrContTableOffset
022E5E04 E1DD00B2 ldrh    r0,[r13,2h]   //R0 = SSBNbStrings
022E5E08 E1DD20B6 ldrh    r2,[r13,6h]   //R2 = SSBConstTblSize
022E5E0C E5943000 ldr     r3,[r4]       //R3 = SSBDestBufferPtr
022E5E10 E0811000 add     r1,r1,r0      //R1 = SSBStrContTableOffset + SSBNbStrings
022E5E14 E28D000C add     r0,r13,0Ch    //R0 = (PtrSSBHeaderBuff + 12)
022E5E18 E0831081 add     r1,r3,r1,lsl 1h //R1 =  SSBDestBufferPtr + ( (SSBStrContTableOffset + SSBNbStrings) * 2)
022E5E1C E1A02082 mov     r2,r2,lsl 1h  //R2 = SSBConstTblSize * 2
022E5E20 EBF4890B bl      2008254h      ///EXEC 02008254 (R0 = (PtrSSBHeaderBuff + 12), R1 = SSBDestBufferPtr + ( (SSBStrContTableOffset + SSBNbStrings) * 2), R2 = (SSBConstTblSize * 2), R3 = SSBDestBufferPtr  )
///LBL2
022E5E24 E1DD00B2 ldrh    r0,[r13,2h]   //R0 = SSBNbStrings
022E5E28 E3500000 cmp     r0,0h         
if( SSBNbStrings == 0 )
    022E5E2C 0A00002A beq     22E5EDCh  ///GOTO LBL3 022E5EDC
022E5E30 E1DD20B4 ldrh    r2,[r13,4h]   //R2 = SSBStrContTableOffset
022E5E34 E1DD10B6 ldrh    r1,[r13,6h]   //R1 = SSBConstTblSize
022E5E38 E59F00B8 ldr     r0,=2324F74h      //StructWithScriptData
022E5E3C E3A03000 mov     r3,0h         //R3 = 0
022E5E40 E0821001 add     r1,r2,r1      //R1 = SSBConstTblSize + SSBStrContTableOffset
022E5E44 E2812006 add     r2,r1,6h      //R2 = (SSBConstTblSize + SSBStrContTableOffset) + 6 
022E5E48 E1D0C0D1 ldrsb   r12,[r0,1h]   //R12 = **** MysteryHeaderOffsetInt16 **** (0) (The same position is written to on line 022E4A5C )
022E5E4C E28D1000 add     r1,r13,0h     //R1 = PtrSSBHeaderBuff + 0
022E5E50 EA000003 b       22E5E64h      ///GOTO LBL4 022E5E64
///LBL5
022E5E54 E0810083 add     r0,r1,r3,lsl 1h
022E5E58 E1D000B8 ldrh    r0,[r0,8h]
022E5E5C E2833001 add     r3,r3,1h
022E5E60 E0822000 add     r2,r2,r0
///LBL4
022E5E64 E153000C cmp     r3,r12
if( R3 < MysteryHeaderOffsetInt16 )
    022E5E68 BAFFFFF9 blt     22E5E54h      ///GOTO LBL5 022E5E54
022E5E6C E1A01082 mov     r1,r2,lsl 1h  //R1 = ((SSBConstTblSize + SSBStrContTableOffset) + 6 ) * 2
022E5E70 E28D000C add     r0,r13,0Ch    //R0 = PtrSSBHeaderBuff + 12
022E5E74 E3A02000 mov     r2,0h         //R2 = 0
022E5E78 EBF4890A bl      20082A8h      ///EXEC 020082A8 (Wrap call to SSBFileConstStrTblROMOffset)
022E5E7C E1DD20B2 ldrh    r2,[r13,2h]   //R2 = SSBNbStrings
022E5E80 E1DD10B4 ldrh    r1,[r13,4h]   //R1 = SSBStrContTableOffset
022E5E84 E5943000 ldr     r3,[r4]       //R3 = SSBDestBufferPtr
022E5E88 E28D000C add     r0,r13,0Ch    //R0 = PtrSSBHeaderBuff + 12
022E5E8C E0831081 add     r1,r3,r1,lsl 1h //R1 = SSBDestBufferPtr + (SSBStrContTableOffset * 2)
022E5E90 E1A02082 mov     r2,r2,lsl 1h  //R2 = SSBNbStrings * 2
022E5E94 EBF488EE bl      2008254h      ///EXEC 02008254 
022E5E98 E59F0058 ldr     r0,=2324F74h      //StructWithScriptData
022E5E9C E28D1000 add     r1,r13,0h     //R1 = PtrSSBHeaderBuff + 0
022E5EA0 E1D000D1 ldrsb   r0,[r0,1h]    //R0 = MysteryHeaderOffsetInt16 (0) (The same position is written to on line 022E4A5C )
022E5EA4 E1DDC0B2 ldrh    r12,[r13,2h]  //R12 = SSBNbStrings
022E5EA8 E1DD20B4 ldrh    r2,[r13,4h]   //R2 = SSBStrContTableOffset
022E5EAC E2800004 add     r0,r0,4h      //R0 = MysteryHeaderOffsetInt16 + 4
022E5EB0 E1A00080 mov     r0,r0,lsl 1h  //R0 = ( MysteryHeaderOffsetInt16 + 4 ) * 2
022E5EB4 E19100B0 ldrh    r0,[r1,r0]    //R0 = (StrBlockSize?)
022E5EB8 E08C1002 add     r1,r12,r2     //R1 = SSBNbStrings + SSBStrContTableOffset
022E5EBC E1DD30B6 ldrh    r3,[r13,6h]   //R3 = SSBConstTblSize
022E5EC0 E040200C sub     r2,r0,r12     //R2 = R0 - SSBNbStrings
022E5EC4 E5944000 ldr     r4,[r4]       //R4 = SSBDestBufferPtr
022E5EC8 E0831001 add     r1,r3,r1      //R1 = SSBConstTblSize + (SSBNbStrings + SSBStrContTableOffset)
022E5ECC E28D000C add     r0,r13,0Ch    //R0 = PtrSSBHeaderBuff + 12
022E5ED0 E0841081 add     r1,r4,r1,lsl 1h //R1 = SSBDestBufferPtr + ( ( SSBConstTblSize + (SSBNbStrings + SSBStrContTableOffset) ) * 2)
022E5ED4 E1A02082 mov     r2,r2,lsl 1h  //R2 = R2 * 2
022E5ED8 EBF488DD bl      2008254h      ///EXEC 02008254 
///LBL3
022E5EDC E28D000C add     r0,r13,0Ch    //R0 = PtrSSBHeaderBuff + 12
022E5EE0 EBF488F7 bl      20082C4h      ///EXEC 020082C4
022E5EE4 EBF488AA bl      2008194h      ///EXEC 02008194
022E5EE8 E3A00001 mov     r0,1h         //R0 = 1
022E5EEC E28DD054 add     r13,r13,54h   //R13 = PtrSSBHeaderBuff + 0x54
022E5EF0 E8BD8030 pop     r4,r5,r15     //


//------------------------------
// Function 02008254 CopySSBContent (R0 = (PtrSSBHeaderBuff + 12), R1 = SSBDestBufferPtr, R2 = (SSBStrContTableOffset *2), R3 = OffsetStringTable  )
//------------------------------
/*
    R0 = Pointer to a struct containing the position in the ROM of the various parts of the SSB file.
    R1 = A position to write to in the destination buffer.
    R2 = An offset ?
*/
02008254 E92D41F0 push    r4-r8,r14
02008258 E1A08000 mov     r8,r0     //R8 = (PtrSSBHeaderBuff + 12)
0200825C E1A07001 mov     r7,r1     //R7 = SSBDestBufferPtr
02008260 E1A06002 mov     r6,r2     //R6 = (SSBStrContTableOffset *2)
02008264 E3A05000 mov     r5,0h     //R5 = 0
02008268 E3A04001 mov     r4,1h     //R4 = 1
0200826C EA000009 b       2008298h  ///GOTO LBL1
///LBL2
02008270 E1A00008 mov     r0,r8     //R0 = (PtrSSBHeaderBuff + 12)
02008274 E1A01007 mov     r1,r7     //R1 = DestBuffWritePos
02008278 E0462005 sub     r2,r6,r5  //R2 = (SSBStrContTableOffset *2) - BytesWritten
0200827C EB01DD65 bl      207F818h  ///EXEC 0207F818 ()
02008280 E3500000 cmp     r0,0h
if( R0 >= 0 )   //Signed
{
    02008284 A0855000 addge   r5,r5,r0  //R5 = BytesWritten + R0
    02008288 A0877000 addge   r7,r7,r0  //R7 = DestBuffWritePos + R0
    0200828C AA000001 bge     2008298h  ///GOTO LBL1
}
02008290 E1A00004 mov     r0,r4
02008294 EBFFEEA4 bl      2003D2Ch  ///EXEC 02003D2C
///LBL1
02008298 E1560005 cmp     r6,r5     //(SSBStrContTableOffset *2) > 0 (signed)
if( (SSBStrContTableOffset *2) > 0 )
    0200829C CAFFFFF3 bgt     2008270h  ///GOTO LBL2
020082A0 E1A00005 mov     r0,r5     //Return 0 !
020082A4 E8BD81F0 pop     r4-r8,r15

//------------------------------
// Function 0207F818 ( R0 = (PtrSSBHeaderBuff + 12), R1 = SSBDestBufferPtr, R2 = BytesLeftToCopy?, R3 = OffsetStringTable  )
//------------------------------
0207F818 E59FC004 ldr     r12,=207F550h
0207F81C E3A03000 mov     r3,0h
0207F820 E12FFF1C bx      r12           ///EXEC 0207F550( R0 = (PtrSSBHeaderBuff + 12), R1 = SSBDestBufferPtr, R2 = BytesLeftToCopy?, R3 = 0 )

//------------------------------
// Function 0207F550 ( R0 = (PtrSSBHeaderBuff + 12), R1 = SSBDestBufferPtr, R2 = BytesLeftToCopy?, R3 = 0 )
//------------------------------
0207F550 E92D40F8 push    r3-r7,r14
0207F554 E1A07000 mov     r7,r0         //R7 = (PtrSSBHeaderBuff + 12)
0207F558 E597402C ldr     r4,[r7,2Ch]   //R4 = 0x3BC0A0C (ROM offset of the data right after the current SSB's 12 bytes header! )
0207F55C E5970028 ldr     r0,[r7,28h]   //R0 = 0x3BC0BA6 (ROM offset of the end of the current SSB file)
0207F560 E1A06002 mov     r6,r2         //R6 = BytesLeftToCopy?
0207F564 E5871030 str     r1,[r7,30h]   //Store SSBDestBufferPtr at R7 + 0x30
0207F568 E0400004 sub     r0,r0,r4      //R0 = SizeSSBData
0207F56C E1560000 cmp     r6,r0         //
if( BytesLeftToCopy > SizeSSBData )     //Clamp to actual SSB size
    0207F570 C1A06000 movgt   r6,r0     //R6 = SizeSSBData
0207F574 E3560000 cmp     r6,0h         
if( BytesLeftToCopy < 0 )               //(Signed) Clamp to 0 any negative amount of bytes left to copy
    0207F578 B3A06000 movlt   r6,0h     //R6 = 0
0207F57C E5872034 str     r2,[r7,34h]   //Store BytesLeftToCopy at R7 + 0x34
0207F580 E1A05003 mov     r5,r3         //R5 = 0
0207F584 E5876038 str     r6,[r7,38h]   //Store BytesLeftToCopy at R7 + 0x38
0207F588 E3550000 cmp     r5,0h
if( R5 == 0 )
    0207F58C 0597000C ldreq   r0,[r7,0Ch]//R0 = (Was 0x10)
0207F590 E3A01000 mov     r1,0h
0207F594 03800004 orreq   r0,r0,4h
0207F598 0587000C streq   r0,[r7,0Ch]
0207F59C E1A00007 mov     r0,r7
0207F5A0 EBFFFD8F bl      207EBE4h
0207F5A4 E3550000 cmp     r5,0h
0207F5A8 1A000005 bne     207F5C4h
0207F5AC E1A00007 mov     r0,r7
0207F5B0 EB000064 bl      207F748h
0207F5B4 E3500000 cmp     r0,0h
0207F5B8 1597002C ldrne   r0,[r7,2Ch]
0207F5BC 10406004 subne   r6,r0,r4
0207F5C0 03E06000 mvneq   r6,0h
0207F5C4 E1A00006 mov     r0,r6
0207F5C8 E8BD80F8 pop     r3-r7,r15



//------------------------------
// Function 02003D2C
//------------------------------
02003D2C E92D4010 push    r4,r14
02003D30 E59F1030 ldr     r1,=20AEF7Ch
02003D34 E1A04000 mov     r4,r0
02003D38 E5D1000D ldrb    r0,[r1,0Dh]
02003D3C E3500000 cmp     r0,0h
02003D40 08BD8010 popeq   r4,r15
02003D44 E59F0020 ldr     r0,=2092508h
02003D48 E1A01004 mov     r1,r4
02003D4C EB00212A bl      200C1FCh
02003D50 E3540000 cmp     r4,0h
02003D54 0A000000 beq     2003D5Ch
02003D58 EB00111F bl      20081DCh
02003D5C E3A00000 mov     r0,0h
02003D60 EBFFFA24 bl      20025F8h
02003D64 E8BD8010 pop     r4,r15

//------------------------------
// Function 020082A8 (R0 = (PtrSSBHeaderBuff + 12), R1 = (((SSBConstTblSize + SSBStrContTableOffset) + 6 ) * 2), R2 = 0, R3 = 0 )
//------------------------------
020082A8 E92D4008 push    r3,r14
020082AC EB01DD5D bl      207F828h  ///EXEC 0207F828 Returns success
020082B0 E3500000 cmp     r0,0h
if( Success != 0 )
    020082B4 18BD8008 popne   r3,r15 //Return
020082B8 E3A00001 mov     r0,1h
020082BC EBFFEE9A bl      2003D2Ch  ///EXEC 02003D2C
020082C0 E8BD8008 pop     r3,r15

//------------------------------
// Function 0207F828 UpdateStrTableRomOffset (Seekg function to move read head!) (R0 = (PtrSSBHeaderBuff + 12), R1 = (((SSBConstTblSize + SSBStrContTableOffset) + 6 ) * 2), R2 = 0 )
//------------------------------
/*
    R0 = Pointer to a struct containing the position in the ROM of the various parts of the SSB file. (PtrStreamIter instance!)
    R1 = Offset from the beginning of the SSB file to the string table.
    R2 = (0 = Add offset to beginning of file, 1 = Add offset to StrTable offset, 2 = Add offset to end of file)
    Returns boolean success.
*/
0207F828 E3520000 cmp     r2,0h
if( R2 == 0 )
    0207F82C 0A000004 beq     207F844h  ///GOTO LBL1 0207F844
0207F830 E3520001 cmp     r2,1h
if( R2 == 1 )
    0207F834 0A000005 beq     207F850h  ///GOTO LBL2 0207F850
0207F838 E3520002 cmp     r2,2h
if( R2 == 2 )
    0207F83C 0A000006 beq     207F85Ch  ///GOTO LBL3 0207F85C
0207F840 EA000008 b       207F868h      ///GOTO LBL5 0207F868
///LBL1
0207F844 E5902024 ldr     r2,[r0,24h]   //R2 = 0x3BC0A00 (SSBFileROMOffset)
0207F848 E0811002 add     r1,r1,r2      //R1 = R1 + SSBFileROMOffset
0207F84C EA000007 b       207F870h      ///GOTO LBL4 0207F870
///LBL2
0207F850 E590202C ldr     r2,[r0,2Ch]   //R2 = 0x3BC0AEA (SSBFileConstStrTblROMOffset)
0207F854 E0811002 add     r1,r1,r2      //R1 = R1 + SSBFileConstStrTblROMOffset
0207F858 EA000004 b       207F870h      ///GOTO LBL4 0207F870
///LBL3
0207F85C E5902028 ldr     r2,[r0,28h]   //R2 = 0x3BC0BA6 (SSBFileEndROMOffset)
0207F860 E0811002 add     r1,r1,r2      //R1 = R1 + SSBFileEndROMOffset
0207F864 EA000001 b       207F870h      ///GOTO LBL4 0207F870
///LBL5
0207F868 E3A00000 mov     r0,0h         //Return 0
0207F86C E12FFF1E bx      r14
///LBL4
0207F870 E5902024 ldr     r2,[r0,24h]   //R2 = SSBFileROMOffset
0207F874 E1510002 cmp     r1,r2         
if( R1 < SSBFileROMOffset )
    0207F878 B1A01002 movlt   r1,r2     //R1 = SSBFileROMOffset
0207F87C E5902028 ldr     r2,[r0,28h]   //R2 = SSBFileEndROMOffset
0207F880 E1510002 cmp     r1,r2
if( R1 > SSBFileEndROMOffset )
    0207F884 C1A01002 movgt   r1,r2     //R1 = SSBFileEndROMOffset
0207F888 E580102C str     r1,[r0,2Ch]   //Set SSBFileConstStrTblROMOffset to R1
0207F88C E3A00001 mov     r0,1h         //Return 1
0207F890 E12FFF1E bx      r14



//------------------------------
//  Function 0200C240
//------------------------------
0200C240 E92D000F push    r0-r3         //Push pointer to script name(R3), ptr to script path(R2), and ptr to "script load" string(R1) on the stack!
0200C244 E28DD010 add     r13,r13,10h   //Set stack pointer past the pointers we pushed above
0200C248 E12FFF1E bx      r14           //Return



//------------------------------
//  Function 02008168
//------------------------------
02008168 E92D4008 push    r3,r14            //Overwrite what was on the stack that we just placed there.
0200816C E59F0018 ldr     r0,=20AF368h      //Address to static struct?
02008170 EBFFEACF bl      2002CB4h          ///EXEC 02002CB4
02008174 EBFFE901 bl      2002580h          ///EXEC 02002580
02008178 E3A01013 mov     r1,13h
0200817C EBFFE99D bl      20027F8h          ///EXEC 020027F8
02008180 E59F1008 ldr     r1,=20AF360h
02008184 E5810004 str     r0,[r1,4h]
02008188 E8BD8008 pop     r3,r15


//------------------------------
// Function 02002CB4
//------------------------------
02002CB4 E92D4070 push    r4-r6,r14
02002CB8 E59F1050 ldr     r1,=22B966Ch
02002CBC E1A06000 mov     r6,r0         //
02002CC0 E5915004 ldr     r5,[r1,4h]
02002CC4 E1A00005 mov     r0,r5
02002CC8 EB01DB8F bl      2079B0Ch
02002CCC E5961020 ldr     r1,[r6,20h]
02002CD0 E1A04000 mov     r4,r0
02002CD4 E3510000 cmp     r1,0h
02002CD8 0A000005 beq     2002CF4h
02002CDC E1510004 cmp     r1,r4
02002CE0 23A04000 movcs   r4,0h
02002CE4 2A000003 bcs     2002CF8h
02002CE8 E1A00005 mov     r0,r5
02002CEC EB01DB5C bl      2079A64h
02002CF0 EA000000 b       2002CF8h
02002CF4 E3A04000 mov     r4,0h
02002CF8 E1A00006 mov     r0,r6
02002CFC EBFFFF00 bl      2002904h
02002D00 E586401C str     r4,[r6,1Ch]
02002D04 EBFFFE1D bl      2002580h
02002D08 E5860018 str     r0,[r6,18h]
02002D0C E8BD8070 pop     r4-r6,r15


//------------------------------
//  Function 02002580
//------------------------------
02002580 E92D4038 push    r3-r5,r14
02002584 E59F105C ldr     r1,=22B966Ch
02002588 E59F005C ldr     r0,=229AFCCh
0200258C E5914004 ldr     r4,[r1,4h]
02002590 EB01DEAC bl      207A048h
02002594 E59F0054 ldr     r0,=229AFC0h
02002598 E3A01000 mov     r1,0h
0200259C E5903000 ldr     r3,[r0]
020025A0 E59F204C ldr     r2,=229AFE4h
020025A4 EA000008 b       20025CCh
020025A8 E5925000 ldr     r5,[r2]
020025AC E5950000 ldr     r0,[r5]
020025B0 E1500004 cmp     r0,r4
020025B4 1A000002 bne     20025C4h
020025B8 E59F002C ldr     r0,=229AFCCh
020025BC EB01DEC2 bl      207A0CCh
020025C0 EA000006 b       20025E0h
020025C4 E2811001 add     r1,r1,1h
020025C8 E2822004 add     r2,r2,4h
020025CC E1510003 cmp     r1,r3
020025D0 BAFFFFF4 blt     20025A8h
020025D4 E59F0010 ldr     r0,=229AFCCh
020025D8 EB01DEBB bl      207A0CCh
020025DC E3A05000 mov     r5,0h
020025E0 E1A00005 mov     r0,r5
020025E4 E8BD8038 pop     r3-r5,r15



//------------------------------
//  Function 020027F8
//------------------------------
020027F8 E92D4070 push    r4-r6,r14
020027FC E1A06000 mov     r6,r0
02002800 E5960000 ldr     r0,[r6]
02002804 E1A05001 mov     r5,r1
02002808 EB01DCBF bl      2079B0Ch
0200280C E1A04000 mov     r4,r0
02002810 E5960000 ldr     r0,[r6]
02002814 E1A01005 mov     r1,r5
02002818 EB01DC91 bl      2079A64h
0200281C E1A00004 mov     r0,r4
02002820 E8BD8070 pop     r4-r6,r15

//------------------------------
// Function 0207F3E4 - ScriptStructConstructor
//  - R0 : Address of script struct
//------------------------------
0207F3E4 E3A02000 mov     r2,0h
0207F3E8 E5802000 str     r2,[r0]
0207F3EC E5802004 str     r2,[r0,4h]
0207F3F0 E580201C str     r2,[r0,1Ch]
0207F3F4 E5802018 str     r2,[r0,18h]
0207F3F8 E5802008 str     r2,[r0,8h]
0207F3FC E3A0100E mov     r1,0Eh
0207F400 E5801010 str     r1,[r0,10h]
0207F404 E580200C str     r2,[r0,0Ch]
0207F408 E12FFF1E bx      r14

//------------------------------
// Function 02008210
//      - R0 : Address of ScriptStruct
//      - R1 : Address of ScriptPath
//------------------------------
02008210 E92D4070 push    r4-r6,r14
02008214 E1A06000 mov     r6,r0
02008218 E1A05001 mov     r5,r1
0200821C E3A04001 mov     r4,1h
02008220 E1A00006 mov     r0,r6
02008224 E1A01005 mov     r1,r5
02008228 EB01DD25 bl      207F6C4h  ///EXEC 0207F6C4
0200822C E3500000 cmp     r0,0h
02008230 18BD8070 popne   r4-r6,r15
02008234 E1A00004 mov     r0,r4
02008238 EBFFEEBB bl      2003D2Ch
0200823C EAFFFFF7 b       2008220h
02008240 E8BD8070 pop     r4-r6,r15



//------------------------------------------------------------
//  Annex
//------------------------------------------------------------

//
//
//
022E4A4C E92D4008 push    r3,r14
022E4A50 EBF4EED6 bl      20205B0h      ///EXEC 020205B0
022E4A54 E59F109C ldr     r1,=2324F74h      //StructWithScriptData
022E4A58 E59F209C ldr     r2,=2324C6Ch      //Pointer to current SSA file buffer.
022E4A5C E5C10001 strb    r0,[r1,1h]    //Write at  0x2324F75 the value returned by Function 020205B0
022E4A60 E3A00A01 mov     r0,1000h
022E4A64 E3A01006 mov     r1,6h
022E4A68 E582002C str     r0,[r2,2Ch]
022E4A6C EBF471BF bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022E4A70 E59F2084 ldr     r2,=2324C6Ch      //Pointer to current SSA file buffer.
022E4A74 E3A01006 mov     r1,6h
022E4A78 E5820028 str     r0,[r2,28h]
022E4A7C E3A00A01 mov     r0,1000h
022E4A80 E5820024 str     r0,[r2,24h]
022E4A84 EBF471B9 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022E4A88 E59F206C ldr     r2,=2324C6Ch      //Pointer to current SSA file buffer.
022E4A8C E3A01006 mov     r1,6h
022E4A90 E5820020 str     r0,[r2,20h]
022E4A94 E3A00A01 mov     r0,1000h
022E4A98 E5820004 str     r0,[r2,4h]
022E4A9C EBF471B3 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022E4AA0 E59F2054 ldr     r2,=2324C6Ch      //Pointer to current SSA file buffer.
022E4AA4 E3A01006 mov     r1,6h
022E4AA8 E5820000 str     r0,[r2]
022E4AAC E3A00A02 mov     r0,2000h
022E4AB0 E582000C str     r0,[r2,0Ch]
022E4AB4 EBF471AD bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022E4AB8 E59F203C ldr     r2,=2324C6Ch      //Pointer to current SSA file buffer.
022E4ABC E3A01006 mov     r1,6h
022E4AC0 E5820008 str     r0,[r2,8h]
022E4AC4 E3A00A02 mov     r0,2000h
022E4AC8 E5820014 str     r0,[r2,14h]
022E4ACC EBF471A7 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022E4AD0 E59F2024 ldr     r2,=2324C6Ch      //Pointer to current SSA file buffer.
022E4AD4 E3A01006 mov     r1,6h
022E4AD8 E5820010 str     r0,[r2,10h]
022E4ADC E3A00902 mov     r0,8000h
022E4AE0 E582001C str     r0,[r2,1Ch]
022E4AE4 EBF471A1 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022E4AE8 E59F100C ldr     r1,=2324C6Ch      //Pointer to current SSA file buffer.
022E4AEC E5810018 str     r0,[r1,18h]
022E4AF0 EB000024 bl      22E4B88h          ///022E4B88
{///022E4B88
    022E4B88 E92D4008 push    r3,r14
    022E4B8C E59F1034 ldr     r1,=2324F74h      //StructWithScriptData
    022E4B90 E3E02000 mvn     r2,0h
    022E4B94 E1C120B6 strh    r2,[r1,6h]
    022E4B98 E1C120BA strh    r2,[r1,0Ah]
    022E4B9C E1C120B8 strh    r2,[r1,8h]
    022E4BA0 E1C120B2 strh    r2,[r1,2h]
    022E4BA4 E1C120B4 strh    r2,[r1,4h]
    022E4BA8 E3A00000 mov     r0,0h
    022E4BAC E5C10000 strb    r0,[r1]
    022E4BB0 E59F0014 ldr     r0,=2324F82h
    022E4BB4 E1C120BC strh    r2,[r1,0Ch]
    022E4BB8 EBFFFE8D bl      22E45F4h
    022E4BBC E59F000C ldr     r0,=2324F8Ah
    022E4BC0 EBFFFE8B bl      22E45F4h
    022E4BC4 E8BD8008 pop     r3,r15
    022E4BC8 02324F74 
    022E4BCC 02324F82 
    022E4BD0 02324F8A 
}
022E4AF4 E8BD8008 pop     r3,r15

//
//  Function 020205B0
//
020205B0 E59F0004 ldr     r0,=20AFCE8h
020205B4 E1D000D0 ldrsb   r0,[r0]
020205B8 E12FFF1E bx      r14

