
///
///
///
0205A340 E92D4070 push    r4-r6,r14
0205A344 E24DD090 sub     r13,r13,90h
0205A348 E28DC04C add     r12,r13,4Ch
0205A34C E1A06000 mov     r6,r0
0205A350 E1A0E001 mov     r14,r1
0205A354 E1A05002 mov     r5,r2
0205A358 E3A04004 mov     r4,4h
0205A35C E8BE000F ldmia   [r14]!,r0-r3
0205A360 E8AC000F stmia   [r12]!,r0-r3
0205A364 E2544001 subs    r4,r4,1h
0205A368 1AFFFFFB bne     205A35Ch
0205A36C E59E1000 ldr     r1,[r14]
0205A370 E28D0040 add     r0,r13,40h
0205A374 E58C1000 str     r1,[r12]
0205A378 E5DD204D ldrb    r2,[r13,4Dh]
0205A37C E1A01005 mov     r1,r5
0205A380 E1DD45F0 ldrsh   r4,[r13,50h]
0205A384 EBFFE504 bl      205379Ch              ///fun_0205379C
0205A388 E5DD0052 ldrb    r0,[r13,52h]
0205A38C E59D1040 ldr     r1,[r13,40h]
0205A390 E1CD55B0 strh    r5,[r13,50h]
0205A394 E3500000 cmp     r0,0h
0205A398 05DD004D ldreqb  r0,[r13,4Dh]
0205A39C E58D105C str     r1,[r13,5Ch]
0205A3A0 05CD0052 streqb  r0,[r13,52h]
0205A3A4 0A000003 beq     205A3B8h
0205A3A8 E5DD0053 ldrb    r0,[r13,53h]
0205A3AC E3500000 cmp     r0,0h
0205A3B0 05DD004D ldreqb  r0,[r13,4Dh]
0205A3B4 05CD0053 streqb  r0,[r13,53h]
0205A3B8 E1A00004 mov     r0,r4
0205A3BC EBFFE0C1 bl      20526C8h
0205A3C0 E1A01000 mov     r1,r0
0205A3C4 E28D0000 add     r0,r13,0h
0205A3C8 EBFF2B98 bl      2025230h
0205A3CC E28D0000 add     r0,r13,0h
0205A3D0 E28D1086 add     r1,r13,86h
0205A3D4 E3A0200A mov     r2,0Ah
0205A3D8 EBFF2B73 bl      20251ACh
0205A3DC E3500000 cmp     r0,0h
0205A3E0 1A000005 bne     205A3FCh
0205A3E4 E1A00005 mov     r0,r5
0205A3E8 EBFFE0B6 bl      20526C8h
0205A3EC E1A01000 mov     r1,r0
0205A3F0 E28D0086 add     r0,r13,86h
0205A3F4 E3A0200A mov     r2,0Ah
0205A3F8 EBFF2BC5 bl      2025314h
0205A3FC E1D600F0 ldrsh   r0,[r6]
0205A400 E3E01000 mvn     r1,0h
0205A404 E1500001 cmp     r0,r1
0205A408 0A000002 beq     205A418h
0205A40C E28D104C add     r1,r13,4Ch
0205A410 EBFFEE59 bl      2055D7Ch
0205A414 EA000002 b       205A424h
0205A418 E28D004C add     r0,r13,4Ch
0205A41C EBFFEE2A bl      2055CCCh
0205A420 E1C600B0 strh    r0,[r6]
0205A424 E1D600F0 ldrsh   r0,[r6]
0205A428 E28DD090 add     r13,r13,90h
0205A42C E8BD8070 pop     r4-r6,r15