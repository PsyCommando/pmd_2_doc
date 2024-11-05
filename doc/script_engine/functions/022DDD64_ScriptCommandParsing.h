//
//  Script engine struct
//
[R4]        = Pointer to ScriptEngineStruct
[R4 + 0x0C] = ?
[R4 + 0x0E] = Last return value?
[R4 + 0x14] = Pointer to SSB file in memory. Points to group table.
[R4 + 0x18] = Pointer to section of ssb file? Possibly beginning of opcodes.
[R4 + 0x1C] = AddressNextOpCode

[R4 + 0x20] = Pointer to section of ssb file? Possibly constant/string table. Points to after end of file if none is there!

[R4 + 0x34]  = Possibly AddressCurrentOpCode
[R4 + 0x38]  = Possibly next op code or paramter for current op code!
[R4 + 0x3C]  = NbParams current command

[r4 + 0x46]  = Unknown (ProcessSpecial places a 0 there!)
[R4 + 0x48]  = ScriptEngineStruct placed there when the last opcode processed returns 2

[R4 + 0x6C]  = 

[R4 + 0xF0]  = Unknown


22AB4FCh = Struct containing values used by main_EnterDungeon
2324FA0h = Struct used with second parameter of main_EnterDungeon
[2324FA0h, 1Ch] = 

//TEMP:
02318F50 entry working on Contains 02316D00, points to "sound_Stop"


//R4 struct at runtime:
0x0     : 02321414
0x14    : 0212F380
0x18    : 02132316
0x1C    : 02132316
0x20    : 0213A8EC

//==========================================================
//Script Command Parsing
//==========================================================
//R0 is the pointer to a struct containing pointers to various things of interests related to the script engine!

022DDD64 E92D4FF8 push    r3-r11,r14
022DDD68 E24DDFB2 sub     r13,r13,2C8h
022DDD6C E1A04000 mov     r4,r0             //R4 = Script engine struct ptr
022DDD70 E594101C ldr     r1,[r4,1Ch]       // [R4 + 0x1C] contains the pointer to the current op-code in the script, possibly?
022DDD74 E59F0F8C ldr     r0,=2318610h      // 2318610 is the address of the table containing the string pointers and info for each script commands.
022DDD78 E2816002 add     r6,r1,2h          //This moves the op-code pointer to the next 16 bits opcode
022DDD7C E5841034 str     r1,[r4,34h]       //Write Pointer to current opcode at [R4 + 0x34]
022DDD80 E5846038 str     r6,[r4,38h]       //Write Pointer to parameter? At [R4 + 0x38]
022DDD84 E1D150B0 ldrh    r5,[r1]           //Load the opcode into R5
022DDD88 E1A01185 mov     r1,r5,lsl 3h      //This multiplies the value by 8 and put it in R1. Probably to get an offset on 8 bytes for the next instruction.
022DDD8C E19000D1 ldrsb   r0,[r0,r1]        //Load NbParams from the script command table matching the value! 
022DDD90 E3500000 cmp     r0,0h             //Check if its 0
if( NbParams >= 0 )
{
    022DDD94 A1C403BC strgeh  r0,[r4,3Ch]                 //NbParams current command //If greater or equal to 0, store the NbParams at [R4 + 0x3C]    
    022DDD98 A594101C ldrge   r1,[r4,1Ch]       //Load AddressCurrentOpCode into R1.
    022DDD9C A2800001 addge   r0,r0,1h          // Add one to NbParams
    022DDDA0 A0810080 addge   r0,r1,r0,lsl 1h   // R0 = AddressCurrentOpCode + (NbParams * 2) <= I think this is the offset in byte past the last parameter of the script opcode!
    022DDDA4 AA000008 bge     22DDDCCh          /// GOTO 022DDDCC LBL1
}
else                                            //When NbParams is -1
{
    022DDDA8 E1D600B0 ldrh    r0,[r6]               //Load next word into R0
    022DDDAC EB001ABE bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
    {//022E48AC
        022E48AC E3100901 tst     r0,4000h      //(R0 & 0x4000)
        if( R0 & 0x4000 != 0 )                  //If the bit at 0x4000 is on
            022E48B0 13A01902 movne   r1,8000h  //R1 = 0x8000    
            022E48B4 12611000 rsbne   r1,r1,0h  //R1 = 0 - 0x8000 = 0xFFFF8000
            022E48B8 11801001 orrne   r1,r0,r1  //R1 = R0 | 0xFFFF8000              ( Ex: 0x7FE8 gives 0xFFFF FFE8 )
        else
            022E48BC 059F1018 ldreq   r1,=3FFFh //R1 = 0x3FFF
            022E48C0 00001001 andeq   r1,r0,r1  //R1 = R0 & 0x3FFF
        022E48C4 E3100902 tst     r0,8000h
        if( R0 & 0x8000 != 0 )                          //If the bit at 0x8000 is on
            022E48C8 11A003C1 movne   r0,r1,asr 7h      //R0 = R1 >> 7 (signed shift)         ( Ex: FFFF FFE8 >> 7 = FFFF FFFF )
            022E48CC 10810C20 addne   r0,r1,r0,lsr 18h  //R0 = R1 + R0 >> 0x18                ( Ex: FFFF FFE8 + FFFF FFFF >> 0x18 == FFFF FFE8 + FF == E7 )
            022E48D0 11A00440 movne   r0,r0,asr 8h      //R0 = R0 >> 8 (signed shift)         ( Ex: E7 >> 8 == 0 )
        else
            022E48D4 01A00001 moveq   r0,r1     //R0 = R1
    }
    022DDDB0 E5942038 ldr     r2,[r4,38h]           //Get pointer to next word
    022DDDB4 E2801002 add     r1,r0,2h              //R1 = NextWord + 2
    022DDDB8 E2822002 add     r2,r2,2h              //R2 = PtrNextWord + 2
    022DDDBC E5842038 str     r2,[r4,38h]           //Set pointer next word to R2
    022DDDC0 E1C403BC strh    r0,[r4,3Ch]                 //NbParams current command           //NbParams = NextWord
    022DDDC4 E594001C ldr     r0,[r4,1Ch]           //R0 = AddressCurrentOpCode
    022DDDC8 E0800081 add     r0,r0,r1,lsl 1h       //R0 = AddressCurrentOpCode + ((NextWord+2) * 2)
}
/// 022DDDCC LBL1
022DDDCC E584001C str     r0,[r4,1Ch]           //Set the address of the next opcode to parse AddressNextOpCode
022DDDD0 E59F0F34 ldr     r0,=172h              //R0 = 0x172
022DDDD4 E1550000 cmp     r5,r0                 //Check if the Opcode is less than 0x172
if( Opcode < 0x172 )
    022DDDD8 908FF105 addls   r15,r15,r5,lsl 2h //R15 = 022DDDDC + (Opcode * 4) + 4  (+4 because PC is incremented after instruction is ran)
else
	022DDDDC EA0009BE b       22E04DCh			//(Opcode > 0x172) 022E04DC

022DDDE0 EA0009BD b       22E04DCh              //0x00 Null
022DDDE4 EA000219 b       22DE650h              //0x01 back_ChangeGround 022DE650
022DDDE8 EA00025D b       22DE764h              //0x02
022DDDEC EA000237 b       22DE6D0h              //0x03
022DDDF0 EA000236 b       22DE6D0h              //0x04 back_SetBackScrollSpeed 022DE6D0
022DDDF4 EA00079A b       22DFC64h              //0x05
022DDDF8 EA000799 b       22DFC64h              //0x06
022DDDFC EA000276 b       22DE7DCh              //0x07
022DDE00 EA000797 b       22DFC64h              //0x08
022DDE04 EA0001BD b       22DE500h              //0x09 back_setground    022DE500
022DDE08 EA000795 b       22DFC64h              //0x0A
022DDE0C EA000794 b       22DFC64h              //0x0B
022DDE10 EA000793 b       22DFC64h              //0x0C
022DDE14 EA000792 b       22DFC64h              //0x0D
022DDE18 EA000223 b       22DE6ACh              //0x0E
022DDE1C EA000269 b       22DE7C8h              //0x0F
022DDE20 EA0009AD b       22E04DCh              //0x10
022DDE24 EA0009AC b       22E04DCh              //0x11
022DDE28 EA000260 b       22DE7B0h              //0x12
022DDE2C EA000227 b       22DE6D0h              //0x13
022DDE30 EA000226 b       22DE6D0h              //0x14
022DDE34 EA0009A8 b       22E04DCh              //0x15
022DDE38 EA000272 b       22DE808h              //0x16
022DDE3C EA0001E5 b       22DE5D8h              //0x17
022DDE40 EA0001DD b       22DE5BCh              //0x18 back2_SetMode 022DE5BC
022DDE44 EA0009A4 b       22E04DCh              //0x19
022DDE48 EA000219 b       22DE6B4h              //0x1A
022DDE4C EA0009A2 b       22E04DCh              //0x1B
022DDE50 EA0009A1 b       22E04DCh              //0x1C
022DDE54 EA0009A0 b       22E04DCh              //0x1D
022DDE58 EA00099F b       22E04DCh              //0x1E
022DDE5C EA00099E b       22E04DCh              //0x1F
022DDE60 EA00099D b       22E04DCh              //0x20
022DDE64 EA00099C b       22E04DCh              //0x21
022DDE68 EA00099B b       22E04DCh              //0x22
022DDE6C EA00099A b       22E04DCh              //0x23
022DDE70 EA000999 b       22E04DCh              //0x24
022DDE74 EA000998 b       22E04DCh              //0x25
022DDE78 EA000997 b       22E04DCh              //0x26
022DDE7C EA000996 b       22E04DCh              //0x27
022DDE80 EA000452 b       22DEFD0h              //0x28
022DDE84 EA00048B b       22DF0B8h              //0x29
022DDE88 EA000526 b       22DF328h              //0x2a
022DDE8C EA00052D b       22DF348h              //0x2b
022DDE90 EA000991 b       22E04DCh              //0x2c
022DDE94 EA00050D b       22DF2D0h              //0x2d
022DDE98 EA0004D3 b       22DF1ECh              //0x2e
022DDE9C EA0004E5 b       22DF238h              //0x2f
022DDEA0 EA0004BE b       22DF1A0h              //0x30
022DDEA4 EA0004F6 b       22DF284h              //0x31
022DDEA8 EA0004A9 b       22DF154h              //0x32
022DDEAC EA000492 b       22DF0FCh              //0x33
022DDEB0 EA000457 b       22DF014h              //0x34
022DDEB4 EA00046A b       22DF064h              //0x35
022DDEB8 EA000512 b       22DF308h              //0x36
022DDEBC EA00094F b       22E0400h              //0x37
022DDEC0 EA000957 b       22E0424h              //0x38
022DDEC4 EA000984 b       22E04DCh              //0x39
022DDEC8 EA000983 b       22E04DCh              //0x3a
022DDECC EA000982 b       22E04DCh              //0x3b
022DDED0 EA000981 b       22E04DCh              //0x3c
022DDED4 EA000980 b       22E04DCh              //0x3d
022DDED8 EA00097F b       22E04DCh              //0x3e
022DDEDC EA00097E b       22E04DCh              //0x3f
022DDEE0 EA00097D b       22E04DCh              //0x40
022DDEE4 EA00097C b       22E04DCh              //0x41
022DDEE8 EA00097B b       22E04DCh              //0x42
022DDEEC EA00097A b       22E04DCh              //0x43
022DDEF0 EA000979 b       22E04DCh              //0x44
022DDEF4 EA000978 b       22E04DCh              //0x45
022DDEF8 EA000977 b       22E04DCh              //0x46
022DDEFC EA000976 b       22E04DCh              //0x47
022DDF00 EA000975 b       22E04DCh              //0x48
022DDF04 EA000974 b       22E04DCh              //0x49
022DDF08 EA000973 b       22E04DCh              //0x4a
022DDF0C EA000972 b       22E04DCh              //0x4b
022DDF10 EA000971 b       22E04DCh              //0x4c
022DDF14 EA000970 b       22E04DCh              //0x4d
022DDF18 EA00096F b       22E04DCh              //0x4e
022DDF1C EA00096E b       22E04DCh              //0x4f
022DDF20 EA00096D b       22E04DCh              //0x50
022DDF24 EA00096C b       22E04DCh              //0x51
022DDF28 EA00096B b       22E04DCh              //0x52
022DDF2C EA00096A b       22E04DCh              //0x53
022DDF30 EA000969 b       22E04DCh              //0x54
022DDF34 EA000968 b       22E04DCh              //0x55
022DDF38 EA000967 b       22E04DCh              //0x56
022DDF3C EA000966 b       22E04DCh              //0x57
022DDF40 EA000965 b       22E04DCh              //0x58
022DDF44 EA000964 b       22E04DCh              //0x59
022DDF48 EA000963 b       22E04DCh              //0x5a
022DDF4C EA000962 b       22E04DCh              //0x5b
022DDF50 EA000961 b       22E04DCh              //0x5c
022DDF54 EA000960 b       22E04DCh              //0x5d
022DDF58 EA00095F b       22E04DCh              //0x5e
022DDF5C EA00095E b       22E04DCh              //0x5f
022DDF60 EA00095D b       22E04DCh              //0x60
022DDF64 EA00095C b       22E04DCh              //0x61
022DDF68 EA00095B b       22E04DCh              //0x62
022DDF6C EA00095A b       22E04DCh              //0x63 Case 022E04DC    
022DDF70 EA000959 b       22E04DCh              //0x64
022DDF74 EA000958 b       22E04DCh              //0x65
022DDF78 EA000957 b       22E04DCh              //0x66 CaseScenario 022E04DC
022DDF7C EA000956 b       22E04DCh              //0x67
022DDF80 EA000955 b       22E04DCh              //0x68
022DDF84 EA000954 b       22E04DCh              //0x69
022DDF88 EA000953 b       22E04DCh              //0x6a
022DDF8C EA000952 b       22E04DCh              //0x6B debug_Print 022E04DC
022DDF90 EA000951 b       22E04DCh              //0x6c
022DDF94 EA000950 b       22E04DCh              //0x6d
022DDF98 EA00094F b       22E04DCh              //0x6e
022DDF9C EA00094C b       22E04D4h              //0x6f
022DDFA0 EA000947 b       22E04C4h              //0x70 End 022E04C4
022DDFA4 EA00094C b       22E04DCh              //0x71
022DDFA8 EA00094B b       22E04DCh              //0x72
022DDFAC EA00094A b       22E04DCh              //0x73
022DDFB0 EA0003A3 b       22DEE44h              //0x74 
022DDFB4 EA000382 b       22DEDC4h              //0x75 
022DDFB8 EA000390 b       22DEE00h              //0x76 
022DDFBC EA00036E b       22DED7Ch              //0x77
022DDFC0 EA000366 b       22DED60h              //0x78
022DDFC4 EA000373 b       22DED98h              //0x79
022DDFC8 EA0003CD b       22DEF04h              //0x7a
022DDFCC EA0003BA b       22DEEBCh              //0x7b
022DDFD0 EA0003ED b       22DEF8Ch              //0x7c
022DDFD4 EA0003E2 b       22DEF64h              //0x7D flag_SetDungeonMode 022DEF64
022DDFD8 EA0003D2 b       22DEF28h              //0x7e
022DDFDC EA0003BE b       22DEEDCh              //0x7f
022DDFE0 EA0003A7 b       22DEE84h              //0x80
022DDFE4 EA000933 b       22E04B8h              //0x81
022DDFE8 EA000937 b       22E04CCh              //0x82 Hold 022E04CC
022DDFEC EA00093A b       22E04DCh              //0x83
022DDFF0 EA000939 b       22E04DCh              //0x84
022DDFF4 EA000938 b       22E04DCh              //0x85
022DDFF8 EA000937 b       22E04DCh              //0x86
022DDFFC EA000903 b       22E0410h              //0x87 
022DE000 EA00090B b       22E0434h              //0x88
022DE004 EA0000E8 b       22DE3ACh              //0x89
022DE008 EA000933 b       22E04DCh              //0x8a
022DE00C EA000332 b       22DECDCh              //0x8b
022DE010 EA000931 b       22E04DCh              //0x8c
022DE014 EA000930 b       22E04DCh              //0x8D main_EnterDungeon    022E04DC
022DE018 EA00010E b       22DE458h              //0x8e
022DE01C EA000118 b       22DE484h              //0x8f
022DE020 EA00092D b       22E04DCh              //0x90 main_EnterRescueUser
022DE024 EA00092C b       22E04DCh              //0x91 main_EnterTraining
022DE028 EA00092B b       22E04DCh              //0x92 main_EnterTraining2
022DE02C EA000122 b       22DE4BCh              //0x93 main_SetGround 022DE4BC
022DE030 EA000929 b       22E04DCh              //0x94 me_Play
022DE034 EA000928 b       22E04DCh              //0x95 me_Stop
022DE038 EA000639 b       22DF924h              //0x96 message_Close
022DE03C EA000638 b       22DF924h              //0x97 message_CloseEnforce
022DE040 EA000682 b       22DFA50h              //0x98 message_Explanation
022DE044 EA00061B b       22DF8B8h              //0x99 message_FacePositionOffset
022DE048 EA000637 b       22DF92Ch              //0x9A message_ImitationSound
022DE04C EA000634 b       22DF924h              //0x9B message_KeyWait
022DE050 EA000635 b       22DF92Ch              //0x9C message_Mail
022DE054 EA0000E5 b       22DE3F0h              //0x9D message_Menu 022DE3F0
022DE058 EA000633 b       22DF92Ch              //0x9E message_Monologue
022DE05C EA000685 b       22DFA78h              //0x9F message_Narration
022DE060 EA000631 b       22DF92Ch              //0xA0 message_Notice
022DE064 EA0005AF b       22DF728h              //0xA1 message_EmptyActor
022DE068 EA0005A8 b       22DF710h              //0xA2 message_ResetActor
022DE06C EA0005B3 b       22DF740h              //0xA3 message_SetActor
022DE070 EA0005BA b       22DF760h              //0xA4 message_SetFace
022DE074 EA0005EE b       22DF834h              //0xA5 message_SetFaceEmpty
022DE078 EA0005D3 b       22DF7CCh              //0xA6 message_SetFaceOnly
022DE07C EA000606 b       22DF89Ch              //0xA7 message_SetFacePosition
022DE080 EA00061B b       22DF8F4h              //0xA8 message_SetWaitMode
022DE084 EA000688 b       22DFAACh              //0xA9 message_SpecialTalk
022DE088 EA00072D b       22DFD44h              //0xAA message_SwitchMenu
022DE08C EA00072C b       22DFD44h              //0xAB message_SwitchMenu2
022DE090 EA0006FF b       22DFC94h              //0xAC message_SwitchMonologue
022DE094 EA0006FE b       22DFC94h              //0xAD message_SwitchTalk
022DE098 EA000623 b       22DF92Ch              //0xAE message_Talk 022DF92C
022DE09C EA00090E b       22E04DCh              //0xAF
022DE0A0 EA00090D b       22E04DCh              //0xB0
022DE0A4 EA00090C b       22E04DCh              //0xB1
022DE0A8 EA00090B b       22E04DCh              //0xB2
022DE0AC EA00090A b       22E04DCh              //0xB3 Move2PositionOffset -1  022E04DC
022DE0B0 EA000909 b       22E04DCh              //0xB4
022DE0B4 EA000908 b       22E04DCh              //0xB5
022DE0B8 EA000907 b       22E04DCh              //0xB6
022DE0BC EA000906 b       22E04DCh              //0xB7
022DE0C0 EA000905 b       22E04DCh              //0xB8 Return 022E04DC
022DE0C4 EA000904 b       22E04DCh              //0xB9
022DE0C8 EA000903 b       22E04DCh              //0xBa
022DE0CC EA000902 b       22E04DCh              //0xBb
022DE0D0 EA000901 b       22E04DCh              //0xBc
022DE0D4 EA000900 b       22E04DCh              //0xBd
022DE0D8 EA0008FF b       22E04DCh              //0xBe
022DE0DC EA0008FE b       22E04DCh              //0xBf
022DE0E0 EA0008FD b       22E04DCh              //0xC0
022DE0E4 EA0008FC b       22E04DCh              //0xC1
022DE0E8 EA0008FB b       22E04DCh              //0xC2
022DE0EC EA0008FA b       22E04DCh              //0xC3
022DE0F0 EA0008F9 b       22E04DCh              //0xC4
022DE0F4 EA0008F8 b       22E04DCh              //0xC5 - MovePositionOffset -1 022E04DC
022DE0F8 EA0008F7 b       22E04DCh              //0xC6 - MovePositionOffset     022E04DC
022DE0FC EA0008F6 b       22E04DCh              //0xC7
022DE100 EA0008F5 b       22E04DCh              //0xC8
022DE104 EA0000A8 b       22DE3ACh              //0xC9 object         022DE3AC
022DE108 EA0008F3 b       22E04DCh              //0xCA
022DE10C EA0000A6 b       22DE3ACh              //0xCB performer      022DE3AC
022DE110 EA0000CC b       22DE448h              //0xCC ProcessSpecial 022DE448
022DE114 EA0008F0 b       22E04DCh              //0xCd
022DE118 EA0008EF b       22E04DCh              //0xCe
022DE11C EA0008EE b       22E04DCh              //0xCf
022DE120 EA0008ED b       22E04DCh              //0xD0
022DE124 EA0008EC b       22E04DCh              //0xD1
022DE128 EA0008EB b       22E04DCh              //0xD2
022DE12C EA0008EA b       22E04DCh              //0xD3
022DE130 EA0008E9 b       22E04DCh              //0xD4
022DE134 EA0008D4 b       22E048Ch              //0xD5
022DE138 EA0008E7 b       22E04DCh              //0xD6
022DE13C EA0007B7 b       22E0020h              //0xD7
022DE140 EA0007F0 b       22E0108h              //0xD8
022DE144 EA00079D b       22DFFC0h              //0xD9
022DE148 EA0007D6 b       22E00A8h              //0xDa
022DE14C EA0007A5 b       22DFFE8h              //0xDb
022DE150 EA0007DE b       22E00D0h              //0xDc
022DE154 EA0008E0 b       22E04DCh              //0xDd
022DE158 EA000846 b       22E0278h              //0xDe
022DE15C EA000845 b       22E0278h              //0xDf
022DE160 EA0007AE b       22E0020h              //0xE0
022DE164 EA0007E7 b       22E0108h              //0xE1
022DE168 EA0008DB b       22E04DCh              //0xE2
022DE16C EA0008DA b       22E04DCh              //0xE3
022DE170 EA00079C b       22DFFE8h              //0xE4
022DE174 EA0007D5 b       22E00D0h              //0xE5
022DE178 EA00081C b       22E01F0h              //0xE6
022DE17C EA0008D6 b       22E04DCh              //0xE7
022DE180 EA000802 b       22E0190h              //0xE8
022DE184 EA0008D4 b       22E04DCh              //0xE9
022DE188 EA00080A b       22E01B8h              //0xEa
022DE18C EA0008D2 b       22E04DCh              //0xEb
022DE190 EA0008D1 b       22E04DCh              //0xEc
022DE194 EA000837 b       22E0278h              //0xEd
022DE198 EA000836 b       22E0278h              //0xEe
022DE19C EA000813 b       22E01F0h              //0xEf
022DE1A0 EA0008CD b       22E04DCh              //0xF0
022DE1A4 EA0008CC b       22E04DCh              //0xF1
022DE1A8 EA0008CB b       22E04DCh              //0xF2
022DE1AC EA000801 b       22E01B8h              //0xF3
022DE1B0 EA0008C9 b       22E04DCh              //0xF4
022DE1B4 EA0008C8 b       22E04DCh              //0xF5
022DE1B8 EA0008C7 b       22E04DCh              //0xF6
022DE1BC EA0008C6 b       22E04DCh              //0xF7
022DE1C0 EA0008C5 b       22E04DCh              //0xF8
022DE1C4 EA0008C4 b       22E04DCh              //0xF9
022DE1C8 EA0008C3 b       22E04DCh              //0xFa
022DE1CC EA0008C2 b       22E04DCh              //0xFb
022DE1D0 EA0008C1 b       22E04DCh              //0xFc
022DE1D4 EA0008C0 b       22E04DCh              //0xFD SetAnimation 022E04DC
022DE1D8 EA0008BF b       22E04DCh              //0xFe
022DE1DC EA0008BE b       22E04DCh              //0xFf
022DE1E0 EA0008BD b       22E04DCh              //0x10
022DE1E4 EA0008BC b       22E04DCh              //0x101
022DE1E8 EA0008BB b       22E04DCh              //0x102
022DE1EC EA0008BA b       22E04DCh              //0x103
022DE1F0 EA0008B9 b       22E04DCh              //0x104
022DE1F4 EA0008B8 b       22E04DCh              //0x105
022DE1F8 EA0008B7 b       22E04DCh              //0x106
022DE1FC EA0008B6 b       22E04DCh              //0x107
022DE200 EA0008B5 b       22E04DCh              //0x108
022DE204 EA0008B4 b       22E04DCh              //0x109
022DE208 EA0008B3 b       22E04DCh              //0x10a
022DE20C EA0008B2 b       22E04DCh              //0x10b
022DE210 EA0008B1 b       22E04DCh              //0x10c
022DE214 EA0008B0 b       22E04DCh              //0x10d
022DE218 EA0008AF b       22E04DCh              //0x10e
022DE21C EA0008AE b       22E04DCh              //0x10f
022DE220 EA0008AD b       22E04DCh              //0x110
022DE224 EA0008AC b       22E04DCh              //0x111
022DE228 EA0008AB b       22E04DCh              //0x112
022DE22C EA0008AA b       22E04DCh              //0x113
022DE230 EA0008A9 b       22E04DCh              //0x114
022DE234 EA0008A8 b       22E04DCh              //0x115
022DE238 EA0008A7 b       22E04DCh              //0x116
022DE23C EA0008A6 b       22E04DCh              //0x117
022DE240 EA0008A5 b       22E04DCh              //0x118
022DE244 EA0008A4 b       22E04DCh              //0x119
022DE248 EA0008A3 b       22E04DCh              //0x11a
022DE24C EA0008A2 b       22E04DCh              //0x11b
022DE250 EA0008A1 b       22E04DCh              //0x11c
022DE254 EA0008A0 b       22E04DCh              //0x11d
022DE258 EA00089F b       22E04DCh              //0x11e
022DE25C EA00089E b       22E04DCh              //0x11f
022DE260 EA00089D b       22E04DCh              //0x120
022DE264 EA00089C b       22E04DCh              //0x121
022DE268 EA00089B b       22E04DCh              //0x122
022DE26C EA00089A b       22E04DCh              //0x123
022DE270 EA000899 b       22E04DCh              //0x124
022DE274 EA000898 b       22E04DCh              //0x125
022DE278 EA000897 b       22E04DCh              //0x126
022DE27C EA000896 b       22E04DCh              //0x127
022DE280 EA000895 b       22E04DCh              //0x128
022DE284 EA000894 b       22E04DCh              //0x129
022DE288 EA000169 b       22DE834h              //0x12A supervision_Acting                  022DE834
022DE28C EA000168 b       22DE834h              //0x12B supervision_ActingInvisible         022DE834
022DE290 EA000243 b       22DEBA4h              //0x12C supervision_ExecuteActing           022DEBA4
022DE294 EA000242 b       22DEBA4h              //0x12D supervision_ExecuteActingSub        022DEBA4
022DE298 EA0001A5 b       22DE934h              //0x12E supervision_ExecuteCommon           022DE934
022DE29C EA0001C5 b       22DE9B8h              //0x12F supervision_ExecuteEnter            022DE9B8
022DE2A0 EA000207 b       22DEAC4h              //0x130 supervision_ExecuteStation          022DEAC4
022DE2A4 EA0001D7 b       22DEA08h              //0x131 supervision_ExecuteStationCommon    022DEA08    
022DE2A8 EA0001D6 b       22DEA08h              //0x132 supervision_ExecuteStationCommonSub 022DEA08
022DE2AC EA000204 b       22DEAC4h              //0x133 supervision_ExecuteStationSub       022DEAC4
022DE2B0 EA0001AA b       22DE960h              //0x134 supervision_ExecuteExport           022DE960
022DE2B4 EA0001A9 b       22DE960h              //0x135 supervision_ExecuteExportSub        022DE960    
022DE2B8 EA00026F b       22DEC7Ch              //0x136 supervision_LoadStation             022DEC7C
022DE2BC EA00015C b       22DE834h              //0x137 supervision_Remove
022DE2C0 EA00015B b       22DE834h              //0x138 supervision_RemoveActing
022DE2C4 EA00015A b       22DE834h              //0x139 supervision_RemoveCommon
022DE2C8 EA0001B8 b       22DE9B0h              //0x13A supervision_SpecialActing
022DE2CC EA000158 b       22DE834h              //0x13B supervision_Station                 022DE834
022DE2D0 EA000157 b       22DE834h              //0x13C supervision_StationCommon           022DE834
022DE2D4 EA000156 b       22DE834h              //0x13D supervision_Suspend
022DE2D8 EA0001B4 b       22DE9B0h              //0x13E supervision2_SpecialActing
022DE2DC EA000424 b       22DF374h              //0x13F Switch 								022DF374
022DE2E0 EA000423 b       22DF374h              //0x140
022DE2E4 EA000422 b       22DF374h              //0x141
022DE2E8 EA000421 b       22DF374h              //0x142
022DE2EC EA000420 b       22DF374h              //0x143
022DE2F0 EA00041F b       22DF374h              //0x144
022DE2F4 EA00041E b       22DF374h              //0x145
022DE2F8 EA00041D b       22DF374h              //0x146
022DE2FC EA00041C b       22DF374h              //0x147
022DE300 EA00041B b       22DF374h              //0x148
022DE304 EA00041A b       22DF374h              //0x149
022DE308 EA000419 b       22DF374h              //0x14a
022DE30C EA000418 b       22DF374h              //0x14b
022DE310 EA000871 b       22E04DCh              //0x14c
022DE314 EA000870 b       22E04DCh              //0x14D Turn2DirectionLives 022E04DC
022DE318 EA00086F b       22E04DCh              //0x14e
022DE31C EA00086E b       22E04DCh              //0x14f
022DE320 EA00086D b       22E04DCh              //0x150
022DE324 EA00086C b       22E04DCh              //0x151
022DE328 EA00086B b       22E04DCh              //0x152
022DE32C EA00086A b       22E04DCh              //0x153
022DE330 EA000869 b       22E04DCh              //0x154
022DE334 EA000868 b       22E04DCh              //0x155
022DE338 EA000282 b       22DED48h              //0x156
022DE33C EA00080D b       22E0378h              //0x157
022DE340 EA00081C b       22E03B8h              //0x158
022DE344 EA00081F b       22E03C8h              //0x159
022DE348 EA00081E b       22E03C8h              //0x15a
022DE34C EA000823 b       22E03E0h              //0x15b
022DE350 EA000822 b       22E03E0h              //0x15c
022DE354 EA000821 b       22E03E0h              //0x15d
022DE358 EA000818 b       22E03C0h              //0x15e
022DE35C EA00085E b       22E04DCh              //0x15f
022DE360 EA000820 b       22E03E8h              //0x160
022DE364 EA00081F b       22E03E8h              //0x161
022DE368 EA00081E b       22E03E8h              //0x162
022DE36C EA00081B b       22E03E0h              //0x163
022DE370 EA00081E b       22E03F0h              //0x164
022DE374 EA00081D b       22E03F0h              //0x165
022DE378 EA00081C b       22E03F0h              //0x166
022DE37C EA00081B b       22E03F0h              //0x167
022DE380 EA000816 b       22E03E0h              //0x168
022DE384 EA00081B b       22E03F8h              //0x169
022DE388 EA00081A b       22E03F8h              //0x16a
022DE38C EA0007FE b       22E038Ch              //0x16b
022DE390 EA000812 b       22E03E0h              //0x16c
022DE394 EA000811 b       22E03E0h              //0x16d
022DE398 EA000810 b       22E03E0h              //0x16e
022DE39C EA00080F b       22E03E0h              //0x16f
022DE3A0 EA00080A b       22E03D0h              //0x170
022DE3A4 EA00080B b       22E03D8h              //0x171
022DE3A8 EA000808 b       22E03D0h              //0x172
//022DE3AC object, performer, etc..
022DE3AC E594201C ldr     r2,[r4,1Ch]               //R2 = AddressCurrentOpCode
022DE3B0 E59F0950 ldr     r0,=2318610h              //R0 = OpCode Table Beginning
022DE3B4 E1D210B0 ldrh    r1,[r2]                   //R1 = CurrentOpCode
022DE3B8 E1A01181 mov     r1,r1,lsl 3h              //R1 = CurrentOpCode * 8
022DE3BC E19000D1 ldrsb   r0,[r0,r1]                //R0 = NbParamsOpCode
022DE3C0 E3500000 cmp     r0,0h                     
if( NbParams >= 0 )
    022DE3C4 A2800001 addge   r0,r0,1h              //R0 = NbParamsOpCode + 1
    022DE3C8 A0820080 addge   r0,r2,r0,lsl 1h       //R0 = AddressCurrentOpCode + ((NbParamsOpCode + 1) * 2)
    022DE3CC AA000004 bge     22DE3E4h              ///022DE3E4
022DE3D0 E1D200B2 ldrh    r0,[r2,2h]
022DE3D4 EB001934 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned                  //(Turn 16bits signed into 14bits unsigned)
022DE3D8 E594101C ldr     r1,[r4,1Ch]
022DE3DC E2800002 add     r0,r0,2h
022DE3E0 E0810080 add     r0,r1,r0,lsl 1h
///NbParams >= 0
022DE3E4 E584001C str     r0,[r4,1Ch]               //[R4 + 0x1C] = AddressCurrentOpCode + ((NbParamsOpCode + 1) * 2)
022DE3E8 E3A00003 mov     r0,3h                     //Return 3
022DE3EC EA001021 b       22E2478h        //Return 022E2478
//0x9D message_Menu
022DE3F0 E1D600B0 ldrh    r0,[r6]                   ///
022DE3F4 EB00192C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned                  //(Turn 16bits signed into 14bits unsigned)
022DE3F8 E1A05000 mov     r5,r0
022DE3FC E59F190C ldr     r1,=23193ECh          //"ScriptData_Command_MENU request %d"
022DE400 E1A02005 mov     r2,r5
022DE404 E3A00002 mov     r0,2h
022DE408 EBF4B78C bl      200C240h              //Debug Printf
022DE40C E28D1F56 add     r1,r13,158h
022DE410 E1A00005 mov     r0,r5
022DE414 EB001F28 bl      22E60BCh
022DE418 E3500000 cmp     r0,0h
022DE41C 13A00005 movne   r0,5h
022DE420 1A001014 bne     22E2478h        //Return 022E2478
022DE424 E59D2158 ldr     r2,[r13,158h]
022DE428 E59F18E4 ldr     r1,=2319410h          //"ScriptData_Command_MENU init result %d"
022DE42C E3A00002 mov     r0,2h
022DE430 EBF4B782 bl      200C240h              //Debug Printf
022DE434 E59D1158 ldr     r1,[r13,158h]
022DE438 E1A00004 mov     r0,r4
022DE43C EB0017E9 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022DE440 E584001C str     r0,[r4,1Ch]
022DE444 EA00100A b       22E2474h
///0xCC ProcessSpecial 
022DE448 E3A00000 mov     r0,0h                     //
022DE44C E1C404B6 strh    r0,[r4,46h]               //
022DE450 E3A00005 mov     r0,5h                     //
022DE454 EA001007 b       22E2478h        //Return 022E2478                  ///022E2478

022DE458 E1D600B0 ldrh    r0,[r6]
022DE45C EB001912 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE460 E1A01800 mov     r1,r0,lsl 10h
022DE464 E1D600B2 ldrh    r0,[r6,2h]
022DE468 E1A04841 mov     r4,r1,asr 10h
022DE46C EB00190E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE470 E1A02000 mov     r2,r0
022DE474 E1A00004 mov     r0,r4
022DE478 E3A01000 mov     r1,0h
022DE47C EB002C13 bl      22E94D0h
022DE480 EA000FFB b       22E2474h
022DE484 E1D600B0 ldrh    r0,[r6]
022DE488 EB001907 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE48C E1A01800 mov     r1,r0,lsl 10h
022DE490 E1D600B2 ldrh    r0,[r6,2h]
022DE494 E1A05841 mov     r5,r1,asr 10h
022DE498 EB001903 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE49C E1A04000 mov     r4,r0
022DE4A0 E1D600B4 ldrh    r0,[r6,4h]
022DE4A4 EB001900 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE4A8 E1A01000 mov     r1,r0
022DE4AC E1A00005 mov     r0,r5
022DE4B0 E1A02004 mov     r2,r4
022DE4B4 EB002C05 bl      22E94D0h
022DE4B8 EA000FED b       22E2474h
//0x93 main_SetGround
022DE4BC E1D600B0 ldrh    r0,[r6]                   //Read parameter 1
022DE4C0 EB0018F9 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE4C4 E1A00800 mov     r0,r0,lsl 10h
022DE4C8 E1A04840 mov     r4,r0,asr 10h
022DE4CC E1A02004 mov     r2,r4
022DE4D0 E3A00000 mov     r0,0h
022DE4D4 E3A0101E mov     r1,1Eh
022DE4D8 EBF5B4D0 bl      204B820h                  //0204B820
{
    0204B820 E92D4038 push    r3-r5,r14
    0204B824 E24DD008 sub     r13,r13,8h
    0204B828 E1A03000 mov     r3,r0
    0204B82C E1A05001 mov     r5,r1
    0204B830 E1A04002 mov     r4,r2
    0204B834 E28D0000 add     r0,r13,0h
    0204B838 E1A01003 mov     r1,r3
    0204B83C E1A02005 mov     r2,r5
    0204B840 EBFFFF15 bl      204B49Ch              //0204B49C GetScriptVariableInfoAndPtr (on stack)
    0204B844 E59D1000 ldr     r1,[r13]
    0204B848 E1D100F0 ldrsh   r0,[r1]
    0204B84C E3500009 cmp     r0,9h
    0204B850 908FF100 addls   r15,r15,r0,lsl 2h
    0204B854 EA000049 b       204B980h
    0204B858 EA000048 b       204B980h
    0204B85C EA000007 b       204B880h
    0204B860 EA000016 b       204B8C0h
    0204B864 EA000015 b       204B8C0h
    0204B868 EA000017 b       204B8CCh
    0204B86C EA000019 b       204B8D8h
    0204B870 EA00001B b       204B8E4h
    0204B874 EA00001D b       204B8F0h
    0204B878 EA00001C b       204B8F0h
    0204B87C EA00001E b       204B8FCh
    0204B880 E1D100F6 ldrsh   r0,[r1,6h]
    0204B884 E3A01001 mov     r1,1h
    0204B888 E3540000 cmp     r4,0h
    0204B88C E1A00011 mov     r0,r1,lsl r0
    0204B890 159D1004 ldrne   r1,[r13,4h]
    0204B894 E20020FF and     r2,r0,0FFh
    0204B898 15D10000 ldrneb  r0,[r1]
    0204B89C 11800002 orrne   r0,r0,r2
    0204B8A0 15C10000 strneb  r0,[r1]
    0204B8A4 1A000035 bne     204B980h
    0204B8A8 E59D1004 ldr     r1,[r13,4h]
    0204B8AC E5D10000 ldrb    r0,[r1]
    0204B8B0 E1800002 orr     r0,r0,r2
    0204B8B4 E0220000 eor     r0,r2,r0
    0204B8B8 E5C10000 strb    r0,[r1]
    0204B8BC EA00002F b       204B980h
    0204B8C0 E59D0004 ldr     r0,[r13,4h]
    0204B8C4 E5C04000 strb    r4,[r0]
    0204B8C8 EA00002C b       204B980h
    0204B8CC E59D0004 ldr     r0,[r13,4h]
    0204B8D0 E5C04000 strb    r4,[r0]
    0204B8D4 EA000029 b       204B980h
    0204B8D8 E59D0004 ldr     r0,[r13,4h]
    0204B8DC E1C040B0 strh    r4,[r0]
    0204B8E0 EA000026 b       204B980h
    0204B8E4 E59D0004 ldr     r0,[r13,4h]
    0204B8E8 E1C040B0 strh    r4,[r0]
    0204B8EC EA000023 b       204B980h
    0204B8F0 E59D0004 ldr     r0,[r13,4h]
    0204B8F4 E5804000 str     r4,[r0]
    0204B8F8 EA000020 b       204B980h
    0204B8FC E3550049 cmp     r5,49h
    0204B900 CA000008 bgt     204B928h
    0204B904 AA000012 bge     204B954h
    0204B908 E355003D cmp     r5,3Dh
    0204B90C CA00001B bgt     204B980h
    0204B910 E355003C cmp     r5,3Ch
    0204B914 BA000019 blt     204B980h
    0204B918 0A000005 beq     204B934h
    0204B91C E355003D cmp     r5,3Dh
    0204B920 0A000008 beq     204B948h
    0204B924 EA000015 b       204B980h
    0204B928 E3550070 cmp     r5,70h
    0204B92C 0A00000E beq     204B96Ch
    0204B930 EA000012 b       204B980h
    0204B934 E1A00004 mov     r0,r4
    0204B938 EBFF0CF7 bl      200ED1Ch
    0204B93C E3A00000 mov     r0,0h
    0204B940 EBFF0D04 bl      200ED58h
    0204B944 EA00000D b       204B980h
    0204B948 E1A00004 mov     r0,r4
    0204B94C EBFF1374 bl      2010724h
    0204B950 EA00000A b       204B980h
    0204B954 EBFFFD99 bl      204AFC0h
    0204B958 E3500001 cmp     r0,1h
    0204B95C 1A000007 bne     204B980h
    0204B960 E1A00004 mov     r0,r4
    0204B964 EB0003EF bl      204C928h
    0204B968 EA000004 b       204B980h
    0204B96C E3540000 cmp     r4,0h
    0204B970 13A00001 movne   r0,1h
    0204B974 03A00000 moveq   r0,0h
    0204B978 E20000FF and     r0,r0,0FFh
    0204B97C EBFFF2CB bl      20484B0h
    0204B980 E28DD008 add     r13,r13,8h
    0204B984 E8BD8038 pop     r3-r5,r15
}
022DE4DC E1A02004 mov     r2,r4
022DE4E0 E3A00000 mov     r0,0h
022DE4E4 E3A0101C mov     r1,1Ch
022DE4E8 EBF5B4CC bl      204B820h
022DE4EC E3A00000 mov     r0,0h
022DE4F0 E1A02000 mov     r2,r0
022DE4F4 E3A0101D mov     r1,1Dh
022DE4F8 EBF5B4C8 bl      204B820h
022DE4FC EA000FDC b       22E2474h
//0x9
022DE500 E1D600B0 ldrh    r0,[r6]                   //Load first parameter
022DE504 EB0018E8 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE508 E59F1808 ldr     r1,=2324E80h              //
022DE50C E1C100B8 strh    r0,[r1,8h]
022DE510 E1D120F8 ldrsh   r2,[r1,8h]
022DE514 E3A00002 mov     r0,2h
022DE518 E1C120B6 strh    r2,[r1,6h]
022DE51C EBF4B744 bl      200C234h                  //
022DE520 E3500000 cmp     r0,0h
if( R0 == 0 )
    022DE524 0A00000A beq     22DE554h              //022DE554
022DE528 E59F37E8 ldr     r3,=2324E80h
022DE52C E3A0000C mov     r0,0Ch
022DE530 E1D320F8 ldrsh   r2,[r3,8h]
022DE534 E59F47E0 ldr     r4,=20A5490h              //Address table of event names!!
022DE538 E59F17E0 ldr     r1,=2319438h              //"    ground select %3d %3d[%s]"
022DE53C E1600082 smulbb  r0,r2,r0
022DE540 E7944000 ldr     r4,[r4,r0]
022DE544 E3A00002 mov     r0,2h
022DE548 E58D4000 str     r4,[r13]
022DE54C E1D330F6 ldrsh   r3,[r3,6h]
022DE550 EBF4B73A bl      200C240h              //Debug Printf
022DE554 E59F17BC ldr     r1,=2324E80h
022DE558 E3A00000 mov     r0,0h
022DE55C E1D120F8 ldrsh   r2,[r1,8h]
022DE560 E3A0101F mov     r1,1Fh
022DE564 EBF5B4AD bl      204B820h
022DE568 E59F07A8 ldr     r0,=2324E80h
022DE56C E3A0100C mov     r1,0Ch
022DE570 E1D030F8 ldrsh   r3,[r0,8h]
022DE574 E59F27A8 ldr     r2,=20A548Ah
022DE578 E3A00000 mov     r0,0h
022DE57C E1610183 smulbb  r1,r3,r1
022DE580 E19220F1 ldrsh   r2,[r2,r1]
022DE584 E3A01020 mov     r1,20h
022DE588 EBF5B4A4 bl      204B820h
022DE58C E59F0784 ldr     r0,=2324E80h
022DE590 E1D000F6 ldrsh   r0,[r0,6h]
022DE594 EB005846 bl      22F46B4h
022DE598 E3A00000 mov     r0,0h
022DE59C EB003295 bl      22EAFF8h
022DE5A0 EB008120 bl      22FEA28h
022DE5A4 EB0065DB bl      22F7D18h
022DE5A8 EB007615 bl      22FBE04h
022DE5AC EB007C2E bl      22FD66Ch
022DE5B0 EB004B23 bl      22F1244h
022DE5B4 E3A00005 mov     r0,5h
022DE5B8 EA000FAE b       22E2478h        //Return 022E2478
//0x18 back2_SetMode
022DE5BC E1D600B0 ldrh    r0,[r6]
022DE5C0 EB0018B9 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE5C4 E1A00C00 mov     r0,r0,lsl 18h
022DE5C8 E1A00C40 mov     r0,r0,asr 18h
022DE5CC EB00262C bl      22E7E84h              //Function that refers to "script ground sub call %3d"
022DE5D0 E3A00004 mov     r0,4h
022DE5D4 EA000FA7 b       22E2478h        //Return 022E2478
022DE5D8 E1D600B0 ldrh    r0,[r6]
022DE5DC EB0018B2 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE5E0 E59F1730 ldr     r1,=2324E80h
022DE5E4 E1C100B2 strh    r0,[r1,2h]
022DE5E8 E1D120F2 ldrsh   r2,[r1,2h]
022DE5EC E3A00002 mov     r0,2h
022DE5F0 E1C120B4 strh    r2,[r1,4h]
022DE5F4 EBF4B70E bl      200C234h
022DE5F8 E3500000 cmp     r0,0h
022DE5FC 0A00000A beq     22DE62Ch
022DE600 E59F3710 ldr     r3,=2324E80h
022DE604 E3A0000C mov     r0,0Ch
022DE608 E1D320F2 ldrsh   r2,[r3,2h]
022DE60C E59F4708 ldr     r4,=20A5490h              //Event Name Lookup Table
022DE610 E59F1710 ldr     r1,=2319458h              //"    ground2 select %3d %3d[%s]"
022DE614 E1600082 smulbb  r0,r2,r0
022DE618 E7944000 ldr     r4,[r4,r0]
022DE61C E3A00002 mov     r0,2h
022DE620 E58D4000 str     r4,[r13]
022DE624 E1D330F4 ldrsh   r3,[r3,4h]
022DE628 EBF4B704 bl      200C240h              //Debug Printf
022DE62C E59F16E4 ldr     r1,=2324E80h
022DE630 E3A00000 mov     r0,0h
022DE634 E1D110F4 ldrsh   r1,[r1,4h]
022DE638 EB00263A bl      22E7F28h
022DE63C E59F06D4 ldr     r0,=2324E80h
022DE640 E1D000F6 ldrsh   r0,[r0,6h]
022DE644 EB00583A bl      22F4734h
022DE648 E3A00004 mov     r0,4h
022DE64C EA000F89 b       22E2478h        //Return 022E2478
//
022DE650 E1D600B0 ldrh    r0,[r6]
022DE654 EB001894 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE658 E1A04000 mov     r4,r0
022DE65C EB004C2B bl      22F1710h
022DE660 E59F16B0 ldr     r1,=2324E80h
022DE664 E3A00002 mov     r0,2h
022DE668 E1C140B6 strh    r4,[r1,6h]
022DE66C EBF4B6F0 bl      200C234h
022DE670 E3500000 cmp     r0,0h
022DE674 0A00000A beq     22DE6A4h
022DE678 E59F3698 ldr     r3,=2324E80h
022DE67C E3A0000C mov     r0,0Ch
022DE680 E1D320F8 ldrsh   r2,[r3,8h]
022DE684 E59F4690 ldr     r4,=20A5490h              //Address table of event names!!
022DE688 E59F169C ldr     r1,=2319478h              //"    ground change %3d %3d[%s]"
022DE68C E1600082 smulbb  r0,r2,r0
022DE690 E7944000 ldr     r4,[r4,r0]
022DE694 E3A00002 mov     r0,2h
022DE698 E58D4000 str     r4,[r13]
022DE69C E1D330F6 ldrsh   r3,[r3,6h]
022DE6A0 EBF4B6E6 bl      200C240h              //Debug Printf
022DE6A4 E3A00005 mov     r0,5h
022DE6A8 EA000F72 b       22E2478h        //Return 022E2478
022DE6AC E3A00005 mov     r0,5h
022DE6B0 EA000F70 b       22E2478h        //Return 022E2478
022DE6B4 E1D600B0 ldrh    r0,[r6]
022DE6B8 EB00187B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE6BC E1A01000 mov     r1,r0
022DE6C0 E3A00001 mov     r0,1h
022DE6C4 EB002617 bl      22E7F28h
022DE6C8 E3A00004 mov     r0,4h
022DE6CC EA000F69 b       22E2478h        //Return 022E2478
//0x04 back_SetBackScrollSpeed
022DE6D0 E59F0658 ldr     r0,=2316A44h
022DE6D4 E5901038 ldr     r1,[r0,38h]           
022DE6D8 E5900034 ldr     r0,[r0,34h]
022DE6DC E58D1154 str     r1,[r13,154h]
022DE6E0 E58D0150 str     r0,[r13,150h]
022DE6E4 E1D600B0 ldrh    r0,[r6]                   //Load first parameter
022DE6E8 EB00186F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE6EC E58D0150 str     r0,[r13,150h]
022DE6F0 E1D600B2 ldrh    r0,[r6,2h]                //Load second parameter
022DE6F4 EB00186C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE6F8 E58D0154 str     r0,[r13,154h]
022DE6FC E3550013 cmp     r5,13h
if( CurrentOpCode > 0x13 )
    022DE700 CA000008 bgt     22DE728h
if( CurrentOpCode >= 0x13 )
    022DE704 AA000010 bge     22DE74Ch
022DE708 E3550004 cmp     r5,4h
if( CurrentOpCode > 4 )
    022DE70C CA000F58 bgt     22E2474h
022DE710 E3550003 cmp     r5,3h
if( CurrentOpCode < 3 )
    022DE714 BA000F56 blt     22E2474h
if( CurrentOpCode == 3 )
    022DE718 0A000005 beq     22DE734h
022DE71C E3550004 cmp     r5,4h
if( CurrentOpCode == 4 )
    022DE720 0A000006 beq     22DE740h              ///022DE740 CurrentOpCode == 4 
022DE724 EA000F52 b       22E2474h
022DE728 E3550014 cmp     r5,14h
022DE72C 0A000009 beq     22DE758h
022DE730 EA000F4F b       22E2474h
022DE734 E28D0E15 add     r0,r13,150h
022DE738 EB004DE3 bl      22F1ECCh
022DE73C EA000F4C b       22E2474h
///022DE740 CurrentOpCode == 4 
022DE740 E28D0E15 add     r0,r13,150h
022DE744 EB004DD8 bl      22F1EACh                  ///fun_ 022F1EAC
{
    022F1EAC E59F2010 ldr     r2,=2324CC0h
    022F1EB0 E1A01000 mov     r1,r0                 //PtrFirstParameter
    022F1EB4 E5920004 ldr     r0,[r2,4h]            //r0 = [2324CC0h,4h]
    022F1EB8 E59FC008 ldr     r12,=22F03F8h
    022F1EBC E2800004 add     r0,r0,4h
    022F1EC0 E12FFF1C bx      r12
    {
        022F03F8 E5912000 ldr     r2,[r1]           //r2 = FirstParameter
        022F03FC E5911004 ldr     r1,[r1,4h]        //r1 = SecondParameter  
        022F0400 E5802004 str     r2,[r0,4h]        //
        022F0404 E5801008 str     r1,[r0,8h]
        022F0408 E12FFF1E bx      r14
    }
    022F1EC4 02324CC0
    022F1EC8 022F03F8
}
022DE748 EA000F49 b       22E2474h                  ///022E2474
022DE74C E28D0E15 add     r0,r13,150h
022DE750 EB00C99A bl      2310DC0h
022DE754 EA000F46 b       22E2474h
022DE758 E28D0E15 add     r0,r13,150h
022DE75C EB00C98F bl      2310DA0h
022DE760 EA000F43 b       22E2474h
022DE764 E1D600B0 ldrh    r0,[r6]
022DE768 EB00184F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE76C E1A04000 mov     r4,r0
022DE770 E20400FF and     r0,r4,0FFh
022DE774 EB004D8B bl      22F1DA8h
022DE778 EB00CB37 bl      231145Ch
022DE77C E3500000 cmp     r0,0h
022DE780 0A000003 beq     22DE794h
022DE784 E1A01004 mov     r1,r4
022DE788 E3A00001 mov     r0,1h
022DE78C EB0025E5 bl      22E7F28h
022DE790 EA000F37 b       22E2474h
022DE794 EB00CC70 bl      231195Ch
022DE798 E3500000 cmp     r0,0h
022DE79C 0A000F34 beq     22E2474h
022DE7A0 E1A01004 mov     r1,r4
022DE7A4 E3A00001 mov     r0,1h
022DE7A8 EB0025DE bl      22E7F28h
022DE7AC EA000F30 b       22E2474h
022DE7B0 E1D600B0 ldrh    r0,[r6]
022DE7B4 EB00183C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE7B8 E1A01000 mov     r1,r0
022DE7BC E3A00002 mov     r0,2h
022DE7C0 EB0025D8 bl      22E7F28h
022DE7C4 EA000F2A b       22E2474h
022DE7C8 E1D600B0 ldrh    r0,[r6]
022DE7CC EB001836 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE7D0 E20000FF and     r0,r0,0FFh
022DE7D4 EB004FC0 bl      22F26DCh
022DE7D8 EA000F25 b       22E2474h
022DE7DC E1D600B0 ldrh    r0,[r6]
022DE7E0 EB001831 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE7E4 E1A04000 mov     r4,r0
022DE7E8 E1D600B2 ldrh    r0,[r6,2h]
022DE7EC EB00182E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE7F0 E1A01804 mov     r1,r4,lsl 10h
022DE7F4 E1A02000 mov     r2,r0
022DE7F8 E1A01821 mov     r1,r1,lsr 10h
022DE7FC E3A00000 mov     r0,0h
022DE800 EB004363 bl      22EF594h
022DE804 EA000F1A b       22E2474h
022DE808 E1D600B0 ldrh    r0,[r6]
022DE80C EB001826 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE810 E1A04000 mov     r4,r0
022DE814 E1D600B2 ldrh    r0,[r6,2h]
022DE818 EB001823 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE81C E1A01804 mov     r1,r4,lsl 10h
022DE820 E1A02000 mov     r2,r0
022DE824 E1A01821 mov     r1,r1,lsr 10h
022DE828 E3A00001 mov     r0,1h
022DE82C EB004358 bl      22EF594h
022DE830 EA000F0F b       22E2474h
//0x12A, 0x12B, etc.. supervision_Acting, etc..
022DE834 E1D600B0 ldrh    r0,[r6]                   //Read Param0
022DE838 EB00181B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE83C E1A00C00 mov     r0,r0,lsl 18h             //R0 = Param0 << 0x18  
022DE840 E3E01000 mvn     r1,0h                     //R1 = 0xFFFFFFFF
022DE844 E1510C40 cmp     r1,r0,asr 18h             
022DE848 E1A06C40 mov     r6,r0,asr 18h             //R6 = (Param0 << 0x18) >> 0x18 (signed shift right)
022DE84C E59F14E0 ldr     r1,=137h                  
if( (Param0 << 0x18)>>0x18(signed shift) == 0xFFFFFFFF )
022DE850 01D461D2 ldreqsb r6,[r4,12h]
022DE854 E1550001 cmp     r5,r1
if( CurrentOpCode > 0x137 )
    022DE858 CA00000B bgt     22DE88Ch              //022DE88C
else if( CurrentOpCode >= 0x137 )
    022DE85C AA000028 bge     22DE904h              //022DE904
022DE860 E241000C sub     r0,r1,0Ch                 //R0 = 0x137 - 0xC = 0x12B
022DE864 E1550000 cmp     r5,r0                     
if(  CurrentOpCode > 0x12B )
    022DE868 CA000F01 bgt     22E2474h              ///022E2474 - Returns 2
022DE86C E241000D sub     r0,r1,0Dh                 //R0 = 0x137 - 0xD = 0x12A
022DE870 E1550000 cmp     r5,r0
if(  CurrentOpCode < 0x12A )
    022DE874 BA000EFE blt     22E2474h
else if(  CurrentOpCode == 0x12A )
    022DE878 0A000017 beq     22DE8DCh              ///022DE8DC
022DE87C E241000C sub     r0,r1,0Ch
022DE880 E1550000 cmp     r5,r0
022DE884 0A000019 beq     22DE8F0h
022DE888 EA000EF9 b       22E2474h
///CurrentOpCode > 0x137
022DE88C E2450F4E sub     r0,r5,138h                //R0 = CurrentOpCode - 0x138
022DE890 E3500005 cmp     r0,5h
if( R0 <= 5 )
    022DE894 908FF100 addls   r15,r15,r0,lsl 2h     ///R15 = 022DE898 + ( R0 * 4 ) + 4
022DE898 EA000EF5 b       22E2474h
022DE89C EA00001E b       22DE91Ch
022DE8A0 EA00001A b       22DE910h
022DE8A4 EA000EF2 b       22E2474h
022DE8A8 EA000001 b       22DE8B4h              ///3 -> supervision_Station 022DE8B4
022DE8AC EA000005 b       22DE8C8h
022DE8B0 EA00001C b       22DE928h
///3 -> supervision_Station
022DE8B4 E1A00006 mov     r0,r6
022DE8B8 EB001C54 bl      22E5A10h  ///Calls a function that calls ScriptLocate 022E5A10
022DE8BC E1A00006 mov     r0,r6
022DE8C0 EB001CAD bl      22E5B7Ch
022DE8C4 EA000EEA b       22E2474h
022DE8C8 E1A00006 mov     r0,r6
022DE8CC EB001C45 bl      22E59E8h  ///Calls a function that calls ScriptLocate
022DE8D0 E1A00006 mov     r0,r6
022DE8D4 EB001C99 bl      22E5B40h
022DE8D8 EA000EE5 b       22E2474h
///CurrentOpCode == 0x12A -> supervision_Acting
022DE8DC E1A00006 mov     r0,r6     //    
022DE8E0 EB001C54 bl      22E5A38h  /// Calls a function that calls ScriptLocate! 022E5A38
{
    022E5A38 E59FC00C ldr     r12,=22E4CD4h         //022E4CD4
    022E5A3C E1A01000 mov     r1,r0         //R1=PtrParam
    022E5A40 E3A00003 mov     r0,3h         //SSA
    022E5A44 E3A02000 mov     r2,0h
    022E5A48 E12FFF1C bx      r12
    022E5A4C 022E4CD4
}
022DE8E4 E1A00006 mov     r0,r6
022DE8E8 EB001CB2 bl      22E5BB8h  ///022E5BB8
022DE8EC EA000EE0 b       22E2474h

022DE8F0 E1A00006 mov     r0,r6
022DE8F4 EB001C55 bl      22E5A50h  ///Calls a function that calls ScriptLocate
022DE8F8 E1A00006 mov     r0,r6
022DE8FC EB001CAD bl      22E5BB8h
022DE900 EA000EDB b       22E2474h
022DE904 E1A00006 mov     r0,r6
022DE908 EB001C71 bl      22E5AD4h
022DE90C EA000ED8 b       22E2474h
022DE910 E1A00006 mov     r0,r6
022DE914 EB001C5F bl      22E5A98h
022DE918 EA000ED5 b       22E2474h
022DE91C E1A00006 mov     r0,r6
022DE920 EB001C7A bl      22E5B10h
022DE924 EA000ED2 b       22E2474h
022DE928 E1A00006 mov     r0,r6
022DE92C EB001CF8 bl      22E5D14h
022DE930 EA000ECF b       22E2474h
///0x12e - supervision_ExecuteCommon
022DE934 E1D600B0 ldrh    r0,[r6]           //R0 = Param0
022DE938 EB0017DB bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE93C E1A00800 mov     r0,r0,lsl 10h
022DE940 E1A00840 mov     r0,r0,asr 10h
022DE944 E3A01000 mov     r1,0h
022DE948 EB00620E bl      22F7188h          //022F7188 GroundSupervision ExecuteCommon
022DE94C E1D400F8 ldrsh   r0,[r4,8h]
022DE950 E3500001 cmp     r0,1h
022DE954 1A000EC6 bne     22E2474h
022DE958 E3A00003 mov     r0,3h
022DE95C EA000EC5 b       22E2478h        //Return 022E2478
//
022DE960 E1D610B0 ldrh    r1,[r6]
022DE964 E2840014 add     r0,r4,14h
022DE968 EB001636 bl      22E4248h                  //GetStringFromScriptsStringTable( strtblptr, stringid)
022DE96C E1A01000 mov     r1,r0
022DE970 E28D0F52 add     r0,r13,148h
022DE974 EB001721 bl      22E4600h
022DE978 E59F03B8 ldr     r0,=135h
022DE97C E1550000 cmp     r5,r0
022DE980 03A01001 moveq   r1,1h
022DE984 13A01000 movne   r1,0h
022DE988 E28D0F52 add     r0,r13,148h
022DE98C E20110FF and     r1,r1,0FFh
022DE990 EB00629C bl      22F7408h
022DE994 E1D400F8 ldrsh   r0,[r4,8h]
022DE998 E3500001 cmp     r0,1h
022DE99C 1A000EB4 bne     22E2474h
022DE9A0 E3A00001 mov     r0,1h
022DE9A4 E1C404B6 strh    r0,[r4,46h]
022DE9A8 E3A00003 mov     r0,3h
022DE9AC EA000EB1 b       22E2478h        //Return 022E2478
///0022DE9B0 Switch
022DE9B0 E3A00004 mov     r0,4h                            //R0=4
022DE9B4 EA000EAF b       22E2478h        //Return 022E2478                         ///022E2478 Switch

022DE9B8 E1D600B0 ldrh    r0,[r6]
022DE9BC EB0017BA bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DE9C0 E1A00800 mov     r0,r0,lsl 10h
022DE9C4 E3E01000 mvn     r1,0h
022DE9C8 E1510840 cmp     r1,r0,asr 10h
022DE9CC E1A00840 mov     r0,r0,asr 10h
022DE9D0 1A000004 bne     22DE9E8h
022DE9D4 E284006C add     r0,r4,6Ch
022DE9D8 E3A0101C mov     r1,1Ch
022DE9DC EBF5B2C2 bl      204B4ECh
022DE9E0 E1A00800 mov     r0,r0,lsl 10h
022DE9E4 E1A00840 mov     r0,r0,asr 10h
022DE9E8 E59F1328 ldr     r1,=2324E80h
022DE9EC E1C100BA strh    r0,[r1,0Ah]
022DE9F0 EB0019D4 bl      22E5148h
022DE9F4 E59F031C ldr     r0,=2324E80h
022DE9F8 E1D000FA ldrsh   r0,[r0,0Ah]
022DE9FC EB0061F8 bl      22F71E4h
022DEA00 E3A00006 mov     r0,6h
022DEA04 EA000E9B b       22E2478h        //Return 022E2478
022DEA08 E1D600B0 ldrh    r0,[r6]
022DEA0C EB0017A6 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEA10 E1A07800 mov     r7,r0,lsl 10h
022DEA14 E1D600B2 ldrh    r0,[r6,2h]
022DEA18 E1A06847 mov     r6,r7,asr 10h
022DEA1C EB0017A2 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEA20 E3E01000 mvn     r1,0h
022DEA24 E1A00C00 mov     r0,r0,lsl 18h
022DEA28 E1510847 cmp     r1,r7,asr 10h
022DEA2C E1A07C40 mov     r7,r0,asr 18h
022DEA30 059F02E0 ldreq   r0,=2324E80h
022DEA34 01D060F8 ldreqsh r6,[r0,8h]
022DEA38 E3570000 cmp     r7,0h
022DEA3C B1D471D2 ldrltsb r7,[r4,12h]
022DEA40 E1A00006 mov     r0,r6
022DEA44 EB0019BF bl      22E5148h
022DEA48 E59F02EC ldr     r0,=132h
022DEA4C E1A01007 mov     r1,r7
022DEA50 E1550000 cmp     r5,r0
022DEA54 03A02001 moveq   r2,1h
022DEA58 13A02000 movne   r2,0h
022DEA5C E1A00006 mov     r0,r6
022DEA60 E20220FF and     r2,r2,0FFh
022DEA64 EB0061F0 bl      22F722Ch
022DEA68 E59F02A8 ldr     r0,=2324E80h
022DEA6C E1D010FA ldrsh   r1,[r0,0Ah]
022DEA70 E1510006 cmp     r1,r6
022DEA74 0A000009 beq     22DEAA0h
022DEA78 E1C060BA strh    r6,[r0,0Ah]
022DEA7C E1D400F8 ldrsh   r0,[r4,8h]
022DEA80 E3500001 cmp     r0,1h
022DEA84 13A00006 movne   r0,6h
022DEA88 1A000E7A bne     22E2478h        //Return 022E2478
022DEA8C E1D401F0 ldrsh   r0,[r4,10h]
022DEA90 E3500000 cmp     r0,0h
022DEA94 0A000E76 beq     22E2474h
022DEA98 E3A00006 mov     r0,6h
022DEA9C EA000E75 b       22E2478h        //Return 022E2478
022DEAA0 E1D400F8 ldrsh   r0,[r4,8h]
022DEAA4 E3500001 cmp     r0,1h
022DEAA8 01D401F0 ldreqsh r0,[r4,10h]
022DEAAC 03500002 cmpeq   r0,2h
022DEAB0 03A00006 moveq   r0,6h
022DEAB4 13A00001 movne   r0,1h
022DEAB8 11C404B6 strneh  r0,[r4,46h]
022DEABC 13A00003 movne   r0,3h
022DEAC0 EA000E6C b       22E2478h        //Return 022E2478
022DEAC4 E1D600B0 ldrh    r0,[r6]
022DEAC8 EB001777 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEACC E1A09800 mov     r9,r0,lsl 10h
022DEAD0 E1D610B2 ldrh    r1,[r6,2h]
022DEAD4 E2840014 add     r0,r4,14h
022DEAD8 E1A07849 mov     r7,r9,asr 10h
022DEADC EB0015D9 bl      22E4248h                  //GetStringFromScriptsStringTable( strtblptr, stringid)
022DEAE0 E1A08000 mov     r8,r0
022DEAE4 E1D600B4 ldrh    r0,[r6,4h]
022DEAE8 EB00176F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEAEC E1A00C00 mov     r0,r0,lsl 18h
022DEAF0 E3E01000 mvn     r1,0h
022DEAF4 E1510849 cmp     r1,r9,asr 10h
022DEAF8 E1A06C40 mov     r6,r0,asr 18h
022DEAFC 059F0214 ldreq   r0,=2324E80h
022DEB00 E1A01008 mov     r1,r8
022DEB04 01D070F8 ldreqsh r7,[r0,8h]
022DEB08 E3560000 cmp     r6,0h
022DEB0C E28D0D05 add     r0,r13,140h
022DEB10 B1D461D2 ldrltsb r6,[r4,12h]
022DEB14 EB0016B9 bl      22E4600h
022DEB18 E28D1D05 add     r1,r13,140h
022DEB1C E1A00007 mov     r0,r7
022DEB20 EB001998 bl      22E5188h
022DEB24 E59F0214 ldr     r0,=133h
022DEB28 E28D1D05 add     r1,r13,140h
022DEB2C E1550000 cmp     r5,r0
022DEB30 03A03001 moveq   r3,1h
022DEB34 13A03000 movne   r3,0h
022DEB38 E1A00007 mov     r0,r7
022DEB3C E1A02006 mov     r2,r6
022DEB40 E20330FF and     r3,r3,0FFh
022DEB44 EB0061D5 bl      22F72A0h
022DEB48 E59F01C8 ldr     r0,=2324E80h
022DEB4C E1D010FA ldrsh   r1,[r0,0Ah]
022DEB50 E1510007 cmp     r1,r7
022DEB54 0A000009 beq     22DEB80h
022DEB58 E1C070BA strh    r7,[r0,0Ah]
022DEB5C E1D400F8 ldrsh   r0,[r4,8h]
022DEB60 E3500001 cmp     r0,1h
022DEB64 13A00006 movne   r0,6h
022DEB68 1A000E42 bne     22E2478h        //Return 022E2478
022DEB6C E1D401F0 ldrsh   r0,[r4,10h]
022DEB70 E3500000 cmp     r0,0h
022DEB74 0A000E3E beq     22E2474h
022DEB78 E3A00006 mov     r0,6h
022DEB7C EA000E3D b       22E2478h        //Return 022E2478
022DEB80 E1D400F8 ldrsh   r0,[r4,8h]
022DEB84 E3500001 cmp     r0,1h
022DEB88 01D401F0 ldreqsh r0,[r4,10h]
022DEB8C 03500002 cmpeq   r0,2h
022DEB90 03A00006 moveq   r0,6h
022DEB94 13A00001 movne   r0,1h
022DEB98 11C404B6 strneh  r0,[r4,46h]
022DEB9C 13A00003 movne   r0,3h
022DEBA0 EA000E34 b       22E2478h        //Return 022E2478
//0x12C 0x12D -> supervision_ExecuteActing, supervision_ExecuteActingSub
022DEBA4 E1D600B0 ldrh    r0,[r6]                   //R0 = Param0
022DEBA8 EB00173F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEBAC E1A09800 mov     r9,r0,lsl 10h             
022DEBB0 E1D610B2 ldrh    r1,[r6,2h]                //R1 = Param1
022DEBB4 E2840014 add     r0,r4,14h                 //R0 = ScriptEngineStruct + 20
022DEBB8 E1A07849 mov     r7,r9,asr 10h             //R7 = (Param0 << 16) >>16
022DEBBC EB0015A1 bl      22E4248h                  //GetStringFromScriptsStringTable( strtblptr, stringid)
022DEBC0 E1A08000 mov     r8,r0                     //R8 = String
022DEBC4 E1D600B4 ldrh    r0,[r6,4h]                //R0 = Param2    
022DEBC8 EB001737 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEBCC E1A00C00 mov     r0,r0,lsl 18h
022DEBD0 E3E01000 mvn     r1,0h
022DEBD4 E1510849 cmp     r1,r9,asr 10h
022DEBD8 E1A06C40 mov     r6,r0,asr 18h
022DEBDC 059F0134 ldreq   r0,=2324E80h              
022DEBE0 E1A01008 mov     r1,r8
022DEBE4 01D070F8 ldreqsh r7,[r0,8h]
022DEBE8 E3560000 cmp     r6,0h
022DEBEC E28D0F4E add     r0,r13,138h
022DEBF0 B1D461D2 ldrltsb r6,[r4,12h]
022DEBF4 EB001681 bl      22E4600h
022DEBF8 E28D1F4E add     r1,r13,138h
022DEBFC E1A00007 mov     r0,r7
022DEC00 EB0019C3 bl      22E5314h
022DEC04 E59F0138 ldr     r0,=12Dh
022DEC08 E28D1F4E add     r1,r13,138h
022DEC0C E1550000 cmp     r5,r0
022DEC10 03A03001 moveq   r3,1h
022DEC14 13A03000 movne   r3,0h
022DEC18 E1A00007 mov     r0,r7
022DEC1C E1A02006 mov     r2,r6
022DEC20 E20330FF and     r3,r3,0FFh
022DEC24 EB0061CA bl      22F7354h
022DEC28 E59F00E8 ldr     r0,=2324E80h
022DEC2C E1D010FA ldrsh   r1,[r0,0Ah]
022DEC30 E1510007 cmp     r1,r7
022DEC34 11C070BA strneh  r7,[r0,0Ah]
022DEC38 11D400F8 ldrnesh r0,[r4,8h]
022DEC3C 13500001 cmpne   r0,1h
022DEC40 13A00006 movne   r0,6h
022DEC44 1A000E0B bne     22E2478h        //Return 022E2478
022DEC48 E1D400F8 ldrsh   r0,[r4,8h]
022DEC4C E3500001 cmp     r0,1h
022DEC50 1A000E07 bne     22E2474h
022DEC54 E1A00004 mov     r0,r4
022DEC58 EBFFF73E bl      22DC958h
022DEC5C E3500000 cmp     r0,0h
022DEC60 11D401F0 ldrnesh r0,[r4,10h]
022DEC64 13500000 cmpne   r0,0h
022DEC68 13A00006 movne   r0,6h
022DEC6C 03A00001 moveq   r0,1h
022DEC70 01C404B6 streqh  r0,[r4,46h]
022DEC74 03A00003 moveq   r0,3h
022DEC78 EA000DFE b       22E2478h        //Return 022E2478
022DEC7C E1D600B0 ldrh    r0,[r6]
022DEC80 EB001709 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEC84 E1A05800 mov     r5,r0,lsl 10h
022DEC88 E1D610B2 ldrh    r1,[r6,2h]
022DEC8C E2840014 add     r0,r4,14h
022DEC90 E1A06845 mov     r6,r5,asr 10h
022DEC94 EB00156B bl      22E4248h                  //GetStringFromScriptsStringTable( strtblptr, stringid)
022DEC98 E3E02000 mvn     r2,0h
022DEC9C E1A01000 mov     r1,r0
022DECA0 E1520845 cmp     r2,r5,asr 10h
022DECA4 059F006C ldreq   r0,=2324E80h
022DECA8 01D060F8 ldreqsh r6,[r0,8h]
022DECAC E28D0E13 add     r0,r13,130h
022DECB0 EB001652 bl      22E4600h
022DECB4 E28D1E13 add     r1,r13,130h
022DECB8 E1A00006 mov     r0,r6
022DECBC EB001965 bl      22E5258h
022DECC0 E28D1E13 add     r1,r13,130h
022DECC4 E1A00006 mov     r0,r6
022DECC8 EB00620C bl      22F7500h
022DECCC E3A00001 mov     r0,1h
022DECD0 E1C404B6 strh    r0,[r4,46h]
022DECD4 E3A00003 mov     r0,3h
022DECD8 EA000DE6 b       22E2478h        //Return 022E2478
022DECDC E1D600B0 ldrh    r0,[r6]
022DECE0 EB0016F1 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DECE4 E1A00800 mov     r0,r0,lsl 10h
022DECE8 E1A01840 mov     r1,r0,asr 10h
022DECEC E1A00004 mov     r0,r4
022DECF0 E1C414B4 strh    r1,[r4,44h]
022DECF4 EBFFF87C bl      22DCEECh
022DECF8 E3500000 cmp     r0,0h
022DECFC 0A000DDC beq     22E2474h
022DED00 E3A00004 mov     r0,4h
022DED04 EA000DDB b       22E2478h        //Return 022E2478
022DED08 02318610 eoreqs  r8,r1,1000000h
022DED0C 00000172 andeq   r0,r0,r2,ror r1
022DED10 023193EC eoreqs  r9,r1,0B0000003h
022DED14 02319410 eoreqs  r9,r1,10000000h
022DED18 02324E80 eoreqs  r4,r2,800h
022DED1C 020A5490 andeq   r5,r10,90000000h
022DED20 02319438 eoreqs  r9,r1,38000000h
022DED24 020A548A andeq   r5,r10,8A000000h
022DED28 02319458 eoreqs  r9,r1,58000000h
022DED2C 02319478 eoreqs  r9,r1,78000000h
022DED30 02316A44 eoreqs  r6,r1,44000h
022DED34 00000137 andeq   r0,r0,r7,lsr r1
022DED38 00000135 andeq   r0,r0,r5,lsr r1
022DED3C 00000132 andeq   r0,r0,r2,lsr r1
022DED40 00000133 andeq   r0,r0,r3,lsr r1
022DED44 0000012D andeq   r0,r0,r13,lsr 2h
022DED48 E1D600B0 ldrh    r0,[r6]
022DED4C EB0016D6 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DED50 E1A00800 mov     r0,r0,lsl 10h
022DED54 E1A00840 mov     r0,r0,asr 10h
022DED58 EBFFF8F4 bl      22DD130h
022DED5C EA000DC4 b       22E2474h
022DED60 E1D600B0 ldrh    r0,[r6]
022DED64 EB0016D0 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DED68 E1A01800 mov     r1,r0,lsl 10h
022DED6C E284006C add     r0,r4,6Ch
022DED70 E1A01841 mov     r1,r1,asr 10h
022DED74 EBF5B195 bl      204B3D0h
022DED78 EA000DBD b       22E2474h
022DED7C E1D600B0 ldrh    r0,[r6]
022DED80 EB0016C9 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DED84 E1A01800 mov     r1,r0,lsl 10h
022DED88 E284006C add     r0,r4,6Ch
022DED8C E1A01841 mov     r1,r1,asr 10h
022DED90 EBF5B1A7 bl      204B434h
022DED94 EA000DB6 b       22E2474h
022DED98 E1D600B0 ldrh    r0,[r6]
022DED9C EB0016C2 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEDA0 E1A05000 mov     r5,r0
022DEDA4 E1D600B2 ldrh    r0,[r6,2h]
022DEDA8 EB0016BF bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEDAC E1A01805 mov     r1,r5,lsl 10h
022DEDB0 E1A02000 mov     r2,r0
022DEDB4 E284006C add     r0,r4,6Ch
022DEDB8 E1A01841 mov     r1,r1,asr 10h
022DEDBC EBF5B297 bl      204B820h
022DEDC0 EA000DAB b       22E2474h
//022DEDC4 - Jump
022DEDC4 E1D600B0 ldrh    r0,[r6]
022DEDC8 EB0016B7 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEDCC E1A07000 mov     r7,r0
022DEDD0 E1D600B2 ldrh    r0,[r6,2h]
022DEDD4 EB0016B4 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEDD8 E1A05000 mov     r5,r0
022DEDDC E1D600B4 ldrh    r0,[r6,4h]
022DEDE0 EB0016B1 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEDE4 E1A01807 mov     r1,r7,lsl 10h
022DEDE8 E1A01841 mov     r1,r1,asr 10h
022DEDEC E1A03005 mov     r3,r5
022DEDF0 E1A02000 mov     r2,r0
022DEDF4 E284006C add     r0,r4,6Ch
022DEDF8 EBF5B436 bl      204BED8h
022DEDFC EA000D9C b       22E2474h
//22DEE00 - JumpCommon
022DEE00 E1D600B0 ldrh    r0,[r6]
022DEE04 EB0016A8 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEE08 E1A07000 mov     r7,r0
022DEE0C E1D600B2 ldrh    r0,[r6,2h]
022DEE10 EB0016A5 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEE14 E1A05000 mov     r5,r0
022DEE18 E1D600B4 ldrh    r0,[r6,4h]
022DEE1C EB0016A2 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEE20 E1A01807 mov     r1,r7,lsl 10h
022DEE24 E1A01841 mov     r1,r1,asr 10h
022DEE28 E1A03005 mov     r3,r5
022DEE2C E1A02000 mov     r2,r0
022DEE30 E284006C add     r0,r4,6Ch
022DEE34 E1A02802 mov     r2,r2,lsl 10h
022DEE38 E1A02842 mov     r2,r2,asr 10h
022DEE3C EBF5B433 bl      204BF10h
022DEE40 EA000D8B b       22E2474h
//022DEE44 - 
022DEE44 E1D600B0 ldrh    r0,[r6]                   //Load next word in the script into r0
022DEE48 EB001697 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEE4C E1A07000 mov     r7,r0						//R7 = NextOpCode
022DEE50 E1D600B2 ldrh    r0,[r6,2h]                //Load the word after the next opcode into r0
022DEE54 EB001694 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEE58 E1A05000 mov     r5,r0						//R5 = SecondNextOpCode
022DEE5C E1D600B4 ldrh    r0,[r6,4h]                //Load the word 4 bytes after the next into the register r0
022DEE60 EB001691 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEE64 E1A01807 mov     r1,r7,lsl 10h				//R1 = NextOpCode << 16
022DEE68 E1A01841 mov     r1,r1,asr 10h				//R1 = R1 >> 16 (Keep Sign)
022DEE6C E1A02805 mov     r2,r5,lsl 10h				//R2 = SecondNextOpCode << 16
022DEE70 E1A02822 mov     r2,r2,lsr 10h				//R2 = R2 >> 16 (Remove sign)
022DEE74 E1A03000 mov     r3,r0						//R3 = ThirdNextOpCode
022DEE78 E284006C add     r0,r4,6Ch					//R0 = Unknown address in the script engine's struct. (Possibly return address?)
022DEE7C EBF5B2C1 bl      204B988h					//Call 0204B988, Which accessed a script variable and does something
022DEE80 EA000D7B b       22E2474h					///022E2474 - Returns 2
//
022DEE84 E1D600B0 ldrh    r0,[r6]
022DEE88 EB001687 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEE8C E1A05000 mov     r5,r0
022DEE90 E1D600B2 ldrh    r0,[r6,2h]
022DEE94 EB001684 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEE98 E1A04000 mov     r4,r0
022DEE9C E1D600B4 ldrh    r0,[r6,4h]
022DEEA0 EB001681 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEEA4 E1A03805 mov     r3,r5,lsl 10h
022DEEA8 E1A02000 mov     r2,r0
022DEEAC E1A01004 mov     r1,r4
022DEEB0 E1A00843 mov     r0,r3,asr 10h
022DEEB4 EBF5B5D7 bl      204C618h
022DEEB8 EA000D6D b       22E2474h
022DEEBC E1D600B0 ldrh    r0,[r6]
022DEEC0 EB001679 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEEC4 E1A00800 mov     r0,r0,lsl 10h
022DEEC8 E3A01000 mov     r1,0h
022DEECC E1A02001 mov     r2,r1
022DEED0 E1A00840 mov     r0,r0,asr 10h
022DEED4 EBF5B5CF bl      204C618h
022DEED8 EA000D65 b       22E2474h
022DEEDC E1D600B0 ldrh    r0,[r6]
022DEEE0 EB001671 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEEE4 E1A04000 mov     r4,r0
022DEEE8 E1D600B2 ldrh    r0,[r6,2h]
022DEEEC EB00166E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEEF0 E1A01000 mov     r1,r0
022DEEF4 E1A00004 mov     r0,r4
022DEEF8 E20110FF and     r1,r1,0FFh
022DEEFC EBF5B70A bl      204CB2Ch
022DEF00 EA000D5B b       22E2474h
022DEF04 E3A01027 mov     r1,27h
022DEF08 E284006C add     r0,r4,6Ch
022DEF0C E2412028 sub     r2,r1,28h
022DEF10 EBF5B242 bl      204B820h
022DEF14 E284006C add     r0,r4,6Ch
022DEF18 E3A0102C mov     r1,2Ch
022DEF1C E3A02000 mov     r2,0h
022DEF20 EBF5B23E bl      204B820h
022DEF24 EA000D52 b       22E2474h
022DEF28 E1D600B0 ldrh    r0,[r6]
022DEF2C EB00165E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEF30 E1A07000 mov     r7,r0
022DEF34 E1D600B2 ldrh    r0,[r6,2h]
022DEF38 EB00165B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEF3C E1A05000 mov     r5,r0
022DEF40 E1A02007 mov     r2,r7
022DEF44 E284006C add     r0,r4,6Ch
022DEF48 E3A01027 mov     r1,27h
022DEF4C EBF5B233 bl      204B820h
022DEF50 E1A02005 mov     r2,r5
022DEF54 E284006C add     r0,r4,6Ch
022DEF58 E3A0102C mov     r1,2Ch
022DEF5C EBF5B22F bl      204B820h
022DEF60 EA000D43 b       22E2474h
//0x7D - flag_SetDungeonMode
022DEF64 E1D600B0 ldrh    r0,[r6]
022DEF68 EB00164F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEF6C E1A04000 mov     r4,r0                     //R4 = Param0_14b
022DEF70 E1D600B2 ldrh    r0,[r6,2h]
022DEF74 EB00164C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEF78 E1A02804 mov     r2,r4,lsl 10h             //R2 = Param0_14b << 16
022DEF7C E1A01000 mov     r1,r0                     //R1 = Param1_14b
022DEF80 E1A00842 mov     r0,r2,asr 10h             //R0 = (Param0_14b << 16) >> 16 (signed shift) (Probably turns signed short into signed int)
022DEF84 EBF5B823 bl      204D018h                  ///0204D018
{///0204D018
    0204D018 E92D4010 push    r4,r14
    0204D01C E3510003 cmp     r1,3h
    if( Param1 <= 3 )
        0204D020 908FF101 addls   r15,r15,r1,lsl 2h // R15 = 0204D024 + (Param1 * 4) + 4
    0204D024 E8BD8010 pop     r4,r15                //Param1 > 3
    0204D028 EA000002 b       204D038h              //0 -> 0204D038
    0204D02C EA00000E b       204D06Ch              //1 -> 0204D06C
    0204D030 EA00001A b       204D0A0h              //2 -> 0204D0A0
    0204D034 EA000026 b       204D0D4h              //3 -> 0204D0D4
    //Param1 == 0
    0204D038 E1A00800 mov     r0,r0,lsl 10h         //R0 = Param0 << 16
    0204D03C E1A04820 mov     r4,r0,lsr 10h         //R4 = Param0 >> 16
    0204D040 E3A00000 mov     r0,0h                 //R0 = 0
    0204D044 E1A02004 mov     r2,r4                 //R2 = ((Param0 <<16) >> 16)
    0204D048 E1A03000 mov     r3,r0                 //R3 = 0
    0204D04C E3A0104F mov     r1,4Fh                //R1 = 0x4F
    0204D050 EBFFFA4C bl      204B988h              ///0204B988
    {///0204B988
        0204B988 E92D4070 push    r4-r6,r14
        0204B98C E24DD008 sub     r13,r13,8h
        0204B990 E1A04000 mov     r4,r0             //R4 = 0 
        0204B994 E1A06001 mov     r6,r1             //R6 = 0x4F
        0204B998 E1A05002 mov     r5,r2             //R5 = ((Param0 <<16) >> 16)
        0204B99C E1A01004 mov     r1,r4             //R1 = ((Param0 <<16) >> 16)
        0204B9A0 E28D0000 add     r0,r13,0h         //R0 = stackptr
        0204B9A4 E1A02006 mov     r2,r6             //R2 = 0x4F
        0204B9A8 E1A04003 mov     r4,r3             //R4 = 0
        0204B9AC EBFFFEBA bl      204B49Ch          ///GetScriptVariableInfo( ptrvarinfoentry, ptrvartablebeg, variableid )
        0204B9B0 E59D1000 ldr     r1,[r13]          //R1 = PtrScriptVarInfo
        0204B9B4 E1D100F0 ldrsh   r0,[r1]           //R0 = VariableType
        0204B9B8 E3500009 cmp     r0,9h             
        if( VariableType <= 9 )
            0204B9BC 908FF100 addls   r15,r15,r0,lsl 2h //R15 = 0204B9C0 + (VariableType * 4) + 4
        0204B9C0 EA00004C b       204BAF8h              //VariableType > 9
        0204B9C4 EA00004B b       204BAF8h              //0
        0204B9C8 EA000007 b       204B9ECh              //1
        0204B9CC EA000017 b       204BA30h              //2
        0204B9D0 EA000016 b       204BA30h              //3
        0204B9D4 EA000018 b       204BA3Ch              //4
        0204B9D8 EA00001A b       204BA48h              //5
        0204B9DC EA00001D b       204BA58h              //6
        0204B9E0 EA000020 b       204BA68h              //7
        0204B9E4 EA00001F b       204BA68h              //8
        0204B9E8 EA000021 b       204BA74h              //9
        0204B9EC E1D100F6 ldrsh   r0,[r1,6h]
        0204B9F0 E3A01001 mov     r1,1h
        0204B9F4 E59D2004 ldr     r2,[r13,4h]
        0204B9F8 E0850000 add     r0,r5,r0
        0204B9FC E1A00800 mov     r0,r0,lsl 10h
        0204BA00 E1A03820 mov     r3,r0,lsr 10h
        0204BA04 E2030007 and     r0,r3,7h
        0204BA08 E1A00011 mov     r0,r1,lsl r0
        0204BA0C E20010FF and     r1,r0,0FFh
        0204BA10 E7D201A3 ldrb    r0,[r2,r3,lsr 3h]
        0204BA14 E3540000 cmp     r4,0h
        0204BA18 11800001 orrne   r0,r0,r1
        0204BA1C 17C201A3 strneb  r0,[r2,r3,lsr 3h]
        0204BA20 01800001 orreq   r0,r0,r1
        0204BA24 00210000 eoreq   r0,r1,r0
        0204BA28 07C201A3 streqb  r0,[r2,r3,lsr 3h]
        0204BA2C EA000031 b       204BAF8h
        0204BA30 E59D0004 ldr     r0,[r13,4h]
        0204BA34 E7C04005 strb    r4,[r0,r5]
        0204BA38 EA00002E b       204BAF8h
        0204BA3C E59D0004 ldr     r0,[r13,4h]
        0204BA40 E7C04005 strb    r4,[r0,r5]
        0204BA44 EA00002B b       204BAF8h
        0204BA48 E59D1004 ldr     r1,[r13,4h]
        0204BA4C E1A00085 mov     r0,r5,lsl 1h
        0204BA50 E18140B0 strh    r4,[r1,r0]
        0204BA54 EA000027 b       204BAF8h
        0204BA58 E59D1004 ldr     r1,[r13,4h]
        0204BA5C E1A00085 mov     r0,r5,lsl 1h
        0204BA60 E18140B0 strh    r4,[r1,r0]
        0204BA64 EA000023 b       204BAF8h
        0204BA68 E59D0004 ldr     r0,[r13,4h]
        0204BA6C E7804105 str     r4,[r0,r5,lsl 2h]
        0204BA70 EA000020 b       204BAF8h
        0204BA74 E3560049 cmp     r6,49h
        0204BA78 CA000008 bgt     204BAA0h
        0204BA7C AA000012 bge     204BACCh
        0204BA80 E356003D cmp     r6,3Dh
        0204BA84 CA00001B bgt     204BAF8h
        0204BA88 E356003C cmp     r6,3Ch
        0204BA8C BA000019 blt     204BAF8h
        0204BA90 0A000005 beq     204BAACh
        0204BA94 E356003D cmp     r6,3Dh
        0204BA98 0A000008 beq     204BAC0h
        0204BA9C EA000015 b       204BAF8h
        0204BAA0 E3560070 cmp     r6,70h
        0204BAA4 0A00000E beq     204BAE4h
        0204BAA8 EA000012 b       204BAF8h
        0204BAAC E1A00004 mov     r0,r4
        0204BAB0 EBFF0C99 bl      200ED1Ch
        0204BAB4 E3A00000 mov     r0,0h
        0204BAB8 EBFF0CA6 bl      200ED58h
        0204BABC EA00000D b       204BAF8h
        0204BAC0 E1A00004 mov     r0,r4
        0204BAC4 EBFF1316 bl      2010724h
        0204BAC8 EA00000A b       204BAF8h
        0204BACC EBFFFD3B bl      204AFC0h
        0204BAD0 E3500001 cmp     r0,1h
        0204BAD4 1A000007 bne     204BAF8h
        0204BAD8 E1A00004 mov     r0,r4
        0204BADC EB000391 bl      204C928h      //0204C928
        {
            0204C928 E59F1004 ldr     r1,=22AB4ACh  //022AB4AC Address near the game flags/script variables
            0204C92C E5810000 str     r0,[r1]
            0204C930 E12FFF1E bx      r14
            0204C934 022AB4AC
        }
        0204BAE0 EA000004 b       204BAF8h      //0204BAF8
        0204BAE4 E3540000 cmp     r4,0h
        0204BAE8 13A00001 movne   r0,1h
        0204BAEC 03A00000 moveq   r0,0h
        0204BAF0 E20000FF and     r0,r0,0FFh
        0204BAF4 EBFFF26D bl      20484B0h
        0204BAF8 E28DD008 add     r13,r13,8h
        0204BAFC E8BD8070 pop     r4-r6,r15
    }
    0204D054 E3A00000 mov     r0,0h         //R0 = 0
    0204D058 E1A02004 mov     r2,r4         //R2 = ((Param0 <<16) >> 16)
    0204D05C E1A03000 mov     r3,r0         //R3 = 0
    0204D060 E3A01054 mov     r1,54h        //R1 = 0x54    
    0204D064 EBFFFA47 bl      204B988h
    0204D068 E8BD8010 pop     r4,r15
    //Param1 == 1
    0204D06C E1A00800 mov     r0,r0,lsl 10h
    0204D070 E1A04820 mov     r4,r0,lsr 10h
    0204D074 E1A02004 mov     r2,r4
    0204D078 E3A00000 mov     r0,0h
    0204D07C E3A0104F mov     r1,4Fh
    0204D080 E3A03001 mov     r3,1h
    0204D084 EBFFFA3F bl      204B988h
    0204D088 E3A00000 mov     r0,0h
    0204D08C E1A02004 mov     r2,r4
    0204D090 E1A03000 mov     r3,r0
    0204D094 E3A01054 mov     r1,54h
    0204D098 EBFFFA3A bl      204B988h
    0204D09C E8BD8010 pop     r4,r15
    ////Param1 == 2
    0204D0A0 E1A00800 mov     r0,r0,lsl 10h
    0204D0A4 E1A04820 mov     r4,r0,lsr 10h
    0204D0A8 E3A00000 mov     r0,0h
    0204D0AC E1A02004 mov     r2,r4
    0204D0B0 E1A03000 mov     r3,r0
    0204D0B4 E3A0104F mov     r1,4Fh
    0204D0B8 EBFFFA32 bl      204B988h
    0204D0BC E1A02004 mov     r2,r4
    0204D0C0 E3A00000 mov     r0,0h
    0204D0C4 E3A01054 mov     r1,54h
    0204D0C8 E3A03001 mov     r3,1h
    0204D0CC EBFFFA2D bl      204B988h
    0204D0D0 E8BD8010 pop     r4,r15
    ////Param1 == 3
    0204D0D4 E1A00800 mov     r0,r0,lsl 10h
    0204D0D8 E1A04820 mov     r4,r0,lsr 10h
    0204D0DC E1A02004 mov     r2,r4
    0204D0E0 E3A00000 mov     r0,0h
    0204D0E4 E3A0104F mov     r1,4Fh
    0204D0E8 E3A03001 mov     r3,1h
    0204D0EC EBFFFA25 bl      204B988h
    0204D0F0 E1A02004 mov     r2,r4
    0204D0F4 E3A00000 mov     r0,0h
    0204D0F8 E3A01054 mov     r1,54h
    0204D0FC E3A03001 mov     r3,1h
    0204D100 EBFFFA20 bl      204B988h
    0204D104 E8BD8010 pop     r4,r15
}
022DEF88 EA000D39 b       22E2474h                  ///022E2474 - Returns 2

022DEF8C E1D600B0 ldrh    r0,[r6]
022DEF90 EB001645 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEF94 E1A04000 mov     r4,r0
022DEF98 E3540FFA cmp     r4,3E8h
022DEF9C BA000007 blt     22DEFC0h
022DEFA0 E2440FFA sub     r0,r4,3E8h
022DEFA4 E3A01001 mov     r1,1h
022DEFA8 EBF5B6A6 bl      204CA48h
022DEFAC E59F0E54 ldr     r0,=2316B0Ch
022DEFB0 E2441FFA sub     r1,r4,3E8h
022DEFB4 E7D00001 ldrb    r0,[r0,r1]
022DEFB8 EBF5C2F7 bl      204FB9Ch
022DEFBC EA000D2C b       22E2474h
022DEFC0 E59F0E44 ldr     r0,=2316CD0h
022DEFC4 E7D00004 ldrb    r0,[r0,r4]
022DEFC8 EBF5C2F3 bl      204FB9Ch
022DEFCC EA000D28 b       22E2474h
///
//R6 = Address next word (After current opcode)
022DEFD0 E1D600B0 ldrh    r0,[r6]           //Load Next Word into R0
022DEFD4 EB001634 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned          ///EXEC Fun_022E48AC ( r0 ) (Turn 16bits signed into 14bits unsigned)
022DEFD8 E1A05000 mov     r5,r0
022DEFDC E1D600B2 ldrh    r0,[r6,2h]
022DEFE0 EB001631 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DEFE4 E1A01805 mov     r1,r5,lsl 10h
022DEFE8 E1A02000 mov     r2,r0
022DEFEC E284006C add     r0,r4,6Ch
022DEFF0 E1A01841 mov     r1,r1,asr 10h
022DEFF4 E3A03002 mov     r3,2h
022DEFF8 EBF5B3DA bl      204BF68h
022DEFFC E3500000 cmp     r0,0h
022DF000 11D600B4 ldrneh  r0,[r6,4h]
022DF004 15941014 ldrne   r1,[r4,14h]
022DF008 10810080 addne   r0,r1,r0,lsl 1h
022DF00C 1584001C strne   r0,[r4,1Ch]
022DF010 EA000D17 b       22E2474h
022DF014 E1D600B0 ldrh    r0,[r6]
022DF018 EB001623 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF01C E1A07000 mov     r7,r0
022DF020 E1D600B2 ldrh    r0,[r6,2h]
022DF024 EB001620 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF028 E1A05000 mov     r5,r0
022DF02C E1D600B4 ldrh    r0,[r6,4h]
022DF030 EB00161D bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF034 E1A01807 mov     r1,r7,lsl 10h
022DF038 E1A02000 mov     r2,r0
022DF03C E1A01841 mov     r1,r1,asr 10h
022DF040 E1A03005 mov     r3,r5
022DF044 E284006C add     r0,r4,6Ch
022DF048 EBF5B3C6 bl      204BF68h
022DF04C E3500000 cmp     r0,0h
022DF050 11D600B6 ldrneh  r0,[r6,6h]
022DF054 15941014 ldrne   r1,[r4,14h]
022DF058 10810080 addne   r0,r1,r0,lsl 1h
022DF05C 1584001C strne   r0,[r4,1Ch]
022DF060 EA000D03 b       22E2474h
022DF064 E1D600B0 ldrh    r0,[r6]
022DF068 EB00160F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF06C E1A07000 mov     r7,r0
022DF070 E1D600B2 ldrh    r0,[r6,2h]
022DF074 EB00160C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF078 E1A05000 mov     r5,r0
022DF07C E1D600B4 ldrh    r0,[r6,4h]
022DF080 EB001609 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF084 E1A01807 mov     r1,r7,lsl 10h
022DF088 E1A00800 mov     r0,r0,lsl 10h
022DF08C E1A02840 mov     r2,r0,asr 10h
022DF090 E1A01841 mov     r1,r1,asr 10h
022DF094 E1A03005 mov     r3,r5
022DF098 E284006C add     r0,r4,6Ch
022DF09C EBF5B3B9 bl      204BF88h
022DF0A0 E3500000 cmp     r0,0h
022DF0A4 11D600B6 ldrneh  r0,[r6,6h]
022DF0A8 15941014 ldrne   r1,[r4,14h]
022DF0AC 10810080 addne   r0,r1,r0,lsl 1h
022DF0B0 1584001C strne   r0,[r4,1Ch]
022DF0B4 EA000CEE b       22E2474h
022DF0B8 E1D600B0 ldrh    r0,[r6]
022DF0BC EB0015FA bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF0C0 E1A05000 mov     r5,r0
022DF0C4 E1D600B2 ldrh    r0,[r6,2h]
022DF0C8 EB0015F7 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF0CC E1A02800 mov     r2,r0,lsl 10h
022DF0D0 E1A01805 mov     r1,r5,lsl 10h
022DF0D4 E284006C add     r0,r4,6Ch
022DF0D8 E1A01841 mov     r1,r1,asr 10h
022DF0DC E1A02822 mov     r2,r2,lsr 10h
022DF0E0 EBF5B164 bl      204B678h
022DF0E4 E3500000 cmp     r0,0h
022DF0E8 11D600B4 ldrneh  r0,[r6,4h]
022DF0EC 15941014 ldrne   r1,[r4,14h]
022DF0F0 10810080 addne   r0,r1,r0,lsl 1h
022DF0F4 1584001C strne   r0,[r4,1Ch]
022DF0F8 EA000CDD b       22E2474h
022DF0FC E1D600B0 ldrh    r0,[r6]
022DF100 EB0015E9 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF104 E1A05000 mov     r5,r0
022DF108 E1D600B2 ldrh    r0,[r6,2h]
022DF10C EB0015E6 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF110 E1A07000 mov     r7,r0
022DF114 E1D600B4 ldrh    r0,[r6,4h]
022DF118 EB0015E3 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF11C E1A01805 mov     r1,r5,lsl 10h
022DF120 E1A05000 mov     r5,r0
022DF124 E1A01841 mov     r1,r1,asr 10h
022DF128 E284006C add     r0,r4,6Ch
022DF12C EBF5B273 bl      204BB00h
022DF130 E1A01005 mov     r1,r5
022DF134 E1A02007 mov     r2,r7
022DF138 EBF5B387 bl      204BF5Ch
022DF13C E3500000 cmp     r0,0h
022DF140 11D600B6 ldrneh  r0,[r6,6h]
022DF144 15941014 ldrne   r1,[r4,14h]
022DF148 10810080 addne   r0,r1,r0,lsl 1h
022DF14C 1584001C strne   r0,[r4,1Ch]
022DF150 EA000CC7 b       22E2474h
022DF154 E1D600B0 ldrh    r0,[r6]
022DF158 EB0015D3 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF15C E1A07000 mov     r7,r0
022DF160 E1D600B2 ldrh    r0,[r6,2h]
022DF164 EB0015D0 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF168 E1A05000 mov     r5,r0
022DF16C E1D600B4 ldrh    r0,[r6,4h]
022DF170 EB0015CD bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF174 E1A01807 mov     r1,r7,lsl 10h
022DF178 E1A02000 mov     r2,r0
022DF17C E1A00841 mov     r0,r1,asr 10h
022DF180 E1A01005 mov     r1,r5
022DF184 EBF5B54F bl      204C6C8h
022DF188 E3500000 cmp     r0,0h
022DF18C 11D600B6 ldrneh  r0,[r6,6h]
022DF190 15941014 ldrne   r1,[r4,14h]
022DF194 10810080 addne   r0,r1,r0,lsl 1h
022DF198 1584001C strne   r0,[r4,1Ch]
022DF19C EA000CB4 b       22E2474h
022DF1A0 E1D600B0 ldrh    r0,[r6]
022DF1A4 EB0015C0 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF1A8 E1A07000 mov     r7,r0
022DF1AC E1D600B2 ldrh    r0,[r6,2h]
022DF1B0 EB0015BD bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF1B4 E1A05000 mov     r5,r0
022DF1B8 E1D600B4 ldrh    r0,[r6,4h]
022DF1BC EB0015BA bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF1C0 E1A01807 mov     r1,r7,lsl 10h
022DF1C4 E1A02000 mov     r2,r0
022DF1C8 E1A00841 mov     r0,r1,asr 10h
022DF1CC E1A01005 mov     r1,r5
022DF1D0 EBF5B557 bl      204C734h
022DF1D4 E3500000 cmp     r0,0h
022DF1D8 11D600B6 ldrneh  r0,[r6,6h]
022DF1DC 15941014 ldrne   r1,[r4,14h]
022DF1E0 10810080 addne   r0,r1,r0,lsl 1h
022DF1E4 1584001C strne   r0,[r4,1Ch]
022DF1E8 EA000CA1 b       22E2474h
022DF1EC E1D600B0 ldrh    r0,[r6]
022DF1F0 EB0015AD bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF1F4 E1A07000 mov     r7,r0
022DF1F8 E1D600B2 ldrh    r0,[r6,2h]
022DF1FC EB0015AA bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF200 E1A05000 mov     r5,r0
022DF204 E1D600B4 ldrh    r0,[r6,4h]
022DF208 EB0015A7 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF20C E1A01807 mov     r1,r7,lsl 10h
022DF210 E1A02000 mov     r2,r0
022DF214 E1A00841 mov     r0,r1,asr 10h
022DF218 E1A01005 mov     r1,r5
022DF21C EBF5B562 bl      204C7ACh
022DF220 E3500000 cmp     r0,0h
022DF224 11D600B6 ldrneh  r0,[r6,6h]
022DF228 15941014 ldrne   r1,[r4,14h]
022DF22C 10810080 addne   r0,r1,r0,lsl 1h
022DF230 1584001C strne   r0,[r4,1Ch]
022DF234 EA000C8E b       22E2474h
022DF238 E1D600B0 ldrh    r0,[r6]
022DF23C EB00159A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF240 E1A07000 mov     r7,r0
022DF244 E1D600B2 ldrh    r0,[r6,2h]
022DF248 EB001597 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF24C E1A05000 mov     r5,r0
022DF250 E1D600B4 ldrh    r0,[r6,4h]
022DF254 EB001594 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF258 E1A01807 mov     r1,r7,lsl 10h
022DF25C E1A02000 mov     r2,r0
022DF260 E1A00841 mov     r0,r1,asr 10h
022DF264 E1A01005 mov     r1,r5
022DF268 EBF5B566 bl      204C808h
022DF26C E3500000 cmp     r0,0h
022DF270 11D600B6 ldrneh  r0,[r6,6h]
022DF274 15941014 ldrne   r1,[r4,14h]
022DF278 10810080 addne   r0,r1,r0,lsl 1h
022DF27C 1584001C strne   r0,[r4,1Ch]
022DF280 EA000C7B b       22E2474h
022DF284 E1D600B0 ldrh    r0,[r6]
022DF288 EB001587 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF28C E1A07000 mov     r7,r0
022DF290 E1D600B2 ldrh    r0,[r6,2h]
022DF294 EB001584 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF298 E1A05000 mov     r5,r0
022DF29C E1D600B4 ldrh    r0,[r6,4h]
022DF2A0 EB001581 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF2A4 E1A01807 mov     r1,r7,lsl 10h
022DF2A8 E1A02000 mov     r2,r0
022DF2AC E1A00841 mov     r0,r1,asr 10h
022DF2B0 E1A01005 mov     r1,r5
022DF2B4 EBF5B571 bl      204C880h
022DF2B8 E3500000 cmp     r0,0h
022DF2BC 11D600B6 ldrneh  r0,[r6,6h]
022DF2C0 15941014 ldrne   r1,[r4,14h]
022DF2C4 10810080 addne   r0,r1,r0,lsl 1h
022DF2C8 1584001C strne   r0,[r4,1Ch]
022DF2CC EA000C68 b       22E2474h
022DF2D0 E1D600B0 ldrh    r0,[r6]
022DF2D4 EB001574 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF2D8 E1A07000 mov     r7,r0
022DF2DC E1D600B2 ldrh    r0,[r6,2h]
022DF2E0 EB001571 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF2E4 E1A05000 mov     r5,r0
022DF2E8 E1A00007 mov     r0,r7
022DF2EC EBF5B5E8 bl      204CA94h
022DF2F0 E1550000 cmp     r5,r0
022DF2F4 01D600B4 ldreqh  r0,[r6,4h]
022DF2F8 05941014 ldreq   r1,[r4,14h]
022DF2FC 00810080 addeq   r0,r1,r0,lsl 1h
022DF300 0584001C streq   r0,[r4,1Ch]
022DF304 EA000C5A b       22E2474h
022DF308 E1D600B0 ldrh    r0,[r6]
022DF30C EB001566 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF310 E3500000 cmp     r0,0h
022DF314 01D600B2 ldreqh  r0,[r6,2h]
022DF318 05941014 ldreq   r1,[r4,14h]
022DF31C 00810080 addeq   r0,r1,r0,lsl 1h
022DF320 0584001C streq   r0,[r4,1Ch]
022DF324 EA000C52 b       22E2474h
022DF328 E1D600B0 ldrh    r0,[r6]
022DF32C EB00155E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF330 E3500000 cmp     r0,0h
022DF334 01D600B2 ldreqh  r0,[r6,2h]
022DF338 05941014 ldreq   r1,[r4,14h]
022DF33C 00810080 addeq   r0,r1,r0,lsl 1h
022DF340 0584001C streq   r0,[r4,1Ch]
022DF344 EA000C4A b       22E2474h
022DF348 E1D600B0 ldrh    r0,[r6]
022DF34C EB001556 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF350 E1A05000 mov     r5,r0
022DF354 E3A0000B mov     r0,0Bh
022DF358 EBF4B36C bl      200C110h
022DF35C E1550000 cmp     r5,r0
022DF360 01D600B2 ldreqh  r0,[r6,2h]
022DF364 05941014 ldreq   r1,[r4,14h]
022DF368 00810080 addeq   r0,r1,r0,lsl 1h
022DF36C 0584001C streq   r0,[r4,1Ch]
022DF370 EA000C3F b       22E2474h
///---------------------------- Handle all 12 SWITCH statements!!! --------------------
//R0 was set to 0x172 before landing here
022DF374 E2400033 sub     r0,r0,33h             //R0 = 0x172 - 0x33 => 0x13F 
022DF378 E0450000 sub     r0,r5,r0              //R5 = CurrentOpCode - 0x13F
022DF37C E350000C cmp     r0,0Ch                
if( R5 < 0xC )
    022DF380 908FF100 addls   r15,r15,r0,lsl 2h //R15 = 022DF384 + (R0 * 4) + 4
else
	022DF384 EA0000D3 b       22DF6D8h			//(R5 > 0xC) 022DF6D8

022DF388 EA00000B b       22DF3BCh              //0x13F Switch statement 		022DF3BC
022DF38C EA000063 b       22DF520h				//0x140 SwitchDirection 		022DF520
022DF390 EA000081 b       22DF59Ch				//0x141 SwitchDirectionLives 	022DF59C
022DF394 EA0000A9 b       22DF640h				//0x142 SwitchDirectionLives2 	022DF640
022DF398 EA000067 b       22DF53Ch				//0x143 SwitchDirectionMark 	022DF53C
022DF39C EA000051 b       22DF4E8h				//0x144 SwitchDungeonMode 		022DF4E8
022DF3A0 EA000057 b       22DF504h				//0x145 SwitchLives 			022DF504
022DF3A4 EA000036 b       22DF484h				//0x146 SwitchRandom 			022DF484
022DF3A8 EA00003C b       22DF4A0h				//0x147 SwitchScenario 			022DF4A0
022DF3AC EA000044 b       22DF4C4h				//0x148 SwitchScenarioLevel 	022DF4C4
022DF3B0 EA000038 b       22DF498h				//0x149 SwitchSector 			022DF498
022DF3B4 EA000008 b       22DF3DCh				//0x14A SwitchValue 			022DF3DC
022DF3B8 EA000019 b       22DF424h				//0x14B SwitchVariable 			022DF424
///----------------------------
//0x13F Switch statement
022DF3BC E1D600B0 ldrh    r0,[r6]                   //Load parameter Parameter
022DF3C0 EB001539 bl      22E48ACh                  //Turn 16bits signed Parameter into 14bits unsigned
022DF3C4 E1A01800 mov     r1,r0,lsl 10h             //R1 = Parameter14b << 16
022DF3C8 E284006C add     r0,r4,6Ch                 //R0 = ScriptEngineStruct + 0x6C (108 bytes)
022DF3CC E1A01841 mov     r1,r1,asr 10h             //R1 = (Parameter14b << 16) >> 16(signed shift right) (basically turn it into a signed int32)
022DF3D0 EBF5B045 bl      204B4ECh                  ///0204B4EC GetScriptVariableValue (R0 = (ScriptEngineStruct+108), R1 = (Parameter14b << 16) >> 16(signed shift right), R2 =  )
022DF3D4 E1A07000 mov     r7,r0                     //R7 = ScriptVariableValue
022DF3D8 EA0000C7 b       22DF6FCh                  ///022DF6FC SwitchStatementHandling
//0x14A SwitchValue
022DF3DC E1D600B0 ldrh    r0,[r6]
022DF3E0 EB001531 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF3E4 E1A05000 mov     r5,r0
022DF3E8 E1D600B2 ldrh    r0,[r6,2h]
022DF3EC EB00152E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF3F0 E1A07000 mov     r7,r0
022DF3F4 E1D600B4 ldrh    r0,[r6,4h]
022DF3F8 EB00152B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF3FC E1A01805 mov     r1,r5,lsl 10h
022DF400 E1A05000 mov     r5,r0
022DF404 E284006C add     r0,r4,6Ch
022DF408 E1A01841 mov     r1,r1,asr 10h
022DF40C EBF5B036 bl      204B4ECh
022DF410 E1A01005 mov     r1,r5
022DF414 E1A02007 mov     r2,r7
022DF418 EBF5B2AB bl      204BECCh
022DF41C E1A07000 mov     r7,r0
022DF420 EA0000B5 b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x14B SwitchVariable
022DF424 E1D600B0 ldrh    r0,[r6]
022DF428 EB00151F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF42C E1A05000 mov     r5,r0
022DF430 E1D600B2 ldrh    r0,[r6,2h]
022DF434 EB00151C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF438 E1A07000 mov     r7,r0
022DF43C E1D600B4 ldrh    r0,[r6,4h]
022DF440 EB001519 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF444 E1A01805 mov     r1,r5,lsl 10h
022DF448 E1A05000 mov     r5,r0
022DF44C E284006C add     r0,r4,6Ch
022DF450 E1A01841 mov     r1,r1,asr 10h
022DF454 EBF5B024 bl      204B4ECh
022DF458 E1A01805 mov     r1,r5,lsl 10h
022DF45C E1A01841 mov     r1,r1,asr 10h
022DF460 E1A05000 mov     r5,r0
022DF464 E284006C add     r0,r4,6Ch
022DF468 EBF5B01F bl      204B4ECh
022DF46C E1A02007 mov     r2,r7
022DF470 E1A01000 mov     r1,r0
022DF474 E1A00005 mov     r0,r5
022DF478 EBF5B293 bl      204BECCh
022DF47C E1A07000 mov     r7,r0
022DF480 EA00009D b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x146 SwitchRandom
022DF484 E1D600B0 ldrh    r0,[r6]
022DF488 EB001507 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF48C EBF48B78 bl      2002274h
022DF490 E1A07000 mov     r7,r0
022DF494 EA000098 b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x149 SwitchSector
022DF498 E1D471D2 ldrsb   r7,[r4,12h]
022DF49C EA000096 b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x147 SwitchScenario
022DF4A0 E1D600B0 ldrh    r0,[r6]
022DF4A4 EB001500 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF4A8 E1A01800 mov     r1,r0,lsl 10h
022DF4AC E3A00000 mov     r0,0h
022DF4B0 E1A02000 mov     r2,r0
022DF4B4 E1A01841 mov     r1,r1,asr 10h
022DF4B8 EBF5B06E bl      204B678h
022DF4BC E1A07000 mov     r7,r0
022DF4C0 EA00008D b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x148 SwitchScenarioLevel
022DF4C4 E1D600B0 ldrh    r0,[r6]
022DF4C8 EB0014F7 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF4CC E1A00800 mov     r0,r0,lsl 10h
022DF4D0 E1A01840 mov     r1,r0,asr 10h
022DF4D4 E3A00000 mov     r0,0h
022DF4D8 E3A02001 mov     r2,1h
022DF4DC EBF5B065 bl      204B678h
022DF4E0 E1A07000 mov     r7,r0
022DF4E4 EA000084 b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x144 SwitchDungeonMode
022DF4E8 E1D600B0 ldrh    r0,[r6]
022DF4EC EB0014EE bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF4F0 E1A00800 mov     r0,r0,lsl 10h
022DF4F4 E1A00840 mov     r0,r0,asr 10h
022DF4F8 EBF5B6A7 bl      204CF9Ch
022DF4FC E1A07000 mov     r7,r0
022DF500 EA00007D b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x145 SwitchLives
022DF504 E1D600B0 ldrh    r0,[r6]
022DF508 EB0014E7 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF50C E1A00800 mov     r0,r0,lsl 10h
022DF510 E1A00840 mov     r0,r0,asr 10h
022DF514 EBF61916 bl      2065974h
022DF518 E1A07000 mov     r7,r0
022DF51C EA000076 b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x140 SwitchDirection
022DF520 E5941000 ldr     r1,[r4]
022DF524 E5940004 ldr     r0,[r4,4h]
022DF528 E5912014 ldr     r2,[r1,14h]
022DF52C E28D100F add     r1,r13,0Fh
022DF530 E12FFF32 blx     r2
022DF534 E1DD70DF ldrsb   r7,[r13,0Fh]
022DF538 EA00006F b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x143 SwitchDirectionMark
022DF53C E5941000 ldr     r1,[r4]
022DF540 E5940004 ldr     r0,[r4,4h]
022DF544 E591200C ldr     r2,[r1,0Ch]
022DF548 E28D1F4A add     r1,r13,128h
022DF54C E12FFF32 blx     r2
022DF550 E59D3128 ldr     r3,[r13,128h]
022DF554 E59D212C ldr     r2,[r13,12Ch]
022DF558 E28D1E12 add     r1,r13,120h
022DF55C E1A00006 mov     r0,r6
022DF560 E58D3120 str     r3,[r13,120h]
022DF564 E58D2124 str     r2,[r13,124h]
022DF568 EB0014E7 bl      22E490Ch
022DF56C E59D3120 ldr     r3,[r13,120h]
022DF570 E59D2128 ldr     r2,[r13,128h]
022DF574 E59D1124 ldr     r1,[r13,124h]
022DF578 E59D012C ldr     r0,[r13,12Ch]
022DF57C E0432002 sub     r2,r3,r2
022DF580 E0411000 sub     r1,r1,r0
022DF584 E28D0F46 add     r0,r13,118h
022DF588 E58D2118 str     r2,[r13,118h]
022DF58C E58D111C str     r1,[r13,11Ch]
022DF590 EBF4980C bl      20055C8h
022DF594 E1A07000 mov     r7,r0
022DF598 EA000057 b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x141 SwitchDirectionLives
022DF59C E1D600B0 ldrh    r0,[r6]
022DF5A0 EB0014C1 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF5A4 E1A00800 mov     r0,r0,lsl 10h
022DF5A8 E1A00840 mov     r0,r0,asr 10h
022DF5AC EB006217 bl      22F7E10h
022DF5B0 E1B05000 movs    r5,r0
022DF5B4 4A00001F bmi     22DF638h
022DF5B8 E5941000 ldr     r1,[r4]
022DF5BC E5940004 ldr     r0,[r4,4h]
022DF5C0 E591200C ldr     r2,[r1,0Ch]
022DF5C4 E28D1E11 add     r1,r13,110h
022DF5C8 E12FFF32 blx     r2
022DF5CC E5941000 ldr     r1,[r4]
022DF5D0 E5940004 ldr     r0,[r4,4h]
022DF5D4 E5912008 ldr     r2,[r1,8h]
022DF5D8 E28D1C01 add     r1,r13,100h
022DF5DC E12FFF32 blx     r2
022DF5E0 E28D1F42 add     r1,r13,108h
022DF5E4 E1A00005 mov     r0,r5
022DF5E8 EB0066C1 bl      22F90F4h
022DF5EC E1A00005 mov     r0,r5
022DF5F0 E28D10F8 add     r1,r13,0F8h
022DF5F4 EB0066AE bl      22F90B4h
022DF5F8 E28D0E11 add     r0,r13,110h
022DF5FC E28D1C01 add     r1,r13,100h
022DF600 E28D2F42 add     r2,r13,108h
022DF604 E28D30F8 add     r3,r13,0F8h
022DF608 EBF498A6 bl      20058A8h
022DF60C E1A07000 mov     r7,r0
022DF610 E3E00000 mvn     r0,0h
022DF614 E1570000 cmp     r7,r0
022DF618 1A000037 bne     22DF6FCh					///022DF6FC SwitchStatementHandling
022DF61C E59F17EC ldr     r1,=2316A60h
022DF620 E28D0E11 add     r0,r13,110h
022DF624 E28D2F42 add     r2,r13,108h
022DF628 E1A03001 mov     r3,r1
022DF62C EBF4985D bl      20057A8h
022DF630 E1A07000 mov     r7,r0
022DF634 EA000030 b       22DF6FCh					///022DF6FC SwitchStatementHandling
022DF638 E3E07000 mvn     r7,0h
022DF63C EA00002E b       22DF6FCh					///022DF6FC SwitchStatementHandling
//0x142 SwitchDirectionLives2
022DF640 E1D600B0 ldrh    r0,[r6]
022DF644 EB001498 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF648 E1A00800 mov     r0,r0,lsl 10h
022DF64C E1A00840 mov     r0,r0,asr 10h
022DF650 EB0061EE bl      22F7E10h
022DF654 E1B05000 movs    r5,r0
022DF658 4A00001C bmi     22DF6D0h
022DF65C E5941000 ldr     r1,[r4]
022DF660 E5940004 ldr     r0,[r4,4h]
022DF664 E591200C ldr     r2,[r1,0Ch]
022DF668 E28D10F0 add     r1,r13,0F0h
022DF66C E12FFF32 blx     r2
022DF670 E5941000 ldr     r1,[r4]
022DF674 E5940004 ldr     r0,[r4,4h]
022DF678 E5912008 ldr     r2,[r1,8h]
022DF67C E28D10E0 add     r1,r13,0E0h
022DF680 E12FFF32 blx     r2
022DF684 E28D10E8 add     r1,r13,0E8h
022DF688 E1A00005 mov     r0,r5
022DF68C EB006698 bl      22F90F4h
022DF690 E59F3778 ldr     r3,=2316A60h
022DF694 E28D00F0 add     r0,r13,0F0h
022DF698 E28D10E0 add     r1,r13,0E0h
022DF69C E28D20E8 add     r2,r13,0E8h
022DF6A0 EBF49880 bl      20058A8h
022DF6A4 E1A07000 mov     r7,r0
022DF6A8 E3E00000 mvn     r0,0h
022DF6AC E1570000 cmp     r7,r0
022DF6B0 1A000011 bne     22DF6FCh					///022DF6FC SwitchStatementHandling
022DF6B4 E59F1754 ldr     r1,=2316A60h
022DF6B8 E28D00F0 add     r0,r13,0F0h
022DF6BC E28D20E8 add     r2,r13,0E8h
022DF6C0 E1A03001 mov     r3,r1
022DF6C4 EBF49837 bl      20057A8h
022DF6C8 E1A07000 mov     r7,r0
022DF6CC EA00000A b       22DF6FCh					///022DF6FC SwitchStatementHandling
022DF6D0 E3E07000 mvn     r7,0h
022DF6D4 EA000008 b       22DF6FCh					///022DF6FC SwitchStatementHandling
022DF6D8 E51F19B0 ldr     r1,=2316A44h
022DF6DC E28D00D8 add     r0,r13,0D8h
022DF6E0 E5913058 ldr     r3,[r1,58h]
022DF6E4 E5916054 ldr     r6,[r1,54h]
022DF6E8 E59F1724 ldr     r1,=2319498h
022DF6EC E1A02005 mov     r2,r5
022DF6F0 E58D60D8 str     r6,[r13,0D8h]
022DF6F4 E58D30DC str     r3,[r13,0DCh]
022DF6F8 EBF4B2D7 bl      200C25Ch					///Print fatal error 0200C25C
///022DF6FC  22DF6FCh (Switch statement handling)
022DF6FC E1A00004 mov     r0,r4             //R0 = R4 (ScriptEngineStruct?)
022DF700 E1A01007 mov     r1,r7             //R1 = ScriptVariableValue
022DF704 EB001337 bl      22E43E8h          //022E43E8 ScriptCaseProcess
{///022E43E8 ScriptCaseProcess
    022E43E8 E92D4FF8 push    r3-r11,r14
    022E43EC E1A07001 mov     r7,r1             //R7 = ScriptVariableValue
    022E43F0 E1A08000 mov     r8,r0             //R8 = (ScriptEngineStruct?)
    022E43F4 E59F11AC ldr     r1,=2319600h      //"_ScriptCaseProcess %d"
    022E43F8 E1A02007 mov     r2,r7             //R2 = ScriptVariableValue
    022E43FC E3A00002 mov     r0,2h             //R0 = 2
    022E4400 E598501C ldr     r5,[r8,1Ch]       //R5 = AddressNextWord
    022E4404 E3A06000 mov     r6,0h             //R6 = 0 (CaseCounter)
    022E4408 EBF49F8C bl      200C240h          //DebugPrint
    022E440C E3A04002 mov     r4,2h             //R4 = 2
    022E4410 E59FA194 ldr     r10,=2319618h     //"    CASE %3d"
    022E4414 E1A0B004 mov     r11,r4            //R11 = 2
    ///22E4418h LOOP_BEG
    022E4418 E1D530B0 ldrh    r3,[r5]           //R3 = NextWord
    022E441C E2430063 sub     r0,r3,63h         //R0 = NextWord - 0x63 (0x63 is the first Case opcode)
    022E4420 E3500006 cmp     r0,6h
    if( R0 <= 6 )
        022E4424 908FF100 addls   r15,r15,r0,lsl 2h //R15 = 022E4428 + (R0 * 4) + 4
	else
		022E4428 EA000057 b       22E458Ch      //022E458C
    022E442C EA000005 b       22E4448h      //0x063 - Case          022E4448
    022E4430 EA000055 b       22E458Ch      //0x064 - CaseMenu      022E458C
    022E4434 EA000054 b       22E458Ch      //0x065 - CaseMenu2     022E458C
    022E4438 EA00001B b       22E44ACh      //0x066 - CaseScenario  022E44AC
    022E443C EA000052 b       22E458Ch      //0x067 - CaseText      022E458C
    022E4440 EA000019 b       22E44ACh      //0x068 - CaseValue     022E44AC
    022E4444 EA000032 b       22E4514h      //0x069 - CaseVariable  022E4514
    //0x063 - Case
    022E4448 E1D500B2 ldrh    r0,[r5,2h]            //R0 = CaseParam1
    022E444C EB000116 bl      22E48ACh              //Turn 16bits signed into 14bits unsigned
    022E4450 E1A09000 mov     r9,r0                 //R9 = CaseParam1_14b
    022E4454 E1A00004 mov     r0,r4                 //R0 = 2
    022E4458 E1A0100A mov     r1,r10                //R1 = "    CASE %3d"
    022E445C E1A02009 mov     r2,r9                 //R2 = CaseParam1_14b
    022E4460 EBF49F76 bl      200C240h              //DebugPrint "    CASE %3d"
    022E4464 E1A01009 mov     r1,r9                 //R1 = CaseParam1_14b
    022E4468 E1A00007 mov     r0,r7                 //R0 = ScriptVariableValue
    022E446C E1A0200B mov     r2,r11                //R2 = 2
    022E4470 EBF59EB9 bl      204BF5Ch              ///0204BF5C
    {///0204BF5C
        0204BF5C E59FC000 ldr     r12,=204BD98h     ///0204BD98    
        0204BF60 E12FFF1C bx      r12
        //Branch and Execute 0204BD98
        0204BD98 E92D4008 push    r3,r14
        0204BD9C E24DD008 sub     r13,r13,8h
        0204BDA0 E352000A cmp     r2,0Ah
        if( R2 <= 0xA )
            0204BDA4 908FF102 addls   r15,r15,r2,lsl 2h     //R15 = 0204BDA8 + (R2 * 4) + 4 
        else
			0204BDA8 EA00003B b       204BE9Ch      //Fatal Error 0204BE9C
        0204BDAC EA000009 b       204BDD8h      //0     0204BDD8
        0204BDB0 EA00000A b       204BDE0h      //1     0204BDE0
        0204BDB4 EA00000B b       204BDE8h      //2     0204BDE8 (Case?)
        0204BDB8 EA000014 b       204BE10h      //3     0204BE10
        0204BDBC EA00001D b       204BE38h      //4     0204BE38
        0204BDC0 EA000017 b       204BE24h      //5     0204BE24
        0204BDC4 EA000020 b       204BE4Ch      //6     0204BE4C
        0204BDC8 EA00000B b       204BDFCh      //7     0204BDFC
        0204BDCC EA000023 b       204BE60h      //8     0204BE60
        0204BDD0 EA000027 b       204BE74h      //9     0204BE74
        0204BDD4 EA00002B b       204BE88h      //0xA   0204BE88
        //Case 0
        0204BDD8 E3A00001 mov     r0,1h
        0204BDDC EA000036 b       204BEBCh      ///0204BEBC Return
        //Case 1
        0204BDE0 E3A00000 mov     r0,0h
        0204BDE4 EA000034 b       204BEBCh      ///0204BEBC Return
        //Case 2
        0204BDE8 E1500001 cmp     r0,r1         //
        if( ScriptVariableValue == CaseParam1_14b )
            0204BDEC 03A00001 moveq   r0,1h     //R0 = 1
        else
            0204BDF0 13A00000 movne   r0,0h     //R0 = 0
        0204BDF4 E20000FF and     r0,r0,0FFh    //R0 = R0 & 0xFF
        0204BDF8 EA00002F b       204BEBCh      ///0204BEBC Return
        //Case 7
        0204BDFC E1500001 cmp     r0,r1
        0204BE00 13A00001 movne   r0,1h
        0204BE04 03A00000 moveq   r0,0h
        0204BE08 E20000FF and     r0,r0,0FFh
        0204BE0C EA00002A b       204BEBCh      ///0204BEBC Return
        //Case 3
        0204BE10 E1500001 cmp     r0,r1
        0204BE14 C3A00001 movgt   r0,1h
        0204BE18 D3A00000 movle   r0,0h
        0204BE1C E20000FF and     r0,r0,0FFh
        0204BE20 EA000025 b       204BEBCh      ///0204BEBC Return
        //Case 5
        0204BE24 E1500001 cmp     r0,r1
        0204BE28 A3A00001 movge   r0,1h
        0204BE2C B3A00000 movlt   r0,0h
        0204BE30 E20000FF and     r0,r0,0FFh
        0204BE34 EA000020 b       204BEBCh      ///0204BEBC Return
        //Case 4
        0204BE38 E1500001 cmp     r0,r1
        0204BE3C B3A00001 movlt   r0,1h
        0204BE40 A3A00000 movge   r0,0h
        0204BE44 E20000FF and     r0,r0,0FFh
        0204BE48 EA00001B b       204BEBCh      ///0204BEBC Return
        //Case 6
        0204BE4C E1500001 cmp     r0,r1
        0204BE50 D3A00001 movle   r0,1h
        0204BE54 C3A00000 movgt   r0,0h
        0204BE58 E20000FF and     r0,r0,0FFh
        0204BE5C EA000016 b       204BEBCh      ///0204BEBC Return
        //Case 8
        0204BE60 E1100001 tst     r0,r1
        0204BE64 13A00001 movne   r0,1h
        0204BE68 03A00000 moveq   r0,0h
        0204BE6C E20000FF and     r0,r0,0FFh
        0204BE70 EA000011 b       204BEBCh      ///0204BEBC Return
        //Case 9
        0204BE74 E1300001 teq     r0,r1
        0204BE78 13A00001 movne   r0,1h
        0204BE7C 03A00000 moveq   r0,0h
        0204BE80 E20000FF and     r0,r0,0FFh
        0204BE84 EA00000C b       204BEBCh      ///0204BEBC Return
        //Case 0xA
        0204BE88 E3A02001 mov     r2,1h
        0204BE8C E1100112 tst     r0,r2,lsl r1
        0204BE90 03A02000 moveq   r2,0h
        0204BE94 E20200FF and     r0,r2,0FFh
        0204BE98 EA000007 b       204BEBCh      ///0204BEBC Return
		//R2 > 0xA (Fatal error)
        0204BE9C E59F1020 ldr     r1,=209CEACh  //a pointer to  "event_flag.c"
        0204BEA0 E28D0000 add     r0,r13,0h
        0204BEA4 E591300C ldr     r3,[r1,0Ch]   //A value?( ex 0x414)
        0204BEA8 E591C008 ldr     r12,[r1,8h]   //
        0204BEAC E59F1014 ldr     r1,=209CF48h  //"event flag rule error %d"
        0204BEB0 E58DC000 str     r12,[r13]
        0204BEB4 E58D3004 str     r3,[r13,4h]
        0204BEB8 EBFF00E7 bl      200C25Ch      ///Print fatal error 0200C25C
        {   ///0200C25C
            0200C25C E92D000F push    r0-r3
            0200C260 E92D4008 push    r3,r14
            0200C264 E24DDC01 sub     r13,r13,100h
            0200C268 E1A01000 mov     r1,r0
            0200C26C E59F0058 ldr     r0,=2094B40h      //"!!!!! Fatal !!!!!\n"
            0200C270 EBFFFFBD bl      200C16Ch
            0200C274 E59D110C ldr     r1,[r13,10Ch]
            0200C278 E28D0000 add     r0,r13,0h
            0200C27C E3510000 cmp     r1,0h
            0200C280 0A000007 beq     200C2A4h
            0200C284 E28D2F43 add     r2,r13,10Ch
            0200C288 E3C22003 bic     r2,r2,3h
            0200C28C E2822004 add     r2,r2,4h
            0200C290 EB01F4AB bl      2089544h
            0200C294 E59F1034 ldr     r1,=2094B54h      //"\n"
            0200C298 E28D0000 add     r0,r13,0h
            0200C29C EB01F542 bl      20897ACh
            0200C2A0 EA000001 b       200C2ACh
            0200C2A4 E59F1028 ldr     r1,=2094B58h      //"(NULL)\n"
            0200C2A8 EB01F4F9 bl      2089694h
            0200C2AC E59F0024 ldr     r0,=2094B60h      //"%s\n"    
            0200C2B0 E28D1000 add     r1,r13,0h
            0200C2B4 EBFFFFC3 bl      200C1C8h
            0200C2B8 EBFFD85E bl      2002438h
            0200C2BC E28DDC01 add     r13,r13,100h
            0200C2C0 E8BD4008 pop     r3,r14
            0200C2C4 E28DD010 add     r13,r13,10h
            0200C2C8 E12FFF1E bx      r14
            0200C2CC 02094B40
            0200C2D0 02094B54
            0200C2D4 02094B58
            0200C2D8 02094B60
        }
        ///0204BEBC Return
        0204BEBC E28DD008 add     r13,r13,8h
        0204BEC0 E8BD8008 pop     r3,r15
        0204BEC4 0209CEAC 
        0204BEC8 0209CF48 
    }
    022E4474 E3500000 cmp     r0,0h                 //Check if the CaseParam1_14b Matched the ScriptVariableValue
    if( R0 == 0 )
        022E4478 0A000008 beq     22E44A0h          //Didn't match 022E44A0
    022E447C E1D530B4 ldrh    r3,[r5,4h]            //R0 = CaseParam2
    022E4480 E59F1128 ldr     r1,=2319628h          //"    CASE JUMP %3d %3d"
    022E4484 E1A02006 mov     r2,r6                 //R2 = CaseCounter
    022E4488 E3A00002 mov     r0,2h                 //R0 = 2
    022E448C EBF49F6B bl      200C240h              //DebugPrint
    022E4490 E1D500B4 ldrh    r0,[r5,4h]            //R0 = CaseParam2
    022E4494 E5981014 ldr     r1,[r8,14h]           //R1 = PointerSSBDataBlockBeg
    022E4498 E0810080 add     r0,r1,r0,lsl 1h       //R0 = PointerSSBDataBlockBeg + (CaseParam2 * 2)
    022E449C E8BD8FF8 pop     r3-r11,r15
    //022E44A0
    022E44A0 E2855006 add     r5,r5,6h              //R5 = AddressNextWord + 6 (Increment to parse the next Case opcode!)
    022E44A4 E2866001 add     r6,r6,1h              //R6 = CaseCounter + 1
    022E44A8 EAFFFFDA b       22E4418h              ///022E4418 LOOP_BEG
    //0x068,0x066 - CaseValue, CaseScenario 
    022E44AC E1D500B4 ldrh    r0,[r5,4h]            //R0 = CaseParam2
    022E44B0 EB0000FD bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
    022E44B4 E1A09000 mov     r9,r0
    022E44B8 E1D520B2 ldrh    r2,[r5,2h]            //R0 = CaseParam1
    022E44BC E59F10F0 ldr     r1,=2319640h          //"    CASE_VALUE %3d %3d"
    022E44C0 E3A00002 mov     r0,2h                 //R0 = 2
    022E44C4 E1A03009 mov     r3,r9
    022E44C8 EBF49F5C bl      200C240h              //DebugPrint
    022E44CC E1D520B2 ldrh    r2,[r5,2h]            //R0 = CaseParam1
    022E44D0 E1A01009 mov     r1,r9
    022E44D4 E1A00007 mov     r0,r7                 //R0 = ScriptVariableValue
    022E44D8 EBF59E9F bl      204BF5Ch
    022E44DC E3500000 cmp     r0,0h
    if( R0 == 0 )
        022E44E0 0A000008 beq     22E4508h          //022E4508    
    022E44E4 E1D530B6 ldrh    r3,[r5,6h]            //R0 = CaseParam3
    022E44E8 E59F10C0 ldr     r1,=2319628h          //"    CASE JUMP %3d %3d"
    022E44EC E1A02006 mov     r2,r6                 //R2 = CaseCounter
    022E44F0 E3A00002 mov     r0,2h                 //R0 = 2
    022E44F4 EBF49F51 bl      200C240h
    022E44F8 E1D500B6 ldrh    r0,[r5,6h]            //R0 = CaseParam3
    022E44FC E5981014 ldr     r1,[r8,14h]           //R1 = PointerSSBDataBlockBeg
    022E4500 E0810080 add     r0,r1,r0,lsl 1h       //R0 = PointerSSBDataBlockBeg + (CaseParam2 * 2)
    022E4504 E8BD8FF8 pop     r3-r11,r15
    //022E4508 R0 == 0
    022E4508 E2855008 add     r5,r5,8h              //R5 = AddressNextWord + 8 (Increment to parse the next Case opcode!)
    022E450C E2866001 add     r6,r6,1h              //R6 = CaseCounter + 1
    022E4510 EAFFFFC0 b       22E4418h              ///022E4418 LOOP_BEG
    ///0x69 - CaseVariable
    022E4514 E1D500B4 ldrh    r0,[r5,4h]
    022E4518 EB0000E3 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
    022E451C E1A09000 mov     r9,r0
    022E4520 E1D520B2 ldrh    r2,[r5,2h]
    022E4524 E59F108C ldr     r1,=2319658h          //"    CASE_VARIABLE %3d %3d"
    022E4528 E3A00002 mov     r0,2h                 //R0 = 2
    022E452C E1A03009 mov     r3,r9
    022E4530 EBF49F42 bl      200C240h
    022E4534 E1A00809 mov     r0,r9,lsl 10h
    022E4538 E1A01840 mov     r1,r0,asr 10h
    022E453C E288006C add     r0,r8,6Ch
    022E4540 EBF59BE9 bl      204B4ECh
    022E4544 E1A01000 mov     r1,r0
    022E4548 E1D520B2 ldrh    r2,[r5,2h]
    022E454C E1A00007 mov     r0,r7
    022E4550 EBF59E81 bl      204BF5Ch
    022E4554 E3500000 cmp     r0,0h
    022E4558 0A000008 beq     22E4580h
    022E455C E1D530B6 ldrh    r3,[r5,6h]
    022E4560 E59F1048 ldr     r1,=2319628h          //"    CASE JUMP %3d %3d"
    022E4564 E1A02006 mov     r2,r6                 //R2 = CaseCounter
    022E4568 E3A00002 mov     r0,2h                 //R0 = 2
    022E456C EBF49F33 bl      200C240h              //DebugPrint
    022E4570 E1D500B6 ldrh    r0,[r5,6h]
    022E4574 E5981014 ldr     r1,[r8,14h]
    022E4578 E0810080 add     r0,r1,r0,lsl 1h
    022E457C E8BD8FF8 pop     r3-r11,r15
    022E4580 E2855008 add     r5,r5,8h              //R5 = AddressNextWord + 8 (Increment to parse the next Case opcode!)
    022E4584 E2866001 add     r6,r6,1h              //R6 = CaseCounter + 1
    022E4588 EAFFFFA2 b       22E4418h              ///022E4418 LOOP_BEG
    //0x064, 0x065, 0x067 - CaseMenu, CaseMenu2, CaseText
    022E458C E59F0028 ldr     r0,=2318614h          //R0 = OpCodeStrPtrs : Address First String Pointer In opcode table!
    022E4590 E59F1028 ldr     r1,=2319674h          //"    CASE OUT %10s[%04x]"
    022E4594 E7902183 ldr     r2,[r0,r3,lsl 3h]     // Load into R2 the string ptr at [OpCodeStrPtrs + (NextWord * 8)]
    022E4598 E3A00002 mov     r0,2h                 //
    022E459C EBF49F27 bl      200C240h              //
    022E45A0 E1A00005 mov     r0,r5                 //R0 = NextWord
    022E45A4 E8BD8FF8 pop     r3-r11,r15
    022E45A8 02319600
    022E45AC 02319618
    022E45B0 02319628
    022E45B4 02319640
    022E45B8 02319658
    022E45BC 02318614
    022E45C0 02319674
}
022DF708 E584001C str     r0,[r4,1Ch]       //Put R0 in AddressCurrentOpCode
022DF70C EA000B58 b       22E2474h          //022E2474 - Returns 2

022DF710 E59F1700 ldr     r1,=2324ECCh
022DF714 E3A02806 mov     r2,60000h
022DF718 E59F06FC ldr     r0,=2324EA4h
022DF71C E581204C str     r2,[r1,4Ch]
022DF720 EBF5B81D bl      204D79Ch
022DF724 EA000B52 b       22E2474h
022DF728 E59F16E8 ldr     r1,=2324ECCh
022DF72C E3A02807 mov     r2,70000h
022DF730 E59F06E4 ldr     r0,=2324EA4h
022DF734 E581204C str     r2,[r1,4Ch]
022DF738 EBF5B817 bl      204D79Ch
022DF73C EA000B4C b       22E2474h
022DF740 E1D600B0 ldrh    r0,[r6]
022DF744 EB001458 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF748 E3802801 orr     r2,r0,10000h
022DF74C E59F16C4 ldr     r1,=2324ECCh
022DF750 E59F06C4 ldr     r0,=2324EA4h
022DF754 E581204C str     r2,[r1,4Ch]
022DF758 EBF5B80F bl      204D79Ch
022DF75C EA000B44 b       22E2474h
022DF760 E1D600B0 ldrh    r0,[r6]
022DF764 EB001450 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF768 E1A07000 mov     r7,r0
022DF76C E1D600B2 ldrh    r0,[r6,2h]
022DF770 EB00144D bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF774 E1A05000 mov     r5,r0
022DF778 E1D600B4 ldrh    r0,[r6,4h]
022DF77C EB00144A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF780 E1A01807 mov     r1,r7,lsl 10h
022DF784 E1A04000 mov     r4,r0
022DF788 E1A00841 mov     r0,r1,asr 10h
022DF78C EBF61878 bl      2065974h
022DF790 E1A01800 mov     r1,r0,lsl 10h
022DF794 E59F267C ldr     r2,=2324ECCh
022DF798 E3873801 orr     r3,r7,10000h
022DF79C E59F0678 ldr     r0,=2324EA4h
022DF7A0 E1A01841 mov     r1,r1,asr 10h
022DF7A4 E582304C str     r3,[r2,4Ch]
022DF7A8 EBF5B809 bl      204D7D4h
022DF7AC E1A01C05 mov     r1,r5,lsl 18h
022DF7B0 E59F0664 ldr     r0,=2324EA4h
022DF7B4 E1A01C41 mov     r1,r1,asr 18h
022DF7B8 EBF5B80D bl      204D7F4h
022DF7BC E59F0658 ldr     r0,=2324EA4h
022DF7C0 E20410FF and     r1,r4,0FFh
022DF7C4 EBF5B80E bl      204D804h
022DF7C8 EA000B29 b       22E2474h
022DF7CC E1D600B0 ldrh    r0,[r6]
022DF7D0 EB001435 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF7D4 E1A04000 mov     r4,r0
022DF7D8 E1D600B2 ldrh    r0,[r6,2h]
022DF7DC EB001432 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF7E0 E1A05000 mov     r5,r0
022DF7E4 E1D600B4 ldrh    r0,[r6,4h]
022DF7E8 EB00142F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF7EC E1A01804 mov     r1,r4,lsl 10h
022DF7F0 E1A04000 mov     r4,r0
022DF7F4 E59F261C ldr     r2,=2324ECCh
022DF7F8 E3A03806 mov     r3,60000h
022DF7FC E1A00841 mov     r0,r1,asr 10h
022DF800 E582304C str     r3,[r2,4Ch]
022DF804 EBF6185A bl      2065974h
022DF808 E1A01000 mov     r1,r0
022DF80C E59F0608 ldr     r0,=2324EA4h
022DF810 EBF5B7EF bl      204D7D4h
022DF814 E1A01C05 mov     r1,r5,lsl 18h
022DF818 E59F05FC ldr     r0,=2324EA4h
022DF81C E1A01C41 mov     r1,r1,asr 18h
022DF820 EBF5B7F3 bl      204D7F4h
022DF824 E59F05F0 ldr     r0,=2324EA4h
022DF828 E20410FF and     r1,r4,0FFh
022DF82C EBF5B7F4 bl      204D804h
022DF830 EA000B0F b       22E2474h
022DF834 E1D600B0 ldrh    r0,[r6]
022DF838 EB00141B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF83C E1A04000 mov     r4,r0
022DF840 E1D600B2 ldrh    r0,[r6,2h]
022DF844 EB001418 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF848 E1A05000 mov     r5,r0
022DF84C E1D600B4 ldrh    r0,[r6,4h]
022DF850 EB001415 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF854 E1A01804 mov     r1,r4,lsl 10h
022DF858 E1A04000 mov     r4,r0
022DF85C E59F25B4 ldr     r2,=2324ECCh
022DF860 E3A03807 mov     r3,70000h
022DF864 E1A00841 mov     r0,r1,asr 10h
022DF868 E582304C str     r3,[r2,4Ch]
022DF86C EBF61840 bl      2065974h
022DF870 E1A01000 mov     r1,r0
022DF874 E59F05A0 ldr     r0,=2324EA4h
022DF878 EBF5B7D5 bl      204D7D4h
022DF87C E1A01C05 mov     r1,r5,lsl 18h
022DF880 E59F0594 ldr     r0,=2324EA4h
022DF884 E1A01C41 mov     r1,r1,asr 18h
022DF888 EBF5B7D9 bl      204D7F4h
022DF88C E59F0588 ldr     r0,=2324EA4h
022DF890 E20410FF and     r1,r4,0FFh
022DF894 EBF5B7DA bl      204D804h
022DF898 EA000AF5 b       22E2474h
022DF89C E1D600B0 ldrh    r0,[r6]
022DF8A0 EB001401 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF8A4 E1A01000 mov     r1,r0
022DF8A8 E59F056C ldr     r0,=2324EA4h
022DF8AC E20110FF and     r1,r1,0FFh
022DF8B0 EBF5B7D3 bl      204D804h
022DF8B4 EA000AEE b       22E2474h
022DF8B8 E51F0B90 ldr     r0,=2316A44h
022DF8BC E5901080 ldr     r1,[r0,80h]
022DF8C0 E590007C ldr     r0,[r0,7Ch]
022DF8C4 E58D10D4 str     r1,[r13,0D4h]
022DF8C8 E58D00D0 str     r0,[r13,0D0h]
022DF8CC E1D600B0 ldrh    r0,[r6]
022DF8D0 EB0013F5 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF8D4 E58D00D0 str     r0,[r13,0D0h]
022DF8D8 E1D600B2 ldrh    r0,[r6,2h]
022DF8DC EB0013F2 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF8E0 E58D00D4 str     r0,[r13,0D4h]
022DF8E4 E59F0530 ldr     r0,=2324EA4h
022DF8E8 E28D10D0 add     r1,r13,0D0h
022DF8EC EBF5B7D5 bl      204D848h
022DF8F0 EA000ADF b       22E2474h
022DF8F4 E1D600B0 ldrh    r0,[r6]
022DF8F8 EB0013EB bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF8FC E51F1BEC ldr     r1,=2324E80h
022DF900 E581000C str     r0,[r1,0Ch]
022DF904 E1D600B2 ldrh    r0,[r6,2h]
022DF908 EB0013E7 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DF90C E51F2BFC ldr     r2,=2324E80h
022DF910 E1A01000 mov     r1,r0
022DF914 E5821010 str     r1,[r2,10h]
022DF918 E592000C ldr     r0,[r2,0Ch]
022DF91C EBF5554F bl      2034E60h
022DF920 EA000AD3 b       22E2474h
022DF924 E3A00005 mov     r0,5h
022DF928 EA000AD2 b       22E2478h        //Return 022E2478
///0xAE message_Talk
022DF92C E1D610B0 ldrh    r1,[r6]               //R1 = Parameter
022DF930 E2840014 add     r0,r4,14h             //R0 = ScriptEngineStruct + 0x14
022DF934 EB001243 bl      22E4248h              ///022E4248
{//022E4248
    022E4248 E590200C ldr     r2,[r0,0Ch]       //R2 = PtrBegScriptStringOffsetTable
    022E424C E1A00081 mov     r0,r1,lsl 1h      //R0 = Parameter * 2
    022E4250 E19200B0 ldrh    r0,[r2,r0]        //R0 = Load StringOffset [PtrBegScriptStringOffsetTable + (Parameter * 2)]
    022E4254 E0820000 add     r0,r2,r0          //R0 = PtrBegScriptStringOffsetTable + StringOffset
    022E4258 E12FFF1E bx      r14
}
022DF938 E1A04000 mov     r4,r0                 //R4 = String
022DF93C E59F14DC ldr     r1,=23194B0h          //"MESSAGE request"
022DF940 E3A00002 mov     r0,2h                 //
022DF944 EBF4B23D bl      200C240h              //Debug Printf
022DF948 E355009E cmp     r5,9Eh
if( CurrentOpCode != 0x9E )
    022DF94C 135500AE cmpne   r5,0AEh
        if( CurrentOpCode != 0xAE )
            022DF950 1A000002 bne     22DF960h  ///022DF960   
022DF954 E59F04C0 ldr     r0,=2324EA4h          
022DF958 EBF55532 bl      2034E28h              ///02034E28
{//02034E28
    02034E28 E92D4010 push    r4,r14
    02034E2C E1A04000 mov     r4,r0
    02034E30 EBFFFFC6 bl      2034D50h
    02034E34 E3540000 cmp     r4,0h
    02034E38 159FC01C ldrne   r12,=20AFDBCh
    02034E3C 1894000F ldmneia [r4],r0-r3
    02034E40 159CC00C ldrne   r12,[r12,0Ch]
    02034E44 188C000F stmneia [r12],r0-r3
    02034E48 18BD8010 popne   r4,r15
    02034E4C E59F0008 ldr     r0,=20AFDBCh
    02034E50 E590000C ldr     r0,[r0,0Ch]
    02034E54 EB006250 bl      204D79Ch
    02034E58 E8BD8010 pop     r4,r15
    02034E5C 020AFDBC 
}
022DF95C EA000001 b       22DF968h              ///022DF968
///022DF960
022DF960 E59F04BC ldr     r0,=2324E94h          
022DF964 EBF5552F bl      2034E28h              ///02034E28
{//02034E28
    02034E28 E92D4010 push    r4,r14
    02034E2C E1A04000 mov     r4,r0
    02034E30 EBFFFFC6 bl      2034D50h
    02034E34 E3540000 cmp     r4,0h
    02034E38 159FC01C ldrne   r12,=20AFDBCh
    02034E3C 1894000F ldmneia [r4],r0-r3
    02034E40 159CC00C ldrne   r12,[r12,0Ch]
    02034E44 188C000F stmneia [r12],r0-r3
    02034E48 18BD8010 popne   r4,r15
    02034E4C E59F0008 ldr     r0,=20AFDBCh
    02034E50 E590000C ldr     r0,[r0,0Ch]
    02034E54 EB006250 bl      204D79Ch
    02034E58 E8BD8010 pop     r4,r15
    02034E5C 020AFDBC 
}
///022DF968
022DF968 E51F0C58 ldr     r0,=2324E80h
022DF96C E590000C ldr     r0,[r0,0Ch]
022DF970 E3500000 cmp     r0,0h
022DF974 A3A01020 movge   r1,20h
022DF978 B3A01000 movlt   r1,0h
022DF97C E35500A0 cmp     r5,0A0h
022DF980 CA000009 bgt     22DF9ACh
022DF984 E255009A subs    r0,r5,9Ah
022DF988 508FF100 addpl   r15,r15,r0,lsl 2h
022DF98C EA00002D b       22DFA48h
022DF990 EA000026 b       22DFA30h
022DF994 EA00002B b       22DFA48h
022DF998 EA00001D b       22DFA14h
022DF99C EA000029 b       22DFA48h
022DF9A0 EA00000C b       22DF9D8h
022DF9A4 EA000027 b       22DFA48h
022DF9A8 EA000002 b       22DF9B8h
022DF9AC E35500AE cmp     r5,0AEh
022DF9B0 0A00000F beq     22DF9F4h
022DF9B4 EA000023 b       22DFA48h
022DF9B8 E3810008 orr     r0,r1,8h
022DF9BC E3800B01 orr     r0,r0,400h
022DF9C0 E1A00800 mov     r0,r0,lsl 10h
022DF9C4 E59F244C ldr     r2,=2324ECCh
022DF9C8 E1A01004 mov     r1,r4
022DF9CC E1A00820 mov     r0,r0,lsr 10h
022DF9D0 EBF55552 bl      2034F20h
022DF9D4 EA00001B b       22DFA48h
022DF9D8 E3810008 orr     r0,r1,8h
022DF9DC E1A00800 mov     r0,r0,lsl 10h
022DF9E0 E59F2430 ldr     r2,=2324ECCh
022DF9E4 E1A01004 mov     r1,r4
022DF9E8 E1A00820 mov     r0,r0,lsr 10h
022DF9EC EBF5554B bl      2034F20h
022DF9F0 EA000014 b       22DFA48h
022DF9F4 E3810008 orr     r0,r1,8h
022DF9F8 E3800A03 orr     r0,r0,3000h
022DF9FC E1A00800 mov     r0,r0,lsl 10h
022DFA00 E59F2410 ldr     r2,=2324ECCh
022DFA04 E1A01004 mov     r1,r4
022DFA08 E1A00820 mov     r0,r0,lsr 10h
022DFA0C EBF55543 bl      2034F20h
022DFA10 EA00000C b       22DFA48h
022DFA14 E3810008 orr     r0,r1,8h
022DFA18 E1A00800 mov     r0,r0,lsl 10h
022DFA1C E59F23F4 ldr     r2,=2324ECCh
022DFA20 E1A01004 mov     r1,r4
022DFA24 E1A00820 mov     r0,r0,lsr 10h
022DFA28 EBF5553C bl      2034F20h
022DFA2C EA000005 b       22DFA48h
022DFA30 E3810004 orr     r0,r1,4h
022DFA34 E1A00800 mov     r0,r0,lsl 10h
022DFA38 E59F23D8 ldr     r2,=2324ECCh
022DFA3C E1A01004 mov     r1,r4
022DFA40 E1A00820 mov     r0,r0,lsr 10h
022DFA44 EBF55535 bl      2034F20h
022DFA48 E3A00005 mov     r0,5h
022DFA4C EA000A89 b       22E2478h        //Return 022E2478
022DFA50 E1D610B0 ldrh    r1,[r6]
022DFA54 E2840014 add     r0,r4,14h
022DFA58 EB0011FA bl      22E4248h                  //GetStringFromScriptsStringTable( strtblptr, stringid)
022DFA5C E51F3D4C ldr     r3,=2324E80h
022DFA60 E59F13B0 ldr     r1,=2324ECCh
022DFA64 E593200C ldr     r2,[r3,0Ch]
022DFA68 E5933010 ldr     r3,[r3,10h]
022DFA6C EB008416 bl      2300ACCh
022DFA70 E3A00005 mov     r0,5h
022DFA74 EA000A7F b       22E2478h        //Return 022E2478
022DFA78 E1D600B0 ldrh    r0,[r6]
022DFA7C EB00138A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFA80 E1A05000 mov     r5,r0
022DFA84 E1D610B2 ldrh    r1,[r6,2h]
022DFA88 E2840014 add     r0,r4,14h
022DFA8C EB0011ED bl      22E4248h                  //GetStringFromScriptsStringTable( strtblptr, stringid)
022DFA90 E51F2D80 ldr     r2,=2324E80h
022DFA94 E59F137C ldr     r1,=2324ECCh
022DFA98 E592300C ldr     r3,[r2,0Ch]
022DFA9C E1A02005 mov     r2,r5
022DFAA0 EB008320 bl      2300728h
022DFAA4 E3A00005 mov     r0,5h
022DFAA8 EA000A72 b       22E2478h        //Return 022E2478
022DFAAC E1D600B0 ldrh    r0,[r6]
022DFAB0 EB00137D bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFAB4 E1A05000 mov     r5,r0
022DFAB8 E59F1368 ldr     r1,=23194C0h          //"SPECIAL MESSAGE request"
022DFABC E3A00002 mov     r0,2h
022DFAC0 E3A06000 mov     r6,0h
022DFAC4 EBF4B1DD bl      200C240h              //Debug Printf
022DFAC8 E51F0DB8 ldr     r0,=2324E80h
022DFACC E590000C ldr     r0,[r0,0Ch]
022DFAD0 E3500000 cmp     r0,0h
022DFAD4 A3A08020 movge   r8,20h
022DFAD8 B1A08006 movlt   r8,r6
022DFADC E3550000 cmp     r5,0h
022DFAE0 13550001 cmpne   r5,1h
022DFAE4 0A000002 beq     22DFAF4h
022DFAE8 E3550002 cmp     r5,2h
022DFAEC 0A00004F beq     22DFC30h
022DFAF0 EA000053 b       22DFC44h
022DFAF4 E3A0002F mov     r0,2Fh
022DFAF8 EBF617A6 bl      2065998h
022DFAFC E1A07000 mov     r7,r0
022DFB00 E3550000 cmp     r5,0h
022DFB04 03A09004 moveq   r9,4h
022DFB08 E59F030C ldr     r0,=2324EA4h
022DFB0C 13A09005 movne   r9,5h
022DFB10 EBF554C4 bl      2034E28h
022DFB14 E59FC2FC ldr     r12,=2324ECCh
022DFB18 E28DBF9E add     r11,r13,278h
022DFB1C E3A0A005 mov     r10,5h
022DFB20 E8BC000F ldmia   [r12]!,r0-r3
022DFB24 E8AB000F stmia   [r11]!,r0-r3
022DFB28 E25AA001 subs    r10,r10,1h
022DFB2C 1AFFFFFB bne     22DFB20h
022DFB30 E59F12F4 ldr     r1,=1002Fh
022DFB34 E3E00000 mvn     r0,0h
022DFB38 E58D127C str     r1,[r13,27Ch]
022DFB3C E1570000 cmp     r7,r0
022DFB40 0A00002A beq     22DFBF0h
022DFB44 E1A00007 mov     r0,r7
022DFB48 EBF5D630 bl      2055410h
022DFB4C E3500000 cmp     r0,0h
022DFB50 0A000008 beq     22DFB78h
022DFB54 E3880008 orr     r0,r8,8h
022DFB58 E3800A03 orr     r0,r0,3000h
022DFB5C E1A00800 mov     r0,r0,lsl 10h
022DFB60 E59F12C8 ldr     r1,=25Eh
022DFB64 E59F22AC ldr     r2,=2324ECCh
022DFB68 E1A00820 mov     r0,r0,lsr 10h
022DFB6C EBF554D0 bl      2034EB4h
022DFB70 E3A00005 mov     r0,5h
022DFB74 EA000A3F b       22E2478h        //Return 022E2478
022DFB78 E1A00007 mov     r0,r7
022DFB7C EBF5D63C bl      2055474h
022DFB80 E3500000 cmp     r0,0h
022DFB84 0A000019 beq     22DFBF0h
022DFB88 E3550001 cmp     r5,1h
022DFB8C 1A000012 bne     22DFBDCh
022DFB90 E3A00000 mov     r0,0h
022DFB94 E3A01043 mov     r1,43h
022DFB98 EBF5AE53 bl      204B4ECh
022DFB9C E28000AF add     r0,r0,0AFh
022DFBA0 E2800B01 add     r0,r0,400h
022DFBA4 E1A00800 mov     r0,r0,lsl 10h
022DFBA8 E1A01009 mov     r1,r9
022DFBAC E1A00840 mov     r0,r0,asr 10h
022DFBB0 E3A02000 mov     r2,0h
022DFBB4 EBF51774 bl      202598Ch
022DFBB8 E3881008 orr     r1,r8,8h
022DFBBC E3811A03 orr     r1,r1,3000h
022DFBC0 E1A03801 mov     r3,r1,lsl 10h
022DFBC4 E1A01000 mov     r1,r0
022DFBC8 E28D2F9E add     r2,r13,278h
022DFBCC E1A00823 mov     r0,r3,lsr 10h
022DFBD0 EBF554B7 bl      2034EB4h
022DFBD4 E3A00005 mov     r0,5h
022DFBD8 EA000A26 b       22E2478h        //Return 022E2478
022DFBDC E3A0000B mov     r0,0Bh
022DFBE0 E3A01000 mov     r1,0h
022DFBE4 EB005D67 bl      22F7188h          //022F7188 GroundSupervision ExecuteCommon
022DFBE8 E3A00001 mov     r0,1h
022DFBEC EA000A21 b       22E2478h        //Return 022E2478
022DFBF0 E3A0002F mov     r0,2Fh
022DFBF4 EBF6175E bl      2065974h
022DFBF8 E3500000 cmp     r0,0h
022DFBFC 03E06000 mvneq   r6,0h
022DFC00 0A00000F beq     22DFC44h
022DFC04 E1A01009 mov     r1,r9
022DFC08 E3A02000 mov     r2,0h
022DFC0C EBF5175E bl      202598Ch
022DFC10 E3881008 orr     r1,r8,8h
022DFC14 E3811A03 orr     r1,r1,3000h
022DFC18 E1A03801 mov     r3,r1,lsl 10h
022DFC1C E1A01000 mov     r1,r0
022DFC20 E28D2F9E add     r2,r13,278h
022DFC24 E1A00823 mov     r0,r3,lsr 10h
022DFC28 EBF554A1 bl      2034EB4h
022DFC2C EA000004 b       22DFC44h
022DFC30 E3A0000B mov     r0,0Bh
022DFC34 E3A01000 mov     r1,0h
022DFC38 EB005D52 bl      22F7188h          //022F7188 GroundSupervision ExecuteCommon
022DFC3C E3A00001 mov     r0,1h
022DFC40 EA000A0C b       22E2478h        //Return 022E2478
022DFC44 E3560000 cmp     r6,0h
022DFC48 03A00005 moveq   r0,5h
022DFC4C 0A000A09 beq     22E2478h        //Return 022E2478
022DFC50 E1A00004 mov     r0,r4
022DFC54 E1A01006 mov     r1,r6
022DFC58 EB0011E2 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022DFC5C E584001C str     r0,[r4,1Ch]
022DFC60 EA000A03 b       22E2474h
022DFC64 E51F0F54 ldr     r0,=2324E80h
022DFC68 E1D000F6 ldrsh   r0,[r0,6h]
022DFC6C EB005290 bl      22F46B4h
022DFC70 E3A00000 mov     r0,0h
022DFC74 EB002CDF bl      22EAFF8h
022DFC78 EB007B6A bl      22FEA28h
022DFC7C EB006025 bl      22F7D18h
022DFC80 EB00705F bl      22FBE04h
022DFC84 EB007678 bl      22FD66Ch
022DFC88 EB00456D bl      22F1244h
022DFC8C E3A00004 mov     r0,4h
022DFC90 EA0009F8 b       22E2478h        //Return 022E2478
022DFC94 E1D600B0 ldrh    r0,[r6]
022DFC98 E594501C ldr     r5,[r4,1Ch]
022DFC9C EB001302 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFCA0 E1A06000 mov     r6,r0
022DFCA4 E1A01806 mov     r1,r6,lsl 10h
022DFCA8 E284006C add     r0,r4,6Ch
022DFCAC E1A01841 mov     r1,r1,asr 10h
022DFCB0 EBF5AE0D bl      204B4ECh
022DFCB4 E1C404B4 strh    r0,[r4,44h]
022DFCB8 E1D424F4 ldrsh   r2,[r4,44h]
022DFCBC E59F1170 ldr     r1,=23194D8h          //"    switch talk %3d[%3d]"
022DFCC0 E1A03006 mov     r3,r6
022DFCC4 E3A00002 mov     r0,2h
022DFCC8 EBF4B15C bl      200C240h              //Debug Printf
022DFCCC E3A07002 mov     r7,2h
022DFCD0 E59F6160 ldr     r6,=23194F4h          //"    case [%3d]"
022DFCD4 EA00000C b       22DFD0Ch
022DFCD8 E1D500B2 ldrh    r0,[r5,2h]
022DFCDC EB0012F2 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFCE0 E1A08000 mov     r8,r0
022DFCE4 E1A00007 mov     r0,r7
022DFCE8 E1A01006 mov     r1,r6
022DFCEC E1A02008 mov     r2,r8
022DFCF0 EBF4B152 bl      200C240h              //Debug Printf
022DFCF4 E1D404F4 ldrsh   r0,[r4,44h]
022DFCF8 E1580000 cmp     r8,r0
022DFCFC 0584501C streq   r5,[r4,1Ch]
022DFD00 03A00005 moveq   r0,5h
022DFD04 0A0009DB beq     22E2478h        //Return 022E2478
022DFD08 E2855006 add     r5,r5,6h
022DFD0C E1D500B0 ldrh    r0,[r5]
022DFD10 E3500067 cmp     r0,67h
022DFD14 0AFFFFEF beq     22DFCD8h
022DFD18 E584501C str     r5,[r4,1Ch]
022DFD1C E1D500B0 ldrh    r0,[r5]
022DFD20 E350006E cmp     r0,6Eh
022DFD24 1A0009D2 bne     22E2474h
022DFD28 E59F110C ldr     r1,=2319504h          //"    case default"
022DFD2C E3A00002 mov     r0,2h
022DFD30 EBF4B142 bl      200C240h              //Debug Printf
022DFD34 E3E00000 mvn     r0,0h
022DFD38 E1C404B4 strh    r0,[r4,44h]
022DFD3C E3A00005 mov     r0,5h
022DFD40 EA0009CC b       22E2478h        //Return 022E2478
022DFD44 E59F00F4 ldr     r0,=524Ch         //22DFD44h
022DFD48 E3A01008 mov     r1,8h
022DFD4C E594701C ldr     r7,[r4,1Ch]
022DFD50 EBF48506 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
022DFD54 E59F10E8 ldr     r1,=2324C64h
022DFD58 E35500AB cmp     r5,0ABh
022DFD5C E3A02000 mov     r2,0h
022DFD60 E5810000 str     r0,[r1]
022DFD64 E5802000 str     r2,[r0]
022DFD68 E5910000 ldr     r0,[r1]
022DFD6C 13A08072 movne   r8,72h
022DFD70 E5802004 str     r2,[r0,4h]
022DFD74 1A000003 bne     22DFD88h
022DFD78 E1D600B4 ldrh    r0,[r6,4h]
022DFD7C EB0012CA bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFD80 E1A00800 mov     r0,r0,lsl 10h
022DFD84 E1A08840 mov     r8,r0,asr 10h
022DFD88 E3A0B000 mov     r11,0h
022DFD8C E59F50B0 ldr     r5,=2324C64h
022DFD90 EA000050 b       22DFED8h
022DFD94 E5952000 ldr     r2,[r5]
022DFD98 E3580072 cmp     r8,72h
022DFD9C E5920000 ldr     r0,[r2]
022DFDA0 E2821008 add     r1,r2,8h
022DFDA4 E0800300 add     r0,r0,r0,lsl 6h
022DFDA8 E0819100 add     r9,r1,r0,lsl 2h
022DFDAC 0A00000D beq     22DFDE8h
022DFDB0 E5921004 ldr     r1,[r2,4h]
022DFDB4 E3A00000 mov     r0,0h
022DFDB8 E1A01801 mov     r1,r1,lsl 10h
022DFDBC E1A02821 mov     r2,r1,lsr 10h
022DFDC0 E1A01008 mov     r1,r8
022DFDC4 EBF5AE2B bl      204B678h
022DFDC8 E3500000 cmp     r0,0h
022DFDCC 1A000005 bne     22DFDE8h
022DFDD0 E5951000 ldr     r1,[r5]
022DFDD4 E2877006 add     r7,r7,6h
022DFDD8 E5910004 ldr     r0,[r1,4h]
022DFDDC E2800001 add     r0,r0,1h
022DFDE0 E5810004 str     r0,[r1,4h]
022DFDE4 EA00003B b       22DFED8h
022DFDE8 E1D700B0 ldrh    r0,[r7]
022DFDEC E3500064 cmp     r0,64h
022DFDF0 1A000018 bne     22DFE58h
022DFDF4 E1D710B2 ldrh    r1,[r7,2h]
022DFDF8 E2840014 add     r0,r4,14h
022DFDFC EB001111 bl      22E4248h                  //GetStringFromScriptsStringTable( strtblptr, stringid)
022DFE00 E1A0A000 mov     r10,r0
022DFE04 EA000019 b       22DFE70h
022DFE08 02316B0C eoreqs  r6,r1,3000h
022DFE0C 02316CD0 eoreqs  r6,r1,0D000h
022DFE10 02316A60 eoreqs  r6,r1,60000h
022DFE14 02319498 eoreqs  r9,r1,98000000h
022DFE18 02324ECC eoreqs  r4,r2,0CC0h
022DFE1C 02324EA4 eoreqs  r4,r2,0A40h
022DFE20 023194B0 eoreqs  r9,r1,0B0000000h
022DFE24 02324E94 eoreqs  r4,r2,940h
022DFE28 023194C0 eoreqs  r9,r1,0C0000000h
022DFE2C 0001002F andeq   r0,r1,r15,lsr 20h
022DFE30 0000025E andeq   r0,r0,r14,asr r2
022DFE34 023194D8 eoreqs  r9,r1,0D8000000h
022DFE38 023194F4 eoreqs  r9,r1,0F4000000h
022DFE3C 02319504 eoreqs  r9,r1,1000000h
022DFE40 0000524C andeq   r5,r0,r12,asr 4h
022DFE44 02324C64 eoreqs  r4,r2,6400h
022DFE48 0000C402 andeq   r12,r0,r2,lsl 8h
022DFE4C 02319518 eoreqs  r9,r1,6000000h
022DFE50 02319534 eoreqs  r9,r1,0D000000h
022DFE54 02319554 eoreqs  r9,r1,15000000h
022DFE58 E1D700B2 ldrh    r0,[r7,2h]
022DFE5C EB001292 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFE60 E28DAF5E add     r10,r13,178h
022DFE64 E1A01000 mov     r1,r0
022DFE68 E1A0000A mov     r0,r10
022DFE6C EB001C59 bl      22E6FD8h
022DFE70 E51F3030 ldr     r3,=0C402h
022DFE74 E58DB000 str     r11,[r13]
022DFE78 E1A00009 mov     r0,r9
022DFE7C E3A01C01 mov     r1,100h
022DFE80 E1A0200A mov     r2,r10
022DFE84 EBF50959 bl      20223F0h
022DFE88 E5951000 ldr     r1,[r5]
022DFE8C E3A00002 mov     r0,2h
022DFE90 E5912004 ldr     r2,[r1,4h]
022DFE94 E51F1050 ldr     r1,=2319518h          //"    case [%3d][%3d] %04x %s"    
022DFE98 E2822001 add     r2,r2,1h
022DFE9C E5892100 str     r2,[r9,100h]
022DFEA0 E1D720B4 ldrh    r2,[r7,4h]
022DFEA4 E5953000 ldr     r3,[r5]
022DFEA8 E88D0404 stmea   [r13],r2,r10
022DFEAC E893000C ldmia   [r3],r2,r3
022DFEB0 EBF4B0E2 bl      200C240h              //Debug Printf
022DFEB4 E5951000 ldr     r1,[r5]
022DFEB8 E2877006 add     r7,r7,6h
022DFEBC E5910000 ldr     r0,[r1]
022DFEC0 E2800001 add     r0,r0,1h
022DFEC4 E5810000 str     r0,[r1]
022DFEC8 E5951000 ldr     r1,[r5]
022DFECC E5910004 ldr     r0,[r1,4h]
022DFED0 E2800001 add     r0,r0,1h
022DFED4 E5810004 str     r0,[r1,4h]
022DFED8 E1D700B0 ldrh    r0,[r7]
022DFEDC E3500064 cmp     r0,64h
022DFEE0 13500065 cmpne   r0,65h
022DFEE4 0AFFFFAA beq     22DFD94h
022DFEE8 E51F00AC ldr     r0,=2324C64h
022DFEEC E584701C str     r7,[r4,1Ch]
022DFEF0 E5900000 ldr     r0,[r0]
022DFEF4 E51F10AC ldr     r1,=2319534h          //"switch menu  sum %3d num %3d"
022DFEF8 E890000C ldmia   [r0],r2,r3
022DFEFC E3A00002 mov     r0,2h
022DFF00 EBF4B0CE bl      200C240h              //Debug Printf
022DFF04 E51F00C8 ldr     r0,=2324C64h
022DFF08 E5900000 ldr     r0,[r0]
022DFF0C E5901000 ldr     r1,[r0]
022DFF10 E3510000 cmp     r1,0h
022DFF14 DA000024 ble     22DFFACh
022DFF18 E1D600B0 ldrh    r0,[r6]
022DFF1C EB001262 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFF20 E1A05000 mov     r5,r0
022DFF24 E1D600B2 ldrh    r0,[r6,2h]
022DFF28 EB00125F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFF2C E1A04000 mov     r4,r0
022DFF30 E51F10E4 ldr     r1,=2319554h          //"    canel %3d  init %3d"
022DFF34 E1A02005 mov     r2,r5
022DFF38 E1A03004 mov     r3,r4
022DFF3C E3A00002 mov     r0,2h
022DFF40 EBF4B0BE bl      200C240h              //Debug Printf
022DFF44 E51F1108 ldr     r1,=2324C64h
022DFF48 E3A02000 mov     r2,0h
022DFF4C E5913000 ldr     r3,[r1]
022DFF50 E3550000 cmp     r5,0h
022DFF54 E5930000 ldr     r0,[r3]
022DFF58 E0800300 add     r0,r0,r0,lsl 6h
022DFF5C E0830100 add     r0,r3,r0,lsl 2h
022DFF60 E5C02008 strb    r2,[r0,8h]
022DFF64 E5911000 ldr     r1,[r1]
022DFF68 E5910000 ldr     r0,[r1]
022DFF6C E0800300 add     r0,r0,r0,lsl 6h
022DFF70 E0810100 add     r0,r1,r0,lsl 2h
022DFF74 E5802108 str     r2,[r0,108h]
022DFF78 E51F113C ldr     r1,=2324C64h
022DFF7C 13A00013 movne   r0,13h
022DFF80 E5913000 ldr     r3,[r1]
022DFF84 03A00011 moveq   r0,11h
022DFF88 E5931000 ldr     r1,[r3]
022DFF8C E1A02004 mov     r2,r4
022DFF90 E3510008 cmp     r1,8h
022DFF94 E2831008 add     r1,r3,8h
022DFF98 C3800902 orrgt   r0,r0,8000h
022DFF9C E3A03014 mov     r3,14h
022DFFA0 EBF5541F bl      2035024h
022DFFA4 E3A00005 mov     r0,5h
022DFFA8 EA000932 b       22E2478h        //Return 022E2478
022DFFAC EBF48475 bl      2001188h
022DFFB0 E51F0174 ldr     r0,=2324C64h
022DFFB4 E3A01000 mov     r1,0h
022DFFB8 E5801000 str     r1,[r0]
022DFFBC EA00092C b       22E2474h
022DFFC0 E1D600B0 ldrh    r0,[r6]
022DFFC4 EB001238 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFFC8 E1A04000 mov     r4,r0
022DFFCC E1D600B2 ldrh    r0,[r6,2h]
022DFFD0 EB001235 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFFD4 EB0026CC bl      22E9B0Ch
022DFFD8 E3540000 cmp     r4,0h
022DFFDC 0A000924 beq     22E2474h
022DFFE0 E3A00003 mov     r0,3h
022DFFE4 EA000923 b       22E2478h        //Return 022E2478
022DFFE8 E1D600B0 ldrh    r0,[r6]
022DFFEC EB00122E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFFF0 E1A04000 mov     r4,r0
022DFFF4 E1D600B2 ldrh    r0,[r6,2h]
022DFFF8 EB00122B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022DFFFC E35500DB cmp     r5,0DBh
022E0000 1A000001 bne     22E000Ch
022E0004 EB0026C6 bl      22E9B24h
022E0008 EA000000 b       22E0010h
022E000C EB0026CB bl      22E9B40h
022E0010 E3540000 cmp     r4,0h
022E0014 0A000916 beq     22E2474h
022E0018 E3A00003 mov     r0,3h
022E001C EA000915 b       22E2478h        //Return 022E2478
022E0020 E1D600B0 ldrh    r0,[r6]
022E0024 EB001220 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0028 E1A07000 mov     r7,r0
022E002C E1D600B2 ldrh    r0,[r6,2h]
022E0030 EB00121D bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0034 E1A04000 mov     r4,r0
022E0038 E1D600B4 ldrh    r0,[r6,4h]
022E003C EB00121A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0040 E1A08000 mov     r8,r0
022E0044 E1D600B6 ldrh    r0,[r6,6h]
022E0048 EB001217 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E004C E35500D7 cmp     r5,0D7h
022E0050 1A000008 bne     22E0078h
022E0054 E2481C01 sub     r1,r8,100h
022E0058 E2400C01 sub     r0,r0,100h
022E005C E1A01801 mov     r1,r1,lsl 10h
022E0060 E1A02800 mov     r2,r0,lsl 10h
022E0064 E1A00004 mov     r0,r4
022E0068 E1A01841 mov     r1,r1,asr 10h
022E006C E1A02842 mov     r2,r2,asr 10h
022E0070 EB0026B9 bl      22E9B5Ch
022E0074 EA000007 b       22E0098h
022E0078 E2681C01 rsb     r1,r8,100h
022E007C E2600C01 rsb     r0,r0,100h
022E0080 E1A01801 mov     r1,r1,lsl 10h
022E0084 E1A02800 mov     r2,r0,lsl 10h
022E0088 E1A00004 mov     r0,r4
022E008C E1A01841 mov     r1,r1,asr 10h
022E0090 E1A02842 mov     r2,r2,asr 10h
022E0094 EB0026B0 bl      22E9B5Ch
022E0098 E3570000 cmp     r7,0h
022E009C 0A0008F4 beq     22E2474h
022E00A0 E3A00003 mov     r0,3h
022E00A4 EA0008F3 b       22E2478h        //Return 022E2478
022E00A8 E1D600B0 ldrh    r0,[r6]
022E00AC EB0011FE bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E00B0 E1A04000 mov     r4,r0
022E00B4 E1D600B2 ldrh    r0,[r6,2h]
022E00B8 EB0011FB bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E00BC EB0026B0 bl      22E9B84h
022E00C0 E3540000 cmp     r4,0h
022E00C4 0A0008EA beq     22E2474h
022E00C8 E3A00003 mov     r0,3h
022E00CC EA0008E9 b       22E2478h        //Return 022E2478
022E00D0 E1D600B0 ldrh    r0,[r6]
022E00D4 EB0011F4 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E00D8 E1A04000 mov     r4,r0
022E00DC E1D600B2 ldrh    r0,[r6,2h]
022E00E0 EB0011F1 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E00E4 E35500DC cmp     r5,0DCh
022E00E8 1A000001 bne     22E00F4h
022E00EC EB0026B7 bl      22E9BD0h
022E00F0 EA000000 b       22E00F8h
022E00F4 EB0026C9 bl      22E9C20h
022E00F8 E3540000 cmp     r4,0h
022E00FC 0A0008DC beq     22E2474h
022E0100 E3A00003 mov     r0,3h
022E0104 EA0008DB b       22E2478h        //Return 022E2478
022E0108 E1D600B0 ldrh    r0,[r6]
022E010C EB0011E6 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0110 E1A07000 mov     r7,r0
022E0114 E1D600B2 ldrh    r0,[r6,2h]
022E0118 EB0011E3 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E011C E1A04000 mov     r4,r0
022E0120 E1D600B4 ldrh    r0,[r6,4h]
022E0124 EB0011E0 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0128 E1A08000 mov     r8,r0
022E012C E1D600B6 ldrh    r0,[r6,6h]
022E0130 EB0011DD bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0134 E35500D8 cmp     r5,0D8h
022E0138 1A000008 bne     22E0160h
022E013C E2481C01 sub     r1,r8,100h
022E0140 E2400C01 sub     r0,r0,100h
022E0144 E1A01801 mov     r1,r1,lsl 10h
022E0148 E1A02800 mov     r2,r0,lsl 10h
022E014C E1A00004 mov     r0,r4
022E0150 E1A01841 mov     r1,r1,asr 10h
022E0154 E1A02842 mov     r2,r2,asr 10h
022E0158 EB0026C4 bl      22E9C70h
022E015C EA000007 b       22E0180h
022E0160 E2681C01 rsb     r1,r8,100h
022E0164 E2600C01 rsb     r0,r0,100h
022E0168 E1A01801 mov     r1,r1,lsl 10h
022E016C E1A02800 mov     r2,r0,lsl 10h
022E0170 E1A00004 mov     r0,r4
022E0174 E1A01841 mov     r1,r1,asr 10h
022E0178 E1A02842 mov     r2,r2,asr 10h
022E017C EB0026BB bl      22E9C70h
022E0180 E3570000 cmp     r7,0h
022E0184 0A0008BA beq     22E2474h
022E0188 E3A00003 mov     r0,3h
022E018C EA0008B9 b       22E2478h        //Return 022E2478
022E0190 E1D600B0 ldrh    r0,[r6]
022E0194 EB0011C4 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0198 E1A04000 mov     r4,r0
022E019C E1D600B2 ldrh    r0,[r6,2h]
022E01A0 EB0011C1 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E01A4 EB0026BB bl      22E9C98h
022E01A8 E3540000 cmp     r4,0h
022E01AC 0A0008B0 beq     22E2474h
022E01B0 E3A00003 mov     r0,3h
022E01B4 EA0008AF b       22E2478h        //Return 022E2478
022E01B8 E1D600B0 ldrh    r0,[r6]
022E01BC EB0011BA bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E01C0 E1A04000 mov     r4,r0
022E01C4 E1D600B2 ldrh    r0,[r6,2h]
022E01C8 EB0011B7 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E01CC E35500EA cmp     r5,0EAh
022E01D0 1A000001 bne     22E01DCh
022E01D4 EB0026B5 bl      22E9CB0h
022E01D8 EA000000 b       22E01E0h
022E01DC EB0026BA bl      22E9CCCh
022E01E0 E3540000 cmp     r4,0h
022E01E4 0A0008A2 beq     22E2474h
022E01E8 E3A00003 mov     r0,3h
022E01EC EA0008A1 b       22E2478h        //Return 022E2478
022E01F0 E1D600B0 ldrh    r0,[r6]
022E01F4 EB0011AC bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E01F8 E1A07000 mov     r7,r0
022E01FC E1D600B2 ldrh    r0,[r6,2h]
022E0200 EB0011A9 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0204 E1A04000 mov     r4,r0
022E0208 E1D600B4 ldrh    r0,[r6,4h]
022E020C EB0011A6 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0210 E1A08000 mov     r8,r0
022E0214 E1D600B6 ldrh    r0,[r6,6h]
022E0218 EB0011A3 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E021C E35500E6 cmp     r5,0E6h
022E0220 1A000008 bne     22E0248h
022E0224 E2481C01 sub     r1,r8,100h
022E0228 E2400C01 sub     r0,r0,100h
022E022C E1A01801 mov     r1,r1,lsl 10h
022E0230 E1A02800 mov     r2,r0,lsl 10h
022E0234 E1A00004 mov     r0,r4
022E0238 E1A01841 mov     r1,r1,asr 10h
022E023C E1A02842 mov     r2,r2,asr 10h
022E0240 EB0026A8 bl      22E9CE8h
022E0244 EA000007 b       22E0268h
022E0248 E2681C01 rsb     r1,r8,100h
022E024C E2600C01 rsb     r0,r0,100h
022E0250 E1A01801 mov     r1,r1,lsl 10h
022E0254 E1A02800 mov     r2,r0,lsl 10h
022E0258 E1A00004 mov     r0,r4
022E025C E1A01841 mov     r1,r1,asr 10h
022E0260 E1A02842 mov     r2,r2,asr 10h
022E0264 EB00269F bl      22E9CE8h
022E0268 E3570000 cmp     r7,0h
022E026C 0A000880 beq     22E2474h
022E0270 E3A00003 mov     r0,3h
022E0274 EA00087F b       22E2478h        //Return 022E2478
022E0278 E1D600B0 ldrh    r0,[r6]
022E027C EB00118A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0280 E1A04000 mov     r4,r0
022E0284 E1D600B2 ldrh    r0,[r6,2h]
022E0288 EB001187 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E028C E1A07000 mov     r7,r0
022E0290 E1D600B4 ldrh    r0,[r6,4h]
022E0294 EB001184 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0298 E1A08000 mov     r8,r0
022E029C E1D600B6 ldrh    r0,[r6,6h]
022E02A0 EB001181 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E02A4 E1A09000 mov     r9,r0
022E02A8 E1D600B8 ldrh    r0,[r6,8h]
022E02AC EB00117E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E02B0 E1A0A000 mov     r10,r0
022E02B4 E1D600BA ldrh    r0,[r6,0Ah]
022E02B8 EB00117B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E02BC E59F6F54 ldr     r6,=2316A44h
022E02C0 E28D30CC add     r3,r13,0CCh
022E02C4 E3A02004 mov     r2,4h
022E02C8 E4D61001 ldrb    r1,[r6],1h
022E02CC E2522001 subs    r2,r2,1h
022E02D0 E4C31001 strb    r1,[r3],1h
022E02D4 1AFFFFFB bne     22E02C8h
022E02D8 E5CD90CC strb    r9,[r13,0CCh]
022E02DC E5CDA0CD strb    r10,[r13,0CDh]
022E02E0 E5CD00CE strb    r0,[r13,0CEh]
022E02E4 E35500ED cmp     r5,0EDh
022E02E8 CA000008 bgt     22E0310h
022E02EC AA000014 bge     22E0344h
022E02F0 E35500DF cmp     r5,0DFh
022E02F4 CA00001B bgt     22E0368h
022E02F8 E35500DE cmp     r5,0DEh
022E02FC BA000019 blt     22E0368h
022E0300 0A000005 beq     22E031Ch
022E0304 E35500DF cmp     r5,0DFh
022E0308 0A000008 beq     22E0330h
022E030C EA000015 b       22E0368h
022E0310 E35500EE cmp     r5,0EEh
022E0314 0A00000F beq     22E0358h
022E0318 EA000012 b       22E0368h
022E031C E28D20CC add     r2,r13,0CCh
022E0320 E1A00008 mov     r0,r8
022E0324 E1A01007 mov     r1,r7
022E0328 EB002685 bl      22E9D44h
022E032C EA00000D b       22E0368h
022E0330 E28D20CC add     r2,r13,0CCh
022E0334 E1A00008 mov     r0,r8
022E0338 E1A01007 mov     r1,r7
022E033C EB00268A bl      22E9D6Ch
022E0340 EA000008 b       22E0368h
022E0344 E28D20CC add     r2,r13,0CCh
022E0348 E1A00008 mov     r0,r8
022E034C E1A01007 mov     r1,r7
022E0350 EB00268F bl      22E9D94h
022E0354 EA000003 b       22E0368h
022E0358 E28D20CC add     r2,r13,0CCh
022E035C E1A00008 mov     r0,r8
022E0360 E1A01007 mov     r1,r7
022E0364 EB002694 bl      22E9DBCh
022E0368 E3540000 cmp     r4,0h
022E036C 0A000840 beq     22E2474h
022E0370 E3A00003 mov     r0,3h
022E0374 EA00083F b       22E2478h        //Return 022E2478
022E0378 E1D600B0 ldrh    r0,[r6]
022E037C EB00114A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0380 E1C404B6 strh    r0,[r4,46h]
022E0384 E3A00003 mov     r0,3h
022E0388 EA00083A b       22E2478h        //Return 022E2478
022E038C E1D600B0 ldrh    r0,[r6]
022E0390 EB001145 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0394 E1A05000 mov     r5,r0
022E0398 E1D600B2 ldrh    r0,[r6,2h]
022E039C EB001142 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E03A0 E0400005 sub     r0,r0,r5
022E03A4 EBF487B2 bl      2002274h
022E03A8 E0850000 add     r0,r5,r0
022E03AC E1C404B6 strh    r0,[r4,46h]
022E03B0 E3A00003 mov     r0,3h
022E03B4 EA00082F b       22E2478h        //Return 022E2478
022E03B8 E3A00003 mov     r0,3h
022E03BC EA00082D b       22E2478h        //Return 022E2478
022E03C0 E3A00003 mov     r0,3h
022E03C4 EA00082B b       22E2478h        //Return 022E2478
022E03C8 E3A00003 mov     r0,3h
022E03CC EA000829 b       22E2478h        //Return 022E2478
022E03D0 E3A00004 mov     r0,4h
022E03D4 EA000827 b       22E2478h        //Return 022E2478
022E03D8 E3A00004 mov     r0,4h
022E03DC EA000825 b       22E2478h        //Return 022E2478
022E03E0 E3A00003 mov     r0,3h
022E03E4 EA000823 b       22E2478h        //Return 022E2478
022E03E8 E3A00004 mov     r0,4h
022E03EC EA000821 b       22E2478h        //Return 022E2478
022E03F0 E3A00004 mov     r0,4h
022E03F4 EA00081F b       22E2478h        //Return 022E2478
022E03F8 E3A00003 mov     r0,3h
022E03FC EA00081D b       22E2478h        //Return 022E2478
022E0400 E2840014 add     r0,r4,14h
022E0404 E2845024 add     r5,r4,24h
022E0408 E890000F ldmia   [r0],r0-r3
022E040C E885000F stmia   [r5],r0-r3
///0x87 JUMP
022E0410 E1D600B0 ldrh    r0,[r6]               //Load parameter in R0, jumpaddress?
022E0414 E5941014 ldr     r1,[r4,14h]           //Load from structr4 + 20
022E0418 E0810080 add     r0,r1,r0,lsl 1h       //R0 = R1 + (jumpaddress * 4)
022E041C E584001C str     r0,[r4,1Ch]           //
022E0420 EA000813 b       22E2474h              //

022E0424 E2840014 add     r0,r4,14h
022E0428 E2845024 add     r5,r4,24h
022E042C E890000F ldmia   [r0],r0-r3
022E0430 E885000F stmia   [r5],r0-r3
022E0434 E1D600B0 ldrh    r0,[r6]
022E0438 EB00111B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E043C E1A05000 mov     r5,r0
022E0440 E59F1DD4 ldr     r1,=231956Ch          //"script call or jump commmon %3d"
022E0444 E1A02005 mov     r2,r5
022E0448 E3A00002 mov     r0,2h
022E044C EBF4AF7B bl      200C240h              //Debug Printf
022E0450 E1A01805 mov     r1,r5,lsl 10h
022E0454 E28D0F57 add     r0,r13,15Ch
022E0458 E1A01841 mov     r1,r1,asr 10h
022E045C EB001ED5 bl      22E7FB8h
022E0460 E3500000 cmp     r0,0h
022E0464 0A000006 beq     22E0484h
022E0468 E59D0160 ldr     r0,[r13,160h]
022E046C E584001C str     r0,[r4,1Ch]
022E0470 E59D015C ldr     r0,[r13,15Ch]
022E0474 E5840014 str     r0,[r4,14h]
022E0478 E59D0164 ldr     r0,[r13,164h]
022E047C E5840020 str     r0,[r4,20h]
022E0480 EA0007FB b       22E2474h
022E0484 E3A00006 mov     r0,6h
022E0488 EA0007FA b       22E2478h        //Return 022E2478
022E048C E594002C ldr     r0,[r4,2Ch]
022E0490 E3500000 cmp     r0,0h
022E0494 03A00000 moveq   r0,0h
022E0498 0A0007F6 beq     22E2478h        //Return 022E2478
022E049C E2840024 add     r0,r4,24h
022E04A0 E2845014 add     r5,r4,14h
022E04A4 E890000F ldmia   [r0],r0-r3
022E04A8 E885000F stmia   [r5],r0-r3
022E04AC E3A00000 mov     r0,0h
022E04B0 E584002C str     r0,[r4,2Ch]
022E04B4 EA0007EE b       22E2474h
022E04B8 E3A00000 mov     r0,0h
022E04BC E584002C str     r0,[r4,2Ch]
022E04C0 EA0007EB b       22E2474h
///022E04C4 0x70 End
022E04C4 E3A00000 mov     r0,0h           //R0 = 0 (Return 0)    
022E04C8 EA0007EA b       22E2478h        //Return 022E2478
///022E04CC 0x82 Hold
022E04CC E3A00001 mov     r0,1h           //R0 = 1 (Returns 1)
022E04D0 EA0007E8 b       22E2478h        //Return 022E2478
022E04D4 E3A00006 mov     r0,6h
022E04D8 EA0007E6 b       22E2478h        //Return 022E2478
///=======================================================================
//R4=,
022E04DC E5946034 ldr     r6,[r4,34h]       //Load current opcode pointer into R6
022E04E0 E1D670B0 ldrh    r7,[r6]           //Load current opcode into R7

022E04E4 E35700C4 cmp     r7,0C4h           
if( Opcode > 0xC4 )
    022E04E8 CA000094 bgt     22E0740h      // 022E0740
else if( Opcode >= 0xC4 )
    022E04EC AA0002CB bge     22E1020h      // 022E1020
022E04F0 E35700B7 cmp     r7,0B7h
if( Opcode > 0xB7 )
    022E04F4 CA000070 bgt     22E06BCh          // 022E06BC
else if( Opcode >= 0xB7 )
    022E04F8 AA0002E5 bge     22E1094h      // 022E1094
022E04FC E35700B2 cmp     r7,0B2h
if( Opcode > 0xB2 )
    022E0500 CA000060 bgt     22E0688h      // 022E0688
else if( Opcode >= 0xB2 )
    022E0504 AA0002D4 bge     22E105Ch      // 022E105C
022E0508 E35700AF cmp     r7,0AFh
if( Opcode > 0xAF )
{
    022E050C CA000056 bgt     22E066Ch      // 022E066C
}
else if( Opcode >= 0xAF )
{
    022E0510 AA0002D1 bge     22E105Ch      // 022E105C
}

//Opcodes < 0xAF
022E0514 E2470039 sub     r0,r7,39h         //R0 = Opcode - 57
022E0518 E3500051 cmp     r0,51h            
if( R0 < 0x51 ) //0x51 == 81				//(Opcode - 0x39) < 0x51
{
    022E051C 908FF100 addls   r15,r15,r0,lsl 2h     // R15 = 022E0520 + ( R0 * 4 ) + 4
}
022E0520 EA00051D b       22E199Ch      // (Opcode - 0x39) > 0x51 -> 022E199C

022E0524 EA0004A1 b       22E17B0h		// (Opcode - 0x39 == 0)  ///0x39
022E0528 EA0004AB b       22E17DCh		// (Opcode - 0x39 == 1)  ///0x3A
022E052C EA00048B b       22E1760h		// (Opcode - 0x39 == 2)  ///0x3B
022E0530 EA0004BA b       22E1820h		// (Opcode - 0x39 == 3)  ///0x3C
022E0534 EA0004B9 b       22E1820h		// (Opcode - 0x39 == 4)  ///0x3D
022E0538 EA0004DA b       22E18A8h		// (Opcode - 0x39 == 5)  ///0x3E
022E053C EA0004E4 b       22E18D4h		// (Opcode - 0x39 == 6)  ///0x3F
022E0540 EA0004C4 b       22E1858h		// (Opcode - 0x39 == 7)  ///0x40
022E0544 EA000514 b       22E199Ch		// (Opcode - 0x39 == 8)  ///0x41
022E0548 EA0004F6 b       22E1928h		// (Opcode - 0x39 == 9)  ///0x42
022E054C EA00045C b       22E16C4h		// (Opcode - 0x39 == 10) ///0x43
022E0550 EA000466 b       22E16F0h		// (Opcode - 0x39 == 11) ///0x44
022E0554 EA000446 b       22E1674h		// (Opcode - 0x39 == 12) ///0x45
022E0558 EA000475 b       22E1734h		// (Opcode - 0x39 == 13) ///0x46
022E055C EA000474 b       22E1734h		// (Opcode - 0x39 == 14) ///0x47
022E0560 EA00041C b       22E15D8h		// (Opcode - 0x39 == 15) ///0x48
022E0564 EA0003FC b       22E155Ch		// (Opcode - 0x39 == 16) ///0x49
022E0568 EA000421 b       22E15F4h		// (Opcode - 0x39 == 17) ///0x4A
022E056C EA00040A b       22E159Ch		// (Opcode - 0x39 == 18) ///0x4B
022E0570 EA00042B b       22E1624h		// (Opcode - 0x39 == 19) ///0x4C
022E0574 EA00048D b       22E17B0h		// (Opcode - 0x39 == 20) ///0x4D
022E0578 EA000497 b       22E17DCh		// (Opcode - 0x39 == 21) ///0x4E
022E057C EA000477 b       22E1760h		// (Opcode - 0x39 == 22) ///0x4F
022E0580 EA0004A6 b       22E1820h		// (Opcode - 0x39 == 23) ///0x50
022E0584 EA0004A5 b       22E1820h		// (Opcode - 0x39 == 24) ///0x51
022E0588 EA0004C6 b       22E18A8h		// (Opcode - 0x39 == 25) ///0x52
022E058C EA0004D0 b       22E18D4h		// (Opcode - 0x39 == 26) ///0x53
022E0590 EA0004B0 b       22E1858h		// (Opcode - 0x39 == 27) ///0x54
022E0594 EA000500 b       22E199Ch		// (Opcode - 0x39 == 28) ///0x55
022E0598 EA0004E2 b       22E1928h		// (Opcode - 0x39 == 29) ///0x56
022E059C EA000448 b       22E16C4h		// (Opcode - 0x39 == 30) ///0x57
022E05A0 EA000452 b       22E16F0h		// (Opcode - 0x39 == 31) ///0x58
022E05A4 EA000432 b       22E1674h		// (Opcode - 0x39 == 32) ///0x59
022E05A8 EA000461 b       22E1734h		// (Opcode - 0x39 == 33) ///0x5A
022E05AC EA000460 b       22E1734h		// (Opcode - 0x39 == 34) ///0x5B
022E05B0 EA000408 b       22E15D8h		// (Opcode - 0x39 == 35) ///0x5C
022E05B4 EA0003E8 b       22E155Ch		// (Opcode - 0x39 == 36) ///0x5D
022E05B8 EA00040D b       22E15F4h		// (Opcode - 0x39 == 37) ///0x5E
022E05BC EA0003F6 b       22E159Ch		// (Opcode - 0x39 == 38) ///0x5F
022E05C0 EA000417 b       22E1624h		// (Opcode - 0x39 == 39) ///0x60
022E05C4 EA0004F4 b       22E199Ch		// (Opcode - 0x39 == 40) ///0x61
022E05C8 EA0004F3 b       22E199Ch		// (Opcode - 0x39 == 41) ///0x62
022E05CC EA0004F2 b       22E199Ch		// (Opcode - 0x39 == 42) ///0x63 Case 						022E199C
022E05D0 EA0004F1 b       22E199Ch		// (Opcode - 0x39 == 43) ///0x64 CaseMenu 					022E199C
022E05D4 EA0004F0 b       22E199Ch		// (Opcode - 0x39 == 44) ///0x65 CaseMenu2 					022E199C
022E05D8 EA0004EF b       22E199Ch		// (Opcode - 0x39 == 45) ///0x66 CaseScenario 				022E199C
022E05DC EA0004EE b       22E199Ch		// (Opcode - 0x39 == 46) ///0x67 CaseText 					022E199C
022E05E0 EA0004ED b       22E199Ch		// (Opcode - 0x39 == 47) ///0x68 CaseValue 					022E199C
022E05E4 EA0004EC b       22E199Ch		// (Opcode - 0x39 == 48) ///0x69 CaseVariable 				022E199C
022E05E8 EA0004EB b       22E199Ch		// (Opcode - 0x39 == 49) ///0x6A debug_Assert 				022E199C
022E05EC EA0004EA b       22E199Ch		// (Opcode - 0x39 == 50) ///0x6B debug_Print 				022E199C
022E05F0 EA0004E9 b       22E199Ch		// (Opcode - 0x39 == 51) ///0x6C debug_PrintFlag 			022E199C
022E05F4 EA0004E8 b       22E199Ch		// (Opcode - 0x39 == 52) ///0x6D debug_PrintScenario 		022E199C
022E05F8 EA0004E7 b       22E199Ch		// (Opcode - 0x39 == 53) ///0x6E DefaultText 				022E199C
022E05FC EA0004E6 b       22E199Ch		// (Opcode - 0x39 == 54) ///0x6F Destroy 					022E199C
022E0600 EA0004E5 b       22E199Ch		// (Opcode - 0x39 == 55) ///0x70 End 						022E199C
022E0604 EA00014E b       22E0B44h		// (Opcode - 0x39 == 56) ///0x71 EndAnimation 				022E0B44
022E0608 EA0004E3 b       22E199Ch		// (Opcode - 0x39 == 57) ///0x72 ExecuteActing 				022E199C
022E060C EA0004E2 b       22E199Ch		// (Opcode - 0x39 == 58) ///0x73 ExecuteCommon 				022E199C
022E0610 EA0004E1 b       22E199Ch		// (Opcode - 0x39 == 59) ///0x74 flag_CalcBit 				022E199C
022E0614 EA0004E0 b       22E199Ch		// (Opcode - 0x39 == 60) ///0x75 flag_CalcValue 			022E199C
022E0618 EA0004DF b       22E199Ch		// (Opcode - 0x39 == 61) ///0x76 flag_CalcVariable 			022E199C
022E061C EA0004DE b       22E199Ch		// (Opcode - 0x39 == 62) ///0x77 flag_Clear 				022E199C
022E0620 EA0004DD b       22E199Ch		// (Opcode - 0x39 == 63) ///0x78 flag_Initial 				022E199C
022E0624 EA0004DC b       22E199Ch		// (Opcode - 0x39 == 64) ///0x79 flag_Set 					022E199C
022E0628 EA0004DB b       22E199Ch		// (Opcode - 0x39 == 65) ///0x7A flag_ResetDungeonResult 	022E199C
022E062C EA0004DA b       22E199Ch		// (Opcode - 0x39 == 66) ///0x7B flag_ResetScenario 		022E199C
022E0630 EA0004D9 b       22E199Ch		// (Opcode - 0x39 == 67) ///0x7C flag_SetAdventureLog 		022E199C
022E0634 EA0004D8 b       22E199Ch		// (Opcode - 0x39 == 68) ///0x7D flag_SetDungeonMode 		022E199C
022E0638 EA0004D7 b       22E199Ch		// (Opcode - 0x39 == 69) ///0x7E flag_SetDungeonResult 		022E199C
022E063C EA0004D6 b       22E199Ch		// (Opcode - 0x39 == 70) ///0x7F flag_SetPerformance 		022E199C
022E0640 EA0004D5 b       22E199Ch		// (Opcode - 0x39 == 71) ///0x80 flag_SetScenario			022E199C
022E0644 EA0004D4 b       22E199Ch		// (Opcode - 0x39 == 72) ///0x80 Flash						022E199C
022E0648 EA0004D3 b       22E199Ch		// (Opcode - 0x39 == 73) ///0x81 Hold						022E199C
022E064C EA0004D2 b       22E199Ch		// (Opcode - 0x39 == 74) ///0x82 item_GetVariable			022E199C
022E0650 EA0004D1 b       22E199Ch		// (Opcode - 0x39 == 75) ///0x83 item_Set					022E199C
022E0654 EA0004D0 b       22E199Ch		// (Opcode - 0x39 == 76) ///0x84 item_SetTableData			022E199C
022E0658 EA0004CF b       22E199Ch		// (Opcode - 0x39 == 77) ///0x85 item_SetVariable			022E199C
022E065C EA0004CE b       22E199Ch		// (Opcode - 0x39 == 78) ///0x86 Jump						022E199C
022E0660 EA0004CD b       22E199Ch		// (Opcode - 0x39 == 79) ///0x87 Jump						022E199C
022E0664 EA0004CC b       22E199Ch		// (Opcode - 0x39 == 80) ///0x88 JumpCommon					022E199C
022E0668 EA00019E b       22E0CE8h		// (Opcode - 0x39 == 81) ///0x89 lives						022E0CE8
///===========================================================
022E066C E35700B0 cmp     r7,0B0h
022E0670 CA000001 bgt     22E067Ch
022E0674 0A000278 beq     22E105Ch
022E0678 EA0004C7 b       22E199Ch
022E067C E35700B1 cmp     r7,0B1h
022E0680 0A000275 beq     22E105Ch
022E0684 EA0004C4 b       22E199Ch
///Opcode > 0xB2
022E0688 E35700B4 cmp     r7,0B4h
if( CurrentOpCode > 0xB4 )
    022E068C CA000003 bgt     22E06A0h
else if( CurrentOpCode >= 0xB4 )
    022E0690 AA000271 bge     22E105Ch
022E0694 E35700B3 cmp     r7,0B3h
if( CurrentOpCode == 0xB3 )
    022E0698 0A00026F beq     22E105Ch              ///022E105C
else
    022E069C EA0004BE b       22E199Ch
022E06A0 E35700B5 cmp     r7,0B5h
022E06A4 CA000001 bgt     22E06B0h
022E06A8 0A00026B beq     22E105Ch
022E06AC EA0004BA b       22E199Ch
022E06B0 E35700B6 cmp     r7,0B6h
022E06B4 0A000276 beq     22E1094h
022E06B8 EA0004B7 b       22E199Ch
//OpCode > 0x87
022E06BC E35700BE cmp     r7,0BEh
if( CurrentOpCode > 0xBE )
    022E06C0 CA00000D bgt     22E06FCh
else if( CurrentOpCode >= 0xBE )
    022E06C4 AA00038B bge     22E14F8h
022E06C8 E35700BB cmp     r7,0BBh
if( CurrentOpCode > 0xBB )
    022E06CC CA000003 bgt     22E06E0h
else if( CurrentOpCode >= 0xBB )
    022E06D0 AA00026F bge     22E1094h
022E06D4 E35700B9 cmp     r7,0B9h
if(CurrentOpCode == 0xB9)
    022E06D8 0A00026D beq     22E1094h
022E06DC EA0004AE b       22E199Ch 					///022E199C
022E06E0 E35700BC cmp     r7,0BCh
022E06E4 CA000001 bgt     22E06F0h
022E06E8 0A000269 beq     22E1094h
022E06EC EA0004AA b       22E199Ch
022E06F0 E35700BD cmp     r7,0BDh
022E06F4 0A000349 beq     22E1420h
022E06F8 EA0004A7 b       22E199Ch
022E06FC E35700C1 cmp     r7,0C1h
022E0700 CA000007 bgt     22E0724h
022E0704 AA000245 bge     22E1020h
022E0708 E35700BF cmp     r7,0BFh
022E070C CA000001 bgt     22E0718h
022E0710 0A000242 beq     22E1020h
022E0714 EA0004A0 b       22E199Ch
022E0718 E35700C0 cmp     r7,0C0h
022E071C 0A00023F beq     22E1020h
022E0720 EA00049D b       22E199Ch
022E0724 E35700C2 cmp     r7,0C2h
022E0728 CA000001 bgt     22E0734h
022E072C 0A00023B beq     22E1020h
022E0730 EA000499 b       22E199Ch
022E0734 E35700C3 cmp     r7,0C3h
022E0738 0A000238 beq     22E1020h
022E073C EA000496 b       22E199Ch
///CurrentOpCode > 0xC4
022E0740 E35700D2 cmp     r7,0D2h               
if( CurrentOpCode > 0xD2 )
    022E0744 CA00001D bgt     22E07C0h              ///022E07C0
else if( CurrentOpCode >= 0xD2 )
    022E0748 AA0000BC bge     22E0A40h              ///022E0A40
022E074C E35700CA cmp     r7,0CAh
if(  CurrentOpCode > 0xCA )
    022E0750 CA00000D bgt     22E078Ch
else if( CurrentOpCode >= 0xD2 )
    022E0754 AA00013C bge     22E0C4Ch
022E0758 E35700C6 cmp     r7,0C6h
if( CurrentOpCode > 0xC6 )
    022E075C CA000003 bgt     22E0770h
else if( CurrentOpCode >= 0xC6 )
    022E0760 AA00022E bge     22E1020h
022E0764 E35700C5 cmp     r7,0C5h
if( CurrentOpCode == 0xC5 )
    022E0768 0A00022C beq     22E1020h              //022E1020
022E076C EA00048A b       22E199Ch
022E0770 E35700C7 cmp     r7,0C7h
022E0774 CA000001 bgt     22E0780h
022E0778 0A000368 beq     22E1520h
022E077C EA000486 b       22E199Ch
022E0780 E35700C8 cmp     r7,0C8h
022E0784 0A000335 beq     22E1460h
022E0788 EA000483 b       22E199Ch
022E078C E35700CE cmp     r7,0CEh
022E0790 CA000003 bgt     22E07A4h
022E0794 AA00034A bge     22E14C4h
022E0798 E35700CD cmp     r7,0CDh
022E079C 0A000348 beq     22E14C4h
022E07A0 EA00047D b       22E199Ch
022E07A4 E35700D0 cmp     r7,0D0h
022E07A8 CA000001 bgt     22E07B4h
022E07AC 0A000066 beq     22E094Ch
022E07B0 EA000479 b       22E199Ch
022E07B4 E35700D1 cmp     r7,0D1h
022E07B8 0A00007C beq     22E09B0h
022E07BC EA000476 b       22E199Ch
///CurrentOpCode > 0xD2
022E07C0 E35700FF cmp     r7,0FFh               ///
if( CurrentOpCode > 0xFF )
    022E07C4 CA00000D bgt     22E0800h              ///022E0800
else if( CurrentOpCode >= 0xFF )
    022E07C8 AA0000EA bge     22E0B78h              ///022E0B78
022E07CC E35700D4 cmp     r7,0D4h
if( CurrentOpCode > 0xD4 )
    022E07D0 CA000003 bgt     22E07E4h              ///022E07E4
else if( CurrentOpCode >= 0xD4 )
    022E07D4 AA000127 bge     22E0C78h              ///022E0C78
022E07D8 E35700D3 cmp     r7,0D3h
022E07DC 0A000085 beq     22E09F8h
022E07E0 EA00046D b       22E199Ch
///CurrentOpCode > 0xD4
022E07E4 E35700D6 cmp     r7,0D6h
if( CurrentOpCode > 0xD6 )
    022E07E8 CA000001 bgt     22E07F4h              ///022E07F4
else if( CurrentOpCode == 0xD6 )
    022E07EC 0A00016F beq     22E0DB0h              ///022E0DB0
022E07F0 EA000469 b       22E199Ch
///CurrentOpCode > 0xD6
022E07F4 E35700FD cmp     r7,0FDh    
if( CurrentOpCode == 0xFD )   
    022E07F8 0A000099 beq     22E0A64h              ///022E0A64
else
    022E07FC EA000466 b       22E199Ch              ///022E199C
///=============================================================================== 0x128
022E0800 E59F1A18 ldr     r1,=129h          //297
022E0804 E1570001 cmp     r7,r1
if( CurrentOpCode > 0x129 )
    022E0808 CA000033 bgt     22E08DCh          //022E08DC
022E080C E2410027 sub     r0,r1,27h
022E0810 E0570000 subs    r0,r7,r0
022E08E8 E2570F53 subs    r0,r7,14Ch            //R0 = CurrentOpCode - 0x14C    
if( R0 > 0 )                                    //the pl suffix means negative flag cleared!
    022E0814 508FF100 addpl   r15,r15,r0,lsl 2h
022E0818 EA000027 b       22E08BCh
022E081C EA0000F2 b       22E0BECh
022E0820 EA000041 b       22E092Ch
022E0824 EA0001F5 b       22E1000h
022E0828 EA000057 b       22E098Ch
022E082C EA00011C b       22E0CA4h
022E0830 EA000079 b       22E0A1Ch
022E0834 EA000194 b       22E0E8Ch
022E0838 EA000188 b       22E0E60h
022E083C EA0001D2 b       22E0F8Ch
022E0840 EA0001E0 b       22E0FC8h
022E0844 EA0001A1 b       22E0ED0h
022E0848 EA0001B1 b       22E0F14h
022E084C EA000060 b       22E09D4h
022E0850 EA000094 b       22E0AA8h
022E0854 EA000200 b       22E105Ch
022E0858 EA0001FF b       22E105Ch
022E085C EA0001FE b       22E105Ch
022E0860 EA0001FD b       22E105Ch
022E0864 EA0001FC b       22E105Ch
022E0868 EA0001FB b       22E105Ch
022E086C EA0001FA b       22E105Ch
022E0870 EA000207 b       22E1094h
022E0874 EA000206 b       22E1094h
022E0878 EA000447 b       22E199Ch
022E087C EA000204 b       22E1094h
022E0880 EA000445 b       22E199Ch
022E0884 EA000202 b       22E1094h
022E0888 EA000201 b       22E1094h
022E088C EA000319 b       22E14F8h
022E0890 EA0001E2 b       22E1020h
022E0894 EA0001E1 b       22E1020h
022E0898 EA0001E0 b       22E1020h
022E089C EA0001DF b       22E1020h
022E08A0 EA0001DE b       22E1020h
022E08A4 EA0001DD b       22E1020h
022E08A8 EA0001DC b       22E1020h
022E08AC EA0001DB b       22E1020h
022E08B0 EA000439 b       22E199Ch
///===================================================================== 0x128
022E08B4 EA000438 b       22E199Ch      //022E199C
022E08B8 EA00009B b       22E0B2Ch
022E08BC E3570C01 cmp     r7,100h
022E08C0 CA000001 bgt     22E08CCh
022E08C4 0A000201 beq     22E10D0h
022E08C8 EA000433 b       22E199Ch
022E08CC E59F0950 ldr     r0,=101h
022E08D0 E1570000 cmp     r7,r0
022E08D4 0A000206 beq     22E10F4h
022E08D8 EA00042F b       22E199Ch
//Values bigger than 0x129
022E08DC E281002C add     r0,r1,2Ch             //R0 = 0x129 + 0x2C
022E08E0 E1570000 cmp     r7,r0                 //Opcode > 0x155
if( CurrentOpCode > 0x155 )
    022E08E4 CA00000C bgt     22E091Ch              ///022E091C
022E08E8 E2570F53 subs    r0,r7,14Ch            //R0 = CurrentOpCode - 0x14C    
if( R0 > 0 )                                    //the pl suffix means negative flag cleared!
    022E08EC 508FF100 addpl   r15,r15,r0,lsl 2h
022E08F0 EA000429 b       22E199Ch
022E08F4 EA00028E b       22E1334h                  //0x14C
022E08F8 EA0002A4 b       22E1390h                  //0x14D - Turn2DirectionLives 022E1390
022E08FC EA0002A3 b       22E1390h                  //0x14E
022E0900 EA0002A6 b       22E13A0h                  //0x14F
022E0904 EA000291 b       22E1350h                  //0x150
022E0908 EA0002A8 b       22E13B0h                  //0x151
022E090C EA00020F b       22E1150h                  //0x152
022E0910 EA000220 b       22E1198h                  //0x153
022E0914 EA00021F b       22E1198h                  //0x154
022E0918 EA00021E b       22E1198h                  //0x155
//Values bigger than 0x155
022E091C E59F0904 ldr     r0,=15Fh
022E0920 E1570000 cmp     r7,r0                 
022E0924 0A00008C beq     22E0B5Ch                  ///022E0B5C OpCode == 0x15F
022E0928 EA00041B b       22E199Ch                  ///022E199C Opcode != 0x15F

022E092C E1D600B2 ldrh    r0,[r6,2h]
022E0930 EB000FDD bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0934 E5942000 ldr     r2,[r4]
022E0938 E1A01000 mov     r1,r0
022E093C E5940004 ldr     r0,[r4,4h]
022E0940 E5922038 ldr     r2,[r2,38h]
022E0944 E12FFF32 blx     r2
022E0948 EA0006C7 b       22E246Ch
022E094C E1D600B2 ldrh    r0,[r6,2h]
022E0950 EB000FD5 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0954 E1A05000 mov     r5,r0
022E0958 E3150010 tst     r5,10h
022E095C 0A000004 beq     22E0974h
022E0960 E1D407FC ldrsh   r0,[r4,7Ch]
022E0964 E3500000 cmp     r0,0h
022E0968 1A000001 bne     22E0974h
022E096C E284007C add     r0,r4,7Ch
022E0970 EBFFEFAE bl      22DC830h
022E0974 E5941000 ldr     r1,[r4]
022E0978 E5940004 ldr     r0,[r4,4h]
022E097C E591203C ldr     r2,[r1,3Ch]
022E0980 E1A01005 mov     r1,r5
022E0984 E12FFF32 blx     r2
022E0988 EA0006B7 b       22E246Ch
022E098C E1D600B2 ldrh    r0,[r6,2h]
022E0990 EB000FC5 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0994 E1A03000 mov     r3,r0
022E0998 E5941000 ldr     r1,[r4]
022E099C E5940004 ldr     r0,[r4,4h]
022E09A0 E5912038 ldr     r2,[r1,38h]
022E09A4 E1A01303 mov     r1,r3,lsl 6h
022E09A8 E12FFF32 blx     r2
022E09AC EA0006AE b       22E246Ch
022E09B0 E1D600B2 ldrh    r0,[r6,2h]
022E09B4 EB000FBC bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E09B8 E1A03000 mov     r3,r0
022E09BC E5941000 ldr     r1,[r4]
022E09C0 E5940004 ldr     r0,[r4,4h]
022E09C4 E591203C ldr     r2,[r1,3Ch]
022E09C8 E1A01303 mov     r1,r3,lsl 6h
022E09CC E12FFF32 blx     r2
022E09D0 EA0006A5 b       22E246Ch
022E09D4 E1D600B2 ldrh    r0,[r6,2h]
022E09D8 EB000FB3 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E09DC E1A03000 mov     r3,r0
022E09E0 E5941000 ldr     r1,[r4]
022E09E4 E5940004 ldr     r0,[r4,4h]
022E09E8 E5912038 ldr     r2,[r1,38h]
022E09EC E1A01583 mov     r1,r3,lsl 0Bh
022E09F0 E12FFF32 blx     r2
022E09F4 EA00069C b       22E246Ch
022E09F8 E1D600B2 ldrh    r0,[r6,2h]
022E09FC EB000FAA bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0A00 E1A03000 mov     r3,r0
022E0A04 E5941000 ldr     r1,[r4]
022E0A08 E5940004 ldr     r0,[r4,4h]
022E0A0C E591203C ldr     r2,[r1,3Ch]
022E0A10 E1A01583 mov     r1,r3,lsl 0Bh
022E0A14 E12FFF32 blx     r2
022E0A18 EA000693 b       22E246Ch
022E0A1C E1D600B2 ldrh    r0,[r6,2h]
022E0A20 EB000FA1 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0A24 E1A03000 mov     r3,r0
022E0A28 E5941000 ldr     r1,[r4]
022E0A2C E5940004 ldr     r0,[r4,4h]
022E0A30 E5912038 ldr     r2,[r1,38h]
022E0A34 E1A01803 mov     r1,r3,lsl 10h
022E0A38 E12FFF32 blx     r2
022E0A3C EA00068A b       22E246Ch
022E0A40 E1D600B2 ldrh    r0,[r6,2h]
022E0A44 EB000F98 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0A48 E1A03000 mov     r3,r0
022E0A4C E5941000 ldr     r1,[r4]
022E0A50 E5940004 ldr     r0,[r4,4h]
022E0A54 E591203C ldr     r2,[r1,3Ch]
022E0A58 E1A01803 mov     r1,r3,lsl 10h
022E0A5C E12FFF32 blx     r2
022E0A60 EA000681 b       22E246Ch
///CurrentOpCode == 0xFD (SetAnimation)
022E0A64 E1D600B2 ldrh    r0,[r6,2h]
022E0A68 EB000F8F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0A6C E5941000 ldr     r1,[r4]
022E0A70 E1A05000 mov     r5,r0
022E0A74 E5912014 ldr     r2,[r1,14h]
022E0A78 E5940004 ldr     r0,[r4,4h]
022E0A7C E2841042 add     r1,r4,42h
022E0A80 E12FFF32 blx     r2
022E0A84 E3550000 cmp     r5,0h
022E0A88 11C454B0 strneh  r5,[r4,40h]
022E0A8C E5942000 ldr     r2,[r4]
022E0A90 E1A01805 mov     r1,r5,lsl 10h
022E0A94 E5940004 ldr     r0,[r4,4h]
022E0A98 E5922030 ldr     r2,[r2,30h]
022E0A9C E1A01821 mov     r1,r1,lsr 10h
022E0AA0 E12FFF32 blx     r2
022E0AA4 EA000670 b       22E246Ch
022E0AA8 E1D600B2 ldrh    r0,[r6,2h]
022E0AAC EB000F7E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0AB0 E1A07000 mov     r7,r0
022E0AB4 E1D600B4 ldrh    r0,[r6,4h]
022E0AB8 EB000F7B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0ABC E1A05000 mov     r5,r0
022E0AC0 E1D600B6 ldrh    r0,[r6,6h]
022E0AC4 EB000F78 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0AC8 E5941000 ldr     r1,[r4]
022E0ACC E1A06000 mov     r6,r0
022E0AD0 E5912038 ldr     r2,[r1,38h]
022E0AD4 E5940004 ldr     r0,[r4,4h]
022E0AD8 E1A01807 mov     r1,r7,lsl 10h
022E0ADC E12FFF32 blx     r2
022E0AE0 E5941000 ldr     r1,[r4]
022E0AE4 E5940004 ldr     r0,[r4,4h]
022E0AE8 E591203C ldr     r2,[r1,3Ch]
022E0AEC E1A01805 mov     r1,r5,lsl 10h
022E0AF0 E12FFF32 blx     r2
022E0AF4 E5941000 ldr     r1,[r4]
022E0AF8 E5940004 ldr     r0,[r4,4h]
022E0AFC E5912014 ldr     r2,[r1,14h]
022E0B00 E2841042 add     r1,r4,42h
022E0B04 E12FFF32 blx     r2
022E0B08 E3560000 cmp     r6,0h
022E0B0C 11C464B0 strneh  r6,[r4,40h]
022E0B10 E5942000 ldr     r2,[r4]
022E0B14 E1A01806 mov     r1,r6,lsl 10h
022E0B18 E5940004 ldr     r0,[r4,4h]
022E0B1C E5922030 ldr     r2,[r2,30h]
022E0B20 E1A01821 mov     r1,r1,lsr 10h
022E0B24 E12FFF32 blx     r2
022E0B28 EA00064F b       22E246Ch
022E0B2C E5941000 ldr     r1,[r4]
022E0B30 E5940004 ldr     r0,[r4,4h]
022E0B34 E5912030 ldr     r2,[r1,30h]
022E0B38 E3A01C01 mov     r1,100h
022E0B3C E12FFF32 blx     r2
022E0B40 EA000649 b       22E246Ch
022E0B44 E5941000 ldr     r1,[r4]
022E0B48 E5940004 ldr     r0,[r4,4h]
022E0B4C E5912030 ldr     r2,[r1,30h]
022E0B50 E3A01B02 mov     r1,800h
022E0B54 E12FFF32 blx     r2
022E0B58 EA000643 b       22E246Ch
022E0B5C E5941000 ldr     r1,[r4]
022E0B60 E5940004 ldr     r0,[r4,4h]
022E0B64 E5912030 ldr     r2,[r1,30h]
022E0B68 E3A01B02 mov     r1,800h
022E0B6C E12FFF32 blx     r2
022E0B70 E3A00003 mov     r0,3h
022E0B74 EA00063F b       22E2478h        //Return 022E2478
022E0B78 E1D600B2 ldrh    r0,[r6,2h]
022E0B7C EB000F4A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0B80 E1A05000 mov     r5,r0
022E0B84 E1D600B4 ldrh    r0,[r6,4h]
022E0B88 EB000F47 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0B8C E1A02000 mov     r2,r0
022E0B90 E3520000 cmp     r2,0h
022E0B94 E5941000 ldr     r1,[r4]
022E0B98 E5940004 ldr     r0,[r4,4h]
022E0B9C CA000009 bgt     22E0BC8h
022E0BA0 E591203C ldr     r2,[r1,3Ch]
022E0BA4 E3A01602 mov     r1,200000h
022E0BA8 E12FFF32 blx     r2
022E0BAC E5942000 ldr     r2,[r4]
022E0BB0 E3A01000 mov     r1,0h
022E0BB4 E5923040 ldr     r3,[r2,40h]
022E0BB8 E5940004 ldr     r0,[r4,4h]
022E0BBC E1A02001 mov     r2,r1
022E0BC0 E12FFF33 blx     r3
022E0BC4 EA000628 b       22E246Ch
022E0BC8 E5913040 ldr     r3,[r1,40h]
022E0BCC E1A01005 mov     r1,r5
022E0BD0 E12FFF33 blx     r3
022E0BD4 E5941000 ldr     r1,[r4]
022E0BD8 E5940004 ldr     r0,[r4,4h]
022E0BDC E5912038 ldr     r2,[r1,38h]
022E0BE0 E3A01602 mov     r1,200000h
022E0BE4 E12FFF32 blx     r2
022E0BE8 EA00061F b       22E246Ch
022E0BEC E1D600B2 ldrh    r0,[r6,2h]
022E0BF0 EB000F2D bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0BF4 E1A05000 mov     r5,r0
022E0BF8 E1D600B4 ldrh    r0,[r6,4h]
022E0BFC EB000F2A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0C00 E1A03000 mov     r3,r0
022E0C04 E3550000 cmp     r5,0h
022E0C08 0A000007 beq     22E0C2Ch
022E0C0C E5942000 ldr     r2,[r4]
022E0C10 E1A01805 mov     r1,r5,lsl 10h
022E0C14 E5940004 ldr     r0,[r4,4h]
022E0C18 E5924034 ldr     r4,[r2,34h]
022E0C1C E1A02841 mov     r2,r1,asr 10h
022E0C20 E3A01000 mov     r1,0h
022E0C24 E12FFF34 blx     r4
022E0C28 EA00060F b       22E246Ch
022E0C2C E5941000 ldr     r1,[r4]
022E0C30 E3A02000 mov     r2,0h
022E0C34 E5940004 ldr     r0,[r4,4h]
022E0C38 E5914034 ldr     r4,[r1,34h]
022E0C3C E1A03002 mov     r3,r2
022E0C40 E3A01001 mov     r1,1h
022E0C44 E12FFF34 blx     r4
022E0C48 EA000607 b       22E246Ch
022E0C4C E1D600B2 ldrh    r0,[r6,2h]
022E0C50 EB000F15 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0C54 E1A01800 mov     r1,r0,lsl 10h
022E0C58 E5942000 ldr     r2,[r4]
022E0C5C E5940004 ldr     r0,[r4,4h]
022E0C60 E5924034 ldr     r4,[r2,34h]
022E0C64 E1A02841 mov     r2,r1,asr 10h
022E0C68 E3A01002 mov     r1,2h
022E0C6C E3A03000 mov     r3,0h
022E0C70 E12FFF34 blx     r4
022E0C74 EA0005FC b       22E246Ch
022E0C78 E1D600B2 ldrh    r0,[r6,2h]
022E0C7C EB000F0A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0C80 E1A01800 mov     r1,r0,lsl 10h
022E0C84 E5942000 ldr     r2,[r4]
022E0C88 E5940004 ldr     r0,[r4,4h]
022E0C8C E5924034 ldr     r4,[r2,34h]
022E0C90 E1A02841 mov     r2,r1,asr 10h
022E0C94 E3A01003 mov     r1,3h
022E0C98 E3A03000 mov     r3,0h
022E0C9C E12FFF34 blx     r4
022E0CA0 EA0005F1 b       22E246Ch
022E0CA4 E5941000 ldr     r1,[r4]
022E0CA8 E5940004 ldr     r0,[r4,4h]
022E0CAC E591200C ldr     r2,[r1,0Ch]
022E0CB0 E28D1010 add     r1,r13,10h
022E0CB4 E12FFF32 blx     r2
022E0CB8 E28D1018 add     r1,r13,18h
022E0CBC E28D2020 add     r2,r13,20h
022E0CC0 E28D3010 add     r3,r13,10h
022E0CC4 E2860002 add     r0,r6,2h
022E0CC8 EB000F2D bl      22E4984h
022E0CCC E5941000 ldr     r1,[r4]
022E0CD0 E5940004 ldr     r0,[r4,4h]
022E0CD4 E5913020 ldr     r3,[r1,20h]
022E0CD8 E28D1018 add     r1,r13,18h
022E0CDC E28D2020 add     r2,r13,20h
022E0CE0 E12FFF33 blx     r3
022E0CE4 EA0005E0 b       22E246Ch
022E0CE8 E1D600B2 ldrh    r0,[r6,2h]
022E0CEC EB000EEE bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0CF0 E59F1534 ldr     r1,=2316A44h
022E0CF4 E1A05000 mov     r5,r0
022E0CF8 E1A00805 mov     r0,r5,lsl 10h
022E0CFC E5913010 ldr     r3,[r1,10h]
022E0D00 E591600C ldr     r6,[r1,0Ch]
022E0D04 E1A02820 mov     r2,r0,lsr 10h
022E0D08 E3A00000 mov     r0,0h
022E0D0C E3A01058 mov     r1,58h
022E0D10 E58D6028 str     r6,[r13,28h]
022E0D14 E58D302C str     r3,[r13,2Ch]
022E0D18 EBF5AA56 bl      204B678h
022E0D1C E1A01805 mov     r1,r5,lsl 10h
022E0D20 E58D0028 str     r0,[r13,28h]
022E0D24 E1A02821 mov     r2,r1,lsr 10h
022E0D28 E3A00000 mov     r0,0h
022E0D2C E3A01059 mov     r1,59h
022E0D30 EBF5AA50 bl      204B678h
022E0D34 E1A01805 mov     r1,r5,lsl 10h
022E0D38 E58D002C str     r0,[r13,2Ch]
022E0D3C E1A02821 mov     r2,r1,lsr 10h
022E0D40 E3A00000 mov     r0,0h
022E0D44 E3A0105A mov     r1,5Ah
022E0D48 EBF5AA4A bl      204B678h
022E0D4C E1A01805 mov     r1,r5,lsl 10h
022E0D50 E1A02821 mov     r2,r1,lsr 10h
022E0D54 E1A05000 mov     r5,r0
022E0D58 E3A00000 mov     r0,0h
022E0D5C E3A0105B mov     r1,5Bh
022E0D60 EBF5AA44 bl      204B678h
022E0D64 E5942000 ldr     r2,[r4]
022E0D68 E1A00C00 mov     r0,r0,lsl 18h
022E0D6C E1A06C40 mov     r6,r0,asr 18h
022E0D70 E5940004 ldr     r0,[r4,4h]
022E0D74 E5922024 ldr     r2,[r2,24h]
022E0D78 E28D1028 add     r1,r13,28h
022E0D7C E12FFF32 blx     r2
022E0D80 E5942000 ldr     r2,[r4]
022E0D84 E5940004 ldr     r0,[r4,4h]
022E0D88 E5922028 ldr     r2,[r2,28h]
022E0D8C E1A01005 mov     r1,r5
022E0D90 E12FFF32 blx     r2
022E0D94 E5C46042 strb    r6,[r4,42h]
022E0D98 E5942000 ldr     r2,[r4]
022E0D9C E5940004 ldr     r0,[r4,4h]
022E0DA0 E592202C ldr     r2,[r2,2Ch]
022E0DA4 E1A01006 mov     r1,r6
022E0DA8 E12FFF32 blx     r2
022E0DAC EA0005AE b       22E246Ch
022E0DB0 E1D600B2 ldrh    r0,[r6,2h]
022E0DB4 EB000EBC bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0DB8 E5941000 ldr     r1,[r4]
022E0DBC E1A05000 mov     r5,r0
022E0DC0 E591200C ldr     r2,[r1,0Ch]
022E0DC4 E5940004 ldr     r0,[r4,4h]
022E0DC8 E28D1030 add     r1,r13,30h
022E0DCC E12FFF32 blx     r2
022E0DD0 E5941000 ldr     r1,[r4]
022E0DD4 E5940004 ldr     r0,[r4,4h]
022E0DD8 E5913010 ldr     r3,[r1,10h]
022E0DDC E28D1038 add     r1,r13,38h
022E0DE0 E28D203C add     r2,r13,3Ch
022E0DE4 E12FFF33 blx     r3
022E0DE8 E5942000 ldr     r2,[r4]
022E0DEC E5940004 ldr     r0,[r4,4h]
022E0DF0 E5922014 ldr     r2,[r2,14h]
022E0DF4 E28D1008 add     r1,r13,8h
022E0DF8 E12FFF32 blx     r2
022E0DFC E1A02805 mov     r2,r5,lsl 10h
022E0E00 E59D3030 ldr     r3,[r13,30h]
022E0E04 E3A00000 mov     r0,0h
022E0E08 E3A01058 mov     r1,58h
022E0E0C E1A02822 mov     r2,r2,lsr 10h
022E0E10 EBF5AADC bl      204B988h
022E0E14 E1A02805 mov     r2,r5,lsl 10h
022E0E18 E59D3034 ldr     r3,[r13,34h]
022E0E1C E3A00000 mov     r0,0h
022E0E20 E3A01059 mov     r1,59h
022E0E24 E1A02822 mov     r2,r2,lsr 10h
022E0E28 EBF5AAD6 bl      204B988h
022E0E2C E1A02805 mov     r2,r5,lsl 10h
022E0E30 E59D3038 ldr     r3,[r13,38h]
022E0E34 E3A00000 mov     r0,0h
022E0E38 E3A0105A mov     r1,5Ah
022E0E3C E1A02822 mov     r2,r2,lsr 10h
022E0E40 EBF5AAD0 bl      204B988h
022E0E44 E1A00805 mov     r0,r5,lsl 10h
022E0E48 E1A02820 mov     r2,r0,lsr 10h
022E0E4C E1DD30D8 ldrsb   r3,[r13,8h]
022E0E50 E3A00000 mov     r0,0h
022E0E54 E3A0105B mov     r1,5Bh
022E0E58 EBF5AACA bl      204B988h
022E0E5C EA000582 b       22E246Ch
022E0E60 E1D407FC ldrsh   r0,[r4,7Ch]
022E0E64 E3500000 cmp     r0,0h
022E0E68 1A000001 bne     22E0E74h
022E0E6C E284007C add     r0,r4,7Ch
022E0E70 EBFFEE6E bl      22DC830h
022E0E74 E5941000 ldr     r1,[r4]
022E0E78 E5940004 ldr     r0,[r4,4h]
022E0E7C E591201C ldr     r2,[r1,1Ch]
022E0E80 E3A01000 mov     r1,0h
022E0E84 E12FFF32 blx     r2
022E0E88 EA000577 b       22E246Ch
022E0E8C E59F0398 ldr     r0,=2316A44h
022E0E90 E5901040 ldr     r1,[r0,40h]
022E0E94 E590003C ldr     r0,[r0,3Ch]
022E0E98 E58D1044 str     r1,[r13,44h]
022E0E9C E58D0040 str     r0,[r13,40h]
022E0EA0 E1D600B2 ldrh    r0,[r6,2h]
022E0EA4 EB000E8D bl      22E48E0h
022E0EA8 E58D0040 str     r0,[r13,40h]
022E0EAC E1D600B4 ldrh    r0,[r6,4h]
022E0EB0 EB000E8A bl      22E48E0h
022E0EB4 E58D0044 str     r0,[r13,44h]
022E0EB8 E5941000 ldr     r1,[r4]
022E0EBC E5940004 ldr     r0,[r4,4h]
022E0EC0 E5912024 ldr     r2,[r1,24h]
022E0EC4 E28D1040 add     r1,r13,40h
022E0EC8 E12FFF32 blx     r2
022E0ECC EA000566 b       22E246Ch
022E0ED0 E59F0354 ldr     r0,=2316A44h
022E0ED4 E5901028 ldr     r1,[r0,28h]
022E0ED8 E5900024 ldr     r0,[r0,24h]
022E0EDC E58D104C str     r1,[r13,4Ch]
022E0EE0 E58D0048 str     r0,[r13,48h]
022E0EE4 E1D600B2 ldrh    r0,[r6,2h]
022E0EE8 EB000E7C bl      22E48E0h
022E0EEC E58D0048 str     r0,[r13,48h]
022E0EF0 E1D600B4 ldrh    r0,[r6,4h]
022E0EF4 EB000E79 bl      22E48E0h
022E0EF8 E58D004C str     r0,[r13,4Ch]
022E0EFC E5941000 ldr     r1,[r4]
022E0F00 E5940004 ldr     r0,[r4,4h]
022E0F04 E5912044 ldr     r2,[r1,44h]
022E0F08 E28D1048 add     r1,r13,48h
022E0F0C E12FFF32 blx     r2
022E0F10 EA000555 b       22E246Ch
022E0F14 E59F1310 ldr     r1,=2316A44h
022E0F18 E1D600B2 ldrh    r0,[r6,2h]
022E0F1C E591202C ldr     r2,[r1,2Ch]
022E0F20 E5911030 ldr     r1,[r1,30h]
022E0F24 E58D2050 str     r2,[r13,50h]
022E0F28 E58D1054 str     r1,[r13,54h]
022E0F2C EB000E5E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0F30 E58D0050 str     r0,[r13,50h]
022E0F34 E1D600B4 ldrh    r0,[r6,4h]
022E0F38 EB000E5B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0F3C E59F12E8 ldr     r1,=2316A44h
022E0F40 E58D0054 str     r0,[r13,54h]
022E0F44 E5912048 ldr     r2,[r1,48h]
022E0F48 E5911044 ldr     r1,[r1,44h]
022E0F4C E59D0050 ldr     r0,[r13,50h]
022E0F50 E58D1058 str     r1,[r13,58h]
022E0F54 E58D205C str     r2,[r13,5Ch]
022E0F58 EBF484C5 bl      2002274h
022E0F5C E1A01400 mov     r1,r0,lsl 8h
022E0F60 E59D0054 ldr     r0,[r13,54h]
022E0F64 E58D1058 str     r1,[r13,58h]
022E0F68 EBF484C1 bl      2002274h
022E0F6C E1A00400 mov     r0,r0,lsl 8h
022E0F70 E58D005C str     r0,[r13,5Ch]
022E0F74 E5941000 ldr     r1,[r4]
022E0F78 E5940004 ldr     r0,[r4,4h]
022E0F7C E5912044 ldr     r2,[r1,44h]
022E0F80 E28D1058 add     r1,r13,58h
022E0F84 E12FFF32 blx     r2
022E0F88 EA000537 b       22E246Ch
022E0F8C E1D600B2 ldrh    r0,[r6,2h]
022E0F90 EB000E45 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E0F94 E1A00800 mov     r0,r0,lsl 10h
022E0F98 E1A00840 mov     r0,r0,asr 10h
022E0F9C EB005B9B bl      22F7E10h
022E0FA0 E3500000 cmp     r0,0h
022E0FA4 BA000530 blt     22E246Ch
022E0FA8 E28D1060 add     r1,r13,60h
022E0FAC EB006050 bl      22F90F4h
022E0FB0 E5941000 ldr     r1,[r4]
022E0FB4 E5940004 ldr     r0,[r4,4h]
022E0FB8 E5912024 ldr     r2,[r1,24h]
022E0FBC E28D1060 add     r1,r13,60h
022E0FC0 E12FFF32 blx     r2
022E0FC4 EA000528 b       22E246Ch
022E0FC8 E5941000 ldr     r1,[r4]
022E0FCC E5940004 ldr     r0,[r4,4h]
022E0FD0 E591200C ldr     r2,[r1,0Ch]
022E0FD4 E28D1068 add     r1,r13,68h
022E0FD8 E12FFF32 blx     r2
022E0FDC E28D1068 add     r1,r13,68h
022E0FE0 E2860002 add     r0,r6,2h
022E0FE4 EB000E48 bl      22E490Ch
022E0FE8 E5941000 ldr     r1,[r4]
022E0FEC E5940004 ldr     r0,[r4,4h]
022E0FF0 E5912024 ldr     r2,[r1,24h]
022E0FF4 E28D1068 add     r1,r13,68h
022E0FF8 E12FFF32 blx     r2
022E0FFC EA00051A b       22E246Ch
022E1000 E1D600B2 ldrh    r0,[r6,2h]
022E1004 EB000E35 bl      22E48E0h
022E1008 E5942000 ldr     r2,[r4]
022E100C E1A01000 mov     r1,r0
022E1010 E5940004 ldr     r0,[r4,4h]
022E1014 E5922028 ldr     r2,[r2,28h]
022E1018 E12FFF32 blx     r2
022E101C EA000512 b       22E246Ch
///CurrentOpCode == 0xC5 - MovePositionOffset
022E1020 E5941038 ldr     r1,[r4,38h]
022E1024 E2810002 add     r0,r1,2h
022E1028 E5840038 str     r0,[r4,38h]
022E102C E1D100B0 ldrh    r0,[r1]               //Load first parameter?
022E1030 EB000E2A bl      22E48E0h              //Convert to proper format
022E1034 E584004C str     r0,[r4,4Ch]
022E1038 E1D423FC ldrsh   r2,[r4,3Ch]                 //NbParams current command
022E103C E3E01000 mvn     r1,0h
022E1040 E3A00000 mov     r0,0h
022E1044 E2422001 sub     r2,r2,1h
022E1048 E1C423BC strh    r2,[r4,3Ch]                 //NbParams current command
022E104C E1C414B6 strh    r1,[r4,46h]
022E1050 E1C404B4 strh    r0,[r4,44h]
022E1054 E3A00003 mov     r0,3h
022E1058 EA000506 b       22E2478h        //Return 022E2478
///CurrentOpCode == 0xB3 (Move2PositionOffset -1 )
022E105C E5941038 ldr     r1,[r4,38h]       //R1 = AddressNextWord
022E1060 E2810002 add     r0,r1,2h          //R0 = AddressNextWord + 2
022E1064 E5840038 str     r0,[r4,38h]       //Set AddressNextWord = (AddressNextWord + 2)
022E1068 E1D100B0 ldrh    r0,[r1]           //R0 = NextWord
022E106C EB000E1B bl      22E48E0h          ///022E48E0
{
    022E48E0 E3100901 tst     r0,4000h      //
    if( (R0 & 0x4000) != 0 )
        022E48E4 13A01902 movne   r1,8000h  //R1 = 0x8000
        022E48E8 12611000 rsbne   r1,r1,0h  //R1 = 0 - 0x8000 = 0xFFFF8000
        022E48EC 11801001 orrne   r1,r0,r1  //R1 = R0 | R1
    else
        022E48F0 059F1010 ldreq   r1,=3FFFh //R1 = 0x3FFF
        022E48F4 00001001 andeq   r1,r0,r1  //R1 = R0 & 0x3FFF
    022E48F8 E3100902 tst     r0,8000h
    if( (R0 & 0x8000) != 0 )
        022E48FC 11A00001 movne   r0,r1     //R0 = R1
    else
        022E4900 01A00401 moveq   r0,r1,lsl 8h //R0 = (R1 << 8)
    022E4904 E12FFF1E bx      r14
}
022E1070 E584004C str     r0,[r4,4Ch]       //
022E1074 E1D423FC ldrsh   r2,[r4,3Ch]                 //NbParams current command
022E1078 E3A01000 mov     r1,0h
022E107C E3A00003 mov     r0,3h
022E1080 E2422001 sub     r2,r2,1h
022E1084 E1C423BC strh    r2,[r4,3Ch]                 //NbParams current command
022E1088 E1C414B6 strh    r1,[r4,46h]
022E108C E1C414B4 strh    r1,[r4,44h]
022E1090 EA0004F8 b       22E2478h        //Return 022E2478
022E1094 E5941038 ldr     r1,[r4,38h]
022E1098 E2810002 add     r0,r1,2h
022E109C E5840038 str     r0,[r4,38h]
022E10A0 E1D100B0 ldrh    r0,[r1]
022E10A4 EB000E00 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E10A8 E1C404B6 strh    r0,[r4,46h]
022E10AC E1D423FC ldrsh   r2,[r4,3Ch]                 //NbParams current command
022E10B0 E3A01C01 mov     r1,100h
022E10B4 E3A00000 mov     r0,0h
022E10B8 E2422001 sub     r2,r2,1h
022E10BC E1C423BC strh    r2,[r4,3Ch]                 //NbParams current command
022E10C0 E584104C str     r1,[r4,4Ch]
022E10C4 E1C404B4 strh    r0,[r4,44h]
022E10C8 E3A00003 mov     r0,3h
022E10CC EA0004E9 b       22E2478h        //Return 022E2478
022E10D0 E1D600B2 ldrh    r0,[r6,2h]
022E10D4 EB000DF4 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E10D8 E5C40042 strb    r0,[r4,42h]
022E10DC E5942000 ldr     r2,[r4]
022E10E0 E1D414D2 ldrsb   r1,[r4,42h]
022E10E4 E5940004 ldr     r0,[r4,4h]
022E10E8 E592202C ldr     r2,[r2,2Ch]
022E10EC E12FFF32 blx     r2
022E10F0 EA0004DD b       22E246Ch
022E10F4 E1D600B4 ldrh    r0,[r6,4h]
022E10F8 EB000DEB bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E10FC E1A00800 mov     r0,r0,lsl 10h
022E1100 E1A00840 mov     r0,r0,asr 10h
022E1104 EB005B41 bl      22F7E10h
022E1108 E1B07000 movs    r7,r0
022E110C 4A0004D6 bmi     22E246Ch
022E1110 E1D600B2 ldrh    r0,[r6,2h]
022E1114 EB000DE4 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1118 E1A05000 mov     r5,r0
022E111C E28D1009 add     r1,r13,9h
022E1120 E1A00007 mov     r0,r7
022E1124 EB006007 bl      22F9148h
022E1128 E1DD00D9 ldrsb   r0,[r13,9h]
022E112C E20510FF and     r1,r5,0FFh
022E1130 EBF4901A bl      20051A0h
022E1134 E5C40042 strb    r0,[r4,42h]
022E1138 E5942000 ldr     r2,[r4]
022E113C E1D414D2 ldrsb   r1,[r4,42h]
022E1140 E5940004 ldr     r0,[r4,4h]
022E1144 E592202C ldr     r2,[r2,2Ch]
022E1148 E12FFF32 blx     r2
022E114C EA0004C6 b       22E246Ch
//0x15B -- WaitBgm
022E1150 E1D600B2 ldrh    r0,[r6,2h]
022E1154 EB000DD4 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1158 E5941000 ldr     r1,[r4]
022E115C E1A05000 mov     r5,r0
022E1160 E5912014 ldr     r2,[r1,14h]
022E1164 E5940004 ldr     r0,[r4,4h]
022E1168 E28D100A add     r1,r13,0Ah
022E116C E12FFF32 blx     r2
022E1170 E1DD00DA ldrsb   r0,[r13,0Ah]
022E1174 E20510FF and     r1,r5,0FFh
022E1178 EBF49008 bl      20051A0h
022E117C E5C40042 strb    r0,[r4,42h]
022E1180 E5942000 ldr     r2,[r4]
022E1184 E1D414D2 ldrsb   r1,[r4,42h]
022E1188 E5940004 ldr     r0,[r4,4h]
022E118C E592202C ldr     r2,[r2,2Ch]
022E1190 E12FFF32 blx     r2
022E1194 EA0004B4 b       22E246Ch
022E1198 E281002A add     r0,r1,2Ah
022E119C E1570000 cmp     r7,r0
022E11A0 E3A05000 mov     r5,0h
022E11A4 13570F55 cmpne   r7,154h
022E11A8 0A000003 beq     22E11BCh
022E11AC E281002C add     r0,r1,2Ch
022E11B0 E1570000 cmp     r7,r0
022E11B4 0A00001E beq     22E1234h
022E11B8 EA00002B b       22E126Ch
022E11BC E1D600B4 ldrh    r0,[r6,4h]
022E11C0 EB000DB9 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E11C4 E1A00800 mov     r0,r0,lsl 10h
022E11C8 E1A00840 mov     r0,r0,asr 10h
022E11CC EB005B0F bl      22F7E10h
022E11D0 E1B08000 movs    r8,r0
022E11D4 4A000024 bmi     22E126Ch
022E11D8 E28D1078 add     r1,r13,78h
022E11DC E3A05001 mov     r5,1h
022E11E0 EB005FC3 bl      22F90F4h
022E11E4 E59F0044 ldr     r0,=153h
022E11E8 E1570000 cmp     r7,r0
022E11EC 1A000003 bne     22E1200h
022E11F0 E28D1088 add     r1,r13,88h
022E11F4 E1A00008 mov     r0,r8
022E11F8 EB005FAD bl      22F90B4h
022E11FC EA00001A b       22E126Ch
022E1200 E59F0024 ldr     r0,=2316A44h
022E1204 E5901020 ldr     r1,[r0,20h]
022E1208 E590001C ldr     r0,[r0,1Ch]
022E120C E58D108C str     r1,[r13,8Ch]
022E1210 E58D0088 str     r0,[r13,88h]
022E1214 EA000014 b       22E126Ch
022E1218 02316A44 eoreqs  r6,r1,44000h
022E121C 0231956C eoreqs  r9,r1,1B000000h
022E1220 00000129 andeq   r0,r0,r9,lsr 2h
022E1224 00000101 andeq   r0,r0,r1,lsl 2h
022E1228 0000015F andeq   r0,r0,r15,asr r1
022E122C 02316A44 eoreqs  r6,r1,44000h
022E1230 00000153 andeq   r0,r0,r3,asr r1
022E1234 E5941000 ldr     r1,[r4]
022E1238 E5940004 ldr     r0,[r4,4h]
022E123C E591200C ldr     r2,[r1,0Ch]
022E1240 E28D1078 add     r1,r13,78h
022E1244 E3A05001 mov     r5,1h
022E1248 E12FFF32 blx     r2
022E124C E5941000 ldr     r1,[r4]
022E1250 E5940004 ldr     r0,[r4,4h]
022E1254 E5912008 ldr     r2,[r1,8h]
022E1258 E28D1088 add     r1,r13,88h
022E125C E12FFF32 blx     r2
022E1260 E28D1078 add     r1,r13,78h
022E1264 E2860004 add     r0,r6,4h
022E1268 EB000DA7 bl      22E490Ch
022E126C E3550000 cmp     r5,0h
022E1270 0A00047D beq     22E246Ch
022E1274 E1D600B2 ldrh    r0,[r6,2h]
022E1278 EB000D8B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E127C E5941000 ldr     r1,[r4]
022E1280 E1A05000 mov     r5,r0
022E1284 E591200C ldr     r2,[r1,0Ch]
022E1288 E5940004 ldr     r0,[r4,4h]
022E128C E28D1070 add     r1,r13,70h
022E1290 E12FFF32 blx     r2
022E1294 E5941000 ldr     r1,[r4]
022E1298 E5940004 ldr     r0,[r4,4h]
022E129C E5912008 ldr     r2,[r1,8h]
022E12A0 E28D1080 add     r1,r13,80h
022E12A4 E12FFF32 blx     r2
022E12A8 E28D0070 add     r0,r13,70h
022E12AC E28D1080 add     r1,r13,80h
022E12B0 E28D2078 add     r2,r13,78h
022E12B4 E28D3088 add     r3,r13,88h
022E12B8 EBF4917A bl      20058A8h
022E12BC E5CD000B strb    r0,[r13,0Bh]
022E12C0 E1DD10DB ldrsb   r1,[r13,0Bh]
022E12C4 E3E00000 mvn     r0,0h
022E12C8 E1510000 cmp     r1,r0
022E12CC 1A00000E bne     22E130Ch
022E12D0 E59F1F9C ldr     r1,=2316A60h
022E12D4 E28D0070 add     r0,r13,70h
022E12D8 E28D2078 add     r2,r13,78h
022E12DC E1A03001 mov     r3,r1
022E12E0 EBF49130 bl      20057A8h
022E12E4 E5CD000B strb    r0,[r13,0Bh]
022E12E8 E1DD10DB ldrsb   r1,[r13,0Bh]
022E12EC E3E00000 mvn     r0,0h
022E12F0 E1510000 cmp     r1,r0
022E12F4 1A000004 bne     22E130Ch
022E12F8 E5941000 ldr     r1,[r4]
022E12FC E5940004 ldr     r0,[r4,4h]
022E1300 E5912014 ldr     r2,[r1,14h]
022E1304 E28D100B add     r1,r13,0Bh
022E1308 E12FFF32 blx     r2
022E130C E1DD00DB ldrsb   r0,[r13,0Bh]
022E1310 E20510FF and     r1,r5,0FFh
022E1314 EBF48FA1 bl      20051A0h
022E1318 E5C40042 strb    r0,[r4,42h]
022E131C E5942000 ldr     r2,[r4]
022E1320 E1D414D2 ldrsb   r1,[r4,42h]
022E1324 E5940004 ldr     r0,[r4,4h]
022E1328 E592202C ldr     r2,[r2,2Ch]
022E132C E12FFF32 blx     r2
022E1330 EA00044D b       22E246Ch
022E1334 E1D600B6 ldrh    r0,[r6,6h]
022E1338 EB000D5B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E133C E5C40069 strb    r0,[r4,69h]
022E1340 E3A00000 mov     r0,0h
022E1344 E1C404B6 strh    r0,[r4,46h]
022E1348 E3A00003 mov     r0,3h
022E134C EA000449 b       22E2478h        //Return 022E2478
022E1350 E1D600B6 ldrh    r0,[r6,6h]
022E1354 EB000D54 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1358 E5941000 ldr     r1,[r4]
022E135C E1A05000 mov     r5,r0
022E1360 E5912014 ldr     r2,[r1,14h]
022E1364 E5940004 ldr     r0,[r4,4h]
022E1368 E28D100C add     r1,r13,0Ch
022E136C E12FFF32 blx     r2
022E1370 E1DD00DC ldrsb   r0,[r13,0Ch]
022E1374 E20510FF and     r1,r5,0FFh
022E1378 EBF48F88 bl      20051A0h
022E137C E5C40069 strb    r0,[r4,69h]
022E1380 E3A00000 mov     r0,0h
022E1384 E1C404B6 strh    r0,[r4,46h]
022E1388 E3A00003 mov     r0,3h
022E138C EA000439 b       22E2478h        //Return 022E2478
///0x14D - Turn2DirectionLives
022E1390 E3A00000 mov     r0,0h
022E1394 E1C404B6 strh    r0,[r4,46h]
022E1398 E3A00003 mov     r0,3h
022E139C EA000435 b       22E2478h        //Return 022E2478

022E13A0 E3A00000 mov     r0,0h
022E13A4 E1C404B6 strh    r0,[r4,46h]
022E13A8 E3A00003 mov     r0,3h
022E13AC EA000431 b       22E2478h        //Return 022E2478
022E13B0 E1D600B4 ldrh    r0,[r6,4h]
022E13B4 EB000D3C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E13B8 E1A08000 mov     r8,r0
022E13BC E1D600B6 ldrh    r0,[r6,6h]
022E13C0 EB000D39 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E13C4 E1A07000 mov     r7,r0
022E13C8 E1D600B8 ldrh    r0,[r6,8h]
022E13CC EB000D36 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E13D0 E5941000 ldr     r1,[r4]
022E13D4 E1A05000 mov     r5,r0
022E13D8 E5912014 ldr     r2,[r1,14h]
022E13DC E5940004 ldr     r0,[r4,4h]
022E13E0 E28D100D add     r1,r13,0Dh
022E13E4 E12FFF32 blx     r2
022E13E8 E1DD00DD ldrsb   r0,[r13,0Dh]
022E13EC E20510FF and     r1,r5,0FFh
022E13F0 EBF48F6A bl      20051A0h
022E13F4 E3570003 cmp     r7,3h
022E13F8 13570004 cmpne   r7,4h
022E13FC 1357000B cmpne   r7,0Bh
022E1400 01A08108 moveq   r8,r8,lsl 2h
022E1404 E5C40069 strb    r0,[r4,69h]
022E1408 11A08188 movne   r8,r8,lsl 3h
022E140C E1C484B4 strh    r8,[r4,44h]
022E1410 E3A00000 mov     r0,0h
022E1414 E1C404B6 strh    r0,[r4,46h]
022E1418 E3A00003 mov     r0,3h
022E141C EA000415 b       22E2478h        //Return 022E2478
022E1420 E1D600B6 ldrh    r0,[r6,6h]
022E1424 EB000D20 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1428 E5C40042 strb    r0,[r4,42h]
022E142C E5942000 ldr     r2,[r4]
022E1430 E1D414D2 ldrsb   r1,[r4,42h]
022E1434 E5940004 ldr     r0,[r4,4h]
022E1438 E592202C ldr     r2,[r2,2Ch]
022E143C E12FFF32 blx     r2
022E1440 E1D600B4 ldrh    r0,[r6,4h]
022E1444 EB000D25 bl      22E48E0h
022E1448 E584004C str     r0,[r4,4Ch]
022E144C E1D600B2 ldrh    r0,[r6,2h]
022E1450 EB000D15 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1454 E1C404B6 strh    r0,[r4,46h]
022E1458 E3A00003 mov     r0,3h
022E145C EA000405 b       22E2478h        //Return 022E2478
022E1460 E1D600B6 ldrh    r0,[r6,6h]
022E1464 EB000D10 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1468 E5941000 ldr     r1,[r4]
022E146C E1A05000 mov     r5,r0
022E1470 E5912014 ldr     r2,[r1,14h]
022E1474 E5940004 ldr     r0,[r4,4h]
022E1478 E28D100E add     r1,r13,0Eh
022E147C E12FFF32 blx     r2
022E1480 E1DD00DE ldrsb   r0,[r13,0Eh]
022E1484 E20510FF and     r1,r5,0FFh
022E1488 EBF48F44 bl      20051A0h
022E148C E5C40042 strb    r0,[r4,42h]
022E1490 E5942000 ldr     r2,[r4]
022E1494 E1D414D2 ldrsb   r1,[r4,42h]
022E1498 E5940004 ldr     r0,[r4,4h]
022E149C E592202C ldr     r2,[r2,2Ch]
022E14A0 E12FFF32 blx     r2
022E14A4 E1D600B4 ldrh    r0,[r6,4h]
022E14A8 EB000D0C bl      22E48E0h
022E14AC E584004C str     r0,[r4,4Ch]
022E14B0 E1D600B2 ldrh    r0,[r6,2h]
022E14B4 EB000CFC bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E14B8 E1C404B6 strh    r0,[r4,46h]
022E14BC E3A00003 mov     r0,3h
022E14C0 EA0003EC b       22E2478h        //Return 022E2478
022E14C4 E1D600B2 ldrh    r0,[r6,2h]
022E14C8 EB000CF7 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E14CC E1A05000 mov     r5,r0
022E14D0 E1D600B8 ldrh    r0,[r6,8h]
022E14D4 EB000CF4 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E14D8 E1A01800 mov     r1,r0,lsl 10h
022E14DC E1A00004 mov     r0,r4
022E14E0 E1C454B6 strh    r5,[r4,46h]
022E14E4 E1A01841 mov     r1,r1,asr 10h
022E14E8 E1C414B4 strh    r1,[r4,44h]
022E14EC EBFFEE7E bl      22DCEECh
022E14F0 E3A00003 mov     r0,3h
022E14F4 EA0003DF b       22E2478h        //Return 022E2478
022E14F8 E1D600B4 ldrh    r0,[r6,4h]
022E14FC EB000CF7 bl      22E48E0h
022E1500 E5840064 str     r0,[r4,64h]
022E1504 E1D600B2 ldrh    r0,[r6,2h]
022E1508 EB000CF4 bl      22E48E0h
022E150C E584004C str     r0,[r4,4Ch]
022E1510 E3E00000 mvn     r0,0h
022E1514 E1C404B6 strh    r0,[r4,46h]
022E1518 E3A00003 mov     r0,3h
022E151C EA0003D5 b       22E2478h        //Return 022E2478
022E1520 E1D600B2 ldrh    r0,[r6,2h]
022E1524 EB000CE0 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1528 E1A07000 mov     r7,r0
022E152C E1D600B4 ldrh    r0,[r6,4h]
022E1530 EB000CDD bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1534 E1A05000 mov     r5,r0
022E1538 E1D600B6 ldrh    r0,[r6,6h]
022E153C EB000CDA bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1540 E20710FF and     r1,r7,0FFh
022E1544 E1A02005 mov     r2,r5
022E1548 E1A03000 mov     r3,r0
022E154C E1A00004 mov     r0,r4
022E1550 EB001963 bl      22E7AE4h
022E1554 E3A00003 mov     r0,3h
022E1558 EA0003C6 b       22E2478h        //Return 022E2478
022E155C E1D600B2 ldrh    r0,[r6,2h]
022E1560 EB000CD1 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1564 E1A05000 mov     r5,r0
022E1568 E1D600B4 ldrh    r0,[r6,4h]
022E156C EB000CCE bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1570 E1A04000 mov     r4,r0
022E1574 E1D600B6 ldrh    r0,[r6,6h]
022E1578 EB000CCB bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E157C E1A03000 mov     r3,r0
022E1580 E3570049 cmp     r7,49h
022E1584 03A00000 moveq   r0,0h
022E1588 13A00001 movne   r0,1h
022E158C E1A01005 mov     r1,r5
022E1590 E1A02004 mov     r2,r4
022E1594 EB0026BA bl      22EB084h
022E1598 EA0003B3 b       22E246Ch
022E159C E1D430F8 ldrsh   r3,[r4,8h]
022E15A0 E5941000 ldr     r1,[r4]
022E15A4 E59F2CCC ldr     r2,=2316AD8h
022E15A8 E357004B cmp     r7,4Bh
022E15AC 03A05000 moveq   r5,0h
022E15B0 E5940004 ldr     r0,[r4,4h]
022E15B4 E7924103 ldr     r4,[r2,r3,lsl 2h]
022E15B8 E5911004 ldr     r1,[r1,4h]
022E15BC 13A05001 movne   r5,1h
022E15C0 E12FFF31 blx     r1
022E15C4 E1A02000 mov     r2,r0
022E15C8 E1A00005 mov     r0,r5
022E15CC E1A01004 mov     r1,r4
022E15D0 EB0026D4 bl      22EB128h
022E15D4 EA0003A4 b       22E246Ch
022E15D8 E3570048 cmp     r7,48h
022E15DC 03A00000 moveq   r0,0h
022E15E0 13A00001 movne   r0,1h
022E15E4 E3A01002 mov     r1,2h
022E15E8 E3A02000 mov     r2,0h
022E15EC EB0026CD bl      22EB128h
022E15F0 EA00039D b       22E246Ch
022E15F4 E5941000 ldr     r1,[r4]
022E15F8 E5940004 ldr     r0,[r4,4h]
022E15FC E591200C ldr     r2,[r1,0Ch]
022E1600 E28D1090 add     r1,r13,90h
022E1604 E12FFF32 blx     r2
022E1608 E357004A cmp     r7,4Ah
022E160C 03A00000 moveq   r0,0h
022E1610 13A00001 movne   r0,1h
022E1614 E28D2090 add     r2,r13,90h
022E1618 E3A01001 mov     r1,1h
022E161C EB0026B0 bl      22EB0E4h
022E1620 EA000391 b       22E246Ch
022E1624 E5940000 ldr     r0,[r4]
022E1628 E590200C ldr     r2,[r0,0Ch]
022E162C E3520000 cmp     r2,0h
022E1630 03A00000 moveq   r0,0h
022E1634 058D0098 streq   r0,[r13,98h]
022E1638 058D009C streq   r0,[r13,9Ch]
022E163C 0A000002 beq     22E164Ch
022E1640 E5940004 ldr     r0,[r4,4h]
022E1644 E28D1098 add     r1,r13,98h
022E1648 E12FFF32 blx     r2
022E164C E28D1098 add     r1,r13,98h
022E1650 E2860002 add     r0,r6,2h
022E1654 EB000CAC bl      22E490Ch
022E1658 E357004C cmp     r7,4Ch
022E165C 03A00000 moveq   r0,0h
022E1660 13A00001 movne   r0,1h
022E1664 E28D2098 add     r2,r13,98h
022E1668 E3A01001 mov     r1,1h
022E166C EB00269C bl      22EB0E4h
022E1670 EA00037D b       22E246Ch
022E1674 E5941000 ldr     r1,[r4]
022E1678 E1D430F8 ldrsh   r3,[r4,8h]
022E167C E59F2BF4 ldr     r2,=2316AD8h
022E1680 E3570045 cmp     r7,45h
022E1684 03A05000 moveq   r5,0h
022E1688 E5940004 ldr     r0,[r4,4h]
022E168C E5911004 ldr     r1,[r1,4h]
022E1690 13A05001 movne   r5,1h
022E1694 E7927103 ldr     r7,[r2,r3,lsl 2h]
022E1698 E12FFF31 blx     r1
022E169C E1A04000 mov     r4,r0
022E16A0 E1D600B2 ldrh    r0,[r6,2h]
022E16A4 EB000C8D bl      22E48E0h
022E16A8 E1A03000 mov     r3,r0
022E16AC E1A00005 mov     r0,r5
022E16B0 E1A01007 mov     r1,r7
022E16B4 E1A02004 mov     r2,r4
022E16B8 EB0026B7 bl      22EB19Ch
022E16BC E3A00003 mov     r0,3h
022E16C0 EA00036C b       22E2478h        //Return 022E2478
022E16C4 E1D600B2 ldrh    r0,[r6,2h]
022E16C8 EB000C84 bl      22E48E0h
022E16CC E1A03000 mov     r3,r0
022E16D0 E3570043 cmp     r7,43h
022E16D4 03A00000 moveq   r0,0h
022E16D8 13A00001 movne   r0,1h
022E16DC E3A01002 mov     r1,2h
022E16E0 E3A02000 mov     r2,0h
022E16E4 EB0026AC bl      22EB19Ch
022E16E8 E3A00003 mov     r0,3h
022E16EC EA000361 b       22E2478h        //Return 022E2478
022E16F0 E1D600B2 ldrh    r0,[r6,2h]
022E16F4 EB000C79 bl      22E48E0h
022E16F8 E5941000 ldr     r1,[r4]
022E16FC E1A05000 mov     r5,r0
022E1700 E591200C ldr     r2,[r1,0Ch]
022E1704 E5940004 ldr     r0,[r4,4h]
022E1708 E28D10A0 add     r1,r13,0A0h
022E170C E12FFF32 blx     r2
022E1710 E3570044 cmp     r7,44h
022E1714 03A00000 moveq   r0,0h
022E1718 13A00001 movne   r0,1h
022E171C E28D20A0 add     r2,r13,0A0h
022E1720 E1A03005 mov     r3,r5
022E1724 E3A01001 mov     r1,1h
022E1728 EB002688 bl      22EB150h
022E172C E3A00003 mov     r0,3h
022E1730 EA000350 b       22E2478h        //Return 022E2478
022E1734 E5941038 ldr     r1,[r4,38h]
022E1738 E2810002 add     r0,r1,2h
022E173C E5840038 str     r0,[r4,38h]
022E1740 E1D100B0 ldrh    r0,[r1]
022E1744 EB000C65 bl      22E48E0h
022E1748 E584004C str     r0,[r4,4Ch]
022E174C E1D413FC ldrsh   r1,[r4,3Ch]                 //NbParams current command
022E1750 E3A00003 mov     r0,3h
022E1754 E2411001 sub     r1,r1,1h
022E1758 E1C413BC strh    r1,[r4,3Ch]                 //NbParams current command
022E175C EA000345 b       22E2478h        //Return 022E2478
022E1760 E5941000 ldr     r1,[r4]
022E1764 E1D430F8 ldrsh   r3,[r4,8h]
022E1768 E59F2B08 ldr     r2,=2316AD8h
022E176C E357003B cmp     r7,3Bh
022E1770 03A05000 moveq   r5,0h
022E1774 E5940004 ldr     r0,[r4,4h]
022E1778 E5911004 ldr     r1,[r1,4h]
022E177C 13A05001 movne   r5,1h
022E1780 E7927103 ldr     r7,[r2,r3,lsl 2h]
022E1784 E12FFF31 blx     r1
022E1788 E1A04000 mov     r4,r0
022E178C E1D600B2 ldrh    r0,[r6,2h]
022E1790 EB000C52 bl      22E48E0h
022E1794 E1A03000 mov     r3,r0
022E1798 E1A00005 mov     r0,r5
022E179C E1A01007 mov     r1,r7
022E17A0 E1A02004 mov     r2,r4
022E17A4 EB002697 bl      22EB208h
022E17A8 E3A00003 mov     r0,3h
022E17AC EA000331 b       22E2478h        //Return 022E2478
022E17B0 E1D600B2 ldrh    r0,[r6,2h]
022E17B4 EB000C49 bl      22E48E0h
022E17B8 E1A03000 mov     r3,r0
022E17BC E3570039 cmp     r7,39h
022E17C0 03A00000 moveq   r0,0h
022E17C4 13A00001 movne   r0,1h
022E17C8 E3A01002 mov     r1,2h
022E17CC E3A02000 mov     r2,0h
022E17D0 EB00268C bl      22EB208h
022E17D4 E3A00003 mov     r0,3h
022E17D8 EA000326 b       22E2478h        //Return 022E2478
022E17DC E1D600B2 ldrh    r0,[r6,2h]
022E17E0 EB000C3E bl      22E48E0h
022E17E4 E5941000 ldr     r1,[r4]
022E17E8 E1A05000 mov     r5,r0
022E17EC E591200C ldr     r2,[r1,0Ch]
022E17F0 E5940004 ldr     r0,[r4,4h]
022E17F4 E28D10A8 add     r1,r13,0A8h
022E17F8 E12FFF32 blx     r2
022E17FC E357003A cmp     r7,3Ah
022E1800 03A00000 moveq   r0,0h
022E1804 13A00001 movne   r0,1h
022E1808 E28D20A8 add     r2,r13,0A8h
022E180C E1A03005 mov     r3,r5
022E1810 E3A01001 mov     r1,1h
022E1814 EB00266C bl      22EB1CCh
022E1818 E3A00003 mov     r0,3h
022E181C EA000315 b       22E2478h        //Return 022E2478
022E1820 E5941038 ldr     r1,[r4,38h]
022E1824 E2810002 add     r0,r1,2h
022E1828 E5840038 str     r0,[r4,38h]
022E182C E1D100B0 ldrh    r0,[r1]
022E1830 EB000C2A bl      22E48E0h
022E1834 E584004C str     r0,[r4,4Ch]
022E1838 E1D423FC ldrsh   r2,[r4,3Ch]                 //NbParams current command
022E183C E3A01000 mov     r1,0h
022E1840 E3A00003 mov     r0,3h
022E1844 E2422001 sub     r2,r2,1h
022E1848 E1C423BC strh    r2,[r4,3Ch]                 //NbParams current command
022E184C E1C414B6 strh    r1,[r4,46h]
022E1850 E1C414B4 strh    r1,[r4,44h]
022E1854 EA000307 b       22E2478h        //Return 022E2478
022E1858 E5941000 ldr     r1,[r4]
022E185C E1D430F8 ldrsh   r3,[r4,8h]
022E1860 E59F2A10 ldr     r2,=2316AD8h
022E1864 E3570040 cmp     r7,40h
022E1868 03A05000 moveq   r5,0h
022E186C E5940004 ldr     r0,[r4,4h]
022E1870 E5911004 ldr     r1,[r1,4h]
022E1874 13A05001 movne   r5,1h
022E1878 E7927103 ldr     r7,[r2,r3,lsl 2h]
022E187C E12FFF31 blx     r1
022E1880 E1A04000 mov     r4,r0
022E1884 E1D600B2 ldrh    r0,[r6,2h]
022E1888 EB000C07 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E188C E1A03000 mov     r3,r0
022E1890 E1A00005 mov     r0,r5
022E1894 E1A01007 mov     r1,r7
022E1898 E1A02004 mov     r2,r4
022E189C EB002674 bl      22EB274h
022E18A0 E3A00003 mov     r0,3h
022E18A4 EA0002F3 b       22E2478h        //Return 022E2478
022E18A8 E1D600B2 ldrh    r0,[r6,2h]
022E18AC EB000BFE bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E18B0 E1A03000 mov     r3,r0
022E18B4 E357003E cmp     r7,3Eh
022E18B8 03A00000 moveq   r0,0h
022E18BC 13A00001 movne   r0,1h
022E18C0 E3A01002 mov     r1,2h
022E18C4 E3A02000 mov     r2,0h
022E18C8 EB002669 bl      22EB274h
022E18CC E3A00003 mov     r0,3h
022E18D0 EA0002E8 b       22E2478h        //Return 022E2478
022E18D4 E5941000 ldr     r1,[r4]
022E18D8 E5940004 ldr     r0,[r4,4h]
022E18DC E5911004 ldr     r1,[r1,4h]
022E18E0 E12FFF31 blx     r1
022E18E4 E1D600B2 ldrh    r0,[r6,2h]
022E18E8 EB000BEF bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E18EC E5941000 ldr     r1,[r4]
022E18F0 E1A05000 mov     r5,r0
022E18F4 E591200C ldr     r2,[r1,0Ch]
022E18F8 E5940004 ldr     r0,[r4,4h]
022E18FC E28D10B0 add     r1,r13,0B0h
022E1900 E12FFF32 blx     r2
022E1904 E357003F cmp     r7,3Fh
022E1908 03A00000 moveq   r0,0h
022E190C 13A00001 movne   r0,1h
022E1910 E28D20B0 add     r2,r13,0B0h
022E1914 E1A03005 mov     r3,r5
022E1918 E3A01001 mov     r1,1h
022E191C EB002645 bl      22EB238h
022E1920 E3A00003 mov     r0,3h
022E1924 EA0002D3 b       22E2478h        //Return 022E2478
022E1928 E5941000 ldr     r1,[r4]
022E192C E5940004 ldr     r0,[r4,4h]
022E1930 E5911004 ldr     r1,[r1,4h]
022E1934 E12FFF31 blx     r1
022E1938 E1D600B2 ldrh    r0,[r6,2h]
022E193C EB000BDA bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1940 E5941000 ldr     r1,[r4]
022E1944 E1A05000 mov     r5,r0
022E1948 E591200C ldr     r2,[r1,0Ch]
022E194C E3520000 cmp     r2,0h
022E1950 03A00000 moveq   r0,0h
022E1954 058D00B8 streq   r0,[r13,0B8h]
022E1958 058D00BC streq   r0,[r13,0BCh]
022E195C 0A000002 beq     22E196Ch
022E1960 E5940004 ldr     r0,[r4,4h]
022E1964 E28D10B8 add     r1,r13,0B8h
022E1968 E12FFF32 blx     r2
022E196C E28D10B8 add     r1,r13,0B8h
022E1970 E2860004 add     r0,r6,4h
022E1974 EB000BE4 bl      22E490Ch
022E1978 E3570042 cmp     r7,42h
022E197C 03A00000 moveq   r0,0h
022E1980 13A00001 movne   r0,1h
022E1984 E28D20B8 add     r2,r13,0B8h
022E1988 E1A03005 mov     r3,r5
022E198C E3A01001 mov     r1,1h
022E1990 EB002628 bl      22EB238h
022E1994 E3A00003 mov     r0,3h
022E1998 EA0002B6 b       22E2478h        //Return 022E2478
///================================================================== 0x128
// Coming from: Opcode < 0xBB and != B9
022E199C E5945034 ldr     r5,[r4,34h]           //Load current Opcode pointer
022E19A0 E1D560B0 ldrh    r6,[r5]               //Load current Opcode
022E19A4 E35600F6 cmp     r6,0F6h               
if( CurrentOpCode > 0xF6 )
    022E19A8 CA000045 bgt     22E1AC4h              ///022E1AC4
else if( CurrentOpCode >= 0xF6 )
    022E19AC AA00026B bge     22E2360h
022E19B0 E3560084 cmp     r6,84h
if( CurrentOpCode > 0x84 )
    022E19B4 CA000021 bgt     22E1A40h              ///022E1A40
else if(CurrentOpCode >= 0x84 )
    022E19B8 AA000170 bge     22E1F80h
022E19BC E356006A cmp     r6,6Ah
if( CurrentOpCode > 0x6A )
    022E19C0 CA000014 bgt     22E1A18h              ///022E1A18
else if( CurrentOpCode >= 0x6A )
    022E19C4 AA0002A6 bge     22E2464h
022E19C8 E3560061 cmp     r6,61h
if( CurrentOpCode > 0x61 )
    022E19CC CA00000E bgt     22E1A0Ch              ///022E1A0C
else if( CurrentOpCode >= 0x61 )
    022E19D0 AA000295 bge     22E242Ch
022E19D4 E246001E sub     r0,r6,1Eh					//Opcode - 0x1E
022E19D8 E3500009 cmp     r0,9h
if(R0 < 9) 
	022E19DC 908FF100 addls   r15,r15,r0,lsl 2h
else
	022E19E0 EA00029F b       22E2464h				///022E2464
022E19E4 EA0001D1 b       22E2130h
022E19E8 EA0001AD b       22E20A4h
022E19EC EA0001B7 b       22E20D0h
022E19F0 EA0001CC b       22E2128h
022E19F4 EA0001D5 b       22E2150h
022E19F8 EA000202 b       22E2208h
022E19FC EA0001DE b       22E217Ch
022E1A00 EA0001E8 b       22E21A8h
022E1A04 EA0001FD b       22E2200h
022E1A08 EA000206 b       22E2228h
///OpCode > 0x61
022E1A0C E3560062 cmp     r6,62h
if( CurrentOpCode == 0x62 )
    022E1A10 0A00028A beq     22E2440h
else
    022E1A14 EA000292 b       22E2464h              ///022E2464
///OpCode > 0x6A
022E1A18 E356006C cmp     r6,6Ch
if( CurrentOpCode > 0x6C )
    022E1A1C CA000002 bgt     22E1A2Ch
else if( CurrentOpCode >= 0x6C )
    022E1A20 AA00028F bge     22E2464h
022E1A24 E356006B cmp     r6,6Bh                    //
022E1A28 EA00028D b       22E2464h                  ///022E2464
022E1A2C E356006D cmp     r6,6Dh
022E1A30 DA00028B ble     22E2464h
022E1A34 E3560083 cmp     r6,83h
022E1A38 0A000178 beq     22E2020h
022E1A3C EA000288 b       22E2464h
//OpCode > 0x84
022E1A40 E3560090 cmp     r6,90h
if( CurrentOpCode > 0x90 )
    022E1A44 CA00000D bgt     22E1A80h				///022E1A80
else if( CurrentOpCode >= 0x90 )
    022E1A48 AA0000F6 bge     22E1E28h
022E1A4C E3560086 cmp     r6,86h
if( CurrentOpCode > 0x86 )
    022E1A50 CA000003 bgt     22E1A64h              //022E1A64
else if( CurrentOpCode >= 0x86 )
    022E1A54 AA00015A bge     22E1FC4h
022E1A58 E3560085 cmp     r6,85h
022E1A5C 0A000156 beq     22E1FBCh
022E1A60 EA00027F b       22E2464h
//OpCode > 0x86
022E1A64 E356008C cmp     r6,8Ch
if( CurrentOpCode > 0x8C )
    022E1A68 CA000001 bgt     22E1A74h              //022E1A74
else if( CurrentOpCode == 0x8C )
    022E1A6C 0A000085 beq     22E1C88h
022E1A70 EA00027B b       22E2464h
//OpCode > 0x8C
022E1A74 E356008D cmp     r6,8Dh
if(OpCode == 0x8D)
	022E1A78 0A000058 beq     22E1BE0h              //022E1BE0
022E1A7C EA000278 b       22E2464h
///022E1A80 Opcode > 0x90
022E1A80 E3560092 cmp     r6,92h
if(CurrentOpCode > 0x92)
	022E1A84 CA000003 bgt     22E1A98h			///022E1A98
else if(CurrentOpCode >= 0x92)
	022E1A88 AA00007E bge     22E1C88h			///022E1C88
022E1A8C E3560091 cmp     r6,91h
022E1A90 0A00007C beq     22E1C88h
022E1A94 EA000272 b       22E2464h
//022E1A98 Opcode > 0x92
022E1A98 E3560095 cmp     r6,95h
if(CurrentOpCode > 0x95)
	022E1A9C CA000005 bgt     22E1AB8h			///022E1AB8
022E1AA0 E3560094 cmp     r6,94h
022E1AA4 BA00026E blt     22E2464h
022E1AA8 0A0001E9 beq     22E2254h
022E1AAC E3560095 cmp     r6,95h
022E1AB0 0A0001ED beq     22E226Ch
022E1AB4 EA00026A b       22E2464h
///022E1AB8 Opcode > 0x95
022E1AB8 E35600F5 cmp     r6,0F5h
if(CurrentOpCode == 0xF5)
	022E1ABC 0A000237 beq     22E23A0h			///022E23A0
022E1AC0 EA000267 b       22E2464h				///022E2464
///==================================================================== 0x128
022E1AC4 E3560F5D cmp     r6,174h   //Opcode > 0x174
022E1AC8 CA00001F bgt     22E1B4Ch          ///022E1B4C if Opcode > 0x174
022E1ACC AA0000ED bge     22E1E88h
022E1AD0 E35600FB cmp     r6,0FBh
022E1AD4 CA00000D bgt     22E1B10h  //0x128 true here 022E1B10
022E1AD8 AA0001F6 bge     22E22B8h
022E1ADC E35600F8 cmp     r6,0F8h
022E1AE0 CA000003 bgt     22E1AF4h
022E1AE4 AA0001ED bge     22E22A0h
022E1AE8 E35600F7 cmp     r6,0F7h
022E1AEC 0A000241 beq     22E23F8h
022E1AF0 EA00025B b       22E2464h
022E1AF4 E35600F9 cmp     r6,0F9h
022E1AF8 CA000001 bgt     22E1B04h
022E1AFC 0A000205 beq     22E2318h
022E1B00 EA000257 b       22E2464h
022E1B04 E35600FA cmp     r6,0FAh
022E1B08 0A0001F5 beq     22E22E4h
022E1B0C EA000254 b       22E2464h
///==================================================================== 0x128
022E1B10 E59F0764 ldr     r0,=127h
022E1B14 E1560000 cmp     r6,r0
022E1B18 CA000003 bgt     22E1B2Ch  //0x128 true here 022E1B2C
022E1B1C AA000158 bge     22E2084h
022E1B20 E35600FC cmp     r6,0FCh
022E1B24 0A00022D beq     22E23E0h
022E1B28 EA00024D b       22E2464h
///==================================================================== 0x128
022E1B2C E3560F4A cmp     r6,128h
022E1B30 CA000001 bgt     22E1B3Ch
022E1B34 0A000150 beq     22E207Ch  //0x128 true here 022E207C
022E1B38 EA000249 b       22E2464h
022E1B3C E280004C add     r0,r0,4Ch
022E1B40 E1560000 cmp     r6,r0
022E1B44 0A0000E4 beq     22E1EDCh
022E1B48 EA000245 b       22E2464h
//
022E1B4C E59F172C ldr     r1,=179h
022E1B50 E1560001 cmp     r6,r1
022E1B54 CA000010 bgt     22E1B9Ch          /// 022E1B9C if Opcode > 0x179
022E1B58 AA0000F3 bge     22E1F2Ch
022E1B5C E2410003 sub     r0,r1,3h
022E1B60 E1560000 cmp     r6,r0
022E1B64 CA000004 bgt     22E1B7Ch
022E1B68 AA0000F5 bge     22E1F44h
022E1B6C E2410004 sub     r0,r1,4h
022E1B70 E1560000 cmp     r6,r0
022E1B74 0A0000E9 beq     22E1F20h
022E1B78 EA000239 b       22E2464h
022E1B7C E2410002 sub     r0,r1,2h
022E1B80 E1560000 cmp     r6,r0
022E1B84 CA000001 bgt     22E1B90h
022E1B88 0A0000F3 beq     22E1F5Ch
022E1B8C EA000234 b       22E2464h
022E1B90 E3560F5E cmp     r6,178h
022E1B94 0A0000DB beq     22E1F08h
022E1B98 EA000231 b       22E2464h
//Values bigger than 0x179
022E1B9C E2810002 add     r0,r1,2h              //R0 = 0x179 + 2
022E1BA0 E1560000 cmp     r6,r0                 //Opcode > 0x17B
022E1BA4 CA000004 bgt     22E1BBCh              ///022E1BBC if Opcode > 0x17B
022E1BA8 AA0000C0 bge     22E1EB0h
022E1BAC E2810001 add     r0,r1,1h              //R0 = 0x179 + 1
022E1BB0 E1560000 cmp     r6,r0
022E1BB4 0A0000A9 beq     22E1E60h              ///022E1E60 0x17C
022E1BB8 EA000229 b       22E2464h              ///022E2464 
//Values bigger than 0x17B
022E1BBC E2810004 add     r0,r1,4h              //R0 = 0x179 + 4
022E1BC0 E1560000 cmp     r6,r0
022E1BC4 CA000001 bgt     22E1BD0h              ///022E1BD0 Opcode > 0x17D
022E1BC8 0A0000E6 beq     22E1F68h
022E1BCC EA000224 b       22E2464h
//Bigger than 0x17D
022E1BD0 E2810005 add     r0,r1,5h              //R0 = 0x179 + 5 == 0x17E
022E1BD4 E1560000 cmp     r6,r0
022E1BD8 0A00009C beq     22E1E50h              ///022E1E50 0x17E
022E1BDC EA000220 b       22E2464h              ///022E2464 > 0x17E

///Opcode == 0x8D -> main_EnterDungeon
022E1BE0 E1D500B2 ldrh    r0,[r5,2h]                //r0 = FirstParameter
022E1BE4 EB000B30 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1BE8 E1A06800 mov     r6,r0,lsl 10h				//R6 = FirstParameter << 16
022E1BEC E1D500B4 ldrh    r0,[r5,4h]                //r0 = SecondParameter
022E1BF0 E1A05846 mov     r5,r6,asr 10h				//R5 = r6 >> 16 (Signed)
022E1BF4 EB000B2C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1BF8 E3E01000 mvn     r1,0h                     //r1 = 0xFFFFFFFF
022E1BFC E1A07000 mov     r7,r0                     //r7 = SecondParameter
022E1C00 E1510846 cmp     r1,r6,asr 10h
if( 0xFFFFFFFF != ((FirstParameter >> 10) << 10) )	//If first param isn't -1
    022E1C04 1A000014 bne     22E1C5Ch              ///022E1C5C   v 
022E1C08 EBF5B2D4 bl      204E760h              //0204E760
{
    0204E760 E59F0004 ldr     r0,=22AB4FCh	//Struct in memory
    0204E764 E1D000F2 ldrsh   r0,[r0,2h]	//R0 = 0xFFFF
    0204E768 E12FFF1E bx      r14
    0204E76C 022AB4FC 
}
022E1C0C E1A05000 mov     r5,r0                 //R5 = 0xFFFF
022E1C10 E59F166C ldr     r1,=231958Ch          //"    enter dungeon request %3d"
022E1C14 E1A02005 mov     r2,r5					//R2 = 0xFFFF
022E1C18 E3A00002 mov     r0,2h					//R0 = 2
022E1C1C EBF4A987 bl      200C240h              //Debug Printf
022E1C20 E3E00000 mvn     r0,0h                 //0xFFFFFFFF
022E1C24 E1550000 cmp     r5,r0
if( r5 == 0xFFFFFFFF )							//R5 == -1
    022E1C28 0A000006 beq     22E1C48h          ///022E1C48
022E1C2C E1A00007 mov     r0,r7					//R0 = SecondParameter
022E1C30 EB001E55 bl      22E958Ch              //022E958C
{
    022E958C E92D4010 push    r4,r14
    022E9590 E59F103C ldr     r1,=2324FA0h			//02324FA0 - StructEnter_02324FA0
    022E9594 E1A04000 mov     r4,r0					//R4 = SecondParameter
    022E9598 E591001C ldr     r0,[r1,1Ch]			//R0 = [0x2324FA0 + 0x1C]
    022E959C E3500000 cmp     r0,0h
	if(R0 != 0)
		022E95A0 13A00000 movne   r0,0h				//R0 = 0
		022E95A4 18BD8010 popne   r4,r15			// Return 0
    022E95A8 E59F1028 ldr     r1,=2320B34h          //"GroundMain dungeon request %3d %3d"
    022E95AC E1A02004 mov     r2,r4					//R2 = SecondParameter
    022E95B0 E3A00001 mov     r0,1h					//R0 = 1
    022E95B4 EBF48B21 bl      200C240h              //DebugPrint
    022E95B8 E59F1014 ldr     r1,=2324FA0h			//2324FA0 - StructEnter_02324FA0
    022E95BC E3A00004 mov     r0,4h					//R0 = 4
    022E95C0 E581001C str     r0,[r1,1Ch]			//[2324FA0h + 0x1C] = 4
    022E95C4 E3A00001 mov     r0,1h					//R0 = 1
    022E95C8 E5810010 str     r0,[r1,10h]			//[2324FA0h + 0x10] = 1
    022E95CC E5814018 str     r4,[r1,18h]			//[2324FA0h + 0x18] = SecondParameter
    022E95D0 E8BD8010 pop     r4,r15
    022E95D4 02324FA0
    022E95D8 02320B34
}
022E1C34 E1A00004 mov     r0,r4				//R0 = ScriptEngineStruct
022E1C38 E3A01001 mov     r1,1h				//R1 = 1
022E1C3C EB0009E9 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022E1C40 E584001C str     r0,[r4,1Ch]       //[ptr_curOpcode] = r0
022E1C44 EA000206 b       22E2464h          ///022E2464 - Return 2
///022E1C48 - ( r5 == 0xFFFFFFFF ) 
022E1C48 E1A00004 mov     r0,r4				//R0 = ScriptEngineStruct
022E1C4C E3A01000 mov     r1,0h				//R1 = 0
022E1C50 EB0009E4 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022E1C54 E584001C str     r0,[r4,1Ch]       //[ptr_curOpcode] = r0
022E1C58 EA000201 b       22E2464h          ///022E2464 - Return 2
///022E1C5C - ( 0xFFFFFFFF !=  ((FirstParameter >> 10) << 10) )
022E1C5C E59F1624 ldr     r1,=23195ACh      //"    enter dungeon set %3d"
022E1C60 E1A02005 mov     r2,r5             //r2 = ((FirstParameter >> 10) << 10)
022E1C64 E3A00002 mov     r0,2h             //r0 = 2
022E1C68 EBF4A974 bl      200C240h          //Debug Printf
022E1C6C E1A00005 mov     r0,r5             //r0 =   ((FirstParameter >> 10) << 10)  
022E1C70 EBF5B335 bl      204E94Ch          ///0204E94C
{
    ///fun_0204E94C
    0204E94C E92D4008 push    r3,r14
    0204E950 E1A01000 mov     r1,r0				//R1 = FirstParameter
    0204E954 E59F0010 ldr     r0,=22AB4FCh		//022AB4FC Struct_022AB4FC
    0204E958 EB00003F bl      204EA5Ch
    {
        0204EA5C E92D40F8 push    r3-r7,r14
        0204EA60 E1A07000 mov     r7,r0			//R7 = Struct_022AB4FC 
        0204EA64 E1A06001 mov     r6,r1			//R6 = FirstParameter
        0204EA68 EBFFF154 bl      204AFC0h		///204AFC0
		{
			0204AFC0 E59F0004 ldr     r0,=20AFF70h		//020AFF70
			0204AFC4 E5D00000 ldrb    r0,[r0]			//R0 = [020AFF70]
			0204AFC8 E12FFF1E bx      r14
			0204AFCC 020AFF70 andeq   r15,r10,1C0h
		}
        0204EA6C E1A04000 mov     r4,r0			//R4 = 0x2
        0204EA70 E1A00006 mov     r0,r6			//R0 = FirstParameter
        0204EA74 E1C760B2 strh    r6,[r7,2h]	//[Struct_022AB4FC + 0x2] = FirstParameter
        0204EA78 EB006F34 bl      206A750h		///0206A750 - fun_0206A750
		{
			0206A750 E3E01000 mvn     r1,0h			//R1 = 0
			0206A754 E1500001 cmp     r0,r1			//
			if(R0 != R1)
				0206A758 159F1008 ldrne   r1,=20A93E0h	//R1 = 020A93E0
			else
				0206A75C 03A000FF moveq   r0,0FFh		//R0 = 0xFF
			if(R0 != R1)
				0206A760 17D10000 ldrneb  r0,[r1,r0]	//R0 = [020A93E0 + R0]
			0206A764 E12FFF1E bx      r14
			0206A768 020A93E0
		}
        0204EA7C E5C70000 strb    r0,[r7]			//[Struct_022AB4FC] = R0
        0204EA80 E3A01001 mov     r1,1h				//R1 = 1
        0204EA84 E5C71001 strb    r1,[r7,1h]		//[Struct_022AB4FC + 0x1] = 1
        0204EA88 E2411002 sub     r1,r1,2h			//R1 = 1 - 2 = -1
        0204EA8C E5C71012 strb    r1,[r7,12h]		//[Struct_022AB4FC + 0x12] = -1
        0204EA90 E3A00000 mov     r0,0h				//R0 = 0
        0204EA94 E58700AC str     r0,[r7,0ACh]		//[Struct_022AB4FC + 0xAC] = 0
        0204EA98 E5C7000D strb    r0,[r7,0Dh]		//[Struct_022AB4FC + 0x0D] = 0
        0204EA9C E5C7100E strb    r1,[r7,0Eh]		//[Struct_022AB4FC + 0x0E] = -1
        0204EAA0 E5C70004 strb    r0,[r7,4h]		//[Struct_022AB4FC + 0x4] = 0
        0204EAA4 E5C7000F strb    r0,[r7,0Fh]		//[Struct_022AB4FC + 0xF] = 0
        0204EAA8 E5C70010 strb    r0,[r7,10h]		//[Struct_022AB4FC + 0x10] = 0
        0204EAAC E5C70011 strb    r0,[r7,11h]		//[Struct_022AB4FC + 0x11] = 0
        0204EAB0 E3540002 cmp     r4,2h
		if(R4 != 2)
			0204EAB4 15C70005 strneb  r0,[r7,5h]	//[Struct_022AB4FC + 5] = 0
			0204EAB8 15C70006 strneb  r0,[r7,6h]	//[Struct_022AB4FC + 6] = 0
			0204EABC 15C70008 strneb  r0,[r7,8h]	//[Struct_022AB4FC + 8] = 0
			0204EAC0 1A00000A bne     204EAF0h		///0204EAF0
        0204EAC4 E3A00007 mov     r0,7h				//R0 = 0x7
        0204EAC8 EBFFF7F1 bl      204CA94h			///0204CA94 - fun_0204CA94
        0204EACC E5C70005 strb    r0,[r7,5h]		//[Struct_022AB4FC + 5] = R0
        0204EAD0 E3A00005 mov     r0,5h				//R0 = 5
        0204EAD4 EBFFF7EE bl      204CA94h			///0204CA94 - fun_0204CA94
        0204EAD8 E5C70006 strb    r0,[r7,6h]		//[Struct_022AB4FC + 6] = R0
        0204EADC E3A00006 mov     r0,6h				//R0 = 6
        0204EAE0 EBFFF7EB bl      204CA94h			///0204CA94 - fun_0204CA94
        0204EAE4 E5C70008 strb    r0,[r7,8h]		//[Struct_022AB4FC + 8] = R0
        0204EAE8 E3A00001 mov     r0,1h				//R0 = 1
        0204EAEC EBFFF7E8 bl      204CA94h			///0204CA94 - fun_0204CA94
		///0204EAF0 - If Return of fun_0206A750 is not equal to 2!
        0204EAF0 E5C70009 strb    r0,[r7,9h]		//[Struct_022AB4FC + 9] = R0
        0204EAF4 E3A03000 mov     r3,0h				//R3 = 0
        0204EAF8 E5C7300A strb    r3,[r7,0Ah]		//[Struct_022AB4FC + 0xA] = 0
        0204EAFC E5C7300B strb    r3,[r7,0Bh]		//[Struct_022AB4FC + 0xB] = 0
        0204EB00 E5C73007 strb    r3,[r7,7h]		//[Struct_022AB4FC + 0x7] = 0
        0204EB04 E5C7300C strb    r3,[r7,0Ch]		//[Struct_022AB4FC + 0xC] = 0
        0204EB08 E1A01003 mov     r1,r3				//R1 = 0
        0204EB0C E3A00044 mov     r0,44h			//R0 = 0x44
		///0204EB10
        0204EB10 E0227093 mla     r2,r3,r0,r7		//R2 = R3 * R0 + Struct_022AB4FC
        0204EB14 E5C2101C strb    r1,[r2,1Ch]		//[R2 + 0x1C] = R1
        0204EB18 E2833001 add     r3,r3,1h			//R3 = R3 + 1
        0204EB1C E1C212B0 strh    r1,[r2,20h]		//[R2 + 0x20] = R1
        0204EB20 E3530002 cmp     r3,2h
		if(R3 < 0x2) //signed
			0204EB24 BAFFFFF9 blt     204EB10h		///0204EB10
        0204EB28 E3540005 cmp     r4,5h
		if(R4 != 0x5)
			0204EB2C 1A000015 bne     204EB88h		///0204EB88
        0204EB30 E3A00001 mov     r0,1h				//R0 = 1
        0204EB34 E5C70005 strb    r0,[r7,5h]		//[Struct_022AB4FC + 0x5] = 1
        0204EB38 E5C71006 strb    r1,[r7,6h]		//[Struct_022AB4FC + 0x6] = R1
        0204EB3C E5C71008 strb    r1,[r7,8h]		//[Struct_022AB4FC + 0x8] = R1
        0204EB40 E5C71009 strb    r1,[r7,9h]		//[Struct_022AB4FC + 0x9] = R1
        0204EB44 E5C7100B strb    r1,[r7,0Bh]		//[Struct_022AB4FC + 0xB] = R1
        0204EB48 E3A00003 mov     r0,3h				//R0 = 3
        0204EB4C E5C7000D strb    r0,[r7,0Dh]		//[Struct_022AB4FC + 0xD] = 3
        0204EB50 E5C71004 strb    r1,[r7,4h]		//[Struct_022AB4FC + 0x4] = R1
        0204EB54 E2460026 sub     r0,r6,26h			//R0 = FirstParameter - 0x26
        0204EB58 E5C71011 strb    r1,[r7,11h]		//[Struct_022AB4FC + 0x11] = R1
        0204EB5C E3500005 cmp     r0,5h
		if(R0 <= 5)
			0204EB60 908FF100 addls   r15,r15,r0,lsl 2h	//jump to instruction
		else
			0204EB64 EA000006 b       204EB84h		///0204EB84
        0204EB68 EA000004 b       204EB80h			///0204EB80 - 0
        0204EB6C EA000003 b       204EB80h			///0204EB80 - 1
        0204EB70 EA000002 b       204EB80h			///0204EB80 - 2
        0204EB74 EA000001 b       204EB80h			///0204EB80 - 3
        0204EB78 EA000000 b       204EB80h			///0204EB80 - 4
        0204EB7C EAFFFFFF b       204EB80h			///0204EB80 - 5
		///0204EB80
        0204EB80 E5C71005 strb    r1,[r7,5h]		//[Struct_022AB4FC + 0x5] = R1
		///0204EB84
        0204EB84 E8BD80F8 pop     r3-r7,r15
		///0204EB88
        0204EB88 E1A00006 mov     r0,r6				//R0 = FirstParameter
        0204EB8C EB006EF6 bl      206A76Ch			//0206A76C - fun_0206A76C
		{
			0206A76C E92D4008 push    r3,r14
			0206A770 E35000D6 cmp     r0,0D6h
			if(Param0 == 0xD6)
				0206A774 03A00004 moveq   r0,4h		//R0 = 4
				0206A778 08BD8008 popeq   r3,r15	//Return
			0206A77C E35000B4 cmp     r0,0B4h
			if(R0 < 0xB4)	//Signed
				0206A780 BA000002 blt     206A790h	///0206A790
			0206A784 E35000D3 cmp     r0,0D3h
			if(R0 <= 0xD3)	//signed
				0206A788 D3A00003 movle   r0,3h		//R0 = 3
				0206A78C D8BD8008 pople   r3,r15	//Return
			///0206A790
			0206A790 E35000D7 cmp     r0,0D7h
			if(R0 == 0xD7)
				0206A794 03A00002 moveq   r0,2h		//R0 = 2
				0206A798 08BD8008 popeq   r3,r15	//Return
			0206A79C E35000D8 cmp     r0,0D8h
			if(R0 = 0xD8)
				0206A7A0 03A00003 moveq   r0,3h		//R0 = 3
				0206A7A4 08BD8008 popeq   r3,r15	//Return
			0206A7A8 EBFF89FB bl      204CF9Ch		//0204CF9C - fun_0204CF9C
			{
				0204CF9C E92D4010 push    r4,r14
				0204CFA0 E1A00800 mov     r0,r0,lsl 10h		//R0 = Param0 << 16
				0204CFA4 E1A04820 mov     r4,r0,lsr 10h		//R4 = R0 >> 16
				0204CFA8 E1A02004 mov     r2,r4				//R2 = R4
				0204CFAC E3A00000 mov     r0,0h				//R0 = 0
				0204CFB0 E3A0104F mov     r1,4Fh			//R1 = 0x4F
				0204CFB4 EBFFF9AF bl      204B678h			//0204B678 - fun_0204B678
				0204CFB8 E3500000 cmp     r0,0h
				if(R0 != 0)
					0204CFBC 13A00001 movne   r0,1h			//R0 = 1
				else
					0204CFC0 03A00000 moveq   r0,0h			//R0 = 0
				0204CFC4 E31000FF tst     r0,0FFh			//R0 & 0xFF
				0204CFC8 E1A02004 mov     r2,r4				//R2 = R4
				0204CFCC E3A00000 mov     r0,0h				//R0 = 0
				0204CFD0 E3A01054 mov     r1,54h			//R1 = 0x54
				if(R0 & 0xFF == 0)
					0204CFD4 0A000007 beq     204CFF8h		///0204CFF8
				0204CFD8 EBFFF9A6 bl      204B678h			//0204B678 - fun_0204B678
				0204CFDC E3500000 cmp     r0,0h
				if(R0 != 0)
					0204CFE0 13A00001 movne   r0,1h			//R0 = 1
				else
					0204CFE4 03A00000 moveq   r0,0h			//R0 = 0
				0204CFE8 E31000FF tst     r0,0FFh
				if(R0 & 0xFF != 0)
					0204CFEC 13A00003 movne   r0,3h			//R0 = 3
				else
					0204CFF0 03A00001 moveq   r0,1h			//R0 = 1
				0204CFF4 E8BD8010 pop     r4,r15
				///0204CFF8
				0204CFF8 EBFFF99E bl      204B678h			//0204B678 - fun_0204B678
				0204CFFC E3500000 cmp     r0,0h
				if(R0 != 0)
					0204D000 13A00001 movne   r0,1h			//R0 = 1
				else
					0204D004 03A00000 moveq   r0,0h			//R0 = 0
				0204D008 E31000FF tst     r0,0FFh
				if(R0 & 0xFF != 0)
					0204D00C 13A00002 movne   r0,2h			//R0 = 2
				else
					0204D010 03A00000 moveq   r0,0h			//R0 = 0
				0204D014 E8BD8010 pop     r4,r15			//Return
			}
			0206A7AC E3500001 cmp     r0,1h
			if(R0 == 1)
				0206A7B0 03A00001 moveq   r0,1h				//R0 = 1
				0206A7B4 08BD8008 popeq   r3,r15			//Return
			0206A7B8 E2400002 sub     r0,r0,2h				//R0 = R0 - 2
			0206A7BC E3500001 cmp     r0,1h
			if(R0 <= 1) //signed
				0206A7C0 93A00002 movls   r0,2h				//R0 = 2
			else if(R0 <= 1) //unsigned
				0206A7C4 83A00001 movhi   r0,1h				//R0 = 1
			0206A7C8 E8BD8008 pop     r3,r15				//Return
		}
        0204EB90 E3500001 cmp     r0,1h
		if(R0 == 1)
			0204EB94 0A000005 beq     204EBB0h		///0204EBB0
        0204EB98 E3500002 cmp     r0,2h
		if(R0 == 2)
			0204EB9C 0A0000EB beq     204EF50h		///0204EF50
        0204EBA0 E3500003 cmp     r0,3h
		if(R0 == 3)
			0204EBA4 03A00000 moveq   r0,0h			//R0 = 0
			0204EBA8 05C7000D streqb  r0,[r7,0Dh]	//[Struct_022AB4FC + 0xD] = R0
        0204EBAC EA0000F5 b       204EF88h			///0204EF88
		///0204EBB0
        0204EBB0 E3A04000 mov     r4,0h				//R4 = 0
        0204EBB4 E1A00006 mov     r0,r6				//R0 = FirstParameter
        0204EBB8 E5C7400D strb    r4,[r7,0Dh]		//[Struct_022AB4FC + 0xD] = 0
        0204EBBC E1A05004 mov     r5,r4				//R5 = 0
        0204EBC0 EB000106 bl      204EFE0h			//0204EFE0 - fun_0204EFE0
		{
			0204EFE0 E92D4010 push    r4,r14
			0204EFE4 E350001A cmp     r0,1Ah
			0204EFE8 E3A04000 mov     r4,0h			//R4 = 0
			0204EFEC 13500079 cmpne   r0,79h
			if(R0 == 0x1A || R0 == 0x79)
				0204EFF0 0A000002 beq     204F000h	///0204F000
			0204EFF4 E350007D cmp     r0,7Dh
			if(R0 == 0x7D)
				0204EFF8 0A000002 beq     204F008h	///0204F008
			0204EFFC EA000006 b       204F01Ch		///0204F01C
			///0204F000
			0204F000 E3A04001 mov     r4,1h			//R4 = 1
			0204F004 EA000004 b       204F01Ch		///0204F01C
			///0204F008
			0204F008 E1A00004 mov     r0,r4			//R0 = R4
			0204F00C E3A01010 mov     r1,10h		//R1 = 0x10
			0204F010 EBFFF135 bl      204B4ECh		//0204B4EC - fun_0204B4EC
			0204F014 E3500000 cmp     r0,0h
			if(R0 == 0)
				0204F018 03A04001 moveq   r4,1h		//R4 = 1
			///0204F01C
			0204F01C E1A00004 mov     r0,r4			//R0 = R4
			0204F020 E8BD8010 pop     r4,r15		//Return
		}
        0204EBC4 E5C7000C strb    r0,[r7,0Ch]		//[Struct_022AB4FC + 0xC] = R0
        0204EBC8 E3560044 cmp     r6,44h
		if(FirstParameter > 0x44) //signed
			0204EBCC CA000034 bgt     204ECA4h		///0204ECA4
		else if(FirstParameter >= 0x44) //signed
			0204EBD0 AA0000A6 bge     204EE70h		///0204EE70
        0204EBD4 E356003E cmp     r6,3Eh
		if(FirstParameter > 0x3E)
			0204EBD8 CA000020 bgt     204EC60h		///0204EC60
		else if(FirstParameter >= 0x3E)
			0204EBDC AA0000A1 bge     204EE68h		///0204EE68
        0204EBE0 E3560021 cmp     r6,21h
		if(FirstParameter > 0x21)
			0204EBE4 CA00000C bgt     204EC1Ch		///0204EC1C
		else if(FirstParameter >= 0x21)
			0204EBE8 AA00007F bge     204EDECh		///0204EDEC
        0204EBEC E356000B cmp     r6,0Bh
		if(FirstParameter > 0xB)
			0204EBF0 CA000006 bgt     204EC10h		///0204EC10
        0204EBF4 E2560008 subs    r0,r6,8h			//R0 = FirstParameter - 8 (Result keeps conditional flag)
		if(R0 >= 0)
			0204EBF8 508FF100 addpl   r15,r15,r0,lsl 2h	//Jump to instruction
		else
			0204EBFC EA0000CC b       204EF34h		///0204EF34 
        0204EC00 EA000060 b       204ED88h			///0204ED88 0
        0204EC04 EA000064 b       204ED9Ch			///0204ED9C 1
        0204EC08 EA000068 b       204EDB0h			///0204EDB0 2
        0204EC0C EA00006C b       204EDC4h			///0204EDC4 3
		///0204EC10
        0204EC10 E3560020 cmp     r6,20h
		if(FirstParameter == 0x20)
			0204EC14 0A00006F beq     204EDD8h		///0204EDD8
        0204EC18 EA0000C5 b       204EF34h			///0204EF34
		///0204EC1C
        0204EC1C E3560022 cmp     r6,22h
		if(FirstParameter > 0x22) //signed
			0204EC20 CA000001 bgt     204EC2Ch		///0204EC2C
		else if(FirstParameter == 0x22)
			0204EC24 0A000079 beq     204EE10h		///0204EE10
        0204EC28 EA0000C1 b       204EF34h			///0204EF34
		///0204EC2C
        0204EC2C E2460023 sub     r0,r6,23h			//R0 = FirstParameter - 0x23
        0204EC30 E3500008 cmp     r0,8h
		if(R0 <= 0x8) //unsigned
			0204EC34 908FF100 addls   r15,r15,r0,lsl 2h	//Jump to instruction
		else
			0204EC38 EA0000BD b       204EF34h		///0204EF34 
        0204EC3C EA000078 b       204EE24h			///0204EE24 0
        0204EC40 EA0000BB b       204EF34h			///0204EF34 1
        0204EC44 EA0000BA b       204EF34h			///0204EF34 2 
        0204EC48 EA00007E b       204EE48h			///0204EE48 3
        0204EC4C EA00007D b       204EE48h			///0204EE48 4
        0204EC50 EA00007C b       204EE48h			///0204EE48 5
        0204EC54 EA000081 b       204EE60h			///0204EE60 6
        0204EC58 EA000080 b       204EE60h			///0204EE60 7
        0204EC5C EA00007F b       204EE60h			///0204EE60 8
		///0204EC60
        0204EC60 E3560041 cmp     r6,41h
		if(FirstParameter > 0x41) //signed
			0204EC64 CA000007 bgt     204EC88h		///0204EC88
		else if(FirstParameter >= 0x41) //signed
			0204EC68 AA00007E bge     204EE68h		///0204EE68
        0204EC6C E356003F cmp     r6,3Fh
		if(FirstParameter > 0x3F)	//signed
			0204EC70 CA000001 bgt     204EC7Ch		///0204EC7C
		else if(FirstParameter >= 0x3F)	//signed
			0204EC74 0A00007B beq     204EE68h		///0204EE68
        0204EC78 EA0000AD b       204EF34h			///0204EF34
		///0204EC7C
        0204EC7C E3560040 cmp     r6,40h
		if(FirstParameter == 0x40)
			0204EC80 0A000078 beq     204EE68h		///0204EE68
        0204EC84 EA0000AA b       204EF34h			///0204EF34
		///0204EC88
        0204EC88 E3560042 cmp     r6,42h
		if(FirstParameter > 0x42)
			0204EC8C CA000001 bgt     204EC98h		///0204EC98
		else if(FirstParameter == 0x42)
			0204EC90 0A000074 beq     204EE68h		///0204EE68
        0204EC94 EA0000A6 b       204EF34h			///0204EF34
		///0204EC98
        0204EC98 E3560043 cmp     r6,43h
		if(FirstParameter == 0x43)
			0204EC9C 0A000073 beq     204EE70h		///0204EE70
        0204ECA0 EA0000A3 b       204EF34h			///0204EF34
		///0204ECA4
        0204ECA4 E3560073 cmp     r6,73h
		if(FirstParameter > 0x73)//signed
			0204ECA8 CA000011 bgt     204ECF4h		///0204ECF4
		else if(FirstParameter >= 0x73) //signed
			0204ECAC AA000075 bge     204EE88h		///0204EE88
        0204ECB0 E3560070 cmp     r6,70h
		if(FirstParameter > 0x70) //signed
			0204ECB4 CA000007 bgt     204ECD8h		///0204ECD8
        else if(FirstParameter >= 0x70)//signed
			0204ECB8 AA000072 bge     204EE88h		///0204EE88
        0204ECBC E3560045 cmp     r6,45h
		if(FirstParameter > 0x45) //signed
			0204ECC0 CA000001 bgt     204ECCCh		///0204ECCC
		else if(FirstParameter == 0x45)
			0204ECC4 0A000069 beq     204EE70h		///0204EE70
        0204ECC8 EA000099 b       204EF34h			///0204EF34
		///0204ECCC
        0204ECCC E356006F cmp     r6,6Fh
		if(FirstParameter == 0x6F)
			0204ECD0 0A00006C beq     204EE88h		///0204EE88
        0204ECD4 EA000096 b       204EF34h			///0204EF34
		///0204ECD8
        0204ECD8 E3560071 cmp     r6,71h
		if(FirstParameter > 0x71)//signed
			0204ECDC CA000001 bgt     204ECE8h		///0204ECE8
		else if(FirstParameter == 0x71)
			0204ECE0 0A000068 beq     204EE88h		///0204EE88
        0204ECE4 EA000092 b       204EF34h			///0204EF34
		///0204ECE8
        0204ECE8 E3560072 cmp     r6,72h
		if(FirstParameter == 0x72)
			0204ECEC 0A000065 beq     204EE88h		///0204EE88
        0204ECF0 EA00008F b       204EF34h			///0204EF34
		///0204ECF4
        0204ECF4 E3560076 cmp     r6,76h
		if(FirstParameter > 0x76)//signed
			0204ECF8 CA000007 bgt     204ED1Ch		///0204ED1C
		else if(FirstParameter >= 0x76)
			0204ECFC AA000061 bge     204EE88h		///0204EE88
        0204ED00 E3560074 cmp     r6,74h
		if(FirstParameter > 0x74) //signed
			0204ED04 CA000001 bgt     204ED10h		///0204ED10
		else if(FirstParameter == 0x74)
			0204ED08 0A00005E beq     204EE88h		///0204EE88
        0204ED0C EA000088 b       204EF34h			///0204EF34
		///0204ED10
        0204ED10 E3560075 cmp     r6,75h
		if(FirstParameter == 0x75)
			0204ED14 0A00005B beq     204EE88h		///0204EE88
        0204ED18 EA000085 b       204EF34h			///0204EF34
		///0204ED1C
        0204ED1C E3560087 cmp     r6,87h
		if(FirstParameter > 0x87)
			0204ED20 CA000015 bgt     204ED7Ch		///0204ED7C
        0204ED24 E2560078 subs    r0,r6,78h			//R0 = FirstParameter -0x78 (Keep conditional flags)
		if(R0 >= 0)
			0204ED28 508FF100 addpl   r15,r15,r0,lsl 2h	//Jump to instruction
		else
			0204ED2C EA00000F b       204ED70h		///0204ED70
        0204ED30 EA000054 b       204EE88h			///0204EE88 0
        0204ED34 EA000053 b       204EE88h			///0204EE88 1
        0204ED38 EA000052 b       204EE88h			///0204EE88 2
        0204ED3C EA000056 b       204EE9Ch			///0204EE9C 3
        0204ED40 EA00005F b       204EEC4h			///0204EEC4 4
        0204ED44 EA00007A b       204EF34h			///0204EF34 5
        0204ED48 EA000079 b       204EF34h			///0204EF34 6
        0204ED4C EA000078 b       204EF34h			///0204EF34 7
        0204ED50 EA000065 b       204EEECh			///0204EEEC 8
        0204ED54 EA000076 b       204EF34h			///0204EF34 9
        0204ED58 EA000075 b       204EF34h			///0204EF34 10
        0204ED5C EA000074 b       204EF34h			///0204EF34 11
        0204ED60 EA000073 b       204EF34h			///0204EF34 12
        0204ED64 EA000072 b       204EF34h			///0204EF34 13
        0204ED68 EA000071 b       204EF34h			///0204EF34 14
        0204ED6C EA000067 b       204EF10h			///0204EF10 15
		///0204ED70
        0204ED70 E3560077 cmp     r6,77h
		if(FirstParameter == 0x77)
			0204ED74 0A000043 beq     204EE88h		///0204EE88
        0204ED78 EA00006D b       204EF34h			///0204EF34
		///0204ED7C
        0204ED7C E35600A4 cmp     r6,0A4h
		if(FirstParameter == 0xA4)
			0204ED80 0A000067 beq     204EF24h		///0204EF24
        0204ED84 EA00006A b       204EF34h			///0204EF34
		///0204ED88
        0204ED88 E59F2208 ldr     r2,=20A2904h
        0204ED8C E1A00007 mov     r0,r7
        0204ED90 E1A01004 mov     r1,r4
        0204ED94 EB0002D1 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
		{
			0204F8E0 E92D4078 push    r3-r6,r14
			0204F8E4 E24DD03C sub     r13,r13,3Ch
			0204F8E8 E28DE018 add     r14,r13,18h
			0204F8EC E1A05000 mov     r5,r0
			0204F8F0 E1A04001 mov     r4,r1
			0204F8F4 E1A06002 mov     r6,r2
			0204F8F8 E3A0C002 mov     r12,2h
			0204F8FC E8B6000F ldmia   [r6]!,r0-r3
			0204F900 E8AE000F stmia   [r14]!,r0-r3
			0204F904 E25CC001 subs    r12,r12,1h
			0204F908 1AFFFFFB bne     204F8FCh
			0204F90C E5961000 ldr     r1,[r6]
			0204F910 E3A00001 mov     r0,1h
			0204F914 E58E1000 str     r1,[r14]
			0204F918 E5C5000F strb    r0,[r5,0Fh]
			0204F91C E1DD01FC ldrsh   r0,[r13,1Ch]
			0204F920 EB000B68 bl      20526C8h
			0204F924 E1A01000 mov     r1,r0
			0204F928 E28D0000 add     r0,r13,0h
			0204F92C E3A0200A mov     r2,0Ah
			0204F930 EBFF5677 bl      2025314h
			0204F934 E28D2000 add     r2,r13,0h
			0204F938 E285101C add     r1,r5,1Ch
			0204F93C E3A00044 mov     r0,44h
			0204F940 E0201094 mla     r0,r4,r0,r1
			0204F944 E28D1018 add     r1,r13,18h
			0204F948 E58D2018 str     r2,[r13,18h]
			0204F94C EB000D3F bl      2052E50h
			0204F950 E28DD03C add     r13,r13,3Ch
			0204F954 E8BD8078 pop     r3-r6,r15
		}
        0204ED98 EA000065 b       204EF34h			///0204EF34
		///0204ED9C
        0204ED9C E59F21F8 ldr     r2,=20A2AD8h
        0204EDA0 E1A00007 mov     r0,r7
        0204EDA4 E1A01004 mov     r1,r4
        0204EDA8 EB0002CC bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EDAC EA000060 b       204EF34h			///0204EF34
		///0204EDB0
        0204EDB0 E59F21E8 ldr     r2,=20A2AB4h
        0204EDB4 E1A00007 mov     r0,r7
        0204EDB8 E1A01004 mov     r1,r4
        0204EDBC EB0002C7 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EDC0 EA00005B b       204EF34h			///0204EF34
		///0204EDC4
        0204EDC4 E59F21D8 ldr     r2,=20A294Ch
        0204EDC8 E1A00007 mov     r0,r7
        0204EDCC E1A01004 mov     r1,r4
        0204EDD0 EB0002C2 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EDD4 EA000056 b       204EF34h			///0204EF34
		///0204EDD8
        0204EDD8 E59F21C8 ldr     r2,=20A2970h
        0204EDDC E1A00007 mov     r0,r7
        0204EDE0 E3A01000 mov     r1,0h
        0204EDE4 EB0002BD bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EDE8 EA000051 b       204EF34h			///0204EF34
		///0204EDEC
        0204EDEC E59F21B8 ldr     r2,=20A2A48h
        0204EDF0 E1A00007 mov     r0,r7
        0204EDF4 E1A01004 mov     r1,r4
        0204EDF8 EB0002B8 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EDFC E59F21AC ldr     r2,=20A2A24h
        0204EE00 E1A00007 mov     r0,r7
        0204EE04 E3A01001 mov     r1,1h
        0204EE08 EB0002B4 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EE0C EA000048 b       204EF34h			///0204EF34
		///0204EE10
        0204EE10 E59F219C ldr     r2,=20A29DCh
        0204EE14 E1A00007 mov     r0,r7
        0204EE18 E1A01004 mov     r1,r4
        0204EE1C EB0002AF bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EE20 EA000043 b       204EF34h			///0204EF34
		///0204EE24
        0204EE24 E3A00019 mov     r0,19h
        0204EE28 EBFFF719 bl      204CA94h
        0204EE2C E3500000 cmp     r0,0h
        0204EE30 1A00003F bne     204EF34h			///0204EF34
        0204EE34 E59F217C ldr     r2,=20A2A00h
        0204EE38 E1A00007 mov     r0,r7
        0204EE3C E1A01004 mov     r1,r4
        0204EE40 EB0002A6 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EE44 EA00003A b       204EF34h			///0204EF34
        0204EE48 E59F216C ldr     r2,=20A2A6Ch
        0204EE4C E1A00007 mov     r0,r7
        0204EE50 E1A01004 mov     r1,r4
        0204EE54 E3A05001 mov     r5,1h
        0204EE58 EB0002A0 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EE5C EA000034 b       204EF34h			///0204EF34
        0204EE60 E3A04001 mov     r4,1h
        0204EE64 EA000032 b       204EF34h			///0204EF34
		///0204EE68
        0204EE68 E3A04001 mov     r4,1h
        0204EE6C EA000030 b       204EF34h			///0204EF34
		///0204EE70
        0204EE70 E59F2148 ldr     r2,=20A2A90h
        0204EE74 E1A00007 mov     r0,r7
        0204EE78 E3A01000 mov     r1,0h
        0204EE7C EB000297 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EE80 E3A04001 mov     r4,1h
        0204EE84 EA00002A b       204EF34h			///0204EF34
		///0204EE88
        0204EE88 E59F2134 ldr     r2,=20A2AFCh
        0204EE8C E1A00007 mov     r0,r7
        0204EE90 E3A01000 mov     r1,0h
        0204EE94 EB000291 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EE98 EA000025 b       204EF34h			///0204EF34
		///0204EE9C
        0204EE9C E1A00004 mov     r0,r4
        0204EEA0 E3A01010 mov     r1,10h
        0204EEA4 EBFFF190 bl      204B4ECh
        0204EEA8 E3500000 cmp     r0,0h
        0204EEAC 1A000020 bne     204EF34h			///0204EF34
        0204EEB0 E59F2110 ldr     r2,=20A2B20h
        0204EEB4 E1A00007 mov     r0,r7
        0204EEB8 E1A01004 mov     r1,r4
        0204EEBC EB000287 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EEC0 EA00001B b       204EF34h			///0204EF34
		///0204EEC4
        0204EEC4 E1A00004 mov     r0,r4
        0204EEC8 E3A01010 mov     r1,10h
        0204EECC EBFFF186 bl      204B4ECh
        0204EED0 E3500000 cmp     r0,0h
        0204EED4 1A000016 bne     204EF34h			///0204EF34
        0204EED8 E59F20EC ldr     r2,=20A2928h
        0204EEDC E1A00007 mov     r0,r7
        0204EEE0 E1A01004 mov     r1,r4
        0204EEE4 EB00027D bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EEE8 EA000011 b       204EF34h			///0204EF34
		///0204EEEC
        0204EEEC E59F20DC ldr     r2,=20A28BCh
        0204EEF0 E1A00007 mov     r0,r7
        0204EEF4 E1A01004 mov     r1,r4
        0204EEF8 EB000278 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EEFC E59F20D0 ldr     r2,=20A28E0h
        0204EF00 E1A00007 mov     r0,r7
        0204EF04 E3A01001 mov     r1,1h
        0204EF08 EB000274 bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EF0C EA000008 b       204EF34h			///0204EF34
		///0204EF10
        0204EF10 E59F20C0 ldr     r2,=20A29B8h
        0204EF14 E1A00007 mov     r0,r7
        0204EF18 E1A01004 mov     r1,r4
        0204EF1C EB00026F bl      204F8E0h			//0204F8E0 - fun_0204F8E0
        0204EF20 EA000003 b       204EF34h			///0204EF34
		///0204EF24
        0204EF24 E59F20B0 ldr     r2,=20A2994h
        0204EF28 E1A00007 mov     r0,r7
        0204EF2C E1A01004 mov     r1,r4
        0204EF30 EB00026A bl      204F8E0h			//0204F8E0 - fun_0204F8E0
		///0204EF34
        0204EF34 E3540000 cmp     r4,0h
        0204EF38 13A00001 movne   r0,1h
        0204EF3C 15C7000B strneb  r0,[r7,0Bh]
        0204EF40 E3550000 cmp     r5,0h
        0204EF44 13A00001 movne   r0,1h
        0204EF48 15C70007 strneb  r0,[r7,7h]
        0204EF4C EA00000D b       204EF88h
		///0204EF50
        0204EF50 E2460026 sub     r0,r6,26h
        0204EF54 E3500005 cmp     r0,5h
        0204EF58 908FF100 addls   r15,r15,r0,lsl 2h
        0204EF5C EA000007 b       204EF80h
        0204EF60 EA000004 b       204EF78h
        0204EF64 EA000003 b       204EF78h
        0204EF68 EA000002 b       204EF78h
        0204EF6C EA000001 b       204EF78h
        0204EF70 EA000000 b       204EF78h
        0204EF74 EAFFFFFF b       204EF78h
        0204EF78 E3A00001 mov     r0,1h
        0204EF7C E5C7000B strb    r0,[r7,0Bh]
        0204EF80 E3A00001 mov     r0,1h
        0204EF84 E5C7000D strb    r0,[r7,0Dh]
		///0204EF88
        0204EF88 E3A00000 mov     r0,0h				//R0 = 0
        0204EF8C E5C70004 strb    r0,[r7,4h]		//[Struct_022AB4FC + 0x4] = 0
        0204EF90 E5C70011 strb    r0,[r7,11h]		//[Struct_022AB4FC + 0x11] = 0
        0204EF94 E8BD80F8 pop     r3-r7,r15			//Return
    }
    0204E95C E59F000C ldr     r0,=22AB4F8h		//R0 = 022AB4F8
    0204E960 E3A01001 mov     r1,1h				//R1 = 1
    0204E964 E5C01000 strb    r1,[r0]			//[022AB4F8] = 1
    0204E968 E8BD8008 pop     r3,r15			//Return
}
022E1C74 E1A00004 mov     r0,r4				//R0 = ScriptEngineStruct
022E1C78 E3A01001 mov     r1,1h				//R1 = 1
022E1C7C EB0009D9 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022E1C80 E584001C str     r0,[r4,1Ch]       //[ptr_curOpcode] = r0
022E1C84 EA0001F6 b       22E2464h          ///022E2464 - Return 2 
///END main_EnterDungeon
022E1C88 E1D500B2 ldrh    r0,[r5,2h]
022E1C8C EB000B06 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1C90 E1A07800 mov     r7,r0,lsl 10h
022E1C94 E1D500B4 ldrh    r0,[r5,4h]
022E1C98 E1A08847 mov     r8,r7,asr 10h
022E1C9C EB000B02 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1CA0 E3E00000 mvn     r0,0h
022E1CA4 E1500847 cmp     r0,r7,asr 10h
022E1CA8 1A000012 bne     22E1CF8h
022E1CAC EBF5A4C3 bl      204AFC0h
022E1CB0 E3560091 cmp     r6,91h
022E1CB4 03A00002 moveq   r0,2h
022E1CB8 0A000008 beq     22E1CE0h
022E1CBC E3560092 cmp     r6,92h
022E1CC0 03A00003 moveq   r0,3h
022E1CC4 0A000005 beq     22E1CE0h
022E1CC8 E3500005 cmp     r0,5h
022E1CCC 03A00004 moveq   r0,4h
022E1CD0 0A000002 beq     22E1CE0h
022E1CD4 E3500003 cmp     r0,3h
022E1CD8 03A00001 moveq   r0,1h
022E1CDC 13A00000 movne   r0,0h
022E1CE0 E3E01000 mvn     r1,0h
022E1CE4 EB007770 bl      22FFAACh
022E1CE8 E3A00000 mov     r0,0h
022E1CEC E1C404B4 strh    r0,[r4,44h]
022E1CF0 E3A00005 mov     r0,5h
022E1CF4 EA0001DF b       22E2478h        //Return 022E2478
022E1CF8 E35800FB cmp     r8,0FBh
022E1CFC 1A00001B bne     22E1D70h
022E1D00 EBF5B281 bl      204E70Ch
022E1D04 E286106F add     r1,r6,6Fh
022E1D08 E2811CFF add     r1,r1,0FF00h
022E1D0C E1A01801 mov     r1,r1,lsl 10h
022E1D10 E1A01821 mov     r1,r1,lsr 10h
022E1D14 E1A05000 mov     r5,r0
022E1D18 E3510001 cmp     r1,1h
022E1D1C 959F0568 ldrls   r0,=296h
022E1D20 9A00000D bls     22E1D5Ch
022E1D24 E3550002 cmp     r5,2h
022E1D28 059F0560 ldreq   r0,=297h
022E1D2C 0A00000A beq     22E1D5Ch
022E1D30 E3550003 cmp     r5,3h
022E1D34 03A00FA6 moveq   r0,298h
022E1D38 0A000007 beq     22E1D5Ch
022E1D3C E3A00000 mov     r0,0h
022E1D40 EBF5AB53 bl      204CA94h
022E1D44 E3500000 cmp     r0,0h
022E1D48 159F0544 ldrne   r0,=299h
022E1D4C 1A000002 bne     22E1D5Ch
022E1D50 E3550001 cmp     r5,1h
022E1D54 059F053C ldreq   r0,=295h
022E1D58 13A00FA5 movne   r0,294h
022E1D5C E3A01000 mov     r1,0h
022E1D60 EB005508 bl      22F7188h          //022F7188 GroundSupervision ExecuteCommon
022E1D64 E3A00001 mov     r0,1h
022E1D68 E1C400BE strh    r0,[r4,0Eh]
022E1D6C EA0001BC b       22E2464h
022E1D70 E35800FA cmp     r8,0FAh
022E1D74 03A00001 moveq   r0,1h
022E1D78 01C404B4 streqh  r0,[r4,44h]
022E1D7C 0A000027 beq     22E1E20h
022E1D80 E1A00008 mov     r0,r8
022E1D84 EBF62271 bl      206A750h
022E1D88 E1A05000 mov     r5,r0
022E1D8C E1A02008 mov     r2,r8
022E1D90 E3A00000 mov     r0,0h
022E1D94 E3A01027 mov     r1,27h
022E1D98 EBF5A6A0 bl      204B820h
022E1D9C E1A02008 mov     r2,r8
022E1DA0 E3A00000 mov     r0,0h
022E1DA4 E3A01029 mov     r1,29h
022E1DA8 EBF5A69C bl      204B820h
022E1DAC E1A00008 mov     r0,r8
022E1DB0 EBF5B2E5 bl      204E94Ch
022E1DB4 E356008C cmp     r6,8Ch
022E1DB8 0A000004 beq     22E1DD0h
022E1DBC E1A02008 mov     r2,r8
022E1DC0 E3A00000 mov     r0,0h
022E1DC4 E3A01026 mov     r1,26h
022E1DC8 EBF5A694 bl      204B820h
022E1DCC EA000011 b       22E1E18h
022E1DD0 E1A00005 mov     r0,r5
022E1DD4 EBF5B57A bl      204F3C4h
022E1DD8 E3100020 tst     r0,20h
022E1DDC 13A01007 movne   r1,7h
022E1DE0 1A000008 bne     22E1E08h
022E1DE4 E3100040 tst     r0,40h
022E1DE8 13A01008 movne   r1,8h
022E1DEC 1A000005 bne     22E1E08h
022E1DF0 E3100001 tst     r0,1h
022E1DF4 13A01012 movne   r1,12h
022E1DF8 1A000002 bne     22E1E08h
022E1DFC E3500000 cmp     r0,0h
022E1E00 03A01000 moveq   r1,0h
022E1E04 13A01013 movne   r1,13h
022E1E08 E1A00004 mov     r0,r4
022E1E0C EB000975 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022E1E10 E584001C str     r0,[r4,1Ch]
022E1E14 EA000192 b       22E2464h
022E1E18 E3A00001 mov     r0,1h
022E1E1C E1C404B4 strh    r0,[r4,44h]
022E1E20 E3A00005 mov     r0,5h
022E1E24 EA000193 b       22E2478h        //Return 022E2478
022E1E28 E1D500B2 ldrh    r0,[r5,2h]
022E1E2C EB000A9E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned                  ///022E48AC (R0)
022E1E30 E1A05000 mov     r5,r0
022E1E34 E1A00004 mov     r0,r4
022E1E38 E3A01001 mov     r1,1h
022E1E3C EB000969 bl      22E43E8h          //022E43E8 ScriptCaseProcess
022E1E40 E584001C str     r0,[r4,1Ch]
022E1E44 E1A00005 mov     r0,r5
022E1E48 EB001DCF bl      22E958Ch
022E1E4C EA000184 b       22E2464h
//Values bigger or equal to 0x17E
022E1E50 E1D500B2 ldrh    r0,[r5,2h]                //Load the int16 after the current data
022E1E54 EB000A94 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned 
022E1E58 EB00B469 bl      230F004h                  ///0230F004 (R0)
022E1E5C EA000180 b       22E2464h                  ///022E2464 LBL022E2464 V
//Value == 0x17C
022E1E60 E1D500B2 ldrh    r0,[r5,2h]                
022E1E64 EB000A90 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1E68 E1A04000 mov     r4,r0
022E1E6C E1A00C04 mov     r0,r4,lsl 18h
022E1E70 E1A00C40 mov     r0,r0,asr 18h
022E1E74 EBF5ABDB bl      204CDE8h
022E1E78 E1A00C04 mov     r0,r4,lsl 18h
022E1E7C E1A00C40 mov     r0,r0,asr 18h
022E1E80 EB00B472 bl      230F050h
022E1E84 EA000176 b       22E2464h
022E1E88 E1D500B2 ldrh    r0,[r5,2h]
022E1E8C EB000A86 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1E90 E1A04000 mov     r4,r0
022E1E94 E1A00C04 mov     r0,r4,lsl 18h
022E1E98 E1A00C40 mov     r0,r0,asr 18h
022E1E9C EBF5ABD1 bl      204CDE8h
022E1EA0 E1A00C04 mov     r0,r4,lsl 18h
022E1EA4 E1A00C40 mov     r0,r0,asr 18h
022E1EA8 EB00B477 bl      230F08Ch
022E1EAC EA00016C b       22E2464h
022E1EB0 E1D500B2 ldrh    r0,[r5,2h]
022E1EB4 EB000A7C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1EB8 E1A04000 mov     r4,r0
022E1EBC E1A00804 mov     r0,r4,lsl 10h
022E1EC0 E1A00840 mov     r0,r0,asr 10h
022E1EC4 E3A01001 mov     r1,1h
022E1EC8 EBF5ABDE bl      204CE48h
022E1ECC E1A00804 mov     r0,r4,lsl 10h
022E1ED0 E1A00840 mov     r0,r0,asr 10h
022E1ED4 EB00B4A9 bl      230F180h
022E1ED8 EA000161 b       22E2464h
022E1EDC E1D500B2 ldrh    r0,[r5,2h]
022E1EE0 EB000A71 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1EE4 E1A04000 mov     r4,r0
022E1EE8 E1A00804 mov     r0,r4,lsl 10h
022E1EEC E1A00840 mov     r0,r0,asr 10h
022E1EF0 E3A01001 mov     r1,1h
022E1EF4 EBF5ABD3 bl      204CE48h
022E1EF8 E1A00804 mov     r0,r4,lsl 10h
022E1EFC E1A00840 mov     r0,r0,asr 10h
022E1F00 EB00B4AB bl      230F1B4h
022E1F04 EA000156 b       22E2464h
022E1F08 E1D500B2 ldrh    r0,[r5,2h]
022E1F0C EB000A66 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1F10 E1A00800 mov     r0,r0,lsl 10h
022E1F14 E1A00840 mov     r0,r0,asr 10h
022E1F18 EB00B4B3 bl      230F1ECh
022E1F1C EA000150 b       22E2464h
022E1F20 E2610F5E rsb     r0,r1,178h
022E1F24 EB00B4B0 bl      230F1ECh
022E1F28 EA00014D b       22E2464h
022E1F2C E1D500B2 ldrh    r0,[r5,2h]
022E1F30 EB000A5D bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1F34 E1A00800 mov     r0,r0,lsl 10h
022E1F38 E1A00840 mov     r0,r0,asr 10h
022E1F3C EB00B460 bl      230F0C4h
022E1F40 EA000147 b       22E2464h
022E1F44 E1D500B2 ldrh    r0,[r5,2h]
022E1F48 EB000A57 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1F4C E1A00800 mov     r0,r0,lsl 10h
022E1F50 E1A00840 mov     r0,r0,asr 10h
022E1F54 EB00B464 bl      230F0ECh
022E1F58 EA000141 b       22E2464h
022E1F5C E2610F5E rsb     r0,r1,178h
022E1F60 EB00B4AD bl      230F21Ch
022E1F64 EA00013E b       22E2464h
022E1F68 E1D500B2 ldrh    r0,[r5,2h]
022E1F6C EB000A4E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1F70 E1A00800 mov     r0,r0,lsl 10h
022E1F74 E1A00840 mov     r0,r0,asr 10h
022E1F78 EB00B4A7 bl      230F21Ch
022E1F7C EA000138 b       22E2464h
022E1F80 E1D500B2 ldrh    r0,[r5,2h]
022E1F84 EB000A48 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1F88 E1A06000 mov     r6,r0
022E1F8C E1D500B4 ldrh    r0,[r5,4h]
022E1F90 EB000A45 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1F94 E1A04000 mov     r4,r0
022E1F98 E1D500B6 ldrh    r0,[r5,6h]
022E1F9C EB000A42 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1FA0 E1A02806 mov     r2,r6,lsl 10h
022E1FA4 E1CD0CBA strh    r0,[r13,0CAh]
022E1FA8 E28D10C8 add     r1,r13,0C8h
022E1FAC E1A00842 mov     r0,r2,asr 10h
022E1FB0 E1CD4CB8 strh    r4,[r13,0C8h]
022E1FB4 EBF60F3E bl      2065CB4h
022E1FB8 EA000129 b       22E2464h
022E1FBC E3A00005 mov     r0,5h
022E1FC0 EA00012C b       22E2478h        //Return 022E2478
022E1FC4 E1D500B2 ldrh    r0,[r5,2h]
022E1FC8 EB000A37 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1FCC E1A06000 mov     r6,r0
022E1FD0 E1D500B4 ldrh    r0,[r5,4h]
022E1FD4 EB000A34 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E1FD8 E1A05000 mov     r5,r0
022E1FDC E1A01805 mov     r1,r5,lsl 10h
022E1FE0 E284006C add     r0,r4,6Ch
022E1FE4 E1A01841 mov     r1,r1,asr 10h
022E1FE8 E3A02000 mov     r2,0h
022E1FEC EBF5A5A1 bl      204B678h
022E1FF0 E1A01805 mov     r1,r5,lsl 10h
022E1FF4 E1CD0CB4 strh    r0,[r13,0C4h]
022E1FF8 E284006C add     r0,r4,6Ch
022E1FFC E1A01841 mov     r1,r1,asr 10h
022E2000 E3A02001 mov     r2,1h
022E2004 EBF5A59B bl      204B678h
022E2008 E1A02806 mov     r2,r6,lsl 10h
022E200C E1CD0CB6 strh    r0,[r13,0C6h]
022E2010 E28D10C4 add     r1,r13,0C4h
022E2014 E1A00842 mov     r0,r2,asr 10h
022E2018 EBF60F25 bl      2065CB4h
022E201C EA000110 b       22E2464h
022E2020 E1D500B2 ldrh    r0,[r5,2h]
022E2024 EB000A20 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2028 E1A06000 mov     r6,r0
022E202C E1D500B4 ldrh    r0,[r5,4h]
022E2030 EB000A1D bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2034 E1A02806 mov     r2,r6,lsl 10h
022E2038 E1A05000 mov     r5,r0
022E203C E28D10C0 add     r1,r13,0C0h
022E2040 E1A00842 mov     r0,r2,asr 10h
022E2044 EBF60F2B bl      2065CF8h
022E2048 E1A01805 mov     r1,r5,lsl 10h
022E204C E1DD3CF0 ldrsh   r3,[r13,0C0h]
022E2050 E284006C add     r0,r4,6Ch
022E2054 E1A01841 mov     r1,r1,asr 10h
022E2058 E3A02000 mov     r2,0h
022E205C EBF5A649 bl      204B988h
022E2060 E1A01805 mov     r1,r5,lsl 10h
022E2064 E1DD3CB2 ldrh    r3,[r13,0C2h]
022E2068 E284006C add     r0,r4,6Ch
022E206C E1A01841 mov     r1,r1,asr 10h
022E2070 E3A02001 mov     r2,1h
022E2074 EBF5A643 bl      204B988h
022E2078 EA0000F9 b       22E2464h
///============================================================ 0x128
022E207C EBF4D68B bl      2017AB0h      ///EXEC Fun_02017AB0
022E2080 EA0000F7 b       22E2464h
022E2084 E1D500B2 ldrh    r0,[r5,2h]
022E2088 EB000A07 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E208C E3500000 cmp     r0,0h
022E2090 B3A0001E movlt   r0,1Eh
022E2094 E1A00800 mov     r0,r0,lsl 10h
022E2098 E1A00820 mov     r0,r0,lsr 10h
022E209C EBF4D68A bl      2017ACCh
022E20A0 EA0000EF b       22E2464h
022E20A4 E1D500B2 ldrh    r0,[r5,2h]
022E20A8 EB0009FF bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E20AC E1A00800 mov     r0,r0,lsl 10h
022E20B0 E59F11E4 ldr     r1,=3E7h
022E20B4 E1A00820 mov     r0,r0,lsr 10h
022E20B8 E1500001 cmp     r0,r1
022E20BC 0A000001 beq     22E20C8h
022E20C0 EBF4D6A4 bl      2017B58h
022E20C4 EA0000E6 b       22E2464h
022E20C8 EBF4D6A8 bl      2017B70h
022E20CC EA0000E4 b       22E2464h
022E20D0 E1D500B2 ldrh    r0,[r5,2h]
022E20D4 EB0009F4 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E20D8 E1A06000 mov     r6,r0
022E20DC E1D500B4 ldrh    r0,[r5,4h]
022E20E0 EB0009F1 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E20E4 E1A04000 mov     r4,r0
022E20E8 E1D500B6 ldrh    r0,[r5,6h]
022E20EC EB0009EE bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E20F0 E1A01806 mov     r1,r6,lsl 10h
022E20F4 E1A03000 mov     r3,r0
022E20F8 E59F219C ldr     r2,=3E7h
022E20FC E1A00821 mov     r0,r1,lsr 10h
022E2100 E1500002 cmp     r0,r2
022E2104 0A000005 beq     22E2120h
022E2108 E1A01804 mov     r1,r4,lsl 10h
022E210C E1A02803 mov     r2,r3,lsl 10h
022E2110 E1A01821 mov     r1,r1,lsr 10h
022E2114 E1A02822 mov     r2,r2,lsr 10h
022E2118 EBF4D691 bl      2017B64h
022E211C EA0000D0 b       22E2464h
022E2120 EBF4D692 bl      2017B70h
022E2124 EA0000CE b       22E2464h
022E2128 EBF4D690 bl      2017B70h
022E212C EA0000CC b       22E2464h
022E2130 E1D500B2 ldrh    r0,[r5,2h]
022E2134 EB0009DC bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2138 E3500000 cmp     r0,0h
022E213C B3A0001E movlt   r0,1Eh
022E2140 E1A00800 mov     r0,r0,lsl 10h
022E2144 E1A00820 mov     r0,r0,lsr 10h
022E2148 EBF4D68B bl      2017B7Ch
022E214C EA0000C4 b       22E2464h
022E2150 E1D500B2 ldrh    r0,[r5,2h]
022E2154 EB0009D4 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2158 E1A04000 mov     r4,r0
022E215C E1D500B4 ldrh    r0,[r5,4h]
022E2160 EB0009D1 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2164 E1A01804 mov     r1,r4,lsl 10h
022E2168 E1A02800 mov     r2,r0,lsl 10h
022E216C E1A00821 mov     r0,r1,lsr 10h
022E2170 E1A01822 mov     r1,r2,lsr 10h
022E2174 EBF4D683 bl      2017B88h
022E2178 EA0000B9 b       22E2464h
022E217C E1D500B2 ldrh    r0,[r5,2h]
022E2180 EB0009C9 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2184 E1A00800 mov     r0,r0,lsl 10h
022E2188 E59F110C ldr     r1,=3E7h
022E218C E1A00820 mov     r0,r0,lsr 10h
022E2190 E1500001 cmp     r0,r1
022E2194 0A000001 beq     22E21A0h
022E2198 EBF4D68D bl      2017BD4h
022E219C EA0000B0 b       22E2464h
022E21A0 EBF4D691 bl      2017BECh
022E21A4 EA0000AE b       22E2464h
022E21A8 E1D500B2 ldrh    r0,[r5,2h]
022E21AC EB0009BE bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E21B0 E1A06000 mov     r6,r0
022E21B4 E1D500B4 ldrh    r0,[r5,4h]
022E21B8 EB0009BB bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E21BC E1A04000 mov     r4,r0
022E21C0 E1D500B6 ldrh    r0,[r5,6h]
022E21C4 EB0009B8 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E21C8 E1A01806 mov     r1,r6,lsl 10h
022E21CC E1A03000 mov     r3,r0
022E21D0 E59F20C4 ldr     r2,=3E7h
022E21D4 E1A00821 mov     r0,r1,lsr 10h
022E21D8 E1500002 cmp     r0,r2
022E21DC 0A000005 beq     22E21F8h
022E21E0 E1A01804 mov     r1,r4,lsl 10h
022E21E4 E1A02803 mov     r2,r3,lsl 10h
022E21E8 E1A01821 mov     r1,r1,lsr 10h
022E21EC E1A02822 mov     r2,r2,lsr 10h
022E21F0 EBF4D67A bl      2017BE0h
022E21F4 EA00009A b       22E2464h
022E21F8 EBF4D67B bl      2017BECh
022E21FC EA000098 b       22E2464h
022E2200 EBF4D679 bl      2017BECh
022E2204 EA000096 b       22E2464h
022E2208 E1D500B2 ldrh    r0,[r5,2h]
022E220C EB0009A6 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2210 E3500000 cmp     r0,0h
022E2214 B3A0001E movlt   r0,1Eh
022E2218 E1A00800 mov     r0,r0,lsl 10h
022E221C E1A00820 mov     r0,r0,lsr 10h
022E2220 EBF4D674 bl      2017BF8h
022E2224 EA00008E b       22E2464h
022E2228 E1D500B2 ldrh    r0,[r5,2h]
022E222C EB00099E bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2230 E1A04000 mov     r4,r0
022E2234 E1D500B4 ldrh    r0,[r5,4h]
022E2238 EB00099B bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E223C E1A01804 mov     r1,r4,lsl 10h
022E2240 E1A02800 mov     r2,r0,lsl 10h
022E2244 E1A00821 mov     r0,r1,lsr 10h
022E2248 E1A01822 mov     r1,r2,lsr 10h
022E224C EBF4D66C bl      2017C04h
022E2250 EA000083 b       22E2464h
022E2254 E1D500B2 ldrh    r0,[r5,2h]
022E2258 EB000993 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E225C E1A00800 mov     r0,r0,lsl 10h
022E2260 E1A00820 mov     r0,r0,lsr 10h
022E2264 EBF4D679 bl      2017C50h
022E2268 EA00007D b       22E2464h
022E226C EBF4D67A bl      2017C5Ch
022E2270 EA00007B b       22E2464h
022E2274 02316A60 eoreqs  r6,r1,60000h
022E2278 02316AD8 eoreqs  r6,r1,0D8000h
022E227C 00000127 andeq   r0,r0,r7,lsr 2h
022E2280 00000179 andeq   r0,r0,r9,ror r1
022E2284 0231958C eoreqs  r9,r1,23000000h
022E2288 023195AC eoreqs  r9,r1,2B000000h
022E228C 00000296 muleq   r0,r6,r2
022E2290 00000297 muleq   r0,r7,r2
022E2294 00000299 muleq   r0,r9,r2
022E2298 00000295 muleq   r0,r5,r2
022E229C 000003E7 andeq   r0,r0,r7,ror 7h
022E22A0 E1D500B2 ldrh    r0,[r5,2h]
022E22A4 EB000980 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E22A8 E1A00800 mov     r0,r0,lsl 10h
022E22AC E1A00820 mov     r0,r0,lsr 10h
022E22B0 EBF4D672 bl      2017C80h
022E22B4 EA00006A b       22E2464h
022E22B8 E1D500B2 ldrh    r0,[r5,2h]
022E22BC EB00097A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E22C0 E1A04000 mov     r4,r0
022E22C4 E1D500B4 ldrh    r0,[r5,4h]
022E22C8 EB000977 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E22CC E1A01804 mov     r1,r4,lsl 10h
022E22D0 E1A02800 mov     r2,r0,lsl 10h
022E22D4 E1A00821 mov     r0,r1,lsr 10h
022E22D8 E1A01822 mov     r1,r2,lsr 10h
022E22DC EBF4D664 bl      2017C74h
022E22E0 EA00005F b       22E2464h
022E22E4 E1D500B2 ldrh    r0,[r5,2h]
022E22E8 EB00096F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E22EC E1A04000 mov     r4,r0
022E22F0 E1D500B4 ldrh    r0,[r5,4h]
022E22F4 EB00096C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E22F8 E1A02800 mov     r2,r0,lsl 10h
022E22FC E1A01804 mov     r1,r4,lsl 10h
022E2300 E1A00821 mov     r0,r1,lsr 10h
022E2304 E1A03842 mov     r3,r2,asr 10h
022E2308 E3A01000 mov     r1,0h
022E230C E3A02C01 mov     r2,100h
022E2310 EBF4D65E bl      2017C90h
022E2314 EA000052 b       22E2464h
022E2318 E1D500B2 ldrh    r0,[r5,2h]
022E231C EB000962 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2320 E1A06000 mov     r6,r0
022E2324 E1D500B4 ldrh    r0,[r5,4h]
022E2328 EB00095F bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E232C E1A04000 mov     r4,r0
022E2330 E1D500B6 ldrh    r0,[r5,6h]
022E2334 EB00095C bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2338 E1A01806 mov     r1,r6,lsl 10h
022E233C E1A02804 mov     r2,r4,lsl 10h
022E2340 E1A02822 mov     r2,r2,lsr 10h
022E2344 E1A03000 mov     r3,r0
022E2348 E1A00821 mov     r0,r1,lsr 10h
022E234C E1A01803 mov     r1,r3,lsl 10h
022E2350 E1A03841 mov     r3,r1,asr 10h
022E2354 E3A01000 mov     r1,0h
022E2358 EBF4D64C bl      2017C90h
022E235C EA000040 b       22E2464h
022E2360 E1D500B2 ldrh    r0,[r5,2h]
022E2364 EB000950 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2368 E1A06000 mov     r6,r0
022E236C E1D500B4 ldrh    r0,[r5,4h]
022E2370 EB00094D bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2374 E1A04000 mov     r4,r0
022E2378 E1D500B6 ldrh    r0,[r5,6h]
022E237C EB00094A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2380 E1A01806 mov     r1,r6,lsl 10h
022E2384 E1A03800 mov     r3,r0,lsl 10h
022E2388 E1A02804 mov     r2,r4,lsl 10h
022E238C E1A00821 mov     r0,r1,lsr 10h
022E2390 E1A01822 mov     r1,r2,lsr 10h
022E2394 E1A02823 mov     r2,r3,lsr 10h
022E2398 EBF4D63F bl      2017C9Ch
022E239C EA000030 b       22E2464h
022E23A0 E1D500B2 ldrh    r0,[r5,2h]
022E23A4 EB000940 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E23A8 E1A06000 mov     r6,r0
022E23AC E1D500B4 ldrh    r0,[r5,4h]
022E23B0 EB00093D bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E23B4 E1A04000 mov     r4,r0
022E23B8 E1D500B6 ldrh    r0,[r5,6h]
022E23BC EB00093A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E23C0 E1A01806 mov     r1,r6,lsl 10h
022E23C4 E1A03800 mov     r3,r0,lsl 10h
022E23C8 E1A02804 mov     r2,r4,lsl 10h
022E23CC E1A00821 mov     r0,r1,lsr 10h
022E23D0 E1A01822 mov     r1,r2,lsr 10h
022E23D4 E1A02843 mov     r2,r3,asr 10h
022E23D8 EBF4D632 bl      2017CA8h
022E23DC EA000020 b       22E2464h
022E23E0 E1D500B2 ldrh    r0,[r5,2h]
022E23E4 EB000930 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E23E8 E1A00800 mov     r0,r0,lsl 10h
022E23EC E1A00820 mov     r0,r0,lsr 10h
022E23F0 EBF4D62F bl      2017CB4h
022E23F4 EA00001A b       22E2464h
022E23F8 E1D500B2 ldrh    r0,[r5,2h]
022E23FC EB00092A bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2400 E1A04000 mov     r4,r0
022E2404 E1D500B4 ldrh    r0,[r5,4h]
022E2408 EB000927 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E240C E3500000 cmp     r0,0h
022E2410 B3A0001E movlt   r0,1Eh
022E2414 E1A01804 mov     r1,r4,lsl 10h
022E2418 E1A02800 mov     r2,r0,lsl 10h
022E241C E1A00821 mov     r0,r1,lsr 10h
022E2420 E1A01822 mov     r1,r2,lsr 10h
022E2424 EBF4D625 bl      2017CC0h
022E2428 EA00000D b       22E2464h
022E242C E3A00000 mov     r0,0h
022E2430 EB001CB6 bl      22E9710h
022E2434 E3A00000 mov     r0,0h
022E2438 EB001CAD bl      22E96F4h
022E243C EA000008 b       22E2464h
022E2440 E1D500B2 ldrh    r0,[r5,2h]
022E2444 EB000918 bl      22E48ACh                  //Turn 16bits signed into 14bits unsigned
022E2448 E1A00800 mov     r0,r0,lsl 10h
022E244C E1A00840 mov     r0,r0,asr 10h
022E2450 EBF62725 bl      206C0ECh
022E2454 E3A00001 mov     r0,1h
022E2458 EB001CAC bl      22E9710h
022E245C E3A00001 mov     r0,1h
022E2460 EB001CA3 bl      22E96F4h
//LBL022E2464
022E2464 E3A00002 mov     r0,2h
022E2468 EA000002 b       22E2478h        //Return 022E2478              //LBL022E2478
//
022E246C E3A00002 mov     r0,2h
022E2470 EA000000 b       22E2478h        //Return 022E2478
///022E2474 - Returns 2
022E2474 E3A00002 mov     r0,2h
//LBL022E2478
022E2478 E28DDFB2 add     r13,r13,2C8h
022E247C E8BD8FF8 pop     r3-r11,r15




//
//  Other Functions
//

//Ex: 0x7FE8
// 22E48ACh 022E48AC_S16ToU14 ( r0 ) return r0  (Turns a 16bits signed value into an unsigned 14 bits integer)
022E48AC E3100901 tst     r0,4000h      //(R0 & 0x4000)
if( R0 & 0x4000 != 0 )                                 //If the bit at 0x4000 is on
    022E48B0 13A01902 movne   r1,8000h  //R1 = 0x8000    
    022E48B4 12611000 rsbne   r1,r1,0h  //R1 = 0 - 0x8000 = 0xFFFF8000
    022E48B8 11801001 orrne   r1,r0,r1  //R1 = R0 | 0xFFFF8000              ( Ex: 0x7FE8 gives 0xFFFF FFE8 )
else
    022E48BC 059F1018 ldreq   r1,=3FFFh //R1 = 0x3FFF
    022E48C0 00001001 andeq   r1,r0,r1  //R1 = R0 & 0x3FFF
022E48C4 E3100902 tst     r0,8000h
if( R0 & 0x8000 != 0 )                                 //If the bit at 0x8000 is on
    022E48C8 11A003C1 movne   r0,r1,asr 7h      //R0 = R1 >> 7 (signed shift)         ( Ex: FFFF FFE8 >> 7 = FFFF FFFF )
    022E48CC 10810C20 addne   r0,r1,r0,lsr 18h  //R0 = R1 + R0 >> 0x18                ( Ex: FFFF FFE8 + FFFF FFFF >> 18 == FFFF FFE8 + FF == E7 )
    022E48D0 11A00440 movne   r0,r0,asr 8h      //R0 = R0 >> 8 (signed shift)         ( Ex: E7 >> 8 == 0 )
else
    022E48D4 01A00001 moveq   r0,r1     //R0 = R1
022E48D8 E12FFF1E bx      r14


// Fun_02017AB0
02017AB0 E92D4008 push    r3,r14
02017AB4 EB00010B bl      2017EE8h
02017AB8 EB000196 bl      2018118h
02017ABC EB0001ED bl      2018278h
02017AC0 E3A00C3F mov     r0,3F00h
02017AC4 EB000257 bl      2018428h
02017AC8 E8BD8008 pop     r3,r15


// Fun_02017EE8
02017EE8 E92D4008 push    r3,r14
02017EEC EB000317 bl      2018B50h
02017EF0 E59F0028 ldr     r0,=3E7h
02017EF4 E59F1028 ldr     r1,=22A4BECh
02017EF8 E3A02000 mov     r2,0h
02017EFC E1C100B0 strh    r0,[r1]
02017F00 E1C100B2 strh    r0,[r1,2h]
02017F04 E3A00005 mov     r0,5h
02017F08 E1C120B4 strh    r2,[r1,4h]
02017F0C EB000316 bl      2018B6Ch
02017F10 E3A01000 mov     r1,0h
02017F14 E1C010B8 strh    r1,[r0,8h]
02017F18 EB000322 bl      2018BA8h
02017F1C E8BD8008 pop     r3,r15


//0230F004 ( R0 )
0230F004 E59F1040 ldr     r1,=2324DE4h
0230F008 E5810000 str     r0,[r1]
0230F00C E5913008 ldr     r3,[r1,8h]
0230F010 E3530000 cmp     r3,0h
0230F014 1593202C ldrne   r2,[r3,2Ch]
0230F018 11520000 cmpne   r2,r0
0230F01C 012FFF1E bxeq    r14
0230F020 E1D320D4 ldrsb   r2,[r3,4h]
0230F024 E2830A0F add     r0,r3,0F000h
0230F028 E3A03000 mov     r3,0h
0230F02C E5C02999 strb    r2,[r0,999h]
0230F030 E5910008 ldr     r0,[r1,8h]
0230F034 E3A02001 mov     r2,1h
0230F038 E2800A0F add     r0,r0,0F000h
0230F03C E5C0399A strb    r3,[r0,99Ah]
0230F040 E5910008 ldr     r0,[r1,8h]
0230F044 E5C0200C strb    r2,[r0,0Ch]
0230F048 E12FFF1E bx      r14
0230F04C 02324DE4 


//0204B4EC 204B4ECh GetScriptVariableValue(R0 = (ScriptEngineStruct+108), R1 = CommandParameter14b)
0204B4EC E92D4010 push    r4,r14
0204B4F0 E24DD008 sub     r13,r13,8h
0204B4F4 E1A02000 mov     r2,r0                     //R2 = (ScriptEngineStruct+108)
0204B4F8 E1A04001 mov     r4,r1                     //R4 = CommandParameter14b
0204B4FC E1A01002 mov     r1,r2                     //R1 = (ScriptEngineStruct+108)
0204B500 E28D0000 add     r0,r13,0h                 //R0 = Stack
0204B504 E1A02004 mov     r2,r4                     //R2 = CommandParameter14b
0204B508 EBFFFFE3 bl      204B49Ch                  //0204B49C (StackPtr, (ScriptEngineStruct+108), CommandParameter14b )
0204B50C E59D1000 ldr     r1,[r13]                  //R1 = AddressScriptGlobalInfo
0204B510 E1D100F0 ldrsh   r0,[r1]                   //R0 = VariableType
0204B514 E3500009 cmp     r0,9h             
if( VariableType <= 9 )
    0204B518 908FF100 addls   r15,r15,r0,lsl 2h     //R15 = 0204B51C + (R0 * 4) + 4
0204B51C EA000052 b       204B66Ch                  //      0204B66C
0204B520 EA000051 b       204B66Ch                  //0     0204B66C
0204B524 EA000007 b       204B548h                  //1     0204B548 
0204B528 EA00000F b       204B56Ch                  //2     0204B56C
0204B52C EA00000E b       204B56Ch                  //3     0204B56C
0204B530 EA000010 b       204B578h                  //4     0204B578
0204B534 EA000012 b       204B584h                  //5     0204B584
0204B538 EA000014 b       204B590h                  //6     0204B590
0204B53C EA000016 b       204B59Ch                  //7     0204B59C
0204B540 EA000015 b       204B59Ch                  //8     0204B59C
0204B544 EA000017 b       204B5A8h                  //9     0204B5A8
//Case 1 Single Bit
0204B548 E1D110F6 ldrsh   r1,[r1,6h]                //R1 = ScriptGlobalInfoUnk2
0204B54C E59D2004 ldr     r2,[r13,4h]               //AddressScriptGlobalValue
0204B550 E3A00001 mov     r0,1h                     //R0 = 1
0204B554 E1A01110 mov     r1,r0,lsl r1              //R1 = 1 << ScriptGlobalInfoUnk2
0204B558 E5D22000 ldrb    r2,[r2]                   //R2 = ScriptGlobalValue
0204B55C E20110FF and     r1,r1,0FFh                //R1 = R1 & 0xFF
0204B560 E1120001 tst     r2,r1                     //R2 & R1
if( R2 & R1 == 0 )
    0204B564 03A00000 moveq   r0,0h                 //R0 = 0
0204B568 EA000040 b       204B670h                  ///LBL_Return
//Case 2 and 3 ( ? and uint8 )
0204B56C E59D0004 ldr     r0,[r13,4h]               //AddressScriptGlobalValue
0204B570 E5D00000 ldrb    r0,[r0]                   //R0 = ScriptGlobalValue
0204B574 EA00003D b       204B670h                  ///LBL_Return
//Case 4 ( int8 )
0204B578 E59D0004 ldr     r0,[r13,4h]               //AddressScriptGlobalValue
0204B57C E1D000D0 ldrsb   r0,[r0]                   //R0 = ScriptGlobalValue
0204B580 EA00003A b       204B670h                  ///LBL_Return
//Case 5 ( uint16 )
0204B584 E59D0004 ldr     r0,[r13,4h]               //AddressScriptGlobalValue
0204B588 E1D000B0 ldrh    r0,[r0]                   //R0 = ScriptGlobalValue
0204B58C EA000037 b       204B670h                  ///LBL_Return
//Case 6 ( int16 )
0204B590 E59D0004 ldr     r0,[r13,4h]               //AddressScriptGlobalValue
0204B594 E1D000F0 ldrsh   r0,[r0]                   //R0 = ScriptGlobalValue
0204B598 EA000034 b       204B670h                  ///LBL_Return
//Case 7 and 8 ( uint32 and int32 )
0204B59C E59D0004 ldr     r0,[r13,4h]               //AddressScriptGlobalValue
0204B5A0 E5900000 ldr     r0,[r0]                   //R0 = ScriptGlobalValue
0204B5A4 EA000031 b       204B670h                  ///LBL_Return
//Case 9
0204B5A8 E3540048 cmp     r4,48h
if( CommandParameter14b > 0x48 )
    0204B5AC CA00000C bgt     204B5E4h              ///0204B5E4
else if( CommandParameter14b >= 0x48 ) 
    0204B5B0 AA00001D bge     204B62Ch              ///0204B62C
0204B5B4 E354003D cmp     r4,3Dh
if( CommandParameter14b > 0x3D )
    0204B5B8 CA000006 bgt     204B5D8h              ///0204B5D8
0204B5BC E254003A subs    r0,r4,3Ah                 //R0 = CommandParameter14b - 0x3A
0204B5C0 508FF100 addpl   r15,r15,r0,lsl 2h
0204B5C4 EA000028 b       204B66Ch
0204B5C8 EA00000C b       204B600h
0204B5CC EA00000D b       204B608h
0204B5D0 EA00000F b       204B614h
0204B5D4 EA000010 b       204B61Ch
///0204B5D8
0204B5D8 E3540047 cmp     r4,47h
0204B5DC 0A000010 beq     204B624h
0204B5E0 EA000021 b       204B66Ch
///0204B5E4
0204B5E4 E3540049 cmp     r4,49h
0204B5E8 CA000001 bgt     204B5F4h
0204B5EC 0A000010 beq     204B634h
0204B5F0 EA00001D b       204B66Ch
0204B5F4 E3540070 cmp     r4,70h
0204B5F8 0A000019 beq     204B664h
0204B5FC EA00001A b       204B66Ch
0204B600 E3A00001 mov     r0,1h
0204B604 EA000019 b       204B670h              ///LBL_Return
0204B608 E3A00000 mov     r0,0h
0204B60C EB002D83 bl      2056C20h
0204B610 EA000016 b       204B670h              ///LBL_Return
0204B614 EBFF0DB8 bl      200ECFCh
0204B618 EA000014 b       204B670h              ///LBL_Return
0204B61C EBFF143A bl      201070Ch
0204B620 EA000012 b       204B670h              ///LBL_Return
0204B624 EBFF53DD bl      20205A0h
0204B628 EA000010 b       204B670h              ///LBL_Return
///0204B62C
0204B62C EBFFFE63 bl      204AFC0h
0204B630 EA00000E b       204B670h              ///LBL_Return
0204B634 EBFFFE61 bl      204AFC0h
0204B638 E3500001 cmp     r0,1h
0204B63C 0A000002 beq     204B64Ch
0204B640 E3500003 cmp     r0,3h
0204B644 0A000002 beq     204B654h
0204B648 EA000003 b       204B65Ch
0204B64C EB0004B1 bl      204C918h
0204B650 EA000006 b       204B670h              ///LBL_Return
0204B654 EB0004A4 bl      204C8ECh
0204B658 EA000004 b       204B670h              ///LBL_Return
0204B65C E3E00000 mvn     r0,0h
0204B660 EA000002 b       204B670h              ///LBL_Return
0204B664 EBFFF38D bl      20484A0h
0204B668 EA000000 b       204B670h              ///LBL_Return
//Case 0 and invalid
0204B66C E3A00000 mov     r0,0h
///LBL_Return
0204B670 E28DD008 add     r13,r13,8h
0204B674 E8BD8010 pop     r4,r15



/*
    fun_0204B49C GetScriptVariableInfoAndPtr
    Returns 2 pointers on stack, first is AddressScriptGlobalInfo, the other is AddressScriptGlobalValue
*/
//R0 = DestinationStringPointer, R1 =(ScriptEngineStruct+108), R2 = Parameter14b
0204B49C E3520B01 cmp     r2,400h           //
if( Parameter14b >= 0x400  )
    //0204B4A0 AA000006 bge     204B4C0h      ///0204B4C0 LBL1
    0204B4C0 E59F3020 ldr     r3,=209CECCh      //Seems to be a table of possible in-game variables?
    0204B4C4 E2422B01 sub     r2,r2,400h
    0204B4C8 E0832202 add     r2,r3,r2,lsl 4h
    0204B4CC E5802000 str     r2,[r0]
    0204B4D0 E1D220F4 ldrsh   r2,[r2,4h]
    0204B4D4 E0811102 add     r1,r1,r2,lsl 2h
else
    0204B4A4 E59F1034 ldr     r1,=209D870h      //Seems to be a table of possible in-game variables info?
    0204B4A8 E59F3034 ldr     r3,=22AB0ACh      //Seems to be where some of the game runtime variables are stored!
    0204B4AC E0811202 add     r1,r1,r2,lsl 4h   //R1 = 0x209D870 + (Parameter14b * 16)
    0204B4B0 E5801000 str     r1,[r0]           //Put Address to variable info address on the stack
    0204B4B4 E1D110F4 ldrsh   r1,[r1,4h]        //R1 = OffsetVariableValue? (Gets the value 4 bytes in the variable info table.)
    0204B4B8 E0831001 add     r1,r3,r1          //R1 = 22AB0ACh + OffsetVariableValue
    //0204B4BC EA000005 b       204B4D8h          //0204B4D8
///0204B4C0 LBL1
//0204B4C0 E59F3020 ldr     r3,=209CECCh
//0204B4C4 E2422B01 sub     r2,r2,400h
//0204B4C8 E0832202 add     r2,r3,r2,lsl 4h
//0204B4CC E5802000 str     r2,[r0]
//0204B4D0 E1D220F4 ldrsh   r2,[r2,4h]
//0204B4D4 E0811102 add     r1,r1,r2,lsl 2h
0204B4D8 E5801004 str     r1,[r0,4h]            //PlaceAddress to ScriptGlobal at stack+4
0204B4DC E12FFF1E bx      r14
0204B4E0 0209D870
0204B4E4 022AB0AC
0204B4E8 0209CECC


//022E7E84 22E7E84h
022E7E84 E92D4010 push    r4,r14
022E7E88 E1A04000 mov     r4,r0
022E7E8C E59F1090 ldr     r1,=2319A98h          //"script ground sub call %3d"
022E7E90 E1A02004 mov     r2,r4
022E7E94 E3A00002 mov     r0,2h
022E7E98 EBF490E8 bl      200C240h              //Debug Print
022E7E9C E2840001 add     r0,r4,1h
022E7EA0 E3500009 cmp     r0,9h
022E7EA4 908FF100 addls   r15,r15,r0,lsl 2h
022E7EA8 E8BD8010 pop     r4,r15
022E7EAC EA000008 b       22E7ED4h
022E7EB0 EA000009 b       22E7EDCh
022E7EB4 EA00000A b       22E7EE4h
022E7EB8 EA00000B b       22E7EECh
022E7EBC EA00000C b       22E7EF4h
022E7EC0 EA00000D b       22E7EFCh
022E7EC4 EA00000E b       22E7F04h
022E7EC8 EA00000F b       22E7F0Ch
022E7ECC EA000010 b       22E7F14h
022E7ED0 EA000011 b       22E7F1Ch
022E7ED4 EB009449 bl      230D000h
022E7ED8 E8BD8010 pop     r4,r15
022E7EDC EB009444 bl      230CFF4h
022E7EE0 E8BD8010 pop     r4,r15
022E7EE4 EB009690 bl      230D92Ch
022E7EE8 E8BD8010 pop     r4,r15
022E7EEC EB00AE85 bl      2313908h
022E7EF0 E8BD8010 pop     r4,r15
022E7EF4 EB009A9A bl      230E964h
022E7EF8 E8BD8010 pop     r4,r15
022E7EFC EB00A34F bl      2310C40h
022E7F00 E8BD8010 pop     r4,r15
022E7F04 EB00A518 bl      231136Ch
022E7F08 E8BD8010 pop     r4,r15
022E7F0C EB00A656 bl      231186Ch
022E7F10 E8BD8010 pop     r4,r15
022E7F14 EB00A794 bl      2311D6Ch
022E7F18 E8BD8010 pop     r4,r15
022E7F1C EB00A8CB bl      2312250h
022E7F20 E8BD8010 pop     r4,r15
022E7F24 02319A98 eoreqs  r9,r1,98000h


/*
    ScriptLocateSet
    R0 = ScriptDTypeType
    R1 = PtrParam
    R2 = 0
*/
022E4CD4 E92D4FF0 push    r4-r11,r14
022E4CD8 E24DD044 sub     r13,r13,44h
022E4CDC E1A09001 mov     r9,r1                 //R9  = PtrParam
022E4CE0 E1A0A000 mov     r10,r0                //R10 = ScriptDTypeType
022E4CE4 E1A08002 mov     r8,r2                 //R8 = 
022E4CE8 E59F12D4 ldr     r1,=2319714h          //"script locate set %3d %3d"
022E4CEC E1A0200A mov     r2,r10
022E4CF0 E1A03009 mov     r3,r9
022E4CF4 E3A00002 mov     r0,2h
022E4CF8 EBF49D50 bl      200C240h              //DebugPrint
022E4CFC E35A0002 cmp     r10,2h
if( ScriptDTypeType == 2 )
    022E4D00 059F02C0 ldreq   r0,=2324C6Ch      //Pointer to current SSA file buffer.
    022E4D04 05904020 ldreq   r4,[r0,20h]       //SSS data pointer
    022E4D08 0A000004 beq     22E4D20h
022E4D0C E35A0003 cmp     r10,3h
if( ScriptDTypeType == 3 )
    022E4D10 059F02B0 ldreq   r0,=2324C6Ch      //Pointer to current SSA file buffer.
    022E4D14 05904000 ldreq   r4,[r0]           //SSA data pointer
else
    022E4D18 159F02A8 ldrne   r0,=2324C6Ch      //Pointer to current SSA file buffer.
    022E4D1C 15904028 ldrne   r4,[r0,28h]       //SSE data pointer
022E4D20 E1D410B2 ldrh    r1,[r4,2h]            //R1 = PtrLayers
022E4D24 E3A0000A mov     r0,0Ah                //R0 = 0xA
022E4D28 E3E0B000 mvn     r11,0h                //R11 = 0xFFFFFFFF
022E4D2C E0201099 mla     r0,r9,r0,r1           //R0 = (PtrParam * 0xA) + PtrLayers
022E4D30 E0845080 add     r5,r4,r0,lsl 1h       //R5 = PtrSdataBuffer + (((PtrParam * 0xA) + PtrLayers) * 2)
022E4D34 E1A00080 mov     r0,r0,lsl 1h          //R0 = ((PtrParam * 0xA) + PtrLayers) * 2
022E4D38 E19400B0 ldrh    r0,[r4,r0]            //R0 = [PtrSdataBuffer + (((PtrParam * 0xA) + PtrLayers) * 2)]
022E4D3C E1D510B2 ldrh    r1,[r5,2h]
022E4D40 E3A07000 mov     r7,0h
022E4D44 E58D0008 str     r0,[r13,8h]
022E4D48 E28B0801 add     r0,r11,10000h
022E4D4C E0846081 add     r6,r4,r1,lsl 1h
022E4D50 E58D0010 str     r0,[r13,10h]
022E4D54 EA000019 b       22E4DC0h
022E4D58 E1D600B0 ldrh    r0,[r6]
022E4D5C E1A0200A mov     r2,r10
022E4D60 E1A03009 mov     r3,r9
022E4D64 E1CD03B8 strh    r0,[r13,38h]
022E4D68 E1D600B2 ldrh    r0,[r6,2h]
022E4D6C E2400001 sub     r0,r0,1h
022E4D70 E5CD003A strb    r0,[r13,3Ah]
022E4D74 E1D600B4 ldrh    r0,[r6,4h]
022E4D78 E5CD003B strb    r0,[r13,3Bh]
022E4D7C E1D600B6 ldrh    r0,[r6,6h]
022E4D80 E5CD003C strb    r0,[r13,3Ch]
022E4D84 E1D600B8 ldrh    r0,[r6,8h]
022E4D88 E5CD003D strb    r0,[r13,3Dh]
022E4D8C E1D600BA ldrh    r0,[r6,0Ah]
022E4D90 E5CD003E strb    r0,[r13,3Eh]
022E4D94 E1D610BC ldrh    r1,[r6,0Ch]
022E4D98 E59D0010 ldr     r0,[r13,10h]
022E4D9C E1510000 cmp     r1,r0
022E4DA0 01CDB4B0 streqh  r11,[r13,40h]
022E4DA4 11CD14B0 strneh  r1,[r13,40h]
022E4DA8 E1A0000B mov     r0,r11
022E4DAC E28D1038 add     r1,r13,38h
022E4DB0 E58D8000 str     r8,[r13]
022E4DB4 EB004C2F bl      22F7E78h
022E4DB8 E2877001 add     r7,r7,1h
022E4DBC E2866010 add     r6,r6,10h
022E4DC0 E59D0008 ldr     r0,[r13,8h]
022E4DC4 E1570000 cmp     r7,r0
022E4DC8 BAFFFFE2 blt     22E4D58h
022E4DCC E1D500B4 ldrh    r0,[r5,4h]
022E4DD0 E1D510B6 ldrh    r1,[r5,6h]
022E4DD4 E3E0B000 mvn     r11,0h
022E4DD8 E58D0004 str     r0,[r13,4h]
022E4DDC E28B0801 add     r0,r11,10000h
022E4DE0 E3A07000 mov     r7,0h
022E4DE4 E0846081 add     r6,r4,r1,lsl 1h
022E4DE8 E58D0014 str     r0,[r13,14h]
022E4DEC EA00001D b       22E4E68h
022E4DF0 E1D600B0 ldrh    r0,[r6]
022E4DF4 E1A0200A mov     r2,r10
022E4DF8 E1A03009 mov     r3,r9
022E4DFC E1CD02BC strh    r0,[r13,2Ch]
022E4E00 E1D600B2 ldrh    r0,[r6,2h]
022E4E04 E2400001 sub     r0,r0,1h
022E4E08 E5CD002E strb    r0,[r13,2Eh]
022E4E0C E1D600B4 ldrh    r0,[r6,4h]
022E4E10 E5CD002F strb    r0,[r13,2Fh]
022E4E14 E1D600B6 ldrh    r0,[r6,6h]
022E4E18 E5CD0030 strb    r0,[r13,30h]
022E4E1C E1D600B8 ldrh    r0,[r6,8h]
022E4E20 E5CD0031 strb    r0,[r13,31h]
022E4E24 E1D600BA ldrh    r0,[r6,0Ah]
022E4E28 E5CD0032 strb    r0,[r13,32h]
022E4E2C E1D600BC ldrh    r0,[r6,0Ch]
022E4E30 E5CD0033 strb    r0,[r13,33h]
022E4E34 E1D600BE ldrh    r0,[r6,0Eh]
022E4E38 E5CD0034 strb    r0,[r13,34h]
022E4E3C E1D611B0 ldrh    r1,[r6,10h]
022E4E40 E59D0014 ldr     r0,[r13,14h]
022E4E44 E1510000 cmp     r1,r0
022E4E48 01CDB3B6 streqh  r11,[r13,36h]
022E4E4C 11CD13B6 strneh  r1,[r13,36h]
022E4E50 E1A0000B mov     r0,r11
022E4E54 E28D102C add     r1,r13,2Ch
022E4E58 E58D8000 str     r8,[r13]
022E4E5C EB005C18 bl      22FBEC4h
022E4E60 E2877001 add     r7,r7,1h
022E4E64 E2866014 add     r6,r6,14h
022E4E68 E59D0004 ldr     r0,[r13,4h]
022E4E6C E1570000 cmp     r7,r0
022E4E70 BAFFFFDE blt     22E4DF0h
022E4E74 E1D500BA ldrh    r0,[r5,0Ah]
022E4E78 E1D5B0B8 ldrh    r11,[r5,8h]
022E4E7C E3A07000 mov     r7,0h
022E4E80 E0846080 add     r6,r4,r0,lsl 1h
022E4E84 E3E00000 mvn     r0,0h
022E4E88 E58D000C str     r0,[r13,0Ch]
022E4E8C EA000018 b       22E4EF4h
022E4E90 E1D620B0 ldrh    r2,[r6]
022E4E94 E59D000C ldr     r0,[r13,0Ch]
022E4E98 E28D1022 add     r1,r13,22h
022E4E9C E1CD22B2 strh    r2,[r13,22h]
022E4EA0 E1D6C0B2 ldrh    r12,[r6,2h]
022E4EA4 E1A0200A mov     r2,r10
022E4EA8 E1A03009 mov     r3,r9
022E4EAC E24CC001 sub     r12,r12,1h
022E4EB0 E5CDC024 strb    r12,[r13,24h]
022E4EB4 E1D6C0B4 ldrh    r12,[r6,4h]
022E4EB8 E5CDC025 strb    r12,[r13,25h]
022E4EBC E1D6C0B6 ldrh    r12,[r6,6h]
022E4EC0 E5CDC026 strb    r12,[r13,26h]
022E4EC4 E1D6C0B8 ldrh    r12,[r6,8h]
022E4EC8 E5CDC027 strb    r12,[r13,27h]
022E4ECC E1D6C0BA ldrh    r12,[r6,0Ah]
022E4ED0 E5CDC028 strb    r12,[r13,28h]
022E4ED4 E1D6C0BC ldrh    r12,[r6,0Ch]
022E4ED8 E5CDC029 strb    r12,[r13,29h]
022E4EDC E1D6C0BE ldrh    r12,[r6,0Eh]
022E4EE0 E5CDC02A strb    r12,[r13,2Ah]
022E4EE4 E58D8000 str     r8,[r13]
022E4EE8 EB00620F bl      22FD72Ch
022E4EEC E2877001 add     r7,r7,1h
022E4EF0 E2866014 add     r6,r6,14h
022E4EF4 E157000B cmp     r7,r11
022E4EF8 BAFFFFE4 blt     22E4E90h
022E4EFC E24A0001 sub     r0,r10,1h
022E4F00 E1A00800 mov     r0,r0,lsl 10h
022E4F04 E1A00840 mov     r0,r0,asr 10h
022E4F08 E1A00800 mov     r0,r0,lsl 10h
022E4F0C E1A00820 mov     r0,r0,lsr 10h
022E4F10 E3500001 cmp     r0,1h
022E4F14 8A000027 bhi     22E4FB8h
022E4F18 E1D500BE ldrh    r0,[r5,0Eh]
022E4F1C E1D570BC ldrh    r7,[r5,0Ch]
022E4F20 E3E05000 mvn     r5,0h
022E4F24 E3A08000 mov     r8,0h
022E4F28 E0846080 add     r6,r4,r0,lsl 1h
022E4F2C E285B902 add     r11,r5,8000h
022E4F30 EA00001E b       22E4FB0h
022E4F34 E1D610BC ldrh    r1,[r6,0Ch]
022E4F38 E3110902 tst     r1,8000h
022E4F3C 1001000B andne   r0,r1,r11
022E4F40 11CD01B8 strneh  r0,[r13,18h]
022E4F44 11CD52B0 strneh  r5,[r13,20h]
022E4F48 1A000005 bne     22E4F64h
022E4F4C E1A00081 mov     r0,r1,lsl 1h
022E4F50 E19400F0 ldrsh   r0,[r4,r0]
022E4F54 E0841081 add     r1,r4,r1,lsl 1h
022E4F58 E1CD01B8 strh    r0,[r13,18h]
022E4F5C E1D100F6 ldrsh   r0,[r1,6h]
022E4F60 E1CD02B0 strh    r0,[r13,20h]
022E4F64 E1D620B0 ldrh    r2,[r6]
022E4F68 E1A00005 mov     r0,r5
022E4F6C E28D1018 add     r1,r13,18h
022E4F70 E5CD201A strb    r2,[r13,1Ah]
022E4F74 E1D6C0B2 ldrh    r12,[r6,2h]
022E4F78 E1A0200A mov     r2,r10
022E4F7C E1A03009 mov     r3,r9
022E4F80 E5CDC01B strb    r12,[r13,1Bh]
022E4F84 E1D6C0B4 ldrh    r12,[r6,4h]
022E4F88 E5CDC01C strb    r12,[r13,1Ch]
022E4F8C E1D6C0B6 ldrh    r12,[r6,6h]
022E4F90 E5CDC01D strb    r12,[r13,1Dh]
022E4F94 E1D6C0B8 ldrh    r12,[r6,8h]
022E4F98 E5CDC01E strb    r12,[r13,1Eh]
022E4F9C E1D6C0BA ldrh    r12,[r6,0Ah]
022E4FA0 E5CDC01F strb    r12,[r13,1Fh]
022E4FA4 EB0066BB bl      22FEA98h
022E4FA8 E2888001 add     r8,r8,1h
022E4FAC E2866010 add     r6,r6,10h
022E4FB0 E1580007 cmp     r8,r7
022E4FB4 BAFFFFDE blt     22E4F34h
022E4FB8 E3A00001 mov     r0,1h
022E4FBC E28DD044 add     r13,r13,44h
022E4FC0 E8BD8FF0 pop     r4-r11,r15
022E4FC4 02319714 eoreqs  r9,r1,500000h
022E4FC8 02324C6C eoreqs  r4,r2,6C00h


///0204CA94 - fun_0204CA94(R0 = Param0)
0204CA94 E92D4010 push    r4,r14
0204CA98 E1A04000 mov     r4,r0				//R4 = Param0
0204CA9C E3540007 cmp     r4,7h				
if(Param0 < 0x7)
	0204CAA0 908FF104 addls   r15,r15,r4,lsl 2h
else
	0204CAA4 EA000019 b       204CB10h		///0204CB10
0204CAA8 EA000006 b       204CAC8h		///0204CAC8	- 0
0204CAAC EA000013 b       204CB00h		///0204CB00	- 1
0204CAB0 EA000012 b       204CB00h		///0204CB00	- 2
0204CAB4 EA00000C b       204CAECh		///0204CAEC	- 3
0204CAB8 EA00000B b       204CAECh		///0204CAEC	- 4
0204CABC EA00000A b       204CAECh		///0204CAEC	- 5
0204CAC0 EA000009 b       204CAECh		///0204CAEC	- 6
0204CAC4 EA000008 b       204CAECh		///0204CAEC	- 7
///0204CAC8
0204CAC8 E3A00000 mov     r0,0h			//R0 = 0
0204CACC E1A02000 mov     r2,r0			//R2 = 0
0204CAD0 E3A01003 mov     r1,3h			//R1 = 3
0204CAD4 EBFFFAE7 bl      204B678h		///0204B678 - fun_0204B678
{
	0204B678 E92D4038 push    r3-r5,r14
	0204B67C E24DD008 sub     r13,r13,8h
	0204B680 E1A03000 mov     r3,r0				//R3 = Param0 = 0
	0204B684 E1A05001 mov     r5,r1				//R5 = Param1 = 3
	0204B688 E1A04002 mov     r4,r2				//R4 = Param2 = 0
	0204B68C E28D0000 add     r0,r13,0h			//R0 = StackPtr
	0204B690 E1A01003 mov     r1,r3				//R1 = Param0
	0204B694 E1A02005 mov     r2,r5				//R2 = Param1
	0204B698 EBFFFF7F bl      204B49Ch          //0204B49C GetScriptVariableInfo ptrvarinfoentry, ptrvartablebeg, variableid 
	0204B69C E59D1000 ldr     r1,[r13]			//R1 = AddressScriptGlobalInfo
	0204B6A0 E1D100F0 ldrsh   r0,[r1]			//R0 = (Var VERSION??) 0x8
	0204B6A4 E3500009 cmp     r0,9h
	if(R0 < 0x9)
		0204B6A8 908FF100 addls   r15,r15,r0,lsl 2h
	else
		0204B6AC EA000058 b       204B814h		///0204B814
	0204B6B0 EA000057 b       204B814h			///0204B814
	0204B6B4 EA000007 b       204B6D8h			///0204B6D8
	0204B6B8 EA000013 b       204B70Ch			///0204B70C
	0204B6BC EA000012 b       204B70Ch			///0204B70C
	0204B6C0 EA000014 b       204B718h			///0204B718
	0204B6C4 EA000016 b       204B724h			///0204B724
	0204B6C8 EA000019 b       204B734h			///0204B734
	0204B6CC EA00001C b       204B744h			///0204B744
	0204B6D0 EA00001B b       204B744h			///0204B744
	0204B6D4 EA00001D b       204B750h			///0204B750
	///0204B6D8
	0204B6D8 E1D110F6 ldrsh   r1,[r1,6h]			//R1 = [AddressScriptGlobalInfo + 0x6] = 0
	0204B6DC E59D2004 ldr     r2,[r13,4h]			//R2 = [StackPointer + 0x4] == AddressScriptGlobalValue
	0204B6E0 E3A00001 mov     r0,1h					//R0 = 1
	0204B6E4 E0841001 add     r1,r4,r1				//R1 = Param2 + R1
	0204B6E8 E1A01801 mov     r1,r1,lsl 10h			//R1 = R1 << 16
	0204B6EC E1A03821 mov     r3,r1,lsr 10h			//R3 = R1 >> 16 (Unsigned)
	0204B6F0 E2031007 and     r1,r3,7h				//R1 = R3 & 0x7
	0204B6F4 E1A01110 mov     r1,r0,lsl r1			//R1 = R0 << R1
	0204B6F8 E7D221A3 ldrb    r2,[r2,r3,lsr 3h]		//R2 = [AddressScriptGlobalValue + (R3 >> 3)]
	0204B6FC E20110FF and     r1,r1,0FFh			//R1 = R1 & 0xFF
	0204B700 E1120001 tst     r2,r1					//R2 & R1
	if(R2 & R1 == 0)
		0204B704 03A00000 moveq   r0,0h				//R0 = 0
	0204B708 EA000042 b       204B818h				///0204B818 - Return
	///00204B70C
	0204B70C E59D0004 ldr     r0,[r13,4h]			//R0 = [StackPointer + 0x4] == AddressScriptGlobalValue
	0204B710 E7D00004 ldrb    r0,[r0,r4]			//R0 = [AddressScriptGlobalValue + Param2]
	0204B714 EA00003F b       204B818h				///0204B818 - Return
	///0204B718
	0204B718 E59D0004 ldr     r0,[r13,4h]			//R0 = [StackPointer + 0x4] == AddressScriptGlobalValue
	0204B71C E19000D4 ldrsb   r0,[r0,r4]			//R0 = [AddressScriptGlobalValue + Param2]
	0204B720 EA00003C b       204B818h				///0204B818 - Return
	///0204B724
	0204B724 E59D1004 ldr     r1,[r13,4h]			//R1 = [StackPointer + 0x4] == AddressScriptGlobalValue
	0204B728 E1A00084 mov     r0,r4,lsl 1h			//R0 = Param2 << 1
	0204B72C E19100B0 ldrh    r0,[r1,r0]			//R0 = [AddressScriptGlobalValue + R0]
	0204B730 EA000038 b       204B818h				///0204B818 - Return
	///0204B734
	0204B734 E59D1004 ldr     r1,[r13,4h]			//R1 = [StackPointer + 0x4] == AddressScriptGlobalValue
	0204B738 E1A00084 mov     r0,r4,lsl 1h			//R0 = Param2 << 1
	0204B73C E19100F0 ldrsh   r0,[r1,r0]			//R0 = [AddressScriptGlobalValue + R0]
	0204B740 EA000034 b       204B818h				///0204B818 - Return
	///0204B744
	0204B744 E59D0004 ldr     r0,[r13,4h]			//R0 = [StackPointer + 0x4] == AddressScriptGlobalValue
	0204B748 E7900104 ldr     r0,[r0,r4,lsl 2h]		//R0 = [AddressScriptGlobalValue + (Param2 << 2)]
	0204B74C EA000031 b       204B818h				///0204B818 - Return
	///0204B750
	0204B750 E3550048 cmp     r5,48h
	if(R5 > 0x48)
		0204B754 CA00000C bgt     204B78Ch			///0204B78C
	else if(R5 >= 0x48)
		0204B758 AA00001D bge     204B7D4h			///0204B7D4
	0204B75C E355003D cmp     r5,3Dh
	if(R5 > 0x3D)
		0204B760 CA000006 bgt     204B780h			///0204B780
	0204B764 E255003A subs    r0,r5,3Ah				//R0 = R5 - 0x3A
	if(R0 >= 0 )
		0204B768 508FF100 addpl   r15,r15,r0,lsl 2h
	else
		0204B76C EA000028 b       204B814h			///0204B814
	0204B770 EA00000C b       204B7A8h				///0204B7A8 1
	0204B774 EA00000D b       204B7B0h				///0204B7B0 2
	0204B778 EA00000F b       204B7BCh				///0204B7BC 3
	0204B77C EA000010 b       204B7C4h				///0204B7C4 4
	0204B780 E3550047 cmp     r5,47h
	if(R5 == 0x47)
		0204B784 0A000010 beq     204B7CCh			///0204B7CC
	0204B788 EA000021 b       204B814h				///0204B814
	///0204B78C
	0204B78C E3550049 cmp     r5,49h
	if(R5 > 0x49)
		0204B790 CA000001 bgt     204B79Ch			///0204B79C
	else if(R5 >= 0x49)
		0204B794 0A000010 beq     204B7DCh			///0204B7DC
	0204B798 EA00001D b       204B814h				///0204B814
	///0204B79C
	0204B79C E3550070 cmp     r5,70h
	if(R5 == 0x70)
		0204B7A0 0A000019 beq     204B80Ch			///0204B80C
	0204B7A4 EA00001A b       204B814h				///0204B814
	///0204B7A8
	0204B7A8 E3A00001 mov     r0,1h					//R0 = 1
	0204B7AC EA000019 b       204B818h				///0204B818 - Return
	///0204B7B0
	0204B7B0 E3A00000 mov     r0,0h					//R0 = 0
	0204B7B4 EB002D19 bl      2056C20h				//02056C20 - fun_02056C20(R0 = Param0)
	{
		02056C20 E92D47F0 push    r4-r10,r14
		02056C24 E24DD008 sub     r13,r13,8h		//Alloc 8 bytes on stack
		02056C28 E1A05000 mov     r5,r0				//R5 = Param0
		02056C2C EBFFFF1C bl      20568A4h
		02056C30 E1A06000 mov     r6,r0
		02056C34 EBFFFA9B bl      20556A8h
		02056C38 E1A07000 mov     r7,r0
		02056C3C EBFFFAAA bl      20556ECh
		02056C40 E1A08000 mov     r8,r0
		02056C44 EBFFFAB9 bl      2055730h
		02056C48 E1A09000 mov     r9,r0
		02056C4C E3550000 cmp     r5,0h
		02056C50 E3E00000 mvn     r0,0h
		02056C54 028D5000 addeq   r5,r13,0h
		02056C58 E1560000 cmp     r6,r0
		02056C5C E3A04000 mov     r4,0h
		02056C60 E3E00000 mvn     r0,0h
		02056C64 11C560B0 strneh  r6,[r5]
		02056C68 12844001 addne   r4,r4,1h
		02056C6C E1570000 cmp     r7,r0
		02056C70 0A000006 beq     2056C90h
		02056C74 E1A00007 mov     r0,r7
		02056C78 EBFFFDE4 bl      2056410h
		02056C7C E3500000 cmp     r0,0h
		02056C80 11570006 cmpne   r7,r6
		02056C84 11A00084 movne   r0,r4,lsl 1h
		02056C88 118570B0 strneh  r7,[r5,r0]
		02056C8C 12844001 addne   r4,r4,1h
		02056C90 E3E00000 mvn     r0,0h
		02056C94 E1580000 cmp     r8,r0
		02056C98 0A000006 beq     2056CB8h
		02056C9C E1A00008 mov     r0,r8
		02056CA0 EBFFFDDA bl      2056410h
		02056CA4 E3500000 cmp     r0,0h
		02056CA8 11580006 cmpne   r8,r6
		02056CAC 11A00084 movne   r0,r4,lsl 1h
		02056CB0 118580B0 strneh  r8,[r5,r0]
		02056CB4 12844001 addne   r4,r4,1h
		02056CB8 E3E00000 mvn     r0,0h
		02056CBC E1590000 cmp     r9,r0
		02056CC0 0A000006 beq     2056CE0h
		02056CC4 E1A00009 mov     r0,r9
		02056CC8 EBFFFDD0 bl      2056410h
		02056CCC E3500000 cmp     r0,0h
		02056CD0 11590006 cmpne   r9,r6
		02056CD4 11A00084 movne   r0,r4,lsl 1h
		02056CD8 118590B0 strneh  r9,[r5,r0]
		02056CDC 12844001 addne   r4,r4,1h
		02056CE0 E59F0084 ldr     r0,=20B0A48h
		02056CE4 E3A08000 mov     r8,0h
		02056CE8 E5900000 ldr     r0,[r0]
		02056CEC E1A06008 mov     r6,r8
		02056CF0 E2800A09 add     r0,r0,9000h
		02056CF4 E590984C ldr     r9,[r0,84Ch]
		02056CF8 E3A07001 mov     r7,1h
		02056CFC EA000015 b       2056D58h
		02056D00 E5D90000 ldrb    r0,[r9]
		02056D04 E3100002 tst     r0,2h
		02056D08 11A00007 movne   r0,r7
		02056D0C 01A00006 moveq   r0,r6
		02056D10 E31000FF tst     r0,0FFh
		02056D14 0A00000D beq     2056D50h
		02056D18 E1D9A0F8 ldrsh   r10,[r9,8h]
		02056D1C E1A0000A mov     r0,r10
		02056D20 EBFFFDE2 bl      20564B0h
		02056D24 E3500000 cmp     r0,0h
		02056D28 1A000008 bne     2056D50h
		02056D2C E1A0000A mov     r0,r10
		02056D30 EBFFF9FC bl      2055528h
		02056D34 E3500000 cmp     r0,0h
		02056D38 1A000004 bne     2056D50h
		02056D3C E1A00084 mov     r0,r4,lsl 1h
		02056D40 E2844001 add     r4,r4,1h
		02056D44 E185A0B0 strh    r10,[r5,r0]
		02056D48 E3540004 cmp     r4,4h
		02056D4C AA000003 bge     2056D60h
		02056D50 E2888001 add     r8,r8,1h
		02056D54 E2899068 add     r9,r9,68h
		02056D58 E3580004 cmp     r8,4h
		02056D5C BAFFFFE7 blt     2056D00h
		02056D60 E1A00004 mov     r0,r4
		02056D64 E28DD008 add     r13,r13,8h
		02056D68 E8BD87F0 pop     r4-r10,r15
	}
	0204B7B8 EA000016 b       204B818h				///0204B818 - Return
	///0204B7BC
	0204B7BC EBFF0D4E bl      200ECFCh				//0200ECFC - fun_0200ECFC
	0204B7C0 EA000014 b       204B818h				///0204B818 - Return
	///0204B7C4
	0204B7C4 EBFF13D0 bl      201070Ch				//0201070C - fun_0201070C
	0204B7C8 EA000012 b       204B818h				///0204B818 - Return
	///0204B7CC
	0204B7CC EBFF5373 bl      20205A0h				//020205A0 - fun_020205A0
	0204B7D0 EA000010 b       204B818h				///0204B818 - Return
	///0204B7D4
	0204B7D4 EBFFFDF9 bl      204AFC0h				//0204AFC0 - fun_0204AFC0
	0204B7D8 EA00000E b       204B818h				///0204B818 - Return
	///0204B7DC
	0204B7DC EBFFFDF7 bl      204AFC0h				//0204AFC0 - fun_0204AFC0
	0204B7E0 E3500001 cmp     r0,1h
	if(R0 == 1)
		0204B7E4 0A000002 beq     204B7F4h			///0204B7F4
	0204B7E8 E3500003 cmp     r0,3h
	if(R0 == 3)
		0204B7EC 0A000002 beq     204B7FCh			///0204B7FC
	0204B7F0 EA000003 b       204B804h				///0204B804
	///0204B7F4
	0204B7F4 EB000447 bl      204C918h				//0204C918 - fun_0204C918
	0204B7F8 EA000006 b       204B818h				///0204B818 - Return
	///0204B7FC
	0204B7FC EB00043A bl      204C8ECh				//0204C8EC - fun_0204C8EC
	0204B800 EA000004 b       204B818h				///0204B818 - Return
	///0204B804
	0204B804 E3E00000 mvn     r0,0h					//R0 = 0xFFFFFFFF
	0204B808 EA000002 b       204B818h				///0204B818 - Return
	///0204B80C
	0204B80C EBFFF323 bl      20484A0h				//020484A0 - fun_020484A0
	0204B810 EA000000 b       204B818h				///0204B818 - Return
	///0204B814
	0204B814 E3A00000 mov     r0,0h					//R0 = 0
	///0204B818 - Return
	0204B818 E28DD008 add     r13,r13,8h			//Clear stack
	0204B81C E8BD8038 pop     r3-r5,r15				//Return
}
0204CAD8 E3500035 cmp     r0,35h
if(R0 == 0x35)
	0204CADC 03A00001 moveq   r0,1h			//R0 = 1
else
	0204CAE0 13A00000 movne   r0,0h			//R0 = 0
0204CAE4 E20000FF and     r0,r0,0FFh		//R0 = R0 & 0xFF
0204CAE8 E8BD8010 pop     r4,r15
///0204CAEC
0204CAEC EBFFF933 bl      204AFC0h
{
	0204AFC0 E59F0004 ldr     r0,=20AFF70h		//020AFF70
	0204AFC4 E5D00000 ldrb    r0,[r0]			//R0 = [020AFF70]
	0204AFC8 E12FFF1E bx      r14
}
0204CAF0 E3500003 cmp     r0,3h
if(R0 != 3)
	0204CAF4 1A000005 bne     204CB10h		///0204CB10
0204CAF8 E3A00000 mov     r0,0h				//R0 = 0
0204CAFC E8BD8010 pop     r4,r15			//Return 0
///0204CB00
0204CB00 EBFFF92E bl      204AFC0h
{
	0204AFC0 E59F0004 ldr     r0,=20AFF70h		//020AFF70
	0204AFC4 E5D00000 ldrb    r0,[r0]			//R0 = [020AFF70]
	0204AFC8 E12FFF1E bx      r14
}
0204CB04 E3500003 cmp     r0,3h
if(R0 == 3)
	0204CB08 03A00001 moveq   r0,1h			//R0 = 1
	0204CB0C 08BD8010 popeq   r4,r15		//Return 1
///0204CB10 - Param0 >= 0x7
0204CB10 E1A00804 mov     r0,r4,lsl 10h		//R0 = R4 << 16
0204CB14 E1A02820 mov     r2,r0,lsr 10h		//R2 = R0 >> 16
0204CB18 E3A00000 mov     r0,0h				//R0 = 0
0204CB1C E3A0104E mov     r1,4Eh			//R1 = 0x4E
0204CB20 EBFFFAD4 bl      204B678h			///0204B678 - fun_0204B678
0204CB24 E20000FF and     r0,r0,0FFh		//R0 = R0 & 0xFF
0204CB28 E8BD8010 pop     r4,r15			//Return R0