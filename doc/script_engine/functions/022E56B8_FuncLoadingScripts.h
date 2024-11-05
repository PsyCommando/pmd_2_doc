//Breakpoints
[0212C380..0212C38F]!!?
[02324C6C]!!?

//Static Addresses :
02324F74 - Script Strings table.
02319888 - "ScriptStation load acting %d %s %d"
0231984C - "ScriptStation load common %d"
023198F0 - "ScriptStation_RemoveAll"
020A5490 - Lookup table, ScriptID to Resource String ( ex: 0x120 to S13P02A ) (Entries are 12 bytes long)

023196D8 - "SCRIPT/%s/enter.sse"
0209DFA0 - "SCENARIO CALC [%3d] %4d %4 -> %4d %4d"

2324C6Ch - Pointer to current SSA file buffer.
2324C8Ch - Pointer to current SSS file buffer.
2324C94h - Pointer to current SSE file buffer.

2324CF4h - Static struct that contains script engine runtime data. 
2324F8Ah - Static string containing the current script's name


//Function of interest:
200C240h - This is the dummied out debug printf function!
204C618h - This function seems to determine the next scenario to load??? It refers to the string "SCENARIO CALC [%3d] %4d %4 -> %4d %4d"
2064FFCh - GetScriptIDName looks up an indext to a script id in the 020A5490 table, and return the string ptr in R0
2089584h - Replace tokens in a file path. Works a bit like sprintf, but with some more code for handling paths



22E4BD4h - LoadScriptData
22E4CD4h - ScriptLocateSet

22E544Ch - LoadSSE
22E551Ch - LoadSSS
22E56B8h - LoadSSA
22E584Ch - LoadTalkExport
22E590Ch - LoadTalk

//These call ScriptLocateSet with various different parameters
22E59E8h
22E5A10h
22E5A38h
22E5A50h
2AE5A38h
2AE5A50h
//End

22E5D50h - LoadSSB


22F7BC4h - DecideScriptDataToLoad

//Functions Handling OpCodes:
022DDD64 - Handle opcodes first pass. Classifies the opcodes using a value from 0 to 6, store it in script engine struct at 0xE
022E2480 - Hanlde some script opcodes. Unknown purpose
022DD164 - Calls 022DDD64, and 022E2480. Also does some handling of script data on its own depending on how the last instruction is classified.
022DD2C0 - Handles opcodes after they were handled by 022DD164 and 022DDD64. Uses value in the script engine struct at 0xE.


//Enums
enum ScriptDataTy
{
    ??? = 0,
    SSE = 1,
    SSS = 2,
    SSA = 3,
};

//Structure in memory
struct CurrentSSBFilePointers_
{
    uint32_t PtrUnk1;           //00
    uint32_t unk2;              //04
    uint32_t unk3;              //08
    uint16_t unk4;              //0C
    uint16_t unkstate;          //0E When this is 5, it apparently loads a SSA/SSS/SSE file
    uint32_t unk6;              //10
    uint32_t PtrScriptDataHdr;  //14    
    uint32_t PtrScriptOpCodes;  //18
    uint32_t PtrUnk9;           //1C Possibly current opcode
    uint32_t PtrUnk10;          //20 Points one after the end of the SSB file, when no strings or constants
    uint32_t unk11;             //24
    uint32_t unk12;             //28
    uint32_t unk13;             //2C
    uint32_t unk14;             //30
    uint32_t PtrUnk15;          //34 Possibly previous opcode beg
    uint32_t PtrUnk16;          //38 Possibly previous opcode's first parameter

};


/*
    fun_022F7BC4 22F7BC4h DecideScriptDataToLoad
        Seems to be linked to deciding if should load SSA/SSE/SSS
*/
022F7BC4 E92D4008 push    r3,r14
022F7BC8 E59F00E8 ldr     r0,=2324CF4h      //ScriptEngineLoaderState?
022F7BCC E5903000 ldr     r3,[r0]
022F7BD0 E59300F0 ldr     r0,[r3,0F0h]      //Get ScriptEngineEnumValue1
022F7BD4 E3500001 cmp     r0,1h
if( R0 != 1 )
    022F7BD8 1A00002F bne     22F7C9Ch      ///022F7C9C LBL9
022F7BDC E59300F4 ldr     r0,[r3,0F4h]
022F7BE0 E3500009 cmp     r0,9h
if( R0 < 9 )
    022F7BE4 908FF100 addls   r15,r15,r0,lsl 2h
//CASE_DEFAULT:
022F7BE8 EA000027 b       22F7C8Ch          ///022F7C8C LBL1
//CASE_0:
022F7BEC EA000026 b       22F7C8Ch          ///022F7C8C LBL1
//CASE_1:
022F7BF0 EA000025 b       22F7C8Ch          ///022F7C8C LBL1
//CASE_2:
022F7BF4 EA000024 b       22F7C8Ch          ///022F7C8C LBL1
//CASE_3:
022F7BF8 EA000005 b       22F7C14h          ///022F7C14 LBL2
//CASE_4:
022F7BFC EA000007 b       22F7C20h          ///022F7C20 LBL3
//CASE_5:
022F7C00 EA000009 b       22F7C2Ch          ///022F7C2C LBL4
//CASE_6:
022F7C04 EA00000E b       22F7C44h          ///022F7C44 LBL5
//CASE_7:
022F7C08 EA000013 b       22F7C5Ch          ///022F7C5C LBL6
//CASE_8:
022F7C0C EA000015 b       22F7C68h          ///022F7C68 LBL7
//CASE_9:
022F7C10 EA000019 b       22F7C7Ch          ///022F7C7C LBL8
///022F7C14 LBL2
022F7C14 E1D30FFA ldrsh   r0,[r3,0FAh]      //Possibly Script Directory Name Index?
022F7C18 EBFFB60B bl      22E544Ch          ///fun_022E544C LoadSSE
022F7C1C EA00001A b       22F7C8Ch          ///022F7C8C LBL1
///022F7C20 LBL3
022F7C20 E1D30FFA ldrsh   r0,[r3,0FAh]      //Possibly Script Directory Name Index?
022F7C24 EBFFB608 bl      22E544Ch          ///fun_022E544C LoadSSE    
022F7C28 EA000017 b       22F7C8Ch          ///022F7C8C LBL1
///022F7C2C LBL4
022F7C2C E2831C01 add     r1,r3,100h
022F7C30 E1D30FFA ldrsh   r0,[r3,0FAh]      //Possibly Script Directory Name Index?
022F7C34 E1D120D4 ldrsb   r2,[r1,4h]
022F7C38 E28310FC add     r1,r3,0FCh
022F7C3C EBFFB636 bl      22E551Ch          ///fun_022E551C LoadSSS 
022F7C40 EA000011 b       22F7C8Ch          ///022F7C8C LBL1
///022F7C44 LBL5
022F7C44 E2831C01 add     r1,r3,100h
022F7C48 E1D30FFA ldrsh   r0,[r3,0FAh]      //Possibly Script Directory Name Index?
022F7C4C E1D120D4 ldrsb   r2,[r1,4h]
022F7C50 E28310FC add     r1,r3,0FCh
022F7C54 EBFFB697 bl      22E56B8h          ///022E56B8 LoadSSA
022F7C58 EA00000B b       22F7C8Ch          ///022F7C8C LBL1
///022F7C5C LBL6
022F7C5C E28300FC add     r0,r3,0FCh
022F7C60 EBFFB6F9 bl      22E584Ch          ///fun_022E584C LoadTalkExport
022F7C64 EA000008 b       22F7C8Ch          ///022F7C8C LBL1
///022F7C68 LBL7
022F7C68 E2831C01 add     r1,r3,100h
022F7C6C E1D100F6 ldrsh   r0,[r1,6h]
022F7C70 E1D110F8 ldrsh   r1,[r1,8h]
022F7C74 EBFFB724 bl      22E590Ch          ///022E590C LoadTalk
022F7C78 EA000003 b       22F7C8Ch          ///022F7C8C LBL1
///022F7C7C LBL8
022F7C7C E1D30FFA ldrsh   r0,[r3,0FAh]      //Possibly Script Directory Name Index?
022F7C80 E28310FC add     r1,r3,0FCh
022F7C84 E3A02000 mov     r2,0h
022F7C88 EBFFB623 bl      22E551Ch          ///fun_022E551C LoadSSS 
///022F7C8C LBL1
022F7C8C E59F0024 ldr     r0,=2324CF4h      //ScriptEngineLoaderState?
022F7C90 E3A01002 mov     r1,2h
022F7C94 E5900000 ldr     r0,[r0]
022F7C98 E58010F0 str     r1,[r0,0F0h]      //Sets ScriptEngineEnumValue1 to 2
///022F7C9C LBL9
022F7C9C E59F0014 ldr     r0,=2324CF4h      //ScriptEngineLoaderState?
022F7CA0 E5900000 ldr     r0,[r0]
022F7CA4 E59010F0 ldr     r1,[r0,0F0h]      //Gets ScriptEngineEnumValue1
022F7CA8 E3510000 cmp     r1,0h
if( R1 != 0 )                               //ScriptEngineEnumValue1 != 0
    022F7CAC 18BD8008 popne   r3,r15    
022F7CB0 EBFF9582 bl      22DD2C0h          ///fun_022DD2C0 ( R0,  )
022F7CB4 E8BD8008 pop     r3,r15
022F7CB8 02324CF4 


/*
    fun_022E56B8 LoadSSA 22E56B8h
        Seems to handle SSA loading among other things!
*/
// R0 == ScriptResourceID ###TODO: find where that number came from!!!!
022E56B8 E92D4070 push    r4-r6,r14
022E56BC E24DD098 sub     r13,r13,98h
022E56C0 E1A05000 mov     r5,r0             //R0 = ScriptResourceID
022E56C4 E3A00002 mov     r0,2h             //R0 = 2
022E56C8 E1A04001 mov     r4,r1             //R1 = ScriptFilename // is the address of the string that indicated the current Resource/Script ID "T02A0201" for instance.
022E56CC E1A06002 mov     r6,r2             //R6 =  //R2 was 0 on that run
022E56D0 EBF49AD7 bl      200C234h          //This always set R0 to 0????
022E56D4 E3500000 cmp     r0,0h
if( R0 == 0 )
    022E56D8 0A000008 beq     22E5700h          //LBL1 022E5700 
else
    022E56DC E28D100D add     r1,r13,0Dh
    022E56E0 E1A00004 mov     r0,r4
    022E56E4 EBFFFBE0 bl      22E466Ch
    022E56E8 E59F1148 ldr     r1,=2319888h      //Pointer to string "ScriptStation load acting %d %s %d"
    022E56EC E28D300D add     r3,r13,0Dh        //
    022E56F0 E1A02005 mov     r2,r5             //R2 = ScriptResourceID
    022E56F4 E3A00002 mov     r0,2h             //
    022E56F8 E58D6000 str     r6,[r13]
    022E56FC EBF49ACF bl      200C240h          //Seems to be "printf"
///LBL1 022E5700 -- AKA Find ScriptResourceNameID in the table.
022E5700 E59F0134 ldr     r0,=2324F74h      //StructWithScriptData
022E5704 E3E01000 mvn     r1,0h             // R1 = 0xFFFF
022E5708 E1C010BC strh    r1,[r0,0Ch]       // Store R1 in the the scriptstatestruct
022E570C E1D000F6 ldrsh   r0,[r0,6h]        //
022E5710 E1500005 cmp     r0,r5             //Check if the entry's ID matches R5, aka ScriptResourceID
if( R0 == ScriptResourceID )
    022E5714 0A000001 beq     22E5720h          //If it does goto LBL2 022E5720 
else
    022E5718 E1A00005 mov     r0,r5             //If not, pass "ScriptResourceID" as parameter to fun 022E544C!
    022E571C EBFFFF4A bl      22E544Ch          ///22E544Ch - LoadSSE
///LBL2 022E5720
022E5720 E59F0114 ldr     r0,=2324F74h  //StructWithScriptData   
022E5724 E1D000F4 ldrsh   r0,[r0,4h]
022E5728 E1500005 cmp     r0,r5
022E572C 1A000008 bne     22E5754h
022E5730 E3550000 cmp     r5,0h
022E5734 B3A00000 movlt   r0,0h
022E5738 BA00003C blt     22E5830h
022E573C E59F00FC ldr     r0,=2324F8Ah  //StringCurrentScriptName
022E5740 E1A01004 mov     r1,r4
022E5744 EBFFFBC4 bl      22E465Ch
022E5748 E3500000 cmp     r0,0h
022E574C 13A00000 movne   r0,0h
022E5750 1A000036 bne     22E5830h
022E5754 E3A00003 mov     r0,3h
022E5758 E2401005 sub     r1,r0,5h
022E575C EB004B97 bl      22F85C0h
022E5760 E3A00003 mov     r0,3h
022E5764 E2401005 sub     r1,r0,5h
022E5768 EB005B49 bl      22FC494h
022E576C E3A00003 mov     r0,3h
022E5770 E2401005 sub     r1,r0,5h
022E5774 EB00612E bl      22FDC34h
022E5778 E3550000 cmp     r5,0h
022E577C AA000006 bge     22E579Ch
022E5780 E59F10B4 ldr     r1,=2324F74h      //StructWithScriptData
022E5784 E3E02000 mvn     r2,0h
022E5788 E59F00B0 ldr     r0,=2324F8Ah      //StringCurrentScriptName
022E578C E1C120B4 strh    r2,[r1,4h]
022E5790 EBFFFB97 bl      22E45F4h          ///022E45F4 (Puts 0 at 0x02324F8A StringCurrentScriptName)
022E5794 E3A00001 mov     r0,1h
022E5798 EA000024 b       22E5830h
022E579C E1A01005 mov     r1,r5
022E57A0 E1A02004 mov     r2,r4
022E57A4 E3A00003 mov     r0,3h
022E57A8 EBFFFD09 bl      22E4BD4h          //022E4BD4 LoadScriptData( ScriptDataTy (3 == SSA), R1, R2 )
022E57AC E3500000 cmp     r0,0h
if( LoadScriptData( 3, R1, R2) == 0 )       //If loading the SSA file failed, AKA returned 0
    022E57B0 0A000018 beq     22E5818h      ///022E5818 LBL3
022E57B4 E59F0080 ldr     r0,=2324F74h      //StructWithScriptData
022E57B8 E59F2080 ldr     r2,=2324F8Ah      //StringCurrentScriptName
022E57BC E1A03004 mov     r3,r4             //R3 = PtrEventName
022E57C0 E1C050B4 strh    r5,[r0,4h]        //Put ScriptResourceID into (StructWithScriptData + 4)
022E57C4 E3A01008 mov     r1,8h             //R1 = NbCharsToCopy
///022E57C8 LOOP_CopyEvNameBeg
022E57C8 E4D30001 ldrb    r0,[r3],1h        //R0 = PtrEventName, PtrEventName = PtrEventName + 1
022E57CC E2511001 subs    r1,r1,1h          //NbCharsToCopy = NbCharsToCopy - 1 (This also toggle the z flag when the result is 0)
022E57D0 E4C20001 strb    r0,[r2],1h        //R2 = PtrEventNameCopy, PtrEventNameCopy = PtrEventNameCopy + 1
if( NbCharsToCopy != 0 )
    022E57D4 1AFFFFFB bne     22E57C8h      ///022E57C8 LOOP_CopyEvNameBeg
022E57D8 E28D1004 add     r1,r13,4h         //R1 =  (data at ptr r1 after was 0x16 )
022E57DC E1A00004 mov     r0,r4             //R0 = PtrEventName
022E57E0 EBFFFBAD bl      22E469Ch          ///022E469C CopyEventNameInLowerCase( PtrEventName, PtrEvNameCopy )
022E57E4 E1A00005 mov     r0,r5             //R0 = ScriptResourceID
022E57E8 EBF5FE03 bl      2064FFCh          ///R0 = GetScriptIDName(ScriptResourceID)
022E57EC E1A02000 mov     r2,r0             //R2 = ScriptIDName
022E57F0 E59F104C ldr     r1,=2319730h      //"SCRIPT/%s/%s.ssb"
022E57F4 E28D0016 add     r0,r13,16h        //R0 = Possibly target string
022E57F8 E28D3004 add     r3,r13,4h         //R3 = lower case filename
022E57FC EBF68F60 bl      2089584h          ///fun_02089584 ReplaceTokens
022E5800 E59F0040 ldr     r0,=2324C84h      //R0 = PtrSSBBufferPtr/Struct
022E5804 E28D1016 add     r1,r13,16h        //R1 = PtrToStrResultReplaceTokens //AKA the path to the full filename of the ssb file.
022E5808 EB000150 bl      22E5D50h          ///022E5D50 Load SSB script
022E580C E3500000 cmp     r0,0h
if( R0 != 0 )
    022E5810 13A00001 movne   r0,1h             //Return 1
    022E5814 1A000005 bne     22E5830h          ///022E5830 LBL4
///022E5818 LBL3
022E5818 E59F101C ldr     r1,=2324F74h      //StructWithScriptData
022E581C E3E02000 mvn     r2,0h
022E5820 E59F0018 ldr     r0,=2324F8Ah      //StringCurrentScriptName
022E5824 E1C120B4 strh    r2,[r1,4h]
022E5828 EBFFFB71 bl      22E45F4h          ///022E45F4 (Puts 0 at 0x02324F8A StringCurrentScriptName)
022E582C E3A00000 mov     r0,0h             //Return 0
///022E5830 LBL4
022E5830 E28DD098 add     r13,r13,98h
022E5834 E8BD8070 pop     r4-r6,r15



/*
    fun_022E544C LoadSSE
        Seems to load SSE among other things
*/
//R0 == ScriptResourceNameID
022E544C E92D4018 push    r3,r4,r14
022E5450 E24DD004 sub     r13,r13,4h
022E5454 E1A04000 mov     r4,r0         // R4 = ScriptResourceNameID
022E5458 E59F10AC ldr     r1,=231984Ch  // Pointer to string "ScriptStation load common %d"
022E545C E1A02004 mov     r2,r4         // R2 = ScriptResourceNameID
022E5460 E3A00002 mov     r0,2h         // R0 = 2
022E5464 EBF49B75 bl      200C240h      // Probably debug printf, its completely empty.
022E5468 E59F10A0 ldr     r1,=2324F74h  //StructWithScriptData
022E546C E1D100F6 ldrsh   r0,[r1,6h]    //Read CurResourceID
022E5470 E1500004 cmp     r0,r4
if( CurResourceID == ScriptResourceNameID )
    022E5474 03A00000 moveq   r0,0h         //Set the return value to 0
    022E5478 0A000021 beq     22E5504h      ///022E5504 LBL1
022E547C E3E02000 mvn     r2,0h         // R2 = 0xFFFF
022E5480 E1C120B8 strh    r2,[r1,8h]    // Put 0xFFFF at StructWithScriptData + 8
022E5484 E1C120BA strh    r2,[r1,0Ah]   // Put 0xFFFF at StructWithScriptData + 0xA
022E5488 E1C120B2 strh    r2,[r1,2h]    // Put 0xFFFF at StructWithScriptData + 2
022E548C E59F0080 ldr     r0,=2324F82h  // Pass as parameter to fun_022E45F4 the address of StructB.
022E5490 E1C120B4 strh    r2,[r1,4h]    // Put 0xFFFF at StructWithScriptData + 4 
022E5494 EBFFFC56 bl      22E45F4h      /// fun_022E45F4, put 0 in StructB
022E5498 E59F0078 ldr     r0,=2324F8Ah  //StringCurrentScriptName
022E549C EBFFFC54 bl      22E45F4h      /// fun_022E45F4, put 0 in StringCurrentScriptName
022E54A0 E3540000 cmp     r4,0h         // If the ScriptResourceNameID is 0
if( ScriptResourceNameID < 0 )
    022E54A4 B59F0064 ldrlt   r0,=2324F74h  //StructWithScriptData
    022E54A8 B3E01000 mvnlt   r1,0h         //
    022E54AC B1C010B6 strlth  r1,[r0,6h]    // (StructWithScriptData + 6) = 0
    022E54B0 BA000012 blt     22E5500h      /// 022E5500 LBL2
022E54B4 E59F0054 ldr     r0,=2324F74h  //StructWithScriptData
022E54B8 E1C040B6 strh    r4,[r0,6h]    // (StructWithScriptData + 6) = ScriptResourceNameID
022E54BC EB000169 bl      22E5A68h      /// fun_022E5A68
022E54C0 E1A01004 mov     r1,r4
022E54C4 E3A00001 mov     r0,1h         //Type1 is SSE
022E54C8 E3A02000 mov     r2,0h
022E54CC EBFFFDC0 bl      22E4BD4h      /// fun_022E4BD4( 1, ScriptResourceNameID, 0 ) LoadScriptData
022E54D0 E3500000 cmp     r0,0h
if( R0 != 0 )
    022E54D4 159F0034 ldrne   r0,=2324F74h  //StructWithScriptData
022E54D8 E3A03000 mov     r3,0h
if( R0 != 0 )
    022E54DC 11C040B8 strneh  r4,[r0,8h]
022E54E0 E3A00001 mov     r0,1h
022E54E4 E1A01004 mov     r1,r4
022E54E8 E2402002 sub     r2,r0,2h
022E54EC E58D3000 str     r3,[r13]
022E54F0 EBFFFEB5 bl      22E4FCCh          ///022E4FCC
022E54F4 E3500000 cmp     r0,0h
if( R0 != 0 )
    022E54F8 159F0010 ldrne   r0,=2324F74h  //StructWithScriptData
    022E54FC 11C040BA strneh  r4,[r0,0Ah]
///022E5500 LBL2
022E5500 E3A00001 mov     r0,1h
///022E5504 LBL1
022E5504 E28DD004 add     r13,r13,4h
022E5508 E8BD8018 pop     r3,r4,r15
022E550C 0231984C 
022E5510 02324F74 
022E5514 02324F82 
022E5518 02324F8A 

/*
    fun_022E45F4
*/
022E45F4 E3A01000 mov     r1,0h     
022E45F8 E5C01000 strb    r1,[r0]   //Put 0 in StructB
022E45FC E12FFF1E bx      r14

/*
    fun_022E5A68
*/
022E5A68 E92D4008 push    r3,r14
022E5A6C E59F1020 ldr     r1,=23198F0h  // "ScriptStation_RemoveAll"
022E5A70 E3A00002 mov     r0,2h
022E5A74 EBF499F1 bl      200C240h      //debug print
022E5A78 E3A00000 mov     r0,0h         //Pass 0 as param
022E5A7C EB00155D bl      22EAFF8h      /// fun_022EAFF8
022E5A80 EB0063E8 bl      22FEA28h      /// 022FEA28    //Loops through some static memory
022E5A84 EB0048A3 bl      22F7D18h      /// 022F7D18    
022E5A88 EB0058DD bl      22FBE04h      /// 022FBE04
022E5A8C EB005EF6 bl      22FD66Ch      /// 022FD66C
022E5A90 E8BD8008 pop     r3,r15
022E5A94 023198F0 


/*
    fun_022EAFF8
*/
022EAFF8 E92D4010 push    r4,r14
022EAFFC E24DD008 sub     r13,r13,8h
022EB000 E59F3074 ldr     r3,=23891F8h
022EB004 E59F2074 ldr     r2,=2320BBCh
022EB008 E3A01054 mov     r1,54h
022EB00C E592C01C ldr     r12,[r2,1Ch]
022EB010 E592E018 ldr     r14,[r2,18h]
022EB014 E0243190 mla     r4,r0,r1,r3   // R4 = R0 * 0x54 + 0x023891F8
022EB018 E58DE000 str     r14,[r13]
022EB01C E58DC004 str     r12,[r13,4h]
022EB020 E5923020 ldr     r3,[r2,20h]   // Load from struct at 02320BBC + 0x20
022EB024 E5921024 ldr     r1,[r2,24h]   
022EB028 E28D2000 add     r2,r13,0h
022EB02C E5843048 str     r3,[r4,48h]
022EB030 E584104C str     r1,[r4,4Ch]
022EB034 E3A01000 mov     r1,0h
022EB038 E5C41050 strb    r1,[r4,50h]
022EB03C EB000028 bl      22EB0E4h
022EB040 E3A01000 mov     r1,0h
022EB044 E5841008 str     r1,[r4,8h]
022EB048 E584100C str     r1,[r4,0Ch]
022EB04C E5841010 str     r1,[r4,10h]
022EB050 E5841014 str     r1,[r4,14h]
022EB054 E2410001 sub     r0,r1,1h
022EB058 E5840004 str     r0,[r4,4h]
022EB05C E5841030 str     r1,[r4,30h]
022EB060 E5841034 str     r1,[r4,34h]
022EB064 E5841038 str     r1,[r4,38h]
022EB068 E584103C str     r1,[r4,3Ch]
022EB06C E5841040 str     r1,[r4,40h]
022EB070 E5841044 str     r1,[r4,44h]
022EB074 E28DD008 add     r13,r13,8h
022EB078 E8BD8010 pop     r4,r15
022EB07C 023891F8 eoreqs  r9,r8,3Eh
022EB080 02320BBC eoreqs  r0,r2,2F000h





/*
    fun_022E4BD4 LoadScriptData( ScriptDataTy, ScriptResourceNameID, StrScriptDataFilename )
*/
//R0 = ScriptDataType, R1 = ScriptResourceID, R2 = StrScriptDataFilename
022E4BD4 E92D4030 push    r4,r5,r14
022E4BD8 E24DD08C sub     r13,r13,8Ch
022E4BDC E1A05000 mov     r5,r0             //R5 = ScriptDataType
022E4BE0 E1A04001 mov     r4,r1             //R4 = ScriptResourceID
022E4BE4 E3550002 cmp     r5,2h             
if( ScriptDataType != 2 )
    022E4BE8 1A00000B bne     22E4C1Ch      ///022E4C1C LBL1
022E4BEC E28D1000 add     r1,r13,0h
022E4BF0 E1A00002 mov     r0,r2             //R0 = StrScriptDataFilename
022E4BF4 EBFFFEA8 bl      22E469Ch          ///022E469C CopyEventNameInLowerCase( StrScriptDataFilename, PtrEvNameCopy )
022E4BF8 E1A00004 mov     r0,r4             //R0 = ScriptResourceID
022E4BFC EBF600FE bl      2064FFCh          ///R0 = GetScriptIDName(ScriptResourceID)
022E4C00 E1A02000 mov     r2,r0             //R2 = ScriptEventName
022E4C04 E59F10AC ldr     r1,=23196B0h      //"SCRIPT/%s/%s.sss" 
022E4C08 E28D0009 add     r0,r13,9h         //R0 = PtrFullPathOnStack
022E4C0C E28D3000 add     r3,r13,0h         //R3 = PtrFilenameLowerCaseNoExt
022E4C10 EBF6925B bl      2089584h          ///fun_02089584 ReplaceTokens
022E4C14 E59F40A0 ldr     r4,=2324C8Ch      //R4 = AddressPtrToSDatBuffer = 02324C8C //SSS struct address
022E4C18 EA000019 b       22E4C84h          ///022E4C84 LBL3
///022E4C1C LBL1
022E4C1C E3550003 cmp     r5,3h
if( ScriptDataType != 3 )
    022E4C20 1A00000B bne     22E4C54h      ///022E4C54 LBL2
022E4C24 E28D1000 add     r1,r13,0h
022E4C28 E1A00002 mov     r0,r2             //R0 = StrScriptDataFilename
022E4C2C EBFFFE9A bl      22E469Ch          ///022E469C CopyEventNameInLowerCase( PtrEvName, PtrEvNameCopy )
022E4C30 E1A00004 mov     r0,r4             //R0 = ScriptResourceID
022E4C34 EBF600F0 bl      2064FFCh          ///R0 = GetScriptIDName(ScriptResourceID)
022E4C38 E1A02000 mov     r2,r0             //R2 = ScriptEventName
022E4C3C E59F107C ldr     r1,=23196C4h      //"SCRIPT/%s/%s.ssa"
022E4C40 E28D0009 add     r0,r13,9h         //R0 = PtrFullPathOnStack    
022E4C44 E28D3000 add     r3,r13,0h         //R3 = PtrFilenameLowerCaseNoExt
022E4C48 EBF6924D bl      2089584h          ///fun_02089584 ReplaceTokens
022E4C4C E59F4070 ldr     r4,=2324C6Ch      //R4 = AddressPtrToSDatBuffer = 02324C6C //SSA struct address
022E4C50 EA00000B b       22E4C84h          ///022E4C84 LBL3
///022E4C54 LBL2
022E4C54 E1A00004 mov     r0,r4             //R0 = ScriptResourceID
022E4C58 EBF600E7 bl      2064FFCh          ///R0 = GetScriptIDName( ScriptResourceID )
022E4C5C E1A02000 mov     r2,r0             //R2 = ScriptEventName
022E4C60 E59F1060 ldr     r1,=23196D8h      //"SCRIPT/%s/enter.sse"
022E4C64 E28D0009 add     r0,r13,9h         //R0 = PtrFullPathOnStack
022E4C68 EBF69245 bl      2089584h          ///fun_02089584 ReplaceTokens
022E4C6C E28D0009 add     r0,r13,9h         //R0 = PtrFullPathOnStack
022E4C70 EBF48FCC bl      2008BA8h          /// fun_02008BA8
022E4C74 E3500000 cmp     r0,0h
if( R0 <= 0 )
    022E4C78 D3A00000 movle   r0,0h         //Return 0, failed
    022E4C7C DA00000B ble     22E4CB0h      ///022E4CB0 LBL4
022E4C80 E59F4044 ldr     r4,=2324C94h      //R4 = AddressPtrToSDatBuffer = 02324C94 //SSE struct address
///022E4C84 LBL3
022E4C84 E59F1044 ldr     r1,=23196ECh      //"station load hanger %d file name [%s]"
022E4C88 E28D3009 add     r3,r13,9h         //R3 = PtrFullPathOnStack
022E4C8C E1A02005 mov     r2,r5             //R2 = ScriptDataType
022E4C90 E3A00002 mov     r0,2h
022E4C94 EBF49D69 bl      200C240h          ///Call debug print ( 2, "station load hanger %d file name [%s]", ScriptDataType, PtrFullPathOnStack)
022E4C98 E8940003 ldmia   [r4],r0,r1
022E4C9C EBF4796B bl      2003250h          //FillBufferWithZeros( R0=buffaddress, R1=BuffLen )
{
    02003250 E3A02000 mov     r2,0h
    02003254 EA000001 b       2003260h
    //LOOP_BEG
    02003258 E2411001 sub     r1,r1,1h
    0200325C E4C02001 strb    r2,[r0],1h
    02003260 E3510000 cmp     r1,0h
    if( BytesLeftToZero > 0 )
        02003264 CAFFFFFB bgt     2003258h      //02003258 LOOP_BEG
    02003268 E12FFF1E bx      r14
}
022E4CA0 E28D1009 add     r1,r13,9h         //R1 = PtrFullPathOnStack
022E4CA4 E1A00004 mov     r0,r4             //Set as parameter the address of the pointer to place the buffer containing the loaded bytes of the SSA.
022E4CA8 EBF48FDA bl      2008C18h          ///02008C18 This function loaded the SSA file at one point, and put the pointer to the data in R4!
{
    02008C18 E92D4008 push    r3,r14
    02008C1C E24DD028 sub     r13,r13,28h
    02008C20 E1A03000 mov     r3,r0
    02008C24 E1A02001 mov     r2,r1
    02008C28 E28D0000 add     r0,r13,0h
    02008C2C E1A01003 mov     r1,r3
    02008C30 EB000036 bl      2008D10h          ///02008D10
    {
        02008D10 E92D4070 push    r4-r6,r14
        02008D14 E24DDC01 sub     r13,r13,100h
        02008D18 E1A06000 mov     r6,r0
        02008D1C E1A04002 mov     r4,r2
        02008D20 E1A05001 mov     r5,r1
        02008D24 EBFFFDC8 bl      200844Ch
        02008D28 E28D0000 add     r0,r13,0h
        02008D2C E1A01006 mov     r1,r6
        02008D30 E1A03004 mov     r3,r4
        02008D34 E3A02008 mov     r2,8h
        02008D38 EBFFFF10 bl      2008980h
        02008D3C E5951000 ldr     r1,[r5]
        02008D40 E28D0000 add     r0,r13,0h
        02008D44 E58D100C str     r1,[r13,0Ch]
        02008D48 E5951004 ldr     r1,[r5,4h]
        02008D4C E58D1014 str     r1,[r13,14h]
        02008D50 EBFFFF4B bl      2008A84h
        02008D54 E5960010 ldr     r0,[r6,10h]
        02008D58 E28DDC01 add     r13,r13,100h
        02008D5C E8BD8070 pop     r4-r6,r15
    }
    02008C34 E28DD028 add     r13,r13,28h
    02008C38 E8BD8008 pop     r3,r15
}
022E4CAC E3A00001 mov     r0,1h             //Return 1
///022E4CB0 LBL4
022E4CB0 E28DD08C add     r13,r13,8Ch
022E4CB4 E8BD8030 pop     r4,r5,r15         //
022E4CB8 023196B0 
022E4CBC 02324C8C 
022E4CC0 023196C4 
022E4CC4 02324C6C 
022E4CC8 023196D8 
022E4CCC 02324C94 
022E4CD0 023196EC 



/*
    fun_02064FFC const char* GetScriptIDName( int index )
        Pass ScriptResourceNameID, and returns the pointer to the Resource name string at that index!
        Returns the ptr to the name string
*/
// R0 = ScriptResourceNameID
02064FFC E3A0100C mov     r1,0Ch            //
02065000 E1610180 smulbb  r1,r0,r1          // R1 = ScriptResourceNameID * 12
02065004 E59F0004 ldr     r0,=20A5490h      //Lookup table for script ids!
02065008 E7900001 ldr     r0,[r0,r1]        // R0 = [0x20A5490 + (ScriptResourceNameID * 12) ]
0206500C E12FFF1E bx      r14
02065010 020A5490 


/*
    fun_022E551C LoadSSS
        Seems to load SSS files and do other stuff
*/
//22E4BD4h

022E551C E92D4070 push    r4-r6,r14
022E5520 E24DD010 sub     r13,r13,10h
022E5524 E1A06000 mov     r6,r0
022E5528 E3A00002 mov     r0,2h
022E552C E1A05001 mov     r5,r1
022E5530 E1A04002 mov     r4,r2
022E5534 EBF49B3E bl      200C234h
022E5538 E3500000 cmp     r0,0h
022E553C 0A000008 beq     22E5564h
022E5540 E28D1004 add     r1,r13,4h
022E5544 E1A00005 mov     r0,r5
022E5548 EBFFFC47 bl      22E466Ch
022E554C E59F1158 ldr     r1,=231986Ch          //"ScriptStation load %d %s %d"
022E5550 E28D3004 add     r3,r13,4h
022E5554 E1A02006 mov     r2,r6
022E5558 E3A00002 mov     r0,2h
022E555C E58D4000 str     r4,[r13]
022E5560 EBF49B36 bl      200C240h              //DebugPrint
022E5564 E59F0144 ldr     r0,=2324F74h      //StructWithScriptData
022E5568 E1D000F6 ldrsh   r0,[r0,6h]
022E556C E1500006 cmp     r0,r6
022E5570 0A000001 beq     22E557Ch
022E5574 E1A00006 mov     r0,r6
022E5578 EBFFFFB3 bl      22E544Ch
022E557C E59F012C ldr     r0,=2324F74h      //StructWithScriptData
022E5580 E1D000F2 ldrsh   r0,[r0,2h]
022E5584 E1500006 cmp     r0,r6
022E5588 1A000008 bne     22E55B0h
022E558C E3560000 cmp     r6,0h
022E5590 B3A00000 movlt   r0,0h
022E5594 BA000042 blt     22E56A4h
022E5598 E59F0114 ldr     r0,=2324F82h
022E559C E1A01005 mov     r1,r5
022E55A0 EBFFFC2D bl      22E465Ch
022E55A4 E3500000 cmp     r0,0h
022E55A8 13A00000 movne   r0,0h
022E55AC 1A00003C bne     22E56A4h
022E55B0 E3A00002 mov     r0,2h
022E55B4 E2401004 sub     r1,r0,4h
022E55B8 EB004C00 bl      22F85C0h
022E55BC E3A00002 mov     r0,2h
022E55C0 E2401004 sub     r1,r0,4h
022E55C4 EB005BB2 bl      22FC494h
022E55C8 E3A00002 mov     r0,2h
022E55CC E2401004 sub     r1,r0,4h
022E55D0 EB006197 bl      22FDC34h
022E55D4 E3A00002 mov     r0,2h
022E55D8 E2401004 sub     r1,r0,4h
022E55DC EB0065AE bl      22FEC9Ch
022E55E0 E3560000 cmp     r6,0h
022E55E4 AA000009 bge     22E5610h
022E55E8 E59F10C0 ldr     r1,=2324F74h      //StructWithScriptData
022E55EC E3E02000 mvn     r2,0h
022E55F0 E59F00BC ldr     r0,=2324F82h
022E55F4 E1C120B2 strh    r2,[r1,2h]
022E55F8 EBFFFBFD bl      22E45F4h
022E55FC E59F00AC ldr     r0,=2324F74h      //StructWithScriptData
022E5600 E3A01000 mov     r1,0h
022E5604 E5C01000 strb    r1,[r0]
022E5608 E3A00001 mov     r0,1h
022E560C EA000024 b       22E56A4h
022E5610 E1A01006 mov     r1,r6
022E5614 E1A02005 mov     r2,r5
022E5618 E3A00002 mov     r0,2h
022E561C EBFFFD6C bl      22E4BD4h
022E5620 E3500000 cmp     r0,0h
022E5624 0A000016 beq     22E5684h
022E5628 E59F0080 ldr     r0,=2324F74h      //StructWithScriptData
022E562C E59F2080 ldr     r2,=2324F82h
022E5630 E1A03005 mov     r3,r5
022E5634 E1C060B2 strh    r6,[r0,2h]
022E5638 E3A01008 mov     r1,8h
022E563C E4D30001 ldrb    r0,[r3],1h
022E5640 E2511001 subs    r1,r1,1h
022E5644 E4C20001 strb    r0,[r2],1h
022E5648 1AFFFFFB bne     22E563Ch
022E564C E3A00002 mov     r0,2h
022E5650 E1A01006 mov     r1,r6
022E5654 E1A03005 mov     r3,r5
022E5658 E2402003 sub     r2,r0,3h
022E565C E58D4000 str     r4,[r13]
022E5660 EBFFFE59 bl      22E4FCCh
022E5664 E3500000 cmp     r0,0h
022E5668 159F0040 ldrne   r0,=2324F74h      //StructWithScriptData
022E566C 13A01001 movne   r1,1h
022E5670 059F0038 ldreq   r0,=2324F74h      //StructWithScriptData
022E5674 03A01000 moveq   r1,0h
022E5678 E5C01000 strb    r1,[r0]
022E567C E3A00001 mov     r0,1h
022E5680 EA000007 b       22E56A4h
022E5684 E59F1024 ldr     r1,=2324F74h      //StructWithScriptData
022E5688 E3E02000 mvn     r2,0h
022E568C E59F0020 ldr     r0,=2324F82h
022E5690 E1C120B2 strh    r2,[r1,2h]
022E5694 EBFFFBD6 bl      22E45F4h
022E5698 E59F1010 ldr     r1,=2324F74h      //StructWithScriptData
022E569C E3A00000 mov     r0,0h
022E56A0 E5C10000 strb    r0,[r1]
022E56A4 E28DD010 add     r13,r13,10h
022E56A8 E8BD8070 pop     r4-r6,r15
022E56AC 0231986C 
022E56B0 02324F74 
022E56B4 02324F82 



/*
    fun_022E584C LoadTalkExport
    
*/
022E584C E92D4010 push    r4,r14
022E5850 E24DD010 sub     r13,r13,10h
022E5854 E1A04000 mov     r4,r0
022E5858 E3A00002 mov     r0,2h
022E585C EBF49A74 bl      200C234h
022E5860 E3500000 cmp     r0,0h
022E5864 0A000006 beq     22E5884h
022E5868 E28D1004 add     r1,r13,4h
022E586C E1A00004 mov     r0,r4
022E5870 EBFFFB7D bl      22E466Ch
022E5874 E59F1084 ldr     r1,=23198ACh      //"ScriptStation load talk export %s"
022E5878 E28D2004 add     r2,r13,4h
022E587C E3A00002 mov     r0,2h
022E5880 EBF49A6E bl      200C240h              //DebugPrint
022E5884 E59F1078 ldr     r1,=2324F74h      //
022E5888 E3E02000 mvn     r2,0h
022E588C E59F0074 ldr     r0,=2324F8Ah
022E5890 E1C120B4 strh    r2,[r1,4h]
022E5894 EBFFFB56 bl      22E45F4h
022E5898 E3A02000 mov     r2,0h
022E589C E59F0060 ldr     r0,=2324F74h      //StructWithScriptData
022E58A0 E58D2000 str     r2,[r13]
022E58A4 E1D010F8 ldrsh   r1,[r0,8h]
022E58A8 E1A03004 mov     r3,r4
022E58AC E3A00006 mov     r0,6h
022E58B0 EBFFFDC5 bl      22E4FCCh                  //022E4FCC 
022E58B4 E1A04000 mov     r4,r0
022E58B8 E3A00003 mov     r0,3h
022E58BC E2401005 sub     r1,r0,5h
022E58C0 EB004B3E bl      22F85C0h                  //022F85C0 GroundLives Alternate hanger
022E58C4 E3A00003 mov     r0,3h
022E58C8 E2401005 sub     r1,r0,5h
022E58CC EB005AF0 bl      22FC494h                  //022FC494 GroundObject Altemate hanger
022E58D0 E3A00003 mov     r0,3h
022E58D4 E2401005 sub     r1,r0,5h
022E58D8 EB0060D5 bl      22FDC34h                  //022FDC34 GroundPerformer Remove hanger
022E58DC E3540000 cmp     r4,0h
022E58E0 159F001C ldrne   r0,=2324F74h      //StructWithScriptData
022E58E4 13A01006 movne   r1,6h
022E58E8 059F0014 ldreq   r0,=2324F74h      //StructWithScriptData
022E58EC 03E01000 mvneq   r1,0h
022E58F0 E1C010BC strh    r1,[r0,0Ch]
022E58F4 E1A00004 mov     r0,r4
022E58F8 E28DD010 add     r13,r13,10h
022E58FC E8BD8010 pop     r4,r15
022E5900 023198AC
022E5904 02324F74
022E5908 02324F8A


/*
    fun_022E590C    LoadTalk
        
*/
022E590C E92D4038 push    r3-r5,r14
022E5910 E1A04000 mov     r4,r0
022E5914 E1A05001 mov     r5,r1
022E5918 E59F10B8 ldr     r1,=23198D0h      //"ScriptStation load talk %d %d"
022E591C E1A02004 mov     r2,r4
022E5920 E1A03005 mov     r3,r5
022E5924 E3A00002 mov     r0,2h
022E5928 EBF49A44 bl      200C240h              //DebugPrint
022E592C E59F10A8 ldr     r1,=2324F74h      //StructWithScriptData
022E5930 E3E02000 mvn     r2,0h
022E5934 E59F00A4 ldr     r0,=2324F8Ah
022E5938 E1C120B4 strh    r2,[r1,4h]
022E593C EBFFFB2C bl      22E45F4h
022E5940 E3540002 cmp     r4,2h
022E5944 1A000009 bne     22E5970h
022E5948 E59F008C ldr     r0,=2324F74h      //StructWithScriptData
022E594C E3A01000 mov     r1,0h
022E5950 E58D1000 str     r1,[r13]
022E5954 E1D010F2 ldrsh   r1,[r0,2h]
022E5958 E3A04005 mov     r4,5h
022E595C E59F3080 ldr     r3,=2324F82h
022E5960 E1A00004 mov     r0,r4
022E5964 E1A02005 mov     r2,r5
022E5968 EBFFFD97 bl      22E4FCCh
022E596C EA000007 b       22E5990h
022E5970 E3A03000 mov     r3,0h
022E5974 E59F0060 ldr     r0,=2324F74h      //StructWithScriptData
022E5978 E58D3000 str     r3,[r13]
022E597C E1D010F8 ldrsh   r1,[r0,8h]
022E5980 E3A04004 mov     r4,4h
022E5984 E1A00004 mov     r0,r4
022E5988 E1A02005 mov     r2,r5
022E598C EBFFFD8E bl      22E4FCCh
022E5990 E1A05000 mov     r5,r0
022E5994 E3A00003 mov     r0,3h
022E5998 E2401005 sub     r1,r0,5h
022E599C EB004B07 bl      22F85C0h
022E59A0 E3A00003 mov     r0,3h
022E59A4 E2401005 sub     r1,r0,5h
022E59A8 EB005AB9 bl      22FC494h
022E59AC E3A00003 mov     r0,3h
022E59B0 E2401005 sub     r1,r0,5h
022E59B4 EB00609E bl      22FDC34h
022E59B8 E3550000 cmp     r5,0h
022E59BC 159F0018 ldrne   r0,=2324F74h      //StructWithScriptData
022E59C0 03E01000 mvneq   r1,0h
022E59C4 11C040BC strneh  r4,[r0,0Ch]
022E59C8 059F000C ldreq   r0,=2324F74h      //StructWithScriptData
022E59CC 01C010BC streqh  r1,[r0,0Ch]
022E59D0 E1A00005 mov     r0,r5
022E59D4 E8BD8038 pop     r3-r5,r15
022E59D8 023198D0
022E59DC 02324F74
022E59E0 02324F8A
022E59E4 02324F82

/*
    fun_0022E469C 22E469Ch CopyEventNameInLowerCase( PtrEvName, PtrEvNameCopy )
*/
//R0 = PtrEvName, R1 = PtrEvNameCopy 
022E469C E92D4070 push    r4-r6,r14
022E46A0 E1A06000 mov     r6,r0         //R6 = PtrEvName
022E46A4 E1A05001 mov     r5,r1         //R5 = PtrEvNameCopy
022E46A8 E3A04000 mov     r4,0h         //R4 = CntLetter = 0    
022E46AC EA000005 b       22E46C8h      ///022E46C8 LBL1
///022E46B0 LOOP_BEG
022E46B0 E0D600D1 ldrsb   r0,[r6],1h    //R0 = *PtrEvName, PtrEvName = PtrEvName + 1 
022E46B4 E3500000 cmp     r0,0h         //Is character read == 0
if( CharRead == 0 )
    022E46B8 0A000004 beq     22E46D0h  ///022E46D0 LBL2
022E46BC EBF47732 bl      200238Ch      /// R0 = ToLowerCase( R0 )
022E46C0 E4C50001 strb    r0,[r5],1h    //*PtrEvNameCopy = CharRead, PtrEvNameCopy = PtrEvNameCopy + 1
022E46C4 E2844001 add     r4,r4,1h      //++CntLetter
///022E46C8 LBL1
022E46C8 E3540008 cmp     r4,8h         
if( CntLetter < 8 )
    022E46CC BAFFFFF7 blt     22E46B0h      ///022E46B0 LOOP_BEG
///022E46D0 LBL2
022E46D0 E3A00000 mov     r0,0h
022E46D4 E5C50000 strb    r0,[r5]       //Place terminating 0 at the end of the copied string.
022E46D8 E8BD8070 pop     r4-r6,r15


/*
    fun_0200238C 200238Ch char ToLowerCase( char )
*/
//R0 = Character
0200238C E3500041 cmp     r0,41h
if( Character < 'A' )
    02002390 B12FFF1E bxlt    r14   
02002394 E350005A cmp     r0,5Ah
if( Character < 'Z' )
    02002398 D2800020 addle   r0,r0,20h
    0200239C D1A00C00 movle   r0,r0,lsl 18h
    020023A0 D1A00C40 movle   r0,r0,asr 18h
020023A4 E12FFF1E bx      r14

/*  
    fun_022DD2C0 Process Structures and special opcodes
*/
//R0 = CurrentSSBFilePointers, R1, R2
022DD2C0 E92D41F0 push    r4-r8,r14
022DD2C4 E24DD0A0 sub     r13,r13,0A0h
022DD2C8 E1A04000 mov     r4,r0             //R4 = CurrentSSBFilePointers
022DD2CC E1D400FE ldrsh   r0,[r4,0Eh]       //LastReturnCode (Aka OpCode type)
022DD2D0 E3500000 cmp     r0,0h
if( R0 != 0 )
    022DD2D4 0A000289 beq     22DDD00h      ///022DDD00 LBL1
022DD2D8 E3500005 cmp     r0,5h
if( R0 != 5 )
    022DD2DC 1A000287 bne     22DDD00h      ///022DDD00 LBL1
022DD2E0 E5946034 ldr     r6,[r4,34h]       //Gets the pointer to the current SSB opcode
022DD2E4 E1D670B0 ldrh    r7,[r6]           //load opcode into R7 
022DD2E8 E3570096 cmp     r7,96h            
if( LastOpcode > 0x96 )
    022DD2EC CA000023 bgt     22DD380h              ///022DD380 LBL2
if( LastOpcode >= 0x96 )
    022DD2F0 AA0000BE bge     22DD5F0h              ///022DD5F0 LBL3
022DD2F4 E357008C cmp     r7,8Ch
if( LastOpcode > 0x8C )
    022DD2F8 CA000017 bgt     22DD35Ch              ///022DD35C LBL4
if( LastOpcode >= 0x8C )
    022DD2FC AA0001CB bge     22DDA30h              ///022DDA30 LBL5
022DD300 E357000E cmp     r7,0Eh
if( LastOpcode > 0x0E )
    022DD304 CA000011 bgt     22DD350h              ///022DD350 LBL7
022DD308 E3570000 cmp     r7,0h
if( LastOpcode >= 0 )
    022DD30C A08FF107 addge   r15,r15,r7,lsl 2h
022DD310 EA00027A b       22DDD00h          ///022DDD00 LBL1
022DD314 EA000279 b       22DDD00h          //0x0 022DDD00 LBL1
022DD318 EA00007B b       22DD50Ch          //0x1 022DD50C LBL6
022DD31C EA000277 b       22DDD00h          //0x2 022DDD00 LBL1
022DD320 EA000276 b       22DDD00h          //0x3 022DDD00 LBL1
022DD324 EA000275 b       22DDD00h          //0x4 022DDD00 LBL1
022DD328 EA0000E8 b       22DD6D0h          //0x5 022DD6D0 LBL8
022DD32C EA0000F3 b       22DD700h          //0x6 022DD700 LBL9
022DD330 EA000272 b       22DDD00h          //0x7 022DDD00 LBL1
022DD334 EA0000E5 b       22DD6D0h          //0x8 022DD6D0 LBL8
022DD338 EA00006D b       22DD4F4h          //0x9 022DD4F4 LBL10
022DD33C EA000117 b       22DD7A0h          //0xA 022DD7A0 LBL11
022DD340 EA000122 b       22DD7D0h          //0xB 022DD7D0 LBL12
022DD344 EA00012D b       22DD800h          //0xC 022DD800 LBL13
022DD348 EA0000E0 b       22DD6D0h          //0xD 022DD6D0 LBL8
022DD34C EA000074 b       22DD524h          //0xE 022DD524 LBL14
///LastOpcode > 0x0E
022DD350 E3570085 cmp     r7,85h
if( LastOpcode == 0x85 )
    022DD354 0A000244 beq     22DDC6Ch          ///022DDC6C LBL15
else
    022DD358 EA000268 b       22DDD00h          ///022DDD00 LBL1
///LastOpcode > 0x8C
022DD35C E3570091 cmp     r7,91h
if( LastOpcode > 0x91 )
    022DD360 CA000003 bgt     22DD374h          ///022DD374 LBL16
if( LastOpcode >= 0x91 )
    022DD364 AA0001B1 bge     22DDA30h          ///022DDA30 LBL5
022DD368 E3570090 cmp     r7,90h
if( LastOpcode == 0x90 )
    022DD36C 0A000236 beq     22DDC4Ch          ///022DDC4C LBL17
else
    022DD370 EA000262 b       22DDD00h          ///022DDD00 LBL1
///022DD374 LBL16
022DD374 E3570092 cmp     r7,92h
if( LastOpcode == 0x92 )
    022DD378 0A0001AC beq     22DDA30h           ///022DDA30 LBL5
else
    022DD37C EA00025F b       22DDD00h          ///022DDD00 LBL1
///022DD380 LBL2
022DD380 E357009A cmp     r7,9Ah
if( LastOpcode > 0x9A )
    022DD384 CA000007 bgt     22DD3A8h          ///022DD3A8
if( LastOpcode >= 0x9A )
    022DD388 AA0000BE bge     22DD688h
022DD38C E3570097 cmp     r7,97h
022DD390 CA000001 bgt     22DD39Ch
022DD394 0A0000AC beq     22DD64Ch
022DD398 EA000258 b       22DDD00h          ///022DDD00 LBL1
022DD39C E3570098 cmp     r7,98h
022DD3A0 0A0000C0 beq     22DD6A8h
022DD3A4 EA000255 b       22DDD00h          ///022DDD00 LBL1
///LastOpcode > 0x9A
022DD3A8 E35700CC cmp     r7,0CCh
if( LastOpcode > 0xCC )
    022DD3AC CA000018 bgt     22DD414h
else if( LastOpcode >= 0xCC )
    022DD3B0 AA00002C bge     22DD468h      ///022DD468
022DD3B4 E247009B sub     r0,r7,9Bh
022DD3B8 E3500013 cmp     r0,13h
if( (LastOpcode - 0x9B) <= 0x13 )
    022DD3BC 908FF100 addls   r15,r15,r0,lsl 2h
022DD3C0 EA00024E b       22DDD00h          ///022DDD00 LBL1
022DD3C4 EA000072 b       22DD594h          //0x9B  022DD594
022DD3C8 EA0000AE b       22DD688h          //0x9C  022DD688
022DD3CC EA000014 b       22DD424h          //0x9D  022DD424
022DD3D0 EA0000AC b       22DD688h          //0x9E  022DD688
022DD3D4 EA0000B8 b       22DD6BCh          //0x9F  022DD6BC
022DD3D8 EA0000AA b       22DD688h          //0xA0  022DD688
022DD3DC EA000247 b       22DDD00h          //0xA1  022DDD00 LBL1
022DD3E0 EA000246 b       22DDD00h          //0xA2  022DDD00 LBL1
022DD3E4 EA000245 b       22DDD00h          //0xA3  022DDD00 LBL1
022DD3E8 EA000244 b       22DDD00h          //0xA4  022DDD00 LBL1
022DD3EC EA000243 b       22DDD00h          //0xA5  022DDD00 LBL1
022DD3F0 EA000242 b       22DDD00h          //0xA6  022DDD00 LBL1
022DD3F4 EA000241 b       22DDD00h          //0xA7  022DDD00 LBL1
022DD3F8 EA000240 b       22DDD00h          //0xA8  022DDD00 LBL1
022DD3FC EA0000A1 b       22DD688h          //0xA9  022DD688
022DD400 EA000162 b       22DD990h          //0xAA  022DD990
022DD404 EA000161 b       22DD990h          //0xAB  022DD990
022DD408 EA000108 b       22DD830h          //0xAC  022DD830
022DD40C EA000107 b       22DD830h          //0xAD  022DD830
022DD410 EA00009C b       22DD688h          //0xAE  022DD688
022DD414 E59F08EC ldr     r0,=12Fh  
022DD418 E1570000 cmp     r7,r0
022DD41C 0A000048 beq     22DD544h
022DD420 EA000236 b       22DDD00h          ///022DDD00 LBL1
022DD424 E1D600B2 ldrh    r0,[r6,2h]    
022DD428 EB001D1F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DD42C E28D1008 add     r1,r13,8h
022DD430 EB00252B bl      22E68E4h
022DD434 E3500000 cmp     r0,0h
022DD438 0A000230 beq     22DDD00h          ///022DDD00 LBL1
022DD43C E59D2008 ldr     r2,[r13,8h]
022DD440 E59F18C4 ldr     r1,=2319260h          //"ScriptData_Command_MENU result %d"
022DD444 E3A00002 mov     r0,2h
022DD448 EBF4BB7C bl      200C240h              //DebugPrint
022DD44C E59D1008 ldr     r1,[r13,8h]
022DD450 E1A00004 mov     r0,r4
022DD454 EB001BE3 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022DD458 E584001C str     r0,[r4,1Ch]
022DD45C E3A00002 mov     r0,2h
022DD460 E1C400BE strh    r0,[r4,0Eh]
022DD464 EA000225 b       22DDD00h          ///022DDD00 LBL1
///******* LastOpcode == 0xCC ProcessSpecial *******
022DD468 E1D600B2 ldrh    r0,[r6,2h]        //Grab Parameter1
022DD46C EB001D0E bl      22E48ACh          //Turn 16bits signed into 14bits unsigned
022DD470 E1A08000 mov     r8,r0             //R8 = Parameter1_14b
022DD474 E1D600B4 ldrh    r0,[r6,4h]        //Grab Parameter2
022DD478 EB001D0B bl      22E48ACh          //Turn 16bits signed into 14bits unsigned
022DD47C E1A07000 mov     r7,r0             //R7 = Parameter2_14b
022DD480 E1D600B6 ldrh    r0,[r6,6h]        //Grab Parameter3
022DD484 EB001D08 bl      22E48ACh          //Turn 16bits signed into 14bits unsigned
022DD488 E1A05000 mov     r5,r0             //R5 = Parameter3_14b
022DD48C E58D5000 str     r5,[r13]          //Put Parameter3_14b on the stack
022DD490 E3A00002 mov     r0,2h                     
022DD494 E59F1874 ldr     r1,=2319284h      //"ScriptData_Command_ProcessSpecial %3d %3d %3d"
022DD498 E1A02008 mov     r2,r8             
022DD49C E1A03007 mov     r3,r7
022DD4A0 EBF4BB66 bl      200C240h              //DebugPrint
022DD4A4 E1A02007 mov     r2,r7
022DD4A8 E1A03005 mov     r3,r5
022DD4AC E1A00004 mov     r0,r4                 //R0 = ScriptEngineStruct
022DD4B0 E20810FF and     r1,r8,0FFh
022DD4B4 EB002717 bl      22E7118h              ///022E7118
{///022E7118
    022E7118 E92D4FF8 push    r3-r11,r14
    022E711C E24DDF6E sub     r13,r13,1B8h
    022E7120 E1A05001 mov     r5,r1             //R5 = ProcessSpecial_Param1_14b
    022E7124 E1A04000 mov     r4,r0             //R4 = ScriptEngineStruct
    022E7128 E1A07002 mov     r7,r2             //R7 = ProcessSpecial_Param2_14b
    022E712C E59F1994 ldr     r1,=2319A48h      //"script special process call %3d"
    022E7130 E1A02005 mov     r2,r5             //R2 = ProcessSpecial_Param1_14b
    022E7134 E3A00002 mov     r0,2h             //R0 = 2
    022E7138 E1A06003 mov     r6,r3             //R6 = ProcessSpecial_Param3_14b
    022E713C EBF4943F bl      200C240h          //DebugPrint
    022E7140 E355003E cmp     r5,3Eh            
    if( ProcessSpecial_Param1_14b <= 0x3E )
        022E7144 908FF105 addls   r15,r15,r5,lsl 2h     //R15 = 0022E7148 + (ProcessSpecial_Param1_14b * 4) + 4
    022E7148 EA00025B b       22E7ABCh          ///022E7ABC Return0
    022E714C EA00025A b       22E7ABCh          ///0x00 022E7ABC Return0
    022E7150 EA00003C b       22E7248h          ///0x01 022E7248
    022E7154 EA00003E b       22E7254h          //0x02
    022E7158 EA000040 b       22E7260h          //0x03
    022E715C EA000042 b       22E726Ch          //0x04
    022E7160 EA000044 b       22E7278h          //0x05
    022E7164 EA000046 b       22E7284h          //0x06    
    022E7168 EA00004F b       22E72ACh          //0x07
    022E716C EA000059 b       22E72D8h          //0x08
    022E7170 EA000067 b       22E7314h          //0x09
    022E7174 EA00007A b       22E7364h          //0x0A
    022E7178 EA00007F b       22E737Ch          //0x0B
    022E717C EA00008E b       22E73BCh          //0x0C
    022E7180 EA000094 b       22E73D8h          //0x0D
    022E7184 EA000095 b       22E73E0h          //0x0E
    022E7188 EA00009A b       22E73F8h          //0x0F
    022E718C EA00009B b       22E7400h          //0x10
    022E7190 EA00009C b       22E7408h          //0x11
    022E7194 EA00009D b       22E7410h          //0x12
    022E7198 EA0000EA b       22E7548h          //0x13
    022E719C EA0000E4 b       22E7534h          //0x14
    022E71A0 EA0000E8 b       22E7548h          //0x15
    022E71A4 EA0000DB b       22E7518h          //0x16
    022E71A8 EA000108 b       22E75D0h          //0x17
    022E71AC EA00010A b       22E75DCh              //0x18 
    022E71B0 EA000109 b       22E75DCh          //0x19
    022E71B4 EA00010C b       22E75ECh          //0x1A
    022E71B8 EA00010F b       22E75FCh          //0x1B
    022E71BC EA000112 b       22E760Ch          //0x1C
    022E71C0 EA00011A b       22E7630h          //0x1D
    022E71C4 EA000125 b       22E7660h          //0x1E
    022E71C8 EA00012C b       22E7680h          //0x1F
    022E71CC EA00013D b       22E76C8h          //0x20
    022E71D0 EA000140 b       22E76D8h          //0x21
    022E71D4 EA000146 b       22E76F4h          //0x22
    022E71D8 EA00014A b       22E7708h          //0x23
    022E71DC EA000150 b       22E7724h          //0x24
    022E71E0 EA000157 b       22E7744h          //0x25
    022E71E4 EA000160 b       22E776Ch          //0x26
    022E71E8 EA000167 b       22E778Ch          //0x27
    022E71EC EA000170 b       22E77B4h          //0x28
    022E71F0 EA000181 b       22E77FCh          //0x29
    022E71F4 EA000195 b       22E7850h          //0x2A
    022E71F8 EA00019C b       22E7870h          //0x2B
    022E71FC EA0001A5 b       22E7898h          //0x2C
    022E7200 EA0001A9 b       22E78ACh          //0x2D
    022E7204 EA0001AF b       22E78C8h          //0x2E
    022E7208 EA0001B5 b       22E78E4h          //0x2F
    022E720C EA0001C9 b       22E7938h          //0x30
    022E7210 EA0001DD b       22E798Ch          //0x31
    022E7214 EA0001ED b       22E79D0h          //0x32
    022E7218 EA0001F2 b       22E79E8h          //0x33
    022E721C EA0001F4 b       22E79F4h          //0x34
    022E7220 EA0001F6 b       22E7A00h          //0x35
    022E7224 EA0001F7 b       22E7A08h          //0x36
    022E7228 EA0001F9 b       22E7A14h          //0x37
    022E722C EA000205 b       22E7A48h          //0x38
    022E7230 EA000209 b       22E7A5Ch          //0x39
    022E7234 EA00020D b       22E7A70h          //0x3A
    022E7238 EA00020F b       22E7A7Ch          //0x3B
    022E723C EA000211 b       22E7A88h          //0x3C
    022E7240 EA000213 b       22E7A94h          //0x3D
    022E7244 EA000217 b       22E7AA8h          //0x3E
    //0x0
    022E7248 EB000871 bl      22E9414h          ///022E9414
    {
        022E9414 E92D4008 push    r3,r14
        022E9418 E59F0014 ldr     r0,=23209ECh  //"GroundMain return dungeon\n"
        022E941C EBF48B76 bl      200C1FCh
        022E9420 E3A00000 mov     r0,0h
        022E9424 E3A0105C mov     r1,5Ch
        022E9428 EBF58801 bl      204B434h
        022E942C EB000001 bl      22E9438h
        022E9430 E8BD8008 pop     r3,r15
        022E9434 023209EC 
    }
    022E724C E3A00000 mov     r0,0h
    022E7250 EA00021A b       22E7AC0h          //022E7AC0 Return
    022E7254 EBF5855A bl      20487C4h          //020487C4
    022E7258 E3A00000 mov     r0,0h
    022E725C EA000217 b       22E7AC0h          //022E7AC0 Return
    022E7260 EBF585E9 bl      2048A0Ch
    022E7264 E3A00000 mov     r0,0h
    022E7268 EA000214 b       22E7AC0h          //022E7AC0 Return
    022E726C EBF58604 bl      2048A84h
    022E7270 E3A00000 mov     r0,0h
    022E7274 EA000211 b       22E7AC0h          //022E7AC0 Return
    022E7278 EB00086E bl      22E9438h
    022E727C E3A00000 mov     r0,0h
    022E7280 EA00020E b       22E7AC0h          //022E7AC0 Return
    022E7284 EBF59D20 bl      204E70Ch
    022E7288 E3500001 cmp     r0,1h
    022E728C 1A000004 bne     22E72A4h
    022E7290 EBF59D36 bl      204E770h
    022E7294 EBF5DEDD bl      205EE10h
    022E7298 E3500000 cmp     r0,0h
    022E729C C3A00001 movgt   r0,1h
    022E72A0 CA000206 bgt     22E7AC0h          //022E7AC0 Return
    022E72A4 E3A00000 mov     r0,0h
    022E72A8 EA000204 b       22E7AC0h          //022E7AC0 Return
    022E72AC E28D0007 add     r0,r13,7h
    022E72B0 EBF59FA8 bl      204F158h
    022E72B4 E3500000 cmp     r0,0h
    022E72B8 0A000004 beq     22E72D0h
    022E72BC E5DD0007 ldrb    r0,[r13,7h]
    022E72C0 EBF5DED2 bl      205EE10h
    022E72C4 E3500000 cmp     r0,0h
    022E72C8 C3A00001 movgt   r0,1h
    022E72CC CA0001FB bgt     22E7AC0h          //022E7AC0 Return
    022E72D0 E3A00000 mov     r0,0h
    022E72D4 EA0001F9 b       22E7AC0h          //022E7AC0 Return
    022E72D8 E28D0006 add     r0,r13,6h
    022E72DC EBF59F9D bl      204F158h
    022E72E0 E3500000 cmp     r0,0h
    022E72E4 0A000008 beq     22E730Ch
    022E72E8 E5DD2006 ldrb    r2,[r13,6h]
    022E72EC E3A03004 mov     r3,4h
    022E72F0 E28D1005 add     r1,r13,5h
    022E72F4 E3A00006 mov     r0,6h
    022E72F8 E5CD3005 strb    r3,[r13,5h]
    022E72FC EBF5E058 bl      205F464h
    022E7300 E3500000 cmp     r0,0h
    022E7304 13A00001 movne   r0,1h
    022E7308 1A0001EC bne     22E7AC0h          //022E7AC0 Return
    022E730C E3A00000 mov     r0,0h
    022E7310 EA0001EA b       22E7AC0h          //022E7AC0 Return
    022E7314 E28D0004 add     r0,r13,4h
    022E7318 EBF59F8E bl      204F158h
    022E731C E3500000 cmp     r0,0h
    022E7320 0A00000D beq     22E735Ch
    022E7324 E5DD2004 ldrb    r2,[r13,4h]
    022E7328 E3A03004 mov     r3,4h
    022E732C E28D1003 add     r1,r13,3h
    022E7330 E3A00006 mov     r0,6h
    022E7334 E5CD3003 strb    r3,[r13,3h]
    022E7338 EBF5E049 bl      205F464h
    022E733C E1B01000 movs    r1,r0
    022E7340 0A000005 beq     22E735Ch
    022E7344 E5DD2004 ldrb    r2,[r13,4h]
    022E7348 E28D0060 add     r0,r13,60h
    022E734C EBF5DF71 bl      205F118h
    022E7350 E3500000 cmp     r0,0h
    022E7354 13A00001 movne   r0,1h
    022E7358 1A0001D8 bne     22E7AC0h          //022E7AC0 Return
    022E735C E3A00000 mov     r0,0h
    022E7360 EA0001D6 b       22E7AC0h          //022E7AC0 Return
    022E7364 E3A00002 mov     r0,2h
    022E7368 E3A01000 mov     r1,0h
    022E736C EBF5D182 bl      205B97Ch
    022E7370 E3500000 cmp     r0,0h
    022E7374 D3A00000 movle   r0,0h
    022E7378 EA0001D0 b       22E7AC0h          //022E7AC0 Return
    022E737C E28D0002 add     r0,r13,2h
    022E7380 EBF59F8B bl      204F1B4h
    022E7384 E3500000 cmp     r0,0h
    022E7388 0A000009 beq     22E73B4h
    022E738C E1DD00D2 ldrsb   r0,[r13,2h]
    022E7390 EBF5D0F9 bl      205B77Ch
    022E7394 E1D04ADC ldrsb   r4,[r0,0ACh]
    022E7398 E1DD10D2 ldrsb   r1,[r13,2h]
    022E739C E59F0728 ldr     r0,=2319A68h
    022E73A0 E1A02004 mov     r2,r4
    022E73A4 EBF49394 bl      200C1FCh
    022E73A8 E3540000 cmp     r4,0h
    022E73AC C1A00004 movgt   r0,r4
    022E73B0 CA0001C2 bgt     22E7AC0h          //022E7AC0 Return
    022E73B4 E3A00000 mov     r0,0h
    022E73B8 EA0001C0 b       22E7AC0h          //022E7AC0 Return
    022E73BC E1A00807 mov     r0,r7,lsl 10h
    022E73C0 E1A01806 mov     r1,r6,lsl 10h
    022E73C4 E1A00840 mov     r0,r0,asr 10h
    022E73C8 E1A01841 mov     r1,r1,asr 10h
    022E73CC EB000354 bl      22E8124h
    022E73D0 E3A00000 mov     r0,0h
    022E73D4 EA0001B9 b       22E7AC0h          //022E7AC0 Return
    022E73D8 EB000377 bl      22E81BCh
    022E73DC EA0001B7 b       22E7AC0h          //022E7AC0 Return
    022E73E0 EB00037B bl      22E81D4h
    022E73E4 E20010FF and     r1,r0,0FFh
    022E73E8 E5CD0000 strb    r0,[r13]
    022E73EC E1A00001 mov     r0,r1
    022E73F0 E5CD1001 strb    r1,[r13,1h]
    022E73F4 EA0001B1 b       22E7AC0h          //022E7AC0 Return
    022E73F8 EB00037C bl      22E81F0h
    022E73FC EA0001AF b       22E7AC0h          //022E7AC0 Return
    022E7400 EB000394 bl      22E8258h
    022E7404 EA0001AD b       22E7AC0h          //022E7AC0 Return
    022E7408 E3A00000 mov     r0,0h
    022E740C EA0001AB b       22E7AC0h          //022E7AC0 Return
    022E7410 E28D100C add     r1,r13,0Ch
    022E7414 E3A00000 mov     r0,0h
    022E7418 EBF47B73 bl      20061ECh
    022E741C E28D100A add     r1,r13,0Ah
    022E7420 E3A00000 mov     r0,0h
    022E7424 EBF47B8C bl      200625Ch
    022E7428 E1DD00BA ldrh    r0,[r13,0Ah]
    022E742C E310000C tst     r0,0Ch
    022E7430 13A00000 movne   r0,0h
    022E7434 1A0001A1 bne     22E7AC0h          //022E7AC0 Return
    022E7438 E1DD00BC ldrh    r0,[r13,0Ch]
    022E743C EB000ECB bl      22EAF70h
    022E7440 E3E01000 mvn     r1,0h
    022E7444 E1500001 cmp     r0,r1
    022E7448 0A000030 beq     22E7510h
    022E744C E28D1058 add     r1,r13,58h
    022E7450 E3A02C01 mov     r2,100h
    022E7454 EBF4780E bl      2005494h
    022E7458 E1DD00BC ldrh    r0,[r13,0Ch]
    022E745C E3A09000 mov     r9,0h
    022E7460 E28DB048 add     r11,r13,48h
    022E7464 E3100002 tst     r0,2h
    022E7468 E59F0660 ldr     r0,=2319A38h
    022E746C 13A0A004 movne   r10,4h
    022E7470 E5908000 ldr     r8,[r0]
    022E7474 E5902004 ldr     r2,[r0,4h]
    022E7478 E5901008 ldr     r1,[r0,8h]
    022E747C E590700C ldr     r7,[r0,0Ch]
    022E7480 03A0A002 moveq   r10,2h
    022E7484 E58D8018 str     r8,[r13,18h]
    022E7488 E58D201C str     r2,[r13,1Ch]
    022E748C E58D1010 str     r1,[r13,10h]
    022E7490 E58D7014 str     r7,[r13,14h]
    022E7494 E28D5050 add     r5,r13,50h
    022E7498 E28D6058 add     r6,r13,58h
    022E749C EA000019 b       22E7508h
    022E74A0 E5941000 ldr     r1,[r4]
    022E74A4 E5940004 ldr     r0,[r4,4h]
    022E74A8 E5912044 ldr     r2,[r1,44h]
    022E74AC E1A01006 mov     r1,r6
    022E74B0 E12FFF32 blx     r2
    022E74B4 E3500000 cmp     r0,0h
    022E74B8 0A000011 beq     22E7504h
    022E74BC E59D005C ldr     r0,[r13,5Ch]
    022E74C0 E58D8050 str     r8,[r13,50h]
    022E74C4 E58D0054 str     r0,[r13,54h]
    022E74C8 E5941000 ldr     r1,[r4]
    022E74CC E5940004 ldr     r0,[r4,4h]
    022E74D0 E5912044 ldr     r2,[r1,44h]
    022E74D4 E1A01005 mov     r1,r5
    022E74D8 E12FFF32 blx     r2
    022E74DC E3500000 cmp     r0,0h
    022E74E0 0A000007 beq     22E7504h
    022E74E4 E59D0058 ldr     r0,[r13,58h]
    022E74E8 E58D704C str     r7,[r13,4Ch]
    022E74EC E58D0048 str     r0,[r13,48h]
    022E74F0 E5941000 ldr     r1,[r4]
    022E74F4 E5940004 ldr     r0,[r4,4h]
    022E74F8 E5912044 ldr     r2,[r1,44h]
    022E74FC E1A0100B mov     r1,r11
    022E7500 E12FFF32 blx     r2
    022E7504 E2899001 add     r9,r9,1h
    022E7508 E159000A cmp     r9,r10
    022E750C BAFFFFE3 blt     22E74A0h
    022E7510 E3E00000 mvn     r0,0h
    022E7514 EA000169 b       22E7AC0h          //022E7AC0 Return
    022E7518 E3570000 cmp     r7,0h
    022E751C 13A00001 movne   r0,1h
    022E7520 03A00000 moveq   r0,0h
    022E7524 E20000FF and     r0,r0,0FFh
    022E7528 EB000871 bl      22E96F4h
    022E752C E3A00000 mov     r0,0h
    022E7530 EA000162 b       22E7AC0h          //022E7AC0 Return
    022E7534 EBF47E1A bl      2006DA4h
    022E7538 E59F1594 ldr     r1,=2324CA0h
    022E753C E3A00000 mov     r0,0h
    022E7540 E5817000 str     r7,[r1]
    022E7544 EA00015D b       22E7AC0h          //022E7AC0 Return
    022E7548 E28D1008 add     r1,r13,8h
    022E754C E3A00000 mov     r0,0h
    022E7550 EBF47B41 bl      200625Ch
    022E7554 EBF58E41 bl      204AE60h
    022E7558 E3500000 cmp     r0,0h
    022E755C E28D0068 add     r0,r13,68h
    022E7560 0A000001 beq     22E756Ch
    022E7564 EBF47DAC bl      2006C1Ch
    022E7568 EA000000 b       22E7570h
    022E756C EBF47DA2 bl      2006BFCh
    022E7570 E1DD10B8 ldrh    r1,[r13,8h]
    022E7574 E59F055C ldr     r0,=0F0Fh
    022E7578 E1110000 tst     r1,r0
    022E757C 1A000005 bne     22E7598h
    022E7580 E28D0068 add     r0,r13,68h
    022E7584 E3A01002 mov     r1,2h
    022E7588 E3A02000 mov     r2,0h
    022E758C EBF47DBE bl      2006C8Ch
    022E7590 E3500000 cmp     r0,0h
    022E7594 0A000001 beq     22E75A0h
    022E7598 E3A00002 mov     r0,2h
    022E759C EA000147 b       22E7AC0h          //022E7AC0 Return
    022E75A0 E59F052C ldr     r0,=2324CA0h
    022E75A4 E5901000 ldr     r1,[r0]
    022E75A8 E3510000 cmp     r1,0h
    022E75AC DA000003 ble     22E75C0h
    022E75B0 E2511001 subs    r1,r1,1h
    022E75B4 E5801000 str     r1,[r0]
    022E75B8 03A00001 moveq   r0,1h
    022E75BC 0A00013F beq     22E7AC0h          //022E7AC0 Return
    022E75C0 E3550013 cmp     r5,13h
    022E75C4 03E00000 mvneq   r0,0h
    022E75C8 13A00000 movne   r0,0h
    022E75CC EA00013B b       22E7AC0h          //022E7AC0 Return
    022E75D0 EBF5F99C bl      2065C48h
    022E75D4 E3A00000 mov     r0,0h
    022E75D8 EA000138 b       22E7AC0h          //022E7AC0 Return
    //0x18-0x19
    022E75DC E1A00007 mov     r0,r7
    022E75E0 EB0007FD bl      22E95DCh          //022E95DC
    {///022E95DC
        022E95DC E92D4010 push    r4,r14
        022E95E0 E59F1054 ldr     r1,=2324FA0h
        022E95E4 E1A04000 mov     r4,r0
        022E95E8 E591001C ldr     r0,[r1,1Ch]
        022E95EC E3500000 cmp     r0,0h
        if( R0 != 0 )
            022E95F0 1A00000F bne     22E9634h          //022E9634
        022E95F4 E59F1044 ldr     r1,=2320B58h          //"GroundMain game end request %3d"
        022E95F8 E1A02004 mov     r2,r4
        022E95FC E3A00001 mov     r0,1h
        022E9600 EBF48B0E bl      200C240h              //Debug Print
        022E9604 E59F0030 ldr     r0,=2324FA0h
        022E9608 E3A01006 mov     r1,6h
        022E960C E580101C str     r1,[r0,1Ch]
        022E9610 E3540000 cmp     r4,0h
        if( R4 >= 0 )
            022E9614 A3A01001 movge   r1,1h
            022E9618 A5801010 strge   r1,[r0,10h]
            022E961C A5804018 strge   r4,[r0,18h]
        else if( R4 < 0 )
            022E9620 B3A01000 movlt   r1,0h
            022E9624 B5801010 strlt   r1,[r0,10h]
            022E9628 B5801018 strlt   r1,[r0,18h]
        022E962C E3A00001 mov     r0,1h
        022E9630 E8BD8010 pop     r4,r15
        //022E9634
        022E9634 E3A00000 mov     r0,0h
        022E9638 E8BD8010 pop     r4,r15
        022E963C 02324FA0
        022E9640 02320B58
    }
    022E75E4 E3A00000 mov     r0,0h
    022E75E8 EA000134 b       22E7AC0h          //022E7AC0 Return
    022E75EC E3E00000 mvn     r0,0h
    022E75F0 EB0007F9 bl      22E95DCh
    022E75F4 E3A00000 mov     r0,0h
    022E75F8 EA000130 b       22E7AC0h          //022E7AC0 Return
    022E75FC E1A00007 mov     r0,r7
    022E7600 EB000823 bl      22E9694h
    022E7604 E3A00000 mov     r0,0h
    022E7608 EA00012C b       22E7AC0h          //022E7AC0 Return
    022E760C E1A00807 mov     r0,r7,lsl 10h
    022E7610 E1A00840 mov     r0,r0,asr 10h
    022E7614 EB000288 bl      22E803Ch
    022E7618 E3A01000 mov     r1,0h
    022E761C EBF5B6C9 bl      2055148h
    022E7620 E3500000 cmp     r0,0h
    022E7624 13A00001 movne   r0,1h
    022E7628 03A00000 moveq   r0,0h
    022E762C EA000123 b       22E7AC0h          //022E7AC0 Return
    022E7630 E1A00807 mov     r0,r7,lsl 10h
    022E7634 E1A00840 mov     r0,r0,asr 10h
    022E7638 EB00027F bl      22E803Ch
    022E763C E3A01000 mov     r1,0h
    022E7640 EBF5B7E2 bl      20555D0h
    022E7644 E3E01000 mvn     r1,0h
    022E7648 E1500001 cmp     r0,r1
    022E764C 03A00000 moveq   r0,0h
    022E7650 0A00011A beq     22E7AC0h          //022E7AC0 Return
    022E7654 EBF5B7D3 bl      20555A8h
    022E7658 E5D00001 ldrb    r0,[r0,1h]
    022E765C EA000117 b       22E7AC0h          //022E7AC0 Return
    022E7660 E1A00807 mov     r0,r7,lsl 10h
    022E7664 E1A00840 mov     r0,r0,asr 10h
    022E7668 EB000273 bl      22E803Ch
    022E766C EBF5BE11 bl      2056EB8h
    022E7670 E3500000 cmp     r0,0h
    022E7674 13A00001 movne   r0,1h
    022E7678 03A00000 moveq   r0,0h
    022E767C EA00010F b       22E7AC0h          //022E7AC0 Return
    022E7680 E1A00807 mov     r0,r7,lsl 10h
    022E7684 E1A00840 mov     r0,r0,asr 10h
    022E7688 EB00027C bl      22E8080h
    022E768C E28D0F5D add     r0,r13,174h
    022E7690 EB00027E bl      22E8090h
    022E7694 E28D0C01 add     r0,r13,100h
    022E7698 E1D007F8 ldrsh   r0,[r0,78h]
    022E769C EBF5F949 bl      2065BC8h
    022E76A0 E28D0C01 add     r0,r13,100h
    022E76A4 E28000AE add     r0,r0,0AEh
    022E76A8 E3A0100A mov     r1,0Ah
    022E76AC EBF5F955 bl      2065C08h
    022E76B0 E28D0F5D add     r0,r13,174h
    022E76B4 EBF5B984 bl      2055CCCh
    022E76B8 E3500000 cmp     r0,0h
    022E76BC A3A00001 movge   r0,1h
    022E76C0 B3A00000 movlt   r0,0h
    022E76C4 EA0000FD b       22E7AC0h          //022E7AC0 Return
    022E76C8 E1A00807 mov     r0,r7,lsl 10h
    022E76CC E1A00840 mov     r0,r0,asr 10h
    022E76D0 EBF49DEC bl      200EE88h
    022E76D4 EA0000F9 b       22E7AC0h          //022E7AC0 Return
    022E76D8 E1A00807 mov     r0,r7,lsl 10h
    022E76DC E28D1044 add     r1,r13,44h
    022E76E0 E1A00840 mov     r0,r0,asr 10h
    022E76E4 EBF5F983 bl      2065CF8h
    022E76E8 E1DD04F4 ldrsh   r0,[r13,44h]
    022E76EC EBF49DE5 bl      200EE88h
    022E76F0 EA0000F2 b       22E7AC0h          //022E7AC0 Return
    022E76F4 E28D0040 add     r0,r13,40h
    022E76F8 E1CD74B0 strh    r7,[r13,40h]
    022E76FC E1CD64B2 strh    r6,[r13,42h]
    022E7700 EBF4A1F7 bl      200FEE4h
    022E7704 EA0000ED b       22E7AC0h          //022E7AC0 Return
    022E7708 E1A00807 mov     r0,r7,lsl 10h
    022E770C E28D103C add     r1,r13,3Ch
    022E7710 E1A00840 mov     r0,r0,asr 10h
    022E7714 EBF5F977 bl      2065CF8h
    022E7718 E28D003C add     r0,r13,3Ch
    022E771C EBF4A1F0 bl      200FEE4h
    022E7720 EA0000E6 b       22E7AC0h          //022E7AC0 Return
    022E7724 E28D0038 add     r0,r13,38h
    022E7728 E1CD73B8 strh    r7,[r13,38h]
    022E772C E1CD63BA strh    r6,[r13,3Ah]
    022E7730 EBF4A045 bl      200F84Ch
    022E7734 E3500000 cmp     r0,0h
    022E7738 13A00001 movne   r0,1h
    022E773C 03A00000 moveq   r0,0h
    022E7740 EA0000DE b       22E7AC0h          //022E7AC0 Return
    022E7744 E1A00807 mov     r0,r7,lsl 10h
    022E7748 E28D1034 add     r1,r13,34h
    022E774C E1A00840 mov     r0,r0,asr 10h
    022E7750 EBF5F968 bl      2065CF8h
    022E7754 E28D0034 add     r0,r13,34h
    022E7758 EBF4A03B bl      200F84Ch
    022E775C E3500000 cmp     r0,0h
    022E7760 13A00001 movne   r0,1h
    022E7764 03A00000 moveq   r0,0h
    022E7768 EA0000D4 b       22E7AC0h          //022E7AC0 Return
    022E776C E28D0030 add     r0,r13,30h
    022E7770 E1CD73B0 strh    r7,[r13,30h]
    022E7774 E1CD63B2 strh    r6,[r13,32h]
    022E7778 EBF4A2E7 bl      201031Ch
    022E777C E3500000 cmp     r0,0h
    022E7780 13A00001 movne   r0,1h
    022E7784 03A00000 moveq   r0,0h
    022E7788 EA0000CC b       22E7AC0h          //022E7AC0 Return
    022E778C E1A00807 mov     r0,r7,lsl 10h
    022E7790 E28D102C add     r1,r13,2Ch
    022E7794 E1A00840 mov     r0,r0,asr 10h
    022E7798 EBF5F956 bl      2065CF8h
    022E779C E28D002C add     r0,r13,2Ch
    022E77A0 EBF4A2DD bl      201031Ch
    022E77A4 E3500000 cmp     r0,0h
    022E77A8 13A00001 movne   r0,1h
    022E77AC 03A00000 moveq   r0,0h
    022E77B0 EA0000C2 b       22E7AC0h          //022E7AC0 Return
    022E77B4 E1A00807 mov     r0,r7,lsl 10h
    022E77B8 E1A00840 mov     r0,r0,asr 10h
    022E77BC EBF49EAA bl      200F26Ch
    022E77C0 E1A04000 mov     r4,r0
    022E77C4 E3E01000 mvn     r1,0h
    022E77C8 E1540001 cmp     r4,r1
    022E77CC 0A000008 beq     22E77F4h
    022E77D0 EBF49EDC bl      200F348h
    022E77D4 E5D00001 ldrb    r0,[r0,1h]
    022E77D8 E3500000 cmp     r0,0h
    022E77DC 0A000000 beq     22E77E4h
    022E77E0 EBF5C2BE bl      20582E0h
    022E77E4 E1A00004 mov     r0,r4
    022E77E8 EBF49EE8 bl      200F390h
    022E77EC E3A00001 mov     r0,1h
    022E77F0 EA0000B2 b       22E7AC0h          //022E7AC0 Return
    022E77F4 E3A00000 mov     r0,0h
    022E77F8 EA0000B0 b       22E7AC0h          //022E7AC0 Return
    022E77FC E1A00807 mov     r0,r7,lsl 10h
    022E7800 E28D1028 add     r1,r13,28h
    022E7804 E1A00840 mov     r0,r0,asr 10h
    022E7808 EBF5F93A bl      2065CF8h
    022E780C E1DD02F8 ldrsh   r0,[r13,28h]
    022E7810 EBF49E95 bl      200F26Ch
    022E7814 E1A04000 mov     r4,r0
    022E7818 E3E01000 mvn     r1,0h
    022E781C E1540001 cmp     r4,r1
    022E7820 0A000008 beq     22E7848h
    022E7824 EBF49EC7 bl      200F348h
    022E7828 E5D00001 ldrb    r0,[r0,1h]
    022E782C E3500000 cmp     r0,0h
    022E7830 0A000000 beq     22E7838h
    022E7834 EBF5C2A9 bl      20582E0h
    022E7838 E1A00004 mov     r0,r4
    022E783C EBF49ED3 bl      200F390h
    022E7840 E3A00001 mov     r0,1h
    022E7844 EA00009D b       22E7AC0h          //022E7AC0 Return
    022E7848 E3A00000 mov     r0,0h
    022E784C EA00009B b       22E7AC0h          //022E7AC0 Return
    022E7850 E28D0024 add     r0,r13,24h
    022E7854 E1CD72B4 strh    r7,[r13,24h]
    022E7858 E1CD62B6 strh    r6,[r13,26h]
    022E785C EBF4A260 bl      20101E4h
    022E7860 E3500000 cmp     r0,0h
    022E7864 13A00001 movne   r0,1h
    022E7868 03A00000 moveq   r0,0h
    022E786C EA000093 b       22E7AC0h          //022E7AC0 Return
    022E7870 E1A00807 mov     r0,r7,lsl 10h
    022E7874 E28D1020 add     r1,r13,20h
    022E7878 E1A00840 mov     r0,r0,asr 10h
    022E787C EBF5F91D bl      2065CF8h
    022E7880 E28D0020 add     r0,r13,20h
    022E7884 EBF4A256 bl      20101E4h
    022E7888 E3500000 cmp     r0,0h
    022E788C 13A00001 movne   r0,1h
    022E7890 03A00000 moveq   r0,0h
    022E7894 EA000089 b       22E7AC0h          //022E7AC0 Return
    022E7898 E1A00807 mov     r0,r7,lsl 10h
    022E789C E1A00840 mov     r0,r0,asr 10h
    022E78A0 EB0001F6 bl      22E8080h
    022E78A4 E3A00000 mov     r0,0h
    022E78A8 EA000084 b       22E7AC0h          //022E7AC0 Return
    022E78AC EBF5B7AF bl      2055770h
    022E78B0 E3500000 cmp     r0,0h
    022E78B4 03A00000 moveq   r0,0h
    022E78B8 0A000080 beq     22E7AC0h          //022E7AC0 Return
    022E78BC E1D000F4 ldrsh   r0,[r0,4h]
    022E78C0 EB0001E2 bl      22E8050h
    022E78C4 EA00007D b       22E7AC0h          //022E7AC0 Return
    022E78C8 EBF5B7B2 bl      2055798h
    022E78CC E3500000 cmp     r0,0h
    022E78D0 03A00000 moveq   r0,0h
    022E78D4 0A000079 beq     22E7AC0h          //022E7AC0 Return
    022E78D8 E1D000F4 ldrsh   r0,[r0,4h]
    022E78DC EB0001DB bl      22E8050h
    022E78E0 EA000076 b       22E7AC0h          //022E7AC0 Return
    022E78E4 EBF5B76F bl      20556A8h
    022E78E8 E3E01000 mvn     r1,0h
    022E78EC E1500001 cmp     r0,r1
    022E78F0 0A00000E beq     22E7930h
    022E78F4 EBF5BAED bl      20564B0h
    022E78F8 E3500000 cmp     r0,0h
    022E78FC 0A00000B beq     22E7930h
    022E7900 EBF5B779 bl      20556ECh
    022E7904 E3E01000 mvn     r1,0h
    022E7908 E1500001 cmp     r0,r1
    022E790C 0A000007 beq     22E7930h
    022E7910 EBF5BABE bl      2056410h
    022E7914 E3500000 cmp     r0,0h
    022E7918 0A000004 beq     22E7930h
    022E791C E3A00000 mov     r0,0h
    022E7920 EBF5BD37 bl      2056E04h
    022E7924 E3500000 cmp     r0,0h
    022E7928 03A00001 moveq   r0,1h
    022E792C 0A000063 beq     22E7AC0h          //022E7AC0 Return
    022E7930 E3A00000 mov     r0,0h
    022E7934 EA000061 b       22E7AC0h          //022E7AC0 Return
    022E7938 EBF5B76B bl      20556ECh
    022E793C E3E01000 mvn     r1,0h
    022E7940 E1500001 cmp     r0,r1
    022E7944 0A00000E beq     22E7984h
    022E7948 EBF5BAD8 bl      20564B0h
    022E794C E3500000 cmp     r0,0h
    022E7950 0A00000B beq     22E7984h
    022E7954 EBF5B753 bl      20556A8h
    022E7958 E3E01000 mvn     r1,0h
    022E795C E1500001 cmp     r0,r1
    022E7960 0A000007 beq     22E7984h
    022E7964 EBF5BAA9 bl      2056410h
    022E7968 E3500000 cmp     r0,0h
    022E796C 0A000004 beq     22E7984h
    022E7970 E3A00000 mov     r0,0h
    022E7974 EBF5BD22 bl      2056E04h
    022E7978 E3500000 cmp     r0,0h
    022E797C 03A00001 moveq   r0,1h
    022E7980 0A00004E beq     22E7AC0h          //022E7AC0 Return
    022E7984 E3A00000 mov     r0,0h
    022E7988 EA00004C b       22E7AC0h          //022E7AC0 Return
    022E798C EBF5B745 bl      20556A8h
    022E7990 E3E01000 mvn     r1,0h
    022E7994 E1500001 cmp     r0,r1
    022E7998 0A00000A beq     22E79C8h
    022E799C EBF5BA9B bl      2056410h
    022E79A0 E3500000 cmp     r0,0h
    022E79A4 0A000007 beq     22E79C8h
    022E79A8 EBF5B74F bl      20556ECh
    022E79AC E3E01000 mvn     r1,0h
    022E79B0 E1500001 cmp     r0,r1
    022E79B4 0A000003 beq     22E79C8h
    022E79B8 EBF5BA94 bl      2056410h
    022E79BC E3500000 cmp     r0,0h
    022E79C0 13A00001 movne   r0,1h
    022E79C4 1A00003D bne     22E7AC0h          //022E7AC0 Return
    022E79C8 E3A00000 mov     r0,0h
    022E79CC EA00003B b       22E7AC0h          //022E7AC0 Return
    022E79D0 E3A00000 mov     r0,0h
    022E79D4 EBF5BC91 bl      2056C20h
    022E79D8 E3500001 cmp     r0,1h
    022E79DC 03A00001 moveq   r0,1h
    022E79E0 13A00000 movne   r0,0h
    022E79E4 EA000035 b       22E7AC0h          //022E7AC0 Return
    022E79E8 EBF5BBF7 bl      20569CCh
    022E79EC E3A00000 mov     r0,0h
    022E79F0 EA000032 b       22E7AC0h          //022E7AC0 Return
    022E79F4 EBF5BC2D bl      2056AB0h
    022E79F8 E3A00000 mov     r0,0h
    022E79FC EA00002F b       22E7AC0h          //022E7AC0 Return
    022E7A00 E3A00000 mov     r0,0h
    022E7A04 EA00002D b       22E7AC0h          //022E7AC0 Return
    022E7A08 EB00B022 bl      2313A98h
    022E7A0C E3A00000 mov     r0,0h
    022E7A10 EA00002A b       22E7AC0h          //022E7AC0 Return
    022E7A14 E28D008C add     r0,r13,8Ch
    022E7A18 EBF59E09 bl      204F244h
    022E7A1C E3500000 cmp     r0,0h
    022E7A20 0A000006 beq     22E7A40h
    022E7A24 E28D008C add     r0,r13,8Ch
    022E7A28 E3570000 cmp     r7,0h
    022E7A2C A5CD70E8 strgeb  r7,[r13,0E8h]
    022E7A30 E3560000 cmp     r6,0h
    022E7A34 E3A01001 mov     r1,1h
    022E7A38 A5CD60E9 strgeb  r6,[r13,0E9h]
    022E7A3C EBF59DEB bl      204F1F0h
    022E7A40 E3A00000 mov     r0,0h
    022E7A44 EA00001D b       22E7AC0h          //022E7AC0 Return
    022E7A48 EBF49CDC bl      200EDC0h
    022E7A4C E3500000 cmp     r0,0h
    022E7A50 13A00001 movne   r0,1h
    022E7A54 03A00000 moveq   r0,0h
    022E7A58 EA000018 b       22E7AC0h          //022E7AC0 Return
    022E7A5C EBF4A0BC bl      200FD54h
    022E7A60 E3500000 cmp     r0,0h
    022E7A64 13A00001 movne   r0,1h
    022E7A68 03A00000 moveq   r0,0h
    022E7A6C EA000013 b       22E7AC0h          //022E7AC0 Return
    022E7A70 EBF5A06C bl      204FC28h
    022E7A74 E3A00000 mov     r0,0h
    022E7A78 EA000010 b       22E7AC0h          //022E7AC0 Return
    022E7A7C EBF5A111 bl      204FEC8h
    022E7A80 E3A00000 mov     r0,0h
    022E7A84 EA00000D b       22E7AC0h          //022E7AC0 Return
    022E7A88 EBF5A13C bl      204FF80h
    022E7A8C E3A00000 mov     r0,0h
    022E7A90 EA00000A b       22E7AC0h          //022E7AC0 Return
    022E7A94 E1A00807 mov     r0,r7,lsl 10h
    022E7A98 E1A00840 mov     r0,r0,asr 10h
    022E7A9C EBF5F82B bl      2065B50h
    022E7AA0 E3A00000 mov     r0,0h
    022E7AA4 EA000005 b       22E7AC0h          //022E7AC0 Return
    022E7AA8 E1A00807 mov     r0,r7,lsl 10h
    022E7AAC E1A00840 mov     r0,r0,asr 10h
    022E7AB0 EBF5F82A bl      2065B60h
    022E7AB4 E3A00000 mov     r0,0h
    022E7AB8 EA000000 b       22E7AC0h          //022E7AC0 Return
    ///022E7ABC Return0
    022E7ABC E3A00000 mov     r0,0h
    //022E7AC0 Return
    022E7AC0 E28DDF6E add     r13,r13,1B8h
    022E7AC4 E8BD8FF8 pop     r3-r11,r15
    022E7AC8 02319A48 
    022E7ACC 02319A68 
    022E7AD0 02319A38 
    022E7AD4 02324CA0 
    022E7AD8 00000F0F 
}
022DD4B8 E1A02008 mov     r2,r8
022DD4BC E1A05000 mov     r5,r0
022DD4C0 E3A00002 mov     r0,2h
022DD4C4 E59F1848 ldr     r1,=23192B4h          //"ScriptData_Command_ProcessSpecial result %3d %3d"
022DD4C8 E1A03005 mov     r3,r5
022DD4CC EBF4BB5B bl      200C240h              //DebugPrint
022DD4D0 E3550000 cmp     r5,0h
if( SpecProceReturn < 0 )
    022DD4D4 BA000209 blt     22DDD00h          ///022DDD00 LBL1
022DD4D8 E3A02002 mov     r2,2h
022DD4DC E1A00004 mov     r0,r4
022DD4E0 E1A01005 mov     r1,r5
022DD4E4 E1C420BE strh    r2,[r4,0Eh]
022DD4E8 EB001BBE bl      22E43E8h          //022E43E8 ScriptCaseProcess
022DD4EC E584001C str     r0,[r4,1Ch]
022DD4F0 EA000202 b       22DDD00h          ///022DDD00 LBL1
///022DD4F4 LBL10
022DD4F4 E59F081C ldr     r0,=2324E80h
022DD4F8 E1D000F6 ldrsh   r0,[r0,6h]
022DD4FC EB004F6F bl      22F12C0h          //GroundMap_Select
022DD500 E3A00002 mov     r0,2h
022DD504 E1C400BE strh    r0,[r4,0Eh]
022DD508 EA0001FC b       22DDD00h          ///022DDD00 LBL1
///022DD50C LBL6
022DD50C E59F0804 ldr     r0,=2324E80h
022DD510 E1D000F6 ldrsh   r0,[r0,6h]
022DD514 EB005088 bl      22F173Ch
022DD518 E3A00002 mov     r0,2h
022DD51C E1C400BE strh    r0,[r4,0Eh]
022DD520 EA0001F6 b       22DDD00h          ///022DDD00 LBL1
///022DD524 LBL14
022DD524 E1D600B2 ldrh    r0,[r6,2h]
022DD528 EB001CDF bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DD52C E1A00800 mov     r0,r0,lsl 10h
022DD530 E1A00840 mov     r0,r0,asr 10h
022DD534 EB00541A bl      22F25A4h
022DD538 E3A00002 mov     r0,2h
022DD53C E1C400BE strh    r0,[r4,0Eh]
022DD540 EA0001EE b       22DDD00h          ///022DDD00 LBL1
022DD544 E1D600B2 ldrh    r0,[r6,2h]
022DD548 EB001CD7 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DD54C E1A00800 mov     r0,r0,lsl 10h
022DD550 E3E01000 mvn     r1,0h
022DD554 E1510840 cmp     r1,r0,asr 10h
022DD558 E1A05840 mov     r5,r0,asr 10h
022DD55C 1A000004 bne     22DD574h
022DD560 E284006C add     r0,r4,6Ch
022DD564 E3A0101C mov     r1,1Ch
022DD568 EBF5B7DF bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
022DD56C E1A00800 mov     r0,r0,lsl 10h
022DD570 E1A05840 mov     r5,r0,asr 10h
022DD574 EB006E92 bl      22F8FC4h
022DD578 EB007D9C bl      22FCBF0h
022DD57C EB00835F bl      22FE300h
022DD580 E1A00005 mov     r0,r5
022DD584 EB006716 bl      22F71E4h
022DD588 E3A00002 mov     r0,2h
022DD58C E1C400BE strh    r0,[r4,0Eh]
022DD590 EA0001DA b       22DDD00h          ///022DDD00 LBL1
022DD594 E1D404F8 ldrsh   r0,[r4,48h]
022DD598 E3500000 cmp     r0,0h
022DD59C 1A00000B bne     22DD5D0h
022DD5A0 EBF55E00 bl      2034DA8h
022DD5A4 E3500000 cmp     r0,0h
022DD5A8 0A000003 beq     22DD5BCh
022DD5AC E59F1768 ldr     r1,=23192E8h          //"MESSAGE KEY WAIT is busy1"
022DD5B0 E3A00002 mov     r0,2h
022DD5B4 EBF4BB21 bl      200C240h              //DebugPrint
022DD5B8 EA0001D0 b       22DDD00h          ///022DDD00 LBL1
022DD5BC EBF55E2B bl      2034E70h
022DD5C0 E3500000 cmp     r0,0h
022DD5C4 13A00001 movne   r0,1h
022DD5C8 11C404B8 strneh  r0,[r4,48h]
022DD5CC EA0001CB b       22DDD00h          ///022DDD00 LBL1
022DD5D0 EBF55DF4 bl      2034DA8h
022DD5D4 E3500000 cmp     r0,0h
022DD5D8 E3A00002 mov     r0,2h
022DD5DC 01C400BE streqh  r0,[r4,0Eh]
022DD5E0 0A0001C6 beq     22DDD00h          ///022DDD00 LBL1
022DD5E4 E59F1734 ldr     r1,=2319304h          //"MESSAGE KEY WAIT is busy2"
022DD5E8 EBF4BB14 bl      200C240h              //DebugPrint
022DD5EC EA0001C3 b       22DDD00h          ///022DDD00 LBL1
///022DD5F0 LBL3
022DD5F0 E1D404F8 ldrsh   r0,[r4,48h]
022DD5F4 E3500000 cmp     r0,0h
022DD5F8 1A00000B bne     22DD62Ch
022DD5FC EBF55DE9 bl      2034DA8h
022DD600 E3500000 cmp     r0,0h
022DD604 0A000003 beq     22DD618h
022DD608 E59F1714 ldr     r1,=2319320h          //"MESSAGE CLOSE is busy1"
022DD60C E3A00002 mov     r0,2h
022DD610 EBF4BB0A bl      200C240h              //DebugPrint
022DD614 EA0001B9 b       22DDD00h          ///022DDD00 LBL1
022DD618 EBF55DA1 bl      2034CA4h
022DD61C E3500000 cmp     r0,0h
022DD620 13A00001 movne   r0,1h
022DD624 11C404B8 strneh  r0,[r4,48h]
022DD628 EA0001B4 b       22DDD00h          ///022DDD00 LBL1
022DD62C EBF55DDD bl      2034DA8h
022DD630 E3500000 cmp     r0,0h
022DD634 E3A00002 mov     r0,2h
022DD638 01C400BE streqh  r0,[r4,0Eh]
022DD63C 0A0001AF beq     22DDD00h          ///022DDD00 LBL1
022DD640 E59F16E0 ldr     r1,=2319338h          //"MESSAGE CLOSE is busy2"
022DD644 EBF4BAFD bl      200C240h              //DebugPrint
022DD648 EA0001AC b       22DDD00h          ///022DDD00 LBL1
022DD64C E1D404F8 ldrsh   r0,[r4,48h]
022DD650 E3500000 cmp     r0,0h
022DD654 1A000003 bne     22DD668h
022DD658 EBF55DAB bl      2034D0Ch
022DD65C E3A00001 mov     r0,1h
022DD660 E1C404B8 strh    r0,[r4,48h]
022DD664 EA0001A5 b       22DDD00h          ///022DDD00 LBL1
022DD668 EBF55DCE bl      2034DA8h
022DD66C E3500000 cmp     r0,0h
022DD670 E3A00002 mov     r0,2h
022DD674 01C400BE streqh  r0,[r4,0Eh]
022DD678 0A0001A0 beq     22DDD00h          ///022DDD00 LBL1
022DD67C E59F16A8 ldr     r1,=2319350h          //"MESSAGE CLOSE ENFORCE is busy"
022DD680 EBF4BAEE bl      200C240h              //DebugPrint
022DD684 EA00019D b       22DDD00h          ///022DDD00 LBL1
022DD688 E59F16A0 ldr     r1,=2319370h          //"MESSAGE is busy"
022DD68C E3A00002 mov     r0,2h
022DD690 EBF4BAEA bl      200C240h              //DebugPrint
022DD694 EBF55DC3 bl      2034DA8h
022DD698 E3500000 cmp     r0,0h
022DD69C 03A00002 moveq   r0,2h
022DD6A0 01C400BE streqh  r0,[r4,0Eh]
022DD6A4 EA000195 b       22DDD00h          ///022DDD00 LBL1
022DD6A8 EB008CEC bl      2300A60h
022DD6AC E3500000 cmp     r0,0h
022DD6B0 03A00002 moveq   r0,2h
022DD6B4 01C400BE streqh  r0,[r4,0Eh]
022DD6B8 EA000190 b       22DDD00h          ///022DDD00 LBL1
022DD6BC EB008C06 bl      23006DCh
022DD6C0 E3500000 cmp     r0,0h
022DD6C4 03A00002 moveq   r0,2h
022DD6C8 01C400BE streqh  r0,[r4,0Eh]
022DD6CC EA00018B b       22DDD00h          ///022DDD00 LBL1
///022DD6D0 LBL8
022DD6D0 E59F065C ldr     r0,=113h
022DD6D4 E59F163C ldr     r1,=2324E80h
022DD6D8 E1C100B6 strh    r0,[r1,6h]
022DD6DC EB004EF7 bl      22F12C0h          //GroundMap_Select
022DD6E0 E3A00003 mov     r0,3h
022DD6E4 EB005302 bl      22F22F4h
022DD6E8 E1D414F8 ldrsh   r1,[r4,48h]
022DD6EC E3A00004 mov     r0,4h
022DD6F0 E2811001 add     r1,r1,1h
022DD6F4 E1C414B8 strh    r1,[r4,48h]
022DD6F8 E1C400BE strh    r0,[r4,0Eh]
022DD6FC EA00017F b       22DDD00h          ///022DDD00 LBL1
///022DD700 LBL9
022DD700 E1D600B2 ldrh    r0,[r6,2h]
022DD704 EB001C68 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DD708 E1A07000 mov     r7,r0
022DD70C E1D600B4 ldrh    r0,[r6,4h]
022DD710 EB001C65 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DD714 E1A05000 mov     r5,r0
022DD718 E1D600B6 ldrh    r0,[r6,6h]
022DD71C EB001C62 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DD720 E1A08000 mov     r8,r0
022DD724 E1D600B8 ldrh    r0,[r6,8h]
022DD728 EB001C5F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DD72C E59F15E4 ldr     r1,=2324E80h
022DD730 E1A06000 mov     r6,r0
022DD734 E1C170B6 strh    r7,[r1,6h]
022DD738 E1D100F6 ldrsh   r0,[r1,6h]
022DD73C EB004EDF bl      22F12C0h                  //GroundMap_Select
022DD740 E3A00007 mov     r0,7h
022DD744 EB0052EA bl      22F22F4h
022DD748 E3550000 cmp     r5,0h
022DD74C 0A000001 beq     22DD758h
022DD750 E3A00004 mov     r0,4h
022DD754 EB005193 bl      22F1DA8h
022DD758 E1A00808 mov     r0,r8,lsl 10h
022DD75C E1A00820 mov     r0,r0,lsr 10h
022DD760 EB001C5E bl      22E48E0h
022DD764 E1A01806 mov     r1,r6,lsl 10h
022DD768 E58D000C str     r0,[r13,0Ch]
022DD76C E1A00821 mov     r0,r1,lsr 10h
022DD770 EB001C5A bl      22E48E0h
022DD774 E58D0010 str     r0,[r13,10h]
022DD778 E28D200C add     r2,r13,0Ch
022DD77C E3A00000 mov     r0,0h
022DD780 E3A01001 mov     r1,1h
022DD784 EB003656 bl      22EB0E4h
022DD788 E1D414F8 ldrsh   r1,[r4,48h]
022DD78C E3A00004 mov     r0,4h
022DD790 E2811001 add     r1,r1,1h
022DD794 E1C414B8 strh    r1,[r4,48h]
022DD798 E1C400BE strh    r0,[r4,0Eh]
022DD79C EA000157 b       22DDD00h          ///022DDD00 LBL1
///022DD7A0 LBL11
022DD7A0 E59F058C ldr     r0,=113h
022DD7A4 E59F156C ldr     r1,=2324E80h
022DD7A8 E1C100B6 strh    r0,[r1,6h]
022DD7AC EB004EC3 bl      22F12C0h          //GroundMap_Select
022DD7B0 E3A00004 mov     r0,4h
022DD7B4 EB0052CE bl      22F22F4h
022DD7B8 E1D414F8 ldrsh   r1,[r4,48h]
022DD7BC E3A00004 mov     r0,4h
022DD7C0 E2811001 add     r1,r1,1h
022DD7C4 E1C414B8 strh    r1,[r4,48h]
022DD7C8 E1C400BE strh    r0,[r4,0Eh]
022DD7CC EA00014B b       22DDD00h          ///022DDD00 LBL1
///022DD7D0 LBL12
022DD7D0 E59F0560 ldr     r0,=132h
022DD7D4 E59F153C ldr     r1,=2324E80h
022DD7D8 E1C100B6 strh    r0,[r1,6h]
022DD7DC EB004EB7 bl      22F12C0h          //GroundMap_Select
022DD7E0 E3A00005 mov     r0,5h
022DD7E4 EB0052C2 bl      22F22F4h
022DD7E8 E1D414F8 ldrsh   r1,[r4,48h]
022DD7EC E3A00004 mov     r0,4h
022DD7F0 E2811001 add     r1,r1,1h
022DD7F4 E1C414B8 strh    r1,[r4,48h]
022DD7F8 E1C400BE strh    r0,[r4,0Eh]
022DD7FC EA00013F b       22DDD00h          ///022DDD00 LBL1
///022DD800 LBL13
022DD800 E59F052C ldr     r0,=113h
022DD804 E59F150C ldr     r1,=2324E80h
022DD808 E1C100B6 strh    r0,[r1,6h]
022DD80C EB004EAB bl      22F12C0h          //GroundMap_Select
022DD810 E3A00006 mov     r0,6h
022DD814 EB0052B6 bl      22F22F4h
022DD818 E1D414F8 ldrsh   r1,[r4,48h]
022DD81C E3A00004 mov     r0,4h
022DD820 E2811001 add     r1,r1,1h
022DD824 E1C414B8 strh    r1,[r4,48h]
022DD828 E1C400BE strh    r0,[r4,0Eh]
022DD82C EA000133 b       22DDD00h          ///022DDD00 LBL1
022DD830 E1D404F8 ldrsh   r0,[r4,48h]
022DD834 E594601C ldr     r6,[r4,1Ch]
022DD838 E3500000 cmp     r0,0h
022DD83C 0A000007 beq     22DD860h
022DD840 EBF55D58 bl      2034DA8h
022DD844 E3500000 cmp     r0,0h
022DD848 03A00000 moveq   r0,0h
022DD84C 01C404B8 streqh  r0,[r4,48h]
022DD850 0A000002 beq     22DD860h
022DD854 E59F14E0 ldr     r1,=2319380h          //"SWITCH MESSAGE is busy"
022DD858 E3A00002 mov     r0,2h
022DD85C EBF4BA77 bl      200C240h              //DebugPrint
022DD860 E1D404F8 ldrsh   r0,[r4,48h]
022DD864 E3500000 cmp     r0,0h
022DD868 1A000124 bne     22DDD00h          ///022DDD00 LBL1
022DD86C E1D404F4 ldrsh   r0,[r4,44h]
022DD870 E3A05000 mov     r5,0h
022DD874 E3500000 cmp     r0,0h
022DD878 BA000020 blt     22DD900h
022DD87C EA00000B b       22DD8B0h
022DD880 E1D600B2 ldrh    r0,[r6,2h]
022DD884 E2868002 add     r8,r6,2h
022DD888 EB001C07 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DD88C E1D414F4 ldrsh   r1,[r4,44h]
022DD890 E2866006 add     r6,r6,6h
022DD894 E1500001 cmp     r0,r1
022DD898 1A000004 bne     22DD8B0h
022DD89C E1D810B2 ldrh    r1,[r8,2h]
022DD8A0 E2840014 add     r0,r4,14h
022DD8A4 EB001A67 bl      22E4248h
022DD8A8 E1A05000 mov     r5,r0
022DD8AC EA000002 b       22DD8BCh
022DD8B0 E1D600B0 ldrh    r0,[r6]
022DD8B4 E3500067 cmp     r0,67h
022DD8B8 0AFFFFF0 beq     22DD880h
022DD8BC E584601C str     r6,[r4,1Ch]
022DD8C0 E3550000 cmp     r5,0h
022DD8C4 1A00001B bne     22DD938h
022DD8C8 EA000000 b       22DD8D0h
022DD8CC E2866006 add     r6,r6,6h
022DD8D0 E1D600B0 ldrh    r0,[r6]
022DD8D4 E3500067 cmp     r0,67h
022DD8D8 0AFFFFFB beq     22DD8CCh
022DD8DC EA000000 b       22DD8E4h
022DD8E0 E2866004 add     r6,r6,4h
022DD8E4 E1D600B0 ldrh    r0,[r6]
022DD8E8 E350006E cmp     r0,6Eh
022DD8EC 0AFFFFFB beq     22DD8E0h
022DD8F0 E584601C str     r6,[r4,1Ch]
022DD8F4 E3A00002 mov     r0,2h
022DD8F8 E1C400BE strh    r0,[r4,0Eh]
022DD8FC EA0000FF b       22DDD00h              ///022DDD00 LBL1
022DD900 E1D600B0 ldrh    r0,[r6]
022DD904 E350006E cmp     r0,6Eh
022DD908 1A000006 bne     22DD928h
022DD90C E2860004 add     r0,r6,4h
022DD910 E584001C str     r0,[r4,1Ch]
022DD914 E1D610B2 ldrh    r1,[r6,2h]
022DD918 E2840014 add     r0,r4,14h
022DD91C EB001A49 bl      22E4248h
022DD920 E1A05000 mov     r5,r0
022DD924 EA000003 b       22DD938h
022DD928 E584601C str     r6,[r4,1Ch]
022DD92C E3A00002 mov     r0,2h
022DD930 E1C400BE strh    r0,[r4,0Eh]
022DD934 EA0000F1 b       22DDD00h              ///022DDD00 LBL1
022DD938 E59F1400 ldr     r1,=2319398h          //"SWTICH MESSAGE set"
022DD93C E3A00002 mov     r0,2h
022DD940 EBF4BA3E bl      200C240h              //DebugPrint
022DD944 E59F03F8 ldr     r0,=2324EA4h          
022DD948 EBF55D36 bl      2034E28h
022DD94C E59F03C4 ldr     r0,=2324E80h
022DD950 E59F23F0 ldr     r2,=2324ECCh
022DD954 E590000C ldr     r0,[r0,0Ch]
022DD958 E3500000 cmp     r0,0h
022DD95C A3A00020 movge   r0,20h
022DD960 B3A00000 movlt   r0,0h
022DD964 E35700AC cmp     r7,0ACh
022DD968 03A01008 moveq   r1,8h
022DD96C 159F13D8 ldrne   r1,=3008h
022DD970 E1800001 orr     r0,r0,r1
022DD974 E1A00800 mov     r0,r0,lsl 10h
022DD978 E1A01005 mov     r1,r5
022DD97C E1A00820 mov     r0,r0,lsr 10h
022DD980 EBF55D66 bl      2034F20h
022DD984 E3A00001 mov     r0,1h
022DD988 E1C404B8 strh    r0,[r4,48h]
022DD98C EA0000DB b       22DDD00h              ///022DDD00 LBL1
022DD990 E1D404F8 ldrsh   r0,[r4,48h]
022DD994 E3500000 cmp     r0,0h
022DD998 1A0000D8 bne     22DDD00h              ///022DDD00 LBL1
022DD99C EBF55D7D bl      2034F98h
022DD9A0 E3500000 cmp     r0,0h
022DD9A4 1A0000D5 bne     22DDD00h              ///022DDD00 LBL1
022DD9A8 EBF55DBA bl      2035098h
022DD9AC E1A05000 mov     r5,r0
022DD9B0 E59F1398 ldr     r1,=23193ACh          //"menu result %3d"
022DD9B4 E1A02005 mov     r2,r5
022DD9B8 E3A00002 mov     r0,2h
022DD9BC EBF4BA1F bl      200C240h              //DebugPrint
022DD9C0 E3550000 cmp     r5,0h
022DD9C4 DA00000E ble     22DDA04h
022DD9C8 E0850085 add     r0,r5,r5,lsl 1h
022DD9CC E0866080 add     r6,r6,r0,lsl 1h
022DD9D0 E35700AB cmp     r7,0ABh
022DD9D4 02866002 addeq   r6,r6,2h
022DD9D8 E1D600B4 ldrh    r0,[r6,4h]
022DD9DC EB001BB2 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DD9E0 E1D600B4 ldrh    r0,[r6,4h]
022DD9E4 E5942014 ldr     r2,[r4,14h]
022DD9E8 E59F1364 ldr     r1,=23193BCh          //"case menu jump %3d %04x"
022DD9EC E0820080 add     r0,r2,r0,lsl 1h
022DD9F0 E584001C str     r0,[r4,1Ch]
022DD9F4 E1D630B4 ldrh    r3,[r6,4h]
022DD9F8 E1A02005 mov     r2,r5
022DD9FC E3A00002 mov     r0,2h
022DDA00 EBF4BA0E bl      200C240h              //DebugPrint
022DDA04 E59F034C ldr     r0,=2324C64h          
022DDA08 E5900000 ldr     r0,[r0]
022DDA0C EBF48DDD bl      2001188h
022DDA10 E59F1340 ldr     r1,=2324C64h
022DDA14 E3A02000 mov     r2,0h
022DDA18 E3A00001 mov     r0,1h
022DDA1C E5812000 str     r2,[r1]
022DDA20 EBFFFDC2 bl      22DD130h
022DDA24 E3A00002 mov     r0,2h
022DDA28 E1C400BE strh    r0,[r4,0Eh]
022DDA2C EA0000B3 b       22DDD00h              ///022DDD00 LBL1
022DDA30 E1D404F4 ldrsh   r0,[r4,44h]
022DDA34 E3500000 cmp     r0,0h
022DDA38 1A00006A bne     22DDBE8h
022DDA3C E28D0004 add     r0,r13,4h
022DDA40 EB008B0D bl      230067Ch
022DDA44 E3500000 cmp     r0,0h
022DDA48 0A0000AC beq     22DDD00h              ///022DDD00 LBL1
022DDA4C E1DD00F4 ldrsh   r0,[r13,4h]
022DDA50 E3E01000 mvn     r1,0h
022DDA54 E1500001 cmp     r0,r1
022DDA58 0A000060 beq     22DDBE0h
022DDA5C EBF6333B bl      206A750h
022DDA60 E1A06000 mov     r6,r0
022DDA64 E3570091 cmp     r7,91h
022DDA68 0A000003 beq     22DDA7Ch
022DDA6C E1DD20F4 ldrsh   r2,[r13,4h]
022DDA70 E3A00000 mov     r0,0h
022DDA74 E3A01026 mov     r1,26h
022DDA78 EBF5B768 bl      204B820h
022DDA7C E1DD20F4 ldrsh   r2,[r13,4h]
022DDA80 E3A00000 mov     r0,0h
022DDA84 E3A01027 mov     r1,27h
022DDA88 EBF5B764 bl      204B820h
022DDA8C E1DD20F4 ldrsh   r2,[r13,4h]
022DDA90 E3A00000 mov     r0,0h
022DDA94 E3A01029 mov     r1,29h
022DDA98 EBF5B760 bl      204B820h
022DDA9C E1DD00F4 ldrsh   r0,[r13,4h]
022DDAA0 EBF5C3A9 bl      204E94Ch
022DDAA4 E1A00006 mov     r0,r6
022DDAA8 EBF5C645 bl      204F3C4h
022DDAAC E3100020 tst     r0,20h
022DDAB0 13A05007 movne   r5,7h
022DDAB4 1A000042 bne     22DDBC4h
022DDAB8 E3100040 tst     r0,40h
022DDABC 13A05008 movne   r5,8h
022DDAC0 1A00003F bne     22DDBC4h
022DDAC4 E3100901 tst     r0,4000h
022DDAC8 0A000037 beq     22DDBACh
022DDACC E1DD10F4 ldrsh   r1,[r13,4h]
022DDAD0 E3510063 cmp     r1,63h
022DDAD4 CA00000B bgt     22DDB08h
022DDAD8 AA00001F bge     22DDB5Ch
022DDADC E3510029 cmp     r1,29h
022DDAE0 CA000005 bgt     22DDAFCh
022DDAE4 E3510026 cmp     r1,26h
022DDAE8 BA000035 blt     22DDBC4h
022DDAEC 0A000018 beq     22DDB54h
022DDAF0 E3510029 cmp     r1,29h
022DDAF4 03A05015 moveq   r5,15h
022DDAF8 EA000031 b       22DDBC4h
022DDAFC E351003F cmp     r1,3Fh
022DDB00 03A05016 moveq   r5,16h
022DDB04 EA00002E b       22DDBC4h
022DDB08 E351006E cmp     r1,6Eh
022DDB0C CA00000D bgt     22DDB48h
022DDB10 E2510064 subs    r0,r1,64h
022DDB14 508FF100 addpl   r15,r15,r0,lsl 2h
022DDB18 EA000029 b       22DDBC4h
022DDB1C EA000010 b       22DDB64h
022DDB20 EA000011 b       22DDB6Ch
022DDB24 EA000012 b       22DDB74h
022DDB28 EA000013 b       22DDB7Ch
022DDB2C EA000014 b       22DDB84h
022DDB30 EA000023 b       22DDBC4h
022DDB34 EA000022 b       22DDBC4h
022DDB38 EA000013 b       22DDB8Ch
022DDB3C EA000014 b       22DDB94h
022DDB40 EA000015 b       22DDB9Ch
022DDB44 EA000016 b       22DDBA4h
022DDB48 E35100AE cmp     r1,0AEh
022DDB4C 03A05021 moveq   r5,21h
022DDB50 EA00001B b       22DDBC4h
022DDB54 E3A05014 mov     r5,14h
022DDB58 EA000019 b       22DDBC4h
022DDB5C E3A05017 mov     r5,17h
022DDB60 EA000017 b       22DDBC4h
022DDB64 E3A05018 mov     r5,18h
022DDB68 EA000015 b       22DDBC4h
022DDB6C E3A05019 mov     r5,19h
022DDB70 EA000013 b       22DDBC4h
022DDB74 E3A0501A mov     r5,1Ah
022DDB78 EA000011 b       22DDBC4h
022DDB7C E3A0501B mov     r5,1Bh
022DDB80 EA00000F b       22DDBC4h
022DDB84 E3A0501C mov     r5,1Ch
022DDB88 EA00000D b       22DDBC4h
022DDB8C E3A0501D mov     r5,1Dh
022DDB90 EA00000B b       22DDBC4h
022DDB94 E3A0501E mov     r5,1Eh
022DDB98 EA000009 b       22DDBC4h
022DDB9C E3A0501F mov     r5,1Fh
022DDBA0 EA000007 b       22DDBC4h
022DDBA4 E3A05020 mov     r5,20h
022DDBA8 EA000005 b       22DDBC4h
022DDBAC E3100001 tst     r0,1h
022DDBB0 13A05012 movne   r5,12h
022DDBB4 1A000002 bne     22DDBC4h
022DDBB8 E3500000 cmp     r0,0h
022DDBBC 03A05000 moveq   r5,0h
022DDBC0 13A05013 movne   r5,13h
022DDBC4 E1A00004 mov     r0,r4
022DDBC8 E1A01005 mov     r1,r5
022DDBCC EB001A05 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022DDBD0 E584001C str     r0,[r4,1Ch]
022DDBD4 E3A00002 mov     r0,2h
022DDBD8 E1C400BE strh    r0,[r4,0Eh]
022DDBDC EA000047 b       22DDD00h              ///022DDD00 LBL1
022DDBE0 E1C414B4 strh    r1,[r4,44h]
022DDBE4 EA000045 b       22DDD00h              ///022DDD00 LBL1
022DDBE8 E3500001 cmp     r0,1h
022DDBEC 1A000009 bne     22DDC18h
022DDBF0 E1D600B4 ldrh    r0,[r6,4h]
022DDBF4 EB001B2C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DDBF8 EB002E56 bl      22E9558h
022DDBFC E1A00004 mov     r0,r4
022DDC00 E3A01000 mov     r1,0h
022DDC04 EB0019F7 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022DDC08 E584001C str     r0,[r4,1Ch]
022DDC0C E3A00002 mov     r0,2h
022DDC10 E1C400BE strh    r0,[r4,0Eh]
022DDC14 EA000039 b       22DDD00h              ///022DDD00 LBL1
022DDC18 E3A00000 mov     r0,0h
022DDC1C EBF5C332 bl      204E8ECh
022DDC20 E3A01027 mov     r1,27h
022DDC24 E2412028 sub     r2,r1,28h
022DDC28 E3A00000 mov     r0,0h
022DDC2C EBF5B6FB bl      204B820h
022DDC30 E1A00004 mov     r0,r4
022DDC34 E3A01001 mov     r1,1h
022DDC38 EB0019EA bl      22E43E8h          //022E43E8 ScriptCaseProcess
022DDC3C E584001C str     r0,[r4,1Ch]
022DDC40 E3A00002 mov     r0,2h
022DDC44 E1C400BE strh    r0,[r4,0Eh]
022DDC48 EA00002C b       22DDD00h              ///022DDD00 LBL1
///022DDC4C LBL17
022DDC4C E1D600B2 ldrh    r0,[r6,2h]
022DDC50 EB001B15 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DDC54 EB002E4C bl      22E958Ch
022DDC58 E1A00004 mov     r0,r4
022DDC5C E3A01001 mov     r1,1h
022DDC60 EB0019E0 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022DDC64 E584001C str     r0,[r4,1Ch]
022DDC68 EA000024 b       22DDD00h              ///022DDD00 LBL1
///022DDC6C LBL15
022DDC6C E1D600B2 ldrh    r0,[r6,2h]
022DDC70 EB001B0D bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DDC74 E1A07000 mov     r7,r0
022DDC78 E1D600B4 ldrh    r0,[r6,4h]
022DDC7C EB001B0A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DDC80 E1A02000 mov     r2,r0
022DDC84 E59F10D0 ldr     r1,=23193D4h          //"TABLEDAT/item%02d.dat"
022DDC88 E28D0020 add     r0,r13,20h
022DDC8C EBF6AE3C bl      2089584h                      ///fun_02089584 ReplaceTokens
022DDC90 E59F20C8 ldr     r2,=30Fh
022DDC94 E28D0014 add     r0,r13,14h
022DDC98 E28D1020 add     r1,r13,20h
022DDC9C EBF4ABE6 bl      2008C3Ch                //LoadFileFromRom(R1=filepath)
022DDCA0 E59D1014 ldr     r1,[r13,14h]
022DDCA4 E3A00B01 mov     r0,400h
022DDCA8 E2816002 add     r6,r1,2h
022DDCAC E1D150F0 ldrsh   r5,[r1]
022DDCB0 EBF4916F bl      2002274h
022DDCB4 EA000004 b       22DDCCCh
022DDCB8 E1D610F0 ldrsh   r1,[r6]
022DDCBC E1500001 cmp     r0,r1
022DDCC0 BA000003 blt     22DDCD4h
022DDCC4 E2866004 add     r6,r6,4h
022DDCC8 E2455001 sub     r5,r5,1h
022DDCCC E3550001 cmp     r5,1h
022DDCD0 CAFFFFF8 bgt     22DDCB8h
022DDCD4 E1D610F2 ldrsh   r1,[r6,2h]
022DDCD8 E28D001C add     r0,r13,1Ch
022DDCDC EBF4BCA5 bl      200CF78h
022DDCE0 E1A00807 mov     r0,r7,lsl 10h
022DDCE4 E28D101C add     r1,r13,1Ch
022DDCE8 E1A00840 mov     r0,r0,asr 10h
022DDCEC EBF61FF0 bl      2065CB4h
022DDCF0 E28D0014 add     r0,r13,14h
022DDCF4 EBF4ABB6 bl      2008BD4h
022DDCF8 E3A00002 mov     r0,2h
022DDCFC E1C400BE strh    r0,[r4,0Eh]
///022DDD00 LBL1
022DDD00 E28DD0A0 add     r13,r13,0A0h
022DDD04 E8BD81F0 pop     r4-r8,r15
022DDD08 0000012F 
022DDD0C 02319260 
022DDD10 02319284 
022DDD14 023192B4 
022DDD18 02324E80 
022DDD1C 023192E8 
022DDD20 02319304 
022DDD24 02319320 
022DDD28 02319338 
022DDD2C 02319350 
022DDD30 02319370 
022DDD34 00000113 
022DDD38 00000132 
022DDD3C 02319380 
022DDD40 02319398 
022DDD44 02324EA4 
022DDD48 02324ECC 
022DDD4C 00003008 
022DDD50 023193AC 
022DDD54 023193BC 
022DDD58 02324C64
022DDD5C 023193D4
022DDD60 0000030F


22E5D50h
22E8774h
/*
    fun_022E8774 CallsScriptOpcodeParsing
*/
022E8774 E92D4FF0 push    r4-r11,r14
022E8778 E24DD034 sub     r13,r13,34h
022E877C E59F2A64 ldr     r2,=2324FA0h
022E8780 E1A04000 mov     r4,r0
022E8784 E5824014 str     r4,[r2,14h]
022E8788 E3A00000 mov     r0,0h
022E878C E5C20004 strb    r0,[r2,4h]
022E8790 E3A01001 mov     r1,1h
022E8794 E5C21002 strb    r1,[r2,2h]
022E8798 E5C20003 strb    r0,[r2,3h]
022E879C E5C20001 strb    r0,[r2,1h]
022E87A0 E3A0102C mov     r1,2Ch
022E87A4 E5C20000 strb    r0,[r2]
022E87A8 EBF58B4F bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
022E87AC E1A02000 mov     r2,r0
022E87B0 E59F0A34 ldr     r0,=23209B0h
022E87B4 E1A01004 mov     r1,r4
022E87B8 EBF48E8F bl      200C1FCh
022E87BC EBF5019A bl      2028E2Ch
022E87C0 E3A00000 mov     r0,0h
022E87C4 E3A0180B mov     r1,0B0000h
022E87C8 E3A02020 mov     r2,20h
022E87CC E3A03001 mov     r3,1h
022E87D0 EBF46271 bl      200119Ch
022E87D4 E59F1A14 ldr     r1,=2324CB0h
022E87D8 E59F2A14 ldr     r2,=2324FC0h
022E87DC E5810008 str     r0,[r1,8h]
022E87E0 E3A00919 mov     r0,64000h
022E87E4 E58D000C str     r0,[r13,0Ch]
022E87E8 E28D0008 add     r0,r13,8h
022E87EC E3A01034 mov     r1,34h
022E87F0 E58D2008 str     r2,[r13,8h]
022E87F4 EBF462A1 bl      2001280h
022E87F8 E59F29F0 ldr     r2,=2324CB0h
022E87FC E59F19F4 ldr     r1,=22E93C0h
022E8800 E5820004 str     r0,[r2,4h]
022E8804 E59F09F0 ldr     r0,=22E935Ch
022E8808 EBF46198 bl      2000E70h
022E880C E244000C sub     r0,r4,0Ch
022E8810 E3500008 cmp     r0,8h
022E8814 908FF100 addls   r15,r15,r0,lsl 2h
022E8818 EA000024 b       22E88B0h
022E881C EA000007 b       22E8840h
022E8820 EA000006 b       22E8840h
022E8824 EA000005 b       22E8840h
022E8828 EA000020 b       22E88B0h
022E882C EA00001F b       22E88B0h
022E8830 EA000009 b       22E885Ch
022E8834 EA00000D b       22E8870h
022E8838 EA000017 b       22E889Ch
022E883C EA000016 b       22E889Ch
022E8840 E59F09A0 ldr     r0,=2324FA0h
022E8844 E3A02001 mov     r2,1h
022E8848 E5C02000 strb    r2,[r0]
022E884C E3A01000 mov     r1,0h
022E8850 E5C01002 strb    r1,[r0,2h]
022E8854 E5C02004 strb    r2,[r0,4h]
022E8858 EA000014 b       22E88B0h
022E885C E59F0984 ldr     r0,=2324FA0h
022E8860 E3A01001 mov     r1,1h
022E8864 E5C01002 strb    r1,[r0,2h]
022E8868 E5C01004 strb    r1,[r0,4h]
022E886C EA00000F b       22E88B0h
022E8870 E28D0004 add     r0,r13,4h
022E8874 EBF582AF bl      2049338h
022E8878 E59D0004 ldr     r0,[r13,4h]
022E887C E3A01001 mov     r1,1h
022E8880 E3500000 cmp     r0,0h
022E8884 03A02001 moveq   r2,1h
022E8888 E59F0958 ldr     r0,=2324FA0h
022E888C 13A02000 movne   r2,0h
022E8890 E5C02002 strb    r2,[r0,2h]
022E8894 E5C01004 strb    r1,[r0,4h]
022E8898 EA000004 b       22E88B0h
022E889C E59F0944 ldr     r0,=2324FA0h
022E88A0 E3A01000 mov     r1,0h
022E88A4 E5C01002 strb    r1,[r0,2h]
022E88A8 E3A01001 mov     r1,1h
022E88AC E5C01004 strb    r1,[r0,4h]
022E88B0 EBF4BC6C bl      2017A68h
022E88B4 EB000BE3 bl      22EB848h
022E88B8 E3540011 cmp     r4,11h
022E88BC 03A00002 moveq   r0,2h
022E88C0 13A00000 movne   r0,0h
022E88C4 EB000395 bl      22E9720h
022E88C8 EB0009C1 bl      22EAFD4h
022E88CC EBFFCF7C bl      22DC6C4h
022E88D0 EB002F10 bl      22F4518h
022E88D4 EB000C5A bl      22EBA44h
022E88D8 E3A00001 mov     r0,1h
022E88DC EBF57DC6 bl      2047FFCh
022E88E0 EB0039ED bl      22F709Ch
022E88E4 EB00223C bl      22F11DCh
022E88E8 EB000806 bl      22EA908h
022E88EC EB00583B bl      22FE9E0h
022E88F0 EB003CF1 bl      22F7CBCh
022E88F4 EB004D30 bl      22FBDBCh
022E88F8 EB005349 bl      22FD624h
022E88FC EB00915D bl      230CE78h
022E8900 EB0091BB bl      230CFF4h
022E8904 E59F18DC ldr     r1,=2324FA0h
022E8908 E3A02001 mov     r2,1h
022E890C E3A00000 mov     r0,0h
022E8910 E581201C str     r2,[r1,1Ch]
022E8914 E5810010 str     r0,[r1,10h]
022E8918 E2402001 sub     r2,r0,1h
022E891C E5812018 str     r2,[r1,18h]
022E8920 E3A0101C mov     r1,1Ch
022E8924 EBF58AF0 bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
022E8928 E59F28B8 ldr     r2,=2324FA0h
022E892C E3A0101D mov     r1,1Dh
022E8930 E1C200B8 strh    r0,[r2,8h]
022E8934 E3A00000 mov     r0,0h
022E8938 EBF58AEB bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
022E893C E59F18A4 ldr     r1,=2324FA0h
022E8940 E3E02000 mvn     r2,0h
022E8944 E581000C str     r0,[r1,0Ch]
022E8948 E1C120B6 strh    r2,[r1,6h]
022E894C E59F189C ldr     r1,=2324CB0h
022E8950 E3A02001 mov     r2,1h
022E8954 E59F08A4 ldr     r0,=22E9258h
022E8958 E5C12000 strb    r2,[r1]
022E895C EBF46BB6 bl      200383Ch                  ///0200383C
{
    0200383C E92D4038 push    r3-r5,r14
    02003840 E24DD008 sub     r13,r13,8h
    02003844 E1A05000 mov     r5,r0                 //R5 = 022E9258
    02003848 E28D0000 add     r0,r13,0h
    0200384C E3A0100B mov     r1,0Bh
    02003850 EBFFFB74 bl      2002628h
    02003854 EBFFFB85 bl      2002670h
    02003858 E59F006C ldr     r0,=20AEF7Ch          //
    0200385C E3A01001 mov     r1,1h
    02003860 E5C01002 strb    r1,[r0,2h]
    02003864 E5904010 ldr     r4,[r0,10h]
    02003868 E5805010 str     r5,[r0,10h]
    0200386C EBFFFB91 bl      20026B8h
    02003870 E3550000 cmp     r5,0h
    02003874 1A000007 bne     2003898h
    02003878 E3540000 cmp     r4,0h
    0200387C 0A00000E beq     20038BCh
    02003880 E59F1044 ldr     r1,=20AEF7Ch
    02003884 E3A02000 mov     r2,0h
    02003888 E59F0040 ldr     r0,=20AF028h
    0200388C E5C12003 strb    r2,[r1,3h]
    02003890 EBFFFC42 bl      20029A0h
    02003894 EA000008 b       20038BCh
    02003898 E3540000 cmp     r4,0h
    0200389C 1A000006 bne     20038BCh
    020038A0 E59F1024 ldr     r1,=20AEF7Ch
    020038A4 E3A02001 mov     r2,1h
    020038A8 E59F0020 ldr     r0,=20AF028h
    020038AC E5C12000 strb    r2,[r1]
    020038B0 EBFFFC3A bl      20029A0h
    020038B4 E59F0018 ldr     r0,=20AF000h
    020038B8 EBFFFC61 bl      2002A44h
    020038BC E28D0000 add     r0,r13,0h
    020038C0 EBFFFB65 bl      200265Ch
    020038C4 E28DD008 add     r13,r13,8h
    020038C8 E8BD8038 pop     r3-r5,r15
    020038CC 020AEF7C
    020038D0 020AF028
    020038D4 020AF000
}
022E8960 E3E00000 mvn     r0,0h
022E8964 E1CD00B2 strh    r0,[r13,2h]
022E8968 EBF46BDE bl      20038E8h
022E896C EBFFCFA5 bl      22DC808h
022E8970 E59F0870 ldr     r0,=2324FA0h
022E8974 E590001C ldr     r0,[r0,1Ch]
022E8978 E3500001 cmp     r0,1h
022E897C 13500002 cmpne   r0,2h
022E8980 0A000002 beq     22E8990h
022E8984 E59F0878 ldr     r0,=23209CCh
022E8988 EBF48E1B bl      200C1FCh
022E898C EA000192 b       22E8FDCh
022E8990 EBF530DD bl      2034D0Ch
022E8994 E59F084C ldr     r0,=2324FA0h
022E8998 E5902014 ldr     r2,[r0,14h]
022E899C E3520014 cmp     r2,14h
022E89A0 908FF102 addls   r15,r15,r2,lsl 2h
022E89A4 EA0000CA b       22E8CD4h
022E89A8 EA000013 b       22E89FCh
022E89AC EA0000C8 b       22E8CD4h
022E89B0 EA00001B b       22E8A24h
022E89B4 EA0000C6 b       22E8CD4h
022E89B8 EA0000C5 b       22E8CD4h
022E89BC EA0000C4 b       22E8CD4h
022E89C0 EA0000C3 b       22E8CD4h
022E89C4 EA0000C2 b       22E8CD4h
022E89C8 EA000045 b       22E8AE4h
022E89CC EA000044 b       22E8AE4h
022E89D0 EA000043 b       22E8AE4h
022E89D4 EA000042 b       22E8AE4h
022E89D8 EA000090 b       22E8C20h
022E89DC EA00008F b       22E8C20h
022E89E0 EA00000F b       22E8A24h
022E89E4 EA000080 b       22E8BECh
022E89E8 EA000013 b       22E8A3Ch
022E89EC EA0000AD b       22E8CA8h
022E89F0 EA0000AF b       22E8CB4h
022E89F4 EA0000B1 b       22E8CC0h
022E89F8 EA0000B3 b       22E8CCCh
022E89FC E59F0804 ldr     r0,=23209ECh
022E8A00 EBF48DFD bl      200C1FCh
022E8A04 E3A00000 mov     r0,0h
022E8A08 E3A0105C mov     r1,5Ch
022E8A0C EBF58A88 bl      204B434h
022E8A10 EB000288 bl      22E9438h
022E8A14 E59F07CC ldr     r0,=2324FA0h
022E8A18 E3A01001 mov     r1,1h
022E8A1C E5801014 str     r1,[r0,14h]
022E8A20 EA0000AB b       22E8CD4h
022E8A24 E28D0002 add     r0,r13,2h
022E8A28 EBF60DAA bl      206C0D8h
022E8A2C E59F07B4 ldr     r0,=2324FA0h
022E8A30 E3A01001 mov     r1,1h
022E8A34 E5801014 str     r1,[r0,14h]
022E8A38 EA0000A5 b       22E8CD4h
022E8A3C EBF59732 bl      204E70Ch
022E8A40 E1A05000 mov     r5,r0
022E8A44 EBF59745 bl      204E760h
022E8A48 E1A04000 mov     r4,r0
022E8A4C E1A02004 mov     r2,r4
022E8A50 E3A00000 mov     r0,0h
022E8A54 E3A01027 mov     r1,27h
022E8A58 EBF58B70 bl      204B820h
022E8A5C E1A02004 mov     r2,r4
022E8A60 E3A00000 mov     r0,0h
022E8A64 E3A01029 mov     r1,29h
022E8A68 EBF58B6C bl      204B820h
022E8A6C E59F0798 ldr     r0,=2320A08h
022E8A70 E1A01005 mov     r1,r5
022E8A74 E1A02004 mov     r2,r4
022E8A78 EBF48DDF bl      200C1FCh
022E8A7C E3550002 cmp     r5,2h
022E8A80 059F0788 ldreq   r0,=29Dh
022E8A84 01CD00B2 streqh  r0,[r13,2h]
022E8A88 0A000091 beq     22E8CD4h
022E8A8C E3550003 cmp     r5,3h
022E8A90 059F077C ldreq   r0,=29Eh
022E8A94 01CD00B2 streqh  r0,[r13,2h]
022E8A98 0A00008D beq     22E8CD4h
022E8A9C E3A00000 mov     r0,0h
022E8AA0 EBF58FFB bl      204CA94h
022E8AA4 E3500000 cmp     r0,0h
022E8AA8 159F0768 ldrne   r0,=29Fh
022E8AAC 11CD00B2 strneh  r0,[r13,2h]
022E8AB0 1A000087 bne     22E8CD4h
022E8AB4 E1A00004 mov     r0,r4
022E8AB8 EBF6072B bl      206A76Ch
022E8ABC E3500003 cmp     r0,3h
022E8AC0 03A00FA7 moveq   r0,29Ch
022E8AC4 01CD00B2 streqh  r0,[r13,2h]
022E8AC8 0A000081 beq     22E8CD4h
022E8ACC E3550001 cmp     r5,1h
022E8AD0 059F0744 ldreq   r0,=29Bh
022E8AD4 01CD00B2 streqh  r0,[r13,2h]
022E8AD8 159F0740 ldrne   r0,=29Ah
022E8ADC 11CD00B2 strneh  r0,[r13,2h]
022E8AE0 EA00007B b       22E8CD4h
022E8AE4 E3A00000 mov     r0,0h
022E8AE8 E3A0102B mov     r1,2Bh
022E8AEC EBF58B4B bl      204B820h
022E8AF0 E59F0710 ldr     r0,=23209ECh
022E8AF4 EBF48DC0 bl      200C1FCh
022E8AF8 E3A00000 mov     r0,0h
022E8AFC E3A0105C mov     r1,5Ch
022E8B00 EBF58A4B bl      204B434h
022E8B04 EB00024B bl      22E9438h
022E8B08 E3A00000 mov     r0,0h
022E8B0C E3A0102A mov     r1,2Ah
022E8B10 E3A02001 mov     r2,1h
022E8B14 E3A03002 mov     r3,2h
022E8B18 EBF58CEE bl      204BED8h
022E8B1C EBF59944 bl      204F034h
022E8B20 E1A04000 mov     r4,r0
022E8B24 EBF59949 bl      204F050h
022E8B28 E1A05000 mov     r5,r0
022E8B2C EBF6070E bl      206A76Ch
022E8B30 E1A03000 mov     r3,r0
022E8B34 E59F06E8 ldr     r0,=2320A28h
022E8B38 E1A01004 mov     r1,r4
022E8B3C E1A02005 mov     r2,r5
022E8B40 EBF48DAD bl      200C1FCh
022E8B44 E3540002 cmp     r4,2h
022E8B48 13540003 cmpne   r4,3h
022E8B4C 0A000010 beq     22E8B94h
022E8B50 E3A00000 mov     r0,0h
022E8B54 EBF58FCE bl      204CA94h
022E8B58 E3500000 cmp     r0,0h
022E8B5C 159F06C4 ldrne   r0,=2A5h
022E8B60 11CD00B2 strneh  r0,[r13,2h]
022E8B64 1A00000A bne     22E8B94h
022E8B68 E1A00005 mov     r0,r5
022E8B6C EBF606FE bl      206A76Ch
022E8B70 E3500003 cmp     r0,3h
022E8B74 03A00FA9 moveq   r0,2A4h
022E8B78 01CD00B2 streqh  r0,[r13,2h]
022E8B7C 0A000004 beq     22E8B94h
022E8B80 E3540001 cmp     r4,1h
022E8B84 059F06A0 ldreq   r0,=2A1h
022E8B88 01CD00B2 streqh  r0,[r13,2h]
022E8B8C 13A00E2A movne   r0,2A0h
022E8B90 11CD00B2 strneh  r0,[r13,2h]
022E8B94 E59F064C ldr     r0,=2324FA0h
022E8B98 E5900014 ldr     r0,[r0,14h]
022E8B9C E3500008 cmp     r0,8h
022E8BA0 1A000007 bne     22E8BC4h
022E8BA4 E3540000 cmp     r4,0h
022E8BA8 13540001 cmpne   r4,1h
022E8BAC 1A000004 bne     22E8BC4h
022E8BB0 E35500D5 cmp     r5,0D5h
022E8BB4 AA000002 bge     22E8BC4h
022E8BB8 E1A00005 mov     r0,r5
022E8BBC E3A01001 mov     r1,1h
022E8BC0 EBF590DC bl      204CF38h
022E8BC4 E3A00000 mov     r0,0h
022E8BC8 EBF58FB1 bl      204CA94h
022E8BCC E3500000 cmp     r0,0h
022E8BD0 0A00003F beq     22E8CD4h
022E8BD4 E59F060C ldr     r0,=2324FA0h
022E8BD8 E3A01001 mov     r1,1h
022E8BDC E1C010B8 strh    r1,[r0,8h]
022E8BE0 E3A01000 mov     r1,0h
022E8BE4 E580100C str     r1,[r0,0Ch]
022E8BE8 EA000039 b       22E8CD4h
022E8BEC EBF596DB bl      204E760h
022E8BF0 E1A04000 mov     r4,r0
022E8BF4 E1A02004 mov     r2,r4
022E8BF8 E3A00000 mov     r0,0h
022E8BFC E3A01027 mov     r1,27h
022E8C00 EBF58B06 bl      204B820h
022E8C04 E1A02004 mov     r2,r4
022E8C08 E3A00000 mov     r0,0h
022E8C0C E3A01029 mov     r1,29h
022E8C10 EBF58B02 bl      204B820h
022E8C14 E59F0614 ldr     r0,=2A6h
022E8C18 E1CD00B2 strh    r0,[r13,2h]
022E8C1C EA00002C b       22E8CD4h
022E8C20 E59F05E0 ldr     r0,=23209ECh
022E8C24 EBF48D74 bl      200C1FCh
022E8C28 E3A00000 mov     r0,0h
022E8C2C E3A0105C mov     r1,5Ch
022E8C30 EBF589FF bl      204B434h
022E8C34 EB0001FF bl      22E9438h
022E8C38 E59F15A8 ldr     r1,=2324FA0h
022E8C3C E3A00000 mov     r0,0h
022E8C40 E5912014 ldr     r2,[r1,14h]
022E8C44 E3A0102C mov     r1,2Ch
022E8C48 EBF58AF4 bl      204B820h
022E8C4C E3A00000 mov     r0,0h
022E8C50 E3A0101E mov     r1,1Eh
022E8C54 E3A02001 mov     r2,1h
022E8C58 EBF58AF0 bl      204B820h
022E8C5C E3A00000 mov     r0,0h
022E8C60 E3A0101C mov     r1,1Ch
022E8C64 E3A02001 mov     r2,1h
022E8C68 EBF58AEC bl      204B820h
022E8C6C E3A00000 mov     r0,0h
022E8C70 E3A0101D mov     r1,1Dh
022E8C74 E1A02000 mov     r2,r0
022E8C78 EBF58AE8 bl      204B820h
022E8C7C E59F0564 ldr     r0,=2324FA0h
022E8C80 E3A01001 mov     r1,1h
022E8C84 E5900014 ldr     r0,[r0,14h]
022E8C88 E350000C cmp     r0,0Ch
022E8C8C 03A00046 moveq   r0,46h
022E8C90 01CD00B2 streqh  r0,[r13,2h]
022E8C94 13A00047 movne   r0,47h
022E8C98 11CD00B2 strneh  r0,[r13,2h]
022E8C9C E59F0544 ldr     r0,=2324FA0h
022E8CA0 E5801014 str     r1,[r0,14h]
022E8CA4 EA00000A b       22E8CD4h
022E8CA8 E3A0004E mov     r0,4Eh
022E8CAC E1CD00B2 strh    r0,[r13,2h]
022E8CB0 EA000007 b       22E8CD4h
022E8CB4 E3A0004F mov     r0,4Fh
022E8CB8 E1CD00B2 strh    r0,[r13,2h]
022E8CBC EA000004 b       22E8CD4h
022E8CC0 E3A00050 mov     r0,50h
022E8CC4 E1CD00B2 strh    r0,[r13,2h]
022E8CC8 EA000001 b       22E8CD4h
022E8CCC E3A00051 mov     r0,51h
022E8CD0 E1CD00B2 strh    r0,[r13,2h]
022E8CD4 E59F050C ldr     r0,=2324FA0h
022E8CD8 E5D00004 ldrb    r0,[r0,4h]
022E8CDC E3500000 cmp     r0,0h
022E8CE0 1A000024 bne     22E8D78h
022E8CE4 E3A00000 mov     r0,0h
022E8CE8 E3A0101C mov     r1,1Ch
022E8CEC EBF589FE bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
022E8CF0 E59F14F0 ldr     r1,=2324FA0h
022E8CF4 E1A04000 mov     r4,r0
022E8CF8 E5912014 ldr     r2,[r1,14h]
022E8CFC E3A00000 mov     r0,0h
022E8D00 E3A0102C mov     r1,2Ch
022E8D04 EBF58AC5 bl      204B820h
022E8D08 E59F14D8 ldr     r1,=2324FA0h
022E8D0C E59F0520 ldr     r0,=2320A44h
022E8D10 E5911014 ldr     r1,[r1,14h]
022E8D14 EBF48D38 bl      200C1FCh
022E8D18 E59F04C8 ldr     r0,=2324FA0h
022E8D1C E5901014 ldr     r1,[r0,14h]
022E8D20 E3510001 cmp     r1,1h
022E8D24 0A000009 beq     22E8D50h
022E8D28 E3510004 cmp     r1,4h
022E8D2C E3A0101E mov     r1,1Eh
022E8D30 1A000003 bne     22E8D44h
022E8D34 E1D020F8 ldrsh   r2,[r0,8h]
022E8D38 E3A00000 mov     r0,0h
022E8D3C EBF58AB7 bl      204B820h
022E8D40 EA000002 b       22E8D50h
022E8D44 E1A02004 mov     r2,r4
022E8D48 E3A00000 mov     r0,0h
022E8D4C EBF58AB3 bl      204B820h
022E8D50 E59F1490 ldr     r1,=2324FA0h
022E8D54 E3A00000 mov     r0,0h
022E8D58 E1D120F8 ldrsh   r2,[r1,8h]
022E8D5C E3A0101C mov     r1,1Ch
022E8D60 EBF58AAE bl      204B820h
022E8D64 E59F147C ldr     r1,=2324FA0h
022E8D68 E3A00000 mov     r0,0h
022E8D6C E591200C ldr     r2,[r1,0Ch]
022E8D70 E3A0101D mov     r1,1Dh
022E8D74 EBF58AA9 bl      204B820h
022E8D78 E59F0468 ldr     r0,=2324FA0h
022E8D7C E3A02003 mov     r2,3h
022E8D80 E3A01000 mov     r1,0h
022E8D84 E5802014 str     r2,[r0,14h]
022E8D88 E580101C str     r1,[r0,1Ch]
022E8D8C E5801010 str     r1,[r0,10h]
022E8D90 E2412001 sub     r2,r1,1h
022E8D94 E5802018 str     r2,[r0,18h]
022E8D98 E1D010F8 ldrsh   r1,[r0,8h]
022E8D9C E1C010BA strh    r1,[r0,0Ah]
022E8DA0 E1C020B8 strh    r2,[r0,8h]
022E8DA4 EB0002C8 bl      22E98CCh
022E8DA8 EBFFCE5A bl      22DC718h
022E8DAC EB002E36 bl      22F468Ch
022E8DB0 EB0038D3 bl      22F7104h
022E8DB4 EB002122 bl      22F1244h
022E8DB8 E3A00000 mov     r0,0h
022E8DBC EB00088D bl      22EAFF8h
022E8DC0 EB005718 bl      22FEA28h
022E8DC4 EB003BD3 bl      22F7D18h
022E8DC8 EB004C0D bl      22FBE04h
022E8DCC EB005226 bl      22FD66Ch
022E8DD0 EBF59C85 bl      204FFECh
022E8DD4 E1DD00F2 ldrsh   r0,[r13,2h]
022E8DD8 E3E01000 mvn     r1,0h
022E8DDC E1500001 cmp     r0,r1
022E8DE0 E3A01000 mov     r1,0h
022E8DE4 0A000003 beq     22E8DF8h
022E8DE8 EB0038E6 bl      22F7188h          //022F7188 GroundSupervision ExecuteCommon
{//022F7188
    022F7188 E92D4038 push    r3-r5,r14
    022F718C E1A05000 mov     r5,r0
    022F7190 E1A04001 mov     r4,r1
    022F7194 E59F1040 ldr     r1,=23214A0h      //"GroundSupervision ExecuteCommon request %3d %d =================="
    022F7198 E1A02005 mov     r2,r5
    022F719C E1A03004 mov     r3,r4
    022F71A0 E3A00002 mov     r0,2h
    022F71A4 EBF45425 bl      200C240h              //DebugPrint
    022F71A8 E59F1030 ldr     r1,=2324CF4h
    022F71AC E5912000 ldr     r2,[r1]
    022F71B0 E592010C ldr     r0,[r2,10Ch]
    022F71B4 E3500001 cmp     r0,1h
    022F71B8 08BD8038 popeq   r3-r5,r15
    022F71BC E3A00002 mov     r0,2h
    022F71C0 E582010C str     r0,[r2,10Ch]
    022F71C4 E5910000 ldr     r0,[r1]
    022F71C8 E2800C01 add     r0,r0,100h
    022F71CC E1C051B0 strh    r5,[r0,10h]
    022F71D0 E5910000 ldr     r0,[r1]
    022F71D4 E5C0411D strb    r4,[r0,11Dh]
    022F71D8 E8BD8038 pop     r3-r5,r15
    022F71DC 023214A0 
    022F71E0 02324CF4 
}
022E8DEC E3E00000 mvn     r0,0h
022E8DF0 E1CD00B2 strh    r0,[r13,2h]
022E8DF4 EA000001 b       22E8E00h
022E8DF8 E3A0003C mov     r0,3Ch
022E8DFC EB0038E1 bl      22F7188h          //022F7188 GroundSupervision ExecuteCommon
022E8E00 EBF46AF7 bl      20039E4h
022E8E04 E3A0B000 mov     r11,0h
022E8E08 E3A06001 mov     r6,1h
022E8E0C E3A0801E mov     r8,1Eh
022E8E10 E59F53D0 ldr     r5,=2324FA0h
022E8E14 E59F441C ldr     r4,=0F0Fh
022E8E18 E1A07006 mov     r7,r6
022E8E1C E1A0A008 mov     r10,r8
022E8E20 E1A0900B mov     r9,r11
///022E8E24
022E8E24 EBF46AAF bl      20038E8h
022E8E28 EBFFCE76 bl      22DC808h
022E8E2C EBF59AE6 bl      204F9CCh
022E8E30 E595101C ldr     r1,[r5,1Ch]
022E8E34 E3510000 cmp     r1,0h
022E8E38 0A00001F beq     22E8EBCh
022E8E3C E5950010 ldr     r0,[r5,10h]
022E8E40 E3500000 cmp     r0,0h
022E8E44 DA00000F ble     22E8E88h
022E8E48 E2400001 sub     r0,r0,1h
022E8E4C E5850010 str     r0,[r5,10h]
022E8E50 E3500000 cmp     r0,0h
022E8E54 CA00004E bgt     22E8F94h
022E8E58 E3510001 cmp     r1,1h
022E8E5C 13510002 cmpne   r1,2h
022E8E60 1A000002 bne     22E8E70h
022E8E64 E5950018 ldr     r0,[r5,18h]
022E8E68 EB00031A bl      22E9AD8h
022E8E6C EA000048 b       22E8F94h
022E8E70 E5D52004 ldrb    r2,[r5,4h]
022E8E74 E5D51001 ldrb    r1,[r5,1h]
022E8E78 E5950018 ldr     r0,[r5,18h]
022E8E7C E1821001 orr     r1,r2,r1
022E8E80 EB00031A bl      22E9AF0h
022E8E84 EA000042 b       22E8F94h
022E8E88 EB00041B bl      22E9EFCh
022E8E8C E3500000 cmp     r0,0h
022E8E90 1A00003F bne     22E8F94h
022E8E94 E59F034C ldr     r0,=2324FA0h
022E8E98 E590001C ldr     r0,[r0,1Ch]
022E8E9C E3500001 cmp     r0,1h
022E8EA0 13500002 cmpne   r0,2h
022E8EA4 1A000043 bne     22E8FB8h
022E8EA8 E3A00000 mov     r0,0h
022E8EAC EB00031C bl      22E9B24h
022E8EB0 E3A00000 mov     r0,0h
022E8EB4 EB000332 bl      22E9B84h
022E8EB8 EA00003E b       22E8FB8h
022E8EBC E5D50004 ldrb    r0,[r5,4h]
022E8EC0 E3500000 cmp     r0,0h
022E8EC4 05D50001 ldreqb  r0,[r5,1h]
022E8EC8 03500000 cmpeq   r0,0h
022E8ECC 0A000030 beq     22E8F94h
022E8ED0 E5D50003 ldrb    r0,[r5,3h]
022E8ED4 E3500000 cmp     r0,0h
022E8ED8 0A00000F beq     22E8F1Ch
022E8EDC E1A00009 mov     r0,r9
022E8EE0 EBF52E56 bl      2034840h
022E8EE4 E3500000 cmp     r0,0h
022E8EE8 1A000029 bne     22E8F94h
022E8EEC EB0039CD bl      22F7628h
022E8EF0 E3500000 cmp     r0,0h
022E8EF4 1A000026 bne     22E8F94h
022E8EF8 EB00909C bl      230D170h
022E8EFC E3500000 cmp     r0,0h
022E8F00 1A000023 bne     22E8F94h
022E8F04 EB00388D bl      22F7140h
022E8F08 E1A00008 mov     r0,r8
022E8F0C EB0001CC bl      22E9644h
022E8F10 E1A0000A mov     r0,r10
022E8F14 EBF4BAEC bl      2017ACCh
022E8F18 EA00001D b       22E8F94h
022E8F1C E5D50002 ldrb    r0,[r5,2h]
022E8F20 E3500000 cmp     r0,0h
022E8F24 1A00001A bne     22E8F94h
022E8F28 E1A0000B mov     r0,r11
022E8F2C E28D1000 add     r1,r13,0h
022E8F30 EBF474C9 bl      200625Ch
022E8F34 EBF587C9 bl      204AE60h
022E8F38 E3500000 cmp     r0,0h
022E8F3C E28D0010 add     r0,r13,10h
022E8F40 0A000001 beq     22E8F4Ch
022E8F44 EBF47734 bl      2006C1Ch
022E8F48 EA000000 b       22E8F50h
022E8F4C EBF4772A bl      2006BFCh
022E8F50 E5D50001 ldrb    r0,[r5,1h]
022E8F54 E3500000 cmp     r0,0h
022E8F58 0A000003 beq     22E8F6Ch
022E8F5C E1DD00B0 ldrh    r0,[r13]
022E8F60 E3100008 tst     r0,8h
022E8F64 15C57003 strneb  r7,[r5,3h]
022E8F68 EA000009 b       22E8F94h
022E8F6C E1DD00B0 ldrh    r0,[r13]
022E8F70 E1100004 tst     r0,r4
022E8F74 1A000005 bne     22E8F90h
022E8F78 E28D0010 add     r0,r13,10h
022E8F7C E3A01002 mov     r1,2h
022E8F80 E3A02000 mov     r2,0h
022E8F84 EBF47740 bl      2006C8Ch
022E8F88 E3500000 cmp     r0,0h
022E8F8C 0A000000 beq     22E8F94h
022E8F90 E5C56003 strb    r6,[r5,3h]
022E8F94 EB0039B4 bl      22F766Ch              ///022F766C Parses Script OpCodes
022E8F98 EB0090E8 bl      230D340h              ///0230D340 
{///0230D340 
    0230D340 E92D4030 push    r4,r5,r14
    0230D344 E24DD024 sub     r13,r13,24h
    0230D348 E59F0218 ldr     r0,=2324DBCh
    0230D34C E5901000 ldr     r1,[r0]
    0230D350 E5910050 ldr     r0,[r1,50h]
    0230D354 E281402C add     r4,r1,2Ch
    0230D358 E3500001 cmp     r0,1h
    0230D35C 0A00007E beq     230D55Ch
    0230D360 E3500002 cmp     r0,2h
    0230D364 1A000029 bne     230D410h
    0230D368 E28DC000 add     r12,r13,0h
    0230D36C E1A0E004 mov     r14,r4
    0230D370 E3A05002 mov     r5,2h
    0230D374 E8BE000F ldmia   [r14]!,r0-r3
    0230D378 E8AC000F stmia   [r12]!,r0-r3
    0230D37C E2555001 subs    r5,r5,1h
    0230D380 1AFFFFFB bne     230D374h
    0230D384 E59E0000 ldr     r0,[r14]
    0230D388 E58C0000 str     r0,[r12]
    0230D38C E5940014 ldr     r0,[r4,14h]
    0230D390 E12FFF30 blx     r0
    0230D394 E3500001 cmp     r0,1h
    0230D398 1A00001C bne     230D410h
    0230D39C E28D1000 add     r1,r13,0h
    0230D3A0 E1A00004 mov     r0,r4
    0230D3A4 EBFFFF54 bl      230D0FCh
    0230D3A8 E3500000 cmp     r0,0h
    0230D3AC 0A000017 beq     230D410h
    0230D3B0 E59F01B0 ldr     r0,=2324DBCh
    0230D3B4 E3A01003 mov     r1,3h
    0230D3B8 E5900000 ldr     r0,[r0]
    0230D3BC E5801050 str     r1,[r0,50h]
    0230D3C0 E5941008 ldr     r1,[r4,8h]
    0230D3C4 E3510000 cmp     r1,0h
    0230D3C8 0A000001 beq     230D3D4h
    0230D3CC E3A00001 mov     r0,1h
    0230D3D0 E12FFF31 blx     r1
    0230D3D4 E59F5190 ldr     r5,=23231BCh
    0230D3D8 E1A0E004 mov     r14,r4
    0230D3DC E3A0C002 mov     r12,2h
    0230D3E0 E8B5000F ldmia   [r5]!,r0-r3
    0230D3E4 E8AE000F stmia   [r14]!,r0-r3
    0230D3E8 E25CC001 subs    r12,r12,1h
    0230D3EC 1AFFFFFB bne     230D3E0h
    0230D3F0 E5951000 ldr     r1,[r5]
    0230D3F4 E59F016C ldr     r0,=2324DBCh
    0230D3F8 E58E1000 str     r1,[r14]
    0230D3FC E5901000 ldr     r1,[r0]
    0230D400 E3A02000 mov     r2,0h
    0230D404 E5C12028 strb    r2,[r1,28h]
    0230D408 E5900000 ldr     r0,[r0]
    0230D40C E5802050 str     r2,[r0,50h]
    0230D410 E59F0150 ldr     r0,=2324DBCh
    0230D414 E5901000 ldr     r1,[r0]
    0230D418 E5D10029 ldrb    r0,[r1,29h]
    0230D41C E3500000 cmp     r0,0h
    0230D420 0A000005 beq     230D43Ch
    0230D424 E5D10000 ldrb    r0,[r1]
    0230D428 E3500000 cmp     r0,0h
    0230D42C 05D10028 ldreqb  r0,[r1,28h]
    0230D430 03500000 cmpeq   r0,0h
    0230D434 1A000000 bne     230D43Ch
    0230D438 EBFFFF12 bl      230D088h
    0230D43C E59F0124 ldr     r0,=2324DBCh
    0230D440 E5901000 ldr     r1,[r0]
    0230D444 E5D10000 ldrb    r0,[r1]
    0230D448 E3500000 cmp     r0,0h
    0230D44C 0A000025 beq     230D4E8h
    0230D450 E59F0114 ldr     r0,=23231BCh
    0230D454 E1A01004 mov     r1,r4
    0230D458 EBFFFF27 bl      230D0FCh
    0230D45C E3500000 cmp     r0,0h
    0230D460 1A000006 bne     230D480h
    0230D464 E5941008 ldr     r1,[r4,8h]
    0230D468 E3510000 cmp     r1,0h
    0230D46C 0A000003 beq     230D480h
    0230D470 E3A00000 mov     r0,0h
    0230D474 E12FFF31 blx     r1
    0230D478 E3500000 cmp     r0,0h
    0230D47C 0A000036 beq     230D55Ch
    0230D480 E59F00E0 ldr     r0,=2324DBCh
    0230D484 E3A0C002 mov     r12,2h
    0230D488 E5905000 ldr     r5,[r0]
    0230D48C E285E004 add     r14,r5,4h
    0230D490 E8BE000F ldmia   [r14]!,r0-r3
    0230D494 E8A4000F stmia   [r4]!,r0-r3
    0230D498 E25CC001 subs    r12,r12,1h
    0230D49C 1AFFFFFB bne     230D490h
    0230D4A0 E59E0000 ldr     r0,[r14]
    0230D4A4 E3A01000 mov     r1,0h
    0230D4A8 E5840000 str     r0,[r4]
    0230D4AC E59F00B4 ldr     r0,=2324DBCh
    0230D4B0 E5C51000 strb    r1,[r5]
    0230D4B4 E5905000 ldr     r5,[r0]
    0230D4B8 E59FE0AC ldr     r14,=23231BCh
    0230D4BC E285C004 add     r12,r5,4h
    0230D4C0 E3A04002 mov     r4,2h
    0230D4C4 E8BE000F ldmia   [r14]!,r0-r3
    0230D4C8 E8AC000F stmia   [r12]!,r0-r3
    0230D4CC E2544001 subs    r4,r4,1h
    0230D4D0 1AFFFFFB bne     230D4C4h
    0230D4D4 E59E1000 ldr     r1,[r14]
    0230D4D8 E3A00001 mov     r0,1h
    0230D4DC E58C1000 str     r1,[r12]
    0230D4E0 E5850050 str     r0,[r5,50h]
    0230D4E4 EA00001C b       230D55Ch
    0230D4E8 E5D10028 ldrb    r0,[r1,28h]
    0230D4EC E3500000 cmp     r0,0h
    0230D4F0 0A000019 beq     230D55Ch
    0230D4F4 E59F0070 ldr     r0,=23231BCh
    0230D4F8 E1A01004 mov     r1,r4
    0230D4FC EBFFFEFE bl      230D0FCh
    0230D500 E3500000 cmp     r0,0h
    0230D504 1A000006 bne     230D524h
    0230D508 E5941008 ldr     r1,[r4,8h]
    0230D50C E3510000 cmp     r1,0h
    0230D510 0A000003 beq     230D524h
    0230D514 E3A00000 mov     r0,0h
    0230D518 E12FFF31 blx     r1
    0230D51C E3500000 cmp     r0,0h
    0230D520 0A00000D beq     230D55Ch
    0230D524 E59FE040 ldr     r14,=23231BCh
    0230D528 E3A0C002 mov     r12,2h
    0230D52C E8BE000F ldmia   [r14]!,r0-r3
    0230D530 E8A4000F stmia   [r4]!,r0-r3
    0230D534 E25CC001 subs    r12,r12,1h
    0230D538 1AFFFFFB bne     230D52Ch
    0230D53C E59E1000 ldr     r1,[r14]
    0230D540 E59F0020 ldr     r0,=2324DBCh
    0230D544 E5841000 str     r1,[r4]
    0230D548 E5901000 ldr     r1,[r0]
    0230D54C E3A02000 mov     r2,0h
    0230D550 E5C12028 strb    r2,[r1,28h]
    0230D554 E5900000 ldr     r0,[r0]
    0230D558 E5802050 str     r2,[r0,50h]
    0230D55C EB0015E7 bl      2312D00h
    0230D560 E28DD024 add     r13,r13,24h
    0230D564 E8BD8030 pop     r4,r5,r15
    0230D568 02324DBC
    0230D56C 023231BC
}
022E8F9C EBF46A90 bl      20039E4h
022E8FA0 EB003B07 bl      22F7BC4h              ///fun_22F7BC4 DecideScriptDataToLoad
022E8FA4 EB002477 bl      22F2188h              ///022F2188 Calls via bx 022F37D0
022E8FA8 EB009170 bl      230D570h              ///0230D570
022E8FAC EBF52EB3 bl      2034A80h              ///02034A80
022E8FB0 EBF4FE24 bl      2028848h              ///02028848
022E8FB4 EAFFFF9A b       22E8E24h              ///022E8E24 LBL
022E8FB8 EB0020A1 bl      22F1244h              ///022F1244
022E8FBC E3A00000 mov     r0,0h
022E8FC0 EB00080C bl      22EAFF8h
022E8FC4 EB005697 bl      22FEA28h
022E8FC8 EB003B52 bl      22F7D18h
022E8FCC EB004B8C bl      22FBE04h
022E8FD0 EB0051A5 bl      22FD66Ch
022E8FD4 EBF46A82 bl      20039E4h
022E8FD8 EAFFFE60 b       22E8960h
022E8FDC E3A00000 mov     r0,0h
022E8FE0 EBF46A15 bl      200383Ch
022E8FE4 EBF52DC9 bl      2034710h
022E8FE8 EB008FC4 bl      230CF00h
022E8FEC E3A00000 mov     r0,0h
022E8FF0 EB0003F4 bl      22E9FC8h
022E8FF4 E3500002 cmp     r0,2h
022E8FF8 1A000002 bne     22E9008h              ///022E9008
022E8FFC E3A00001 mov     r0,1h
022E9000 EBF59807 bl      204F024h
022E9004 EA000001 b       22E9010h
022E9008 E3A00000 mov     r0,0h
022E900C EBF59804 bl      204F024h
022E9010 EB008FAF bl      230CED4h
022E9014 EB00207E bl      22F1214h
022E9018 EB00063F bl      22EA91Ch
022E901C EB003B6C bl      22F7DD4h
022E9020 EB004B89 bl      22FBE4Ch
022E9024 EB0051A2 bl      22FD6B4h
022E9028 EB005690 bl      22FEA70h
022E902C EB003827 bl      22F70D0h
022E9030 EBF57C05 bl      204804Ch
022E9034 EB002DEE bl      22F47F4h
022E9038 EBFFCDD8 bl      22DC7A0h
022E903C EB0007EC bl      22EAFF4h
022E9040 EB000200 bl      22E9848h
022E9044 EB000A56 bl      22EB9A4h
022E9048 E3A00000 mov     r0,0h
022E904C E1A01000 mov     r1,r0
022E9050 EBF45F86 bl      2000E70h
022E9054 E59F0194 ldr     r0,=2324CB0h
022E9058 E5900004 ldr     r0,[r0,4h]
022E905C EBF4609D bl      20012D8h
022E9060 E59F0188 ldr     r0,=2324CB0h
022E9064 E5900008 ldr     r0,[r0,8h]
022E9068 EBF4609A bl      20012D8h
022E906C E59F0174 ldr     r0,=2324FA0h
022E9070 E5D01000 ldrb    r1,[r0]
022E9074 E3510000 cmp     r1,0h
022E9078 0A000004 beq     22E9090h
022E907C E590001C ldr     r0,[r0,1Ch]
022E9080 E3500008 cmp     r0,8h
022E9084 03A0000F moveq   r0,0Fh
022E9088 13A0000E movne   r0,0Eh
022E908C EA000053 b       22E91E0h
022E9090 E5D01004 ldrb    r1,[r0,4h]
022E9094 E590001C ldr     r0,[r0,1Ch]
022E9098 E3510000 cmp     r1,0h
022E909C 0A00000B beq     22E90D0h
022E90A0 E3500006 cmp     r0,6h
022E90A4 0A000005 beq     22E90C0h
022E90A8 E3500007 cmp     r0,7h
022E90AC 1A000003 bne     22E90C0h
022E90B0 E59F0184 ldr     r0,=2320A60h
022E90B4 EBF48C50 bl      200C1FCh
022E90B8 E3A0000D mov     r0,0Dh
022E90BC EA000047 b       22E91E0h
022E90C0 E59F0178 ldr     r0,=2320A70h
022E90C4 EBF48C4C bl      200C1FCh
022E90C8 E3A0000C mov     r0,0Ch
022E90CC EA000043 b       22E91E0h
022E90D0 E3500003 cmp     r0,3h
022E90D4 0A000002 beq     22E90E4h
022E90D8 E3500004 cmp     r0,4h
022E90DC 0A00001A beq     22E914Ch
022E90E0 EA000022 b       22E9170h
022E90E4 E3A00000 mov     r0,0h
022E90E8 E3A0102C mov     r1,2Ch
022E90EC E3A0200F mov     r2,0Fh
022E90F0 EBF589CA bl      204B820h
022E90F4 E3A00000 mov     r0,0h
022E90F8 E3A0102B mov     r1,2Bh
022E90FC E3A0200F mov     r2,0Fh
022E9100 EBF589C6 bl      204B820h
022E9104 EBF5957C bl      204E6FCh
022E9108 E1A05000 mov     r5,r0
022E910C EBF59593 bl      204E760h
022E9110 E1A04000 mov     r4,r0
022E9114 E3A00000 mov     r0,0h
022E9118 E3A01027 mov     r1,27h
022E911C E1A02004 mov     r2,r4
022E9120 EBF589BE bl      204B820h
022E9124 E3A00000 mov     r0,0h
022E9128 E3A01029 mov     r1,29h
022E912C E1A02004 mov     r2,r4
022E9130 EBF589BA bl      204B820h
022E9134 E1A01005 mov     r1,r5
022E9138 E1A02004 mov     r2,r4
022E913C E59F0100 ldr     r0,=2320A7Ch
022E9140 EBF48C2D bl      200C1FCh
022E9144 E3A00004 mov     r0,4h
022E9148 EA000024 b       22E91E0h
022E914C EBF5956A bl      204E6FCh
022E9150 E1A04000 mov     r4,r0
022E9154 EBF59581 bl      204E760h
022E9158 E1A02000 mov     r2,r0
022E915C E59F00E4 ldr     r0,=2320A94h
022E9160 E1A01004 mov     r1,r4
022E9164 EBF48C24 bl      200C1FCh
022E9168 E3A00005 mov     r0,5h
022E916C EA00001B b       22E91E0h
022E9170 E3A00000 mov     r0,0h
022E9174 E3A0102C mov     r1,2Ch
022E9178 E3A02001 mov     r2,1h
022E917C EBF589A7 bl      204B820h
022E9180 E59F0060 ldr     r0,=2324FA0h
022E9184 E590101C ldr     r1,[r0,1Ch]
022E9188 E3510005 cmp     r1,5h
022E918C 1A000003 bne     22E91A0h
022E9190 E59F00B4 ldr     r0,=2320AACh
022E9194 EBF48C18 bl      200C1FCh
022E9198 E3A00009 mov     r0,9h
022E919C EA00000F b       22E91E0h
022E91A0 E3510007 cmp     r1,7h
022E91A4 1A00000A bne     22E91D4h
022E91A8 E5D00001 ldrb    r0,[r0,1h]
022E91AC E3500000 cmp     r0,0h
022E91B0 0A000003 beq     22E91C4h
022E91B4 E59F0090 ldr     r0,=2320AACh
022E91B8 EBF48C0F bl      200C1FCh
022E91BC E3A00009 mov     r0,9h
022E91C0 EA000006 b       22E91E0h
022E91C4 E59F0084 ldr     r0,=2320AB8h
022E91C8 EBF48C0B bl      200C1FCh
022E91CC E3A0000B mov     r0,0Bh
022E91D0 EA000002 b       22E91E0h
022E91D4 E59F0078 ldr     r0,=2320AC8h
022E91D8 EBF48C07 bl      200C1FCh
022E91DC E3A0000A mov     r0,0Ah
022E91E0 E28DD034 add     r13,r13,34h
022E91E4 E8BD8FF0 pop     r4-r11,r15
022E91E8 02324FA0
022E91EC 023209B0
022E91F0 02324CB0
022E91F4 02324FC0
022E91F8 022E93C0
022E91FC 022E935C
022E9200 022E9258
022E9204 023209CC
022E9208 023209EC
022E920C 02320A08
022E9210 0000029D
022E9214 0000029E
022E9218 0000029F
022E921C 0000029B
022E9220 0000029A
022E9224 02320A28
022E9228 000002A5
022E922C 000002A1
022E9230 000002A6
022E9234 02320A44
022E9238 00000F0F
022E923C 02320A60
022E9240 02320A70
022E9244 02320A7C
022E9248 02320A94
022E924C 02320AAC
022E9250 02320AB8
022E9254 02320AC8


/*
    fun_022F766C 22F766Ch
	Run Script
*/
022F766C E92D4038 push    r3-r5,r14
022F7670 E24DD020 sub     r13,r13,20h
022F7674 E59F053C ldr     r0,=2324CF4h          //ScriptEngineRuntimeData
022F7678 E5901000 ldr     r1,[r0]
022F767C E59100F0 ldr     r0,[r1,0F0h]
022F7680 E3500002 cmp     r0,2h
022F7684 1A00009B bne     22F78F8h
022F7688 E59120F4 ldr     r2,[r1,0F4h]
022F768C E59F1528 ldr     r1,=2321644h          //"GroundSupervision execute B %d"
022F7690 E3A00002 mov     r0,2h
022F7694 EBF452E9 bl      200C240h              //DebugPrint
022F7698 E59F0518 ldr     r0,=2324CF4h          //ScriptEngineRuntimeData
022F769C E3A04000 mov     r4,0h
022F76A0 E5901000 ldr     r1,[r0]
022F76A4 E58140F0 str     r4,[r1,0F0h]
022F76A8 E5901000 ldr     r1,[r0]
022F76AC E59100F4 ldr     r0,[r1,0F4h]
022F76B0 E3500008 cmp     r0,8h
022F76B4 908FF100 addls   r15,r15,r0,lsl 2h
022F76B8 EA000130 b       22F7B80h          //R0 > 8 022F7B80
022F76BC EA00012F b       22F7B80h          //0x00 022F7B80
022F76C0 EA00012E b       22F7B80h          //0x01 022F7B80
022F76C4 EA00012D b       22F7B80h          //0x02 022F7B80
022F76C8 EA000004 b       22F76E0h          //0x03 022F76E0
022F76CC EA00001A b       22F773Ch          //0x04 022F773C
022F76D0 EA000039 b       22F77BCh          //0x05 022F77BC
022F76D4 EA000058 b       22F783Ch          //0x06 022F783C
022F76D8 EA000074 b       22F78B0h          //0x07 022F78B0
022F76DC EA00007F b       22F78E0h          //0x08 022F78E0
//0x03 022F76E0
022F76E0 E3A01001 mov     r1,1h
022F76E4 E28D0004 add     r0,r13,4h
022F76E8 E1A03001 mov     r3,r1
022F76EC E2412002 sub     r2,r1,2h
022F76F0 E58D4000 str     r4,[r13]
022F76F4 EBFFBA00 bl      22E5EFCh
022F76F8 E3500000 cmp     r0,0h
022F76FC 0A00011F beq     22F7B80h              //022F7B80
022F7700 E59F04B0 ldr     r0,=2324CF4h          //ScriptEngineRuntimeData
022F7704 E3A03001 mov     r3,1h
022F7708 E5900000 ldr     r0,[r0]
022F770C E28D2004 add     r2,r13,4h
022F7710 E1A01004 mov     r1,r4
022F7714 E1CD31BC strh    r3,[r13,1Ch]
022F7718 EBFF9573 bl      22DCCECh
022F771C E59F0494 ldr     r0,=2324CF4h          //ScriptEngineRuntimeData
022F7720 E3A03001 mov     r3,1h
022F7724 E5902000 ldr     r2,[r0]
022F7728 E1A00004 mov     r0,r4
022F772C E3A0100A mov     r1,0Ah
022F7730 E5C230EC strb    r3,[r2,0ECh]
022F7734 EB000438 bl      22F881Ch
022F7738 EA000110 b       22F7B80h
//0x04 022F773C
022F773C E2810C01 add     r0,r1,100h
022F7740 E1D040D4 ldrsb   r4,[r0,4h]
022F7744 E3A01001 mov     r1,1h
022F7748 E28D0004 add     r0,r13,4h
022F774C E1A03001 mov     r3,r1
022F7750 E2412002 sub     r2,r1,2h
022F7754 E58D4000 str     r4,[r13]
022F7758 EBFFB9E7 bl      22E5EFCh
022F775C E3500000 cmp     r0,0h
022F7760 0A000106 beq     22F7B80h
022F7764 E59F044C ldr     r0,=2324CF4h           //ScriptEngineRuntimeData
022F7768 E28D2004 add     r2,r13,4h
022F776C E5900000 ldr     r0,[r0]
022F7770 E5D01105 ldrb    r1,[r0,105h]
022F7774 E3510000 cmp     r1,0h
022F7778 13A03002 movne   r3,2h
022F777C 03A03001 moveq   r3,1h
022F7780 E3A01000 mov     r1,0h
022F7784 E1CD31BC strh    r3,[r13,1Ch]
022F7788 EBFF9557 bl      22DCCECh
022F778C E59F0424 ldr     r0,=2324CF4h           //ScriptEngineRuntimeData
022F7790 E3A02001 mov     r2,1h
022F7794 E5901000 ldr     r1,[r0]
022F7798 E5C120EC strb    r2,[r1,0ECh]
022F779C E5900000 ldr     r0,[r0]
022F77A0 E5D00105 ldrb    r0,[r0,105h]
022F77A4 E3500000 cmp     r0,0h
022F77A8 1A0000F4 bne     22F7B80h
022F77AC E3A00000 mov     r0,0h
022F77B0 E3A0100A mov     r1,0Ah
022F77B4 EB000418 bl      22F881Ch
022F77B8 EA0000F0 b       22F7B80h
//0x05 022F77BC
022F77BC E2810C01 add     r0,r1,100h
022F77C0 E1D040D4 ldrsb   r4,[r0,4h]
022F77C4 E3A01001 mov     r1,1h
022F77C8 E28D0004 add     r0,r13,4h
022F77CC E2412002 sub     r2,r1,2h
022F77D0 E3A03002 mov     r3,2h
022F77D4 E58D4000 str     r4,[r13]
022F77D8 EBFFB9C7 bl      22E5EFCh
022F77DC E3500000 cmp     r0,0h
022F77E0 0A0000E6 beq     22F7B80h
022F77E4 E59F03CC ldr     r0,=2324CF4h             //ScriptEngineRuntimeData
022F77E8 E28D2004 add     r2,r13,4h
022F77EC E5900000 ldr     r0,[r0]
022F77F0 E5D01105 ldrb    r1,[r0,105h]
022F77F4 E3510000 cmp     r1,0h
022F77F8 13A03002 movne   r3,2h
022F77FC 03A03001 moveq   r3,1h
022F7800 E3A01000 mov     r1,0h
022F7804 E1CD31BC strh    r3,[r13,1Ch]
022F7808 EBFF9537 bl      22DCCECh
022F780C E59F03A4 ldr     r0,=2324CF4h             //ScriptEngineRuntimeData
022F7810 E3A02001 mov     r2,1h
022F7814 E5901000 ldr     r1,[r0]
022F7818 E5C120EC strb    r2,[r1,0ECh]
022F781C E5900000 ldr     r0,[r0]
022F7820 E5D00105 ldrb    r0,[r0,105h]
022F7824 E3500000 cmp     r0,0h
022F7828 1A0000D4 bne     22F7B80h
022F782C E3A00000 mov     r0,0h
022F7830 E3A0100A mov     r1,0Ah
022F7834 EB0003F8 bl      22F881Ch
022F7838 EA0000D0 b       22F7B80h
//0x06 022F783C
022F783C E2810C01 add     r0,r1,100h
022F7840 E1D040D4 ldrsb   r4,[r0,4h]
022F7844 E3A01001 mov     r1,1h
022F7848 E28D0004 add     r0,r13,4h
022F784C E2412002 sub     r2,r1,2h
022F7850 E3A03003 mov     r3,3h
022F7854 E58D4000 str     r4,[r13]
022F7858 EBFFB9A7 bl      22E5EFCh
022F785C E3500000 cmp     r0,0h
022F7860 0A00000E beq     22F78A0h
022F7864 E59F034C ldr     r0,=2324CF4h				//ScriptEngineRuntimeData
022F7868 E28D2004 add     r2,r13,4h
022F786C E5900000 ldr     r0,[r0]
022F7870 E5D01105 ldrb    r1,[r0,105h]
022F7874 E3510000 cmp     r1,0h
022F7878 13A03002 movne   r3,2h
022F787C 03A03001 moveq   r3,1h
022F7880 E3A01000 mov     r1,0h
022F7884 E1CD31BC strh    r3,[r13,1Ch]
022F7888 EBFF9517 bl      22DCCECh
022F788C E59F0324 ldr     r0,=2324CF4h				//ScriptEngineRuntimeData
022F7890 E3A01001 mov     r1,1h
022F7894 E5900000 ldr     r0,[r0]
022F7898 E5C010EC strb    r1,[r0,0ECh]
022F789C EA0000B7 b       22F7B80h
022F78A0 E3A00000 mov     r0,0h
022F78A4 E3A0100A mov     r1,0Ah
022F78A8 EB0003DB bl      22F881Ch
022F78AC EA0000B3 b       22F7B80h
//0x07 022F78B0
022F78B0 E5D10105 ldrb    r0,[r1,105h]
022F78B4 E3500000 cmp     r0,0h
022F78B8 13A00002 movne   r0,2h
022F78BC 03A00001 moveq   r0,1h
022F78C0 E1A00800 mov     r0,r0,lsl 10h
022F78C4 E1A00840 mov     r0,r0,asr 10h
022F78C8 EBFFB8C9 bl      22E5BF4h
022F78CC E59F02E4 ldr     r0,=2324CF4h				//ScriptEngineRuntimeData
022F78D0 E3A01001 mov     r1,1h
022F78D4 E5900000 ldr     r0,[r0]
022F78D8 E5C010EC strb    r1,[r0,0ECh]
022F78DC EA0000A7 b       22F7B80h
//0x08 022F78E0
022F78E0 EBFFB8E7 bl      22E5C84h
022F78E4 E59F02CC ldr     r0,=2324CF4h				//ScriptEngineRuntimeData
022F78E8 E3A01001 mov     r1,1h
022F78EC E5900000 ldr     r0,[r0]
022F78F0 E5C010EC strb    r1,[r0,0ECh]
022F78F4 EA0000A1 b       22F7B80h
022F78F8 E3500000 cmp     r0,0h
022F78FC 1A00009F bne     22F7B80h
022F7900 E591210C ldr     r2,[r1,10Ch]
022F7904 E3520000 cmp     r2,0h
022F7908 0A00009C beq     22F7B80h
022F790C E59F12AC ldr     r1,=2321664h          //"GroundSupervision execute A %d"
022F7910 E3A00002 mov     r0,2h
022F7914 EBF45249 bl      200C240h              //DebugPrint
022F7918 E59FC298 ldr     r12,=2324CF4h			//ScriptEngineRuntimeData
022F791C E59C5000 ldr     r5,[r12]
022F7920 E2854F43 add     r4,r5,10Ch
022F7924 E285E0F4 add     r14,r5,0F4h
022F7928 E8B4000F ldmia   [r4]!,r0-r3
022F792C E8AE000F stmia   [r14]!,r0-r3
022F7930 E8940003 ldmia   [r4],r0,r1
022F7934 E88E0003 stmia   [r14],r0,r1
022F7938 E3A01001 mov     r1,1h
022F793C E58510F0 str     r1,[r5,0F0h]
022F7940 E59C2000 ldr     r2,[r12]
022F7944 E3A00000 mov     r0,0h
022F7948 E582010C str     r0,[r2,10Ch]
022F794C E59C3000 ldr     r3,[r12]
022F7950 E59320F4 ldr     r2,[r3,0F4h]
022F7954 E3520009 cmp     r2,9h
022F7958 908FF102 addls   r15,r15,r2,lsl 2h
022F795C EA000085 b       22F7B78h
022F7960 EA000084 b       22F7B78h
022F7964 EA000007 b       22F7988h
022F7968 EA000006 b       22F7988h
022F796C EA00001D b       22F79E8h
022F7970 EA000026 b       22F7A10h
022F7974 EA000034 b       22F7A4Ch
022F7978 EA000043 b       22F7A8Ch
022F797C EA000056 b       22F7ADCh
022F7980 EA000069 b       22F7B2Ch
022F7984 EA000077 b       22F7B68h
022F7988 E1D31FF8 ldrsh   r1,[r3,0F8h]
022F798C E28D0004 add     r0,r13,4h
022F7990 EBFFC188 bl      22E7FB8h
022F7994 E3500000 cmp     r0,0h
022F7998 0A00000D beq     22F79D4h
022F799C E59F0214 ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F79A0 E28D2004 add     r2,r13,4h
022F79A4 E5900000 ldr     r0,[r0]
022F79A8 E5D01105 ldrb    r1,[r0,105h]
022F79AC E3510000 cmp     r1,0h
022F79B0 13A03002 movne   r3,2h
022F79B4 03A03001 moveq   r3,1h
022F79B8 E3A01000 mov     r1,0h
022F79BC E1CD31BC strh    r3,[r13,1Ch]
022F79C0 EBFF94C9 bl      22DCCECh
022F79C4 E59F01EC ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F79C8 E3A01001 mov     r1,1h
022F79CC E5900000 ldr     r0,[r0]
022F79D0 E5C010EC strb    r1,[r0,0ECh]
022F79D4 E59F01DC ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F79D8 E3A01000 mov     r1,0h
022F79DC E5900000 ldr     r0,[r0]
022F79E0 E58010F0 str     r1,[r0,0F0h]
022F79E4 EA000065 b       22F7B80h
022F79E8 E1A00003 mov     r0,r3
022F79EC EBFF9455 bl      22DCB48h
022F79F0 E59F01C0 ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F79F4 E3A02001 mov     r2,1h
022F79F8 E5901000 ldr     r1,[r0]
022F79FC E5C120EC strb    r2,[r1,0ECh]
022F7A00 E5900000 ldr     r0,[r0]
022F7A04 E1D00FFA ldrsh   r0,[r0,0FAh]
022F7A08 EBFFB5CE bl      22E5148h
022F7A0C EA00005B b       22F7B80h
022F7A10 E5D30105 ldrb    r0,[r3,105h]
022F7A14 E3500000 cmp     r0,0h
022F7A18 13A01002 movne   r1,2h
022F7A1C E1A01801 mov     r1,r1,lsl 10h
022F7A20 E1A00003 mov     r0,r3
022F7A24 E1A01841 mov     r1,r1,asr 10h
022F7A28 EBFF9446 bl      22DCB48h
022F7A2C E59F0184 ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F7A30 E3A02001 mov     r2,1h
022F7A34 E5901000 ldr     r1,[r0]
022F7A38 E5C120EC strb    r2,[r1,0ECh]
022F7A3C E5900000 ldr     r0,[r0]
022F7A40 E1D00FFA ldrsh   r0,[r0,0FAh]
022F7A44 EBFFB5BF bl      22E5148h
022F7A48 EA00004C b       22F7B80h
022F7A4C E5D30105 ldrb    r0,[r3,105h]
022F7A50 E3500000 cmp     r0,0h
022F7A54 13A01002 movne   r1,2h
022F7A58 E1A01801 mov     r1,r1,lsl 10h
022F7A5C E1A00003 mov     r0,r3
022F7A60 E1A01841 mov     r1,r1,asr 10h
022F7A64 EBFF9437 bl      22DCB48h
022F7A68 E59F0148 ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F7A6C E3A02001 mov     r2,1h
022F7A70 E5901000 ldr     r1,[r0]
022F7A74 E5C120EC strb    r2,[r1,0ECh]
022F7A78 E5901000 ldr     r1,[r0]
022F7A7C E1D10FFA ldrsh   r0,[r1,0FAh]
022F7A80 E28110FC add     r1,r1,0FCh
022F7A84 EBFFB5BF bl      22E5188h
022F7A88 EA00003C b       22F7B80h
022F7A8C E3A0100B mov     r1,0Bh
022F7A90 EB000361 bl      22F881Ch
022F7A94 E59F011C ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F7A98 E5900000 ldr     r0,[r0]
022F7A9C E5D01105 ldrb    r1,[r0,105h]
022F7AA0 E3510000 cmp     r1,0h
022F7AA4 13A01002 movne   r1,2h
022F7AA8 03A01001 moveq   r1,1h
022F7AAC E1A01801 mov     r1,r1,lsl 10h
022F7AB0 E1A01841 mov     r1,r1,asr 10h
022F7AB4 EBFF9423 bl      22DCB48h
022F7AB8 E59F00F8 ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F7ABC E3A02001 mov     r2,1h
022F7AC0 E5901000 ldr     r1,[r0]
022F7AC4 E5C120EC strb    r2,[r1,0ECh]
022F7AC8 E5901000 ldr     r1,[r0]
022F7ACC E1D10FFA ldrsh   r0,[r1,0FAh]
022F7AD0 E28110FC add     r1,r1,0FCh
022F7AD4 EBFFB60E bl      22E5314h
022F7AD8 EA000028 b       22F7B80h
022F7ADC E3A0100B mov     r1,0Bh
022F7AE0 EB00034D bl      22F881Ch
022F7AE4 E59F00CC ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F7AE8 E5900000 ldr     r0,[r0]
022F7AEC E5D01105 ldrb    r1,[r0,105h]
022F7AF0 E3510000 cmp     r1,0h
022F7AF4 13A01002 movne   r1,2h
022F7AF8 03A01001 moveq   r1,1h
022F7AFC E1A01801 mov     r1,r1,lsl 10h
022F7B00 E1A01841 mov     r1,r1,asr 10h
022F7B04 EBFF940F bl      22DCB48h
022F7B08 E59F00A8 ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F7B0C E3A02001 mov     r2,1h
022F7B10 E5901000 ldr     r1,[r0]
022F7B14 E5C120EC strb    r2,[r1,0ECh]
022F7B18 E5900000 ldr     r0,[r0]
022F7B1C E2800C01 add     r0,r0,100h
022F7B20 E1D000F6 ldrsh   r0,[r0,6h]
022F7B24 EBFFB62E bl      22E53E4h
022F7B28 EA000014 b       22F7B80h
022F7B2C E3A0100B mov     r1,0Bh
022F7B30 EB000339 bl      22F881Ch
022F7B34 E59F007C ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F7B38 E3A01001 mov     r1,1h
022F7B3C E5900000 ldr     r0,[r0]
022F7B40 EBFF9400 bl      22DCB48h
022F7B44 E59F006C ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F7B48 E3A02001 mov     r2,1h
022F7B4C E5901000 ldr     r1,[r0]
022F7B50 E5C120EC strb    r2,[r1,0ECh]
022F7B54 E5900000 ldr     r0,[r0]
022F7B58 E2800C01 add     r0,r0,100h
022F7B5C E1D000F6 ldrsh   r0,[r0,6h]
022F7B60 EBFFB61F bl      22E53E4h
022F7B64 EA000005 b       22F7B80h
022F7B68 E1D30FFA ldrsh   r0,[r3,0FAh]
022F7B6C E28310FC add     r1,r3,0FCh
022F7B70 EBFFB5B8 bl      22E5258h
022F7B74 EA000001 b       22F7B80h
022F7B78 E3A00000 mov     r0,0h
022F7B7C E58300F0 str     r0,[r3,0F0h]
///022F7B80 22F7B80h
022F7B80 E59F0030 ldr     r0,=2324CF4h					//ScriptEngineRuntimeData
022F7B84 E5900000 ldr     r0,[r0]
022F7B88 E59010F0 ldr     r1,[r0,0F0h]
022F7B8C E3510000 cmp     r1,0h
022F7B90 1A000000 bne     22F7B98h
022F7B94 EBFF9572 bl      22DD164h                      //Calls the function that calls script opcode parsing 22F7B94h
022F7B98 E59F0018 ldr     r0,=2324CF4h                  //ScriptEngineRuntimeData
022F7B9C E5900000 ldr     r0,[r0]
022F7BA0 EBFF9366 bl      22DC940h                      //022DC940 IsLastReturnCode!=0
{//022DC940
    022DC940 E1D000FE ldrsh   r0,[r0,0Eh]
    022DC944 E3500000 cmp     r0,0h
    022DC948 13A00001 movne   r0,1h
    022DC94C 03A00000 moveq   r0,0h
    022DC950 E20000FF and     r0,r0,0FFh
    022DC954 E12FFF1E bx      r14
}
022F7BA4 E59F100C ldr     r1,=2324CF4h					//ScriptEngineRuntimeData
022F7BA8 E5911000 ldr     r1,[r1]
022F7BAC E5C100EC strb    r0,[r1,0ECh]                  //LastReturnCode = IsLastReturnCode!=0
022F7BB0 E28DD020 add     r13,r13,20h
022F7BB4 E8BD8038 pop     r3-r5,r15
022F7BB8 02324CF4
022F7BBC 02321644
022F7BC0 02321664


/*
    fun_22F37D0
*/
022F37D0 E92D4008 push    r3,r14
022F37D4 E59F066C ldr     r0,=2324CE0h
022F37D8 E5903000 ldr     r3,[r0]
022F37DC E5930000 ldr     r0,[r3]
022F37E0 E3500001 cmp     r0,1h
022F37E4 1A00017D bne     22F3DE0h
022F37E8 E1D300F4 ldrsh   r0,[r3,4h]
022F37EC E3500022 cmp     r0,22h
022F37F0 908FF100 addls   r15,r15,r0,lsl 2h
022F37F4 EA000179 b       22F3DE0h
022F37F8 EA000178 b       22F3DE0h
022F37FC EA00002A b       22F38ACh
022F3800 EA000176 b       22F3DE0h
022F3804 EA000175 b       22F3DE0h
022F3808 EA000033 b       22F38DCh
022F380C EA00004A b       22F393Ch
022F3810 EA000061 b       22F399Ch
022F3814 EA0000D9 b       22F3B80h
022F3818 EA000170 b       22F3DE0h
022F381C EA00016F b       22F3DE0h
022F3820 EA00014A b       22F3D50h
022F3824 EA00016D b       22F3DE0h
022F3828 EA00016C b       22F3DE0h
022F382C EA0000EE b       22F3BECh
022F3830 EA0000E1 b       22F3BBCh
022F3834 EA000169 b       22F3DE0h
022F3838 EA000081 b       22F3A44h
022F383C EA000108 b       22F3C64h
022F3840 EA000122 b       22F3CD0h
022F3844 EA000093 b       22F3A98h
022F3848 EA000017 b       22F38ACh
022F384C EA000067 b       22F39F0h
022F3850 EA00000B b       22F3884h
022F3854 EA000050 b       22F399Ch
022F3858 EA000160 b       22F3DE0h
022F385C EA00015F b       22F3DE0h
022F3860 EA00015E b       22F3DE0h
022F3864 EA00015D b       22F3DE0h
022F3868 EA00015C b       22F3DE0h
022F386C EA00015B b       22F3DE0h
022F3870 EA00015A b       22F3DE0h
022F3874 EA000159 b       22F3DE0h
022F3878 EA000158 b       22F3DE0h
022F387C EA000157 b       22F3DE0h
022F3880 EA00014A b       22F3DB0h
022F3884 E3A0000E mov     r0,0Eh
022F3888 EB008378 bl      2314670h
022F388C E59F05B4 ldr     r0,=2324CE0h
022F3890 E3A03001 mov     r3,1h
022F3894 E5902000 ldr     r2,[r0]
022F3898 E3A01002 mov     r1,2h
022F389C E5C23010 strb    r3,[r2,10h]
022F38A0 E5900000 ldr     r0,[r0]
022F38A4 E5801000 str     r1,[r0]
022F38A8 EA00014C b       22F3DE0h
022F38AC E59F1598 ldr     r1,=12Dh
022F38B0 E59F2598 ldr     r2,=8080000h
022F38B4 E2830E1A add     r0,r3,1A0h
022F38B8 EB000D68 bl      22F6E60h
022F38BC E59F0584 ldr     r0,=2324CE0h
022F38C0 E3A03001 mov     r3,1h
022F38C4 E5902000 ldr     r2,[r0]
022F38C8 E3A01002 mov     r1,2h
022F38CC E5C23011 strb    r3,[r2,11h]
022F38D0 E5900000 ldr     r0,[r0]
022F38D4 E5801000 str     r1,[r0]
022F38D8 EA000140 b       22F3DE0h
022F38DC E3A00001 mov     r0,1h
022F38E0 EB008362 bl      2314670h
022F38E4 E59F055C ldr     r0,=2324CE0h
022F38E8 E3A02001 mov     r2,1h
022F38EC E5901000 ldr     r1,[r0]
022F38F0 E5C12010 strb    r2,[r1,10h]
022F38F4 E5901000 ldr     r1,[r0]
022F38F8 E591033C ldr     r0,[r1,33Ch]
022F38FC E5911340 ldr     r1,[r1,340h]
022F3900 EB008831 bl      23159CCh
022F3904 E59F053C ldr     r0,=2324CE0h
022F3908 E59F2540 ldr     r2,=8080000h
022F390C E5900000 ldr     r0,[r0]
022F3910 E3A010BD mov     r1,0BDh
022F3914 E2800E1A add     r0,r0,1A0h
022F3918 EB000D50 bl      22F6E60h
022F391C E59F0524 ldr     r0,=2324CE0h
022F3920 E3A03001 mov     r3,1h
022F3924 E5902000 ldr     r2,[r0]
022F3928 E3A01002 mov     r1,2h
022F392C E5C23011 strb    r3,[r2,11h]
022F3930 E5900000 ldr     r0,[r0]
022F3934 E5801000 str     r1,[r0]
022F3938 EA000128 b       22F3DE0h
022F393C E3A00002 mov     r0,2h
022F3940 EB00834A bl      2314670h
022F3944 E59F04FC ldr     r0,=2324CE0h
022F3948 E3A02001 mov     r2,1h
022F394C E5901000 ldr     r1,[r0]
022F3950 E5C12010 strb    r2,[r1,10h]
022F3954 E5901000 ldr     r1,[r0]
022F3958 E591033C ldr     r0,[r1,33Ch]
022F395C E5911340 ldr     r1,[r1,340h]
022F3960 EB008819 bl      23159CCh
022F3964 E59F04DC ldr     r0,=2324CE0h
022F3968 E59F24E0 ldr     r2,=8080000h
022F396C E5900000 ldr     r0,[r0]
022F3970 E3A010BD mov     r1,0BDh
022F3974 E2800E1A add     r0,r0,1A0h
022F3978 EB000D38 bl      22F6E60h
022F397C E59F04C4 ldr     r0,=2324CE0h
022F3980 E3A03001 mov     r3,1h
022F3984 E5902000 ldr     r2,[r0]
022F3988 E3A01002 mov     r1,2h
022F398C E5C23011 strb    r3,[r2,11h]
022F3990 E5900000 ldr     r0,[r0]
022F3994 E5801000 str     r1,[r0]
022F3998 EA000110 b       22F3DE0h
022F399C E593133C ldr     r1,[r3,33Ch]
022F39A0 E5932340 ldr     r2,[r3,340h]
022F39A4 E3A00003 mov     r0,3h
022F39A8 EB008111 bl      2313DF4h
022F39AC E59F0494 ldr     r0,=2324CE0h
022F39B0 E3A03001 mov     r3,1h
022F39B4 E5901000 ldr     r1,[r0]
022F39B8 E59F2494 ldr     r2,=8080005h
022F39BC E5C13010 strb    r3,[r1,10h]
022F39C0 E5900000 ldr     r0,[r0]
022F39C4 E3A010B6 mov     r1,0B6h
022F39C8 E2800E1A add     r0,r0,1A0h
022F39CC EB000D23 bl      22F6E60h
022F39D0 E59F0470 ldr     r0,=2324CE0h
022F39D4 E3A03001 mov     r3,1h
022F39D8 E5902000 ldr     r2,[r0]
022F39DC E3A01002 mov     r1,2h
022F39E0 E5C23011 strb    r3,[r2,11h]
022F39E4 E5900000 ldr     r0,[r0]
022F39E8 E5801000 str     r1,[r0]
022F39EC EA0000FB b       22F3DE0h
022F39F0 E593133C ldr     r1,[r3,33Ch]
022F39F4 E5932340 ldr     r2,[r3,340h]
022F39F8 E3A00004 mov     r0,4h
022F39FC EB0080FC bl      2313DF4h
022F3A00 E59F0440 ldr     r0,=2324CE0h
022F3A04 E3A03001 mov     r3,1h
022F3A08 E5901000 ldr     r1,[r0]
022F3A0C E59F2440 ldr     r2,=8080005h
022F3A10 E5C13010 strb    r3,[r1,10h]
022F3A14 E5900000 ldr     r0,[r0]
022F3A18 E3A010BE mov     r1,0BEh
022F3A1C E2800E1A add     r0,r0,1A0h
022F3A20 EB000D0E bl      22F6E60h
022F3A24 E59F041C ldr     r0,=2324CE0h
022F3A28 E3A03001 mov     r3,1h
022F3A2C E5902000 ldr     r2,[r0]
022F3A30 E3A01002 mov     r1,2h
022F3A34 E5C23011 strb    r3,[r2,11h]
022F3A38 E5900000 ldr     r0,[r0]
022F3A3C E5801000 str     r1,[r0]
022F3A40 EA0000E6 b       22F3DE0h
022F3A44 E593133C ldr     r1,[r3,33Ch]
022F3A48 E5932340 ldr     r2,[r3,340h]
022F3A4C E3A00012 mov     r0,12h
022F3A50 EB0080E7 bl      2313DF4h
022F3A54 E59F03EC ldr     r0,=2324CE0h
022F3A58 E3A03001 mov     r3,1h
022F3A5C E5902000 ldr     r2,[r0]
022F3A60 E59F13F0 ldr     r1,=222h
022F3A64 E5C23010 strb    r3,[r2,10h]
022F3A68 E5900000 ldr     r0,[r0]
022F3A6C E3A02302 mov     r2,8000000h
022F3A70 E2800E1A add     r0,r0,1A0h
022F3A74 EB000CF9 bl      22F6E60h
022F3A78 E59F03C8 ldr     r0,=2324CE0h
022F3A7C E3A03001 mov     r3,1h
022F3A80 E5902000 ldr     r2,[r0]
022F3A84 E3A01002 mov     r1,2h
022F3A88 E5C23011 strb    r3,[r2,11h]
022F3A8C E5900000 ldr     r0,[r0]
022F3A90 E5801000 str     r1,[r0]
022F3A94 EA0000D1 b       22F3DE0h
022F3A98 EB02647D bl      238CC94h
022F3A9C E59F13A4 ldr     r1,=2324CE0h
022F3AA0 E5911000 ldr     r1,[r1]
022F3AA4 E581033C str     r0,[r1,33Ch]
022F3AA8 EB02647E bl      238CCA8h
022F3AAC E59F1394 ldr     r1,=2324CE0h
022F3AB0 E5911000 ldr     r1,[r1]
022F3AB4 E5810340 str     r0,[r1,340h]
022F3AB8 E3A00011 mov     r0,11h
022F3ABC EB0082EB bl      2314670h
022F3AC0 E59F0380 ldr     r0,=2324CE0h
022F3AC4 E3A03001 mov     r3,1h
022F3AC8 E5901000 ldr     r1,[r0]
022F3ACC E59F2388 ldr     r2,=0B040000h
022F3AD0 E5C13010 strb    r3,[r1,10h]
022F3AD4 E5900000 ldr     r0,[r0]
022F3AD8 E590133C ldr     r1,[r0,33Ch]
022F3ADC E2800018 add     r0,r0,18h
022F3AE0 E1A01801 mov     r1,r1,lsl 10h
022F3AE4 E1A01841 mov     r1,r1,asr 10h
022F3AE8 EB000833 bl      22F5BBCh
022F3AEC E59F0354 ldr     r0,=2324CE0h
022F3AF0 E59F2364 ldr     r2,=0B040000h
022F3AF4 E5901000 ldr     r1,[r0]
022F3AF8 E28100DC add     r0,r1,0DCh
022F3AFC E5911340 ldr     r1,[r1,340h]
022F3B00 E1A01801 mov     r1,r1,lsl 10h
022F3B04 E1A01841 mov     r1,r1,asr 10h
022F3B08 EB00082B bl      22F5BBCh
022F3B0C E59F0334 ldr     r0,=2324CE0h
022F3B10 E3A01B02 mov     r1,800h
022F3B14 E5900000 ldr     r0,[r0]
022F3B18 E3A02004 mov     r2,4h
022F3B1C E2800018 add     r0,r0,18h
022F3B20 EB0005F0 bl      22F52E8h
022F3B24 EB02645F bl      238CCA8h
022F3B28 EBF5842C bl      2054BE0h
022F3B2C E59F132C ldr     r1,=807h
022F3B30 EB000886 bl      22F5D50h
022F3B34 E1A00800 mov     r0,r0,lsl 10h
022F3B38 E1A01840 mov     r1,r0,asr 10h
022F3B3C E59F0304 ldr     r0,=2324CE0h
022F3B40 E3A02000 mov     r2,0h
022F3B44 E5900000 ldr     r0,[r0]
022F3B48 E28000DC add     r0,r0,0DCh
022F3B4C EB0005E5 bl      22F52E8h
022F3B50 E59F02F0 ldr     r0,=2324CE0h
022F3B54 E3A03001 mov     r3,1h
022F3B58 E5902000 ldr     r2,[r0]
022F3B5C E3A01002 mov     r1,2h
022F3B60 E5C23013 strb    r3,[r2,13h]
022F3B64 E5902000 ldr     r2,[r0]
022F3B68 E5C23014 strb    r3,[r2,14h]
022F3B6C E5902000 ldr     r2,[r0]
022F3B70 E5C23016 strb    r3,[r2,16h]
022F3B74 E5900000 ldr     r0,[r0]
022F3B78 E5801000 str     r1,[r0]
022F3B7C EA000097 b       22F3DE0h
022F3B80 E3A00005 mov     r0,5h
022F3B84 EB0082B9 bl      2314670h
022F3B88 E59F02B8 ldr     r0,=2324CE0h
022F3B8C E3A02001 mov     r2,1h
022F3B90 E5901000 ldr     r1,[r0]
022F3B94 E5C12010 strb    r2,[r1,10h]
022F3B98 E5901000 ldr     r1,[r0]
022F3B9C E591033C ldr     r0,[r1,33Ch]
022F3BA0 E5911340 ldr     r1,[r1,340h]
022F3BA4 EB008788 bl      23159CCh
022F3BA8 E59F0298 ldr     r0,=2324CE0h
022F3BAC E3A01002 mov     r1,2h
022F3BB0 E5900000 ldr     r0,[r0]
022F3BB4 E5801000 str     r1,[r0]
022F3BB8 EA000088 b       22F3DE0h
022F3BBC E593133C ldr     r1,[r3,33Ch]
022F3BC0 E5932340 ldr     r2,[r3,340h]
022F3BC4 E3A00007 mov     r0,7h
022F3BC8 EB008089 bl      2313DF4h
022F3BCC E59F0274 ldr     r0,=2324CE0h
022F3BD0 E3A03001 mov     r3,1h
022F3BD4 E5902000 ldr     r2,[r0]
022F3BD8 E3A01002 mov     r1,2h
022F3BDC E5C23010 strb    r3,[r2,10h]
022F3BE0 E5900000 ldr     r0,[r0]
022F3BE4 E5801000 str     r1,[r0]
022F3BE8 EA00007C b       22F3DE0h
022F3BEC EB0082A4 bl      2314684h
022F3BF0 E3500000 cmp     r0,0h
022F3BF4 0A00000B beq     22F3C28h
022F3BF8 EB0082AC bl      23146B0h
022F3BFC E3500000 cmp     r0,0h
022F3C00 1A000002 bne     22F3C10h
022F3C04 E59F0258 ldr     r0,=2321340h
022F3C08 EBF4617B bl      200C1FCh
022F3C0C EA000073 b       22F3DE0h
022F3C10 E59F0230 ldr     r0,=2324CE0h
022F3C14 E5901000 ldr     r1,[r0]
022F3C18 E591033C ldr     r0,[r1,33Ch]
022F3C1C E5911340 ldr     r1,[r1,340h]
022F3C20 EB0082AB bl      23146D4h
022F3C24 EA000006 b       22F3C44h
022F3C28 E3A0000C mov     r0,0Ch
022F3C2C EB00828F bl      2314670h
022F3C30 E59F0210 ldr     r0,=2324CE0h
022F3C34 E5901000 ldr     r1,[r0]
022F3C38 E591033C ldr     r0,[r1,33Ch]
022F3C3C E5911340 ldr     r1,[r1,340h]
022F3C40 EB008761 bl      23159CCh
022F3C44 E59F01FC ldr     r0,=2324CE0h
022F3C48 E3A03001 mov     r3,1h
022F3C4C E5902000 ldr     r2,[r0]
022F3C50 E3A01002 mov     r1,2h
022F3C54 E5C23010 strb    r3,[r2,10h]
022F3C58 E5900000 ldr     r0,[r0]
022F3C5C E5801000 str     r1,[r0]
022F3C60 EA00005E b       22F3DE0h
022F3C64 E3A0000F mov     r0,0Fh
022F3C68 EB008280 bl      2314670h
022F3C6C E59F01D4 ldr     r0,=2324CE0h
022F3C70 E3A03001 mov     r3,1h
022F3C74 E5901000 ldr     r1,[r0]
022F3C78 E59F21E8 ldr     r2,=0C080000h
022F3C7C E5C13010 strb    r3,[r1,10h]
022F3C80 E5900000 ldr     r0,[r0]
022F3C84 E3A010EE mov     r1,0EEh
022F3C88 E2800E1A add     r0,r0,1A0h
022F3C8C EB000C73 bl      22F6E60h
022F3C90 E59F01B0 ldr     r0,=2324CE0h
022F3C94 E59F21CC ldr     r2,=0C080000h
022F3C98 E5900000 ldr     r0,[r0]
022F3C9C E3A010EF mov     r1,0EFh
022F3CA0 E2800F99 add     r0,r0,264h
022F3CA4 EB000C6D bl      22F6E60h
022F3CA8 E59F0198 ldr     r0,=2324CE0h
022F3CAC E3A03001 mov     r3,1h
022F3CB0 E5902000 ldr     r2,[r0]
022F3CB4 E3A01002 mov     r1,2h
022F3CB8 E5C23011 strb    r3,[r2,11h]
022F3CBC E5902000 ldr     r2,[r0]
022F3CC0 E5C23012 strb    r3,[r2,12h]
022F3CC4 E5900000 ldr     r0,[r0]
022F3CC8 E5801000 str     r1,[r0]
022F3CCC EA000043 b       22F3DE0h
022F3CD0 E3A00010 mov     r0,10h
022F3CD4 EB008265 bl      2314670h
022F3CD8 E59F0168 ldr     r0,=2324CE0h
022F3CDC E3A02001 mov     r2,1h
022F3CE0 E5901000 ldr     r1,[r0]
022F3CE4 E5C12010 strb    r2,[r1,10h]
022F3CE8 E5901000 ldr     r1,[r0]
022F3CEC E591033C ldr     r0,[r1,33Ch]
022F3CF0 E5911340 ldr     r1,[r1,340h]
022F3CF4 EB008734 bl      23159CCh
022F3CF8 E59F0148 ldr     r0,=2324CE0h
022F3CFC E59F2164 ldr     r2,=0C080000h
022F3D00 E5900000 ldr     r0,[r0]
022F3D04 E3A010EC mov     r1,0ECh
022F3D08 E2800E1A add     r0,r0,1A0h
022F3D0C EB000C53 bl      22F6E60h
022F3D10 E59F0130 ldr     r0,=2324CE0h
022F3D14 E59F214C ldr     r2,=0C080000h
022F3D18 E5900000 ldr     r0,[r0]
022F3D1C E3A010ED mov     r1,0EDh
022F3D20 E2800F99 add     r0,r0,264h
022F3D24 EB000C4D bl      22F6E60h
022F3D28 E59F0118 ldr     r0,=2324CE0h
022F3D2C E3A03001 mov     r3,1h
022F3D30 E5902000 ldr     r2,[r0]
022F3D34 E3A01002 mov     r1,2h
022F3D38 E5C23011 strb    r3,[r2,11h]
022F3D3C E5902000 ldr     r2,[r0]
022F3D40 E5C23012 strb    r3,[r2,12h]
022F3D44 E5900000 ldr     r0,[r0]
022F3D48 E5801000 str     r1,[r0]
022F3D4C EA000023 b       22F3DE0h
022F3D50 E3A00008 mov     r0,8h
022F3D54 EB008245 bl      2314670h
022F3D58 E59F00E8 ldr     r0,=2324CE0h
022F3D5C E3A02001 mov     r2,1h
022F3D60 E5901000 ldr     r1,[r0]
022F3D64 E5C12010 strb    r2,[r1,10h]
022F3D68 E5901000 ldr     r1,[r0]
022F3D6C E591033C ldr     r0,[r1,33Ch]
022F3D70 E5911340 ldr     r1,[r1,340h]
022F3D74 EB008714 bl      23159CCh
022F3D78 E59F00C8 ldr     r0,=2324CE0h
022F3D7C E59F10E8 ldr     r1,=10Dh
022F3D80 E5900000 ldr     r0,[r0]
022F3D84 E59F20C4 ldr     r2,=8080000h
022F3D88 E2800E1A add     r0,r0,1A0h
022F3D8C EB000C33 bl      22F6E60h
022F3D90 E59F00B0 ldr     r0,=2324CE0h
022F3D94 E3A03001 mov     r3,1h
022F3D98 E5902000 ldr     r2,[r0]
022F3D9C E3A01002 mov     r1,2h
022F3DA0 E5C23011 strb    r3,[r2,11h]
022F3DA4 E5900000 ldr     r0,[r0]
022F3DA8 E5801000 str     r1,[r0]
022F3DAC EA00000B b       22F3DE0h
022F3DB0 E3A0000B mov     r0,0Bh
022F3DB4 EB00822D bl      2314670h
022F3DB8 E59F0088 ldr     r0,=2324CE0h
022F3DBC E5901000 ldr     r1,[r0]
022F3DC0 E591033C ldr     r0,[r1,33Ch]
022F3DC4 E5911340 ldr     r1,[r1,340h]
022F3DC8 EB0086FF bl      23159CCh
022F3DCC E59F0074 ldr     r0,=2324CE0h
022F3DD0 E3A01002 mov     r1,2h
022F3DD4 E5900000 ldr     r0,[r0]
022F3DD8 E5801000 str     r1,[r0]
022F3DDC EB0001C5 bl      22F44F8h
022F3DE0 E59F0060 ldr     r0,=2324CE0h
022F3DE4 E5901000 ldr     r1,[r0]
022F3DE8 E5D10354 ldrb    r0,[r1,354h]
022F3DEC E3500000 cmp     r0,0h
022F3DF0 08BD8008 popeq   r3,r15
022F3DF4 E1D100F6 ldrsh   r0,[r1,6h]
022F3DF8 E2400018 sub     r0,r0,18h
022F3DFC E3500007 cmp     r0,7h
022F3E00 908FF100 addls   r15,r15,r0,lsl 2h
022F3E04 EA00000A b       22F3E34h
022F3E08 EA000006 b       22F3E28h
022F3E0C EA000005 b       22F3E28h
022F3E10 EA000004 b       22F3E28h
022F3E14 EA000005 b       22F3E30h
022F3E18 EA000004 b       22F3E30h
022F3E1C EA000003 b       22F3E30h
022F3E20 EA000002 b       22F3E30h
022F3E24 EA000001 b       22F3E30h
022F3E28 EBF5DB60 bl      206ABB0h
022F3E2C EA000000 b       22F3E34h
022F3E30 EBFF3CDB bl      22C31A4h
022F3E34 E59F000C ldr     r0,=2324CE0h
022F3E38 E3A01000 mov     r1,0h
022F3E3C E5900000 ldr     r0,[r0]
022F3E40 E5C01354 strb    r1,[r0,354h]
022F3E44 E8BD8008 pop     r3,r15
022F3E48 02324CE0
022F3E4C 0000012D
022F3E50 08080000
022F3E54 08080005
022F3E58 00000222
022F3E5C 0B040000
022F3E60 00000807
022F3E64 02321340
022F3E68 0C080000
022F3E6C 0000010D


/*
2324C6Ch - Pointer to current SSA file buffer.
2324C8Ch - Pointer to current SSS file buffer.
2324C94h - Pointer to current SSE file buffer.
*/
/*
    fun_022E4CD4 22E4CD4h ScriptLocateSet
    
    (r0=3,r1=0,r2=0)
    
referred to there:
022E5A08
022E5A30
022E5A38
022E5A50
02AE5A38
02AE5A50

	Line 759427: 022E5A08 EBFFFCB1 bl      22E4CD4h
	Line 759437: 022E5A30 EBFFFCA7 bl      22E4CD4h
	Line 759439: 022E5A38 E59FC00C ldr     r12,=22E4CD4h
	Line 759445: 022E5A50 E59FC00C ldr     r12,=22E4CD4h
	Line 2856591: 02AE5A38 E59FC00C ldr     r12,=22E4CD4h
	Line 2856597: 02AE5A50 E59FC00C ldr     r12,=22E4CD4h
    
    
    SSE = 1,
    SSS = 2,
    SSA = 3,
    
    r0 = Param0
    r1 = Param1
    r2 = LayerID : Is the layer ID!
    
*/
//R0 = (possibly ScriptDataTy?), R1 = LayerID, R2 = Param3 (Possibly a boolean)
022E4CD4 E92D4FF0 push    r4-r11,r14
022E4CD8 E24DD044 sub     r13,r13,44h
022E4CDC E1A09001 mov     r9,r1                 //R9 = LayerID (was 0, was also 5)
022E4CE0 E1A0A000 mov     r10,r0                //R10 = ScriptDataTy (was 3)
022E4CE4 E1A08002 mov     r8,r2                 //R8 = Param3 (was 0)
022E4CE8 E59F12D4 ldr     r1,=2319714h          //"script locate set %3d %3d"
022E4CEC E1A0200A mov     r2,r10                //R2 = ScriptDataTy
022E4CF0 E1A03009 mov     r3,r9                 //R3 = LayerID
022E4CF4 E3A00002 mov     r0,2h                 
022E4CF8 EBF49D50 bl      200C240h              ///DebugPrint( 2,  )
022E4CFC E35A0002 cmp     r10,2h
if( ScriptDataTy == 2 )                             //2 == SSS
    022E4D00 059F02C0 ldreq   r0,=2324C6Ch          //SSA struct address
    022E4D04 05904020 ldreq   r4,[r0,20h]           // 0x02324C6C + 0x20 == 2324C8Ch -> Pointer to current SSS file buffer.
    022E4D08 0A000004 beq     22E4D20h              ///022E4D20 LBL1 v
022E4D0C E35A0003 cmp     r10,3h
if( ScriptDataTy == 3 )                             //3 == SSA
    022E4D10 059F02B0 ldreq   r0,=2324C6Ch          //SSA struct address
    022E4D14 05904000 ldreq   r4,[r0]               //R4 = PtrFileBuff
else
    022E4D18 159F02A8 ldrne   r0,=2324C6Ch          //SSA struct address
    022E4D1C 15904028 ldrne   r4,[r0,28h]           // 0x02324C6C + 0x28 == 2324C94h -> Pointer to current SSE file buffer.
///022E4D20 LBL1
022E4D20 E1D410B2 ldrh    r1,[r4,2h]            //R1 = ptrlayers
022E4D24 E3A0000A mov     r0,0Ah                //R0 = Possibly (entrysize / 2)?
022E4D28 E3E0B000 mvn     r11,0h
022E4D2C E0201099 mla     r0,r9,r0,r1           //R0 = (LayerID * 0xA) + ptrlayers
022E4D30 E0845080 add     r5,r4,r0,lsl 1h       //R5 = PtrFileBuff + (((LayerID * 0xA) + ptrlayers) * 2) (AKA PtrCurrentLayer!!)
022E4D34 E1A00080 mov     r0,r0,lsl 1h          //R0 *= 2
022E4D38 E19400B0 ldrh    r0,[r4,r0]            //r0 = nbactors for current layer
022E4D3C E1D510B2 ldrh    r1,[r5,2h]            //r1 = ptractors for current layer
022E4D40 E3A07000 mov     r7,0h                 //r7 = CntActors = 0
022E4D44 E58D0008 str     r0,[r13,8h]           //Put nbactors on the stack at r13+8
022E4D48 E28B0801 add     r0,r11,10000h         //R0 = 0x0000FFFF
022E4D4C E0846081 add     r6,r4,r1,lsl 1h       //r6 = PtrFileBuff + (ptractors * 2) = PtrActor
022E4D50 E58D0010 str     r0,[r13,10h]          //Put 0x0000FFFF on stack at r13+16
022E4D54 EA000019 b       22E4DC0h              ///022E4DC0 LBL2 v
///022E4D58 LBL3_ParseActor
022E4D58 E1D600B0 ldrh    r0,[r6]               //r0 = actorid
022E4D5C E1A0200A mov     r2,r10                //ScriptDataTy
022E4D60 E1A03009 mov     r3,r9                 //r3 = LayerID
022E4D64 E1CD03B8 strh    r0,[r13,38h]          //[r13,38h] = actorid
022E4D68 E1D600B2 ldrh    r0,[r6,2h]            //r0 = direction
022E4D6C E2400001 sub     r0,r0,1h              //r0 = direction - 1 
022E4D70 E5CD003A strb    r0,[r13,3Ah]          //[r13,3Ah] = (direction - 1)
022E4D74 E1D600B4 ldrh    r0,[r6,4h]            //r0 = xoffset
022E4D78 E5CD003B strb    r0,[r13,3Bh]          //[r13,3Bh] = xoffset
022E4D7C E1D600B6 ldrh    r0,[r6,6h]            //r0 = yoffset
022E4D80 E5CD003C strb    r0,[r13,3Ch]          //[r13,3Ch] = yoffset
022E4D84 E1D600B8 ldrh    r0,[r6,8h]            //r0 = actunk0x08
022E4D88 E5CD003D strb    r0,[r13,3Dh]          //[r13,3Dh] = actunk0x08
022E4D8C E1D600BA ldrh    r0,[r6,0Ah]           //r0 = actunk0x0A
022E4D90 E5CD003E strb    r0,[r13,3Eh]          //[r13,3Eh] = actunk0x0A
022E4D94 E1D610BC ldrh    r1,[r6,0Ch]           //r1 = scriptid
022E4D98 E59D0010 ldr     r0,[r13,10h]          //r0 = 0x0000FFFF    
022E4D9C E1510000 cmp     r1,r0                 //
if( scriptid == -1 )
    022E4DA0 01CDB4B0 streqh  r11,[r13,40h]     //[r13,40h] = -1
else
    022E4DA4 11CD14B0 strneh  r1,[r13,40h]      //[r13,40h] = scriptid
022E4DA8 E1A0000B mov     r0,r11                //r0 = 0x0000FFFF
022E4DAC E28D1038 add     r1,r13,38h            //r1 = r13 + 38h
022E4DB0 E58D8000 str     r8,[r13]              //[r13] = Param3
//  struct ActorEntryMemory
//      0x00 actorid
//      0x02 direction
//      0x03 xoffset
//      0x04 yoffset
//      0x05 actunk0x08
//      0x06 actunk0x0A
//      0x07 --
//      0x08 scriptid
022E4DB4 EB004C2F bl      22F7E78h              ///fun_022F7E78 GroundLiveAdd
022E4DB8 E2877001 add     r7,r7,1h              //R7 = CntActors + 1
022E4DBC E2866010 add     r6,r6,10h             //r6 = PtrActor + 16
///022E4DC0 LBL2
022E4DC0 E59D0008 ldr     r0,[r13,8h]           //R0 = nbactors
022E4DC4 E1570000 cmp     r7,r0                 //is CntActors < nbactors
if( CntActors < nbactors )
    022E4DC8 BAFFFFE2 blt     22E4D58h          ///022E4D58 LBL3_ParseActor ^
022E4DCC E1D500B4 ldrh    r0,[r5,4h]            //R0 = [PtrCurrentLayer + 4] = nbobjects
022E4DD0 E1D510B6 ldrh    r1,[r5,6h]            //R1 = ptrobjects
022E4DD4 E3E0B000 mvn     r11,0h                //R11 = 0xFFFFFFFF
022E4DD8 E58D0004 str     r0,[r13,4h]           //Put nbobjects on stack at r13+4
022E4DDC E28B0801 add     r0,r11,10000h         //R0 = 0x0001FFFF
022E4DE0 E3A07000 mov     r7,0h                 //R7 = CntObjects = 0
022E4DE4 E0846081 add     r6,r4,r1,lsl 1h       //R6 = PtrFileBuff + (ptrobjects * 2) = PtrObject
022E4DE8 E58D0014 str     r0,[r13,14h]          //Put 0x0001FFFF at r13+20
022E4DEC EA00001D b       22E4E68h              ///022E4E68 LBL4 v
///022E4DF0 LBL5_ParseObject
022E4DF0 E1D600B0 ldrh    r0,[r6]               //R0 = objectid
022E4DF4 E1A0200A mov     r2,r10                //R2 = ScriptDataTy
022E4DF8 E1A03009 mov     r3,r9                 //R3 = LayerID
022E4DFC E1CD02BC strh    r0,[r13,2Ch]          //[r13,2Ch] = objectid
022E4E00 E1D600B2 ldrh    r0,[r6,2h]            //R0 = direction
022E4E04 E2400001 sub     r0,r0,1h              //r0 = direction - 1
022E4E08 E5CD002E strb    r0,[r13,2Eh]          //[r13,2Eh] = (direction - 1)
022E4E0C E1D600B4 ldrh    r0,[r6,4h]            //r0 = objunk0x04
022E4E10 E5CD002F strb    r0,[r13,2Fh]          //[r13,2Fh] = objunk0x04
022E4E14 E1D600B6 ldrh    r0,[r6,6h]            //r0 = objunk0x06
022E4E18 E5CD0030 strb    r0,[r13,30h]          //[r13,30h] = objunk0x06
022E4E1C E1D600B8 ldrh    r0,[r6,8h]            //r0 = xoffset
022E4E20 E5CD0031 strb    r0,[r13,31h]          //[r13,31h] = xoffset
022E4E24 E1D600BA ldrh    r0,[r6,0Ah]           //r0 = yoffset
022E4E28 E5CD0032 strb    r0,[r13,32h]          //[r13,32h] = yoffset
022E4E2C E1D600BC ldrh    r0,[r6,0Ch]           //r0 = objunk0x0C
022E4E30 E5CD0033 strb    r0,[r13,33h]          //[r13,33h] = objunk0x0C
022E4E34 E1D600BE ldrh    r0,[r6,0Eh]           //r0 = objunk0x0E
022E4E38 E5CD0034 strb    r0,[r13,34h]          //[r13,34h] = objunk0x0E
022E4E3C E1D611B0 ldrh    r1,[r6,10h]           //r1 = scriptid
022E4E40 E59D0014 ldr     r0,[r13,14h]          //r0 = 0x0001FFFF
022E4E44 E1510000 cmp     r1,r0
if( scriptid == 0x0001FFFF )
    022E4E48 01CDB3B6 streqh  r11,[r13,36h]     //[r13,36h] = 0xFFFFFFFF
else
    022E4E4C 11CD13B6 strneh  r1,[r13,36h]      //[r13,36h] = scriptid
022E4E50 E1A0000B mov     r0,r11                //R0 = 0xFFFFFFFF
022E4E54 E28D102C add     r1,r13,2Ch            //R1 = r13 + 2Ch
022E4E58 E58D8000 str     r8,[r13]              //[r13] = Param3
022E4E5C EB005C18 bl      22FBEC4h              ///fun_022FBEC4 GroundObjectAdd ( 0xFFFFFFFF, PtrIndxSev, ScriptDataTy, Param2 ) "GroundObject Add id %3d  kind %3d[%3d]  type %3d  hanger %3d  sector %3d"
022E4E60 E2877001 add     r7,r7,1h              //R7 = CntObjects + 1
022E4E64 E2866014 add     r6,r6,14h             //r6 = PtrObject + 20
///022E4E68 LBL4
022E4E68 E59D0004 ldr     r0,[r13,4h]           //R0 = nbobjects
022E4E6C E1570000 cmp     r7,r0                 //CntObjects < nbobjects
if( CntObjects < nbobjects )
    022E4E70 BAFFFFDE blt     22E4DF0h          ///022E4DF0 LBL5_ParseObject ^
022E4E74 E1D500BA ldrh    r0,[r5,0Ah]           //R0  = ptrperf
022E4E78 E1D5B0B8 ldrh    r11,[r5,8h]           //R11 = nbperf
022E4E7C E3A07000 mov     r7,0h                 //R7  = CntPerformers = 0
022E4E80 E0846080 add     r6,r4,r0,lsl 1h       //R6 = PtrFileBuff + ( ptrperf * 2 ) = PtrPerformer
022E4E84 E3E00000 mvn     r0,0h                 //R0 = 0xFFFFFFFF
022E4E88 E58D000C str     r0,[r13,0Ch]          //[r13,0Ch] = 0xFFFFFFFF
022E4E8C EA000018 b       22E4EF4h              ///022E4EF4 LBL6 v
///022E4E90 LBL7_ReadPerformer
022E4E90 E1D620B0 ldrh    r2,[r6]               //r2 = perfid
022E4E94 E59D000C ldr     r0,[r13,0Ch]          //R0 = 0xFFFFFFFF 
022E4E98 E28D1022 add     r1,r13,22h            //R1 = R13 + 0x22
022E4E9C E1CD22B2 strh    r2,[r13,22h]          //[r13,22h] = perfid
022E4EA0 E1D6C0B2 ldrh    r12,[r6,2h]           //R12 = direction
022E4EA4 E1A0200A mov     r2,r10                //R2 = ScriptDataTy?
022E4EA8 E1A03009 mov     r3,r9                 //R3 = LayerID
022E4EAC E24CC001 sub     r12,r12,1h            //R12 = direction - 1
022E4EB0 E5CDC024 strb    r12,[r13,24h]         //[r13,24h] = (direction - 1)
022E4EB4 E1D6C0B4 ldrh    r12,[r6,4h]           //r12 = perfunk0x04
022E4EB8 E5CDC025 strb    r12,[r13,25h]         //[r13,25h] = perfunk0x04
022E4EBC E1D6C0B6 ldrh    r12,[r6,6h]           //r12 = perfunk0x06
022E4EC0 E5CDC026 strb    r12,[r13,26h]         //[r13,26h] = perfunk0x06
022E4EC4 E1D6C0B8 ldrh    r12,[r6,8h]           //r12 = perfunk0x08    
022E4EC8 E5CDC027 strb    r12,[r13,27h]         //[r13,27h] = perfunk0x08
022E4ECC E1D6C0BA ldrh    r12,[r6,0Ah]          //r12 = perfunk0x0A
022E4ED0 E5CDC028 strb    r12,[r13,28h]         //[r13,28h] = perfunk0x0A
022E4ED4 E1D6C0BC ldrh    r12,[r6,0Ch]          //r12 = perfunk0x0C
022E4ED8 E5CDC029 strb    r12,[r13,29h]         //[r13,29h] = perfunk0x0C
022E4EDC E1D6C0BE ldrh    r12,[r6,0Eh]          //r12 = perfunk0x0E
022E4EE0 E5CDC02A strb    r12,[r13,2Ah]         //[r13,2Ah] = perfunk0x0E
022E4EE4 E58D8000 str     r8,[r13]              //Put Param3 at the current stack position
//  struct PerformerEntryMemory
//      0x00 perfid
//      0x02 direction
//      0x03 perfunk0x04
//      0x04 perfunk0x06
//      0x05 perfunk0x08
//      0x06 perfunk0x0A
//      0x07 perfunk0x0C
//      0x08 perfunk0x0E    
022E4EE8 EB00620F bl      22FD72Ch              ///fun_022FD72C GroundPerformerAdd( 0xFFFFFFFF, (R13 + 0x22), ScriptDataTy, Param2, PtrFileBuff )
022E4EEC E2877001 add     r7,r7,1h              //R7 = CntPerformers + 1
022E4EF0 E2866014 add     r6,r6,14h             //PtrPerformer = PtrPerformer + 20
///022E4EF4 LBL6
022E4EF4 E157000B cmp     r7,r11                //CntPerformers < nbperf 
if( CntPerformers < nbperf )
    022E4EF8 BAFFFFE4 blt     22E4E90h          ///022E4E90 LBL7_ReadPerformer ^
022E4EFC E24A0001 sub     r0,r10,1h             //R0 = ScriptDataTy? - 1
022E4F00 E1A00800 mov     r0,r0,lsl 10h         //R0 = R0 << 16
022E4F04 E1A00840 mov     r0,r0,asr 10h         //R0 = R0 >> 16 (keep sign)
022E4F08 E1A00800 mov     r0,r0,lsl 10h         //R0 = R0 << 16
022E4F0C E1A00820 mov     r0,r0,lsr 10h         //R0 = R0 >> 16 (discard sign)
022E4F10 E3500001 cmp     r0,1h                 //R0 > 1 (unsigned)
if( R0 > 1 )
    022E4F14 8A000027 bhi     22E4FB8h          ///022E4FB8 LBL8_Return v
022E4F18 E1D500BE ldrh    r0,[r5,0Eh]           //R0 = ptrevents
022E4F1C E1D570BC ldrh    r7,[r5,0Ch]           //R7 = nbevents
022E4F20 E3E05000 mvn     r5,0h                 //R5 = 0xFFFFFFFF
022E4F24 E3A08000 mov     r8,0h                 //R8 = CntEvents = 0
022E4F28 E0846080 add     r6,r4,r0,lsl 1h       //R6 = PtrFileBuff + (ptrevents * 2) = PtrEvent
022E4F2C E285B902 add     r11,r5,8000h          //r11 = 0x7FFF (Overflow of one bit!)
022E4F30 EA00001E b       22E4FB0h              ///022E4FB0 LBL9 v
///22E4F34h LBL10_ParseEvent 
022E4F34 E1D610BC ldrh    r1,[r6,0Ch]           //R1 = evntunk0x0C
022E4F38 E3110902 tst     r1,8000h
if( evntunk0x0C & 0x8000 != 0 )
    022E4F3C 1001000B andne   r0,r1,r11         //r0 = evntunk0x0C + 0x7FFF
    022E4F40 11CD01B8 strneh  r0,[r13,18h]      //[r13,18h] = (evntunk0x0C + 0x7FFF)
    022E4F44 11CD52B0 strneh  r5,[r13,20h]      //[r13,20h] = 0xFFFFFFFF
    022E4F48 1A000005 bne     22E4F64h          ///022E4F64
022E4F4C E1A00081 mov     r0,r1,lsl 1h          //r0 = evntunk0x0C * 2
022E4F50 E19400F0 ldrsh   r0,[r4,r0]            //r0 = croutineid (From a trigger in the TriggersTable)
022E4F54 E0841081 add     r1,r4,r1,lsl 1h       //r1 = PtrFileBuff + (evntunk0x0C * 2)
022E4F58 E1CD01B8 strh    r0,[r13,18h]          //[r13,18h] = croutineid (From a trigger in the TriggersTable)
022E4F5C E1D100F6 ldrsh   r0,[r1,6h]            //r0 = scriptid (From a trigger in the TriggersTable)
022E4F60 E1CD02B0 strh    r0,[r13,20h]          //[r13,20h] = scriptid (From a trigger in the TriggersTable)
///022E4F64
022E4F64 E1D620B0 ldrh    r2,[r6]               //r2 = evntunk0x00
022E4F68 E1A00005 mov     r0,r5                 //R0 = 0xFFFFFFFF
022E4F6C E28D1018 add     r1,r13,18h            //R1 = PtrEventStructCopy
022E4F70 E5CD201A strb    r2,[r13,1Ah]          //[r13,1Ah] = evntunk0x00
022E4F74 E1D6C0B2 ldrh    r12,[r6,2h]           //r12 = evntunk0x02
022E4F78 E1A0200A mov     r2,r10                //R2 = ScriptDataTy
022E4F7C E1A03009 mov     r3,r9                 //r3 = LayerID
022E4F80 E5CDC01B strb    r12,[r13,1Bh]         //[r13,1Bh] = evntunk0x02
022E4F84 E1D6C0B4 ldrh    r12,[r6,4h]           //r12 = evntunk0x04
022E4F88 E5CDC01C strb    r12,[r13,1Ch]         //[r13,1Ch] = evntunk0x04
022E4F8C E1D6C0B6 ldrh    r12,[r6,6h]           //r12 = evntunk0x06
022E4F90 E5CDC01D strb    r12,[r13,1Dh]         //[r13,1Dh] = evntunk0x06
022E4F94 E1D6C0B8 ldrh    r12,[r6,8h]           //r12 = evntunk0x08
022E4F98 E5CDC01E strb    r12,[r13,1Eh]         //[r13,1Eh] = evntunk0x08
022E4F9C E1D6C0BA ldrh    r12,[r6,0Ah]          //r12 = evntunk0x0A
022E4FA0 E5CDC01F strb    r12,[r13,1Fh]         //[r13,1Fh] = evntunk0x0A
//  struct EventEntry
//      0x00 trigger croutineid
//      0x02 evntunk0x00
//      0x03 evntunk0x02
//      0x04 evntunk0x04
//      0x05 evntunk0x06
//      0x06 evntunk0x08
//      0x07 evntunk0x0A
//      0x08 trigger scriptid
022E4FA4 EB0066BB bl      22FEA98h              ///022FEA98 GroundEventAdd( 0xFFFFFFFF, PtrEventStructCopy, ScriptDataTy, Param2 )
022E4FA8 E2888001 add     r8,r8,1h              //R8 = CntEvents + 1
022E4FAC E2866010 add     r6,r6,10h             //R6 = PtrEvent + 16
///022E4FB0 LBL9
022E4FB0 E1580007 cmp     r8,r7
if( CntEvents < nbevents )
    022E4FB4 BAFFFFDE blt     22E4F34h          ///22E4F34h LBL10_ParseEvent ^
///022E4FB8 LBL8_Return
022E4FB8 E3A00001 mov     r0,1h
022E4FBC E28DD044 add     r13,r13,44h
022E4FC0 E8BD8FF0 pop     r4-r11,r15
022E4FC4 02319714 
022E4FC8 02324C6C 


/*
    fun_022F7E78 22F7E78h GroundLiveAdd
    r1 = ActorEntryMemory
    
//  struct ActorEntryMemory
//      0x00 actorid
//      0x02 direction
//      0x03 xoffset
//      0x04 yoffset
//      0x05 actunk0x08
//      0x06 actunk0x0A
//      0x07 --
//      0x08 scriptid
*/
022F7E78 E92D4FF0 push    r4-r11,r14
022F7E7C E24DD014 sub     r13,r13,14h
022F7E80 E1A08001 mov     r8,r1             //r1 = ActorEntryMemory
022F7E84 E1D840B0 ldrh    r4,[r8]           //r4 = actorid
022F7E88 E1A09000 mov     r9,r0             //r9 = 
022F7E8C E28D0012 add     r0,r13,12h        
022F7E90 E28D1010 add     r1,r13,10h        
022F7E94 E1CD41B2 strh    r4,[r13,12h]      //[r13,12h] = actorid
022F7E98 E1A0A002 mov     r10,r2
022F7E9C E1A0B003 mov     r11,r3
022F7EA0 EBF5B57D bl      206549Ch          ///0206549C
{
    0206549C E92D4070 push    r4-r6,r14
    020654A0 E24DD010 sub     r13,r13,10h
    020654A4 E1A06000 mov     r6,r0
    020654A8 E1A05001 mov     r5,r1
    020654AC EBFFFEE7 bl      2065050h
    {
        02065050 E92D4070 push    r4-r6,r14
        02065054 E24DD010 sub     r13,r13,10h
        02065058 E1A05000 mov     r5,r0
        0206505C E1D500F0 ldrsh   r0,[r5]
        02065060 E3E04000 mvn     r4,0h
        02065064 E350002E cmp     r0,2Eh
        02065068 059F0424 ldreq   r0,=20B0B08h
        0206506C 01D001F8 ldreqsh r0,[r0,18h]
        02065070 01C500B0 streqh  r0,[r5]
        02065074 0A00000D beq     20650B0h
        02065078 E350002F cmp     r0,2Fh
        0206507C 059F0410 ldreq   r0,=20B0B08h
        02065080 01D001F6 ldreqsh r0,[r0,16h]
        02065084 01C500B0 streqh  r0,[r5]
        02065088 0A000008 beq     20650B0h
        0206508C E3500030 cmp     r0,30h
        02065090 059F03FC ldreq   r0,=20B0B08h
        02065094 01D001F4 ldreqsh r0,[r0,14h]
        02065098 01C500B0 streqh  r0,[r5]
        0206509C 0A000003 beq     20650B0h
        020650A0 E3500031 cmp     r0,31h
        020650A4 059F03E8 ldreq   r0,=20B0B08h
        020650A8 01D001F2 ldreqsh r0,[r0,12h]
        020650AC 01C500B0 streqh  r0,[r5]
        020650B0 E1D520F0 ldrsh   r2,[r5]
        020650B4 E3E00000 mvn     r0,0h
        020650B8 E1520000 cmp     r2,r0
        020650BC 0A0000F2 beq     206548Ch
        020650C0 E3A0000C mov     r0,0Ch
        020650C4 E1610082 smulbb  r1,r2,r0
        020650C8 E59F03C8 ldr     r0,=20A7FF0h              ///ActorTablePtr
        020650CC E19000F1 ldrsh   r0,[r0,r1]
        020650D0 E3500001 cmp     r0,1h
        020650D4 1A000034 bne     20651ACh
        020650D8 E3520000 cmp     r2,0h
        020650DC 1A000030 bne     20651A4h
        020650E0 E3A00000 mov     r0,0h
        020650E4 E3A01034 mov     r1,34h
        020650E8 EBFF98FF bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
        020650EC E3500008 cmp     r0,8h
        020650F0 908FF100 addls   r15,r15,r0,lsl 2h
        020650F4 EA000027 b       2065198h
        020650F8 EA000007 b       206511Ch
        020650FC EA000009 b       2065128h
        02065100 EA000024 b       2065198h
        02065104 EA00000A b       2065134h
        02065108 EA00001C b       2065180h
        0206510C EA00001B b       2065180h
        02065110 EA00001A b       2065180h
        02065114 EA000019 b       2065180h
        02065118 EA000018 b       2065180h
        0206511C E3A00001 mov     r0,1h
        02065120 E1C500B0 strh    r0,[r5]
        02065124 EA00001E b       20651A4h
        02065128 E3A00002 mov     r0,2h
        0206512C E1C500B0 strh    r0,[r5]
        02065130 EA00001B b       20651A4h
        02065134 EBFFC5DA bl      20568A4h
        02065138 E1A04000 mov     r4,r0
        0206513C E3E00000 mvn     r0,0h
        02065140 E1540000 cmp     r4,r0
        02065144 03A00001 moveq   r0,1h
        02065148 01C500B0 streqh  r0,[r5]
        0206514C 0A000014 beq     20651A4h
        02065150 EBFFC154 bl      20556A8h
        02065154 E1540000 cmp     r4,r0
        02065158 03A00001 moveq   r0,1h
        0206515C 01C500B0 streqh  r0,[r5]
        02065160 0A00000F beq     20651A4h
        02065164 EBFFC160 bl      20556ECh
        02065168 E1540000 cmp     r4,r0
        0206516C 03A00002 moveq   r0,2h
        02065170 01C500B0 streqh  r0,[r5]
        02065174 13A00004 movne   r0,4h
        02065178 11C500B0 strneh  r0,[r5]
        0206517C EA000008 b       20651A4h
        02065180 E3A00000 mov     r0,0h
        02065184 E3A01034 mov     r1,34h
        02065188 EBFF98D7 bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
        0206518C E2800001 add     r0,r0,1h
        02065190 E1C500B0 strh    r0,[r5]
        02065194 EA000002 b       20651A4h
        02065198 E3E00000 mvn     r0,0h
        0206519C E1C500B0 strh    r0,[r5]
        020651A0 EA0000B9 b       206548Ch
        020651A4 E3A04000 mov     r4,0h
        020651A8 EA0000B6 b       2065488h
        020651AC E3500002 cmp     r0,2h
        020651B0 1A00005E bne     2065330h
        020651B4 E3A00000 mov     r0,0h
        020651B8 E3A01035 mov     r1,35h
        020651BC EBFF98CA bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
        020651C0 E1A04000 mov     r4,r0
        020651C4 E3A00000 mov     r0,0h
        020651C8 E3A01036 mov     r1,36h
        020651CC EBFF98C6 bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
        020651D0 E1D520F0 ldrsh   r2,[r5]
        020651D4 E242100A sub     r1,r2,0Ah
        020651D8 E1A01801 mov     r1,r1,lsl 10h
        020651DC E1A01841 mov     r1,r1,asr 10h
        020651E0 E1A01801 mov     r1,r1,lsl 10h
        020651E4 E1A01821 mov     r1,r1,lsr 10h
        020651E8 E3510001 cmp     r1,1h
        020651EC 8A000037 bhi     20652D0h
        020651F0 E352000A cmp     r2,0Ah
        020651F4 1A000002 bne     2065204h
        020651F8 E3540000 cmp     r4,0h
        020651FC 01A04000 moveq   r4,r0
        02065200 EA000002 b       2065210h
        02065204 E3540000 cmp     r4,0h
        02065208 03A00000 moveq   r0,0h
        0206520C E1A04000 mov     r4,r0
        02065210 E354000A cmp     r4,0Ah
        02065214 908FF104 addls   r15,r15,r4,lsl 2h
        02065218 EA000029 b       20652C4h
        0206521C EA000028 b       20652C4h
        02065220 EA000008 b       2065248h
        02065224 EA00000A b       2065254h
        02065228 EA000025 b       20652C4h
        0206522C EA00000B b       2065260h
        02065230 EA00001D b       20652ACh
        02065234 EA00001C b       20652ACh
        02065238 EA00001B b       20652ACh
        0206523C EA00001A b       20652ACh
        02065240 EA000019 b       20652ACh
        02065244 EA00001B b       20652B8h
        02065248 E3A0000C mov     r0,0Ch
        0206524C E1C500B0 strh    r0,[r5]
        02065250 EA00001E b       20652D0h
        02065254 E3A0000D mov     r0,0Dh
        02065258 E1C500B0 strh    r0,[r5]
        0206525C EA00001B b       20652D0h
        02065260 EBFFC58F bl      20568A4h
        02065264 E1A04000 mov     r4,r0
        02065268 E3E00000 mvn     r0,0h
        0206526C E1540000 cmp     r4,r0
        02065270 03A0000C moveq   r0,0Ch
        02065274 01C500B0 streqh  r0,[r5]
        02065278 0A000014 beq     20652D0h
        0206527C EBFFC109 bl      20556A8h
        02065280 E1540000 cmp     r4,r0
        02065284 03A0000C moveq   r0,0Ch
        02065288 01C500B0 streqh  r0,[r5]
        0206528C 0A00000F beq     20652D0h
        02065290 EBFFC115 bl      20556ECh
        02065294 E1540000 cmp     r4,r0
        02065298 03A0000D moveq   r0,0Dh
        0206529C 01C500B0 streqh  r0,[r5]
        020652A0 13A0000F movne   r0,0Fh
        020652A4 11C500B0 strneh  r0,[r5]
        020652A8 EA000008 b       20652D0h
        020652AC E284000B add     r0,r4,0Bh
        020652B0 E1C500B0 strh    r0,[r5]
        020652B4 EA000005 b       20652D0h
        020652B8 E3A00015 mov     r0,15h
        020652BC E1C500B0 strh    r0,[r5]
        020652C0 EA000002 b       20652D0h
        020652C4 E3E00000 mvn     r0,0h
        020652C8 E1C500B0 strh    r0,[r5]
        020652CC EA00006E b       206548Ch
        020652D0 E1D500F0 ldrsh   r0,[r5]
        020652D4 E240000C sub     r0,r0,0Ch
        020652D8 E3500009 cmp     r0,9h
        020652DC 908FF100 addls   r15,r15,r0,lsl 2h
        020652E0 EA00000F b       2065324h
        020652E4 EA000008 b       206530Ch
        020652E8 EA000009 b       2065314h
        020652EC EA00000C b       2065324h
        020652F0 EA000009 b       206531Ch
        020652F4 EA00000A b       2065324h
        020652F8 EA000009 b       2065324h
        020652FC EA000008 b       2065324h
        02065300 EA000007 b       2065324h
        02065304 EA000006 b       2065324h
        02065308 EA000003 b       206531Ch
        0206530C E3A04002 mov     r4,2h
        02065310 EA00005C b       2065488h
        02065314 E3A04001 mov     r4,1h
        02065318 EA00005A b       2065488h
        0206531C E3A04002 mov     r4,2h
        02065320 EA000058 b       2065488h
        02065324 E3E00000 mvn     r0,0h
        02065328 E1C500B0 strh    r0,[r5]
        0206532C EA000056 b       206548Ch
        02065330 E3500003 cmp     r0,3h
        02065334 1A000039 bne     2065420h
        02065338 E3520016 cmp     r2,16h
        0206533C BA000019 blt     20653A8h
        02065340 E3520019 cmp     r2,19h
        02065344 CA000017 bgt     20653A8h
        02065348 E3A0000D mov     r0,0Dh
        0206534C E2424016 sub     r4,r2,16h
        02065350 EBFE7ADE bl      2003ED0h
        02065354 E3500000 cmp     r0,0h
        02065358 0A000003 beq     206536Ch
        0206535C EB0A10E0 bl      22E96E4h
        02065360 E3500000 cmp     r0,0h
        02065364 13E00000 mvnne   r0,0h
        02065368 1A000047 bne     206548Ch
        0206536C E3A00005 mov     r0,5h
        02065370 EBFE9B66 bl      200C110h
        02065374 E3500000 cmp     r0,0h
        02065378 0A000004 beq     2065390h
        0206537C E1D500F0 ldrsh   r0,[r5]
        02065380 E3500017 cmp     r0,17h
        02065384 CA000022 bgt     2065414h
        02065388 E3E00000 mvn     r0,0h
        0206538C EA00003E b       206548Ch
        02065390 E28D0008 add     r0,r13,8h
        02065394 EBFFC69A bl      2056E04h
        02065398 E1540000 cmp     r4,r0
        0206539C AA00001C bge     2065414h
        020653A0 E3E00000 mvn     r0,0h
        020653A4 EA000038 b       206548Ch
        020653A8 E352001A cmp     r2,1Ah
        020653AC BA000018 blt     2065414h
        020653B0 E352001D cmp     r2,1Dh
        020653B4 CA000016 bgt     2065414h
        020653B8 E3A0000D mov     r0,0Dh
        020653BC E242401A sub     r4,r2,1Ah
        020653C0 EBFE7AC2 bl      2003ED0h
        020653C4 E3500000 cmp     r0,0h
        020653C8 0A000003 beq     20653DCh
        020653CC EB0A10C4 bl      22E96E4h
        020653D0 E3500000 cmp     r0,0h
        020653D4 13E00000 mvnne   r0,0h
        020653D8 1A00002B bne     206548Ch
        020653DC E3A00005 mov     r0,5h
        020653E0 EBFE9B4A bl      200C110h
        020653E4 E3500000 cmp     r0,0h
        020653E8 0A000004 beq     2065400h
        020653EC E1D500F0 ldrsh   r0,[r5]
        020653F0 E350001C cmp     r0,1Ch
        020653F4 CA000006 bgt     2065414h
        020653F8 E3E00000 mvn     r0,0h
        020653FC EA000022 b       206548Ch
        02065400 E28D0000 add     r0,r13,0h
        02065404 EBFFC659 bl      2056D70h
        02065408 E1540000 cmp     r4,r0
        0206540C B3E00000 mvnlt   r0,0h
        02065410 BA00001D blt     206548Ch
        02065414 E3E00000 mvn     r0,0h
        02065418 E1C500B0 strh    r0,[r5]
        0206541C EA00001A b       206548Ch
        02065420 EBFFC51F bl      20568A4h
        02065424 E1D510F0 ldrsh   r1,[r5]
        02065428 E1A06000 mov     r6,r0
        0206542C E351003F cmp     r1,3Fh
        02065430 1A000014 bne     2065488h
        02065434 E3E01000 mvn     r1,0h
        02065438 E1560001 cmp     r6,r1
        0206543C 03A0003C moveq   r0,3Ch
        02065440 01C500B0 streqh  r0,[r5]
        02065444 0A00000F beq     2065488h
        02065448 EBFFBFF0 bl      2055410h
        0206544C E3500000 cmp     r0,0h
        02065450 13A0003C movne   r0,3Ch
        02065454 11C500B0 strneh  r0,[r5]
        02065458 1A00000A bne     2065488h
        0206545C E1A00006 mov     r0,r6
        02065460 EBFFC003 bl      2055474h
        02065464 E3500000 cmp     r0,0h
        02065468 13A0003D movne   r0,3Dh
        0206546C 11C500B0 strneh  r0,[r5]
        02065470 1A000004 bne     2065488h
        02065474 E1A00006 mov     r0,r6
        02065478 EBFFC016 bl      20554D8h
        0206547C E3500000 cmp     r0,0h
        02065480 13A0003E movne   r0,3Eh
        02065484 11C500B0 strneh  r0,[r5]
        02065488 E1A00004 mov     r0,r4
        0206548C E28DD010 add     r13,r13,10h
        02065490 E8BD8070 pop     r4-r6,r15
        02065494 020B0B08 
        02065498 020A7FF0 
    }
    020654B0 E1A04000 mov     r4,r0
    020654B4 E3A00000 mov     r0,0h
    020654B8 E1C500B0 strh    r0,[r5]
    020654BC E1D6E0F0 ldrsh   r14,[r6]
    020654C0 E2401001 sub     r1,r0,1h
    020654C4 E15E0001 cmp     r14,r1
    020654C8 02400001 subeq   r0,r0,1h
    020654CC 0A000122 beq     206595Ch
    020654D0 E3A0100C mov     r1,0Ch
    020654D4 E163018E smulbb  r3,r14,r1
    020654D8 E59FC484 ldr     r12,=20A7FF0h              ///ActorTablePtr
    020654DC E19C20F3 ldrsh   r2,[r12,r3]
    020654E0 E08C3003 add     r3,r12,r3
    020654E4 E3520003 cmp     r2,3h
    020654E8 1A000071 bne     20656B4h
    020654EC E35E0016 cmp     r14,16h
    020654F0 BA000031 blt     20655BCh
    020654F4 E35E0019 cmp     r14,19h
    020654F8 CA00002F bgt     20655BCh
    020654FC E3A0000D mov     r0,0Dh
    02065500 E24E4016 sub     r4,r14,16h
    02065504 EBFE7A71 bl      2003ED0h
    02065508 E3500000 cmp     r0,0h
    0206550C 0A00000B beq     2065540h
    02065510 EB0A1073 bl      22E96E4h
    02065514 E3500000 cmp     r0,0h
    02065518 0A000008 beq     2065540h
    0206551C E59F0444 ldr     r0,=20A68BCh
    02065520 E1A01084 mov     r1,r4,lsl 1h
    02065524 E19000F1 ldrsh   r0,[r0,r1]
    02065528 E1C500B0 strh    r0,[r5]
    0206552C E3500000 cmp     r0,0h
    02065530 03E00000 mvneq   r0,0h
    02065534 01C600B0 streqh  r0,[r6]
    02065538 E3E00000 mvn     r0,0h
    0206553C EA000106 b       206595Ch
    02065540 E3A00005 mov     r0,5h
    02065544 EBFE9AF1 bl      200C110h
    02065548 E3500000 cmp     r0,0h
    0206554C 0A00000B beq     2065580h
    02065550 E1D600F0 ldrsh   r0,[r6]
    02065554 E3500016 cmp     r0,16h
    02065558 03A0005F moveq   r0,5Fh
    0206555C 01C500B0 streqh  r0,[r5]
    02065560 02400060 subeq   r0,r0,60h
    02065564 0A0000FC beq     206595Ch
    02065568 E3500017 cmp     r0,17h
    0206556C 1A00004D bne     20656A8h
    02065570 E3A00022 mov     r0,22h
    02065574 E1C500B0 strh    r0,[r5]
    02065578 E2400023 sub     r0,r0,23h
    0206557C EA0000F6 b       206595Ch
    02065580 E28D0008 add     r0,r13,8h
    02065584 EBFFC61E bl      2056E04h
    02065588 E1540000 cmp     r4,r0
    0206558C AA000045 bge     20656A8h
    02065590 E59F03D4 ldr     r0,=20B0A48h
    02065594 E28D1008 add     r1,r13,8h
    02065598 E1A02084 mov     r2,r4,lsl 1h
    0206559C E19120F2 ldrsh   r2,[r1,r2]
    020655A0 E5903000 ldr     r3,[r0]
    020655A4 E3A01044 mov     r1,44h
    020655A8 E2410045 sub     r0,r1,45h
    020655AC E1013182 smlabb  r1,r2,r1,r3
    020655B0 E1D110F4 ldrsh   r1,[r1,4h]
    020655B4 E1C510B0 strh    r1,[r5]
    020655B8 EA0000E7 b       206595Ch
    020655BC E35E001A cmp     r14,1Ah
    020655C0 BA000038 blt     20656A8h
    020655C4 E35E001D cmp     r14,1Dh
    020655C8 CA000036 bgt     20656A8h
    020655CC E3A0000D mov     r0,0Dh
    020655D0 E24E401A sub     r4,r14,1Ah
    020655D4 EBFE7A3D bl      2003ED0h
    020655D8 E3500000 cmp     r0,0h
    020655DC 0A00000B beq     2065610h
    020655E0 EB0A103F bl      22E96E4h
    020655E4 E3500000 cmp     r0,0h
    020655E8 0A000008 beq     2065610h
    020655EC E59F0374 ldr     r0,=20A68BCh
    020655F0 E1A01084 mov     r1,r4,lsl 1h
    020655F4 E19000F1 ldrsh   r0,[r0,r1]
    020655F8 E1C500B0 strh    r0,[r5]
    020655FC E3500000 cmp     r0,0h
    02065600 03E00000 mvneq   r0,0h
    02065604 01C600B0 streqh  r0,[r6]
    02065608 E3E00000 mvn     r0,0h
    0206560C EA0000D2 b       206595Ch
    02065610 E3A00005 mov     r0,5h
    02065614 EBFE9ABD bl      200C110h
    02065618 E3500000 cmp     r0,0h
    0206561C 0A000012 beq     206566Ch
    02065620 E1D600F0 ldrsh   r0,[r6]
    02065624 E350001A cmp     r0,1Ah
    02065628 1A000004 bne     2065640h
    0206562C EBFFC059 bl      2055798h
    02065630 E1D010F4 ldrsh   r1,[r0,4h]
    02065634 E3E00000 mvn     r0,0h
    02065638 E1C510B0 strh    r1,[r5]
    0206563C EA0000C6 b       206595Ch
    02065640 E350001B cmp     r0,1Bh
    02065644 03A0005F moveq   r0,5Fh
    02065648 01C500B0 streqh  r0,[r5]
    0206564C 02400060 subeq   r0,r0,60h
    02065650 0A0000C1 beq     206595Ch
    02065654 E350001C cmp     r0,1Ch
    02065658 1A000012 bne     20656A8h
    0206565C E3A00022 mov     r0,22h
    02065660 E1C500B0 strh    r0,[r5]
    02065664 E2400023 sub     r0,r0,23h
    02065668 EA0000BB b       206595Ch
    0206566C E28D0000 add     r0,r13,0h
    02065670 EBFFC5BE bl      2056D70h
    02065674 E1540000 cmp     r4,r0
    02065678 AA00000A bge     20656A8h
    0206567C E59F02E8 ldr     r0,=20B0A48h
    02065680 E28D1000 add     r1,r13,0h
    02065684 E1A02084 mov     r2,r4,lsl 1h
    02065688 E19120F2 ldrsh   r2,[r1,r2]
    0206568C E5903000 ldr     r3,[r0]
    02065690 E3A01044 mov     r1,44h
    02065694 E2410045 sub     r0,r1,45h
    02065698 E1013182 smlabb  r1,r2,r1,r3
    0206569C E1D110F4 ldrsh   r1,[r1,4h]
    020656A0 E1C510B0 strh    r1,[r5]
    020656A4 EA0000AC b       206595Ch
    020656A8 E3E00000 mvn     r0,0h
    020656AC E1C600B0 strh    r0,[r6]
    020656B0 EA0000A9 b       206595Ch
    020656B4 E35E0035 cmp     r14,35h
    020656B8 CA00001D bgt     2065734h
    020656BC AA000077 bge     20658A0h
    020656C0 E35E0032 cmp     r14,32h
    020656C4 CA000013 bgt     2065718h
    020656C8 AA000064 bge     2065860h
    020656CC E35E000F cmp     r14,0Fh
    020656D0 908FF10E addls   r15,r15,r14,lsl 2h
    020656D4 EA00009B b       2065948h
    020656D8 EA00009A b       2065948h
    020656DC EA00002D b       2065798h
    020656E0 EA000030 b       20657A8h
    020656E4 EA000033 b       20657B8h
    020656E8 EA000036 b       20657C8h
    020656EC EA000095 b       2065948h
    020656F0 EA000094 b       2065948h
    020656F4 EA000093 b       2065948h
    020656F8 EA000092 b       2065948h
    020656FC EA000091 b       2065948h
    02065700 EA000090 b       2065948h
    02065704 EA00008F b       2065948h
    02065708 EA000022 b       2065798h
    0206570C EA000025 b       20657A8h
    02065710 EA000028 b       20657B8h
    02065714 EA00002B b       20657C8h
    02065718 E35E0033 cmp     r14,33h
    0206571C CA000001 bgt     2065728h
    02065720 0A000056 beq     2065880h
    02065724 EA000087 b       2065948h
    02065728 E35E0034 cmp     r14,34h
    0206572C 0A00007B beq     2065920h
    02065730 EA000084 b       2065948h
    02065734 E35E0037 cmp     r14,37h
    02065738 CA000003 bgt     206574Ch
    0206573C AA000067 bge     20658E0h
    02065740 E35E0036 cmp     r14,36h
    02065744 0A00005D beq     20658C0h
    02065748 EA00007E b       2065948h
    0206574C E35E0038 cmp     r14,38h
    02065750 CA000001 bgt     206575Ch
    02065754 0A000069 beq     2065900h
    02065758 EA00007A b       2065948h
    0206575C E24E2039 sub     r2,r14,39h
    02065760 E352000A cmp     r2,0Ah
    02065764 908FF102 addls   r15,r15,r2,lsl 2h
    02065768 EA000076 b       2065948h
    0206576C EA000033 b       2065840h
    02065770 EA000028 b       2065818h
    02065774 EA00002C b       206582Ch
    02065778 EA000006 b       2065798h
    0206577C EA000009 b       20657A8h
    02065780 EA00000C b       20657B8h
    02065784 EA00000F b       20657C8h
    02065788 EA000012 b       20657D8h
    0206578C EA000015 b       20657E8h
    02065790 EA000018 b       20657F8h
    02065794 EA00001B b       2065808h
    02065798 EBFFC009 bl      20557C4h
    0206579C E1D000F4 ldrsh   r0,[r0,4h]
    020657A0 E1C500B0 strh    r0,[r5]
    020657A4 EA00006B b       2065958h
    020657A8 EBFFC017 bl      205580Ch
    020657AC E1D000F4 ldrsh   r0,[r0,4h]
    020657B0 E1C500B0 strh    r0,[r5]
    020657B4 EA000067 b       2065958h
    020657B8 EBFFC025 bl      2055854h
    020657BC E1D000F4 ldrsh   r0,[r0,4h]
    020657C0 E1C500B0 strh    r0,[r5]
    020657C4 EA000063 b       2065958h
    020657C8 EBFFC451 bl      2056914h
    020657CC E1D000F4 ldrsh   r0,[r0,4h]
    020657D0 E1C500B0 strh    r0,[r5]
    020657D4 EA00005F b       2065958h
    020657D8 EBFFBFE4 bl      2055770h
    020657DC E1D000F4 ldrsh   r0,[r0,4h]
    020657E0 E1C500B0 strh    r0,[r5]
    020657E4 EA00005B b       2065958h
    020657E8 EBFFBFEA bl      2055798h
    020657EC E1D000F4 ldrsh   r0,[r0,4h]
    020657F0 E1C500B0 strh    r0,[r5]
    020657F4 EA000057 b       2065958h
    020657F8 E3A0103E mov     r1,3Eh
    020657FC EBFF973A bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
    02065800 E1C500B0 strh    r0,[r5]
    02065804 EA000053 b       2065958h
    02065808 E3A01040 mov     r1,40h
    0206580C EBFF9736 bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
    02065810 E1C500B0 strh    r0,[r5]
    02065814 EA00004F b       2065958h
    02065818 E59F2150 ldr     r2,=20B0B08h
    0206581C E241000D sub     r0,r1,0Dh
    02065820 E1D210FA ldrsh   r1,[r2,0Ah]
    02065824 E1C510B0 strh    r1,[r5]
    02065828 EA00004B b       206595Ch
    0206582C E59F213C ldr     r2,=20B0B08h
    02065830 E241000D sub     r0,r1,0Dh
    02065834 E1D210F4 ldrsh   r1,[r2,4h]
    02065838 E1C510B0 strh    r1,[r5]
    0206583C EA000046 b       206595Ch
    02065840 E59F0128 ldr     r0,=20B0B08h
    02065844 E1D000FC ldrsh   r0,[r0,0Ch]
    02065848 E3500000 cmp     r0,0h
    0206584C 11C500B0 strneh  r0,[r5]
    02065850 0241000D subeq   r0,r1,0Dh
    02065854 01C600B0 streqh  r0,[r6]
    02065858 E3E00000 mvn     r0,0h
    0206585C EA00003E b       206595Ch
    02065860 E59F0108 ldr     r0,=20B0B08h
    02065864 E1D001F0 ldrsh   r0,[r0,10h]
    02065868 E1C500B0 strh    r0,[r5]
    0206586C E3500000 cmp     r0,0h
    02065870 0241000D subeq   r0,r1,0Dh
    02065874 01C600B0 streqh  r0,[r6]
    02065878 E3E00000 mvn     r0,0h
    0206587C EA000036 b       206595Ch
    02065880 E59F00E8 ldr     r0,=20B0B08h
    02065884 E1D000FE ldrsh   r0,[r0,0Eh]
    02065888 E1C500B0 strh    r0,[r5]
    0206588C E3500000 cmp     r0,0h
    02065890 03E00000 mvneq   r0,0h
    02065894 01C600B0 streqh  r0,[r6]
    02065898 E3E00000 mvn     r0,0h
    0206589C EA00002E b       206595Ch
    020658A0 E59F00C8 ldr     r0,=20B0B08h
    020658A4 E1D000F2 ldrsh   r0,[r0,2h]
    020658A8 E1C500B0 strh    r0,[r5]
    020658AC E3500000 cmp     r0,0h
    020658B0 0241000D subeq   r0,r1,0Dh
    020658B4 01C600B0 streqh  r0,[r6]
    020658B8 E3E00000 mvn     r0,0h
    020658BC EA000026 b       206595Ch
    020658C0 E59F00A8 ldr     r0,=20B0B08h
    020658C4 E1D000F0 ldrsh   r0,[r0]
    020658C8 E1C500B0 strh    r0,[r5]
    020658CC E3500000 cmp     r0,0h
    020658D0 0241000D subeq   r0,r1,0Dh
    020658D4 01C600B0 streqh  r0,[r6]
    020658D8 E3E00000 mvn     r0,0h
    020658DC EA00001E b       206595Ch
    020658E0 E59F0088 ldr     r0,=20B0B08h
    020658E4 E1D000F8 ldrsh   r0,[r0,8h]
    020658E8 E1C500B0 strh    r0,[r5]
    020658EC E3500000 cmp     r0,0h
    020658F0 0241000D subeq   r0,r1,0Dh
    020658F4 01C600B0 streqh  r0,[r6]
    020658F8 E3E00000 mvn     r0,0h
    020658FC EA000016 b       206595Ch
    02065900 E59F0068 ldr     r0,=20B0B08h
    02065904 E1D000F6 ldrsh   r0,[r0,6h]
    02065908 E1C500B0 strh    r0,[r5]
    0206590C E3500000 cmp     r0,0h
    02065910 0241000D subeq   r0,r1,0Dh
    02065914 01C600B0 streqh  r0,[r6]
    02065918 E3E00000 mvn     r0,0h
    0206591C EA00000E b       206595Ch
    02065920 E3A00000 mov     r0,0h
    02065924 E3A01044 mov     r1,44h
    02065928 EBFF96EF bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
    0206592C E1C500B0 strh    r0,[r5]
    02065930 E1D500F0 ldrsh   r0,[r5]
    02065934 E3500000 cmp     r0,0h
    02065938 03E00000 mvneq   r0,0h
    0206593C 01C600B0 streqh  r0,[r6]
    02065940 E3E00000 mvn     r0,0h
    02065944 EA000004 b       206595Ch
    02065948 E1D500F0 ldrsh   r0,[r5]
    0206594C E3500000 cmp     r0,0h
    02065950 01D300F2 ldreqsh r0,[r3,2h]
    02065954 01C500B0 streqh  r0,[r5]
    02065958 E1A00004 mov     r0,r4
    0206595C E28DD010 add     r13,r13,10h
    02065960 E8BD8070 pop     r4-r6,r15
    02065964 020A7FF0 
    02065968 020A68BC 
    0206596C 020B0A48 
    02065970 020B0B08 
}
022F7EA4 E1A06000 mov     r6,r0
022F7EA8 E1DD11F2 ldrsh   r1,[r13,12h]      //r1 = actorid
022F7EAC E3E00000 mvn     r0,0h             //r0 = 0xFFFFFFFF
022F7EB0 E1510000 cmp     r1,r0
if( actorid == -1 )
    022F7EB4 0A000147 beq     22F83D8h      //022F83D8 Return
022F7EB8 E3560000 cmp     r6,0h
if( r6 == 0 )
    022F7EBC 0A000004 beq     22F7ED4h      ///022F7ED4 r6 == 0    
022F7EC0 E3560001 cmp     r6,1h
if( r6 == 1 )
    022F7EC4 0A000004 beq     22F7EDCh      ///022F7EDC r6 == 1
022F7EC8 E3560002 cmp     r6,2h
if( r6 == 2 )
    022F7ECC 0A000004 beq     22F7EE4h      ///022F7EE4 r6 == 2
022F7ED0 EA000009 b       22F7EFCh          ///022F7EFC    
///022F7ED4 r6 == 0 
022F7ED4 E3A09000 mov     r9,0h
022F7ED8 EA000007 b       22F7EFCh          ///022F7EFC
///022F7EDC r6 == 1
022F7EDC E3A09001 mov     r9,1h
022F7EE0 EA000005 b       22F7EFCh          ///022F7EFC
///022F7EE4 r6 == 2
022F7EE4 E3A00000 mov     r0,0h
022F7EE8 E3A01035 mov     r1,35h                //GROUND_GETOUT_BACKUP
022F7EEC EBF54D7E bl      204B4ECh              ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
022F7EF0 E3500000 cmp     r0,0h
if( ScrVarVal == 0 )
    022F7EF4 03A06001 moveq   r6,1h             //r6 = 1
022F7EF8 E3A09002 mov     r9,2h                 //r9 = 2
///022F7EFC   
022F7EFC E1DD31F2 ldrsh   r3,[r13,12h]          //r3 = actorid
022F7F00 E1DD41F0 ldrsh   r4,[r13,10h]          //r4 = 

022F7F04 E59F74D4 ldr     r7,=20A7FF0h          //r7 = ActorTablePtr
022F7F08 E3A0C00C mov     r12,0Ch
022F7F0C E59F14D0 ldr     r1,=2321974h          //"GroundLives Locate id %3d  kind %3d  index %3d"
022F7F10 E1A02009 mov     r2,r9
022F7F14 E3A00001 mov     r0,1h
022F7F18 E58D4000 str     r4,[r13]
022F7F1C E1047C83 smlabb  r4,r3,r12,r7          //r4 = (actorid * 12) + ActorTablePtr

022F7F20 EBF450C6 bl      200C240h              //DebugPrint
022F7F24 E3590000 cmp     r9,0h
022F7F28 AA00002E bge     22F7FE8h                  //022F7FE8
022F7F2C E1DD01F2 ldrsh   r0,[r13,12h]              //r0 = actorid
022F7F30 E350003C cmp     r0,3Ch
022F7F34 0A000002 beq     22F7F44h                  //022F7F44
022F7F38 E350003F cmp     r0,3Fh
022F7F3C 0A000009 beq     22F7F68h                  //022F7F68
022F7F40 EA000011 b       22F7F8Ch                  //022F7F8C    
022F7F44 E59F149C ldr     r1,=2324CF8h
022F7F48 E5911004 ldr     r1,[r1,4h]
022F7F4C E1D110F2 ldrsh   r1,[r1,2h]
022F7F50 E3510001 cmp     r1,1h
022F7F54 03E00000 mvneq   r0,0h
022F7F58 0A00011E beq     22F83D8h                  //022F83D8    
022F7F5C EBFFFFAB bl      22F7E10h
022F7F60 E1A09000 mov     r9,r0
022F7F64 EA00000A b       22F7F94h                  //022F7F94
022F7F68 E59F1478 ldr     r1,=2324CF8h
022F7F6C E5911004 ldr     r1,[r1,4h]
022F7F70 E1D110F2 ldrsh   r1,[r1,2h]
022F7F74 E3510004 cmp     r1,4h
022F7F78 03E00000 mvneq   r0,0h
022F7F7C 0A000115 beq     22F83D8h
022F7F80 EBFFFFA2 bl      22F7E10h
022F7F84 E1A09000 mov     r9,r0
022F7F88 EA000001 b       22F7F94h
022F7F8C EBFFFF9F bl      22F7E10h
022F7F90 E1A09000 mov     r9,r0
022F7F94 E3590000 cmp     r9,0h
022F7F98 AA00000F bge     22F7FDCh
022F7F9C E59F0444 ldr     r0,=2324CF8h
022F7FA0 E3A01003 mov     r1,3h
022F7FA4 E5900004 ldr     r0,[r0,4h]
022F7FA8 E3E02000 mvn     r2,0h
022F7FAC E2800E6F add     r0,r0,6F0h
022F7FB0 EA000007 b       22F7FD4h
022F7FB4 E1D030F2 ldrsh   r3,[r0,2h]
022F7FB8 E1530002 cmp     r3,r2
022F7FBC 01A09001 moveq   r9,r1
022F7FC0 0A000005 beq     22F7FDCh
022F7FC4 E2811001 add     r1,r1,1h
022F7FC8 E1A01801 mov     r1,r1,lsl 10h
022F7FCC E1A01841 mov     r1,r1,asr 10h
022F7FD0 E2800E25 add     r0,r0,250h
022F7FD4 E3510018 cmp     r1,18h
022F7FD8 BAFFFFF5 blt     22F7FB4h
022F7FDC E3590000 cmp     r9,0h
022F7FE0 B3E00000 mvnlt   r0,0h
022F7FE4 BA0000FB blt     22F83D8h

022F7FE8 E59F03F8 ldr     r0,=2324CF8h
022F7FEC E1D410F0 ldrsh   r1,[r4]               //r1 = actortype
022F7FF0 E5902004 ldr     r2,[r0,4h]
022F7FF4 E3A00E25 mov     r0,250h
022F7FF8 E88D0C02 stmea   [r13],r1,r10,r11
022F7FFC E1DD31F2 ldrsh   r3,[r13,12h]          //r3 = actorid
022F8000 E1072089 smlabb  r7,r9,r0,r2           //r7 = (r9 * 250h) + r2
022F8004 E59F13E0 ldr     r1,=23219A4h          //"GroundLive Add id %3d  kind %3d  type %3d  hanger %3d  sector %3d"
022F8008 E1A02009 mov     r2,r9
022F800C E3A00001 mov     r0,1h
022F8010 EBF4508A bl      200C240h              //DebugPrint
022F8014 E1D710F2 ldrsh   r1,[r7,2h]
022F8018 E3E00000 mvn     r0,0h
022F801C E1510000 cmp     r1,r0
022F8020 1A000005 bne     22F803Ch
022F8024 E3A01000 mov     r1,0h
022F8028 E2870F63 add     r0,r7,18Ch
022F802C E5C71004 strb    r1,[r7,4h]
022F8030 E3A05001 mov     r5,1h
022F8034 EBFFF276 bl      22F4A14h
022F8038 EA000002 b       22F8048h
022F803C E1DD01F2 ldrsh   r0,[r13,12h]
022F8040 E1510000 cmp     r1,r0
022F8044 03A05000 moveq   r5,0h
022F8048 E24A0001 sub     r0,r10,1h
022F804C E1C790B0 strh    r9,[r7]
022F8050 E1A00800 mov     r0,r0,lsl 10h
022F8054 E1A00840 mov     r0,r0,asr 10h
022F8058 E1A00800 mov     r0,r0,lsl 10h
022F805C E1DD11F2 ldrsh   r1,[r13,12h]
022F8060 E1A00820 mov     r0,r0,lsr 10h
022F8064 E58D000C str     r0,[r13,0Ch]
022F8068 E1C710B2 strh    r1,[r7,2h]
022F806C E59D000C ldr     r0,[r13,0Ch]
022F8070 E3500001 cmp     r0,1h
022F8074 91C7A0B6 strlsh  r10,[r7,6h]
022F8078 95C7B008 strlsb  r11,[r7,8h]
022F807C 93E00000 mvnls   r0,0h
022F8080 95C70009 strlsb  r0,[r7,9h]
022F8084 9A000004 bls     22F809Ch
022F8088 E3550000 cmp     r5,0h
022F808C 13E00000 mvnne   r0,0h
022F8090 11C700B6 strneh  r0,[r7,6h]
022F8094 15C70008 strneb  r0,[r7,8h]
022F8098 E5C7B009 strb    r11,[r7,9h]
022F809C E1DD11F0 ldrsh   r1,[r13,10h]
022F80A0 E2870F4B add     r0,r7,12Ch
022F80A4 E1C710BA strh    r1,[r7,0Ah]
022F80A8 EBFFCA29 bl      22EA954h
022F80AC E3A00009 mov     r0,9h
022F80B0 E587012C str     r0,[r7,12Ch]
022F80B4 E3A02000 mov     r2,0h
022F80B8 E587214C str     r2,[r7,14Ch]
022F80BC E3A01001 mov     r1,1h
022F80C0 E5C71156 strb    r1,[r7,156h]
022F80C4 E1D400F0 ldrsh   r0,[r4]
022F80C8 E350000B cmp     r0,0Bh
022F80CC 908FF100 addls   r15,r15,r0,lsl 2h
022F80D0 EA000023 b       22F8164h
022F80D4 EA000022 b       22F8164h
022F80D8 EA000009 b       22F8104h
022F80DC EA00000C b       22F8114h
022F80E0 EA00000E b       22F8120h
022F80E4 EA00001E b       22F8164h
022F80E8 EA00001D b       22F8164h
022F80EC EA00001C b       22F8164h
022F80F0 EA00000D b       22F812Ch
022F80F4 EA00000F b       22F8138h
022F80F8 EA000011 b       22F8144h
022F80FC EA000012 b       22F814Ch
022F8100 EA000014 b       22F8158h
022F8104 E59F02E4 ldr     r0,=3000743h
022F8108 E5870128 str     r0,[r7,128h]
022F810C E587112C str     r1,[r7,12Ch]
022F8110 EA000015 b       22F816Ch
022F8114 E59F02D8 ldr     r0,=3000002h
022F8118 E5870128 str     r0,[r7,128h]
022F811C EA000012 b       22F816Ch
022F8120 E59F02D0 ldr     r0,=3002358h
022F8124 E5870128 str     r0,[r7,128h]
022F8128 EA00000F b       22F816Ch
022F812C E3A00D0D mov     r0,340h
022F8130 E5870128 str     r0,[r7,128h]
022F8134 EA00000C b       22F816Ch
022F8138 E3A00802 mov     r0,20000h
022F813C E5870128 str     r0,[r7,128h]
022F8140 EA000009 b       22F816Ch
022F8144 E5872128 str     r2,[r7,128h]
022F8148 EA000007 b       22F816Ch
022F814C E3A00703 mov     r0,0C0000h
022F8150 E5870128 str     r0,[r7,128h]
022F8154 EA000004 b       22F816Ch
022F8158 E3A00801 mov     r0,10000h
022F815C E5870128 str     r0,[r7,128h]
022F8160 EA000001 b       22F816Ch
022F8164 E59F028C ldr     r0,=3002358h
022F8168 E5870128 str     r0,[r7,128h]
022F816C E5D4000A ldrb    r0,[r4,0Ah]
022F8170 E1A00580 mov     r0,r0,lsl 0Bh
022F8174 E587000C str     r0,[r7,0Ch]
022F8178 E5D4000B ldrb    r0,[r4,0Bh]
022F817C E1A00580 mov     r0,r0,lsl 0Bh
022F8180 E5870010 str     r0,[r7,10h]
022F8184 E597000C ldr     r0,[r7,0Ch]
022F8188 E0800FA0 add     r0,r0,r0,lsr 1Fh
022F818C E1A000C0 mov     r0,r0,asr 1h
022F8190 E5870014 str     r0,[r7,14h]           //[r7,14h] = ([r7,0Ch] + ([r7,0Ch] >> 0x1F)) >> 1
022F8194 E5970010 ldr     r0,[r7,10h]
022F8198 E0800FA0 add     r0,r0,r0,lsr 1Fh
022F819C E1A000C0 mov     r0,r0,asr 1h
022F81A0 E5870018 str     r0,[r7,18h]
//  struct ActorEntryMemory
//      0x00 actorid
//      0x02 direction
//      0x03 xoffset
//      0x04 yoffset
//      0x05 actunk0x08
//      0x06 actunk0x0A
//      0x07 --
//      0x08 scriptid
022F81A4 E1D800D2 ldrsb   r0,[r8,2h]                //r0 = direction
022F81A8 E5C7001C strb    r0,[r7,1Ch]
022F81AC E5D80005 ldrb    r0,[r8,5h]                //r0 = actunk0x08
022F81B0 E3100004 tst     r0,4h
if( actunk0x08 & 4 != 0 )
    022F81B4 1A000007 bne     22F81D8h              //022F81D8 actunk0x08 & 4 != 0
022F81B8 E5D80003 ldrb    r0,[r8,3h]                //r0 = xoffset
022F81BC E1A00580 mov     r0,r0,lsl 0Bh             //r0 = xoffset << 11
022F81C0 E5870020 str     r0,[r7,20h]               //[r7,20h] = (xoffset << 11)
022F81C4 E5D80005 ldrb    r0,[r8,5h]                //r0 = actunk0x08
022F81C8 E3100002 tst     r0,2h
if( actunk0x08 & 2 != 0 )
    022F81CC 15970020 ldrne   r0,[r7,20h]           //r0 = (xoffset << 11)
    022F81D0 12800B01 addne   r0,r0,400h            //r0 = (xoffset << 11) + 0x400
    022F81D4 15870020 strne   r0,[r7,20h]           //[r7,20h] = (xoffset << 11) + 0x400
///022F81D8 actunk0x08 & 4 != 0
022F81D8 E5D80006 ldrb    r0,[r8,6h]                //r0 = actunk0x0A
022F81DC E3100004 tst     r0,4h
if( actunk0x0A & 4 != 0 )
    022F81E0 1A000007 bne     22F8204h              ///022F8204 actunk0x0A & 4 != 0
022F81E4 E5D80004 ldrb    r0,[r8,4h]                //r0 = yoffset
022F81E8 E1A00580 mov     r0,r0,lsl 0Bh             //r0 = (yoffset << 11)
022F81EC E5870024 str     r0,[r7,24h]               //[r7,24h] = (yoffset << 11)
022F81F0 E5D80006 ldrb    r0,[r8,6h]                //r0 = actunk0x0A
022F81F4 E3100002 tst     r0,2h
if( actunk0x0A & 2 != 0 )
    022F81F8 15970024 ldrne   r0,[r7,24h]           //r0 = (yoffset << 11)
    022F81FC 12800B01 addne   r0,r0,400h            //r0 = (yoffset << 11) + 0x400
    022F8200 15870024 strne   r0,[r7,24h]           //[r7,24h] = (yoffset << 11) + 0x400
///022F8204 actunk0x0A & 4 != 0
022F8204 E5DD0038 ldrb    r0,[r13,38h]
022F8208 E3500000 cmp     r0,0h
022F820C 15970128 ldrne   r0,[r7,128h]
022F8210 13800502 orrne   r0,r0,800000h
022F8214 15870128 strne   r0,[r7,128h]
022F8218 E3550000 cmp     r5,0h
022F821C 0A00002C beq     22F82D4h
022F8220 E1D711DC ldrsb   r1,[r7,1Ch]
022F8224 E3A00000 mov     r0,0h                     //r0 = 0
022F8228 E3A0200C mov     r2,0Ch                    //r2 = 12
022F822C E5C7115A strb    r1,[r7,15Ah]
022F8230 E5973020 ldr     r3,[r7,20h]               //r3 = (xoffset << 11) + 0x400 or (xoffset << 11)
022F8234 E5971014 ldr     r1,[r7,14h]
022F8238 E0431001 sub     r1,r3,r1                  //r1 = ((xoffset << 11) + 0x400 or (xoffset << 11)) - r1
022F823C E587115C str     r1,[r7,15Ch]
022F8240 E597A020 ldr     r10,[r7,20h]              //r10 = (xoffset << 11) + 0x400 or (xoffset << 11)
022F8244 E5973014 ldr     r3,[r7,14h]
022F8248 E2871C01 add     r1,r7,100h                //r1 = r7 + 100h
022F824C E08A3003 add     r3,r10,r3                 //r3 = ((xoffset << 11) + 0x400 or (xoffset << 11)) + r3
022F8250 E5873164 str     r3,[r7,164h]
022F8254 E597B024 ldr     r11,[r7,24h]               //r11 = (yoffset << 11) + 0x400 or (yoffset << 11) 
022F8258 E597A018 ldr     r10,[r7,18h]
022F825C E59F3184 ldr     r3,=2324CF8h
022F8260 E04BA00A sub     r10,r11,r10
022F8264 E587A160 str     r10,[r7,160h]
022F8268 E597C024 ldr     r12,[r7,24h]              //r12 = (yoffset << 11) + 0x400 or (yoffset << 11) 
022F826C E597B018 ldr     r11,[r7,18h]
022F8270 E16A0286 smulbb  r10,r6,r2
022F8274 E08CB00B add     r11,r12,r11
022F8278 E587B168 str     r11,[r7,168h]
022F827C E5870170 str     r0,[r7,170h]
022F8280 E587016C str     r0,[r7,16Ch]
022F8284 E1C165B0 strh    r6,[r1,50h]
022F8288 E5933000 ldr     r3,[r3]
022F828C E242100D sub     r1,r2,0Dh
022F8290 E083200A add     r2,r3,r10
022F8294 E5821008 str     r1,[r2,8h]
022F8298 E783000A str     r0,[r3,r10]
022F829C E5820004 str     r0,[r2,4h]
022F82A0 E1D700F2 ldrsh   r0,[r7,2h]
022F82A4 E350001E cmp     r0,1Eh
022F82A8 BA000006 blt     22F82C8h
022F82AC E350002D cmp     r0,2Dh
022F82B0 CA000004 bgt     22F82C8h
022F82B4 E3A00012 mov     r0,12h
022F82B8 EBF427ED bl      2002274h
022F82BC E2871C01 add     r1,r7,100h
022F82C0 E1C105B8 strh    r0,[r1,58h]
022F82C4 EA000002 b       22F82D4h
022F82C8 E2870C01 add     r0,r7,100h
022F82CC E3A01000 mov     r1,0h
022F82D0 E1C015B8 strh    r1,[r0,58h]
022F82D4 E3A00001 mov     r0,1h
022F82D8 E5C70152 strb    r0,[r7,152h]
022F82DC E2401002 sub     r1,r0,2h
022F82E0 E2870C01 add     r0,r7,100h
022F82E4 E1C015B4 strh    r1,[r0,54h]
022F82E8 E3550000 cmp     r5,0h
022F82EC 0A000018 beq     22F8354h
022F82F0 E2870028 add     r0,r7,28h
022F82F4 E2871030 add     r1,r7,30h
022F82F8 EBFFE702 bl      22F1F08h
022F82FC E3E06000 mvn     r6,0h
022F8300 E5C76175 strb    r6,[r7,175h]
022F8304 E2870C01 add     r0,r7,100h
022F8308 E3A01C03 mov     r1,300h
022F830C E1C017B6 strh    r1,[r0,76h]
022F8310 E3A03001 mov     r3,1h
022F8314 E1C037B8 strh    r3,[r0,78h]
022F8318 E3A02000 mov     r2,0h
022F831C E59F10D8 ldr     r1,=807h
022F8320 E587217C str     r2,[r7,17Ch]
022F8324 E1C018B0 strh    r1,[r0,80h]
022F8328 E5C73174 strb    r3,[r7,174h]
022F832C E1C068B2 strh    r6,[r0,82h]
022F8330 E1C028B4 strh    r2,[r0,84h]
022F8334 E1D710FA ldrsh   r1,[r7,0Ah]
022F8338 E5972128 ldr     r2,[r7,128h]
022F833C E2870F63 add     r0,r7,18Ch
022F8340 EBFFF61D bl      22F5BBCh
022F8344 E59F10B4 ldr     r1,=232187Ch
022F8348 E1A02007 mov     r2,r7
022F834C E2870038 add     r0,r7,38h
022F8350 EBFF9164 bl      22DC8E8h
022F8354 E5970160 ldr     r0,[r7,160h]
022F8358 E59F10A4 ldr     r1,=23219E8h          //"GroundLives Locate kind %3d  px %8d  py %8d"
022F835C E58D0000 str     r0,[r13]
022F8360 E1D720F2 ldrsh   r2,[r7,2h]            //r2 = actorid
022F8364 E597315C ldr     r3,[r7,15Ch]          //
022F8368 E3A00001 mov     r0,1h
022F836C EBF44FB3 bl      200C240h              //DebugPrint
022F8370 E59D000C ldr     r0,[r13,0Ch]
022F8374 E3500001 cmp     r0,1h
if( r0 < 1 )
    022F8378 91D810F8 ldrlssh r1,[r8,8h]                //r1 = scriptid
    022F837C 92870C01 addls   r0,r7,100h                //r0 = r7 + 100h
    022F8380 91C012B4 strlsh  r1,[r0,24h]               //[r0,24h] = scriptid
    022F8384 9A000003 bls     22F8398h                  ///022F8398
022F8388 E3550000 cmp     r5,0h
022F838C 12870C01 addne   r0,r7,100h
022F8390 13E01000 mvnne   r1,0h
022F8394 11C012B4 strneh  r1,[r0,24h]
///022F8398
022F8398 E3550000 cmp     r5,0h
022F839C 0A00000A beq     22F83CCh
022F83A0 E1D400F0 ldrsh   r0,[r4]
022F83A4 E3500001 cmp     r0,1h
022F83A8 1A000007 bne     22F83CCh
022F83AC E3A00000 mov     r0,0h
022F83B0 EBFFCBBB bl      22EB2A4h
022F83B4 E3500000 cmp     r0,0h
022F83B8 0A000003 beq     22F83CCh
022F83BC E1A02009 mov     r2,r9
022F83C0 E3A00000 mov     r0,0h
022F83C4 E3A01002 mov     r1,2h
022F83C8 EBFFCB56 bl      22EB128h
022F83CC E3A01001 mov     r1,1h
022F83D0 E1A00009 mov     r0,r9
022F83D4 E5C71004 strb    r1,[r7,4h]
///022F83D8
022F83D8 E28DD014 add     r13,r13,14h
022F83DC E8BD8FF0 pop     r4-r11,r15
022F83E0 020A7FF0
022F83E4 02321974
022F83E8 02324CF8
022F83EC 023219A4
022F83F0 03000743 
022F83F4 03000002 
022F83F8 03002358
022F83FC 00000807
022F8400 0232187C 
022F8404 023219E8 

/*
    fun_022FBEC4 22FBEC4h GroundObjectAdd
*/
// 0xFFFFFFFF, PtrEntryFromScriptData, ScriptDataTy, Param2, PtrFileBuff
//R0, R1 = PtrEntryFromScriptData, R2, R3
022FBEC4 E92D4FF8 push    r3-r11,r14
022FBEC8 E24DD010 sub     r13,r13,10h
022FBECC E1A08001 mov     r8,r1                 //R8 = PtrEntryFromScriptData
022FBED0 E1D840F0 ldrsh   r4,[r8]               //R4 = IndexSEv
022FBED4 E59F53E4 ldr     r5,=231EE54h          //EventSubFileTable
022FBED8 E3A0100C mov     r1,0Ch                //Length of a single entry in the EventSubFileTable
022FBEDC E1A09000 mov     r9,r0                 //R9  = R0
022FBEE0 E1055184 smlabb  r5,r4,r1,r5           //R5  = ( IndexSEv * 12 ) + 0x0231EE54
022FBEE4 E1A07002 mov     r7,r2                 //R7  = ScriptDataTy
022FBEE8 E1A0A003 mov     r10,r3                //R10 = Param2
022FBEEC E3590000 cmp     r9,0h
if( R9 > 0 )
    022FBEF0 AA000018 bge     22FBF58h          ///022FBF58 LBL1 v
022FBEF4 E3540005 cmp     r4,5h
if( IndexSEv != 5 )
    022FBEF8 1A000002 bne     22FBF08h          ///022FBF08 LBL2 v
022FBEFC E1A00004 mov     r0,r4                 //R0 = IndexSEv
022FBF00 EBFFFFDB bl      22FBE74h              ///022FBE74 SearchForSpecifiedIndexInStructs
022FBF04 E1A09000 mov     r9,r0                 //R9 = Returnvalue22FBE74h
///022FBF08 LBL2
022FBF08 E3590000 cmp     r9,0h
if( R9 > 0 )
    022FBF0C AA00000E bge     22FBF4Ch          ///022FBF4C LBL3 v
022FBF10 E59F03AC ldr     r0,=2324D00h
022FBF14 E3A03000 mov     r3,0h
022FBF18 E5902000 ldr     r2,[r0]
022FBF1C E3E01000 mvn     r1,0h
022FBF20 EA000007 b       22FBF44h
022FBF24 E1D200F6 ldrsh   r0,[r2,6h]
022FBF28 E1500001 cmp     r0,r1
022FBF2C 01A09003 moveq   r9,r3
022FBF30 0A000005 beq     22FBF4Ch
022FBF34 E2830001 add     r0,r3,1h
022FBF38 E1A00800 mov     r0,r0,lsl 10h
022FBF3C E1A03840 mov     r3,r0,asr 10h
022FBF40 E2822F86 add     r2,r2,218h
022FBF44 E3530010 cmp     r3,10h
022FBF48 BAFFFFF5 blt     22FBF24h
///022FBF4C LBL3
022FBF4C E3590000 cmp     r9,0h
if( R9 < 0 )
    022FBF50 B3E00000 mvnlt   r0,0h             //R0 = 0xFFFFFFFF
    022FBF54 BA0000D7 blt     22FC2B8h          ///022FC2B8 LBL4 v
///022FBF58 LBL1
022FBF58 E59F0364 ldr     r0,=2324D00h
022FBF5C E1D810B0 ldrh    r1,[r8]               //R1 = IndexSEv
022FBF60 E5902000 ldr     r2,[r0]
022FBF64 E3A00F86 mov     r0,218h
022FBF68 E58D1000 str     r1,[r13]
022FBF6C E1D510F0 ldrsh   r1,[r5]               //Unk1, first short of entry in EventSubFileTable
022FBF70 E1062089 smlabb  r6,r9,r0,r2
022FBF74 E98D0082 stmfa   [r13],r1,r7
022FBF78 E59F1348 ldr     r1,=2321E4Ch          //"GroundObject Add id %3d  kind %3d[%3d]  type %3d  hanger %3d  sector %3d"     
022FBF7C E1A02009 mov     r2,r9
022FBF80 E1A03004 mov     r3,r4
022FBF84 E58DA00C str     r10,[r13,0Ch]
022FBF88 E3A00001 mov     r0,1h
022FBF8C EBF440AB bl      200C240h              //DebugPrint
022FBF90 E1D610F6 ldrsh   r1,[r6,6h]
022FBF94 E3E00000 mvn     r0,0h
022FBF98 E1510000 cmp     r1,r0
022FBF9C 03A00001 moveq   r0,1h
022FBFA0 13A00000 movne   r0,0h
022FBFA4 E210B0FF ands    r11,r0,0FFh
022FBFA8 0A000003 beq     22FBFBCh
022FBFAC E3A01000 mov     r1,0h
022FBFB0 E2860F55 add     r0,r6,154h
022FBFB4 E5C61008 strb    r1,[r6,8h]
022FBFB8 EBFFE295 bl      22F4A14h
022FBFBC E2470001 sub     r0,r7,1h
022FBFC0 E1A00800 mov     r0,r0,lsl 10h
022FBFC4 E1A00840 mov     r0,r0,asr 10h
022FBFC8 E1A00800 mov     r0,r0,lsl 10h
022FBFCC E1C690B4 strh    r9,[r6,4h]
022FBFD0 E1A00820 mov     r0,r0,lsr 10h
022FBFD4 E3500001 cmp     r0,1h
022FBFD8 E1C640B6 strh    r4,[r6,6h]
022FBFDC 91C670BA strlsh  r7,[r6,0Ah]
022FBFE0 95C6A00C strlsb  r10,[r6,0Ch]
022FBFE4 93E00000 mvnls   r0,0h
022FBFE8 95C6000D strlsb  r0,[r6,0Dh]
022FBFEC 9A000004 bls     22FC004h
022FBFF0 E35B0000 cmp     r11,0h
022FBFF4 13E00000 mvnne   r0,0h
022FBFF8 11C600BA strneh  r0,[r6,0Ah]
022FBFFC 15C6000C strneb  r0,[r6,0Ch]
022FC000 E5C6A00D strb    r10,[r6,0Dh]
022FC004 E1D500F0 ldrsh   r0,[r5]                   //Unk1 the value for an object entry that goes from 0 to 16
022FC008 E3500010 cmp     r0,10h
if( R0 <= 16 )
    022FC00C 908FF100 addls   r15,r15,r0,lsl 2h
022FC010 EA00003A b       22FC100h              //R0 > 16
022FC014 EA000039 b       22FC100h              //0x00  022FC100
022FC018 EA00000E b       22FC058h              //0x01  022FC058
022FC01C EA000034 b       22FC0F4h              //0x02  022FC0F4
022FC020 EA000024 b       22FC0B8h              //0x03  022FC0B8
022FC024 EA000026 b       22FC0C4h              //0x04  022FC0C4
022FC028 EA000025 b       22FC0C4h              //0x05  022FC0C4
022FC02C EA000027 b       22FC0D0h              //0x06  022FC0D0
022FC030 EA000029 b       22FC0DCh              //0x07  022FC0DC
022FC034 EA00002B b       22FC0E8h              //0x08  022FC0E8
022FC038 EA00001B b       22FC0ACh              //0x09  022FC0AC
022FC03C EA000008 b       22FC064h              //0x0A  022FC064
022FC040 EA000004 b       22FC058h              //0x0B  022FC058
022FC044 EA000009 b       22FC070h              //0x0C  022FC070
022FC048 EA000014 b       22FC0A0h              //0x0D  022FC0A0
022FC04C EA000010 b       22FC094h              //0x0E  022FC094
022FC050 EA00000C b       22FC088h              //0x0F  022FC088
022FC054 EA000008 b       22FC07Ch              //0x10  022FC07C
//0x01 and 0x0B
022FC058 E3A00000 mov     r0,0h
022FC05C E586012C str     r0,[r6,12Ch]
022FC060 EA000028 b       22FC108h              //022FC108
//0x0A  022FC064
022FC064 E3A00802 mov     r0,20000h
022FC068 E586012C str     r0,[r6,12Ch]
022FC06C EA000025 b       22FC108h              //022FC108
//0x0C  022FC070
022FC070 E3A00701 mov     r0,40000h
022FC074 E586012C str     r0,[r6,12Ch]
022FC078 EA000022 b       22FC108h              //022FC108
//0x10  022FC07C
022FC07C E3A00202 mov     r0,20000000h
022FC080 E586012C str     r0,[r6,12Ch]
022FC084 EA00001F b       22FC108h              //022FC108
//0x0F  022FC088
022FC088 E3A00783 mov     r0,20C0000h
022FC08C E586012C str     r0,[r6,12Ch]
022FC090 EA00001C b       22FC108h              //022FC108
//0x0E  022FC094
022FC094 E3A00783 mov     r0,20C0000h
022FC098 E586012C str     r0,[r6,12Ch]
022FC09C EA000019 b       22FC108h              //022FC108
//0x0D  022FC0A0
022FC0A0 E3A00801 mov     r0,10000h
022FC0A4 E586012C str     r0,[r6,12Ch]
022FC0A8 EA000016 b       22FC108h              //022FC108
//0x09  022FC0AC
022FC0AC E3A00D05 mov     r0,140h
022FC0B0 E586012C str     r0,[r6,12Ch]
022FC0B4 EA000013 b       22FC108h              //022FC108
//0x03  022FC0B8
022FC0B8 E59F020C ldr     r0,=2006100h
022FC0BC E586012C str     r0,[r6,12Ch]
022FC0C0 EA000010 b       22FC108h              //022FC108
//0x04 and 0x05  022FC0C4
022FC0C4 E3A00C61 mov     r0,6100h
022FC0C8 E586012C str     r0,[r6,12Ch]
022FC0CC EA00000D b       22FC108h              //022FC108
//0x06  022FC0D0
022FC0D0 E3A00C11 mov     r0,1100h
022FC0D4 E586012C str     r0,[r6,12Ch]
022FC0D8 EA00000A b       22FC108h              //022FC108
//0x07  022FC0DC
022FC0DC E3A00C21 mov     r0,2100h
022FC0E0 E586012C str     r0,[r6,12Ch]
022FC0E4 EA000007 b       22FC108h              //022FC108
//0x08  022FC0E8
022FC0E8 E3A00A06 mov     r0,6000h
022FC0EC E586012C str     r0,[r6,12Ch]
022FC0F0 EA000004 b       22FC108h              //022FC108
//0x02
022FC0F4 E59F01D4 ldr     r0,=2003100h
022FC0F8 E586012C str     r0,[r6,12Ch]
022FC0FC EA000001 b       22FC108h              //022FC108
//0x00 and R0 > 16  022FC100
022FC100 E59F01CC ldr     r0,=2006140h
022FC104 E586012C str     r0,[r6,12Ch]
//022FC108
022FC108 E5D50008 ldrb    r0,[r5,8h]            //Get unk3 from the object's entry
022FC10C E3500000 cmp     r0,0h
if( unk3 != 0 )
    022FC110 1596012C ldrne   r0,[r6,12Ch]
    022FC114 13800102 orrne   r0,r0,80000000h
    022FC118 1586012C strne   r0,[r6,12Ch]
022FC11C E5D50002 ldrb    r0,[r5,2h]            //Get unk2's high nybble from the object's entry
022FC120 E3500000 cmp     r0,0h
if( unk2 == 0 )
    022FC124 05D80003 ldreqb  r0,[r8,3h]
    022FC128 01A00580 moveq   r0,r0,lsl 0Bh
    022FC12C 05860010 streq   r0,[r6,10h]
    022FC130 05D80004 ldreqb  r0,[r8,4h]
else
    022FC134 11A00580 movne   r0,r0,lsl 0Bh
    022FC138 15860010 strne   r0,[r6,10h]
    022FC13C 15D50003 ldrneb  r0,[r5,3h]            //Get unk2's low nybble
022FC140 E1A00580 mov     r0,r0,lsl 0Bh
022FC144 E5860014 str     r0,[r6,14h]
022FC148 E5960010 ldr     r0,[r6,10h]
022FC14C E0800FA0 add     r0,r0,r0,lsr 1Fh
022FC150 E1A000C0 mov     r0,r0,asr 1h
022FC154 E5860018 str     r0,[r6,18h]
022FC158 E5960014 ldr     r0,[r6,14h]
022FC15C E0800FA0 add     r0,r0,r0,lsr 1Fh
022FC160 E1A000C0 mov     r0,r0,asr 1h
022FC164 E586001C str     r0,[r6,1Ch]
022FC168 E1D800D2 ldrsb   r0,[r8,2h]
022FC16C E5C60020 strb    r0,[r6,20h]
022FC170 E5D80007 ldrb    r0,[r8,7h]
022FC174 E3100004 tst     r0,4h
022FC178 1A000007 bne     22FC19Ch
022FC17C E5D80005 ldrb    r0,[r8,5h]
022FC180 E1A00580 mov     r0,r0,lsl 0Bh
022FC184 E5860024 str     r0,[r6,24h]
022FC188 E5D80007 ldrb    r0,[r8,7h]
022FC18C E3100002 tst     r0,2h
022FC190 15960024 ldrne   r0,[r6,24h]
022FC194 12800B01 addne   r0,r0,400h
022FC198 15860024 strne   r0,[r6,24h]
022FC19C E5D80008 ldrb    r0,[r8,8h]
022FC1A0 E3100004 tst     r0,4h
022FC1A4 1A000007 bne     22FC1C8h
022FC1A8 E5D80006 ldrb    r0,[r8,6h]
022FC1AC E1A00580 mov     r0,r0,lsl 0Bh
022FC1B0 E5860028 str     r0,[r6,28h]
022FC1B4 E5D80008 ldrb    r0,[r8,8h]
022FC1B8 E3100002 tst     r0,2h
022FC1BC 15960028 ldrne   r0,[r6,28h]
022FC1C0 12800B01 addne   r0,r0,400h
022FC1C4 15860028 strne   r0,[r6,28h]
022FC1C8 E5DD0038 ldrb    r0,[r13,38h]
022FC1CC E3500000 cmp     r0,0h
022FC1D0 1596012C ldrne   r0,[r6,12Ch]
022FC1D4 13800502 orrne   r0,r0,800000h
022FC1D8 1586012C strne   r0,[r6,12Ch]
022FC1DC E35B0000 cmp     r11,0h
022FC1E0 0A00002E beq     22FC2A0h
022FC1E4 E1D602D0 ldrsb   r0,[r6,20h]
022FC1E8 E3A02000 mov     r2,0h
022FC1EC E5C60130 strb    r0,[r6,130h]
022FC1F0 E5963024 ldr     r3,[r6,24h]
022FC1F4 E5961018 ldr     r1,[r6,18h]
022FC1F8 E286002C add     r0,r6,2Ch
022FC1FC E0431001 sub     r1,r3,r1
022FC200 E5861134 str     r1,[r6,134h]
022FC204 E5964024 ldr     r4,[r6,24h]
022FC208 E5963018 ldr     r3,[r6,18h]
022FC20C E2861034 add     r1,r6,34h
022FC210 E0843003 add     r3,r4,r3
022FC214 E586313C str     r3,[r6,13Ch]
022FC218 E5964028 ldr     r4,[r6,28h]
022FC21C E596301C ldr     r3,[r6,1Ch]
022FC220 E0443003 sub     r3,r4,r3
022FC224 E5863138 str     r3,[r6,138h]
022FC228 E5964028 ldr     r4,[r6,28h]
022FC22C E596301C ldr     r3,[r6,1Ch]
022FC230 E0843003 add     r3,r4,r3
022FC234 E5863140 str     r3,[r6,140h]
022FC238 E5862148 str     r2,[r6,148h]
022FC23C E5862144 str     r2,[r6,144h]
022FC240 EBFFD730 bl      22F1F08h
022FC244 E2860C01 add     r0,r6,100h
022FC248 E3A02B02 mov     r2,800h
022FC24C E3A01001 mov     r1,1h
022FC250 E1C024BE strh    r2,[r0,4Eh]
022FC254 E5C6114C strb    r1,[r6,14Ch]
022FC258 E2411002 sub     r1,r1,2h
022FC25C E1C015B0 strh    r1,[r0,50h]
022FC260 E3A01000 mov     r1,0h
022FC264 E1C015B2 strh    r1,[r0,52h]
022FC268 E1D610F6 ldrsh   r1,[r6,6h]
022FC26C E596212C ldr     r2,[r6,12Ch]
022FC270 E2860F55 add     r0,r6,154h
022FC274 EBFFEAF9 bl      22F6E60h
022FC278 E59F1058 ldr     r1,=2321DFCh
022FC27C E1A02006 mov     r2,r6
022FC280 E286003C add     r0,r6,3Ch
022FC284 EBFF8197 bl      22DC8E8h
022FC288 E1D500F0 ldrsh   r0,[r5]
022FC28C E3500010 cmp     r0,10h
022FC290 1A000002 bne     22FC2A0h
022FC294 E2860F55 add     r0,r6,154h
022FC298 E3A01000 mov     r1,0h
022FC29C EBFFE1B4 bl      22F4974h
022FC2A0 E1D820FA ldrsh   r2,[r8,0Ah]
022FC2A4 E2860C01 add     r0,r6,100h
022FC2A8 E3A01001 mov     r1,1h
022FC2AC E1C022B8 strh    r2,[r0,28h]
022FC2B0 E1A00009 mov     r0,r9
022FC2B4 E5C61008 strb    r1,[r6,8h]
///022FC2B8 LBL4
022FC2B8 E28DD010 add     r13,r13,10h
022FC2BC E8BD8FF8 pop     r3-r11,r15
022FC2C0 0231EE54 
022FC2C4 02324D00 
022FC2C8 02321E4C 
022FC2CC 02006100 
022FC2D0 02003100 
022FC2D4 02006140 
022FC2D8 02321DFC 

/*
    fun_022FBE74 22FBE74h
        Seems to search through the struct pointed to by the pointer at 2324D00h. 
        It looks for the value passed in R0, at struct+6, and return the index of the struct that matched, 
        or 16 if no match. 
*/
//R0 = IndexSEv
022FBE74 E3E01000 mvn     r1,0h             //R1 = 0xFFFFFFFF
022FBE78 E1500001 cmp     r0,r1             
if( IndexSEv == 0xFFFFFFFF )
    022FBE7C 0A00000D beq     22FBEB8h      ///022FBEB8 LBL1
022FBE80 E59F1038 ldr     r1,=2324D00h      //
022FBE84 E3A03000 mov     r3,0h             //R3 = Counter = 0
022FBE88 E5912000 ldr     r2,[r1]           //R2 = PtrStruct02324D00
022FBE8C EA000007 b       22FBEB0h          ///022FBEB0 LBL2
///022FBE90 LOOP_BEG
022FBE90 E1D210F6 ldrsh   r1,[r2,6h]        
022FBE94 E1510000 cmp     r1,r0
if( R1 == IndexSEv )
    022FBE98 01A00003 moveq   r0,r3         //Return Counter
    022FBE9C 012FFF1E bxeq    r14
022FBEA0 E2831001 add     r1,r3,1h          //Counter += 1
022FBEA4 E1A01801 mov     r1,r1,lsl 10h     //
022FBEA8 E1A03841 mov     r3,r1,asr 10h     //The last two spread the sign of the signed int16 to a int32
022FBEAC E2822F86 add     r2,r2,218h        //Move to next struct at 0x218 bytes away
///022FBEB0 LBL2
022FBEB0 E3530010 cmp     r3,10h
if( Counter < 16 )
    022FBEB4 BAFFFFF5 blt     22FBE90h      ///022FBE90 LOOP_BEG
///022FBEB8 LBL1
022FBEB8 E3E00000 mvn     r0,0h             //R0 = 0xFFFFFFFF
022FBEBC E12FFF1E bx      r14
022FBEC0 02324D00 


/*
    fun_022FD72C 22FD72Ch GroundPerformerAdd
    R0=0xFFFFFFFF, R1=PerformerEntryMemory, R2=ScriptDataTy, R3=Param2
    
//  struct PerformerEntryMemory
//      0x00 perftype
//      0x02 direction
//      0x03 perfunk0x04
//      0x04 perfunk0x06
//      0x05 perfunk0x08
//      0x06 perfunk0x0A
//      0x07 perfunk0x0C
//      0x08 perfunk0x0E   
*/
022FD72C E92D47F8 push    r3-r10,r14
022FD730 E24DD00C sub     r13,r13,0Ch
022FD734 E1A08001 mov     r8,r1                 //R8 = PerformerEntryMemory
022FD738 E1D850B0 ldrh    r5,[r8]               //R5 = perftype
022FD73C E59F1324 ldr     r1,=2320858h          //r1 = 2320858h PerfTypeTable?
022FD740 E1B09000 movs    r9,r0                 //R9 = Param0
022FD744 E1A07002 mov     r7,r2                 //R7 = ScriptDataTy 
022FD748 E1A06003 mov     r6,r3                 //R6 = Param2
022FD74C E0814105 add     r4,r1,r5,lsl 2h       //R4 = [PerfTypeTable? + (perftype * 4)] (PerfTypeEntry?)
if( Param0 >= 0 )
    022FD750 5A00001A bpl     22FD7C0h          ///022FD7C0 (PL is positive or 0)
022FD754 E1D400D0 ldrsb   r0,[r4]               //r0 = PerfTypeEntry?_Unk0
022FD758 E3500001 cmp     r0,1h
if( PerfTypeEntry?_Unk0 != 1 )
    022FD75C 1A000003 bne     22FD770h          ///022FD770
022FD760 E1A00805 mov     r0,r5,lsl 10h         //R0 = perftype << 0x10
022FD764 E1A00840 mov     r0,r0,asr 10h         //R0 = (perftype << 0x10) >> 0x10 //Basically makes it a signed 16 bits word!
022FD768 EBFFFFDB bl      22FD6DCh              ///022FD6DC
{///022FD6DC - Seems to lookup existing entities and return their index??
    022FD6DC E3E01000 mvn     r1,0h             //R1 = 0xFFFFFFFF
    022FD6E0 E1500001 cmp     r0,r1             
    if( perftype == 0xFFFFFFFF )
        022FD6E4 0A00000D beq     22FD720h      ///022FD720
    022FD6E8 E59F1038 ldr     r1,=2324D04h
    022FD6EC E3A03000 mov     r3,0h             //R3 = CntEnt = 0
    022FD6F0 E5912000 ldr     r2,[r1]           //R2 = PtrDataStructure?
    022FD6F4 EA000007 b       22FD718h          ///022FD718
    ///022FD6F8
    022FD6F8 E1D210F6 ldrsh   r1,[r2,6h]        //r1 = 
    022FD6FC E1510000 cmp     r1,r0
    if( R1 == R0 )
        022FD700 01A00003 moveq   r0,r3         //R0 = CntEnt    
        022FD704 012FFF1E bxeq    r14           //Return
    022FD708 E2831001 add     r1,r3,1h          //r1 = CntEnt + 1
    022FD70C E1A01801 mov     r1,r1,lsl 10h     //r1 = (CntEnt + 1) << 16
    022FD710 E1A03841 mov     r3,r1,asr 10h     //r3 = ((CntEnt + 1) << 16) >> 16
    022FD714 E2822F85 add     r2,r2,214h        //r2 = r2 + 214h //Increment pointer to next entry? 
    ///022FD718
    022FD718 E3530010 cmp     r3,10h
    if( CntEnt < 16 )
        022FD71C BAFFFFF5 blt     22FD6F8h          ///022FD6F8
    ///022FD720 if( R0 == 0xFFFFFFFF )
    022FD720 E3E00000 mvn     r0,0h             //R0 = 0xFFFFFFFF
    022FD724 E12FFF1E bx      r14               //Return
    022FD728 02324D04
}
022FD76C E1A09000 mov     r9,r0     //R9 = ExistingEntity?
///022FD770 if( R0 != 1 )
022FD770 E3590000 cmp     r9,0h
if( R9 >= 0 )
    022FD774 AA00000E bge     22FD7B4h          ///022FD7B4
022FD778 E59F02EC ldr     r0,=2324D04h          //r0 = EntTablePtr?
022FD77C E3A03000 mov     r3,0h                 //r3 = CntEnt = 0
022FD780 E5902000 ldr     r2,[r0]               //r2 = EntPtr
022FD784 E3E01000 mvn     r1,0h                 //r1 = 0xFFFFFFFF
022FD788 EA000007 b       22FD7ACh              ///022FD7AC    
///022FD78C
022FD78C E1D200F6 ldrsh   r0,[r2,6h]
022FD790 E1500001 cmp     r0,r1
if( r0 == r1 )
    022FD794 01A09003 moveq   r9,r3             //r9 = CntEnt
    022FD798 0A000005 beq     22FD7B4h          ///022FD7B4 v
022FD79C E2830001 add     r0,r3,1h              //r0 = CntEnt + 1
022FD7A0 E1A00800 mov     r0,r0,lsl 10h         //r0 = (CntEnt + 1) << 16
022FD7A4 E1A03840 mov     r3,r0,asr 10h         //r3 = ((CntEnt + 1) << 16) >> 16
022FD7A8 E2822F85 add     r2,r2,214h            //r2 = EntPtr + 214h
///022FD7AC  
022FD7AC E3530010 cmp     r3,10h
if( CntEnt < 16 )
    022FD7B0 BAFFFFF5 blt     22FD78Ch              ///022FD78C ^
///022FD7B4
022FD7B4 E3590000 cmp     r9,0h
if( r9 < 0 )
    022FD7B8 B3E00000 mvnlt   r0,0h             //r0 = 0xFFFFFFFF
    022FD7BC BA0000A7 blt     22FDA60h          ///022FDA60 Return
///022FD7C0 if( R9 >= 0 )
022FD7C0 E59F02A4 ldr     r0,=2324D04h          //r0 = EntTablePtr?
022FD7C4 E1D410D0 ldrsb   r1,[r4]               //r1 = PerfTypeEntry?_Unk0
022FD7C8 E5902000 ldr     r2,[r0]               //
022FD7CC E3A00F85 mov     r0,214h               //r0 = 214h
022FD7D0 E88D0082 stmea   [r13],r1,r7           //
022FD7D4 E58D6008 str     r6,[r13,8h]           //
022FD7D8 E1D830B0 ldrh    r3,[r8]               //r3 = perftype
022FD7DC E1052089 smlabb  r5,r9,r0,r2           //r5 = (r9 * 214h) + EntTablePtr?
022FD7E0 E59F1288 ldr     r1,=2322160h          //"GroundPerformer Add id %3d  kind %3d  type %3d  hanger %3d  sector %3d"
022FD7E4 E1A02009 mov     r2,r9
022FD7E8 E3A00001 mov     r0,1h
022FD7EC EBF43A93 bl      200C240h              //DebugPrint
022FD7F0 E1D510F6 ldrsh   r1,[r5,6h]            
022FD7F4 E3E00000 mvn     r0,0h                 //r0 = 0xFFFFFFFF
022FD7F8 E1510000 cmp     r1,r0
if( r1 == 0xFFFFFFFF )
    022FD7FC 03A00001 moveq   r0,1h
else
    022FD800 13A00000 movne   r0,0h
022FD804 E210A0FF ands    r10,r0,0FFh
if( r10 ==  )
022FD808 0A000003 beq     22FD81Ch
022FD80C E3A01000 mov     r1,0h                 //r1 = 0
022FD810 E2850E15 add     r0,r5,150h            //r0 = EntPtr + 150h
022FD814 E5C51008 strb    r1,[r5,8h]            //
022FD818 EBFFDC7D bl      22F4A14h              ///022F4A14
{
    022F4A14 E92D4010 push    r4,r14
    022F4A18 E1A04000 mov     r4,r0
    022F4A1C E3A00080 mov     r0,80h
    022F4A20 E1C409B0 strh    r0,[r4,90h]
    022F4A24 E3A00000 mov     r0,0h
    022F4A28 E59F3084 ldr     r3,=238A05Ch
    022F4A2C E1A02004 mov     r2,r4
    022F4A30 E58400A8 str     r0,[r4,0A8h]
    022F4A34 E3A01006 mov     r1,6h
    022F4A38 E0D300B2 ldrh    r0,[r3],2h
    022F4A3C E2511001 subs    r1,r1,1h
    022F4A40 E0C200B2 strh    r0,[r2],2h
    022F4A44 1AFFFFFB bne     22F4A38h
    022F4A48 E3A01000 mov     r1,0h
    022F4A4C E58410B8 str     r1,[r4,0B8h]
    022F4A50 E58410BC str     r1,[r4,0BCh]
    022F4A54 E2410001 sub     r0,r1,1h
    022F4A58 E58400C0 str     r0,[r4,0C0h]
    022F4A5C E5C410B6 strb    r1,[r4,0B6h]
    022F4A60 E1C41ABC strh    r1,[r4,0ACh]
    022F4A64 E1C41ABE strh    r1,[r4,0AEh]
    022F4A68 E1C409B2 strh    r0,[r4,92h]
    022F4A6C E1C419B4 strh    r1,[r4,94h]
    022F4A70 E1C409B6 strh    r0,[r4,96h]
    022F4A74 E5C40098 strb    r0,[r4,98h]
    022F4A78 E1C419BA strh    r1,[r4,9Ah]
    022F4A7C E1C409BC strh    r0,[r4,9Ch]
    022F4A80 E1C419BE strh    r1,[r4,9Eh]
    022F4A84 E5C400A0 strb    r0,[r4,0A0h]
    022F4A88 E1C40AB2 strh    r0,[r4,0A2h]
    022F4A8C E3A010FF mov     r1,0FFh
    022F4A90 E284000C add     r0,r4,0Ch
    022F4A94 E5C410A4 strb    r1,[r4,0A4h]
    022F4A98 EBF49D84 bl      201C0B0h
    022F4A9C E3A00000 mov     r0,0h
    022F4AA0 E5C4008A strb    r0,[r4,8Ah]
    022F4AA4 E1C408BC strh    r0,[r4,8Ch]
    022F4AA8 E1C408BE strh    r0,[r4,8Eh]
    022F4AAC E1C408B8 strh    r0,[r4,88h]
    022F4AB0 E8BD8010 pop     r4,r15
    022F4AB4 0238A05C
}
022FD81C E2470001 sub     r0,r7,1h              //r0 = ScriptDataTy - 1
022FD820 E1C590B4 strh    r9,[r5,4h]            
022FD824 E1A00800 mov     r0,r0,lsl 10h         //r0 = ScriptDataTy << 16
022FD828 E1D810B0 ldrh    r1,[r8]               //r1 = perftype
022FD82C E1A00840 mov     r0,r0,asr 10h         //r0 = (ScriptDataTy << 16) >> 16
022FD830 E1A00800 mov     r0,r0,lsl 10h         //r0 = ((ScriptDataTy << 16) >> 16) << 16
022FD834 E1A00820 mov     r0,r0,lsr 10h         //r0 = (((ScriptDataTy << 16) >> 16) << 16) >> 16 (Converted to signed and then to unsigned..)
022FD838 E3500001 cmp     r0,1h
022FD83C E1C510B6 strh    r1,[r5,6h]
if( r0 <= 1 )
    022FD840 91C570BA strlsh  r7,[r5,0Ah]
    022FD844 95C5600C strlsb  r6,[r5,0Ch]
    022FD848 93E00000 mvnls   r0,0h
    022FD84C 95C5000D strlsb  r0,[r5,0Dh]
    022FD850 9A000004 bls     22FD868h              ///022FD868 v
022FD854 E35A0000 cmp     r10,0h
if( r10 != 0 )
    022FD858 13E00000 mvnne   r0,0h
    022FD85C 11C500BA strneh  r0,[r5,0Ah]
    022FD860 15C5000C strneb  r0,[r5,0Ch]
022FD864 E5C5600D strb    r6,[r5,0Dh]
///022FD868
022FD868 E1D400D0 ldrsb   r0,[r4]                   //r0 = PerfTypeEntry?_Unk0
022FD86C E3500005 cmp     r0,5h
if( PerfTypeEntry?_Unk0 <= 5 )
    022FD870 908FF100 addls   r15,r15,r0,lsl 2h //r15 = 022FD874 + (PerfTypeEntry?_Unk0 * 4) + 4
022FD874 EA000014 b       22FD8CCh              ///022FD8CC
022FD878 EA000013 b       22FD8CCh              ///022FD8CC PerfTy0
022FD87C EA000003 b       22FD890h              ///022FD890 PerfTy1
022FD880 EA000005 b       22FD89Ch              ///022FD89C PerfTy2
022FD884 EA000007 b       22FD8A8h              ///022FD8A8 PerfTy3
022FD888 EA000009 b       22FD8B4h              ///022FD8B4 PerfTy4
022FD88C EA00000B b       22FD8C0h              ///022FD8C0 PerfTy5
///022FD890 PerfTy1
022FD890 E3A00502 mov     r0,800000h
022FD894 E5850128 str     r0,[r5,128h]
022FD898 EA00000D b       22FD8D4h              ///022FD8D4
///022FD89C PerfTy2
022FD89C E3A00783 mov     r0,20C0000h
022FD8A0 E5850128 str     r0,[r5,128h]
022FD8A4 EA00000A b       22FD8D4h              ///022FD8D4
///022FD8A8 PerfTy3
022FD8A8 E3A00783 mov     r0,20C0000h
022FD8AC E5850128 str     r0,[r5,128h]
022FD8B0 EA000007 b       22FD8D4h              ///022FD8D4
///022FD8B4 PerfTy4
022FD8B4 E3A00402 mov     r0,2000000h
022FD8B8 E5850128 str     r0,[r5,128h]
022FD8BC EA000004 b       22FD8D4h              ///022FD8D4
///022FD8C0 PerfTy5
022FD8C0 E59F01AC ldr     r0,=2020000h
022FD8C4 E5850128 str     r0,[r5,128h]
022FD8C8 EA000001 b       22FD8D4h              ///022FD8D4
///022FD8CC PerfTy0
022FD8CC E3A00783 mov     r0,20C0000h
022FD8D0 E5850128 str     r0,[r5,128h]
///022FD8D4
022FD8D4 E5D40003 ldrb    r0,[r4,3h]            //r0 = PerfTypeEntry?_Unk3
022FD8D8 E3500000 cmp     r0,0h
if( r0 != 0 )
    022FD8DC 15950128 ldrne   r0,[r5,128h]
    022FD8E0 13800102 orrne   r0,r0,80000000h
    022FD8E4 15850128 strne   r0,[r5,128h]
    //  struct PerformerEntryMemory
//      0x00 perftype
//      0x02 direction
//      0x03 perfunk0x04
//      0x04 perfunk0x06
//      0x05 perfunk0x08
//      0x06 perfunk0x0A
//      0x07 perfunk0x0C
//      0x08 perfunk0x0E  
022FD8E8 E5D40001 ldrb    r0,[r4,1h]            //r0 = PerfTypeEntry?_Unk1
022FD8EC E3500000 cmp     r0,0h
if( PerfTypeEntry?_Unk1 == 0 )
    022FD8F0 05D80003 ldreqb  r0,[r8,3h]            //r0 = perfunk0x04
    022FD8F4 01A00580 moveq   r0,r0,lsl 0Bh         //r0 = perfunk0x04 << 11
    022FD8F8 05850010 streq   r0,[r5,10h]           //[r5,10h] = perfunk0x04 << 11
    022FD8FC 05D80004 ldreqb  r0,[r8,4h]            //r0 = perfunk0x06
else
    022FD900 11A00580 movne   r0,r0,lsl 0Bh         //r0 = PerfTypeEntry?_Unk1 << 11
    022FD904 15850010 strne   r0,[r5,10h]           //[r5,10h] = PerfTypeEntry?_Unk1 << 11
    022FD908 15D40002 ldrneb  r0,[r4,2h]            //r0 = PerfTypeEntry?_Unk2
022FD90C E1A00580 mov     r0,r0,lsl 0Bh             //r0 = r0 << 11
022FD910 E5850014 str     r0,[r5,14h]
022FD914 E5950010 ldr     r0,[r5,10h]
022FD918 E0800FA0 add     r0,r0,r0,lsr 1Fh
022FD91C E1A000C0 mov     r0,r0,asr 1h
022FD920 E5850018 str     r0,[r5,18h]
022FD924 E5950014 ldr     r0,[r5,14h]
022FD928 E0800FA0 add     r0,r0,r0,lsr 1Fh
022FD92C E1A000C0 mov     r0,r0,asr 1h
022FD930 E585001C str     r0,[r5,1Ch]
022FD934 E1D800D2 ldrsb   r0,[r8,2h]                //r0 = direction
022FD938 E5C50020 strb    r0,[r5,20h]
022FD93C E5D80007 ldrb    r0,[r8,7h]                //r0 = perfunk0x0C
022FD940 E3100004 tst     r0,4h
if( perfunk0x0C & 4 != 0 )
    022FD944 1A000007 bne     22FD968h              ///022FD968 perfunk0x0C & 4 != 0
022FD948 E5D80005 ldrb    r0,[r8,5h]                //r0 = perfunk0x08
022FD94C E1A00580 mov     r0,r0,lsl 0Bh             //r0 = perfunk0x08 << 11
022FD950 E5850024 str     r0,[r5,24h]               //[r5,24h] = (perfunk0x08 << 11)
022FD954 E5D80007 ldrb    r0,[r8,7h]                //r0 = perfunk0x0C
022FD958 E3100002 tst     r0,2h
if( perfunk0x0C & 2 != 0 )
    022FD95C 15950024 ldrne   r0,[r5,24h]           //r0 = (perfunk0x08 << 11)
    022FD960 12800B01 addne   r0,r0,400h            //r0 = (perfunk0x08 << 11) + 400h
    022FD964 15850024 strne   r0,[r5,24h]           //[r5,24h] = ((perfunk0x08 << 11) + 400h)
///022FD968 perfunk0x0C & 4 != 0
022FD968 E5D80008 ldrb    r0,[r8,8h]                //r0 = perfunk0x0E
022FD96C E3100004 tst     r0,4h
if( perfunk0x0E & 4 != 0 )
    022FD970 1A000007 bne     22FD994h              ///022FD994 perfunk0x0E & 4 != 0
022FD974 E5D80006 ldrb    r0,[r8,6h]                //r0 = perfunk0x0A
022FD978 E1A00580 mov     r0,r0,lsl 0Bh             //r0 = perfunk0x0A << 11
022FD97C E5850028 str     r0,[r5,28h]               //[r5,28h] = (perfunk0x0A << 11)
022FD980 E5D80008 ldrb    r0,[r8,8h]                //r0 = perfunk0x0E
022FD984 E3100002 tst     r0,2h
if( perfunk0x0E & 2 != 0 )
    022FD988 15950028 ldrne   r0,[r5,28h]
    022FD98C 12800B01 addne   r0,r0,400h
    022FD990 15850028 strne   r0,[r5,28h]
///022FD994 perfunk0x0E & 4 != 0
022FD994 E5DD0030 ldrb    r0,[r13,30h]
022FD998 E3500000 cmp     r0,0h
022FD99C 15950128 ldrne   r0,[r5,128h]
022FD9A0 13800502 orrne   r0,r0,800000h
022FD9A4 15850128 strne   r0,[r5,128h]
022FD9A8 E35A0000 cmp     r10,0h
022FD9AC 0A000028 beq     22FDA54h
022FD9B0 E1D502D0 ldrsb   r0,[r5,20h]
022FD9B4 E3A02000 mov     r2,0h
022FD9B8 E5C5012C strb    r0,[r5,12Ch]
022FD9BC E5953024 ldr     r3,[r5,24h]
022FD9C0 E5951018 ldr     r1,[r5,18h]
022FD9C4 E285002C add     r0,r5,2Ch
022FD9C8 E0431001 sub     r1,r3,r1
022FD9CC E5851130 str     r1,[r5,130h]
022FD9D0 E5954024 ldr     r4,[r5,24h]
022FD9D4 E5953018 ldr     r3,[r5,18h]
022FD9D8 E2851034 add     r1,r5,34h
022FD9DC E0843003 add     r3,r4,r3
022FD9E0 E5853138 str     r3,[r5,138h]
022FD9E4 E5954028 ldr     r4,[r5,28h]
022FD9E8 E595301C ldr     r3,[r5,1Ch]
022FD9EC E0443003 sub     r3,r4,r3
022FD9F0 E5853134 str     r3,[r5,134h]
022FD9F4 E5954028 ldr     r4,[r5,28h]
022FD9F8 E595301C ldr     r3,[r5,1Ch]
022FD9FC E0843003 add     r3,r4,r3
022FDA00 E585313C str     r3,[r5,13Ch]
022FDA04 E5852144 str     r2,[r5,144h]
022FDA08 E5852140 str     r2,[r5,140h]
022FDA0C EBFFD13D bl      22F1F08h
022FDA10 E2850C01 add     r0,r5,100h
022FDA14 E3A02B02 mov     r2,800h
022FDA18 E3A01001 mov     r1,1h
022FDA1C E1C024BA strh    r2,[r0,4Ah]
022FDA20 E5C51148 strb    r1,[r5,148h]
022FDA24 E2411002 sub     r1,r1,2h
022FDA28 E1C014BC strh    r1,[r0,4Ch]
022FDA2C E3A01000 mov     r1,0h
022FDA30 E1C014BE strh    r1,[r0,4Eh]
022FDA34 E1D510F6 ldrsh   r1,[r5,6h]
022FDA38 E5952128 ldr     r2,[r5,128h]
022FDA3C E2850E15 add     r0,r5,150h
022FDA40 EBFFE541 bl      22F6F4Ch
022FDA44 E59F102C ldr     r1,=2322110h
022FDA48 E1A02005 mov     r2,r5
022FDA4C E285003C add     r0,r5,3Ch
022FDA50 EBFF7BA4 bl      22DC8E8h
022FDA54 E3A01001 mov     r1,1h
022FDA58 E1A00009 mov     r0,r9
022FDA5C E5C51008 strb    r1,[r5,8h]
///022FDA60 Return
022FDA60 E28DD00C add     r13,r13,0Ch
022FDA64 E8BD87F8 pop     r3-r10,r15
022FDA68 02320858 eoreqs  r0,r2,580000h
022FDA6C 02324D04 eoreqs  r4,r2,100h
022FDA70 02322160 eoreqs  r2,r2,18h
022FDA74 02020000 andeq   r0,r2,0h
022FDA78 02322110 eoreqs  r2,r2,4h

/*
    fun_022FEA98 22FEA98h GroundEventAdd
        R0=GroundEventAddParam0, R1 =EventEntryMemory, R2 =ScriptDataTy, R3=Param2, R4=PtrFileBuff
        
//  struct EventEntryMemory
//      0x00 trigger croutineid
//      0x02 evntunk0x00
//      0x03 evntunk0x02
//      0x04 evntunk0x04
//      0x05 evntunk0x06
//      0x06 evntunk0x08
//      0x07 evntunk0x0A
//      0x08 trigger scriptid
*/
022FEA98 E92D47F0 push    r4-r10,r14
022FEA9C E24DD010 sub     r13,r13,10h
022FEAA0 E1B0A000 movs    r10,r0            //R10 = GroundEventAddParam0
022FEAA4 E1A09001 mov     r9,r1             //R9  = EventEntryMemory
022FEAA8 E1A08002 mov     r8,r2             //R8  = ScriptDataTy
022FEAAC E1A07003 mov     r7,r3             //R7  = Param2
if( GroundEventAddParam0 >= 0 )
    022FEAB0 5A000012 bpl     22FEB00h      //022FEB00
else
    022FEAB4 E59F0138 ldr     r0,=2324D08h      //
    022FEAB8 E3A02000 mov     r2,0h
    022FEABC E5904000 ldr     r4,[r0]
    022FEAC0 E3E01000 mvn     r1,0h
    022FEAC4 EA000007 b       22FEAE8h          //022FEAE8
///LOOP_BEG 022FEAC8
022FEAC8 E1D400F2 ldrsh   r0,[r4,2h]
022FEACC E1500001 cmp     r0,r1
if( R0 == R1 )
    022FEAD0 01A0A002 moveq   r10,r2
    022FEAD4 0A000005 beq     22FEAF0h          //022FEAF0
else
    022FEAD8 E2820001 add     r0,r2,1h
    022FEADC E1A00800 mov     r0,r0,lsl 10h
    022FEAE0 E1A02840 mov     r2,r0,asr 10h
    022FEAE4 E2844020 add     r4,r4,20h
//022FEAE8
022FEAE8 E3520020 cmp     r2,20h
if( R2 < 0x20 )
    022FEAEC BAFFFFF5 blt     22FEAC8h              ///LOOP_BEG 022FEAC8
//022FEAF0
022FEAF0 E35A0000 cmp     r10,0h
if( PtrFilebuffer? >= 0 )
    022FEAF4 AA000004 bge     22FEB0Ch              ///022FEB0C
022FEAF8 E3E00000 mvn     r0,0h
022FEAFC EA00003A b       22FEBECh
//If GroundEventAddParam0 >= 0
022FEB00 E59F00EC ldr     r0,=2324D08h
022FEB04 E5900000 ldr     r0,[r0]
022FEB08 E080428A add     r4,r0,r10,lsl 5h
///022FEB0C
022FEB0C E58D8000 str     r8,[r13]                  //[r13] = ScriptDataTy
022FEB10 E58D7004 str     r7,[r13,4h]               //[r13,4h] = Param2
022FEB14 E1D930F0 ldrsh   r3,[r9]                   //R3 = trigger croutineid
022FEB18 E59F10D8 ldr     r1,=2322368h              //"GroundEvent Add id %3d  kind %3d  hanger %3d  sector %3d"
022FEB1C E1A0200A mov     r2,r10                    //R2 = GroundEventAddParam0 
022FEB20 E3A00001 mov     r0,1h                     //R0 = 1
022FEB24 EBF435C5 bl      200C240h                  //DebugPrint
022FEB28 E1D900F0 ldrsh   r0,[r9]                   //R0 = trigger croutineid
022FEB2C EBFFA51D bl      22E7FA8h                  ///022E7FA8
{///022E7FA8
    022E7FA8 E59F1004 ldr     r1,=231C828h          //Common Routines Table Beginning
    022E7FAC E0810180 add     r0,r1,r0,lsl 3h       //R0 = PtrEntryCommonRoutine
    022E7FB0 E12FFF1E bx      r14
    022E7FB4 0231C828 
}
022FEB30 E1C4A0B0 strh    r10,[r4]                  //[R4]  = GroundEventAddParam0
022FEB34 E1D910F0 ldrsh   r1,[r9]                   //R1    = trigger croutineid
022FEB38 E1C410B2 strh    r1,[r4,2h]                //[r4,2h] = trigger croutineid
022FEB3C E1C480B4 strh    r8,[r4,4h]                //[r4,4h] = ScriptDataTy?
022FEB40 E5C47006 strb    r7,[r4,6h]                //[r4,6h] = Param2
022FEB44 E1D910F8 ldrsh   r1,[r9,8h]                //R1    = event unk5
022FEB48 E1C410B8 strh    r1,[r4,8h]                //[r4,8h] = trigger scriptid
022FEB4C E1D000F2 ldrsh   r0,[r0,2h]                //R0    = CommonRoutineEntryUnk1
022FEB50 E3500001 cmp     r0,1h
022FEB54 13500002 cmpne   r0,2h
if( CommonRoutineEntryUnk1 == 1 || CommonRoutineEntryUnk1 == 2 )
    022FEB58 0A000001 beq     22FEB64h              ///022FEB64
else
    022FEB5C E3500007 cmp     r0,7h                     ///!!!!!! THIS SETS THE FLAGS AND HANDLES THEM LATER AT 022FEB70 !!!!!
    022FEB60 EA000002 b       22FEB70h                  ///022FEB70
///022FEB64
022FEB64 E3A00B06 mov     r0,1800h
022FEB68 E584000C str     r0,[r4,0Ch]               //[r4,0Ch] = 1800h
022FEB6C EA000001 b       22FEB78h                  ///022FEB78
///022FEB70
022FEB70 E3A00B02 mov     r0,800h
022FEB74 E584000C str     r0,[r4,0Ch]               //[r4,0Ch] = 800h
///022FEB78
022FEB78 E5D92002 ldrb    r2,[r9,2h]                //R2 = evntunk0x00
022FEB7C E5D90003 ldrb    r0,[r9,3h]                //R0 = evntunk0x02
022FEB80 E5D91006 ldrb    r1,[r9,6h]                //R1 = evntunk0x08
022FEB84 E1A02582 mov     r2,r2,lsl 0Bh             //R2 = evntunk0x00 << 0xB
022FEB88 E1A00580 mov     r0,r0,lsl 0Bh             //R0 = evntunk0x02 << 0xB
022FEB8C E58D2008 str     r2,[r13,8h]               //[r13,8h]   = (evntunk0x00 << 0xB)
022FEB90 E58D000C str     r0,[r13,0Ch]              //[r13,0Ch] = (evntunk0x02 << 0xB)
022FEB94 E3110004 tst     r1,4h
if( evntunk0x08 & 4 != 0 )
    022FEB98 1A000003 bne     22FEBACh              ///022FEBAC
022FEB9C E5D90004 ldrb    r0,[r9,4h]                //R0 = evntunk0x04
022FEBA0 E3110002 tst     r1,2h                     
022FEBA4 E1A05580 mov     r5,r0,lsl 0Bh             //R5 = evntunk0x04 << 0xB
if( evntunk0x08 & 2 != 0 )
    022FEBA8 12855B01 addne   r5,r5,400h            //R5 = (evntunk0x04 << 0xB) + 0x400
//022FEBAC evntunk0x08 & 4 != 0 
022FEBAC E5D91007 ldrb    r1,[r9,7h]                //R1 = evntunk0x0A
022FEBB0 E3110004 tst     r1,4h
if( evntunk0x0A & 4 != 0 )
    022FEBB4 1A000003 bne     22FEBC8h              ///022FEBC8
022FEBB8 E5D90005 ldrb    r0,[r9,5h]                ///R0 = evntunk0x06
022FEBBC E3110002 tst     r1,2h
022FEBC0 E1A06580 mov     r6,r0,lsl 0Bh             //R6 = (evntunk0x06 << 0xB)
if( evntunk0x0A & 2 != 0 )
    022FEBC4 12866B01 addne   r6,r6,400h            //R6 = (evntunk0x06 << 0xB) + 0x400
///022FEBC8 evntunk0x0A & 4 != 0
022FEBC8 E59D1008 ldr     r1,[r13,8h]               //R1 = (evntunk0x00 << 0xB)
022FEBCC E59D000C ldr     r0,[r13,0Ch]              //R0 = (evntunk0x02 << 0xB)
022FEBD0 E5845010 str     r5,[r4,10h]               //[r4,10h] = (evntunk0x04 << 0xB) or ((evntunk0x04 << 0xB) + 0x400)
022FEBD4 E0851001 add     r1,r5,r1                  //R1 = (evntunk0x04 << 0xB) or ((evntunk0x04 << 0xB) + 0x400) + (evntunk0x00 << 0xB)
022FEBD8 E5841018 str     r1,[r4,18h]               //[r4,18h] = r1 
022FEBDC E5846014 str     r6,[r4,14h]               //[r4,14h] = (evntunk0x06 << 0xB) or ((evntunk0x06 << 0xB) + 0x400)
022FEBE0 E0861000 add     r1,r6,r0                  //r1 = (evntunk0x06 << 0xB) or ((evntunk0x06 << 0xB) + 0x400)  + (evntunk0x02 << 0xB)
022FEBE4 E1A0000A mov     r0,r10                    //r0 = GroundEventAddParam0
022FEBE8 E584101C str     r1,[r4,1Ch]               //[r4,1Ch] = r1
022FEBEC E28DD010 add     r13,r13,10h
022FEBF0 E8BD87F0 pop     r4-r10,r15
022FEBF4 02324D08
022FEBF8 02322368


//
//

/*
    
        Reference to the the string to load LSD files!
*/
022FF5D0 E92D41F0 push    r4-r8,r14
022FF5D4 E24DDF4A sub     r13,r13,128h
022FF5D8 E59F0400 ldr     r0,=2324D14h
022FF5DC E5901004 ldr     r1,[r0,4h]
022FF5E0 E5910004 ldr     r0,[r1,4h]
022FF5E4 E3500005 cmp     r0,5h
022FF5E8 908FF100 addls   r15,r15,r0,lsl 2h
022FF5EC EA0000F8 b       22FF9D4h
022FF5F0 EA000004 b       22FF608h
022FF5F4 EA000026 b       22FF694h
022FF5F8 EA000049 b       22FF724h
022FF5FC EA0000AD b       22FF8B8h
022FF600 EA0000D2 b       22FF950h
022FF604 EA0000EE b       22FF9C4h
022FF608 E1D100D0 ldrsb   r0,[r1]
022FF60C E3E01001 mvn     r1,1h
022FF610 E1500001 cmp     r0,r1
022FF614 0A00000A beq     22FF644h
022FF618 EBF4B1A7 bl      202BCBCh
022FF61C E3500000 cmp     r0,0h
022FF620 1A0000EB bne     22FF9D4h
022FF624 E59F03B4 ldr     r0,=2324D14h
022FF628 E5900004 ldr     r0,[r0,4h]
022FF62C E1D000D0 ldrsb   r0,[r0]
022FF630 EBF4B183 bl      202BC44h
022FF634 E59F03A4 ldr     r0,=2324D14h
022FF638 E3E01001 mvn     r1,1h
022FF63C E5900004 ldr     r0,[r0,4h]
022FF640 E5C01000 strb    r1,[r0]
022FF644 E59F0394 ldr     r0,=2324D14h
022FF648 E59F1394 ldr     r1,=1AFh
022FF64C E5902004 ldr     r2,[r0,4h]
022FF650 E59F0390 ldr     r0,=23224ACh
022FF654 E1D241FC ldrsh   r4,[r2,1Ch]
022FF658 E59F338C ldr     r3,=22FF9FCh
022FF65C E28D2090 add     r2,r13,90h
022FF660 E58D4090 str     r4,[r13,90h]
022FF664 E58D1000 str     r1,[r13]
022FF668 E3A0400A mov     r4,0Ah
022FF66C E3A01033 mov     r1,33h
022FF670 E58D4004 str     r4,[r13,4h]
022FF674 EBF4B0E9 bl      202BA20h
022FF678 E59F1360 ldr     r1,=2324D14h
022FF67C E3A02001 mov     r2,1h
022FF680 E5913004 ldr     r3,[r1,4h]
022FF684 E5C30000 strb    r0,[r3]
022FF688 E5910004 ldr     r0,[r1,4h]
022FF68C E5802004 str     r2,[r0,4h]
022FF690 EA0000CF b       22FF9D4h
022FF694 E1D100D0 ldrsb   r0,[r1]
022FF698 EBF4B18F bl      202BCDCh
022FF69C E3500000 cmp     r0,0h
022FF6A0 1A0000CB bne     22FF9D4h
022FF6A4 E59F0334 ldr     r0,=2324D14h
022FF6A8 E5900004 ldr     r0,[r0,4h]
022FF6AC E1D000D0 ldrsb   r0,[r0]
022FF6B0 EBF4B196 bl      202BD10h
022FF6B4 E3500000 cmp     r0,0h
022FF6B8 AA00000B bge     22FF6ECh
022FF6BC E59F031C ldr     r0,=2324D14h
022FF6C0 E3E02000 mvn     r2,0h
022FF6C4 E5901004 ldr     r1,[r0,4h]
022FF6C8 E1C121BC strh    r2,[r1,1Ch]
022FF6CC E5900004 ldr     r0,[r0,4h]
022FF6D0 E2800014 add     r0,r0,14h
022FF6D4 EBFF93C6 bl      22E45F4h
022FF6D8 E59F0300 ldr     r0,=2324D14h
022FF6DC E3A01004 mov     r1,4h
022FF6E0 E5900004 ldr     r0,[r0,4h]
022FF6E4 E5801004 str     r1,[r0,4h]
022FF6E8 EA0000B9 b       22FF9D4h
022FF6EC E59F12EC ldr     r1,=2324D14h
022FF6F0 E5913004 ldr     r3,[r1,4h]
022FF6F4 E1D321FC ldrsh   r2,[r3,1Ch]
022FF6F8 E1520000 cmp     r2,r0
022FF6FC 0A000003 beq     22FF710h
022FF700 E1C301BC strh    r0,[r3,1Ch]
022FF704 E5910004 ldr     r0,[r1,4h]
022FF708 E2800014 add     r0,r0,14h
022FF70C EBFF93B8 bl      22E45F4h
022FF710 E59F02C8 ldr     r0,=2324D14h
022FF714 E3A01002 mov     r1,2h
022FF718 E5900004 ldr     r0,[r0,4h]
022FF71C E5801004 str     r1,[r0,4h]
022FF720 EA0000AB b       22FF9D4h
022FF724 E1D100D0 ldrsb   r0,[r1]
022FF728 E3E01001 mvn     r1,1h
022FF72C E1500001 cmp     r0,r1
022FF730 0A00000A beq     22FF760h
022FF734 EBF4B160 bl      202BCBCh
022FF738 E3500000 cmp     r0,0h
022FF73C 1A0000A4 bne     22FF9D4h
022FF740 E59F0298 ldr     r0,=2324D14h
022FF744 E5900004 ldr     r0,[r0,4h]
022FF748 E1D000D0 ldrsb   r0,[r0]
022FF74C EBF4B13C bl      202BC44h
022FF750 E59F0288 ldr     r0,=2324D14h
022FF754 E3E01001 mvn     r1,1h
022FF758 E5900004 ldr     r0,[r0,4h]
022FF75C E5C01000 strb    r1,[r0]
022FF760 E59F0278 ldr     r0,=2324D14h
022FF764 E5900004 ldr     r0,[r0,4h]
022FF768 E1D001FC ldrsh   r0,[r0,1Ch]
022FF76C EBF59622 bl      2064FFCh
022FF770 E1A02000 mov     r2,r0
022FF774 E59F1274 ldr     r1,=23224BCh              //"SCRIPT/%s/%s.lsd"
022FF778 E28D0010 add     r0,r13,10h
022FF77C E1A03002 mov     r3,r2
022FF780 E3A05000 mov     r5,0h
022FF784 EBF6277E bl      2089584h                      ///fun_02089584 ReplaceTokens
022FF788 E28D0010 add     r0,r13,10h
022FF78C EBF42505 bl      2008BA8h
022FF790 E3500000 cmp     r0,0h
022FF794 D59F0244 ldrle   r0,=2324D14h
022FF798 D1A01005 movle   r1,r5
022FF79C D5900004 ldrle   r0,[r0,4h]
022FF7A0 D5801004 strle   r1,[r0,4h]
022FF7A4 DA00008A ble     22FF9D4h
022FF7A8 E59F0230 ldr     r0,=2324D14h
022FF7AC E28D1010 add     r1,r13,10h
022FF7B0 E5900004 ldr     r0,[r0,4h]
022FF7B4 E3A02008 mov     r2,8h
022FF7B8 E2800008 add     r0,r0,8h
022FF7BC EBFF93C6 bl      22E46DCh
022FF7C0 E59F0218 ldr     r0,=2324D14h
022FF7C4 E5902004 ldr     r2,[r0,4h]
022FF7C8 E5921008 ldr     r1,[r2,8h]
022FF7CC E1D110B0 ldrh    r1,[r1]
022FF7D0 E5821010 str     r1,[r2,10h]
022FF7D4 E5901004 ldr     r1,[r0,4h]
022FF7D8 E5910010 ldr     r0,[r1,10h]
022FF7DC E3500000 cmp     r0,0h
022FF7E0 CA000006 bgt     22FF800h
022FF7E4 E2810008 add     r0,r1,8h
022FF7E8 EBF424F9 bl      2008BD4h
022FF7EC E59F01EC ldr     r0,=2324D14h
022FF7F0 E1A01005 mov     r1,r5
022FF7F4 E5900004 ldr     r0,[r0,4h]
022FF7F8 E5801004 str     r1,[r0,4h]
022FF7FC EA000074 b       22FF9D4h
022FF800 E2810014 add     r0,r1,14h
022FF804 EBFF9389 bl      22E4630h
022FF808 E3500000 cmp     r0,0h
022FF80C 1A000016 bne     22FF86Ch
022FF810 E59F81C8 ldr     r8,=2324D14h
022FF814 E1A07005 mov     r7,r5
022FF818 E5980004 ldr     r0,[r8,4h]
022FF81C E28D4008 add     r4,r13,8h
022FF820 E5906008 ldr     r6,[r0,8h]
022FF824 EA00000C b       22FF85Ch
022FF828 E1A00187 mov     r0,r7,lsl 3h
022FF82C E2801002 add     r1,r0,2h
022FF830 E1A00004 mov     r0,r4
022FF834 E0861001 add     r1,r6,r1
022FF838 EBFF9370 bl      22E4600h
022FF83C E5981004 ldr     r1,[r8,4h]
022FF840 E1A00004 mov     r0,r4
022FF844 E2811014 add     r1,r1,14h
022FF848 EBFF9383 bl      22E465Ch
022FF84C E3500000 cmp     r0,0h
022FF850 11A05007 movne   r5,r7
022FF854 1A000004 bne     22FF86Ch
022FF858 E2877001 add     r7,r7,1h
022FF85C E5980004 ldr     r0,[r8,4h]
022FF860 E5900010 ldr     r0,[r0,10h]
022FF864 E1570000 cmp     r7,r0
022FF868 BAFFFFEE blt     22FF828h
022FF86C E59F016C ldr     r0,=2324D14h
022FF870 E58D5090 str     r5,[r13,90h]
022FF874 E5901004 ldr     r1,[r0,4h]
022FF878 E59F0168 ldr     r0,=23224ACh
022FF87C E5911010 ldr     r1,[r1,10h]
022FF880 E59F316C ldr     r3,=22FFA14h
022FF884 E58D1000 str     r1,[r13]
022FF888 E3A0400A mov     r4,0Ah
022FF88C E28D2090 add     r2,r13,90h
022FF890 E3A01033 mov     r1,33h
022FF894 E58D4004 str     r4,[r13,4h]
022FF898 EBF4B060 bl      202BA20h
022FF89C E59F113C ldr     r1,=2324D14h
022FF8A0 E3A02003 mov     r2,3h
022FF8A4 E5913004 ldr     r3,[r1,4h]
022FF8A8 E5C30000 strb    r0,[r3]
022FF8AC E5910004 ldr     r0,[r1,4h]
022FF8B0 E5802004 str     r2,[r0,4h]
022FF8B4 EA000046 b       22FF9D4h
022FF8B8 E1D100D0 ldrsb   r0,[r1]
022FF8BC EBF4B106 bl      202BCDCh
022FF8C0 E3500000 cmp     r0,0h
022FF8C4 1A000042 bne     22FF9D4h
022FF8C8 E59F0110 ldr     r0,=2324D14h
022FF8CC E5900004 ldr     r0,[r0,4h]
022FF8D0 E1D000D0 ldrsb   r0,[r0]
022FF8D4 EBF4B10D bl      202BD10h
022FF8D8 E3500000 cmp     r0,0h
022FF8DC B59F00FC ldrlt   r0,=2324D14h
022FF8E0 B3A01000 movlt   r1,0h
022FF8E4 B5900004 ldrlt   r0,[r0,4h]
022FF8E8 B5801004 strlt   r1,[r0,4h]
022FF8EC BA000012 blt     22FF93Ch
022FF8F0 E59F10E8 ldr     r1,=2324D14h
022FF8F4 E1A00180 mov     r0,r0,lsl 3h
022FF8F8 E5913004 ldr     r3,[r1,4h]
022FF8FC E2801002 add     r1,r0,2h
022FF900 E5932008 ldr     r2,[r3,8h]
022FF904 E2830014 add     r0,r3,14h
022FF908 E0821001 add     r1,r2,r1
022FF90C EBFF933B bl      22E4600h
022FF910 E59F00C8 ldr     r0,=2324D14h
022FF914 E59F20DC ldr     r2,=238A078h
022FF918 E5904004 ldr     r4,[r0,4h]
022FF91C E3A01008 mov     r1,8h
022FF920 E2843014 add     r3,r4,14h
022FF924 E4D30001 ldrb    r0,[r3],1h
022FF928 E2511001 subs    r1,r1,1h
022FF92C E4C20001 strb    r0,[r2],1h
022FF930 1AFFFFFB bne     22FF924h
022FF934 E3A00004 mov     r0,4h
022FF938 E5840004 str     r0,[r4,4h]
022FF93C E59F009C ldr     r0,=2324D14h
022FF940 E5900004 ldr     r0,[r0,4h]
022FF944 E2800008 add     r0,r0,8h
022FF948 EBF424A1 bl      2008BD4h
022FF94C EA000020 b       22FF9D4h
022FF950 E1D100D0 ldrsb   r0,[r1]
022FF954 E3E01001 mvn     r1,1h
022FF958 E1500001 cmp     r0,r1
022FF95C 0A00000A beq     22FF98Ch
022FF960 EBF4B0D5 bl      202BCBCh
022FF964 E3500000 cmp     r0,0h
022FF968 1A000019 bne     22FF9D4h
022FF96C E59F006C ldr     r0,=2324D14h
022FF970 E5900004 ldr     r0,[r0,4h]
022FF974 E1D000D0 ldrsb   r0,[r0]
022FF978 EBF4B0B1 bl      202BC44h
022FF97C E59F005C ldr     r0,=2324D14h
022FF980 E3E01001 mvn     r1,1h
022FF984 E5900004 ldr     r0,[r0,4h]
022FF988 E5C01000 strb    r1,[r0]
022FF98C E59F004C ldr     r0,=2324D14h
022FF990 E59F2060 ldr     r2,=238A078h
022FF994 E5905004 ldr     r5,[r0,4h]
022FF998 E3A01008 mov     r1,8h
022FF99C E1D541FC ldrsh   r4,[r5,1Ch]
022FF9A0 E2853014 add     r3,r5,14h
022FF9A4 E1C040B2 strh    r4,[r0,2h]
022FF9A8 E4D30001 ldrb    r0,[r3],1h
022FF9AC E2511001 subs    r1,r1,1h
022FF9B0 E4C20001 strb    r0,[r2],1h
022FF9B4 1AFFFFFB bne     22FF9A8h
022FF9B8 E3A00005 mov     r0,5h
022FF9BC E5850004 str     r0,[r5,4h]
022FF9C0 EA000003 b       22FF9D4h
022FF9C4 E3A00006 mov     r0,6h
022FF9C8 E5810004 str     r0,[r1,4h]
022FF9CC E3A00004 mov     r0,4h
022FF9D0 EA000000 b       22FF9D8h
022FF9D4 E3A00001 mov     r0,1h
022FF9D8 E28DDF4A add     r13,r13,128h
022FF9DC E8BD81F0 pop     r4-r8,r15
022FF9E0 02324D14
022FF9E4 000001AF
022FF9E8 023224AC
022FF9EC 022FF9FC
022FF9F0 023224BC
022FF9F4 022FFA14
022FF9F8 0238A078


/*
    fun_022E4FCC 022E4FCC
*/
022E4FCC E92D4038 push    r3-r5,r14
022E4FD0 E24DD090 sub     r13,r13,90h
022E4FD4 E1A05001 mov     r5,r1
022E4FD8 E1A04002 mov     r4,r2
022E4FDC E3500003 cmp     r0,3h
022E4FE0 1A00000B bne     22E5014h
022E4FE4 E28D1004 add     r1,r13,4h
022E4FE8 E1A00003 mov     r0,r3
022E4FEC EBFFFDAA bl      22E469Ch
022E4FF0 E1A00005 mov     r0,r5
022E4FF4 EBF60000 bl      2064FFCh
022E4FF8 E1A02000 mov     r2,r0
022E4FFC E59F1128 ldr     r1,=2319730h
022E5000 E28D000D add     r0,r13,0Dh
022E5004 E28D3004 add     r3,r13,4h
022E5008 EBF6915D bl      2089584h                      //Sprintf?
022E500C E59F011C ldr     r0,=2324C84h
022E5010 EA000041 b       22E511Ch
022E5014 E3500005 cmp     r0,5h
022E5018 1A00000C bne     22E5050h
022E501C E28D1004 add     r1,r13,4h
022E5020 E1A00003 mov     r0,r3
022E5024 EBFFFD9C bl      22E469Ch
022E5028 E1A00005 mov     r0,r5
022E502C EBF5FFF2 bl      2064FFCh
022E5030 E1A02000 mov     r2,r0
022E5034 E59F10F8 ldr     r1,=2319744h
022E5038 E28D000D add     r0,r13,0Dh
022E503C E28D3004 add     r3,r13,4h
022E5040 E58D4000 str     r4,[r13]
022E5044 EBF6914E bl      2089584h                      //Sprintf?
022E5048 E59F00E0 ldr     r0,=2324C84h
022E504C EA000032 b       22E511Ch
022E5050 E3500004 cmp     r0,4h
022E5054 1A000008 bne     22E507Ch
022E5058 E1A00005 mov     r0,r5
022E505C EBF5FFE6 bl      2064FFCh
022E5060 E1A02000 mov     r2,r0
022E5064 E59F10CC ldr     r1,=231975Ch
022E5068 E28D000D add     r0,r13,0Dh
022E506C E1A03004 mov     r3,r4
022E5070 EBF69143 bl      2089584h                      //Sprintf?
022E5074 E59F00B4 ldr     r0,=2324C84h
022E5078 EA000027 b       22E511Ch
022E507C E3500006 cmp     r0,6h
022E5080 1A000008 bne     22E50A8h
022E5084 E28D1004 add     r1,r13,4h
022E5088 E1A00003 mov     r0,r3
022E508C EBFFFD82 bl      22E469Ch
022E5090 E59F10A4 ldr     r1,=2319774h
022E5094 E28D000D add     r0,r13,0Dh
022E5098 E28D2004 add     r2,r13,4h
022E509C EBF69138 bl      2089584h                      //Sprintf?
022E50A0 E59F0088 ldr     r0,=2324C84h
022E50A4 EA00001C b       22E511Ch
022E50A8 E3500002 cmp     r0,2h
022E50AC 1A00000D bne     22E50E8h
022E50B0 E28D1004 add     r1,r13,4h
022E50B4 E1A00003 mov     r0,r3
022E50B8 EBFFFD77 bl      22E469Ch
022E50BC E1A00005 mov     r0,r5
022E50C0 EBF5FFCD bl      2064FFCh
022E50C4 E1A02000 mov     r2,r0
022E50C8 E3A0C000 mov     r12,0h
022E50CC E59F1060 ldr     r1,=2319744h
022E50D0 E28D000D add     r0,r13,0Dh
022E50D4 E28D3004 add     r3,r13,4h
022E50D8 E58DC000 str     r12,[r13]
022E50DC EBF69128 bl      2089584h                      //Sprintf?
022E50E0 E59F0058 ldr     r0,=2324C7Ch
022E50E4 EA00000C b       22E511Ch
022E50E8 E1A00005 mov     r0,r5
022E50EC EBF5FFC2 bl      2064FFCh
022E50F0 E1A02000 mov     r2,r0
022E50F4 E59F103C ldr     r1,=231975Ch
022E50F8 E28D000D add     r0,r13,0Dh
022E50FC E3A03000 mov     r3,0h
022E5100 EBF6911F bl      2089584h                      //Sprintf?
022E5104 E28D000D add     r0,r13,0Dh
022E5108 EBF48EA6 bl      2008BA8h
022E510C E3500000 cmp     r0,0h
022E5110 D3A00000 movle   r0,0h
022E5114 DA000002 ble     22E5124h
022E5118 E59F0024 ldr     r0,=2324C74h
022E511C E28D100D add     r1,r13,0Dh
022E5120 EB00030A bl      22E5D50h
022E5124 E28DD090 add     r13,r13,90h
022E5128 E8BD8038 pop     r3-r5,r15
022E512C 02319730 
022E5130 02324C84 
022E5134 02319744 
022E5138 0231975C 
022E513C 02319774 
022E5140 02324C7C 
022E5144 02324C74 



//======================================================================
//  Annex
//======================================================================
/*
    fun_02089584 ReplaceTokens( , StrToReplaceTokensOf, StrResName )
*/
// R0 = stackptr?, R1 = StrToReplaceTokensOf, R2 = StrResName
02089584 E92D000F push    r0-r3
02089588 E92D4008 push    r3,r14
0208958C E28D100C add     r1,r13,0Ch
02089590 E3C11003 bic     r1,r1,3h      
02089594 E2813004 add     r3,r1,4h
02089598 E59D200C ldr     r2,[r13,0Ch]
0208959C E3E01000 mvn     r1,0h
020895A0 EBFFFFCD bl      20894DCh      ///fun_020894DC
020895A4 E8BD4008 pop     r3,r14
020895A8 E28DD010 add     r13,r13,10h
020895AC E12FFF1E bx      r14


/*
    fun_020894DC
        This seems to call sprintf
*/
//R0, R1, R2 
020894DC E92D4030 push    r4,r5,r14
020894E0 E24DD00C sub     r13,r13,0Ch
020894E4 E1A05000 mov     r5,r0
020894E8 E1A04001 mov     r4,r1
020894EC E3A0C000 mov     r12,0h
020894F0 E59F0048 ldr     r0,=2089498h      //Ptr to a function
020894F4 E28D1000 add     r1,r13,0h
020894F8 E58D5000 str     r5,[r13]          //Stack parameter 0 : PtrResultString
020894FC E58D4004 str     r4,[r13,4h]       //Stack parameter 1 : PtrStrToReplaceTokenIn
02089500 E58DC008 str     r12,[r13,8h]      //Stack parameter 2 : PtrStr
02089504 EBFFFDDA bl      2088C74h          /// fun_02088C74, sprintf, returns the resulting string into R5, and the resulting string's length into R0
02089508 E3550000 cmp     r5,0h             // PtrResultString == 0
if( PtrResultString == 0 )
    0208950C 028DD00C addeq   r13,r13,0Ch
    02089510 08BD8030 popeq   r4,r5,r15
02089514 E1500004 cmp     r0,r4             //Unsigned less than
if( ResultingStrLen < R4 )              
    02089518 33A01000 movcc   r1,0h         
    0208951C 328DD00C addcc   r13,r13,0Ch   
    02089520 37C51000 strccb  r1,[r5,r0]    //Put the terminating 0 after the resulting string!
    02089524 38BD8030 popcc   r4,r5,r15     //Return
02089528 E3540000 cmp     r4,0h
if( R4 != 0 )
    0208952C 10851004 addne   r1,r5,r4
    02089530 13A02000 movne   r2,0h
    02089534 15412001 strneb  r2,[r1,-1h]
02089538 E28DD00C add     r13,r13,0Ch
0208953C E8BD8030 pop     r4,r5,r15
02089540 02089498 


/*
    fun_02088C74
        This seems to be "sprintf"
*/
//R0,R1,R2 = StrToReplaceTokensOf
02088C74 E92D000F push    r0-r3
02088C78 E92D4FF0 push    r4-r11,r14
02088C7C E24DDF8B sub     r13,r13,22Ch
02088C80 E3A03020 mov     r3,20h        
02088C84 E1A0B002 mov     r11,r2        //R11 = StrToReplaceTokensOf
02088C88 E5CD3019 strb    r3,[r13,19h]
02088C8C E1DB20D0 ldrsb   r2,[r11]      //Read first letter of StrToReplaceTokensOf into R2
02088C90 E1A09000 mov     r9,r0         //
02088C94 E1A08001 mov     r8,r1         //
02088C98 E3520000 cmp     r2,0h         // If FirstLetter != 0
02088C9C E3A0A000 mov     r10,0h
if( FirstLetter == 0 )
    02088CA0 0A0001F5 beq     208947Ch  /// LBL1 0208947C
///02088CA4 LBL24
02088CA4 E1A0000B mov     r0,r11        //r0 = StrToReplaceTokensOf
02088CA8 E3A01025 mov     r1,25h        //R1 = '%'
02088CAC EB000330 bl      2089974h      // fun_02089974, FindCharacter, find the character 0x25 ('%') in the StrToReplaceTokensOf string
02088CB0 E58D000C str     r0,[r13,0Ch]  // Store the position of the pointer to the found character on the stack
02088CB4 E3500000 cmp     r0,0h         //If PtrFoundPercent != 0
if( PtrFoundPercent != 0 )
    02088CB8 1A00000E bne     2088CF8h  ///LBL2 02088CF8
02088CBC E1A0000B mov     r0,r11
02088CC0 EB00026C bl      2089678h
02088CC4 E1B02000 movs    r2,r0
02088CC8 E08AA002 add     r10,r10,r2
02088CCC 0A0001EA beq     208947Ch
02088CD0 E1A00008 mov     r0,r8
02088CD4 E1A0100B mov     r1,r11
02088CD8 E12FFF39 blx     r9
02088CDC E3500000 cmp     r0,0h
02088CE0 1A0001E5 bne     208947Ch
02088CE4 E28DDF8B add     r13,r13,22Ch
02088CE8 E3E00000 mvn     r0,0h
02088CEC E8BD4FF0 pop     r4-r11,r14
02088CF0 E28DD010 add     r13,r13,10h
02088CF4 E12FFF1E bx      r14
///LBL2 02088CF8
02088CF8 E050200B subs    r2,r0,r11     //
02088CFC E08AA002 add     r10,r10,r2
02088D00 0A000008 beq     2088D28h
02088D04 E1A00008 mov     r0,r8
02088D08 E1A0100B mov     r1,r11
02088D0C E12FFF39 blx     r9
02088D10 E3500000 cmp     r0,0h
02088D14 028DDF8B addeq   r13,r13,22Ch
02088D18 03E00000 mvneq   r0,0h
02088D1C 08BD4FF0 popeq   r4-r11,r14
02088D20 028DD010 addeq   r13,r13,10h
02088D24 012FFF1E bxeq    r14
02088D28 E59D000C ldr     r0,[r13,0Ch] //
02088D2C E28D1F97 add     r1,r13,25Ch
02088D30 E28D201C add     r2,r13,1Ch
02088D34 EBFFF9BF bl      2087438h      ///fun_02087438
02088D38 E5DD1021 ldrb    r1,[r13,21h]  //Read first character of the token into Token(R1)
02088D3C E1A0B000 mov     r11,r0        // R11 = CharBefToken
02088D40 E3510061 cmp     r1,61h        // Token > 'a'
if( Token > 'a' )
    02088D44 CA000012 bgt     2088D94h  ///02088D94 LBL3
else if( Token >= 'a' )
    02088D48 AA0000EA bge     20890F8h  ///020890F8 LBL4
02088D4C E3510047 cmp     r1,47h
if( Token > 'G' )
    02088D50 CA00000C bgt     2088D88h  ///02088D88 LBL5
02088D54 E2510041 subs    r0,r1,41h
02088D58 508FF100 addpl   r15,r15,r0,lsl 2h
02088D5C EA000006 b       2088D7Ch      ///02088D7C LBL6
02088D60 EA0000E4 b       20890F8h      ///020890F8 LBL4
02088D64 EA00015D b       20892E0h      ///020892E0 LBL7
02088D68 EA00015C b       20892E0h      ///020892E0 LBL7
02088D6C EA00015B b       20892E0h      ///020892E0 LBL7
02088D70 EA0000C6 b       2089090h      ///02089090 LBL8
02088D74 EA0000C5 b       2089090h      ///02089090 LBL8
02088D78 EA0000C4 b       2089090h      ///02089090 LBL8
///02088D7C LBL6
02088D7C E3510025 cmp     r1,25h
if( Token == '%' )
    02088D80 0A000151 beq     20892CCh
02088D84 EA000155 b       20892E0h      ///020892E0 LBL7
///02088D88 LBL5
02088D88 E3510058 cmp     r1,58h
if( Token == 'X' )
    02088D8C 0A000070 beq     2088F54h  ///02088F54 LBL13
02088D90 EA000152 b       20892E0h      ///020892E0 LBL7
///02088D94 LBL3
02088D94 E3510075 cmp     r1,75h
if( Token > 'u' )
    02088D98 CA000017 bgt     2088DFCh      ///02088DFC LBL9
02088D9C E2510064 subs    r0,r1,64h         //R0 = Token - 0x64('d')
02088DA0 508FF100 addpl   r15,r15,r0,lsl 2h 
02088DA4 EA000011 b       2088DF0h          ///02088DF0 LBL10
02088DA8 EA000019 b       2088E14h          ///02088E14 LBL11
02088DAC EA0000B7 b       2089090h          ///02089090 LBL8
02088DB0 EA0000B6 b       2089090h          ///02089090 LBL8
02088DB4 EA0000B5 b       2089090h          ///02089090 LBL8
02088DB8 EA000148 b       20892E0h          ///020892E0 LBL7
02088DBC EA000014 b       2088E14h          ///02088E14 LBL11
02088DC0 EA000146 b       20892E0h          ///020892E0 LBL7
02088DC4 EA000145 b       20892E0h          ///020892E0 LBL7
02088DC8 EA000144 b       20892E0h          ///020892E0 LBL7
02088DCC EA000143 b       20892E0h          ///020892E0 LBL7
02088DD0 EA000112 b       2089220h          ///02089220 LBL12
02088DD4 EA00005E b       2088F54h          ///02088F54 LBL13
02088DD8 EA000140 b       20892E0h          ///020892E0 LBL7
02088DDC EA00013F b       20892E0h          ///020892E0 LBL7
02088DE0 EA00013E b       20892E0h          ///020892E0 LBL7
02088DE4 EA0000DD b       2089160h          ///02089160 LBL14
02088DE8 EA00013C b       20892E0h          ///020892E0 LBL7
02088DEC EA000058 b       2088F54h          ///02088F54 LBL13
///02088DF0 LBL10
02088DF0 E3510063 cmp     r1,63h
02088DF4 0A00012C beq     20892ACh
02088DF8 EA000138 b       20892E0h          ///020892E0 LBL7
///02088DFC LBL9
02088DFC E3510078 cmp     r1,78h
02088E00 CA000001 bgt     2088E0Ch
02088E04 0A000052 beq     2088F54h          ///02088F54 LBL13
02088E08 EA000134 b       20892E0h          ///020892E0 LBL7
02088E0C E35100FF cmp     r1,0FFh
02088E10 EA000132 b       20892E0h          ///020892E0 LBL7
///02088E14 LBL11
02088E14 E5DD0020 ldrb    r0,[r13,20h]
02088E18 E3500003 cmp     r0,3h
02088E1C 1A000004 bne     2088E34h
02088E20 E59D125C ldr     r1,[r13,25Ch]
02088E24 E2811004 add     r1,r1,4h
02088E28 E58D125C str     r1,[r13,25Ch]
02088E2C E5115004 ldr     r5,[r1,-4h]
02088E30 EA000025 b       2088ECCh
02088E34 E3500004 cmp     r0,4h
02088E38 1A000007 bne     2088E5Ch
02088E3C E59D125C ldr     r1,[r13,25Ch]
02088E40 E2812008 add     r2,r1,8h
02088E44 E58D225C str     r2,[r13,25Ch]
02088E48 E5121008 ldr     r1,[r2,-8h]
02088E4C E58D1010 str     r1,[r13,10h]
02088E50 E5121004 ldr     r1,[r2,-4h]
02088E54 E58D1014 str     r1,[r13,14h]
02088E58 EA00001B b       2088ECCh
02088E5C E3500006 cmp     r0,6h
02088E60 1A000007 bne     2088E84h
02088E64 E59D125C ldr     r1,[r13,25Ch]
02088E68 E2812008 add     r2,r1,8h
02088E6C E58D225C str     r2,[r13,25Ch]
02088E70 E5121008 ldr     r1,[r2,-8h]
02088E74 E58D1010 str     r1,[r13,10h]
02088E78 E5121004 ldr     r1,[r2,-4h]
02088E7C E58D1014 str     r1,[r13,14h]
02088E80 EA000011 b       2088ECCh
02088E84 E3500007 cmp     r0,7h
02088E88 1A000004 bne     2088EA0h
02088E8C E59D125C ldr     r1,[r13,25Ch]
02088E90 E2811004 add     r1,r1,4h
02088E94 E58D125C str     r1,[r13,25Ch]
02088E98 E5115004 ldr     r5,[r1,-4h]
02088E9C EA00000A b       2088ECCh
02088EA0 E3500008 cmp     r0,8h
02088EA4 1A000004 bne     2088EBCh
02088EA8 E59D125C ldr     r1,[r13,25Ch]
02088EAC E2811004 add     r1,r1,4h
02088EB0 E58D125C str     r1,[r13,25Ch]
02088EB4 E5115004 ldr     r5,[r1,-4h]
02088EB8 EA000003 b       2088ECCh
02088EBC E59D125C ldr     r1,[r13,25Ch]
02088EC0 E2811004 add     r1,r1,4h
02088EC4 E58D125C str     r1,[r13,25Ch]
02088EC8 E5115004 ldr     r5,[r1,-4h]
02088ECC E3500002 cmp     r0,2h
02088ED0 01A01805 moveq   r1,r5,lsl 10h
02088ED4 01A05841 moveq   r5,r1,asr 10h
02088ED8 E3500001 cmp     r0,1h
02088EDC 01A01C05 moveq   r1,r5,lsl 18h
02088EE0 01A05C41 moveq   r5,r1,asr 18h
02088EE4 E3500004 cmp     r0,4h
02088EE8 13500006 cmpne   r0,6h
02088EEC E28D001C add     r0,r13,1Ch
02088EF0 1A00000A bne     2088F20h
02088EF4 E24D4004 sub     r4,r13,4h
02088EF8 E890000F ldmia   [r0],r0-r3
02088EFC E884000F stmia   [r4],r0-r3
02088F00 E5943000 ldr     r3,[r4]
02088F04 E59D0010 ldr     r0,[r13,10h]
02088F08 E59D1014 ldr     r1,[r13,14h]
02088F0C E28D2F8B add     r2,r13,22Ch
02088F10 EBFFFB3F bl      2087C14h
02088F14 E1B07000 movs    r7,r0
02088F18 0A0000F0 beq     20892E0h          ///020892E0 LBL7
02088F1C EA000008 b       2088F44h
02088F20 E24D4008 sub     r4,r13,8h
02088F24 E890000F ldmia   [r0],r0-r3
02088F28 E884000F stmia   [r4],r0-r3
02088F2C E28D1F8B add     r1,r13,22Ch
02088F30 E1A00005 mov     r0,r5
02088F34 E894000C ldmia   [r4],r2,r3
02088F38 EBFFFAA2 bl      20879C8h
02088F3C E1B07000 movs    r7,r0
02088F40 0A0000E6 beq     20892E0h          ///020892E0 LBL7
02088F44 E28D0C02 add     r0,r13,200h
02088F48 E280002B add     r0,r0,2Bh
02088F4C E0406007 sub     r6,r0,r7
02088F50 EA0000F5 b       208932Ch          ///0208932C LBL17
///02088F54 LBL13
02088F54 E5DD0020 ldrb    r0,[r13,20h]
02088F58 E3500003 cmp     r0,3h
02088F5C 1A000004 bne     2088F74h
02088F60 E59D125C ldr     r1,[r13,25Ch]
02088F64 E2811004 add     r1,r1,4h
02088F68 E58D125C str     r1,[r13,25Ch]
02088F6C E5115004 ldr     r5,[r1,-4h]
02088F70 EA000025 b       208900Ch
02088F74 E3500004 cmp     r0,4h
02088F78 1A000007 bne     2088F9Ch
02088F7C E59D125C ldr     r1,[r13,25Ch]
02088F80 E2812008 add     r2,r1,8h
02088F84 E58D225C str     r2,[r13,25Ch]
02088F88 E5121008 ldr     r1,[r2,-8h]
02088F8C E58D1010 str     r1,[r13,10h]
02088F90 E5121004 ldr     r1,[r2,-4h]
02088F94 E58D1014 str     r1,[r13,14h]
02088F98 EA00001B b       208900Ch
02088F9C E3500006 cmp     r0,6h
02088FA0 1A000007 bne     2088FC4h
02088FA4 E59D125C ldr     r1,[r13,25Ch]
02088FA8 E2812008 add     r2,r1,8h
02088FAC E58D225C str     r2,[r13,25Ch]
02088FB0 E5121008 ldr     r1,[r2,-8h]
02088FB4 E58D1010 str     r1,[r13,10h]
02088FB8 E5121004 ldr     r1,[r2,-4h]
02088FBC E58D1014 str     r1,[r13,14h]
02088FC0 EA000011 b       208900Ch
02088FC4 E3500007 cmp     r0,7h
02088FC8 1A000004 bne     2088FE0h
02088FCC E59D125C ldr     r1,[r13,25Ch]
02088FD0 E2811004 add     r1,r1,4h
02088FD4 E58D125C str     r1,[r13,25Ch]
02088FD8 E5115004 ldr     r5,[r1,-4h]
02088FDC EA00000A b       208900Ch
02088FE0 E3500008 cmp     r0,8h
02088FE4 1A000004 bne     2088FFCh
02088FE8 E59D125C ldr     r1,[r13,25Ch]
02088FEC E2811004 add     r1,r1,4h
02088FF0 E58D125C str     r1,[r13,25Ch]
02088FF4 E5115004 ldr     r5,[r1,-4h]
02088FF8 EA000003 b       208900Ch
02088FFC E59D125C ldr     r1,[r13,25Ch]
02089000 E2811004 add     r1,r1,4h
02089004 E58D125C str     r1,[r13,25Ch]
02089008 E5115004 ldr     r5,[r1,-4h]
0208900C E3500002 cmp     r0,2h
02089010 01A01805 moveq   r1,r5,lsl 10h
02089014 01A05821 moveq   r5,r1,lsr 10h
02089018 E3500001 cmp     r0,1h
0208901C 020550FF andeq   r5,r5,0FFh
02089020 E3500004 cmp     r0,4h
02089024 13500006 cmpne   r0,6h
02089028 E28D001C add     r0,r13,1Ch
0208902C 1A00000A bne     208905Ch
02089030 E24D4004 sub     r4,r13,4h
02089034 E890000F ldmia   [r0],r0-r3
02089038 E884000F stmia   [r4],r0-r3
0208903C E5943000 ldr     r3,[r4]
02089040 E59D0010 ldr     r0,[r13,10h]
02089044 E59D1014 ldr     r1,[r13,14h]
02089048 E28D2F8B add     r2,r13,22Ch
0208904C EBFFFAF0 bl      2087C14h
02089050 E1B07000 movs    r7,r0
02089054 0A0000A1 beq     20892E0h          ///020892E0 LBL7
02089058 EA000008 b       2089080h
0208905C E24D4008 sub     r4,r13,8h
02089060 E890000F ldmia   [r0],r0-r3
02089064 E884000F stmia   [r4],r0-r3
02089068 E28D1F8B add     r1,r13,22Ch
0208906C E1A00005 mov     r0,r5
02089070 E894000C ldmia   [r4],r2,r3
02089074 EBFFFA53 bl      20879C8h
02089078 E1B07000 movs    r7,r0
0208907C 0A000097 beq     20892E0h          ///020892E0 LBL7
02089080 E28D0C02 add     r0,r13,200h
02089084 E280002B add     r0,r0,2Bh
02089088 E0406007 sub     r6,r0,r7
0208908C EA0000A6 b       208932Ch
///02089090 LBL8
02089090 E5DD0020 ldrb    r0,[r13,20h]
02089094 E3500009 cmp     r0,9h
02089098 159D025C ldrne   r0,[r13,25Ch]
0208909C 12800008 addne   r0,r0,8h
020890A0 158D025C strne   r0,[r13,25Ch]
020890A4 1A000002 bne     20890B4h
020890A8 E59D025C ldr     r0,[r13,25Ch]
020890AC E2800008 add     r0,r0,8h
020890B0 E58D025C str     r0,[r13,25Ch]
020890B4 E5107008 ldr     r7,[r0,-8h]
020890B8 E5106004 ldr     r6,[r0,-4h]
020890BC E28D001C add     r0,r13,1Ch
020890C0 E24D4004 sub     r4,r13,4h
020890C4 E890000F ldmia   [r0],r0-r3
020890C8 E884000F stmia   [r4],r0-r3
020890CC E5943000 ldr     r3,[r4]
020890D0 E1A00007 mov     r0,r7
020890D4 E1A01006 mov     r1,r6
020890D8 E28D2F8B add     r2,r13,22Ch
020890DC EBFFFD0B bl      2088510h
020890E0 E1B07000 movs    r7,r0
020890E4 0A00007D beq     20892E0h          ///020892E0 LBL7
020890E8 E28D0C02 add     r0,r13,200h
020890EC E280002B add     r0,r0,2Bh
020890F0 E0406007 sub     r6,r0,r7
020890F4 EA00008C b       208932Ch
///020890F8 LBL4
020890F8 E5DD0020 ldrb    r0,[r13,20h]  //Load character before Token
020890FC E3500009 cmp     r0,9h
02089100 159D025C ldrne   r0,[r13,25Ch]
02089104 12800008 addne   r0,r0,8h
02089108 158D025C strne   r0,[r13,25Ch]
0208910C 1A000002 bne     208911Ch
02089110 E59D025C ldr     r0,[r13,25Ch]
02089114 E2800008 add     r0,r0,8h
02089118 E58D025C str     r0,[r13,25Ch]
0208911C E5107008 ldr     r7,[r0,-8h]
02089120 E5106004 ldr     r6,[r0,-4h]
02089124 E28D001C add     r0,r13,1Ch
02089128 E24D4004 sub     r4,r13,4h
0208912C E890000F ldmia   [r0],r0-r3
02089130 E884000F stmia   [r4],r0-r3
02089134 E5943000 ldr     r3,[r4]
02089138 E1A00007 mov     r0,r7
0208913C E1A01006 mov     r1,r6
02089140 E28D2F8B add     r2,r13,22Ch
02089144 EBFFFB71 bl      2087F10h
02089148 E1B07000 movs    r7,r0
0208914C 0A000063 beq     20892E0h          ///020892E0 LBL7
02089150 E28D0C02 add     r0,r13,200h
02089154 E280002B add     r0,r0,2Bh
02089158 E0406007 sub     r6,r0,r7
0208915C EA000072 b       208932Ch          ///0208932C LBL18
///02089160 LBL14
02089160 E5DD0020 ldrb    r0,[r13,20h]      //Read CharBefToken
02089164 E3500005 cmp     r0,5h
if( CharBefToken != 5 )
    02089168 1A00000C bne     20891A0h      ///020891A0 LBL14
0208916C E59D025C ldr     r0,[r13,25Ch]
02089170 E3A02C02 mov     r2,200h
02089174 E2800004 add     r0,r0,4h
02089178 E58D025C str     r0,[r13,25Ch]
0208917C E5101004 ldr     r1,[r0,-4h]
02089180 E28D002C add     r0,r13,2Ch
02089184 E3510000 cmp     r1,0h
02089188 059F1300 ldreq   r1,=20B321Ch
0208918C EBFFF824 bl      2087224h
02089190 E3500000 cmp     r0,0h
02089194 BA000051 blt     20892E0h          ///020892E0 LBL7
02089198 E28D702C add     r7,r13,2Ch
0208919C EA000003 b       20891B0h
///020891A0 LBL14                           //Here [r13,25Ch] -8, -4, 0, +4 pointed to pointers to "ssa", "%s:%s", "rom0", "SCRIPT/S13P02A/enter.sse" 
020891A0 E59D025C ldr     r0,[r13,25Ch]     //R0 = Pointer on stack to a string pointer that points to the string to use to replace the token.
020891A4 E2800004 add     r0,r0,4h          //R0 = Now Points to "SCRIPT/S13P02A/enter.sse" 
020891A8 E58D025C str     r0,[r13,25Ch]     //Put it into the pointer on the stack directly
020891AC E5107004 ldr     r7,[r0,-4h]       // R7 = Pointer to "rom0"
020891B0 E5DD001F ldrb    r0,[r13,1Fh]      // R0 = 
020891B4 E3570000 cmp     r7,0h
if( R7 == 0 )
    020891B8 059F72D4 ldreq   r7,=20B3220h  //R7 = 
020891BC E3500000 cmp     r0,0h
if( R0 == 0 )
    020891C0 0A000007 beq     20891E4h      ///020891E4 LBL15
020891C4 E5DD001E ldrb    r0,[r13,1Eh]
020891C8 E4D76001 ldrb    r6,[r7],1h
020891CC E3500000 cmp     r0,0h
020891D0 0A000055 beq     208932Ch          ///0208932C LBL18
020891D4 E59D0028 ldr     r0,[r13,28h]
020891D8 E1560000 cmp     r6,r0
020891DC C1A06000 movgt   r6,r0
020891E0 EA000051 b       208932Ch          ///0208932C LBL18
///020891E4 LBL15
020891E4 E5DD001E ldrb    r0,[r13,1Eh]
020891E8 E3500000 cmp     r0,0h
if( R0 == 0)
    020891EC 0A000007 beq     2089210h      ///02089210 LBL16
020891F0 E59D6028 ldr     r6,[r13,28h]
020891F4 E1A00007 mov     r0,r7
020891F8 E1A02006 mov     r2,r6
020891FC E3A01000 mov     r1,0h
02089200 EBFFF845 bl      208731Ch
02089204 E3500000 cmp     r0,0h
02089208 10406007 subne   r6,r0,r7
0208920C EA000046 b       208932Ch          ///0208932C LBL18
///02089210 LBL16
02089210 E1A00007 mov     r0,r7             //R0 = FirstString ("rom0")
02089214 EB000117 bl      2089678h          ///fun_02089678, returns length of string
02089218 E1A06000 mov     r6,r0             //R6 = FirstStringLen
0208921C EA000042 b       208932Ch          ///0208932C LBL18
///02089220 LBL12
02089220 E59D125C ldr     r1,[r13,25Ch]
02089224 E5DD0020 ldrb    r0,[r13,20h]
02089228 E2811004 add     r1,r1,4h
0208922C E58D125C str     r1,[r13,25Ch]
02089230 E5111004 ldr     r1,[r1,-4h]
02089234 E3500008 cmp     r0,8h
02089238 908FF100 addls   r15,r15,r0,lsl 2h
0208923C EA00008B b       2089470h
02089240 EA000007 b       2089264h
02089244 EA000089 b       2089470h
02089248 EA000007 b       208926Ch
0208924C EA000008 b       2089274h
02089250 EA000011 b       208929Ch
02089254 EA000085 b       2089470h
02089258 EA000007 b       208927Ch
0208925C EA00000A b       208928Ch
02089260 EA00000B b       2089294h
02089264 E581A000 str     r10,[r1]
02089268 EA000080 b       2089470h
0208926C E1C1A0B0 strh    r10,[r1]
02089270 EA00007E b       2089470h
02089274 E581A000 str     r10,[r1]
02089278 EA00007C b       2089470h
0208927C E581A000 str     r10,[r1]
02089280 E1A00FCA mov     r0,r10,asr 1Fh
02089284 E5810004 str     r0,[r1,4h]
02089288 EA000078 b       2089470h
0208928C E581A000 str     r10,[r1]
02089290 EA000076 b       2089470h
02089294 E581A000 str     r10,[r1]
02089298 EA000074 b       2089470h
0208929C E581A000 str     r10,[r1]
020892A0 E1A00FCA mov     r0,r10,asr 1Fh
020892A4 E5810004 str     r0,[r1,4h]
020892A8 EA000070 b       2089470h
020892AC E59D025C ldr     r0,[r13,25Ch]
020892B0 E28D702C add     r7,r13,2Ch
020892B4 E2800004 add     r0,r0,4h
020892B8 E58D025C str     r0,[r13,25Ch]
020892BC E5100004 ldr     r0,[r0,-4h]
020892C0 E3A06001 mov     r6,1h
020892C4 E5CD002C strb    r0,[r13,2Ch]
020892C8 EA000017 b       208932Ch          ///0208932C LBL18
020892CC E3A00025 mov     r0,25h
020892D0 E5CD002C strb    r0,[r13,2Ch]
020892D4 E28D702C add     r7,r13,2Ch
020892D8 E3A06001 mov     r6,1h
020892DC EA000012 b       208932Ch          ///0208932C LBL18
///020892E0 LBL7
020892E0 E59D000C ldr     r0,[r13,0Ch]
020892E4 EB0000E3 bl      2089678h
020892E8 E1B04000 movs    r4,r0
020892EC 0A000009 beq     2089318h
020892F0 E59D100C ldr     r1,[r13,0Ch]
020892F4 E1A00008 mov     r0,r8
020892F8 E1A02004 mov     r2,r4
020892FC E12FFF39 blx     r9
02089300 E3500000 cmp     r0,0h
02089304 028DDF8B addeq   r13,r13,22Ch
02089308 03E00000 mvneq   r0,0h
0208930C 08BD4FF0 popeq   r4-r11,r14
02089310 028DD010 addeq   r13,r13,10h
02089314 012FFF1E bxeq    r14
02089318 E28DDF8B add     r13,r13,22Ch
0208931C E08A0004 add     r0,r10,r4
02089320 E8BD4FF0 pop     r4-r11,r14
02089324 E28DD010 add     r13,r13,10h
02089328 E12FFF1E bx      r14
///0208932C LBL17
0208932C E5DD001C ldrb    r0,[r13,1Ch]      //
02089330 E1A04006 mov     r4,r6
02089334 E3500000 cmp     r0,0h
02089338 0A000028 beq     20893E0h          ///020893E0 LBL21
0208933C E3500002 cmp     r0,2h
02089340 03A00030 moveq   r0,30h
02089344 13A00020 movne   r0,20h
02089348 E5CD0019 strb    r0,[r13,19h]      //
0208934C E1D700D0 ldrsb   r0,[r7]           //Read first letter for "FirstString"
02089350 E350002B cmp     r0,2Bh            //
if( FLetter != '+' )
    02089354 1350002D cmpne   r0,2Dh
    if( FLetter != '-' )
        02089358 13500020 cmpne   r0,20h
        if( FLetter != ' ' )
            0208935C 1A00000E bne     208939Ch  ///0208939C LBL20
02089360 E1DD01D9 ldrsb   r0,[r13,19h]
02089364 E3500030 cmp     r0,30h
if( R0 != '0' )
    02089368 1A00000B bne     208939Ch          ///0208939C LBL20
0208936C E1A00008 mov     r0,r8
02089370 E1A01007 mov     r1,r7
02089374 E3A02001 mov     r2,1h
02089378 E12FFF39 blx     r9
0208937C E3500000 cmp     r0,0h
02089380 028DDF8B addeq   r13,r13,22Ch
02089384 03E00000 mvneq   r0,0h
02089388 08BD4FF0 popeq   r4-r11,r14
0208938C 028DD010 addeq   r13,r13,10h
02089390 012FFF1E bxeq    r14
02089394 E2877001 add     r7,r7,1h
02089398 E2466001 sub     r6,r6,1h
///0208939C LBL20
0208939C E59D0024 ldr     r0,[r13,24h]
020893A0 E1540000 cmp     r4,r0
if( R4 >= R0 )
    020893A4 AA00000D bge     20893E0h      ///020893E0 LBL21
020893A8 E1A00008 mov     r0,r8
020893AC E28D1019 add     r1,r13,19h
020893B0 E3A02001 mov     r2,1h
020893B4 E12FFF39 blx     r9
020893B8 E3500000 cmp     r0,0h
020893BC 028DDF8B addeq   r13,r13,22Ch
020893C0 03E00000 mvneq   r0,0h
020893C4 08BD4FF0 popeq   r4-r11,r14
020893C8 028DD010 addeq   r13,r13,10h
020893CC 012FFF1E bxeq    r14
020893D0 E59D0024 ldr     r0,[r13,24h]
020893D4 E2844001 add     r4,r4,1h
020893D8 E1540000 cmp     r4,r0
020893DC BAFFFFF1 blt     20893A8h
///020893E0 LBL21
020893E0 E3560000 cmp     r6,0h         //FirstStringLen != 0
if( FirstStringLen == 0 )
    020893E4 0A000009 beq     2089410h      ///02089410 LBL22
020893E8 E1A00008 mov     r0,r8             //R0 = Ptr to PtrTargetString
020893EC E1A01007 mov     r1,r7             //R1 = FirstString
020893F0 E1A02006 mov     r2,r6             //R2 = FirstStringLen
020893F4 E12FFF39 blx     r9                //Run that function (was 0x2089498) This replace the string tokens with the strings.
020893F8 E3500000 cmp     r0,0h             //
if( R0 == 0 )
    020893FC 028DDF8B addeq   r13,r13,22Ch
    02089400 03E00000 mvneq   r0,0h
    02089404 08BD4FF0 popeq   r4-r11,r14
    02089408 028DD010 addeq   r13,r13,10h
    0208940C 012FFF1E bxeq    r14
///02089410 LBL22
02089410 E5DD001C ldrb    r0,[r13,1Ch]      //
02089414 E3500000 cmp     r0,0h             //
if(R0 != 0)
    02089418 1A000013 bne     208946Ch      ///0208946C LBL23
0208941C E59D0024 ldr     r0,[r13,24h]      //    
02089420 E1540000 cmp     r4,r0
02089424 AA000010 bge     208946Ch
02089428 E3A06020 mov     r6,20h
0208942C E28D7018 add     r7,r13,18h
02089430 E1A00008 mov     r0,r8
02089434 E1A01007 mov     r1,r7
02089438 E3A02001 mov     r2,1h
0208943C E5CD6018 strb    r6,[r13,18h]
02089440 E12FFF39 blx     r9
02089444 E3500000 cmp     r0,0h
02089448 028DDF8B addeq   r13,r13,22Ch
0208944C 03E00000 mvneq   r0,0h
02089450 08BD4FF0 popeq   r4-r11,r14
02089454 028DD010 addeq   r13,r13,10h
02089458 012FFF1E bxeq    r14
0208945C E59D0024 ldr     r0,[r13,24h]
02089460 E2844001 add     r4,r4,1h
02089464 E1540000 cmp     r4,r0
02089468 BAFFFFF0 blt     2089430h
///0208946C LBL23
0208946C E08AA004 add     r10,r10,r4        //
02089470 E1DB00D0 ldrsb   r0,[r11]          //Load the next character in StrToReplaceTokensOf ( was the ':' in "%s:%s")
02089474 E3500000 cmp     r0,0h             //
if( NextChar != 0 )
    02089478 1AFFFE09 bne     2088CA4h      ///02088CA4 LBL24
///LBL1 0208947C
0208947C E1A0000A mov     r0,r10
02089480 E28DDF8B add     r13,r13,22Ch
02089484 E8BD4FF0 pop     r4-r11,r14
02089488 E28DD010 add     r13,r13,10h
0208948C E12FFF1E bx      r14
02089490 020B321C 
02089494 020B3220 


/*
    fun_02089974 FindCharacter
*/
//R0 = StrToReplaceTokensOf, R1 = CharToFind(0x25 == %), R2 = FirstLetter
02089974 E0D020D1 ldrsb   r2,[r0],1h        //Read the current letter at the "StrToReplaceTokensOf" pointer address into "CurLetter". And increment the string pointer
02089978 E1A01C01 mov     r1,r1,lsl 18h     //Those two lines are to convert a signed byte, to a signed int32
0208997C E1A01C41 mov     r1,r1,asr 18h     //    
02089980 E3520000 cmp     r2,0h
if( CurLetter == 0 )
    02089984 0A000005 beq     20899A0h      ///LBL1 020899A0

///LOOP_BEG1
02089988 E1520001 cmp     r2,r1
if( CurLetter == CharToFind )
    0208998C 02400001 subeq   r0,r0,1h
    02089990 012FFF1E bxeq    r14
02089994 E0D020D1 ldrsb   r2,[r0],1h        // CurLetter = *StrToReplaceTokensOf, StrToReplaceTokensOf += 1 
02089998 E3520000 cmp     r2,0h
if( CurLetter != 0 )
    0208999C 1AFFFFF9 bne     2089988h      ///LOOP_BEG1 02089988

///LBL1 020899A0
020899A0 E3510000 cmp     r1,0h
if( CharToFind != 0 )
    020899A4 13A00000 movne   r0,0h
else
    020899A8 02400001 subeq   r0,r0,1h
020899AC E12FFF1E bx      r14

/*
    fun_02089678 strlen
        Counts the length of a null terminated C-string.
*/
// R0 = StringPtr
02089678 E3E02000 mvn     r2,0h             //StrLength = 0
///0208967C LOOP_BEG1
0208967C E0D010D1 ldrsb   r1,[r0],1h        //CurChar = StringPtr, StringPtr += 1
02089680 E2822001 add     r2,r2,1h          //StrLength += 1    
02089684 E3510000 cmp     r1,0h             //If CurChar != 0
if( CurChar != 0 )
    02089688 1AFFFFFB bne     208967Ch      ///0208967C LOOP_BEG1
0208968C E1A00002 mov     r0,r2             //Return StrLength
02089690 E12FFF1E bx      r14               //Return


/*
    fun_02007124
        Do something with name
*/
//R0, R1, R2, R3
02007124 E92D47F0 push    r4-r10,r14
02007128 E24DDC01 sub     r13,r13,100h
0200712C E1A04001 mov     r4,r1
02007130 E1A0A000 mov     r10,r0
02007134 E1A00004 mov     r0,r4
02007138 E3A0103A mov     r1,3Ah
0200713C EBFFEC89 bl      2002368h      ///fun_02002368 
02007140 E3500000 cmp     r0,0h
02007144 E28D0080 add     r0,r13,80h
02007148 1A000004 bne     2007160h
0200714C E59F10F8 ldr     r1,=20927E4h
02007150 E59F20F8 ldr     r2,=20AF2B8h
02007154 E1A03004 mov     r3,r4
02007158 EB020909 bl      2089584h      ///fun_02089584, Calls sprintf
0200715C EA000001 b       2007168h      ///02007168 LBL1
02007160 E1A01004 mov     r1,r4
02007164 EB02094A bl      2089694h
///02007168 LBL1 ( This part seems like it might be breaking down the components of the name )
02007168 E28D0080 add     r0,r13,80h
0200716C E3A0103A mov     r1,3Ah
02007170 EBFFEC7C bl      2002368h      ///fun_02002368 
02007174 E28D0080 add     r0,r13,80h
02007178 EB0000F1 bl      2007544h      ///02007544
0200717C E28D2080 add     r2,r13,80h
02007180 E28A000C add     r0,r10,0Ch
02007184 E28A1004 add     r1,r10,4h
02007188 EBFFFFC3 bl      200709Ch      ///0200709C
0200718C E1A08000 mov     r8,r0
02007190 E28A000C add     r0,r10,0Ch
02007194 EB0000B4 bl      200746Ch
02007198 E5900000 ldr     r0,[r0]
0200719C E1A01008 mov     r1,r8
020071A0 E58A0000 str     r0,[r10]
020071A4 E28A006C add     r0,r10,6Ch    
020071A8 EB020939 bl      2089694h      ///fun_02089694, copies entire file string into the stack 
020071AC E3A09000 mov     r9,0h         //Set Directory level to 0
020071B0 E5CA905C strb    r9,[r10,5Ch]
020071B4 E28A401C add     r4,r10,1Ch
020071B8 E1A06009 mov     r6,r9
020071BC E28D7000 add     r7,r13,0h
020071C0 E3A0500A mov     r5,0Ah
///020071C4 LBL32
020071C4 E1A01007 mov     r1,r7
///020071C8 LOOP_BEG1
020071C8 E1D800D0 ldrsb   r0,[r8]       //load char at R8 string pointer ("rom0:SCRIPT/S13P02A/enter.sse") into CurChar
020071CC E3500000 cmp     r0,0h         
if( CurChar != 0 )
    020071D0 1350002F cmpne   r0,2Fh
    if( CurChar != '/' )
        020071D4 1350002E cmpne   r0,2Eh
        if(CurChar != '.' )
            020071D8 12888001 addne   r8,r8,1h      // Increment string pointer
            020071DC 14C10001 strneb  r0,[r1],1h    //Copy part of the string over to somewhere else, and incremment that place's pointer
            020071E0 1AFFFFF8 bne     20071C8h      ///020071C8 LOOP_BEG1
020071E4 E5C16000 strb    r6,[r1]       //Append ending 0 after copying the part of the string before a '/' or '.'
020071E8 E1D800D0 ldrsb   r0,[r8]       //Load the '.', '/', or 0 that stopped the loop.
020071EC E3500000 cmp     r0,0h         
if( CurChar != 0 )
    020071F0 1A000003 bne     2007204h      ///02007204 LBL25
020071F4 E28D1000 add     r1,r13,0h
020071F8 E28A005C add     r0,r10,5Ch
020071FC EB02096A bl      20897ACh
02007200 EA00000E b       2007240h          ///02007240 LBL26
///02007204 LBL25
02007204 E350002E cmp     r0,2Eh
if( CurChar != '.' )
    02007208 1A000004 bne     2007220h      ///02007220 LBL27
0200720C E1A01007 mov     r1,r7
02007210 E1A02005 mov     r2,r5
02007214 E28A005C add     r0,r10,5Ch
02007218 EBFFEC62 bl      20023A8h
0200721C EA000005 b       2007238h          ///02007238 LBL28
///02007220 LBL27
02007220 E350002F cmp     r0,2Fh
if( CurChar != '/' )
    02007224 1A000003 bne     2007238h      ///02007238 LBL28
02007228 E0840209 add     r0,r4,r9,lsl 4h   //R0 = TargetStringOnStack?
0200722C E1A01007 mov     r1,r7             //R1 = StringLastDirectoryLvl?
02007230 E2899001 add     r9,r9,1h          //R9 = FileTreeLevel? (It got incremented whenever we pass a '/')
02007234 EB020916 bl      2089694h          ///fun_02089694
///02007238 LBL28
02007238 E2888001 add     r8,r8,1h          //Increment the read pos in the string pointer
0200723C EAFFFFE0 b       20071C4h          ///020071C4 LBL32
///02007240 LBL26
02007240 E58A9008 str     r9,[r10,8h]
02007244 E28DDC01 add     r13,r13,100h
02007248 E8BD87F0 pop     r4-r10,r15
0200724C 020927E4 
02007250 020AF2B8 


/*
    fun_02089694
*/
//R0 = TargetStringOnStack?,R1 = StringLastDirectoryLvl?,R2,R3,R4
02089694 E92D4038 push    r3-r5,r14
02089698 E2014003 and     r4,r1,3h
0208969C E2003003 and     r3,r0,3h
020896A0 E1A02000 mov     r2,r0
020896A4 E1530004 cmp     r3,r4
if( R3 != R4 )
    020896A8 1A000020 bne     2089730h      ///02089730 LBL29
020896AC E3540000 cmp     r4,0h
if( R4 == 0 )
    020896B0 0A00000D beq     20896ECh      ///020896EC LBL30
020896B4 E5D13000 ldrb    r3,[r1]
020896B8 E5C03000 strb    r3,[r0]
020896BC E3530000 cmp     r3,0h
020896C0 08BD8038 popeq   r3-r5,r15
020896C4 E2744003 rsbs    r4,r4,3h
020896C8 0A000005 beq     20896E4h
020896CC E5F13001 ldrb    r3,[r1,1h]!
020896D0 E3530000 cmp     r3,0h
020896D4 E5E23001 strb    r3,[r2,1h]!
020896D8 08BD8038 popeq   r3-r5,r15
020896DC E2544001 subs    r4,r4,1h
020896E0 1AFFFFF9 bne     20896CCh
020896E4 E2822001 add     r2,r2,1h
020896E8 E2811001 add     r1,r1,1h
///020896EC LBL30
020896EC E5915000 ldr     r5,[r1]           //Load 4 characters from the StringLastDirectoryLvl
020896F0 E59F305C ldr     r3,=0FEFEFEFFh
020896F4 E1E04005 mvn     r4,r5             //R4 = ~R5 (Gave 0xB6ADBCAC for "SCRI")
020896F8 E085E003 add     r14,r5,r3         //R14 = R5 + 0FEFEFEFF  (0xB6ADBCAC was in R5)
020896FC E59FC054 ldr     r12,=80808080h    
02089700 E00E4004 and     r4,r14,r4         //R4 = R14 & R4 (Was 0xB6ADBCAC & 0x48514252 == 0x00010000)
02089704 E114000C tst     r4,r12            //R4 & R12 (Was 0x00010000 & 0x80808080 )
if( R4 & R12 != 0 )
    02089708 1A000008 bne     2089730h      ///02089730 LBL29
0208970C E2422004 sub     r2,r2,4h          //Move write pointer back 4 bytes
///02089710 LOOP_BEG3
02089710 E5A25004 str     r5,[r2,4h]!       //Write content of R5 and increment (R5 was "SCRI")
02089714 E5B15004 ldr     r5,[r1,4h]!       //Load the next 4 bytes of StringLastDirectoryLvl into R5
02089718 E0854003 add     r4,r5,r3          //Add the rest of the string to 0x0FEFEFEFF
0208971C E1E0E005 mvn     r14,r5            //R14 = ~R5
02089720 E004E00E and     r14,r4,r14        //R14 = R4 & R14
02089724 E11E000C tst     r14,r12           //
if( R14 & R12 == 0 )
    02089728 0AFFFFF8 beq     2089710h      ///02089710 LOOP_BEG3
0208972C E2822004 add     r2,r2,4h          //Increment write pointer by 4
///02089730 LBL29
02089730 E5D13000 ldrb    r3,[r1]           //Load the next character from StringLastDirectoryLvl
02089734 E5C23000 strb    r3,[r2]           //Put it at the write pointer
02089738 E3530000 cmp     r3,0h
if( LastChar == 0 )
    0208973C 08BD8038 popeq   r3-r5,r15
///02089740 LOOP_BEG2
02089740 E5F13001 ldrb    r3,[r1,1h]!       // Read character and increment read position from StringLastDirectoryLvl
02089744 E3530000 cmp     r3,0h             //
02089748 E5E23001 strb    r3,[r2,1h]!       //Put it at the write pointer and increment
if( LastChar != 0 )
    0208974C 1AFFFFFB bne     2089740h      ///02089740 LOOP_BEG2
02089750 E8BD8038 pop     r3-r5,r15
02089754 FEFEFEFF
02089758 80808080


/*
    fun_02008BA8
*/
02008BA8 E92D4008 push    r3,r14
02008BAC E24DD040 sub     r13,r13,40h
02008BB0 E1A02000 mov     r2,r0
02008BB4 E28D0000 add     r0,r13,0h
02008BB8 E28D1028 add     r1,r13,28h
02008BBC EB000029 bl      2008C68h      ///
02008BC0 E3500000 cmp     r0,0h
02008BC4 159D0034 ldrne   r0,[r13,34h]
02008BC8 03E00000 mvneq   r0,0h
02008BCC E28DD040 add     r13,r13,40h
02008BD0 E8BD8008 pop     r3,r15


/*
*/

