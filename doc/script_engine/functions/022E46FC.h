
/*
    fun_022E46FC
        Load a script?
        Seems to be mainly used to load unionall.ssb
*/
//R0 = PtrStackArea(Maybe return?), R1 = PathScriptToLoad
022E46FC E92D4030 push    r4,r5,r14
022E4700 E24DD054 sub     r13,r13,54h
022E4704 E1A04001 mov     r4,r1             //R4 = PathScriptToLoad
022E4708 E1A05000 mov     r5,r0             //R5 = PtrStackArea
022E470C E59F1190 ldr     r1,=2319694h      //"script load file name [%s]"
022E4710 E1A02004 mov     r2,r4             //R2 = PathScriptToLoad
022E4714 E3A00002 mov     r0,2h
022E4718 EBF49EC8 bl      200C240h          ///DebugPrint 
022E471C EBF48E91 bl      2008168h          ///fun_02008168
022E4720 E28D000C add     r0,r13,0Ch
022E4724 EBF48EB6 bl      2008204h          ///fun_02008204
022E4728 E28D000C add     r0,r13,0Ch
022E472C E1A01004 mov     r1,r4
022E4730 EBF48EB6 bl      2008210h          ///02008210
022E4734 E28D000C add     r0,r13,0Ch
022E4738 E28D1000 add     r1,r13,0h
022E473C E3A0200C mov     r2,0Ch
022E4740 EBF48EC3 bl      2008254h          ///02008254
022E4744 E1DD30B4 ldrh    r3,[r13,4h]
022E4748 E1DD20B6 ldrh    r2,[r13,6h]
022E474C E59F0154 ldr     r0,=2324F70h
022E4750 E28D1000 add     r1,r13,0h
022E4754 E0833002 add     r3,r3,r2
022E4758 E1D020D0 ldrsb   r2,[r0]
022E475C E5950000 ldr     r0,[r5]
022E4760 E3500000 cmp     r0,0h
022E4764 E2820004 add     r0,r2,4h
022E4768 E1A00080 mov     r0,r0,lsl 1h
022E476C E19100B0 ldrh    r0,[r1,r0]
022E4770 E0830000 add     r0,r3,r0
022E4774 E1A04080 mov     r4,r0,lsl 1h
022E4778 1A000004 bne     22E4790h          ///022E4790
022E477C E1A00004 mov     r0,r4
022E4780 E3A01001 mov     r1,1h
022E4784 E5854004 str     r4,[r5,4h]
022E4788 EBF47278 bl      2001170h          ///02001170
022E478C E5850000 str     r0,[r5]
022E4790 E1DD20B4 ldrh    r2,[r13,4h]
022E4794 E5951000 ldr     r1,[r5]
022E4798 E28D000C add     r0,r13,0Ch
022E479C E1A02082 mov     r2,r2,lsl 1h
022E47A0 EBF48EAB bl      2008254h          ///02008254
022E47A4 E1DD00B0 ldrh    r0,[r13]
022E47A8 E3500000 cmp     r0,0h
022E47AC 0A000008 beq     22E47D4h          ///022E47D4
022E47B0 E1DD10B4 ldrh    r1,[r13,4h]
022E47B4 E1DD00B2 ldrh    r0,[r13,2h]
022E47B8 E1DD20B6 ldrh    r2,[r13,6h]
022E47BC E5953000 ldr     r3,[r5]
022E47C0 E0811000 add     r1,r1,r0
022E47C4 E28D000C add     r0,r13,0Ch
022E47C8 E0831081 add     r1,r3,r1,lsl 1h
022E47CC E1A02082 mov     r2,r2,lsl 1h
022E47D0 EBF48E9F bl      2008254h          ///02008254
022E47D4 E1DD00B2 ldrh    r0,[r13,2h]
022E47D8 E3500000 cmp     r0,0h
022E47DC 0A00002A beq     22E488Ch          ///022E488C
022E47E0 E1DD20B4 ldrh    r2,[r13,4h]
022E47E4 E1DD10B6 ldrh    r1,[r13,6h]
022E47E8 E59F00B8 ldr     r0,=2324F70h
022E47EC E3A03000 mov     r3,0h
022E47F0 E0821001 add     r1,r2,r1
022E47F4 E2812006 add     r2,r1,6h
022E47F8 E1D0C0D0 ldrsb   r12,[r0]
022E47FC E28D1000 add     r1,r13,0h
022E4800 EA000003 b       22E4814h          ///022E4814
022E4804 E0810083 add     r0,r1,r3,lsl 1h
022E4808 E1D000B8 ldrh    r0,[r0,8h]
022E480C E2833001 add     r3,r3,1h
022E4810 E0822000 add     r2,r2,r0
022E4814 E153000C cmp     r3,r12
022E4818 BAFFFFF9 blt     22E4804h          ///022E4804
022E481C E1A01082 mov     r1,r2,lsl 1h
022E4820 E28D000C add     r0,r13,0Ch
022E4824 E3A02000 mov     r2,0h
022E4828 EBF48E9E bl      20082A8h          ///020082A8
022E482C E1DD20B2 ldrh    r2,[r13,2h]
022E4830 E1DD10B4 ldrh    r1,[r13,4h]
022E4834 E5953000 ldr     r3,[r5]
022E4838 E28D000C add     r0,r13,0Ch
022E483C E0831081 add     r1,r3,r1,lsl 1h
022E4840 E1A02082 mov     r2,r2,lsl 1h
022E4844 EBF48E82 bl      2008254h          ///02008254
022E4848 E59F0058 ldr     r0,=2324F70h
022E484C E28D1000 add     r1,r13,0h
022E4850 E1D000D0 ldrsb   r0,[r0]
022E4854 E1DDC0B2 ldrh    r12,[r13,2h]
022E4858 E1DD20B4 ldrh    r2,[r13,4h]
022E485C E2800004 add     r0,r0,4h
022E4860 E1A00080 mov     r0,r0,lsl 1h
022E4864 E19100B0 ldrh    r0,[r1,r0]
022E4868 E08C1002 add     r1,r12,r2
022E486C E1DD30B6 ldrh    r3,[r13,6h]
022E4870 E040200C sub     r2,r0,r12
022E4874 E5955000 ldr     r5,[r5]
022E4878 E0831001 add     r1,r3,r1
022E487C E28D000C add     r0,r13,0Ch
022E4880 E0851081 add     r1,r5,r1,lsl 1h
022E4884 E1A02082 mov     r2,r2,lsl 1h
022E4888 EBF48E71 bl      2008254h          ///02008254
022E488C E28D000C add     r0,r13,0Ch
022E4890 EBF48E8B bl      20082C4h          ///020082C4
022E4894 EBF48E3E bl      2008194h          ///02008194
022E4898 E1A00004 mov     r0,r4
022E489C E28DD054 add     r13,r13,54h
022E48A0 E8BD8030 pop     r4,r5,r15
022E48A4 02319694 
022E48A8 02324F70 


/*
    fun_02008168
*/
02008168 E92D4008 push    r3,r14
0200816C E59F0018 ldr     r0,=20AF368h      //
02008170 EBFFEACF bl      2002CB4h          ///fun_02002CB4
02008174 EBFFE901 bl      2002580h          ///fun_02002580
02008178 E3A01013 mov     r1,13h
0200817C EBFFE99D bl      20027F8h          ///020027F8
02008180 E59F1008 ldr     r1,=20AF360h
02008184 E5810004 str     r0,[r1,4h]
02008188 E8BD8008 pop     r3,r15
0200818C 020AF368
02008190 020AF360

/*
    fun_02002CB4
*/
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

/*
    fun_02002580
*/
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
020025E8 022B966C
020025EC 0229AFCC
020025F0 0229AFC0
020025F4 0229AFE4

/*
    fun_020027F8
*/
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
02002824 E92D4070 push    r4-r6,r14
02002828 E59F1024 ldr     r1,=22B966Ch
0200282C E1A06000 mov     r6,r0
02002830 E5915004 ldr     r5,[r1,4h]
02002834 E1A00005 mov     r0,r5
02002838 EB01DCB3 bl      2079B0Ch
0200283C E1A04000 mov     r4,r0
02002840 E1A00005 mov     r0,r5
02002844 E1A01006 mov     r1,r6
02002848 EB01DC85 bl      2079A64h
0200284C E1A00004 mov     r0,r4
02002850 E8BD8070 pop     r4-r6,r15
02002854 022B966C 

/*
    fun_02008204
*/