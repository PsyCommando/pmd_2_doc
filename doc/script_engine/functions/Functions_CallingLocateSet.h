/*
    SSE = 1,
    SSS = 2,
    SSA = 3,
*/



/*
    fun_022E59E8 22E59E8h
*/
022E59E8 E92D4010 push    r4,r14
022E59EC E1A04000 mov     r4,r0
022E59F0 E1A01004 mov     r1,r4
022E59F4 E3A00001 mov     r0,1h
022E59F8 EB00647F bl      22FEBFCh
022E59FC E1A01004 mov     r1,r4
022E5A00 E3A00001 mov     r0,1h         //SSE
022E5A04 E3A02000 mov     r2,0h
022E5A08 EBFFFCB1 bl      22E4CD4h
022E5A0C E8BD8010 pop     r4,r15


/*
    fun_022E5A10 22E5A10h
        Script Located for "Station" SSS
        Called by supervision_Station, and several other commands!
*/
022E5A10 E92D4010 push    r4,r14
022E5A14 E1A04000 mov     r4,r0         //R4 = PtrNextWordScript
022E5A18 E1A01004 mov     r1,r4         //R1 = PtrNextWordScript
022E5A1C E3A00002 mov     r0,2h         //R0 = 2
022E5A20 EB006475 bl      22FEBFCh      //022FEBFC
{
    022FEBFC E92D41F0 push    r4-r8,r14
    022FEC00 E59F205C ldr     r2,=2324D08h
    022FEC04 E1A07000 mov     r7,r0         //R7 = ScrType?
    022FEC08 E5924000 ldr     r4,[r2]       //R4 = Ptr    
    022FEC0C E1A06001 mov     r6,r1         //R6 = PtrNextWordScript
    022FEC10 E3A05000 mov     r5,0h         //R5 = 0
    022FEC14 E3E08000 mvn     r8,0h         //R8 = 0
    022FEC18 E1D400F2 ldrsh   r0,[r4,2h]    //R0 = 
    022FEC1C E1500008 cmp     r0,r8
    if( R0 == R8 )
        022FEC20 0A000008 beq     22FEC48h  ///022FEC48
    022FEC24 E1D400F4 ldrsh   r0,[r4,4h]
    022FEC28 E1500007 cmp     r0,r7
    if(R0 != R7)
        022FEC2C 1A000005 bne     22FEC48h  ///022FEC48
    022FEC30 E1560008 cmp     r6,r8
    022FEC34 11D400D6 ldrnesb r0,[r4,6h]
    022FEC38 11560000 cmpne   r6,r0
    022FEC3C 1A000001 bne     22FEC48h
    022FEC40 E1A00005 mov     r0,r5
    022FEC44 EB000007 bl      22FEC68h
    ///022FEC48
    022FEC48 E2850001 add     r0,r5,1h
    022FEC4C E1A00800 mov     r0,r0,lsl 10h
    022FEC50 E1A05840 mov     r5,r0,asr 10h
    022FEC54 E3550020 cmp     r5,20h
    022FEC58 E2844020 add     r4,r4,20h
    022FEC5C BAFFFFED blt     22FEC18h
    022FEC60 E8BD81F0 pop     r4-r8,r15
    022FEC64 02324D08
}
022E5A24 E1A01004 mov     r1,r4
022E5A28 E3A00002 mov     r0,2h         //SSS
022E5A2C E3A02000 mov     r2,0h
022E5A30 EBFFFCA7 bl      22E4CD4h      //022E4CD4 ScriptLocateSet
022E5A34 E8BD8010 pop     r4,r15

/*
    fun_022E5A38 22E5A38h
*/
022E5A38 E59FC00C ldr     r12,=22E4CD4h
022E5A3C E1A01000 mov     r1,r0
022E5A40 E3A00003 mov     r0,3h         //SSA
022E5A44 E3A02000 mov     r2,0h
022E5A48 E12FFF1C bx      r12
022E5A4C 022E4CD4

/*
    fun_022E5A50 22E5A50h
*/
022E5A50 E59FC00C ldr     r12,=22E4CD4h
022E5A54 E1A01000 mov     r1,r0
022E5A58 E3A00003 mov     r0,3h         //SSA
022E5A5C E3A02001 mov     r2,1h
022E5A60 E12FFF1C bx      r12
022E5A64 022E4CD4