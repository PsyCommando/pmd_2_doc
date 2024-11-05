/*
    022F1F3C 22F1F3Ch
        Seems to handle the 2 first values in the BMA file? 
        Is called on each redraw of the background.
        r0 = PtrRes2
        r1 = PtrRes1
*/
022F1F3C E92D40F8 push    r3-r7,r14
022F1F40 E59F20FC ldr     r2,=2324CC0h              //r2 = AddressUnkPointerTbl_2324CC0
022F1F44 E1A07000 mov     r7,r0                     //r7 = PtrRes2 (was 0x27E3698)
022F1F48 E5920000 ldr     r0,[r2]                   //r0 = [r2] (was 0x210CA44) 
022F1F4C E1A06001 mov     r6,r1                     //r6 = PtrRes1 (was 0x27E3690)
022F1F50 E3500000 cmp     r0,0h
if( r0 != 0  )
{
    022F1F54 15922004 ldrne   r2,[r2,4h]            //r2 = [r2,4h] (was 0x02115800)
    022F1F58 13E01000 mvnne   r1,0h                 //r1 = 0xFFFFFFFF
    022F1F5C 11D240F0 ldrnesh r4,[r2]               //r4 = lvlid (was 0x120)
    
    022F1F68 E3A0300C mov     r3,0Ch                    //r3 = 12
    022F1F6C E1A01007 mov     r1,r7                     //r1 = PtrRes2
    022F1F70 E1A02006 mov     r2,r6                     //r2 = PtrRes1
    022F1F74 E1640384 smulbb  r4,r4,r3                  //r4 = lvlid * 12
    022F1F78 E59F50C8 ldr     r5,=20A5488h              //r5 = LevelTableBeg
    022F1F7C EBFFF545 bl      22EF498h                  ///022EF498 HandleMapResolution?? Shift them by 11 left!
    {
        022EF498 E3A03000 mov     r3,0h                     //r3 = 0
        022EF49C E5813000 str     r3,[r1]                   //[PtrRes2] = 0
        022EF4A0 E5813004 str     r3,[r1,4h]                //[PtrRes2 + 4] = 0
        022EF4A4 E5D011E0 ldrb    r1,[r0,1E0h]              //r1 = BMA_Width
        022EF4A8 E1A01581 mov     r1,r1,lsl 0Bh             //r1 = BMA_Width * 2048
        022EF4AC E5821000 str     r1,[r2]                   //[PtrRes1] = (BMA_Width * 2048) //(r2 was 027E3690)
        022EF4B0 E5D001E1 ldrb    r0,[r0,1E1h]              //r0 = BMA_Height
        022EF4B4 E1A00580 mov     r0,r0,lsl 0Bh             //r0 = BMA_Height * 2048
        022EF4B8 E5820004 str     r0,[r2,4h]                //[PtrRes1 + 4] = (BMA_Height * 2048) //(r2 + 4 was 027E3694)
        022EF4BC E12FFF1E bx      r14
    }
    022F1F80 E5970000 ldr     r0,[r7]                   //r0 = [PtrRes2] (Will always be 0 here)
    022F1F84 E2800902 add     r0,r0,8000h               //r0 = r0 + 0x8000
    022F1F88 E5870000 str     r0,[r7]                   //[PtrRes2] = (r0 + 0x8000)
    022F1F8C EB007D32 bl      231145Ch                  ///0231145C
    {
        0231145C E59F0014 ldr     r0,=2324E00h
        02311460 E5900000 ldr     r0,[r0]
        02311464 E3500000 cmp     r0,0h
        if( r0 != 0 )
        {
            02311468 13A00001 movne   r0,1h             //r0 = 1
        }
        else
        {
            0231146C 03A00000 moveq   r0,0h             //r0 = 0
        }
        02311470 E20000FF and     r0,r0,0FFh            //r0 = r0 & 0xFF
        02311474 E12FFF1E bx      r14
        02311478 02324E00 
    }
    022F1F90 E3500000 cmp     r0,0h
    if( r0 != 0 )
    {
        022F1F94 15970004 ldrne   r0,[r7,4h]            //r0 = [PtrRes2,4h] (Will always be 0 here)
        022F1F98 12800A12 addne   r0,r0,12000h          //r0 = r0 + 0x12000
        022F1F9C 15870004 strne   r0,[r7,4h]            //[PtrRes2,4h] = r0
        022F1FA0 1A000006 bne     22F1FC0h              ///022F1FC0
    }
    else
    {
        022F1FA4 EB007E6C bl      231195Ch                  ///0231195C 
        {
            0231195C E59F0014 ldr     r0,=2324E08h          //r0 = 0x2324E08 
            02311960 E5900000 ldr     r0,[r0]               //r0 = [0x2324E08] //Some pointer
            02311964 E3500000 cmp     r0,0h
            if( r0 != 0 )
            {
                02311968 13A00001 movne   r0,1h             //r0 = 1
            }
            else
            {
                0231196C 03A00000 moveq   r0,0h             //r0 = 0
            }
            02311970 E20000FF and     r0,r0,0FFh            //r0 = & 0xFF
            02311974 E12FFF1E bx      r14                   //Return
        }
        022F1FA8 E3500000 cmp     r0,0h
        022F1FAC E5970004 ldr     r0,[r7,4h]                //r0 = [PtrRes2,4h]
        if( r0_at_022F1FA8 != 0 )
        {
            022F1FB0 12800A16 addne   r0,r0,16000h          //r0 = r0 + 0x16000 (if we convert back to an integer, (0x16000 >> 8) we get 0x160, which is a number of pixels? Then divided by 8, it gives us a number of tile 0x2C)
            022F1FB4 15870004 strne   r0,[r7,4h]
        }
        else
        {
            022F1FB8 02800A06 addeq   r0,r0,6000h           //r0 = r0 + 0x6000 (0x6000 >> 8 == 0x60, 0x60 / 8 == 0xC)
            022F1FBC 05870004 streq   r0,[r7,4h]
        }
    }
    ///022F1FC0
    022F1FC0 E5960000 ldr     r0,[r6]                   //r0 = (BMA_Width << 11)
    022F1FC4 E2400902 sub     r0,r0,8000h               //r0 = (BMA_Width << 11) - 0x8000 ( 0x8000 >> 8 == 0x80 pixels, 0x80 / 8 == 0x10 tiles )
    022F1FC8 E5860000 str     r0,[r6]                   //[r6] = (BMA_Width << 11) - 0x8000
    022F1FCC E5960004 ldr     r0,[r6,4h]                //r0 = (BMA_Height << 11)
    022F1FD0 E2400A06 sub     r0,r0,6000h               //r0 = (BMA_Height << 11) - 0x6000  (0x6000 >> 8 == 0x60 pixels, 0x60 / 8 == 0xC tiles )
    022F1FD4 E5860004 str     r0,[r6,4h]                //[r6,4h] = (BMA_Height << 11) - 0x6000
    022F1FD8 E19500F4 ldrsh   r0,[r5,r4]                //r0 = [LevelTableBeg + (lvlid * 12)] (unkcategory?) (was 0x9)
    022F1FDC E350000A cmp     r0,0Ah                    
    if( unkcategory? != 0xA )
    {
        022F1FE0 1A00000B bne     22F2014h              ///022F2014
    }
    else
    {
        022F1FE4 E5970000 ldr     r0,[r7]                   //r0 = [r7] (was 0x8000)
        022F1FE8 E2800B06 add     r0,r0,1800h               //r0 = r0 + 0x1800 (0x1800 >> 8 == 0x18 pixels, 0x18 / 8 == 3 tiles )
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
    }
    ///022F2014
    022F2014 E5971000 ldr     r1,[r7]                   //r1 = [r7] (was 0x8000)
    022F2018 E5960000 ldr     r0,[r6]                   //r0 = ((BMA_Width << 11) - 0x8000)
    022F201C E1510000 cmp     r1,r0
    //If the value at [r7] is bigger than the one at [r6], we clamp the value at [r6] to [r7]
    if( r1 > r0 )
    {
        022F2020 C5861000 strgt   r1,[r6]               //[r6] = r1
    }
    022F2024 E5971004 ldr     r1,[r7,4h]                //r1 = [r7,4h] (was 0x12000)
    022F2028 E5960004 ldr     r0,[r6,4h]                //r0 = ((BMA_Height << 11) - 0x6000) //(was 0x1E000)
    022F202C E1510000 cmp     r1,r0
    //If the value at [r7,4h] is bigger than the one at [r6,4h], we clamp the value at [r6,4h] to [r7,4h]
    if( r1 > r0 )               
    {
        022F2030 C5861004 strgt   r1,[r6,4h]            //[r6,4h] = r1
    }
    022F2034 E3A00001 mov     r0,1h                     //r0 = 1
    022F2038 E8BD80F8 pop     r3-r7,r15
}
else if( r0 == 0 || r4 == 0xFFFFFFFF )
{
    022F1F60 11540001 cmpne   r4,r1
    022F1F64 0A000034 beq     22F203Ch              ///022F203C
}
///022F203C
022F203C E3A00000 mov     r0,0h                     //r0 = 0
022F2040 E8BD80F8 pop     r3-r7,r15
022F2044 02324CC0 
022F2048 020A5488 