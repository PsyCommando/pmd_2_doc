//Watchpoints
[02324DE8]! //Watch what sets the value used in the switch statement below!

//
022F7BC4 E92D4008 push    r3,r14
022F7BC8 E59F00E8 ldr     r0,=2324CF4h
022F7BCC E5903000 ldr     r3,[r0]
022F7BD0 E59300F0 ldr     r0,[r3,0F0h]
022F7BD4 E3500001 cmp     r0,1h
022F7BD8 1A00002F bne     22F7C9Ch //022F7C9C
022F7BDC E59300F4 ldr     r0,[r3,0F4h]
022F7BE0 E3500009 cmp     r0,9h
022F7BE4 908FF100 addls   r15,r15,r0,lsl 2h
022F7BE8 EA000027 b       22F7C8Ch ///022F7C8C
022F7BEC EA000026 b       22F7C8Ch
022F7BF0 EA000025 b       22F7C8Ch
022F7BF4 EA000024 b       22F7C8Ch
022F7BF8 EA000005 b       22F7C14h ///022F7C14
022F7BFC EA000007 b       22F7C20h
022F7C00 EA000009 b       22F7C2Ch
022F7C04 EA00000E b       22F7C44h
022F7C08 EA000013 b       22F7C5Ch
022F7C0C EA000015 b       22F7C68h
022F7C10 EA000019 b       22F7C7Ch
///22F7C14h
022F7C14 E1D30FFA ldrsh   r0,[r3,0FAh]
022F7C18 EBFFB60B bl      22E544Ch              //022E544C
022F7C1C EA00001A b       22F7C8Ch
///22F7C20h
022F7C20 E1D30FFA ldrsh   r0,[r3,0FAh]
022F7C24 EBFFB608 bl      22E544Ch
022F7C28 EA000017 b       22F7C8Ch
///22F7C2Ch
022F7C2C E2831C01 add     r1,r3,100h
022F7C30 E1D30FFA ldrsh   r0,[r3,0FAh]
022F7C34 E1D120D4 ldrsb   r2,[r1,4h]
022F7C38 E28310FC add     r1,r3,0FCh
022F7C3C EBFFB636 bl      22E551Ch
022F7C40 EA000011 b       22F7C8Ch
///22F7C44h
022F7C44 E2831C01 add     r1,r3,100h
022F7C48 E1D30FFA ldrsh   r0,[r3,0FAh]
022F7C4C E1D120D4 ldrsb   r2,[r1,4h]
022F7C50 E28310FC add     r1,r3,0FCh
022F7C54 EBFFB697 bl      22E56B8h
022F7C58 EA00000B b       22F7C8Ch
///22F7C5Ch
022F7C5C E28300FC add     r0,r3,0FCh
022F7C60 EBFFB6F9 bl      22E584Ch
022F7C64 EA000008 b       22F7C8Ch
///22F7C68h
022F7C68 E2831C01 add     r1,r3,100h
022F7C6C E1D100F6 ldrsh   r0,[r1,6h]
022F7C70 E1D110F8 ldrsh   r1,[r1,8h]
022F7C74 EBFFB724 bl      22E590Ch
022F7C78 EA000003 b       22F7C8Ch
///22F7C7Ch
022F7C7C E1D30FFA ldrsh   r0,[r3,0FAh]
022F7C80 E28310FC add     r1,r3,0FCh
022F7C84 E3A02000 mov     r2,0h
022F7C88 EBFFB623 bl      22E551Ch
///22F7C8Ch
022F7C8C E59F0024 ldr     r0,=2324CF4h
022F7C90 E3A01002 mov     r1,2h
022F7C94 E5900000 ldr     r0,[r0]
022F7C98 E58010F0 str     r1,[r0,0F0h]
///22F7C9Ch
022F7C9C E59F0014 ldr     r0,=2324CF4h
022F7CA0 E5900000 ldr     r0,[r0]
022F7CA4 E59010F0 ldr     r1,[r0,0F0h]
022F7CA8 E3510000 cmp     r1,0h
022F7CAC 18BD8008 popne   r3,r15
022F7CB0 EBFF9582 bl      22DD2C0h
022F7CB4 E8BD8008 pop     r3,r15