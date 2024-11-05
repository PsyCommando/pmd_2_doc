022C3E8C E92D4FF8 push    r3-r11,r14
022C3E90 E24DD028 sub     r13,r13,28h
022C3E94 E59F444C ldr     r4,=22DC0FCh              //Pointer table containing 5 pointers to file buffers
022C3E98 E1A08001 mov     r8,r1
022C3E9C E5945004 ldr     r5,[r4,4h]
022C3EA0 E5946000 ldr     r6,[r4]
022C3EA4 E1A09000 mov     r9,r0
022C3EA8 E1A0A002 mov     r10,r2
022C3EAC E3A04000 mov     r4,0h
022C3EB0 E58D5024 str     r5,[r13,24h]
022C3EB4 E59F1430 ldr     r1,=22DC114h              //"rom0:BALANCE/mappa_gs.bin"
022C3EB8 E28D0020 add     r0,r13,20h                //R0 = pointer to pointer to first entry in table 22DC0FCh
022C3EBC E58D6020 str     r6,[r13,20h]              //R13+0x20 = pointer to first entry in table 22DC0FCh
022C3EC0 E3A02006 mov     r2,6h
022C3EC4 E1A0B003 mov     r11,r3
022C3EC8 E58D4010 str     r4,[r13,10h]
022C3ECC E59D5050 ldr     r5,[r13,50h]
022C3ED0 EBF51359 bl      2008C3Ch                //LoadFileFromRom(R1=filepath)
{
    02008C3C E92D4008 push    r3,r14
    02008C40 E24DD028 sub     r13,r13,28h
    02008C44 E1A0E000 mov     r14,r0                //r14 = pointer to pointer to first entry in table 22DC0FCh
    02008C48 E1A0C001 mov     r12,r1                //R12 = FilenameToLoad
    02008C4C E1A03002 mov     r3,r2                 //R3  = (6)
    02008C50 E28D0000 add     r0,r13,0h             //R0 = Pointer on stack
    02008C54 E1A0100E mov     r1,r14                //R1 = pointer to pointer to first entry in table 22DC0FCh
    02008C58 E1A0200C mov     r2,r12                //R2  = FilenameToLoad
    02008C5C EB00003F bl      2008D60h
    {
        02008D60 E92D40F8 push    r3-r7,r14
        02008D64 E24DDC01 sub     r13,r13,100h
        02008D68 E1A07000 mov     r7,r0             //R7 = Pointer on stack
        02008D6C E1A05002 mov     r5,r2             //R5 = FilenameToLoad
        02008D70 E1A06001 mov     r6,r1             //R6 = pointer to pointer to first entry in table 22DC0FCh
        02008D74 E1A04003 mov     r4,r3             //R4 = (6)
        02008D78 EBFFFDB3 bl      200844Ch
        {
            0200844C E92D4010 push    r4,r14
            02008450 E1A04000 mov     r4,r0         //R4 = Pointer on stack
            02008454 EBFFFA9E bl      2006ED4h
            {
                02006ED4 E3A02000 mov     r2,0h
                02006ED8 E5802000 str     r2,[r0]   
                02006EDC E2421001 sub     r1,r2,1h
                02006EE0 E9800006 stmib   [r0],r1,r2
                02006EE4 E5C0200C strb    r2,[r0,0Ch]
                02006EE8 E5801010 str     r1,[r0,10h]
                02006EEC E5802014 str     r2,[r0,14h]
                02006EF0 E5802018 str     r2,[r0,18h]
                02006EF4 E12FFF1E bx      r14
            }
            02008458 E3A00000 mov     r0,0h
            0200845C E584001C str     r0,[r4,1Ch]
            02008460 E5840020 str     r0,[r4,20h]
            02008464 E5840024 str     r0,[r4,24h]
            02008468 E8BD8010 pop     r4,r15
        }
        02008D7C E28D0000 add     r0,r13,0h             //R0 = new pointer on stack
        02008D80 E1A01007 mov     r1,r7                 //R1 = Pointer on stack
        02008D84 E1A03005 mov     r3,r5                 //R3 = FilenameToLoad
        02008D88 E3A0200A mov     r2,0Ah
        02008D8C EBFFFEFB bl      2008980h
        {
            02008980 E92D4010 push    r4,r14
            02008984 E1A04000 mov     r4,r0
            02008988 E5842000 str     r2,[r4]
            0200898C E3530000 cmp     r3,0h
            02008990 E5841004 str     r1,[r4,4h]
            02008994 03A00000 moveq   r0,0h
            02008998 05C40028 streqb  r0,[r4,28h]
            0200899C 0A000002 beq     20089ACh
            020089A0 E1A01003 mov     r1,r3             //R1 = FilenameToLoad
            020089A4 E2840028 add     r0,r4,28h
            020089A8 EB020339 bl      2089694h
            {
                02089694 E92D4038 push    r3-r5,r14
                02089698 E2014003 and     r4,r1,3h
                0208969C E2003003 and     r3,r0,3h
                020896A0 E1A02000 mov     r2,r0
                020896A4 E1530004 cmp     r3,r4
                020896A8 1A000020 bne     2089730h              //02089730
                020896AC E3540000 cmp     r4,0h
                020896B0 0A00000D beq     20896ECh              //020896EC
                020896B4 E5D13000 ldrb    r3,[r1]
                020896B8 E5C03000 strb    r3,[r0]
                020896BC E3530000 cmp     r3,0h
                020896C0 08BD8038 popeq   r3-r5,r15
                020896C4 E2744003 rsbs    r4,r4,3h
                020896C8 0A000005 beq     20896E4h              ///020896E4
                020896CC E5F13001 ldrb    r3,[r1,1h]!
                020896D0 E3530000 cmp     r3,0h
                020896D4 E5E23001 strb    r3,[r2,1h]!
                020896D8 08BD8038 popeq   r3-r5,r15
                020896DC E2544001 subs    r4,r4,1h
                020896E0 1AFFFFF9 bne     20896CCh
                020896E4 E2822001 add     r2,r2,1h
                020896E8 E2811001 add     r1,r1,1h
                020896EC E5915000 ldr     r5,[r1]
                020896F0 E59F305C ldr     r3,=0FEFEFEFFh
                020896F4 E1E04005 mvn     r4,r5
                020896F8 E085E003 add     r14,r5,r3
                020896FC E59FC054 ldr     r12,=80808080h
                02089700 E00E4004 and     r4,r14,r4
                02089704 E114000C tst     r4,r12
                02089708 1A000008 bne     2089730h
                0208970C E2422004 sub     r2,r2,4h
                02089710 E5A25004 str     r5,[r2,4h]!
                02089714 E5B15004 ldr     r5,[r1,4h]!
                02089718 E0854003 add     r4,r5,r3
                0208971C E1E0E005 mvn     r14,r5
                02089720 E004E00E and     r14,r4,r14
                02089724 E11E000C tst     r14,r12
                02089728 0AFFFFF8 beq     2089710h
                0208972C E2822004 add     r2,r2,4h
                02089730 E5D13000 ldrb    r3,[r1]
                02089734 E5C23000 strb    r3,[r2]
                02089738 E3530000 cmp     r3,0h
                0208973C 08BD8038 popeq   r3-r5,r15
                02089740 E5F13001 ldrb    r3,[r1,1h]!
                02089744 E3530000 cmp     r3,0h
                02089748 E5E23001 strb    r3,[r2,1h]!
                0208974C 1AFFFFFB bne     2089740h
                02089750 E8BD8038 pop     r3-r5,r15
                02089754 FEFEFEFF 
                02089758 80808080 
            }
            020089AC E3A00000 mov     r0,0h
            020089B0 E5C40009 strb    r0,[r4,9h]
            020089B4 E5C40008 strb    r0,[r4,8h]
            020089B8 E584000C str     r0,[r4,0Ch]
            020089BC E5840010 str     r0,[r4,10h]
            020089C0 E5840014 str     r0,[r4,14h]
            020089C4 E5840018 str     r0,[r4,18h]
            020089C8 E584001C str     r0,[r4,1Ch]
            020089CC E5840020 str     r0,[r4,20h]
            020089D0 E5840024 str     r0,[r4,24h]
            020089D4 E8BD8010 pop     r4,r15
        }
        02008D90 E28D0000 add     r0,r13,0h
        02008D94 E58D6010 str     r6,[r13,10h]
        02008D98 E58D4024 str     r4,[r13,24h]
        02008D9C EBFFFF38 bl      2008A84h
        {
            02008A84 E92D4078 push    r3-r6,r14
            02008A88 E24DD024 sub     r13,r13,24h
            02008A8C E1A05000 mov     r5,r0
            02008A90 E5954004 ldr     r4,[r5,4h]
            02008A94 E1A00004 mov     r0,r4
            02008A98 EBFFFFAA bl      2008948h
            {
                02008948 E92D4010 push    r4,r14
                0200894C E1A04000 mov     r4,r0
                02008950 E59F0024 ldr     r0,=20AF3E4h
                02008954 EBFFE8D6 bl      2002CB4h
                {
                    02002CB4 E92D4070 push    r4-r6,r14
                    02002CB8 E59F1050 ldr     r1,=22B966Ch
                    02002CBC E1A06000 mov     r6,r0
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
                    02002D10 022B966C
                }
                02008958 E5940024 ldr     r0,[r4,24h]
                0200895C E3500004 cmp     r0,4h
                02008960 13500005 cmpne   r0,5h
                02008964 03A04001 moveq   r4,1h
                02008968 E59F000C ldr     r0,=20AF3E4h
                0200896C 13A04000 movne   r4,0h
                02008970 EBFFE948 bl      2002E98h
                02008974 E20400FF and     r0,r4,0FFh
                02008978 E8BD8010 pop     r4,r15
                0200897C 020AF3E4 andeq   r15,r10,90000003h
            }
            02008A9C E3500000 cmp     r0,0h
            02008AA0 1A000016 bne     2008B00h
            02008AA4 E5951000 ldr     r1,[r5]
            02008AA8 E1A00005 mov     r0,r5
            02008AAC E5841020 str     r1,[r4,20h]
            02008AB0 E3A01004 mov     r1,4h
            02008AB4 E5841024 str     r1,[r4,24h]
            02008AB8 EBFFFFC6 bl      20089D8h
            02008ABC E1A01000 mov     r1,r0
            02008AC0 E3A02001 mov     r2,1h
            02008AC4 E59F0098 ldr     r0,=22A369Ch
            02008AC8 E5C12008 strb    r2,[r1,8h]
            02008ACC EBFFE92A bl      2002F7Ch
            02008AD0 E59F6090 ldr     r6,=20AF53Ch
            02008AD4 E1A00006 mov     r0,r6
            02008AD8 EBFFE827 bl      2002B7Ch
            02008ADC E5943024 ldr     r3,[r4,24h]
            02008AE0 E3530004 cmp     r3,4h
            02008AE4 1A000002 bne     2008AF4h
            02008AE8 E1A00006 mov     r0,r6
            02008AEC EBFFE80B bl      2002B20h
            02008AF0 EAFFFFF7 b       2008AD4h
            02008AF4 E3530002 cmp     r3,2h
            02008AF8 8A000001 bhi     2008B04h
            02008AFC EA000016 b       2008B5Ch
            02008B00 E3A03005 mov     r3,5h
            02008B04 E59F0060 ldr     r0,=2092908h
            02008B08 E59F2060 ldr     r2,=20AF454h
            02008B0C E5901024 ldr     r1,[r0,24h]
            02008B10 E5900020 ldr     r0,[r0,20h]
            02008B14 E58D1020 str     r1,[r13,20h]
            02008B18 E58D001C str     r0,[r13,1Ch]
            02008B1C E595C000 ldr     r12,[r5]
            02008B20 E59F104C ldr     r1,=20929B8h
            02008B24 E88D1008 stmea   [r13],r3,r12
            02008B28 E5943020 ldr     r3,[r4,20h]
            02008B2C E28D001C add     r0,r13,1Ch
            02008B30 E58D3008 str     r3,[r13,8h]
            02008B34 E58D400C str     r4,[r13,0Ch]
            02008B38 E595400C ldr     r4,[r5,0Ch]
            02008B3C E2853028 add     r3,r5,28h
            02008B40 E58D4010 str     r4,[r13,10h]
            02008B44 E5954010 ldr     r4,[r5,10h]
            02008B48 E58D4014 str     r4,[r13,14h]
            02008B4C E5954014 ldr     r4,[r5,14h]
            02008B50 E58D4018 str     r4,[r13,18h]
            02008B54 E792210C ldr     r2,[r2,r12,lsl 2h]
            02008B58 EB000DBF bl      200C25Ch
            02008B5C E28DD024 add     r13,r13,24h
            02008B60 E8BD8078 pop     r3-r6,r15
            02008B64 022A369C
            02008B68 020AF53C
            02008B6C 02092908
            02008B70 020AF454
            02008B74 020929B8
        }
        02008DA0 E5970010 ldr     r0,[r7,10h]
        02008DA4 E28DDC01 add     r13,r13,100h
        02008DA8 E8BD80F8 pop     r3-r7,r15
    }
    02008C60 E28DD028 add     r13,r13,28h
    02008C64 E8BD8008 pop     r3,r15
}
022C3ED4 E59D1020 ldr     r1,[r13,20h]
022C3ED8 E28D0010 add     r0,r13,10h
022C3EDC EBF56D74 bl      201F4B4h                  //HandleSir0Translation(R0=OutPtrToSIR0Data, R1=filebuffer)
022C3EE0 E5D50000 ldrb    r0,[r5]
022C3EE4 E59D6010 ldr     r6,[r13,10h]
022C3EE8 EBF62DB6 bl      204F5C8h
{
    0204F5C8 E35000B4 cmp     r0,0B4h
    0204F5CC 359F1008 ldrcc   r1,=209E3A1h
    0204F5D0 23A00035 movcs   r0,35h
    0204F5D4 37D10100 ldrccb  r0,[r1,r0,lsl 2h]
    0204F5D8 E12FFF1E bx      r14
    0204F5DC 0209E3A1 
}
022C3EEC E1A07000 mov     r7,r0
022C3EF0 E5D52000 ldrb    r2,[r5]
022C3EF4 E5D53001 ldrb    r3,[r5,1h]
022C3EF8 E28D000D add     r0,r13,0Dh
022C3EFC E28D100C add     r1,r13,0Ch
022C3F00 EB0000FB bl      22C42F4h
{
    022C42F4 E92D40F8 push    r3-r7,r14
    022C42F8 E1A05002 mov     r5,r2
    022C42FC E1A07000 mov     r7,r0
    022C4300 E1A04003 mov     r4,r3
    022C4304 E1A00005 mov     r0,r5
    022C4308 E1A06001 mov     r6,r1
    022C430C EBF62C9A bl      204F57Ch
    022C4310 E1540000 cmp     r4,r0
    022C4314 C1A04000 movgt   r4,r0
    022C4318 E1A00005 mov     r0,r5
    022C431C EBF62CAF bl      204F5E0h
    022C4320 E0844000 add     r4,r4,r0
    022C4324 E1A00005 mov     r0,r5
    022C4328 EBF62CB2 bl      204F5F8h
    022C432C E3540001 cmp     r4,1h
    022C4330 B3A04001 movlt   r4,1h
    022C4334 BA000001 blt     22C4340h
    022C4338 E1540000 cmp     r4,r0
    022C433C C1A04000 movgt   r4,r0
    022C4340 E5C74000 strb    r4,[r7]
    022C4344 E5C60000 strb    r0,[r6]
    022C4348 E8BD80F8 pop     r3-r7,r15
    022C434C E59DC004 ldr     r12,[r13,4h]
    022C4350 E3500000 cmp     r0,0h
    022C4354 B1A0000C movlt   r0,r12
    022C4358 B12FFF1E bxlt    r14
    022C435C E3510000 cmp     r1,0h
    022C4360 B1A0000C movlt   r0,r12
    022C4364 B12FFF1E bxlt    r14
    022C4368 E1500002 cmp     r0,r2
    022C436C A1A0000C movge   r0,r12
    022C4370 A12FFF1E bxge    r14
    022C4374 E1510003 cmp     r1,r3
    022C4378 B0200291 mlalt   r0,r1,r2,r0
    022C437C B59D1000 ldrlt   r1,[r13]
    022C4380 B1A00080 movlt   r0,r0,lsl 1h
    022C4384 B19100B0 ldrlth  r0,[r1,r0]
    022C4388 B200C003 andlt   r12,r0,3h
    022C438C E1A0000C mov     r0,r12
    022C4390 E12FFF1E bx      r14
    022C4394 022BCE74 
}
022C3F04 E5962000 ldr     r2,[r6]
022C3F08 E5DD100D ldrb    r1,[r13,0Dh]
022C3F0C E7922107 ldr     r2,[r2,r7,lsl 2h]
022C3F10 E5960004 ldr     r0,[r6,4h]
022C3F14 E1A01101 mov     r1,r1,lsl 2h
022C3F18 E19210F1 ldrsh   r1,[r2,r1]
022C3F1C E0800101 add     r0,r0,r1,lsl 2h
022C3F20 E5D05001 ldrb    r5,[r0,1h]
022C3F24 E3550000 cmp     r5,0h
if(R5 == 0)
    022C3F28 0A0000E9 beq     22C42D4h              ///022C42D4 LBL1
022C3F2C E59F13B4 ldr     r1,=22DC0FCh              //Pointer table containing 5 pointers to file buffers
022C3F30 E28D0018 add     r0,r13,18h
022C3F34 E5913014 ldr     r3,[r1,14h]
022C3F38 E5912010 ldr     r2,[r1,10h]
022C3F3C E59F13AC ldr     r1,=22DC190h              //"rom0:BALANCE/fixed.bin"
022C3F40 E58D2018 str     r2,[r13,18h]
022C3F44 E3A0200F mov     r2,0Fh
022C3F48 E58D301C str     r3,[r13,1Ch]
022C3F4C EBF5133A bl      2008C3Ch                //LoadFileFromRom(R0=DestinationBufferPointer, R1=filepath)
022C3F50 E59D1018 ldr     r1,[r13,18h]
022C3F54 E28D0014 add     r0,r13,14h
022C3F58 EBF56D55 bl      201F4B4h                  ///0201F4B4 HandleSIR0(R0=, R1=SIR0FileBufferPtr )
{
    0201F4B4 E92D4038 push    r3-r5,r14
    0201F4B8 E1A04001 mov     r4,r1
    0201F4BC E5D42000 ldrb    r2,[r4]
    0201F4C0 E1A05000 mov     r5,r0
    0201F4C4 E3A00000 mov     r0,0h
    0201F4C8 E3520053 cmp     r2,53h            //'S'
    if(R2 == 'S')
        0201F4CC 05D41001 ldreqb  r1,[r4,1h]
        0201F4D0 03510049 cmpeq   r1,49h            //'I'
        if(R1 == 'I')
            0201F4D4 05D41002 ldreqb  r1,[r4,2h]
            0201F4D8 03510052 cmpeq   r1,52h            //'R'
            if(R1 == 'R')
                0201F4DC 05D41003 ldreqb  r1,[r4,3h]
                0201F4E0 03510030 cmpeq   r1,30h            //'0'
                if(R1 != '0')
                    0201F4E4 1A000005 bne     201F500h          //0201F500 LBL3
    0201F4E8 E3A0104F mov     r1,4Fh            //'O'
    0201F4EC E1A00004 mov     r0,r4
    0201F4F0 E5C41003 strb    r1,[r4,3h]        //Change the '0' or SIR0 to 'O'
    0201F4F4 EB00000E bl      201F534h          ///0201F534 Translate SIR0 pointers
    0201F4F8 E3A00001 mov     r0,1h
    0201F4FC EA000007 b       201F520h          //0201F520 LBL2
    //0201F500 LBL3
    0201F500 E3520053 cmp     r2,53h            //'S'
    if(R2 == 'S')
        0201F504 05D41001 ldreqb  r1,[r4,1h]
        0201F508 03510049 cmpeq   r1,49h            //'I'
        if(R1 == 'I')
            0201F50C 05D41002 ldreqb  r1,[r4,2h]
            0201F510 03510052 cmpeq   r1,52h            //'R'
            if(R1 == 'R')
                0201F514 05D41003 ldreqb  r1,[r4,3h]
                0201F518 0351004F cmpeq   r1,4Fh            //'O'
                if(R1 == 'O')
                    0201F51C 03A00002 moveq   r0,2h         //R0 = 2
    //0201F520 LBL2
    0201F520 E3500000 cmp     r0,0h
    if(R0 != 0)
        0201F524 15941004 ldrne   r1,[r4,4h]
        0201F528 15851000 strne   r1,[r5]
    else
        0201F52C 05854000 streq   r4,[r5]
    0201F530 E8BD8038 pop     r3-r5,r15
}
022C3F5C E59D2014 ldr     r2,[r13,14h]
022C3F60 E1A03005 mov     r3,r5
022C3F64 E1A0000A mov     r0,r10
022C3F68 E1A0100B mov     r1,r11
022C3F6C EBFFFA1D bl      22C27E8h                  ///022C27E8
{
    022C27E8 E92D4008 push    r3,r14
    022C27EC E792E103 ldr     r14,[r2,r3,lsl 2h]
    022C27F0 E59FC03C ldr     r12,=22DC224h
    022C27F4 E1DEE0B0 ldrh    r14,[r14]
    022C27F8 E580E000 str     r14,[r0]
    022C27FC E7920103 ldr     r0,[r2,r3,lsl 2h]
    022C2800 E1D000B2 ldrh    r0,[r0,2h]
    022C2804 E5810000 str     r0,[r1]
    022C2808 E7920103 ldr     r0,[r2,r3,lsl 2h]
    022C280C E2800006 add     r0,r0,6h
    022C2810 E58C0004 str     r0,[r12,4h]
    022C2814 E5910000 ldr     r0,[r1]
    022C2818 E350003E cmp     r0,3Eh
    022C281C C3A0003E movgt   r0,3Eh
    022C2820 C5810000 strgt   r0,[r1]
    022C2824 E59F0008 ldr     r0,=22DC224h
    022C2828 E3A01000 mov     r1,0h
    022C282C E1C010B2 strh    r1,[r0,2h]
    022C2830 E8BD8008 pop     r3,r15
    022C2834 022DC224 
}
022C3F70 E59B0000 ldr     r0,[r11]
022C3F74 E3A03000 mov     r3,0h
022C3F78 E58D0004 str     r0,[r13,4h]
022C3F7C E280500A add     r5,r0,0Ah
022C3F80 E59A4000 ldr     r4,[r10]
022C3F84 E1A00003 mov     r0,r3
022C3F88 E1A01003 mov     r1,r3
022C3F8C EA000009 b       22C3FB8h              ///022C3FB8 LBL4
///022C3F90 LOOP_BEG
022C3F90 E0060893 mul     r6,r3,r8
022C3F94 E1A02001 mov     r2,r1
022C3F98 EA000003 b       22C3FACh              ///022C3FAC LBL5
///022C3F9C LBL7
022C3F9C E0827006 add     r7,r2,r6
022C3FA0 E1A07087 mov     r7,r7,lsl 1h
022C3FA4 E18900B7 strh    r0,[r9,r7]
022C3FA8 E2822001 add     r2,r2,1h
///022C3FAC LBL5
022C3FAC E1520008 cmp     r2,r8
if( R2 < R8 )
    022C3FB0 BAFFFFF9 blt     22C3F9Ch              ///022C3F9C LBL7
022C3FB4 E2833001 add     r3,r3,1h
///022C3FB8 LBL4
022C3FB8 E1530005 cmp     r3,r5
if( R3 < R5 )
    022C3FBC BAFFFFF3 blt     22C3F90h              ///022C3F90 LOOP_BEG
022C3FC0 E3A06005 mov     r6,5h
022C3FC4 EA00009B b       22C4238h
022C3FC8 E0000896 mul     r0,r6,r8
022C3FCC E58D0000 str     r0,[r13]
022C3FD0 E2840005 add     r0,r4,5h
022C3FD4 E3A07005 mov     r7,5h
022C3FD8 E58D0008 str     r0,[r13,8h]
022C3FDC EA000091 b       22C4228h
022C3FE0 EBFFFA14 bl      22C2838h
022C3FE4 E59D1000 ldr     r1,[r13]
022C3FE8 E3570000 cmp     r7,0h
022C3FEC E0871001 add     r1,r7,r1
022C3FF0 E20000FF and     r0,r0,0FFh
022C3FF4 E0891081 add     r1,r9,r1,lsl 1h
022C3FF8 A3560000 cmpge   r6,0h
022C3FFC BA000088 blt     22C4224h
022C4000 E1570008 cmp     r7,r8
022C4004 B1560005 cmplt   r6,r5
022C4008 AA000085 bge     22C4224h
022C400C E3500010 cmp     r0,10h
022C4010 3A000005 bcc     22C402Ch
022C4014 E1D120B0 ldrh    r2,[r1]
022C4018 E3C22003 bic     r2,r2,3h
022C401C E1C120B0 strh    r2,[r1]
022C4020 E1D120B0 ldrh    r2,[r1]
022C4024 E3822001 orr     r2,r2,1h
022C4028 E1C120B0 strh    r2,[r1]
022C402C E3500043 cmp     r0,43h
022C4030 CA000013 bgt     22C4084h
022C4034 AA000042 bge     22C4144h
022C4038 E350000F cmp     r0,0Fh
022C403C 908FF100 addls   r15,r15,r0,lsl 2h
022C4040 EA000077 b       22C4224h
022C4044 EA000014 b       22C409Ch
022C4048 EA00001A b       22C40B8h
022C404C EA000020 b       22C40D4h
022C4050 EA000073 b       22C4224h
022C4054 EA000010 b       22C409Ch
022C4058 EA000024 b       22C40F0h
022C405C EA00002A b       22C410Ch
022C4060 EA00006F b       22C4224h
022C4064 EA000036 b       22C4144h
022C4068 EA00000B b       22C409Ch
022C406C EA00002D b       22C4128h
022C4070 EA000044 b       22C4188h
022C4074 EA00004B b       22C41A8h
022C4078 EA000055 b       22C41D4h
022C407C EA000062 b       22C420Ch
022C4080 EA000067 b       22C4224h
022C4084 E3500044 cmp     r0,44h
022C4088 CA000001 bgt     22C4094h
022C408C 0A000033 beq     22C4160h
022C4090 EA000063 b       22C4224h
022C4094 E3500045 cmp     r0,45h
022C4098 EA000061 b       22C4224h
022C409C E1D100B0 ldrh    r0,[r1]
022C40A0 E3C00003 bic     r0,r0,3h
022C40A4 E1C100B0 strh    r0,[r1]
022C40A8 E1D100B0 ldrh    r0,[r1]
022C40AC E3800001 orr     r0,r0,1h
022C40B0 E1C100B0 strh    r0,[r1]
022C40B4 EA00005A b       22C4224h
022C40B8 E1D100B0 ldrh    r0,[r1]
022C40BC E3C00003 bic     r0,r0,3h
022C40C0 E1C100B0 strh    r0,[r1]
022C40C4 E1D100B0 ldrh    r0,[r1]
022C40C8 E3C00C01 bic     r0,r0,100h
022C40CC E1C100B0 strh    r0,[r1]
022C40D0 EA000053 b       22C4224h
022C40D4 E1D100B0 ldrh    r0,[r1]
022C40D8 E3C00003 bic     r0,r0,3h
022C40DC E1C100B0 strh    r0,[r1]
022C40E0 E1D100B0 ldrh    r0,[r1]
022C40E4 E3800010 orr     r0,r0,10h
022C40E8 E1C100B0 strh    r0,[r1]
022C40EC EA00004C b       22C4224h
022C40F0 E1D100B0 ldrh    r0,[r1]
022C40F4 E3C00003 bic     r0,r0,3h
022C40F8 E1C100B0 strh    r0,[r1]
022C40FC E1D100B0 ldrh    r0,[r1]
022C4100 E3800002 orr     r0,r0,2h
022C4104 E1C100B0 strh    r0,[r1]
022C4108 EA000045 b       22C4224h
022C410C E1D100B0 ldrh    r0,[r1]
022C4110 E3C00003 bic     r0,r0,3h
022C4114 E1C100B0 strh    r0,[r1]
022C4118 E1D100B0 ldrh    r0,[r1]
022C411C E3800003 orr     r0,r0,3h
022C4120 E1C100B0 strh    r0,[r1]
022C4124 EA00003E b       22C4224h
022C4128 E1D100B0 ldrh    r0,[r1]
022C412C E3C00003 bic     r0,r0,3h
022C4130 E1C100B0 strh    r0,[r1]
022C4134 E1D100B0 ldrh    r0,[r1]
022C4138 E3800013 orr     r0,r0,13h
022C413C E1C100B0 strh    r0,[r1]
022C4140 EA000037 b       22C4224h
022C4144 E1D100B0 ldrh    r0,[r1]
022C4148 E3C00003 bic     r0,r0,3h
022C414C E1C100B0 strh    r0,[r1]
022C4150 E1D100B0 ldrh    r0,[r1]
022C4154 E3800001 orr     r0,r0,1h
022C4158 E1C100B0 strh    r0,[r1]
022C415C EA000030 b       22C4224h
022C4160 E1D100B0 ldrh    r0,[r1]
022C4164 E3800B02 orr     r0,r0,800h
022C4168 E1C100B0 strh    r0,[r1]
022C416C E1D100B0 ldrh    r0,[r1]
022C4170 E3C00003 bic     r0,r0,3h
022C4174 E1C100B0 strh    r0,[r1]
022C4178 E1D100B0 ldrh    r0,[r1]
022C417C E3800001 orr     r0,r0,1h
022C4180 E1C100B0 strh    r0,[r1]
022C4184 EA000026 b       22C4224h
022C4188 E1D100B0 ldrh    r0,[r1]
022C418C E3C00003 bic     r0,r0,3h
022C4190 E1C100B0 strh    r0,[r1]
022C4194 E1D100B0 ldrh    r0,[r1]
022C4198 E3800001 orr     r0,r0,1h
022C419C E3800B02 orr     r0,r0,800h
022C41A0 E1C100B0 strh    r0,[r1]
022C41A4 EA00001E b       22C4224h
022C41A8 E1D100B0 ldrh    r0,[r1]
022C41AC E3C00003 bic     r0,r0,3h
022C41B0 E1C100B0 strh    r0,[r1]
022C41B4 E1D100B0 ldrh    r0,[r1]
022C41B8 E3800001 orr     r0,r0,1h
022C41BC E3800A01 orr     r0,r0,1000h
022C41C0 E1C100B0 strh    r0,[r1]
022C41C4 E1D100B0 ldrh    r0,[r1]
022C41C8 E3800E81 orr     r0,r0,810h
022C41CC E1C100B0 strh    r0,[r1]
022C41D0 EA000013 b       22C4224h
022C41D4 E1D100B0 ldrh    r0,[r1]
022C41D8 E3C00003 bic     r0,r0,3h
022C41DC E1C100B0 strh    r0,[r1]
022C41E0 E1D100B0 ldrh    r0,[r1]
022C41E4 E3800001 orr     r0,r0,1h
022C41E8 E3800A01 orr     r0,r0,1000h
022C41EC E1C100B0 strh    r0,[r1]
022C41F0 E1D100B0 ldrh    r0,[r1]
022C41F4 E3800E81 orr     r0,r0,810h
022C41F8 E1C100B0 strh    r0,[r1]
022C41FC E1D100B0 ldrh    r0,[r1]
022C4200 E3800A02 orr     r0,r0,2000h
022C4204 E1C100B0 strh    r0,[r1]
022C4208 EA000005 b       22C4224h
022C420C E1D100B0 ldrh    r0,[r1]
022C4210 E3C00003 bic     r0,r0,3h
022C4214 E1C100B0 strh    r0,[r1]
022C4218 E1D100B0 ldrh    r0,[r1]
022C421C E3800010 orr     r0,r0,10h
022C4220 E1C100B0 strh    r0,[r1]
022C4224 E2877001 add     r7,r7,1h
022C4228 E59D0008 ldr     r0,[r13,8h]
022C422C E1570000 cmp     r7,r0
022C4230 BAFFFF6A blt     22C3FE0h
022C4234 E2866001 add     r6,r6,1h
022C4238 E59D0004 ldr     r0,[r13,4h]
022C423C E2802005 add     r2,r0,5h
022C4240 E1560002 cmp     r6,r2
022C4244 BAFFFF5F blt     22C3FC8h
022C4248 E3A06000 mov     r6,0h
022C424C E2841005 add     r1,r4,5h
022C4250 E3A0EC01 mov     r14,100h
022C4254 EA000014 b       22C42ACh
022C4258 E0030896 mul     r3,r6,r8
022C425C E3A07000 mov     r7,0h
022C4260 EA00000E b       22C42A0h
022C4264 E3570005 cmp     r7,5h
022C4268 E0870003 add     r0,r7,r3
022C426C BA000005 blt     22C4288h
022C4270 E1570001 cmp     r7,r1
022C4274 AA000003 bge     22C4288h
022C4278 E3560005 cmp     r6,5h
022C427C BA000001 blt     22C4288h
022C4280 E1560002 cmp     r6,r2
022C4284 BA000004 blt     22C429Ch
022C4288 E1A00080 mov     r0,r0,lsl 1h
022C428C E189E0B0 strh    r14,[r9,r0]
022C4290 E199C0B0 ldrh    r12,[r9,r0]
022C4294 E38CC010 orr     r12,r12,10h
022C4298 E189C0B0 strh    r12,[r9,r0]
022C429C E2877001 add     r7,r7,1h
022C42A0 E1570008 cmp     r7,r8
022C42A4 BAFFFFEE blt     22C4264h
022C42A8 E2866001 add     r6,r6,1h
022C42AC E1560005 cmp     r6,r5
022C42B0 BAFFFFE8 blt     22C4258h
022C42B4 E28D0018 add     r0,r13,18h
022C42B8 EBF51245 bl      2008BD4h
022C42BC E284000A add     r0,r4,0Ah
022C42C0 E58A0000 str     r0,[r10]
022C42C4 E59D0004 ldr     r0,[r13,4h]
022C42C8 E3A04001 mov     r4,1h
022C42CC E280000A add     r0,r0,0Ah
022C42D0 E58B0000 str     r0,[r11]
///022C42D4 LBL1
022C42D4 E28D0020 add     r0,r13,20h                //mappa_gs.bin buffer
022C42D8 EBF5123D bl      2008BD4h                  //Memfree wrapper
022C42DC E1A00004 mov     r0,r4
022C42E0 E28DD028 add     r13,r13,28h
022C42E4 E8BD8FF8 pop     r3-r11,r15
022C42E8 022DC0FC 
022C42EC 022DC114 
022C42F0 022DC190 