
0204D780 E92D4008 push    r3,r14
0204D784 EBFEEA77 bl      2008168h
0204D788 E59F0008 ldr     r0,=22AB4B0h
0204D78C EBFEEA9C bl      2008204h
0204D790 EBFEEA7F bl      2008194h
0204D794 E8BD8008 pop     r3,r15
0204D798 022AB4B0 eoreq   r11,r10,0B0000000h


0202593C E92D4008 push    r3,r14
02025940 EBFF8A08 bl      2008168h
02025944 E59F0034 ldr     r0,=22A7A0Ch
02025948 EBFF8A2D bl      2008204h
0202594C E59F002C ldr     r0,=22A7A0Ch
02025950 E59F102C ldr     r1,=209AAD8h
02025954 EBFF8A2D bl      2008210h
02025958 E59F0020 ldr     r0,=22A7A0Ch
0202595C E3A01FCF mov     r1,33Ch
02025960 E3A02000 mov     r2,0h
02025964 EBFF8A4F bl      20082A8h
02025968 E59F0010 ldr     r0,=22A7A0Ch
0202596C E59F1014 ldr     r1,=22A7A08h
02025970 E3A02002 mov     r2,2h
02025974 EBFF8A36 bl      2008254h
02025978 EBFF8A05 bl      2008194h
0202597C E8BD8008 pop     r3,r15
02025980 022A7A0C eoreq   r7,r10,0C000h
02025984 0209AAD8 andeq   r10,r9,0D8000h
02025988 022A7A08 eoreq   r7,r10,8000h

//
02052358 E92D4008 push    r3,r14
0205235C E59F0024 ldr     r0,=20B09C0h              //Static pointer to struct containing filebuff ptr and file size!
02052360 E59F1024 ldr     r1,=20A31B8h              //"rom0:/BALANCE/monster.md"
02052364 E3A02000 mov     r2,0h
02052368 EBFEDA33 bl      2008C3Ch                  //LoadFileFromRom
0205236C EB000218 bl      2052BD4h                
{
    02052BD4 E92D4008 push    r3,r14
    02052BD8 E59F004C ldr     r0,=20B09C8h          //Pointer to static "st_n2m_j.bin" filebuffer pointer and size
    02052BDC EBFEC195 bl      2003238h              //IsPointerNull(R0 = Ptr)
    {
        02003238 E5900000 ldr     r0,[r0]
        0200323C E3500000 cmp     r0,0h
        02003240 03A00001 moveq   r0,1h
        02003244 13A00000 movne   r0,0h
        02003248 E20000FF and     r0,r0,0FFh
        0200324C E12FFF1E bx      r14
    }
    02052BE0 E3500000 cmp     r0,0h
    02052BE4 0A000005 beq     2052C00h
    02052BE8 E59F003C ldr     r0,=20B09C8h          //Pointer to static "st_n2m_j.bin" filebuffer pointer and size
    02052BEC EBFEC18D bl      2003228h
    02052BF0 E59F0034 ldr     r0,=20B09C8h          //Pointer to static "st_n2m_j.bin" filebuffer pointer and size
    02052BF4 E59F1034 ldr     r1,=20A3210h
    02052BF8 E3A02000 mov     r2,0h
    02052BFC EBFED80E bl      2008C3Ch                //LoadFileFromRom(R1=filepath)
    02052C00 E59F002C ldr     r0,=20B09D0h
    02052C04 EBFEC18B bl      2003238h              //IsPointerNull(R0 = Ptr)
    02052C08 E3500000 cmp     r0,0h
    02052C0C 08BD8008 popeq   r3,r15
    02052C10 E59F001C ldr     r0,=20B09D0h
    02052C14 EBFEC183 bl      2003228h
    02052C18 E59F0014 ldr     r0,=20B09D0h
    02052C1C E59F1014 ldr     r1,=20A322Ch
    02052C20 E3A02000 mov     r2,0h
    02052C24 EBFED804 bl      2008C3Ch                //LoadFileFromRom(R1=filepath)
    02052C28 E8BD8008 pop     r3,r15
    02052C2C 020B09C8
    02052C30 020A3210
    02052C34 020B09D0
    02052C38 020A322C
}
02052370 E59F0018 ldr     r0,=20B09B4h              //Address to zeroed out area
02052374 E590100C ldr     r1,[r0,0Ch]               //address:0x20B09C0
02052378 E5801004 str     r1,[r0,4h]                //address:0x20B09B8
0205237C E2811008 add     r1,r1,8h
02052380 E5801000 str     r1,[r0]
02052384 E8BD8008 pop     r3,r15
02052388 020B09C0 
0205238C 020A31B8 
02052390 020B09B4 