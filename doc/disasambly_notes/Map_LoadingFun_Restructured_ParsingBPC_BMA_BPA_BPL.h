
//Usually stored at 0x0210CA44
struct StructR10
{
    [r10]       = word set to 1 after allocating level data buffers.
    
    [r10,4h]   = //Table of 16, 12 bytes entries
    {
        uint16_t unk1;
        uint16_t unk2;
        uint32_t unk3;
        uint32_t unk4;
    }
    
    [r10,0C4h] = //0C4h -> Table with 4, 44 bytes entries
    {
        uint8_t unk1;
        uint8_t unk2;
        uint16_t unk3;
        uint16_t unk4;
        uint32_t unk5;
        uint32_t unk6;
        uint32_t unk7;
        uint32_t unk8;
        uint32_t unk9;
        uint32_t unk10;
        uint32_t unk11;
    }
    
    [r10,174h]  = PtrBPLBuff    //Raw file data allocated when parsing
    [r10,17Ch]  = PtrBMA1Buff?  //Raw file data allocated when parsing
    [r10,180h]  = PtrBMA2Buff?  //Raw file data allocated when parsing
    [r10,184h]  = PtrBPCBuff    //Raw file data allocated when parsing
    [r10,18Ch]  = PtrBMABuff    //Raw file data allocated when parsing
    
    [r10,194h] = //Table 12 bytes (0xC)
    
    [r10,1A0h] = //Table 24 bytes (0x18)
    
    [r10,1BCh] = 
    [r10,1BEh]  = Level ID
    
    [r10,1C0h]  = byte set to 0 after allocating level data buffer.
    [r10,1C4h]  = Nb of layers in BPC file.
    [r10,1C8h]  = PtrBPCIndexTblCopy //(was at address 0x210CC0C)
    
    [r10,1E0h]  = BMA_MapWidth  (uint8)
    [r10,1E1h]  = BMA_MapHeight (uint8)
    [r10,1E2h]  = BMA_unk3  (uint8)
    [r10,1E3h]  = BMA_unk4  (uint8)
    [r10,1E4h]  = BMA_unk5 (uint8)
    [r10,1E6h]  = BMA_unk6 (uint16)
    [r10,1E8h]  = BMA_unk7 (uint16)
    
    [r10,1EAh]  = BPL_nbpal
    [r10,1ECh]  = BPL_unk2
    
    [r10,1FCh]  = (set to 1 pretty often)(022EE6C8)
    
    [r10,200h]  = (was 3) (address 0x210CC44)
    [r10,204h]  = (was 0xC0)(address 0x210CC48)
    

    [r10,210h]  = (was 1) (address 0x210CC54)
    {
        [r10,220h]  = (BMA_Width * 8) (022EE90C)
        [r10,224h]  = (BMA_Height * 8) (022EE934)
    }
    
    [r10,244h]  = 
    [r10,248h]  = 
    [r10,24Ch]  = 
    [r10,250h]  = 
    [r10,254h]  = 
    [r10,258h]  = 
    [r10,25Ch]  = 
    [r10,260h]  = 
    
    [r10,2B8h]  = 
    [r10,2BAh]  = 
    [r10,2BBh]  = (boolean)
    
    //This is filled in fun_022EBC18
    struct StructWithDefaultConstants //( 28(0x1C) bytes long)
    {
        0x00 [r10,2BCh]  = //Constant loaded in fun_022EBC18
        0x02 [r10,2BEh]  =  //A quantity? Possibly used by BPA parsing!
        0x04 [r10,2C0h]  = //Probably the nb of layers inside the BPC/BMA/BPA file!!
        0x06 [r10,2C2h]  = (was 0)
        0x08 [r10,2C4h]  = (was 0xE) //Possibly the NB of palettes to parse (The value is actually a constant loaded from the table at 0x2320CD8, on line 022EBC2C)
        0x0A [r10,2C6h]  = //VRAM Bank index
        0x0C [r10,2C8h]  = 
        //Sub table
        0x0E [r10,2CAh]  = //First layer size? If non-zero  a buffer at r10,2DCh, is allocated.. Is multiplied by 0x12 when allocating a buffer
        0x10 [r10,2CCh]  = //Second layer size?
        0x12 [r10,2CEh]  = //Usually 0xBA //Multiplied by 256 when allocating BMA's last table buffer
        0x14 [r10,2D0h]  = //Usually 0x3E //Multiplied by 128 later on when allocating BMA buffers
        0x16 [r10,2D2h]  = //0
        0x18 [r10,2D4h]  = //Function pointer for parsing the last table of the bma??
    }
    [r10,2D8h]  = //Pointer to Last table from BMA (0xFF and zeros), ( size is 0xBA * 256 bytes) 
    
    //Table of pointers to layer buffers?
    [r10,2DCh] = //Dynamically allocated buffer tied to first layer bpc. The BPC Header is stored there apparently!
    [r10,2E0h] = //Dynamically allocated buffer tied to second layer bpc
    
    //Another table (Pointers to BMA layer entry buffers 0x1F00 long usually)
    [r0,2E4h] = //First layer
    [r0,2E8h] = //Second layer
    
    //Layer related table
    [r0,2ECh] = //First layer
    [r0,2F0h] =  //Second layer
}



struct StackStructure_InFun_022EC27C
{
    [r13]       = if [r10,2BCh] == 0 this is set to 6, otherwise its set to 15.
    
    [r13,1Ch]   = PtrCurBPLPalette
    
    [r13,28h]   = PtrBPLBuff
    [r13,34h]   = r10 + 200h
    [r13,38h]   = r10 + 0x200
    [r13,40h]   = PtrBPLHeaderCopy
    [r13,44h]   = BPC_ptruplayer
    [r13,48h]   = BPC_ptrlowlayer
}




//
//
//
    struct tsetconstants
    {
        //
        uint16_t unk_0x2BC;
        uint16_t unk_0x2BE;
        uint16_t unk_0x2CO; //Nb bpas?
        uint16_t unk_0x2C2;
        uint16_t unk_0x2C4; //Nb Palettes used?
        uint16_t unk_0x2C6;
        uint16_t unk_0x2C8; //Usually 0x400
        //
        uint16_t unk_0x2CA; //First layer size? Usually 0x200 or 0x400
        uint16_t unk_0x2CC;
        uint16_t unk_0x2CE; //Usually 0xBA
        uint16_t unk_0x2D0; //Usually 0x3E
        uint16_t unk_0x2D2;
        uint32_t unk_0x2D4; //Pointer to function handling tileset?
    };

//Independent map select default values (fun_02311074)
///StructWithDefaultConstants
{
    //02323920 - When levelid == 0xFFFFFFFF, 
    //           OR when level entry first value is 0,1,2,3,4,5, bigger than 12!
    {
        0x0001, 0x0001, 0x0001, 0x0000, 0x000E, 0x0000, 0x0400, 
        0x0400, 0x0000, 0x00BA, 0x003E, 0x0000, 0x00000000, 
    },

    //0232393C - When level entry first value is 6,7,8,9,10
    {
        0x0001, 0x0000, 0x0002, 0x0000, 0x000E, 0x0000, 0x0400, 
        0x0200, 0x0400, 0x00BA, 0x003E, 0x0000, 0x00000000, 
    },

    //02323958 - When level entry first value is 11,12
    {
        0x0001, 0x0001, 0x0001, 0x0000, 0x000E, 0x0000, 0x0400, 
        0x0400, 0x0000, 0x00BA, 0x003E, 0x0000, 0x00000000, 
    },
};

//Ground map select default values (fun_022F12C0)
{
    //02320CD8 - When levelid == 0xFFFFFFFF
    //           OR when level entry first value is 0,1,2,3,4,5, bigger than 12!
    {
        0x0000, 0x0001, 0x0001, 0x0000, 0x000E, 0x0000, 0x0400, 
        0x0400, 0x0000, 0x00BA, 0x003E, 0x0000, 0x022F1800,   
    },
    
    //02320CF4 - When level entry first value == 6,7,8,9,10
    {
        0x0000, 0x0000, 0x0002, 0x0000, 0x000E, 0x0000, 0x0400, 
        0x0200, 0x0400, 0x00BA, 0x003E, 0x0000, 0x022F1800,  
    },
    
    //02320D10 - When level entry first value == 11,12
    {
        0x0000, 0x0001, 0x0001, 0x0000, 0x000E, 0x0000, 0x0400, 
        0x0400, 0x0000, 0x00BA, 0x003E, 0x0000, 0x022F1800,
    },
};

//Ground SubWorld Map Select default values (fun_02310074)
{
    //02323394 - When levelid == 0xFFFFFFFF
    //           OR level list entry first word == 1,2,3,4,5 OR bigger than 12!
    {
        0x0001, 0x0001, 0x0001, 0x0000, 0x000E, 0x0000, 0x0400, 
        0x0400, 0x0000, 0x00BA, 0x003E, 0x0000, 0x00000000, 
    },
    
    //023233B0 - When level list entry first word == 0,6,7,8,9,10,11,12
    {
        0x0001, 0x0000, 0x0002, 0x0000, 0x000E, 0x0000, 0x0400, 
        0x0200, 0x0400, 0x00BA, 0x003E, 0x0000, 0x00000000,   
    },
};

//
// 0x2320D2C 2320D2Ch LevelID to something else table!! (Referred on line 022F13C4)
//
Table_2320D2Ch[81]
{
    //{LevelID - unk0 - unk1 - unk2}
    { 0x0006, 0x02, 0x64, 0x00000000 }, 
    { 0x0007, 0x01, 0x64, 0x00000000 }, 
    { 0x0007, 0x01, 0x64, 0x00000000 }, 
    { 0x001B, 0x0E, 0x64, 0x00000000 }, 
    { 0x001E, 0x11, 0x64, 0x00000000 }, 
    { 0x0023, 0x15, 0x64, 0x00000000 }, 
    { 0x0030, 0x1D, 0x64, 0x00000000 }, 
    { 0x0038, 0x23, 0x64, 0x00000000 }, 
    { 0x003C, 0x26, 0x64, 0x00000000 }, 
    { 0x0044, 0x29, 0x64, 0x00000000 }, 
    { 0x004D, 0x36, 0x01, 0x00000000 }, 
    { 0x004E, 0x38, 0x01, 0x00000000 }, 
    { 0x004F, 0x3A, 0x01, 0x00000000 }, 
    { 0x0051, 0x36, 0x01, 0x00000000 }, 
    { 0x0052, 0x38, 0x01, 0x00000000 }, 
    { 0x0053, 0x3A, 0x01, 0x00000000 }, 
    { 0x0054, 0x37, 0x64, 0x00000000 }, 
    { 0x0055, 0x39, 0x64, 0x00000000 }, 
    { 0x0056, 0x3B, 0x64, 0x00000000 }, 
    { 0x0058, 0x47, 0x64, 0x00000000 }, 
    { 0x0059, 0x32, 0x64, 0x00000000 }, 
    { 0x005A, 0x33, 0x64, 0x00000000 }, 
    { 0x005B, 0x35, 0x64, 0x00000000 }, 
    { 0x005E, 0x3F, 0x01, 0x00000000 }, 
    { 0x005F, 0x3F, 0x64, 0x00000000 }, 
    { 0x0060, 0x40, 0x01, 0x00000000 }, 
    { 0x0061, 0x40, 0x01, 0x00000000 }, 
    { 0x0062, 0x40, 0x64, 0x00000000 }, 
    { 0x0063, 0x42, 0x64, 0x00000000 }, 
    { 0x0064, 0x42, 0x64, 0x00000000 }, 
    { 0x0066, 0x43, 0x64, 0x00000000 }, 
    { 0x0068, 0x7C, 0x01, 0x00000000 }, 
    { 0x006E, 0x83, 0x01, 0x00000000 }, 
    { 0x0072, 0x86, 0x01, 0x00000000 }, 
    { 0x0077, 0x8A, 0x01, 0x00000000 }, 
    { 0x007B, 0x8F, 0x01, 0x00000000 }, 
    { 0x0083, 0x93, 0x01, 0x00000000 }, 
    { 0x008B, 0x97, 0x01, 0x00000000 }, 
    { 0x008F, 0x9C, 0x01, 0x00000000 }, 
    { 0x0091, 0xA3, 0x01, 0x00000000 }, 
    { 0x009D, 0x6F, 0x01, 0x00000000 }, 
    { 0x009E, 0x71, 0x01, 0x00000000 }, 
    { 0x009F, 0x72, 0x01, 0x00000000 }, 
    { 0x00A0, 0x72, 0x01, 0x00000000 }, 
    { 0x00A1, 0x73, 0x01, 0x00000000 }, 
    { 0x00A2, 0x74, 0x01, 0x00000000 }, 
    { 0x00A3, 0x75, 0x01, 0x00000000 }, 
    { 0x00A5, 0x78, 0x01, 0x00000000 }, 
    { 0x0096, 0x67, 0x64, 0x00000000 }, 
    { 0x0097, 0x6B, 0x64, 0x00000000 }, 
    { 0x0098, 0x6C, 0x64, 0x00000000 }, 
    { 0x0099, 0x6D, 0x64, 0x00000000 }, 
    { 0x009A, 0x6F, 0x64, 0x00000000 }, 
    { 0x009B, 0x68, 0x64, 0x00000000 }, 
    { 0x00A8, 0x50, 0x64, 0x00000000 }, 
    { 0x00A9, 0x4C, 0x64, 0x00000000 }, 
    { 0x00AA, 0x4E, 0x64, 0x00000000 }, 
    { 0x00AB, 0x4A, 0x64, 0x00000000 }, 
    { 0x00AC, 0x52, 0x64, 0x00000000 }, 
    { 0x00AD, 0x56, 0x64, 0x00000000 }, 
    { 0x00AE, 0x54, 0x64, 0x00000000 }, 
    { 0x00AF, 0x48, 0x64, 0x00000000 }, 
    { 0x00B1, 0x2F, 0x64, 0x00000000 }, 
    { 0x00B2, 0x31, 0x64, 0x00000000 }, 
    { 0x00B3, 0x5B, 0x64, 0x00000000 }, 
    { 0x00B4, 0x5D, 0x64, 0x00000000 }, 
    { 0x00B5, 0x5E, 0x64, 0x00000000 }, 
    { 0x00B6, 0x5F, 0x64, 0x00000000 }, 
    { 0x00B7, 0x60, 0x64, 0x00000000 }, 
    { 0x00B8, 0x61, 0x64, 0x00000000 }, 
    { 0x00B9, 0x62, 0x64, 0x00000000 }, 
    { 0x00BA, 0x63, 0x64, 0x00000000 }, 
    { 0x00BB, 0x64, 0x64, 0x00000000 }, 
    { 0x00BC, 0x65, 0x64, 0x00000000 }, 
    { 0x00BD, 0x66, 0x64, 0x00000000 }, 
    { 0x00BE, 0x57, 0x64, 0x00000000 }, 
    { 0x00BF, 0x58, 0x64, 0x00000000 }, 
    { 0x00C0, 0x59, 0x64, 0x00000000 }, 
    { 0x00C1, 0x5A, 0x64, 0x00000000 }, 
    { 0x00C2, 0x5B, 0x64, 0x00000000 }, 
    { 0xFFFF, 0x00, 0x64, 0x00000000 }, 
};

//-----------------------------------------------------------------------------------
//fun_022EBC18 - Setup Default Constant Map Loading
//-----------------------------------------------------------------------------------
/*
    022EBC18 fun_022EBC18 22EBC18h 
        This function seems to assign a few variables that are used throughout the map loading process!
        (r0 = PtrLevelStruct, r1 = Defvaltbl? )
*/
022EBC18 E92D4FF8 push    r3-r11,r14
022EBC1C E24DD008 sub     r13,r13,8h
022EBC20 E1A0A000 mov     r10,r0                //r10 = PtrLevelStruct
022EBC24 E1A05001 mov     r5,r1                 //r5  = DefaultValTbl
022EBC28 E28A4FAF add     r4,r10,2BCh           //r4  = PtrStructWithDefaultConstants
022EBC2C E8B5000F ldmia   [r5]!,r0-r3           //
022EBC30 E8A4000F stmia   [r4]!,r0-r3           // Copy all 28 bytes of the default value table at (r10 + 0x2BC)
022EBC34 E8950007 ldmia   [r5],r0-r2            //
022EBC38 E8840007 stmia   [r4],r0-r2            //
022EBC3C E5DA02BC ldrb    r0,[r10,2BCh]         //r0 = [r10,2BCh] (Is 0 when default for GroundMap_Select are loaded, otherwise always 1!)
022EBC40 E28A4C02 add     r4,r10,200h           //r4 = r10 + 0x200
022EBC44 E3A06000 mov     r6,0h                 //r6 = CNT_Layer = 0
022EBC48 E3500000 cmp     r0,0h
if( [r10,2BCh] == 0 )
    022EBC4C 03A09006 moveq   r9,6h             //r9 = 6
else
    022EBC50 13A0900F movne   r9,0Fh            //r9 =  15    
022EBC54 E1D47BFE ldrsh   r7,[r4,0BEh]          //r7 = [r10,2BEh]
022EBC58 E1A05006 mov     r5,r6                 //r5 = 0
022EBC5C E3A08012 mov     r8,12h                //r8 = 18
022EBC60 EA00000D b       22EBC9Ch              ///022EBC9C v

///
///022EBC64 - LOOP_BEG - Allocates layer buffers!
///
while( CNT_Layer < [r10,2C0h] )
{
    ///022EBC9C
    022EBC9C E1D40CF0 ldrsh   r0,[r4,0C0h]              //r0 = [PtrLevelStruct,2C0h]
    022EBCA0 E1560000 cmp     r6,r0
    022EBCA4 BAFFFFEE blt     22EBC64h                  ///022EBC64 - LoopBeg ^
    ///022EBC64 - LoopBeg
    022EBC64 E08A0086 add     r0,r10,r6,lsl 1h          //r0 = PtrLevelStruct + (CNT_Layer * 2)
    022EBC68 E2800C02 add     r0,r0,200h                //r0 = (PtrLevelStruct + (CNT_Layer * 2)) + 0x200
    022EBC6C E1D01CFA ldrsh   r1,[r0,0CAh]              //r1 = [((PtrLevelStruct + (CNT_Layer * 2)) + 0x200) + 0xCA] //LayerSize?
    022EBC70 E3510000 cmp     r1,0h
    if( LayerSize <= 0 )
    {
        022EBC74 D08A0106 addle   r0,r10,r6,lsl 2h      //r0 = PtrLevelStruct + (CNT_Layer * 4)
        022EBC78 D58052DC strle   r5,[r0,2DCh]          //[r0,2DCh] = 0 //Set the corresponding layer ptr to null
        022EBC7C DA000004 ble     22EBC94h              ///022EBC94
    }
    else
    {
        022EBC80 E0000891 mul     r0,r1,r8              //r0 = LayerSize * 18
        022EBC84 E1A01009 mov     r1,r9                 //r1 =( 6 if [r10,2BCh] == 0, or 15 )
        022EBC88 EBF45538 bl      2001170h              ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements  ) 
        022EBC8C E08A1106 add     r1,r10,r6,lsl 2h      //r1 = r10 + (CNT_Layer * 4)
        022EBC90 E58102DC str     r0,[r1,2DCh]          //[(r10 + (CNT_Layer * 4)),2DCh] = AllocatedBuffer
    }
    ///022EBC94
    022EBC94 E2866001 add     r6,r6,1h                  //r6 = CNT_Layer + 1
    022EBC98 E2877001 add     r7,r7,1h                  //r7 = r7 + 1
}
022EBCA8 E3A01000 mov     r1,0h                     //r1 = 0
022EBCAC EA000003 b       22EBCC0h                  ///022EBCC0 v

///
///022EBCB0 - LOOP_BEG - Zero pointer to unused layers!
///
while( CNT_Layer < 2 )
{
    ///022EBCC0
    022EBCC0 E3560002 cmp     r6,2h
    022EBCC4 BAFFFFF9 blt     22EBCB0h                  ///022EBCB0
    ///022EBCB0
    022EBCB0 E08A0106 add     r0,r10,r6,lsl 2h          //r0 = PtrLevelStruct + (CNT_Layer * 4)
    022EBCB4 E58012DC str     r1,[r0,2DCh]              //[(r10 + (CNT_Layer * 4)),2DCh] = 0 //Set the corresponding layer ptr to the allocated buffer
    022EBCB8 E2866001 add     r6,r6,1h                  //r6 = CNT_Layer + 1
    022EBCBC E2877001 add     r7,r7,1h                  //r7 = r7 + 1

}
022EBCC8 E28A4C02 add     r4,r10,200h                   //r4 = PtrLevelStruct + 0x200
022EBCCC E3A07000 mov     r7,0h                         //r7 = cntr7 = 0
022EBCD0 E1D48BFE ldrsh   r8,[r4,0BEh]                  //r8 = [r4,0BEh] (constantunk1)
022EBCD4 E1A06007 mov     r6,r7                         //r6 = cntr7
022EBCD8 E59F51C4 ldr     r5,=2324CBCh                  //r5 = ptr_ptr_2324CBCh (Address pointer to buffer. Was 02145920)
022EBCDC E3A0B01C mov     r11,1Ch                       //r11 = 28 (layer entry size in bytes!)
022EBCE0 EA00001B b       22EBD54h                      ///022EBD54 v

///
///022EBCE4 - LOOP_Beg - Alloc BMA layer entries buffers!
///
while( cntr7 < NbLayers )
{
    ///022EBD54
    022EBD54 E1D40CF0 ldrsh   r0,[r4,0C0h]                      //r0 = ( [r10,2C0h] ) //NbLayers
    022EBD58 E1570000 cmp     r7,r0
    022EBD5C BAFFFFE0 blt     22EBCE4h                          ///022EBCE4 ^
    
    ///022EBCE4 - LOOP_BEG
    022EBCE4 E5DA02BC ldrb    r0,[r10,2BCh]                 //r0 = [PtrLevelStruct,2BCh] (constantunk0)
    022EBCE8 E3500000 cmp     r0,0h
    022EBCEC E5950000 ldr     r0,[r5]                       //r0 = [ptr_ptr_2324CBCh]
    //Seems to lookup depending on a boolean in the loaded constants a pointer in a set of entries.
    if( constantunk0 != 0 )
    {
        022EBCF0 1A000002 bne     22EBD00h                  ///022EBD00 v
        ///022EBD00 - Use next 2 entries table
        022EBD00 E2801038 add     r1,r0,38h                     //r1 = ptr_ptr_2324CBCh + 56 //(Skips first 2 layer entries)
        022EBD04 E3A0001C mov     r0,1Ch                        //r0 = 28 //Layer entry size in bytes
        022EBD08 E0201098 mla     r0,r8,r0,r1                   //r0 = ([PtrLevelStruct,2BEh] * 28) + ([ptr_ptr_2324CBCh] + 56)
        022EBD0C EBF47DFB bl      200B500h                      ///0200B500 (Returns the pointer at 0x18 in the entry we put the pointer to in R0)
        {
            ///Fun 0200B500
            0200B500 E5900018 ldr     r0,[r0,18h]               //r0 = [r0,18h] = some pointer in the entry
            0200B504 E12FFF1E bx      r14
        }
        ///Continue to 022EBD10..
    }
    else
    {
        ///We use the first 2 entry table
        022EBCF4 E0200B98 mla     r0,r8,r11,r0                  //r0 = ([PtrLevelStruct,2BEh] * 28) + [ptr_ptr_2324CBCh]
        022EBCF8 EBF47E00 bl      200B500h                      ///0200B500 (Returns the pointer at 0x18 in the entry we put the pointer to in R0)
        {
            ///Fun 0200B500
            0200B500 E5900018 ldr     r0,[r0,18h]               //r0 = [r0,18h] = some pointer in the entry
            0200B504 E12FFF1E bx      r14
        }
        022EBCFC EA000003 b       22EBD10h                      ///022EBD10 v
    }
    ///022EBD10
    022EBD10 E08A1107 add     r1,r10,r7,lsl 2h                  //r1 = PtrLevelStruct + (cntr7 * 4)
    022EBD14 E58102EC str     r0,[r1,2ECh]                      //[(PtrLevelStruct + (cntr7 * 4)) + 2ECh] = r0 //PointerFrom default const table!
    022EBD18 E08A0087 add     r0,r10,r7,lsl 1h                  //r0 = PtrLevelStruct + (cntr7 * 2)
    022EBD1C E2800C02 add     r0,r0,200h                        //r0 = (PtrLevelStruct + (cntr7 * 2)) + 0x200
    022EBD20 E1D00CFA ldrsh   r0,[r0,0CAh]                      //r0 = [(PtrLevelStruct + (cntr7 * 2) + 200h),CAh] //[r10,2CAh] AKA LayerSize!
    022EBD24 E3500000 cmp     r0,0h
    if( LayerSize <= 0 )
    {
        022EBD28 D08A0107 addle   r0,r10,r7,lsl 2h              //r0 = PtrLevelStruct + (cntr7 * 4)
        022EBD2C D58062E4 strle   r6,[r0,2E4h]                  //[(PtrLevelStruct + (cntr7 * 4)),2E4h] = 0 
        022EBD30 DA000005 ble     22EBD4Ch                      ///022EBD4C v
    }
    else
    {
        022EBD34 E1D40DF0 ldrsh   r0,[r4,0D0h]                  //r0 = [PtrLevelStruct,2D0h] //usually 0x3E
        022EBD38 E1A01009 mov     r1,r9                         //r1 = (6 if [r10,2BCh] == 0, or 15 )
        022EBD3C E1A00380 mov     r0,r0,lsl 7h                  //r0 = [PtrLevelStruct,2D0h] * 128
        022EBD40 EBF4550A bl      2001170h                      ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
        022EBD44 E08A1107 add     r1,r10,r7,lsl 2h              //r1 = PtrLevelStruct + (cntr7 * 4)
        022EBD48 E58102E4 str     r0,[r1,2E4h]                  //[(PtrLevelStruct + (cntr7 * 4)),2E4h] = PtrAllocatedBuffer
    }
    ///022EBD4C
    022EBD4C E2877001 add     r7,r7,1h                          //r7 = cntr7 + 1
    022EBD50 E2888001 add     r8,r8,1h                          //r8 = r8 + 1    
}
022EBD60 E3A01000 mov     r1,0h                             //r1 = 0
022EBD64 EA000003 b       22EBD78h                          ///022EBD78 v

///
///022EBD68 - LOOP_Beg - Zero pointers of unused layers!
///
while( cntr7 < 2 )
{
    ///022EBD78
    022EBD78 E3570002 cmp     r7,2h
    022EBD7C BAFFFFF9 blt     22EBD68h                          ///022EBD68 ^
    ///022EBD68 - LOOP_Beg
    022EBD68 E08A0107 add     r0,r10,r7,lsl 2h                  //r0 = PtrLevelStruct + (cntr7 * 4)
    022EBD6C E58012EC str     r1,[r0,2ECh]                      //[(r10 + (cntr7 * 4)),2ECh] = 0
    022EBD70 E58012E4 str     r1,[r0,2E4h]                      //[(r10 + (cntr7 * 4)),2E4h] = 0
    022EBD74 E2877001 add     r7,r7,1h                          //r7 = cntr7 + 1
}

022EBD80 E59A02D4 ldr     r0,[r10,2D4h]                 //r0 = [PtrLevelStruct,2D4h] // PtrFun : Function pointer, usually 0 or 0x22F1800
022EBD84 E3500000 cmp     r0,0h
if( PtrFun == 0 )
{
    022EBD88 03A00000 moveq   r0,0h                     //r0 = 0
    022EBD8C 0A000004 beq     22EBDA4h                  ///022EBDA4 v
}
else
{
    022EBD90 E28A0C02 add     r0,r10,200h               //r0 = PtrLevelStruct + 0x200
    022EBD94 E1D00CFE ldrsh   r0,[r0,0CEh]              //r0 = [r0,0CEh] //r0 = [r10,2CEh] //Usually 0xBA
    022EBD98 E1A01009 mov     r1,r9                     //r1 = (6 if [r10,2BCh] == 0, or 15 )
    022EBD9C E1A00400 mov     r0,r0,lsl 8h              //r0 = r0 * 256 //0xBA * 256 = 0xBA00 //Size of the BMA last table??
    022EBDA0 EBF454F2 bl      2001170h                  ///MemAlloc Possibly CAlloc( size_t sizeelement, size_t nbelements ) 
}
///022EBDA4
022EBDA4 E58A02D8 str     r0,[r10,2D8h]                 //[r10,2D8h] = r0
022EBDA8 E1A0000A mov     r0,r10                        //r0 = PtrLevelStruct
022EBDAC EB00006B bl      22EBF60h                      ///022EBF60 ZeroOutserveral buffers?
{
    022EBF60 E92D40F8 push    r3-r7,r14
    022EBF64 E1A07000 mov     r7,r0                     //r7 = PtrLevelStruct
    022EBF68 E3A06000 mov     r6,0h                     //r6 = Cntr6 = 0
    022EBF6C E28750C4 add     r5,r7,0C4h                //r5 = PtrLevelStruct + 0xC4 //[r10,0C4h]
    022EBF70 E3A0402C mov     r4,2Ch                    //r4 = 44
    ///022EBF74 - LOOP_Beg
    do while( Cntr6 < 4 )
    {
        022EBF74 E0205496 mla     r0,r6,r4,r5               //r0 = Cntr6 * 44 + (PtrLevelStruct + 0xC4)
        022EBF78 E2800008 add     r0,r0,8h                  //r0 = (Cntr6 * 44 + (PtrLevelStruct + 0xC4)) + 8
        022EBF7C EBF45CA9 bl      2003228h                  ///02003228
        {
            02003228 E3A01000 mov     r1,0h                 //r1 = 0
            0200322C E5801000 str     r1,[r0]               //[r0] = 0
            02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
            02003234 E12FFF1E bx      r14
        }
        022EBF80 E2866001 add     r6,r6,1h                  //r6 = Cntr6 + 1
        //022EBF84 E3560004 cmp     r6,4h
        //022EBF88 BAFFFFF9 blt     22EBF74h              ///022EBF74
    }
    //Zero out file buffers and tables??
    022EBF8C E2870E1A add     r0,r7,1A0h                //r0 = PtrLevelStruct + 0x1A0
    022EBF90 EBF5DD9A bl      2063600h                  ///02063600
    {
        02063600 E59FC004 ldr     r12,=2003250h         //FillBufferWithZeros( R0=buffaddress, R1=BuffLen )
        02063604 E3A01018 mov     r1,18h                //r1 = 24
        02063608 E12FFF1C bx      r12
        0206360C 02003250
    }
    022EBF94 E2870F65 add     r0,r7,194h                //r0 = PtrLevelStruct + 0x194
    022EBF98 EBF5DD8A bl      20635C8h                  ///020635C8
    {
        020635C8 E59FC004 ldr     r12,=2003250h         //FillBufferWithZeros( R0=buffaddress, R1=BuffLen )
        020635CC E3A0100C mov     r1,0Ch                //r1 = 12
        020635D0 E12FFF1C bx      r12
        020635D4 02003250
    }
    022EBF9C E3A01000 mov     r1,0h                     //r1 = 0
    022EBFA0 E2870F5D add     r0,r7,174h                //r0 = PtrBPLBuff
    022EBFA4 E5C711BC strb    r1,[r7,1BCh]              //[PtrLevelStruct,1BCh] = 0
    022EBFA8 EBF45C9E bl      2003228h
    {
        02003228 E3A01000 mov     r1,0h                 //r1 = 0
        0200322C E5801000 str     r1,[r0]               //[r0] = 0
        02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
        02003234 E12FFF1E bx      r14
    }
    022EBFAC E2870F5F add     r0,r7,17Ch                //r0 = PtrBMA1Buff
    022EBFB0 EBF45C9C bl      2003228h
    {
        02003228 E3A01000 mov     r1,0h                 //r1 = 0
        0200322C E5801000 str     r1,[r0]               //[r0] = 0
        02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
        02003234 E12FFF1E bx      r14
    }
    022EBFB4 E2870F61 add     r0,r7,184h                //r0 = PtrBPCBuff
    022EBFB8 EBF45C9A bl      2003228h
    {
        02003228 E3A01000 mov     r1,0h                 //r1 = 0
        0200322C E5801000 str     r1,[r0]               //[r0] = 0
        02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
        02003234 E12FFF1E bx      r14
    }
    022EBFBC E2870F63 add     r0,r7,18Ch                //r0 = PtrBMABuff
    022EBFC0 EBF45C98 bl      2003228h
    {
        02003228 E3A01000 mov     r1,0h                 //r1 = 0
        0200322C E5801000 str     r1,[r0]               //[r0] = 0
        02003230 E5801004 str     r1,[r0,4h]            //[r0,4h] = 0
        02003234 E12FFF1E bx      r14
    }
    022EBFC4 E8BD80F8 pop     r3-r7,r15
}
022EBDB0 E3A02000 mov     r2,0h             //r2 = 0
022EBDB4 E1CA20B0 strh    r2,[r10]          //
022EBDB8 E5CA21C0 strb    r2,[r10,1C0h]     //Zero
022EBDBC E5CA22BA strb    r2,[r10,2BAh]     //Zero
022EBDC0 E2421001 sub     r1,r2,1h          //r1 = -1
022EBDC4 E28A0C01 add     r0,r10,100h       //r0 = r10 + 100h
022EBDC8 E1C01BBE strh    r1,[r0,0BEh]      //Set LevelId to -1  //[r10,1BEh]
022EBDCC E58A21F0 str     r2,[r10,1F0h]     //
022EBDD0 E5CA21E0 strb    r2,[r10,1E0h]     //
022EBDD4 E5CA21E1 strb    r2,[r10,1E1h]     //
022EBDD8 E5CA21E2 strb    r2,[r10,1E2h]     //
022EBDDC E5CA21E3 strb    r2,[r10,1E3h]     //
022EBDE0 E5CA21EE strb    r2,[r10,1EEh]     // Zero Everything
022EBDE4 E58A21F4 str     r2,[r10,1F4h]     //
022EBDE8 E5CA21F8 strb    r2,[r10,1F8h]     //    
022EBDEC E5CA21F9 strb    r2,[r10,1F9h]     //
022EBDF0 E5CA21FA strb    r2,[r10,1FAh]     //
022EBDF4 E5CA21FB strb    r2,[r10,1FBh]     //
022EBDF8 E5CA21BC strb    r2,[r10,1BCh]     //
022EBDFC E28A0004 add     r0,r10,4h         //r0 = r10 + 4
022EBE00 E1A03002 mov     r3,r2             //r3 = 0

///
///022EBE04 - LOOP_Beg 
///
do while( cntr2 < 16 )
{
    022EBE04 E1C030B0 strh    r3,[r0]           //[r0] = 0
    022EBE08 E1C030B2 strh    r3,[r0,2h]        //[r0,2h] = 0 
    022EBE0C E2822001 add     r2,r2,1h          //r2 = cntr2 + 1
    022EBE10 E5803008 str     r3,[r0,8h]        //[r0,8h] = 0
    022EBE14 E5803004 str     r3,[r0,4h]        //[r0,4h] = 0
    //022EBE18 E3520010 cmp     r2,10h
    022EBE1C E280000C add     r0,r0,0Ch         //r0 = r0 + 12
    if( r2 < 16 )
    //    022EBE20 BAFFFFF7 blt     22EBE04h      ///022EBE04 - LOOP_Beg ^
}
///
022EBE24 E28A20C4 add     r2,r10,0C4h       //r2 = r10 + 0xC4
022EBE28 E3A04000 mov     r4,0h             //r4 = 0
022EBE2C E3A0002C mov     r0,2Ch            //r0 = 44

///
///022EBE30 - LOOP_Beg
///
do while( cntr3 < 4 )
{
    022EBE30 E0010093 mul     r1,r3,r0          //r1 = cntr3 * 44
    022EBE34 E7C24001 strb    r4,[r2,r1]        //[(r10 + 0xC4),(cntr3 * 44)] = 0
    022EBE38 E0821001 add     r1,r2,r1          //r1 = (r10 + 0xC4) + (cntr3 * 44)
    022EBE3C E5C14001 strb    r4,[r1,1h]        //[r1,1h] = 0
    022EBE40 E1C140B2 strh    r4,[r1,2h]        //[r1,2h] = 0
    022EBE44 E1C140B4 strh    r4,[r1,4h]        //[r1,4h] = 0
    022EBE48 E5814010 str     r4,[r1,10h]       //[r1,10h] = 0
    022EBE4C E5814018 str     r4,[r1,18h]       //[r1,18h] = 0
    022EBE50 E5814014 str     r4,[r1,14h]       //[r1,14h] = 0
    022EBE54 E5814020 str     r4,[r1,20h]       //[r1,20h] = 0
    022EBE58 E581401C str     r4,[r1,1Ch]       //[r1,1Ch] = 0
    022EBE5C E2833001 add     r3,r3,1h          //r3 = cntr3 + 1
    022EBE60 E5814024 str     r4,[r1,24h]       //[r1,24h] = 0
    022EBE64 E5814028 str     r4,[r1,28h]       //[r1,28h] = 0
    //022EBE68 E3530004 cmp     r3,4h
    //if( cntr3 < 4 )
    //    022EBE6C BAFFFFEF blt     22EBE30h      ///022EBE30 - LOOP_Beg ^
}

022EBE70 E59F0030 ldr     r0,=2320BE4h      //r0 = ptr_tbl_2320BE4h
022EBE74 E5902014 ldr     r2,[r0,14h]       //r2 = [ptr_tbl_2320BE4h,14h]
022EBE78 E5901018 ldr     r1,[r0,18h]       //r1 = [ptr_tbl_2320BE4h,18h]
022EBE7C E58D2000 str     r2,[r13]          //[r13] = r2
022EBE80 E58D1004 str     r1,[r13,4h]       //[r13,4h] = r1

///
///022EBE84 - LOOP_Beg
///
do while( cntr4 < 2 )
{
    022EBE84 E08A0184 add     r0,r10,r4,lsl 3h  //r0 = r10 + (cntr4 * 8)
    022EBE88 E5802200 str     r2,[r0,200h]      //[(r10 + (cntr4 * 8)),200h] = [ptr_tbl_2320BE4h,14h]
    022EBE8C E2844001 add     r4,r4,1h          //r4 = cntr4 + 1
    022EBE90 E5801204 str     r1,[r0,204h]      //[(r10 + (cntr4 * 8)),204h] = [ptr_tbl_2320BE4h,18h]
    //022EBE94 E3540002 cmp     r4,2h 
    //if( cntr4 < 2 )    
    //    022EBE98 BAFFFFF9 blt     22EBE84h      ///022EBE84 - LOOP_Beg ^
}
022EBE9C E28DD008 add     r13,r13,8h
022EBEA0 E8BD8FF8 pop     r3-r11,r15
022EBEA4 02324CBC 
022EBEA8 02320BE4 

//-----------------------------------------------------------------------------------
//Full LoadBGTileSets function
//-----------------------------------------------------------------------------------

022EC27C E92D4FF0 push    r4-r11,r14
022EC280 E24DDF4F sub     r13,r13,13Ch
022EC284 E1A0A000 mov     r10,r0                        //R10 = PtrStructLevel2
022EC288 E5DA02BC ldrb    r0,[r10,2BCh]                 //r0  = Boolean first constant
022EC28C E1A04001 mov     r4,r1                         //R4 = Param1 (Possibly LevelId!)
022EC290 E3500000 cmp     r0,0h
if( R0 == 0 )
{
    022EC294 03A00006 moveq   r0,6h
    022EC298 058D0000 streq   r0,[r13]
}
else
{
    022EC29C 13A0000F movne   r0,0Fh
    022EC2A0 158D0000 strne   r0,[r13]
}
022EC2A4 E3E00000 mvn     r0,0h                         //r0 = 0xFFFFFFFF
022EC2A8 E1540000 cmp     r4,r0
022EC2AC E1A0000A mov     r0,r10                        //R0 = PtrStructLevel2
if( Param1 != 0xFFFFFFFF )
{
    022EC2B0 1A000001 bne     22EC2BCh                  ///022EC2BC LBL1 v
    ///022EC2BC LBL1
    022EC2BC EBFFFF41 bl      22EBFC8h                      ///fun_022EBFC8 Seems to allocate level data buffer?
    022EC2C0 E3A00001 mov     r0,1h
    022EC2C4 E1CA00B0 strh    r0,[r10]
    022EC2C8 E3A00000 mov     r0,0h
    022EC2CC E5CA01C0 strb    r0,[r10,1C0h]
    022EC2D0 E28A0C01 add     r0,r10,100h
    022EC2D4 E1C04BBE strh    r4,[r0,0BEh]                  //[R0+0xBE] = LevelId
    022EC2D8 E1D01BFE ldrsh   r1,[r0,0BEh]                  //R1 = LevelId
    022EC2DC E28D00CC add     r0,r13,0CCh
    022EC2E0 EBFFFE16 bl      22EBB40h                      /// Looks up the filenames in bg_list.dat! fun_022EBB40(r0 = targetfnamestr, r1 = lvlid ) 
    022EC2E4 E59F1A10 ldr     r1,=2320C58h                  //"rom0:/MAP_BG/%s.bpl
    022EC2E8 E28D004C add     r0,r13,4Ch
    022EC2EC E28D20CC add     r2,r13,0CCh
    022EC2F0 EBF674A3 bl      2089584h                      ///fun_02089584 ReplaceTokens
    022EC2F4 E59D2000 ldr     r2,[r13]                      //r2 = if([r10,2BCh] == 0){0x6} else {0xF}
    022EC2F8 E28D104C add     r1,r13,4Ch
    022EC2FC E28A0F5D add     r0,r10,174h                   //R0 = PtrBPLBuff  
    022EC300 EBF4724D bl      2008C3Ch                      ///LoadFileFromRom
    022EC304 E59F19F4 ldr     r1,=2320C6Ch                  //"rom0:/MAP_BG/%s.bpc"
    022EC308 E28D004C add     r0,r13,4Ch
    022EC30C E28D20D6 add     r2,r13,0D6h
    022EC310 EBF6749B bl      2089584h                      ///fun_02089584 ReplaceTokens
    022EC314 E59F29E8 ldr     r2,=30Fh
    022EC318 E28A0F61 add     r0,r10,184h                   //R0 = PtrBPCBuff
    022EC31C E28D104C add     r1,r13,4Ch
    022EC320 EBF47245 bl      2008C3Ch                      ///LoadFileFromRom
    022EC324 E59F19DC ldr     r1,=2320C80h                  //"rom0:/MAP_BG/%s.bma"
    022EC328 E28D004C add     r0,r13,4Ch
    022EC32C E28D20E0 add     r2,r13,0E0h
    022EC330 EBF67493 bl      2089584h                      ///fun_02089584 ReplaceTokens
    022EC334 E59F29C8 ldr     r2,=30Fh                      //r2 = 0x30F
    022EC338 E28A0F63 add     r0,r10,18Ch                   //R0 = PtrBMABuff
    022EC33C E28D104C add     r1,r13,4Ch                    //r1 = StackPtr + 0x4C
    022EC340 EBF4723D bl      2008C3Ch                      ///LoadFileFromRom
    022EC344 E59A1184 ldr     r1,[r10,184h]                 //R1 = PtrBPCBuff
    022EC348 E59A0174 ldr     r0,[r10,174h]                 //R0 = PtrBPLBuff
    022EC34C E1D120B0 ldrh    r2,[r1]                       //R2 = read BPC_ptruplayer from header
    022EC350 E58D0028 str     r0,[r13,28h]                  //[R13 + 28] = PtrBPLBuff
    022EC354 E59A518C ldr     r5,[r10,18Ch]                 //R5 = PtrBMABuff
    022EC358 E3520000 cmp     r2,0h                         //
    if( BPC_ptruplayer == 0 )                               //BPC_ptruplayer == 0
    {
        022EC35C 0A00000F beq     22EC3A0h                  ///022EC3A0 LBL3 v
        ///022EC3A0 LBL3
        022EC3A0 E3A00000 mov     r0,0h
        022EC3A4 E58D0044 str     r0,[r13,44h]                  //[R13 + 44h] = 0 //This is probably the BPC first layer
        022EC3A8 E58D0048 str     r0,[r13,48h]                  //[R13 + 48h] = 0 //This is probably the BPC second layer
        022EC3AC E58A01C4 str     r0,[r10,1C4h]                 //[R10 + 1C4h] = 0 //NbLayers set to 0
    }
    else
    {
        022EC360 E1D100B2 ldrh    r0,[r1,2h]                    //R0 = read BPC_ptrlowlayer from header
        022EC364 E1A020A2 mov     r2,r2,lsr 1h                  //R2 = BPC_ptruplayer/2
        022EC368 E0812082 add     r2,r1,r2,lsl 1h               //R2 = PtrBPCBuff + ((BPC_ptruplayer/2) * 2) //I think they're making sure the number is even by clearing the lowest bit.
        022EC36C E3500000 cmp     r0,0h
        022EC370 E58D2044 str     r2,[r13,44h]                  //[R13 + 44h] = PtrBPCBuff + ((BPC_ptruplayer/2) * 2)
        if( BPC_ptrlowlayer == 0 )
        {
            022EC374 03A02000 moveq   r2,0h
            022EC378 03A00001 moveq   r0,1h
            022EC37C 058D2048 streq   r2,[r13,48h]                  //[R13 + 48h] = 0 //Second layer ptr set to 0
            022EC380 058A01C4 streq   r0,[r10,1C4h]                 //[R10 + 1C4h] = 1 //NbLayers set to 1
            022EC384 0A000009 beq     22EC3B0h                      ///022EC3B0 LBL4 v
        }
        else
        {
            022EC388 E1A000A0 mov     r0,r0,lsr 1h                  //R0 = BPC_ptrlowlayer/2
            022EC38C E0812080 add     r2,r1,r0,lsl 1h               //R2 = PtrBPCBuff + ((BPC_ptrlowlayer/2) * 2)
            022EC390 E3A00002 mov     r0,2h                         //R0 = 2
            022EC394 E58D2048 str     r2,[r13,48h]                  //[R13 + 48h] = PtrBPCBuff + ((BPC_ptrlowlayer/2) * 2)
            022EC398 E58A01C4 str     r0,[r10,1C4h]                 //[R10 + 1C4h] = 2 //NbLayers set to 2
            022EC39C EA000003 b       22EC3B0h                      ///022EC3B0 LBL4 v
        }
    }
    ///022EC3B0 LBL4
    022EC3B0 E59D0028 ldr     r0,[r13,28h]                  //R0 = PtrBPLBuff
    022EC3B4 E28A20EA add     r2,r10,0EAh
    022EC3B8 E5D03000 ldrb    r3,[r0]                       //R3 = FirstByteBPL
    022EC3BC E28A0C01 add     r0,r10,100h
    022EC3C0 E2811004 add     r1,r1,4h                      //R1 = PtrBPC_DBA
    022EC3C4 E1C03EBA strh    r3,[r0,0EAh]                  //[r0,0EAh] = FirstByteBPL (Basically put at r10 + 0x1EA the first byte of the palette!)
    022EC3C8 E59D0028 ldr     r0,[r13,28h]                  //R0 = PtrBPLBuff
    022EC3CC E5D03002 ldrb    r3,[r0,2h]                    //R3 = Unk2BPL
    022EC3D0 E2820C01 add     r0,r2,100h
    022EC3D4 E58D0040 str     r0,[r13,40h]                  //[r13,40h] = PtrBPLHeaderCopy
    022EC3D8 E1C030B2 strh    r3,[r0,2h]
    022EC3DC E3A00000 mov     r0,0h                         //R0 = CNT ?

    //
    ///022EC3E0 LOOP3_BEG - Copy BPC SizeTable
    //
    while( CNT < 2 )
    {
        022EC3E0 E28A4F72 add     r4,r10,1C8h                   //r4 = PtrBPCIndexTblCopy
        022EC3E4 E3A0200C mov     r2,0Ch                        //r2 = 12 (Size of an bpc index table entry)
        022EC3E8 E28D3044 add     r3,r13,44h                    //PtrBPC_DBB
        022EC3EC E7933100 ldr     r3,[r3,r0,lsl 2h]             //R3 = [PtrBPC_DBB + (CNT * 4)] (This is the pointer in the entry)
        022EC3F0 E0244290 mla     r4,r0,r2,r4                   //R4 = (CNT * 12) + PtrBPCIndexTblCopy
        022EC3F4 E3530000 cmp     r3,0h
        022EC3F8 E3A06000 mov     r6,0h                         //R6 = CNT2 = 0
        if( R3 == 0 )
        {
            022EC3FC 0A00000A beq     22EC42Ch                      ///022EC42C LBL5 v
            ///022EC42C LBL5
            022EC42C E1C460B0 strh    r6,[r4]
            022EC430 E1A03006 mov     r3,r6                             //r3 = 0
            while( CNT2 < 4 )
            {
                ///022EC434 LOOP2_BEG
                022EC434 E0842086 add     r2,r4,r6,lsl 1h               //R2 = R4 + (CNT2 * 2)
                022EC438 E2866001 add     r6,r6,1h                      //R6 = CNT2 + 1
                022EC43C E1C230B2 strh    r3,[r2,2h]
                022EC440 E3560004 cmp     r6,4h
                if( CNT2 < 4 )
                    022EC444 BAFFFFFA blt     22EC434h                      ///022EC434 LOOP2_BEG ^
            }
            022EC448 E1C430BA strh    r3,[r4,0Ah]
            ///022EC44C LBL6 v
        }
        else
        {
            022EC400 E0D120B2 ldrh    r2,[r1],2h                    //R2 = [PtrBPC_DBA] then increment PtrBPC_DBA by 2! (This parses the words from the BPC SizeTable's ScreenEntry)
            022EC404 E1C420B0 strh    r2,[r4]                       //    
            while( CNT2 < 4 )
            {
                ///022EC408 LOOP1_BEG
                022EC408 E0D130B2 ldrh    r3,[r1],2h                    //R2 = [PtrBPC_DBA] then increment PtrBPC_DBA by 2!
                022EC40C E0842086 add     r2,r4,r6,lsl 1h               //R2 = R4 + (CNT2 * 2)
                022EC410 E2866001 add     r6,r6,1h                      //R6 = CNT2 + 1
                022EC414 E1C230B2 strh    r3,[r2,2h]
                022EC418 E3560004 cmp     r6,4h
                if( CNT2 < 4 )
                    022EC41C BAFFFFF9 blt     22EC408h                      ///022EC408 LOOP1_BEG ^
            }
            022EC420 E0D120B2 ldrh    r2,[r1],2h                    //R2 = [PtrBPC_DBA], increment PtrBPC_DBA by 2!
            022EC424 E1C420BA strh    r2,[r4,0Ah]
            022EC428 EA000007 b       22EC44Ch                      ///022EC44C LBL6 v
        }
        ///022EC44C LBL6
        022EC44C E2800001 add     r0,r0,1h                      //R0 = CNT + 1
        022EC450 E3500002 cmp     r0,2h                         //
        if( CNT < 2 )
            022EC454 BAFFFFE1 blt     22EC3E0h                      ///022EC3E0 LOOP3_BEG ^
    }
    ///
    ///022EC458 - Copy BMA stuff
    ///
    022EC458 E5D51000 ldrb    r1,[r5]                       //R1 = [PtrBMABuff] = BMA_MapWidth
    022EC45C E28A0C02 add     r0,r10,200h
    022EC460 E58D0034 str     r0,[r13,34h]
    022EC464 E5CA11E0 strb    r1,[r10,1E0h]                 //[R10 + 0x1E0] = BMA_MapWidth
    022EC468 E5D50001 ldrb    r0,[r5,1h]                    //R0 = [PtrBMABuff+1] = BMA_MapHeight
    022EC46C E28A4E1E add     r4,r10,1E0h
    022EC470 E59F2894 ldr     r2,=2324CBCh                  //Pointer to Pointer, or pointer table?
    022EC474 E5C40001 strb    r0,[r4,1h]                    //[r4,1h] = BMA_MapHeight
    022EC478 E5D53004 ldrb    r3,[r5,4h]                    //R3 = BMA_DBA_unk3
    022EC47C E59D0028 ldr     r0,[r13,28h]                  //R0 = PtrBPLBuff
    022EC480 E3A01028 mov     r1,28h                        //R1 = 40
    022EC484 E2800004 add     r0,r0,4h                      //R0 = PtrBPL_Palettes + 4 //Skips header?
    022EC488 E5C43002 strb    r3,[r4,2h]                    //[R4+2] = BMA_unk3   
    022EC48C E58D001C str     r0,[r13,1Ch]                  //[r13,1Ch] = (PtrBPL_Palettes + 4)
    022EC490 E5D50005 ldrb    r0,[r5,5h]                    //R0 = [PtrBMABuff+5] = BMA_unk4
    022EC494 E3A09000 mov     r9,0h                         //R9 = CNTPalettes = 0
    022EC498 E5C40003 strb    r0,[r4,3h]                    //[R4+3] = BMA_unk4
    022EC49C E5D50006 ldrb    r0,[r5,6h]                    //R0 = BMA_unk5
    022EC4A0 E1C400B4 strh    r0,[r4,4h]                    //[R4+4] = BMA_unk5
    022EC4A4 E5D50008 ldrb    r0,[r5,8h]                    //R0 = BMA_unk6
    022EC4A8 E1C400B6 strh    r0,[r4,6h]                    //[R4+6] = BMA_unk6    
    022EC4AC E5D5000A ldrb    r0,[r5,0Ah]                   //R0 = BMA_unk7
    022EC4B0 E1C400B8 strh    r0,[r4,8h]                    //[R4+8] = BMA_unk7
    022EC4B4 E59D0034 ldr     r0,[r13,34h]                  //r0 = r10 + 0x200
    022EC4B8 E5DA32BC ldrb    r3,[r10,2BCh]                 //r3 =  //unk_r10_2BCh //Constant loaded in fun_022EBC18
    022EC4BC E1D00CF2 ldrsh   r0,[r0,0C2h]                  //r0 = [r10,2C2h] 
    022EC4C0 E5922000 ldr     r2,[r2]                       //r2 = [2324CBCh] 
    022EC4C4 E1660183 smulbb  r6,r3,r1                      //r6 = unk_r10_2BCh * 40
    022EC4C8 E1A00A00 mov     r0,r0,lsl 14h                 //r0 = r0 << 20
    022EC4CC E2827098 add     r7,r2,98h                     //r7 = r2 + 152
    022EC4D0 E1A08820 mov     r8,r0,lsr 10h                 //r8 = (r0 << 20) >> 16
    022EC4D4 EA000012 b       22EC524h                  ///022EC524 LBL7 v
    ///
    ///022EC4D8 LOOP4_BEG  - Copy color palettes!
    ///
    while( CNTPalettes < BPLNbPalettes && CNTPalettes < [r10,2C4h] )
    {
        ///022EC524 LBL7 - Loop Condition evaluated here!!
        022EC524 E59D0040 ldr     r0,[r13,40h]              //r0 = PtrBPLHeaderCopy
        022EC528 E1D000F0 ldrsh   r0,[r0]                   //r0 = BPL_nbpal
        022EC52C E1590000 cmp     r9,r0
        022EC530 AA000003 bge     22EC544h                  ///022EC544 LBL6 v
        022EC534 E59D0034 ldr     r0,[r13,34h]              //r0 = r10 + 200h
        022EC538 E1D00CF4 ldrsh   r0,[r0,0C4h]              //r0 = Unknown?? (Was 0xE) (At 0x210CD08) (The value is actually a constant loaded from the table at 0x2320CD8, on line 022EBC2C) Possibly the nb of palettes to parse!
        022EC53C E1590000 cmp     r9,r0
        022EC540 BAFFFFE4 blt     22EC4D8h                  ///022EC4D8 LOOP4_BEG  We seem to copy part of the palette this way, and continue parsing the rest another way?
        
        ///022EC4D8
        022EC4D8 E2880001 add     r0,r8,1h                  //r0 = r8 + 1
        022EC4DC E1A00800 mov     r0,r0,lsl 10h             //r0 = r0 << 16
        022EC4E0 E1A01008 mov     r1,r8                     //r1 = r8
        022EC4E4 E1A08820 mov     r8,r0,lsr 10h             //r8 = r0 >> 16
        022EC4E8 E59F2820 ldr     r2,=2320BF4h              //r2 = 0x2320BF4
        022EC4EC E0870006 add     r0,r7,r6                  //r0 = r7 + (unk_r10_2BCh * 40)
        022EC4F0 EBF47826 bl      200A590h                  //Copy color at r2, at buffer at r0 + 24
        {
            ///FUN_200A590 //Possibly zeros the palette?
            0200A590 E5900018 ldr     r0,[r0,18h]           //
            0200A594 E3A03004 mov     r3,4h                 //r3 = Cnt = 4
            0200A598 E0801101 add     r1,r0,r1,lsl 2h       //r1 = PtrMemPalette?? + (CntColor * 4)
            while(Cnt != 0)
            {
                ///0200A59C
                0200A59C E4D20001 ldrb    r0,[r2],1h
                0200A5A0 E2533001 subs    r3,r3,1h              //r3 = Cnt - 1
                0200A5A4 E4C10001 strb    r0,[r1],1h
                0200A5A8 1AFFFFFB bne     200A59Ch              ///0200A59C ^
            }
            0200A5AC E12FFF1E bx      r14
        }
        022EC4F4 E59D201C ldr     r2,[r13,1Ch]              //r2 = PtrBPL_Palettes
        022EC4F8 E0870006 add     r0,r7,r6                  //r0 = r7 + (unk_r10_2BCh * 40) //A pointer in a pointer table?
        022EC4FC E1A01008 mov     r1,r8                     //r1 = ColorIndex??
        022EC500 E3A0300F mov     r3,0Fh                    //r3 = Colors to copy??
        022EC504 EBF47829 bl      200A5B0h                  //Seems to copy colors bytes from the bpl
        {
            0200A5B0 E92D4038 push    r3-r5,r14
            0200A5B4 E5900018 ldr     r0,[r0,18h]           //r0 = PtrMemPalette??
            0200A5B8 E3A0E000 mov     r14,0h                //r14 = CntColor
            0200A5BC E080C101 add     r12,r0,r1,lsl 2h      //r12 = PtrMemPalette?? + (ColorIndex?? * 4)
            0200A5C0 EA000009 b       200A5ECh              ///0200A5EC v
            while(CntColor < NbColorsToCopy)
            {
                ///0200A5EC
                0200A5EC E15E0003 cmp     r14,r3                    
                0200A5F0 BAFFFFF3 blt     200A5C4h              ///0200A5C4 ^
                ///0200A5C4
                0200A5C4 E1A04002 mov     r4,r2                 //r4 = PtrSourceBplPalette
                0200A5C8 E1A0500C mov     r5,r12                //r5 = PtrDestinationPalette
                0200A5CC E2822004 add     r2,r2,4h              //r2 = PtrBPL_Palettes + 4
                0200A5D0 E28CC004 add     r12,r12,4h            //r12 = PtrDestinationPalette + 4
                0200A5D4 E3A01004 mov     r1,4h                 //r1 = NbColorBytesLeft = 4
                while(r1 != 0)
                {
                    ///0200A5D8
                    0200A5D8 E4D40001 ldrb    r0,[r4],1h            //r0 = [PtrSourceBplPalette],  ++PtrSourceBplPalette
                    0200A5DC E2511001 subs    r1,r1,1h              //r1 = NbColorBytesLeft - 1
                    0200A5E0 E4C50001 strb    r0,[r5],1h            //[PtrDestinationPalette] = CurColorByte, ++PtrDestinationPalette
                    0200A5E4 1AFFFFFB bne     200A5D8h              ///0200A5D8 ^
                }
                0200A5E8 E28EE001 add     r14,r14,1h            //r14 = CntColor + 1
            }
            0200A5F4 E8BD8038 pop     r3-r5,r15
        }
        022EC508 E288000F add     r0,r8,0Fh                 //r0 = ColorIndex? + 15
        022EC50C E1A00800 mov     r0,r0,lsl 10h             //r0 = r0 << 16
        022EC510 E1A08820 mov     r8,r0,lsr 10h             //r8 = r0 >> 16
        022EC514 E59D001C ldr     r0,[r13,1Ch]              //r0 = PtrBPL_Palettes
        022EC518 E2899001 add     r9,r9,1h                  //r9 = CNTPalettes + 1
        022EC51C E280003C add     r0,r0,3Ch                 //r0 = PtrBPL_Palettes + 60
        022EC520 E58D001C str     r0,[r13,1Ch]              //[r13,1Ch] = (PtrBPL_Palettes + 60)

    }

    ///022EC544 LBL6 - Parse extra palettes
    022EC544 E28A0C02 add     r0,r10,200h               //r0 = r10 + 0x200
    022EC548 E58D0038 str     r0,[r13,38h]              //
    022EC54C EA000015 b       22EC5A8h                  ///022EC5A8 v 
    while( CNTPalettes < [r10,2C4h] )                   //Seems like we parse the remaining palettes 
    {
        ///022EC5A8
        022EC5A8 E59D0038 ldr     r0,[r13,38h]              //r0 = AddressNextMemPalette?
        022EC5AC E1D00CF4 ldrsh   r0,[r0,0C4h]              //r0 = Unknown?? (Was 0xE)
        022EC5B0 E1590000 cmp     r9,r0
        //***LOOP CONDITION***
        022EC5B4 BAFFFFE5 blt     22EC550h                  ///022EC550 ^
        ///022EC550
        022EC550 E2880001 add     r0,r8,1h                  //r0 = ColorIndex? + 1
        022EC554 E1A00800 mov     r0,r0,lsl 10h             //r0 = (ColorIndex? + 1) << 16
        022EC558 E1A01008 mov     r1,r8                     //r1 = ColorIndex?
        022EC55C E1A08820 mov     r8,r0,lsr 10h             //r8 = ((ColorIndex? + 1) << 16) >> 16
        022EC560 E59F27A8 ldr     r2,=2320BF4h              //r2 = 
        022EC564 E0870006 add     r0,r7,r6
        022EC568 EBF47808 bl      200A590h                  ///0200A590
        {
            ///FUN_200A590
            0200A590 E5900018 ldr     r0,[r0,18h]           //
            0200A594 E3A03004 mov     r3,4h                 //r3 = Cnt = 4
            0200A598 E0801101 add     r1,r0,r1,lsl 2h       //r1 = PtrMemPalette?? + (CntColor * 4)
            while(Cnt != 0)
            {
                ///0200A59C
                0200A59C E4D20001 ldrb    r0,[r2],1h
                0200A5A0 E2533001 subs    r3,r3,1h              //r3 = Cnt - 1
                0200A5A4 E4C10001 strb    r0,[r1],1h
                0200A5A8 1AFFFFFB bne     200A59Ch              ///0200A59C ^
            }
            0200A5AC E12FFF1E bx      r14
        }
        022EC56C E3A00001 mov     r0,1h
        022EC570 E58D0018 str     r0,[r13,18h]
        while( r0 < 16 )
        {
            ///022EC574
            022EC574 E2880001 add     r0,r8,1h
            022EC578 E1A00800 mov     r0,r0,lsl 10h
            022EC57C E1A01008 mov     r1,r8
            022EC580 E1A08820 mov     r8,r0,lsr 10h
            022EC584 E59F2788 ldr     r2,=2320BE8h
            022EC588 E0870006 add     r0,r7,r6
            022EC58C EBF477FF bl      200A590h              ///0200A590
            {
                ///FUN_200A590
                0200A590 E5900018 ldr     r0,[r0,18h]           //
                0200A594 E3A03004 mov     r3,4h                 //r3 = Cnt = 4
                0200A598 E0801101 add     r1,r0,r1,lsl 2h       //r1 = PtrMemPalette?? + (CntColor * 4)
                while(Cnt != 0)
                {
                    ///0200A59C
                    0200A59C E4D20001 ldrb    r0,[r2],1h
                    0200A5A0 E2533001 subs    r3,r3,1h              //r3 = Cnt - 1
                    0200A5A4 E4C10001 strb    r0,[r1],1h
                    0200A5A8 1AFFFFFB bne     200A59Ch              ///0200A59C ^
                }
                0200A5AC E12FFF1E bx      r14
            }
            022EC590 E59D0018 ldr     r0,[r13,18h]
            022EC594 E2800001 add     r0,r0,1h
            022EC598 E58D0018 str     r0,[r13,18h]
            022EC59C E3500010 cmp     r0,10h
            022EC5A0 BAFFFFF3 blt     22EC574h                  ///022EC574 ^
        }   
        022EC5A4 E2899001 add     r9,r9,1h                  //r9 = CNTPalettes + 1
    }
    022EC5B8 E0870006 add     r0,r7,r6
    022EC5BC EBF477D0 bl      200A504h
    {
        0200A504 E3A01001 mov     r1,1h
        0200A508 E5C01008 strb    r1,[r0,8h]
        0200A50C E12FFF1E bx      r14
    }
    022EC5C0 E28A0C02 add     r0,r10,200h               //r0 = 
    022EC5C4 E1D00BFE ldrsh   r0,[r0,0BEh]              //r0 = 
    022EC5C8 E3A08000 mov     r8,0h                     //r8 = CNT_4 = 0
    022EC5CC E58D0020 str     r0,[r13,20h]
    022EC5D0 EA0000F2 b       22EC9A0h                  ///022EC9A0 v
    while( CntBpcEntry < r0 )
    {
        ///022EC9A0
        022EC9A0 E28A0C02 add     r0,r10,200h           //
        022EC9A4 E1D00CF0 ldrsh   r0,[r0,0C0h]          //r0 = [r0,0C0h] (was 2, at 0x210CD04) (Probably the NB of BPC entries )
        022EC9A8 E1580000 cmp     r8,r0     
        //**** LOOP CONDITION ACTUALLY EVALUATED HERE! ****
        022EC9AC BAFFFF08 blt     22EC5D4h              ///022EC5D4 LOOP_BEG_ParseBPCIndexTable ^
        
        ///
        ///022EC5D4 LOOP_BEG_ParseBPCIndexTable - We seem to begin reading the BPC Datablock B stuff here?
        ///
        022EC5D4 E28D0044 add     r0,r13,44h                //r0 = PtrBPC_DBB
        022EC5D8 E7900108 ldr     r0,[r0,r8,lsl 2h]         //r0 = [ PtrBPC_DBB + (CntBpcEntry * 4) ]
        022EC5DC E58D0024 str     r0,[r13,24h]              //r0 = PtrBPCCurPos?
        022EC5E0 E3500000 cmp     r0,0h
        if( PtrBPCCurPos? == 0 )
        {
            022EC5E4 0A0000E9 beq     22EC990h                  ///022EC990 v
            ///022EC990
            022EC990 E59D0020 ldr     r0,[r13,20h]          //r0 = [r13,20h] (was 0)
            022EC994 E2888001 add     r8,r8,1h              //r8 = CntBpcEntry + 1
            022EC998 E2800001 add     r0,r0,1h              //r0 = r0 + 1
            022EC99C E58D0020 str     r0,[r13,20h]          //[r13,20h] = (r0 + 1)
        }
        else
        {
            022EC5E8 E3A0000C mov     r0,0Ch                    //r0 = 12
            022EC5EC E0010098 mul     r1,r8,r0                  //r1 = CntBpcEntry * 12
            022EC5F0 E28A0C02 add     r0,r10,200h               //r0 = r10 + 0x200
            022EC5F4 E1D02CF6 ldrsh   r2,[r0,0C6h]              //r2 = [r10,2C6h]
            022EC5F8 E28A6F72 add     r6,r10,1C8h               //r6 = r10 + 0x1C8 = PtrBPC_DBA_Copy (datablockA entries in-memory copy ptr)  
            022EC5FC E0869001 add     r9,r6,r1                  //r9 = PtrBPC_DBA_Copy + (CntBpcEntry * 12) (This gets the entry at the current index CntBpcEntry)
            022EC600 E19610F1 ldrsh   r1,[r6,r1]                //r1 = [ PtrBPC_DBA_Copy + (CntBpcEntry * 12) ] = BPC_Entry_unk1 (First word of the DBA entry!)
            022EC604 E5DA32BC ldrb    r3,[r10,2BCh]             //r3 = [r10,2BCh] (was 0)
            022EC608 E59F0708 ldr     r0,=2320C18h              //Table_02320C18 VRAM addresses table!! (Table with 5, 8 bytes entries made of 2 int 32) 
            022EC60C E3A07000 mov     r7,0h                     //r7 = 0
            022EC610 E0803183 add     r3,r0,r3,lsl 3h           //r3 = 2320C18h + (r3 * 8)
            022EC614 E59D0020 ldr     r0,[r13,20h]              //r0 = [r13,20h] (was 0)
            022EC618 E59D6024 ldr     r6,[r13,24h]              //r6 = PtrBPC_DBB?
            022EC61C E7930100 ldr     r0,[r3,r0,lsl 2h]         //r0 = Table_02320C18[r3][r0] 
            022EC620 E58D7014 str     r7,[r13,14h]              //[r13,14h] = 0
            022EC624 E080B282 add     r11,r0,r2,lsl 5h          //r11 = Table_02320C18[r3][r0] + (r2 * 32)
            022EC628 E08B0281 add     r0,r11,r1,lsl 5h          //r0  = (Table_02320C18[r3][r0] + (r2 * 32)) + (BPC_Entry_unk1 * 32)
            022EC62C E58D0010 str     r0,[r13,10h]              //[r13,10h] = r0
            022EC630 E1A03007 mov     r3,r7                     //r3 = 0
            022EC634 E1A02007 mov     r2,r7                     //r2 = 0
            022EC638 E1A01007 mov     r1,r7                     //r1 = 0 = CNT_5
            022EC63C E1A00007 mov     r0,r7                     //r0 = 0
            ///022EC640 - Zero out 16 bytes of the video ram!
            while( CNT_5 < 16 )
            {
                022EC640 E0CB00B2 strh    r0,[r11],2h           //[r11] = 0, r11 += 2
                022EC644 E2811001 add     r1,r1,1h              //r1 = CNT_5 + 1
                022EC648 E3510010 cmp     r1,10h                    
                022EC64C BAFFFFFB blt     22EC640h              ///022EC640 ^
            }
            022EC650 EA000047 b       22EC774h              ///022EC774 v
            
            //Decode the image!
            while( VRAM_BaseAddress < PtrVRAMEnd )
            {
                ///022EC774 - Validate Loop
                022EC774 E59D0010 ldr     r0,[r13,10h]          //r0 = PtrVRAMEnd
                022EC778 E15B0000 cmp     r11,r0   
                //CONDITION IS CHECKED HERE!
                022EC77C 3AFFFFB4 bcc     22EC654h              ///022EC654 ^
                ///
                ///022EC654 - This loop decodes the BPC image and write it straight to VRAM!!
                ///
                022EC654 E5D60000 ldrb    r0,[r6]               //r0 = [PtrFileReadPos] = cmdbyte
                022EC658 E2861001 add     r1,r6,1h              //r1 = PtrBPC_DBB + 1
                022EC65C E1A06001 mov     r6,r1                 //r6 = PtrFileReadPos
                022EC660 E3500080 cmp     r0,80h
                if( cmdbyte >= 0x80 )                           //SIGNED! ( cmdbyte >= -128 )
                {
                    022EC664 AA00001B bge     22EC6D8h              ///022EC6D8 v
                    ///
                    ///022EC6D8 - cmdbyte >= 0x80
                    ///
                    022EC6D8 E35000C0 cmp     r0,0C0h
                    if( cmdbyte >= 0xC0 )                           //SIGNED
                    {
                        022EC6DC AA000006 bge     22EC6FCh              ///022EC6FC v
                        ///
                        ///022EC6FC - cmdbyte >= 0xC0
                        ///
                        022EC6FC E35000E0 cmp     r0,0E0h
                        if( cmdbyte >= 0xE0 )
                        {
                            022EC700 AA000004 bge     22EC718h              ///022EC718 v
                            ///
                            ///022EC718 - curdbbbyte >= 0xE0
                            ///
                            022EC718 E35000FF cmp     r0,0FFh
                            if( curdbbbyte == 0xFF )
                            {
                                022EC71C 05D10000 ldreqb  r0,[r1]
                                022EC720 02816001 addeq   r6,r1,1h
                            }
                            022EC724 E59D1014 ldr     r1,[r13,14h]
                            if( curdbbbyte != 0xFF )
                            {
                                022EC728 124000E0 subne   r0,r0,0E0h
                            }
                            022EC72C E58D7014 str     r7,[r13,14h]
                            022EC730 E1A07001 mov     r7,r1
                        }
                        else if( cmdbyte == 0xDF )
                        {
                            022EC704 E35000DF cmp     r0,0DFh
                            022EC708 05D10000 ldreqb  r0,[r1]               //r0 = nbtowrite = [PtrFileReadPos]
                            022EC70C 02816001 addeq   r6,r1,1h              //r6 = PtrFileReadPos + 1
                        }
                        else
                        {
                            022EC710 124000C0 subne   r0,r0,0C0h        //r0 = nbtowrite = cmdbyte - 0xC0
                        }
                        022EC714 EA000006 b       22EC734h              ///022EC734 v
                    }
                    else 
                    {
                        if( cmdbyte == 0xBF )
                        {
                            022EC6E0 E35000BF cmp     r0,0BFh
                            022EC6E4 05D10000 ldreqb  r0,[r1]           //r0 = nbtowrite = [PtrFileReadPos] (Load next byte!)
                            022EC6E8 02816001 addeq   r6,r1,1h          //r6 = PtrFileReadPos + 1
                        }
                        022EC6EC E58D7014 str     r7,[r13,14h]
                        if( cmdbyte != 0xBF )
                        {
                            022EC6F0 12400080 subne   r0,r0,80h         //r0 = cmdbyte - 0x80 = nbtowrite
                        }
                        022EC6F4 E4D67001 ldrb    r7,[r6],1h            //r7 = [PtrFileReadPos], ++PtrFileReadPos (Load next byte and increment current byte ptr!)
                        022EC6F8 EA00000D b       22EC734h              ///022EC734 v
                    }
                    ///022EC734
                    022EC734 E3530000 cmp     r3,0h
                    if( r3 != 0 )
                    {
                        022EC738 11821407 orrne   r1,r2,r7,lsl 8h       //r1 = r2 | (nextbpcdbbbyte << 8)
                        022EC73C 10CB10B2 strneh  r1,[r11],2h           //[PtrCurVRAM] = (r2 | (nextbpcdbbbyte << 8)), PtrCurVRAM = PtrCurVRAM + 2
                    }
                    022EC740 E1871407 orr     r1,r7,r7,lsl 8h           //r1 = nextbpcdbbbyte | (nextbpcdbbbyte << 8)
                    022EC744 E1A01801 mov     r1,r1,lsl 10h             //r1 = r1 << 16
                    022EC748 E1A02821 mov     r2,r1,lsr 10h             //r2 = r1 >> 16 (Make unsigned int16)
                    if( r3 != 0 )
                    {
                        022EC74C 13A03000 movne   r3,0h                 //r3 = 0
                        022EC750 12400001 subne   r0,r0,1h              //r0 = nbtowrite - 1
                    }
                    022EC754 E3A01000 mov     r1,0h                     //r1 = CNT_6 = 0
                    022EC758 EA000001 b       22EC764h              ///022EC764 v
                    
                    while( CNT_6 < nbtowrite ))
                    {
                        022EC768 BAFFFFFB blt     22EC75Ch              ///022EC75C ^ 
                        ///022EC75C
                        022EC75C E0CB20B2 strh    r2,[r11],2h               //[PtrCurVRAM] = r2, r11 = PtrCurVRAM + 2
                        022EC760 E2811002 add     r1,r1,2h                  //r1 = CNT_6 + 1
                        ///022EC764
                        022EC764 E1510000 cmp     r1,r0
                    }
                    if( CNT_6 <= nbtowrite )
                    {
                        022EC76C D3A03001 movle   r3,1h                 //r3 = 1
                        022EC770 D1A02007 movle   r2,r7                 //r2 = nextbpcdbbbyte
                    }
                }
                else if( cmdbyte == 0x7F )
                {
                    022EC668 E350007F cmp     r0,7Fh
                    022EC66C 05D1C001 ldreqb  r12,[r1,1h]           //r12 = ThirdByteAfterCur
                    022EC670 05D10000 ldreqb  r0,[r1]               //r0  = SecondByteAfterCur
                    022EC674 02816002 addeq   r6,r1,2h              //r6  = PtrFileReadPos + 2
                    022EC678 0180040C orreq   r0,r0,r12,lsl 8h      //r0  = SecondByteAfterCur | (ThirdByteAfterCur << 8)
                    022EC67C 0A000002 beq     22EC68Ch              ///022EC68C v
                }
                else if( cmdbyte == 0x7E )
                {
                    022EC680 E350007E cmp     r0,7Eh
                    022EC684 05D10000 ldreqb  r0,[r1]
                    022EC688 02816001 addeq   r6,r1,1h
                }
                ///022EC68C
                022EC68C E3530000 cmp     r3,0h                 //r3 is a boolean flag triggered by some command bytes
                if( r3 != 0 )
                {
                    022EC690 14D61001 ldrneb  r1,[r6],1h            //r1 = [PtrFileReadPos], ++PtrFileReadPos
                    022EC694 13A03000 movne   r3,0h                 //r3 = 0
                    022EC698 12400001 subne   r0,r0,1h              //r0 = cmdbyte - 1 = nbtowrite
                    022EC69C 11821401 orrne   r1,r2,r1,lsl 8h       //r1 = r2 | (r1 << 8)
                    022EC6A0 10CB10B2 strneh  r1,[r11],2h           //[PtrCurVRAM] = r1, r11 = PtrCurVRAM + 2
                }
                022EC6A4 E3A01000 mov     r1,0h                     //r1 = CntCopy = 0
                022EC6A8 EA000005 b       22EC6C4h              ///022EC6C4 v

                while( CntCopy < nbtowrite )
                {
                    ///022EC6C4
                    022EC6C4 E1510000 cmp     r1,r0
                    022EC6C8 BAFFFFF7 blt     22EC6ACh              ///022EC6AC ^
                    ///022EC6AC - Copy the next 16bits word!
                    022EC6AC E5D6E001 ldrb    r14,[r6,1h]               //r14 = [PtrFileReadPos + 1]
                    022EC6B0 E4D6C002 ldrb    r12,[r6],2h               //r12 = [PtrFileReadPos], PtrFileReadPos += 2
                    022EC6B4 E2811002 add     r1,r1,2h                  //r1 = CntCopy + 2
                    022EC6B8 E58DC02C str     r12,[r13,2Ch]             //[r13,2Ch] = r12
                    022EC6BC E18CC40E orr     r12,r12,r14,lsl 8h        //r12 = r12 | (r14 << 8)
                    022EC6C0 E0CBC0B2 strh    r12,[r11],2h              //[PtrCurVRAM] = r12, PtrCurVRAM += 2 
                }
                
                if( CntCopy <= nbtowrite )
                {
                    022EC6CC D3A03001 movle   r3,1h             //r3 = 1
                    022EC6D0 D4D62001 ldrleb  r2,[r6],1h        //r2 = [PtrFileReadPos], ++PtrFileReadPos   
                }
                022EC6D4 EA000026 b       22EC774h              ///022EC774 v
            }
            ///
            /// 022EC780 - skip padding at the end of the compressed image if offset is odd
            ///
            022EC780 E59D0024 ldr     r0,[r13,24h]              //r0 = PtrBPC_DBB
            022EC784 E1D970F0 ldrsh   r7,[r9]                   //r7 = CurBpcEntry_NbTiles
            022EC788 E0460000 sub     r0,r6,r0                  //r0 = PtrFileReadPos - PtrBPC_DBB = BPC_ImageCompressedLen
            022EC78C E3100001 tst     r0,1h
            if( (BPC_ImageCompressedLen & 1) != 0 )
            {
                022EC790 12866001 addne   r6,r6,1h              //r6 = PtrFileReadPos +1
            }
            022EC794 E28A0C02 add     r0,r10,200h               //r0 = r10 + 0x200
            022EC798 E59F257C ldr     r2,=0FFFFh                //r2 = 0xFFFF
            022EC79C E3A03000 mov     r3,0h                     //r3 = 0    
            022EC7A0 EA000005 b       22EC7BCh              ///022EC7BC v
            //
            //This fills the remaining space after the BPC entry's image in the VRAM with 0xFFFF!
            //
            while( CntTiles < ScreenNbTiles )
            {
                ///022EC7BC
                022EC7BC E1D01CF8 ldrsh   r1,[r0,0C8h]        //r1 = ScreenNbTiles = (Was 0x400, which is 1024. There's 1024 tiles on the NDS screen.)
                022EC7C0 E1570001 cmp     r7,r1                           
                022EC7C4 BAFFFFF6 blt     22EC7A4h            ///022EC7A4 VRAM_PADDING_LOOP ^
                //
                ///022EC7A4 VRAM_PADDING_LOOP - This nulls out the unused space in video memory after the tileset!
                //
                022EC7A4 E1A01003 mov     r1,r3             //r1 = CntTileWords = 0
                while( CntTileWords < 16 )                  //There's 16 words per tiles, aka 32 bytes, or 64 pixels!
                {
                    ///022EC7A8
                    022EC7A8 E0CB20B2 strh    r2,[r11],2h       //[PtrCurVRAM] = 0xFFFF, PtrCurVRAM += 2
                    022EC7AC E2811001 add     r1,r1,1h          //r1 = CntTileWords + 1
                    022EC7B0 E3510010 cmp     r1,10h
                    022EC7B4 BAFFFFFB blt     22EC7A8h      ///022EC7A8 ^
                }
                022EC7B8 E2877001 add     r7,r7,1h              //r7 = CntWords + 1
            }
            022EC7C8 E28A0C02 add     r0,r10,200h
            022EC7CC E08A1108 add     r1,r10,r8,lsl 2h          //r1 = r10 + (CntBpcEntry * 4)
            022EC7D0 E591C2DC ldr     r12,[r1,2DCh]             //r12 = AddressOfBlockContainingTheCopiedBPCHeaderData
            022EC7D4 E1D07CF2 ldrsh   r7,[r0,0C2h]              //(was 0)
            022EC7D8 E1D02CF6 ldrsh   r2,[r0,0C6h]              //(was 0)
            022EC7DC E3A01001 mov     r1,1h                     //r1 = 1
            022EC7E0 E1A0300C mov     r3,r12                    //r3 = r12 = PtrTMapData
            022EC7E4 E1C01BB8 strh    r1,[r0,0B8h]              //
            022EC7E8 E1820607 orr     r0,r2,r7,lsl 0Ch          //r0 = r2 | (r7 << 12)
            022EC7EC E1A00800 mov     r0,r0,lsl 10h             //r0 = r0 << 16
            022EC7F0 E1D910FA ldrsh   r1,[r9,0Ah]               //r1 = BPC_DBA_Unk6
            022EC7F4 E58D003C str     r0,[r13,3Ch]              //[r13,3Ch] = r0
            022EC7F8 E2410001 sub     r0,r1,1h                  //r0 = BPC_DBA_Unk6 - 1
            022EC7FC E0800180 add     r0,r0,r0,lsl 3h           //r0 = (BPC_DBA_Unk6 - 1) + ((BPC_DBA_Unk6 - 1) * 8)
            022EC800 E08C7080 add     r7,r12,r0,lsl 1h          //r7 = r12 + (r0 * 2) = PtrEndBpcEntr
            022EC804 E3A00000 mov     r0,0h                     //r0 = 0
            022EC808 EA000020 b       22EC890h                  ///022EC890 v
            ///
            ///022EC890 LOOP_BEG_TMDecompPass1 - This is the first pass of the decompression process for the tilemapping data that follows the image
            ///
            while( PtrWritePos < PtrWriteEndPos ) (unsigned!)
            {
                022EC890 E1530007 cmp     r3,r7
                022EC894 3AFFFFDC bcc     22EC80Ch              ///022EC80C ^
                ///022EC80C 
                022EC80C E5D61000 ldrb    r1,[r6]               //r1 = [PtrFileReadPos] = curbyte
                022EC810 E2862001 add     r2,r6,1h              //r2 = PtrFileReadPos + 1
                022EC814 E1A06002 mov     r6,r2                 //r6 = (PtrFileReadPos + 1)
                022EC818 E35100C0 cmp     r1,0C0h
                if( curbyte < 0xC0 )
                {
                    022EC81C BA000008 blt     22EC844h              ///022EC844 v
                    022EC844 E3510080 cmp     r1,80h
                    if( curbyte < 0x80 )
                    {
                        //
                        // Here we zero the amount of words specified in curbyte
                        //
                        022EC848 BA00000A blt     22EC878h              ///022EC878 v
                        022EC878 E3A02000 mov     r2,0h                 //r2 = Cnt = 0
                        022EC87C EA000001 b       22EC888h              ///022EC888 v
                        ///022EC888
                        022EC888 E1520001 cmp     r2,r1
                        while( Cnt <= curbyte )
                        {
                            022EC88C DAFFFFFB ble     22EC880h              ///022EC880 ^
                            ///022EC880
                            022EC880 E0C300B2 strh    r0,[r3],2h            //[PtrWritePos] = 0, PtrWritePos += 2
                            022EC884 E2822001 add     r2,r2,1h              //r2 = Cnt + 1
                        }
                    }
                    else
                    {
                        //
                        // Here we write an unsigned byte at [r2] as the high byte of the specified nb of words.
                        //
                        022EC84C E5D2B000 ldrb    r11,[r2]              //r11 = [PtrFileReadPos]
                        022EC850 E2826001 add     r6,r2,1h              //r6  = r2 + 1
                        022EC854 E3A02080 mov     r2,80h                //r2  = 0x80
                        022EC858 E1A0BC0B mov     r11,r11,lsl 18h       //r11 = [PtrFileReadPos] << 24
                        022EC85C E1A0B82B mov     r11,r11,lsr 10h       //r11 = ([PtrFileReadPos] << 24) >> 16 (This clears the sign bits)
                        022EC860 EA000001 b       22EC86Ch              ///022EC86C v
                        ///022EC86C
                        022EC86C E1520001 cmp     r2,r1
                        while( r2 <= curbyte )
                        {
                            022EC870 DAFFFFFB ble     22EC864h          ///022EC864 ^
                            ///022EC864
                            022EC864 E0C3B0B2 strh    r11,[r3],2h           //[PtrWritePos] = r11, PtrWritePos += 2
                            022EC868 E2822001 add     r2,r2,1h              //r2 = r2 + 1
                        }
                        022EC874 EA000005 b       22EC890h              ///022EC890 v
                        break;
                    }
                }
                else
                {
                    //
                    // Here we write a sequence of signed bytes as the high byte of the specified amount of words.
                    //
                    022EC820 E3A020C0 mov     r2,0C0h               //r2 = 0xC0 = CntWords
                    022EC824 EA000003 b       22EC838h              ///022EC838 v
                    ///022EC838
                    022EC838 E1520001 cmp     r2,r1
                    while( r2 <= curbyte )
                    {
                        022EC83C DAFFFFF9 ble     22EC828h          ///022EC828 ^
                        ///022EC828
                        022EC828 E4D6B001 ldrb    r11,[r6],1h               //r11 = [PtrFileReadPos], PtrFileReadPos += 1
                        022EC82C E2822001 add     r2,r2,1h                  //r2  = CntWords + 1
                        022EC830 E1A0B40B mov     r11,r11,lsl 8h            //r11 = [PtrFileReadPos] << 8
                        022EC834 E0C3B0B2 strh    r11,[r3],2h               //[PtrWritePos] = ([PtrFileReadPos] << 8), PtrWritePos += 2
                    }
                    022EC840 EA000012 b       22EC890h              ///022EC890 v
                }
            }
            022EC898 E1A0300C mov     r3,r12                //r3 = StartOfTheBufferWeJustWrote
            022EC89C EA000021 b       22EC928h              ///022EC928 v
            //
            ///022EC928 LOOP_BEG_TMDecompPass2 - Second pass for decompressing the tile mapping data
            //
            022EC928 E1530007 cmp     r3,r7
            while( PtrWritePos < PtrWriteEndPos )
            {
                022EC92C 3AFFFFDB bcc     22EC8A0h              ///022EC8A0 ^
                ///022EC8A0
                022EC8A0 E5D62000 ldrb    r2,[r6]               //r2 = [PtrFileReadPos] = curbyte
                022EC8A4 E2860001 add     r0,r6,1h              //r0 = (PtrFileReadPos + 1)
                022EC8A8 E1A06000 mov     r6,r0                 //r6 = (PtrFileReadPos + 1)
                022EC8AC E35200C0 cmp     r2,0C0h               
                if( curbyte < 0xC0 )
                {
                    022EC8B0 BA00000B blt     22EC8E4h              ///022EC8E4 v
                    ///022EC8E4
                    022EC8E4 E3520080 cmp     r2,80h
                    if( curbyte < 0x80 )
                    {
                        //
                        // Here we increment the write position by the value of curbyte.
                        //
                        022EC8E8 BA00000C blt     22EC920h              ///022EC920 v
                        ///022EC920
                        022EC920 E2820001 add     r0,r2,1h              //r0 = curbyte + 1
                        022EC924 E0833080 add     r3,r3,r0,lsl 1h       //r3 = PtrWritePos + (r0 * 2)
                    }
                    else
                    {
                        //
                        // Here we seem to just bitwise OR the byte parameter we read to the specified amount of words at "PtrWritePos".
                        //
                        022EC8EC E2806001 add     r6,r0,1h              //r6  = (PtrFileReadPos + 1) + 1
                        022EC8F0 E5D0E000 ldrb    r14,[r0]              //r14 = (PtrFileReadPos] = CurParamByte
                        022EC8F4 E3A0B080 mov     r11,80h               //r11 = 0x80 = CntWord
                        022EC8F8 EA000005 b       22EC914h              ///022EC914 v
                        ///022EC914
                        022EC914 E15B0002 cmp     r11,r2
                        while( CntWord <= curbyte )
                        {
                            022EC918 DAFFFFF7 ble     22EC8FCh              ///022EC8FC ^
                            ///022EC8FC
                            022EC8FC E58D3008 str     r3,[r13,8h]       //[r13,8h] = r3 (Save write address before)
                            022EC900 E0D300B2 ldrh    r0,[r3],2h        //r0  = [PtrWritePos], PtrWritePos += 2
                            022EC904 E28BB001 add     r11,r11,1h        //r11 = CntWord + 1
                            022EC908 E180100E orr     r1,r0,r14         //r1  = CurWord | CurParamByte
                            022EC90C E59D0008 ldr     r0,[r13,8h]       //r0  = [r13,8h] (Get last write address before read!)
                            022EC910 E1C010B0 strh    r1,[r0]           //[r0]= (CurWord | CurParamByte)
                        }
                        022EC91C EA000001 b       22EC928h              ///022EC928 v
                    }
                }
                else
                {
                    //
                    // Here we seem to just bitwise OR the bytes from the file on the specified words starting from "PtrWritePos".
                    //
                    022EC8B4 E3A000C0 mov     r0,0C0h               //r0 = 0xC0 = CntWord
                    022EC8B8 EA000006 b       22EC8D8h              ///022EC8D8 v
                    ///022EC8D8
                    022EC8D8 E1500002 cmp     r0,r2
                    while( CntWord <= curbyte )
                    {
                        022EC8DC DAFFFFF6 ble     22EC8BCh              ///022EC8BC ^
                        ///022EC8BC
                        022EC8BC E58D300C str     r3,[r13,0Ch]          //[r13,0Ch] = r3 (Save write address before)
                        022EC8C0 E0D3B0B2 ldrh    r11,[r3],2h           //r11 = [PtrWritePos], PtrWritePos += 2
                        022EC8C4 E4D61001 ldrb    r1,[r6],1h            //r1  = [PtrFileReadPos], PtrFileReadPos += 1
                        022EC8C8 E2800001 add     r0,r0,1h              //r0  = CntWord + 1
                        022EC8CC E18BB001 orr     r11,r11,r1            //r11 = [PtrWritePos] | [PtrFileReadPos]
                        022EC8D0 E59D100C ldr     r1,[r13,0Ch]          //r1  = [r13,0Ch] (Get last write address before read!)
                        022EC8D4 E1C1B0B0 strh    r11,[r1]              //[r1]= [PtrWritePos] | [PtrFileReadPos]
                    }
                    022EC8E0 EA000010 b       22EC928h              ///022EC928 v
                }
            }
            022EC930 E1D900FA ldrsh   r0,[r9,0Ah]           //r0 = BPC_DBA_Unk6
            022EC934 E3A03001 mov     r3,1h                 //r3 = 1
            022EC938 E0800180 add     r0,r0,r0,lsl 3h       //r0 = BPC_DBA_Unk6 + (BPC_DBA_Unk6 * 8) 
            022EC93C E08C1080 add     r1,r12,r0,lsl 1h      //r1 = StartOfTheBufferWeJustWrote + ((BPC_DBA_Unk6 + (BPC_DBA_Unk6 * 8)) * 2)
            022EC940 E3A00000 mov     r0,0h                 //r0 = 0
            022EC944 EA000008 b       22EC96Ch              ///022EC96C v
            ///
            ///022EC96C - Add a value to all tile info words in memory, from the end to the beginning (tile index and palette shift, maybe?)
            ///
            while( CntWords < BPC_DBA_Unk6 )
            {
                022EC96C E1D920FA ldrsh   r2,[r9,0Ah]           //r2 = BPC_DBA_Unk6
                022EC970 E1530002 cmp     r3,r2
                022EC974 BAFFFFF3 blt     22EC948h              ///022EC948 ^
                ///022EC948
                022EC948 E1A02000 mov     r2,r0                 //r2 = CNT_TileInf = 0
                while( CNT_TileInf < 9 )
                {
                    ///022EC94C
                    022EC94C E177B0B2 ldrh    r11,[r7,-2h]!         //r11 = curword = [PtrWriteEndPos - 2], PtrWriteEndPos -= 2
                    022EC950 E59D603C ldr     r6,[r13,3Ch]          //r6  = ([Address,0C6h] | ([Address,0C2h] << 12)) << 16 (Were both zero)
                    022EC954 E2822001 add     r2,r2,1h              //r2  = CNT_TileInf + 1
                    022EC958 E08B6826 add     r6,r11,r6,lsr 10h     //r6  = curword + (r6 >> 16)
                    022EC95C E3520009 cmp     r2,9h
                    022EC960 E16160B2 strh    r6,[r1,-2h]!          //[PtrNewWritePos,-2h] = r6, PtrNewWritePos -= 2
                    022EC964 BAFFFFF8 blt     22EC94Ch              ///022EC94C ^
                }
                022EC968 E2833001 add     r3,r3,1h                  //r3 = CntWords + 1
            }

            022EC978 E3A01000 mov     r1,0h                 //r1 = 0
            022EC97C E1A00001 mov     r0,r1                 //r0 = 0
            ///
            ///022EC980 - Zero out the first 18 bytes of the tile mapping data in memory
            ///
            while( r1 < 9)
            {
                022EC98C BAFFFFFB blt     22EC980h              ///022EC980 ^
                022EC980 E2811001 add     r1,r1,1h              //r1 = r1 + 1
                022EC984 E3510009 cmp     r1,9h                 
                022EC988 E0CC00B2 strh    r0,[r12],2h           //[StartOfTheBufferWeJustWrote] = r0, StartOfTheBufferWeJustWrote += 2
            }
        }
        //Continue loop at 022EC9A0...
    }

    ///
    ///022EC9B0 - Parsing BMA
    ///
    022EC9B0 E28A0FB9 add     r0,r10,2E4h           //r0 = PtrTablePtrMappingData? =r10 + 0x2E4 (Possibly destination struct?)
    022EC9B4 E285100C add     r1,r5,0Ch             //r1 = PtrBMABuff + 12 (Skips header) //PtrBMAData
    022EC9B8 E28A2FAF add     r2,r10,2BCh           //r2 = r10 + 0x2BC
    022EC9BC E28A3E1E add     r3,r10,1E0h           //r3 = r10 + 0x1E0 //PtrBMAHeaderCopy
    022EC9C0 EB000643 bl      22EE2D4h              ///fun_022EE2D4
    ///Function 22EE2D4h
    022EE2D4 E92D4FF8 push    r3-r11,r14
    022EE2D8 E24DD010 sub     r13,r13,10h           //Alloc 16 bytes on stack
    022EE2DC E3A05000 mov     r5,0h                 //r5 = Cnt = 0
    022EE2E0 EA0000B8 b       22EE5C8h              ///022EE5C8 v
    while( Cnt < r4 )
    {
        ///022EE5C8
        022EE5C8 E1D240F4 ldrsh   r4,[r2,4h]            //r4 = [r2,4h] AKA [r10,2C0h](was 2) Probably the NB of BPC entries
        022EE5CC E1550004 cmp     r5,r4
        //***LOOP CONDITION EVALUATED HERE***
        022EE5D0 BAFFFF43 blt     22EE2E4h              ///022EE2E4 LOOP_BEG_BMA
        ///022EE2E4 LOOP_BEG_BMA
        022EE2E4 E0444005 sub     r4,r4,r5              //r4  = r4 - Cnt
        022EE2E8 E0804104 add     r4,r0,r4,lsl 2h       //r4  = PtrTablePtrMappingData? + (r4 * 4)
        022EE2EC E514A004 ldr     r10,[r4,-4h]          //r10 = [r4 - 4] ( Was 0x for r4 == 1, Was 0x for r4 == 0x2102044 )
        022EE2F0 E3A07000 mov     r7,0h                 //r7 = Cnt1 = 0
        022EE2F4 EA0000A3 b       22EE588h              ///022EE588 v
        
        while( Cnt1 < BMA_unk4 )
        {
            ///022EE588
            022EE588 E5D34003 ldrb    r4,[r3,3h]            //r4 = BMA_unk4
            022EE58C E1570004 cmp     r7,r4             
            //***LOOP CONDITION EVALUATED HERE***
            022EE590 BAFFFF58 blt     22EE2F8h              ///022EE2F8 LOOP_BEG_1
            ///022EE2F8 LOOP_BEG_1
            022EE2F8 E3570000 cmp     r7,0h
            022EE2FC E3A06000 mov     r6,0h                 //r6 = Cnt2 = 0
            if( Cnt1 != 0 )
            {
                022EE300 1A000042 bne     22EE410h          ///022EE410 v
                ///022EE410
                022EE410 E24A9080 sub     r9,r10,80h        //r9 = r10 - 0x80
                022EE414 EA000051 b       22EE560h          ///022EE560 v
                while(r6 < BMA_unk3)
                {
                    ///022EE560
                    022EE560 E5D34002 ldrb    r4,[r3,2h]            //r4 = BMA_unk3
                    022EE564 E1560004 cmp     r6,r4
                    //***LOOP CONDITION EVALUATED HERE***
                    022EE568 BAFFFFAA blt     22EE418h          ///022EE418 LOOP_BEG_7 ^
                    ///022EE418 LOOP_BEG_7
                    022EE418 E4D1B001 ldrb    r11,[r1],1h           //r11 = curbyte2 = [PtrReadBMADataPos], PtrReadBMADataPos += 1
                    022EE41C E35B00C0 cmp     r11,0C0h
                    if( curbyte2 < 0xC0 )
                    {
                        022EE420 BA00001D blt     22EE49Ch          ///022EE49C
                        ///022EE49C
                        022EE49C E35B0080 cmp     r11,80h
                        if( curbyte2 < 0x80 )
                        {
                            022EE4A0 BA00001F blt     22EE524h              ///022EE524 v
                            ///022EE524
                            022EE524 E3A04000 mov     r4,0h                     //r4 = 0
                            022EE528 EA000008 b       22EE550h                  ///022EE550 v
                            while(r4 <= r11)
                            {
                                ///022EE550
                                022EE550 E154000B cmp     r4,r11
                                022EE554 DAFFFFF4 ble     22EE52Ch          ///022EE52C ^
                                ///022EE52C LOOP_BEG_8
                                022EE52C E1D9C0B0 ldrh    r12,[r9]              //r12 = [r9]
                                022EE530 E2898002 add     r8,r9,2h              //r8  = r9 + 2
                                022EE534 E2899004 add     r9,r9,4h              //r9  = r9 + 4
                                022EE538 E1CAC0B0 strh    r12,[r10]             //[r10] = r12
                                022EE53C E1D880B0 ldrh    r8,[r8]               //r8  = [r8]
                                022EE540 E28AC002 add     r12,r10,2h            //r12 = r10 + 2
                                022EE544 E28AA004 add     r10,r10,4h            //r10 = r10 + 4
                                022EE548 E1CC80B0 strh    r8,[r12]              //[r12] = r8
                                022EE54C E2844001 add     r4,r4,1h              //r4 = r4 + 1
                            }
                            022EE558 E28B4001 add     r4,r11,1h
                            022EE55C E0866084 add     r6,r6,r4,lsl 1h
                            //Continue Loop...
                        }
                        else
                        {
                            022EE4A4 E5D14000 ldrb    r4,[r1]                   //r4  = [PtrReadBMADataPos]
                            022EE4A8 E5D1E001 ldrb    r14,[r1,1h]               //r14 = [PtrReadBMADataPos + 1]   
                            022EE4AC E5D18002 ldrb    r8,[r1,2h]                //r8  = [PtrReadBMADataPos + 2]    
                            022EE4B0 E2811003 add     r1,r1,3h                  //r1  = PtrReadBMADataPos + 3
                            022EE4B4 E184440E orr     r4,r4,r14,lsl 8h          //r4  = [PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)
                            022EE4B8 E1844808 orr     r4,r4,r8,lsl 10h          //r4  = ([PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)) | ([PtrReadBMADataPos + 2]  << 16)
                            022EE4BC E59F811C ldr     r8,=0FFFh                 //r8  = 0x0FFF
                            022EE4C0 E3A0C080 mov     r12,80h                   //r12 = 0x80
                            022EE4C4 E0048008 and     r8,r4,r8                  //r8  = (([PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)) | ([PtrReadBMADataPos + 2]  << 16)) & 0x0FFF
                            022EE4C8 E58D8000 str     r8,[r13]                  //[r13] = AssembledDWord = ((([PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)) | ([PtrReadBMADataPos + 2]  << 16)) & 0x0FFF)
                            022EE4CC E59F810C ldr     r8,=0FFFh                 //r8  = 0x0FFF    
                            022EE4D0 E0084644 and     r4,r8,r4,asr 0Ch          //r4  = 0x0FFF &(([PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)) | ([PtrReadBMADataPos + 2]  << 16)) >> 12) (SIGNED Shift Right)
                            022EE4D4 E58D4008 str     r4,[r13,8h]               //[r13,8h] = Assembled12BitsInt = (0x0FFF &(([PtrReadBMADataPos] | ([PtrReadBMADataPos + 1] << 8)) | ([PtrReadBMADataPos + 2]  << 16)) >> 12) (SIGNED Shift Right))
                            022EE4D8 EA00000C b       22EE510h                  ///022EE510 v
                            while(r12 <= r11)
                            {
                                ///022EE510
                                022EE510 E15C000B cmp     r12,r11
                                022EE514 DAFFFFF0 ble     22EE4DCh              ///022EE4DC ^
                                ///022EE4DC
                                022EE4DC E1D980B0 ldrh    r8,[r9]                   //r8  = [r9]
                                022EE4E0 E59D4000 ldr     r4,[r13]                  //r4  = AssembledDWord
                                022EE4E4 E289E002 add     r14,r9,2h                 //r14 = r9 + 2
                                022EE4E8 E0284004 eor     r4,r8,r4                  //r4  = r8 ^ AssembledDWord
                                022EE4EC E1CA40B0 strh    r4,[r10]                  //[r10] = r4
                                022EE4F0 E1DEE0B0 ldrh    r14,[r14]                 //r14 = [r14]
                                022EE4F4 E59D4008 ldr     r4,[r13,8h]               //r4  = Assembled12BitsInt
                                022EE4F8 E28A8002 add     r8,r10,2h                 //r8  = r10 + 2
                                022EE4FC E02E4004 eor     r4,r14,r4                 //r4  = r14 ^ Assembled12BitsInt
                                022EE500 E2899004 add     r9,r9,4h                  //r9  = r9 + 4
                                022EE504 E28AA004 add     r10,r10,4h                //r10 = r10 + 4    
                                022EE508 E1C840B0 strh    r4,[r8]                   //[r8] = r4
                                022EE50C E28CC001 add     r12,r12,1h                //r12 = r12 + 1
                            }
                            022EE518 E24B407F sub     r4,r11,7Fh                //r4 = r11 - 0x7F
                            022EE51C E0866084 add     r6,r6,r4,lsl 1h           //r6 = r6 + (r4 * 2)
                            022EE520 EA00000E b       22EE560h                  ///022EE560 v
                            //Continue Loop...
                        }
                    }
                    else
                    {
                        022EE424 E3A0C0C0 mov     r12,0C0h              //r12 = 0xC0
                        022EE428 EA000016 b       22EE488h              ///022EE488 v
                        while( r12 <= curbyte2 )
                        {
                            ///022EE488
                            022EE488 E15C000B cmp     r12,r11
                            022EE48C DAFFFFE6 ble     22EE42Ch              ///022EE42C 
                            ///022EE42C
                            022EE42C E5D18001 ldrb    r8,[r1,1h]            //r8  = [PtrReadBMADataPos + 1]
                            022EE430 E5D1E000 ldrb    r14,[r1]              //r14 = [PtrReadBMADataPos]
                            022EE434 E5D14002 ldrb    r4,[r1,2h]            //r4  = [PtrReadBMADataPos + 2]
                            022EE438 E58D800C str     r8,[r13,0Ch]          //[r13,0Ch] = r8
                            022EE43C E1D980B0 ldrh    r8,[r9]               //r8  = [r9]
                            022EE440 E2811003 add     r1,r1,3h              //r1  = r1 + 3
                            022EE444 E28CC001 add     r12,r12,1h            //r12 = r12 + 1
                            022EE448 E58D8004 str     r8,[r13,4h]           //[r13,4h] = r8
                            022EE44C E59D800C ldr     r8,[r13,0Ch]          //r8  = [r13 + 0xC]
                            022EE450 E18E8408 orr     r8,r14,r8,lsl 8h      //r8  = r14 | (r8 << 8)
                            022EE454 E188E804 orr     r14,r8,r4,lsl 10h     //r14 = r8  | (r4 << 16)
                            022EE458 E59F4180 ldr     r4,=0FFFh             //r4  = 0x0FFF
                            022EE45C E00E8004 and     r8,r14,r4             //r8  = r14 & 0x0FFF
                            022EE460 E59D4004 ldr     r4,[r13,4h]           //[r13,4h] = r4
                            022EE464 E0244008 eor     r4,r4,r8              //r4  = r4 ^ r8
                            022EE468 E1CA40B0 strh    r4,[r10]              //[r10] = r4
                            022EE46C E59F416C ldr     r4,=0FFFh             //r4  = 0x0FFF
                            022EE470 E004E64E and     r14,r4,r14,asr 0Ch    //r14 = r4 & (r14 >> 12) (SIGNED Shift Right)    
                            022EE474 E1D940B2 ldrh    r4,[r9,2h]            //r4  = [r9 + 2]
                            022EE478 E2899004 add     r9,r9,4h              //r9  = r9 + 4
                            022EE47C E024400E eor     r4,r4,r14             //r4  = r4 ^ r14
                            022EE480 E1CA40B2 strh    r4,[r10,2h]           //[r10,2h] = r4
                            022EE484 E28AA004 add     r10,r10,4h            //r10 = r10 + 4
                        }
                        022EE490 E24B40BF sub     r4,r11,0BFh               //r4 = curbyte2 - 0xBF
                        022EE494 E0866084 add     r6,r6,r4,lsl 1h           //r6 = r6 + (r4 * 2)
                        022EE498 EA000030 b       22EE560h                  ///022EE560 v
                        //Continue Loop...
                    }
                }// while(r6 < BMA_unk3) END
                022EE56C EA000002 b       22EE57Ch              ///022EE57C v
            }
            else
            {
                022EE304 EA00003D b       22EE400h          ///022EE400 v
                while(r6 < BMA_unk3)
                {
                    ///022EE400
                    022EE400 E5D34002 ldrb    r4,[r3,2h]            //r4 = BMA_unk3 (was 0xF)
                    022EE404 E1560004 cmp     r6,r4
                    //***LOOP CONDITION EVALUATED HERE***
                    022EE408 BAFFFFBE blt     22EE308h              ///022EE308 LOOP_BEG_2
                    ///022EE308 LOOP_BEG_2
                    022EE308 E4D19001 ldrb    r9,[r1],1h            //r9 = curbyte = [PtrReadBMADataPos], PtrReadBMADataPos += 1
                    022EE30C E35900C0 cmp     r9,0C0h
                    if( curbyte < 0xC0 )
                    {
                        022EE310 BA000014 blt     22EE368h          ///022EE368 v
                        ///022EE368 curbyte < 0xC0
                        022EE368 E3590080 cmp     r9,80h
                        if( curbyte < 0x80 )
                        {
                            022EE36C BA000018 blt     22EE3D4h              //022EE3D4 curbyte < 0x80 v
                            ///022EE3D4 curbyte < 0x80
                            022EE3D4 E3A0B000 mov     r11,0h                //r11 = CntWords = 0
                            022EE3D8 EA000004 b       22EE3F0h              ///022EE3F0 v
                            ///022EE3F0 
                            022EE3F0 E15B0009 cmp     r11,r9
                            while( CntWords <= curbyte )
                            {
                                022EE3F4 DAFFFFF8 ble     22EE3DCh          ///022EE3DC ^
                                ///022EE3DC
                                022EE3DC E3A04000 mov     r4,0h                 //r4       = 0
                                022EE3E0 E1CA40B0 strh    r4,[r10]              //[r10]    = 0
                                022EE3E4 E1CA40B2 strh    r4,[r10,2h]           //[r10,2h] = 0
                                022EE3E8 E28AA004 add     r10,r10,4h            //r10      = r10 + 4
                                022EE3EC E28BB001 add     r11,r11,1h            //r11      = CntWords + 1
                            }
                            022EE3F8 E2894001 add     r4,r9,1h              //r4 = curbyte + 1
                            022EE3FC E0866084 add     r6,r6,r4,lsl 1h       //r6 = r6 + (curbyte * 2)
                            //ContinueLoop
                        }
                        else
                        {
                            022EE370 E5D1C000 ldrb    r12,[r1]              //r12 = [PtrReadBMADataPos]
                            022EE374 E5D1B001 ldrb    r11,[r1,1h]           //r11 = [PtrReadBMADataPos + 1]
                            022EE378 E5D18002 ldrb    r8,[r1,2h]            //r8  = [PtrReadBMADataPos + 2]
                            022EE37C E2811003 add     r1,r1,3h              //r1  = PtrReadBMADataPos + 3
                            022EE380 E18CB40B orr     r11,r12,r11,lsl 8h    //r11 = r12 | (r11 << 8)    
                            022EE384 E18BB808 orr     r11,r11,r8,lsl 10h    //r11 = r11 | (r8  << 16)
                            022EE388 E59F8250 ldr     r8,=0FFFh             //r8  = 0x0FFF
                            022EE38C E59FC24C ldr     r12,=0FFFh            //r12 = 0x0FFF
                            022EE390 E00B8008 and     r8,r11,r8             //r8  = r11 & r8
                            022EE394 E00CB64B and     r11,r12,r11,asr 0Ch   //r11 = r12 & (r11 >> 12) (SIGNED Shift Right)
                            022EE398 E1A08808 mov     r8,r8,lsl 10h         //r8  = r8  << 16
                            022EE39C E1A0B80B mov     r11,r11,lsl 10h       //r11 = r11 << 16
                            022EE3A0 E3A04080 mov     r4,80h                //r4  = 0x80
                            022EE3A4 E1A08828 mov     r8,r8,lsr 10h         //r8  = r8  >> 16
                            022EE3A8 E1A0B82B mov     r11,r11,lsr 10h       //r11 = r11 >> 16
                            022EE3AC EA000003 b       22EE3C0h              ///022EE3C0 v
                            while(r4 < curbyte)
                            {
                                ///022EE3C0
                                022EE3C0 E1540009 cmp     r4,r9
                                022EE3C4 DAFFFFF9 ble     22EE3B0h          ///022EE3B0 LOOP_BEG_6 ^
                                ///022EE3B0 LOOP_BEG_6
                                022EE3B0 E1CA80B0 strh    r8,[r10]              //[r10]     = r8
                                022EE3B4 E1CAB0B2 strh    r11,[r10,2h]          //[r10 + 2] = r11
                                022EE3B8 E28AA004 add     r10,r10,4h            //r10 = r10 + 4
                                022EE3BC E2844001 add     r4,r4,1h              //r4 = r4 + 1
                            }
                            022EE3C8 E249407F sub     r4,r9,7Fh             //r4 = curbyte - 0x7F
                            022EE3CC E0866084 add     r6,r6,r4,lsl 1h       //r6 = r6 + (r4 * 2)
                            022EE3D0 EA00000A b       22EE400h              ///022EE400 ContinueLoop v
                        }
                    }
                    else
                    {
                        022EE314 E3A080C0 mov     r8,0C0h               //r8 = 0xC0
                        022EE318 EA00000D b       22EE354h              ///022EE354 v
                        while(r8 <= curbyte)
                        {
                            ///022EE354
                            022EE354 E1580009 cmp     r8,r9
                            022EE358 DAFFFFEF ble     22EE31Ch          ///022EE31C ^ 
                            ///022EE31C
                            022EE31C E5D1E000 ldrb    r14,[r1]              //r14 = [PtrReadBMADataPos]
                            022EE320 E5D1C001 ldrb    r12,[r1,1h]           //r12 = [PtrReadBMADataPos + 1]
                            022EE324 E5D14002 ldrb    r4,[r1,2h]            //r4  = [PtrReadBMADataPos + 2]
                            022EE328 E2811003 add     r1,r1,3h              //r1  = PtrReadBMADataPos + 3
                            022EE32C E18EC40C orr     r12,r14,r12,lsl 8h    //r12 = r14 | (r12 << 8)
                            022EE330 E18C4804 orr     r4,r12,r4,lsl 10h     //r4  = r12 | (r4  << 16)
                            022EE334 E59FC2A4 ldr     r12,=0FFFh            //r12 = 0x0FFF
                            022EE338 E2888001 add     r8,r8,1h              //r8  = r8 + 1
                            022EE33C E004C00C and     r12,r4,r12            //r12 = r4 & r12 
                            022EE340 E1CAC0B0 strh    r12,[r10]             //[r10] = r12
                            022EE344 E59FC294 ldr     r12,=0FFFh            //r12  = 0x0FFF
                            022EE348 E00C4644 and     r4,r12,r4,asr 0Ch     //r4   = 0x0FFF & (r4 >> 12) (SIGNED Shift Right)
                            022EE34C E1CA40B2 strh    r4,[r10,2h]           //[r10 + 2] = r4
                            022EE350 E28AA004 add     r10,r10,4h            //r10 = r10 + 4
                        }
                        022EE35C E24940BF sub     r4,r9,0BFh            //r4 = curbyte - 0xBF
                        022EE360 E0866084 add     r6,r6,r4,lsl 1h       //r6 = Cnt2 + (r4 * 2)    
                        022EE364 EA000025 b       22EE400h              ///022EE400 ContinueLoop v
                    }

                } // while(r6 < BMA_unk3) END //022EE404
                022EE40C EA00005A b       22EE57Ch              ///022EE57C v
            }

            while( r6 < 64 )
            {
                ///022EE57C
                022EE57C E3560040 cmp     r6,40h
                022EE580 BAFFFFFA blt     22EE570h          ///022EE570 LOOP_BEG_3
                ///022EE570 LOOP_BEG_3
                022EE570 E3A04000 mov     r4,0h                 //r4 = 0
                022EE574 E0CA40B2 strh    r4,[r10],2h           //[r10] = 0, r10 += 2
                022EE578 E2866001 add     r6,r6,1h              //r6 = r6 + 1
            }
            022EE584 E2877001 add     r7,r7,1h              //r7 = Cnt1 + 1    
        } //while( r7 < BMA_unk4 ) END ///022EE590
        
        022EE594 E3A06000 mov     r6,0h                 //r6 = 0
        022EE598 E1A08006 mov     r8,r6                 //r8 = 0
        022EE59C EA000005 b       22EE5B8h              ///022EE5B8 v
        while(Cnt1 < r4)
        {
            ///022EE5B8
            022EE5B8 E1D241F4 ldrsh   r4,[r2,14h]           //r4 = [r2,14h](was 0x3E)  
            022EE5BC E1570004 cmp     r7,r4
            022EE5C0 BAFFFFF6 blt     22EE5A0h          ///022EE5A0 LOOP_BEG_4 ^
            ///022EE5A0 LOOP_BEG_4
            022EE5A0 E1A04008 mov     r4,r8                 //r4 = Cnt4 = 0
            while(r4 < 64)
            {
                ///022EE5A4 LOOP_BEG_5
                022EE5A4 E2844001 add     r4,r4,1h              //r4 = r4 + 1
                022EE5A8 E3540040 cmp     r4,40h
                022EE5AC E0CA60B2 strh    r6,[r10],2h           //[r10] = 0, r10 += 2
                022EE5B0 BAFFFFFB blt     22EE5A4h          ///022EE5A4 LOOP_BEG_5
            }
            022EE5B4 E2877001 add     r7,r7,1h              //r7 = Cnt1 + 1
        }
        022EE5C4 E2855001 add     r5,r5,1h              //r5 = r5 + 1

    }
    022EE5D4 E1A00001 mov     r0,r1                 //r0 = PtrReadBMADataPos
    022EE5D8 E28DD010 add     r13,r13,10h
    022EE5DC E8BD8FF8 pop     r3-r11,r15
    022EE5E0 00000FFF 
    ///FUNCTION ENDS
    ///022EC9C4
    022EC9C4 E1A01000 mov     r1,r0                 //r1 = PtrReadBMADataPos
    022EC9C8 E58A11F0 str     r1,[r10,1F0h]         //[r10,1F0h] = PtrReadBMADataPos
    022EC9CC E59A02D8 ldr     r0,[r10,2D8h]         //r0 = [r10,2D8h] (was 0x20F6644)
    022EC9D0 E3500000 cmp     r0,0h
    if( r0 == 0 )
    {
        022EC9D4 0A000004 beq     22EC9ECh          ///022EC9EC v
    }
    else
    {
        022EC9D8 E28A2C02 add     r2,r10,200h           //r2 = r10 + 0x200
        022EC9DC E1D23CFE ldrsh   r3,[r2,0CEh]          //r3 = [r2 + 0xCE] (was 0xBA)
        022EC9E0 E59A52D4 ldr     r5,[r10,2D4h]         //r5 = [r10 + 0x2D4] (was 0x22F1800)
        022EC9E4 E1A02004 mov     r2,r4                 //r2 = r4 (PtrBMABuff)
        022EC9E8 E12FFF35 blx     r5                    ///Run r5 (Was 022F1800)
        {
        }
    }
    ///022EC9EC
    022EC9EC E3A01000 mov     r1,0h                 //r1 = 0
    022EC9F0 E59D0040 ldr     r0,[r13,40h]          //r0 = PtrBPLHeaderCopy
    022EC9F4 E5CA11EE strb    r1,[r10,1EEh]         //[r10,1EEh] = 0
    022EC9F8 E1D020F2 ldrsh   r2,[r0,2h]            //r2 = BPL_unk2
    022EC9FC E28A0004 add     r0,r10,4h             //r0 = r10 + 4
    022ECA00 E3520000 cmp     r2,0h
    if(BPL_unk2 == 0)
    {
        022ECA04 0A000026 beq     22ECAA4h          ///022ECAA4 v
        ///022ECAA4
        022ECAA4 E58A11F4 str     r1,[r10,1F4h]
        022ECAA8 E5CA11F8 strb    r1,[r10,1F8h]
        022ECAAC E5CA11F9 strb    r1,[r10,1F9h]
        022ECAB0 E5CA11FA strb    r1,[r10,1FAh]
        022ECAB4 E5CA11FB strb    r1,[r10,1FBh]
        022ECAB8 E5CA11BC strb    r1,[r10,1BCh]
    }
    else
    {
        022ECA08 E59D2040 ldr     r2,[r13,40h]          //r2 = PtrBPLHeaderCopy
        022ECA0C E3A03001 mov     r3,1h                 //r3 = 1
        022ECA10 E1D250F0 ldrsh   r5,[r2]               //r5 = nbpal
        022ECA14 E59D2028 ldr     r2,[r13,28h]          //R2 = PtrBPLBuff
        022ECA18 E1A07001 mov     r7,r1                 //r7 = r1
        022ECA1C E2824004 add     r4,r2,4h              //R4 = PtrBPLBuff + 4
        022ECA20 E0652205 rsb     r2,r5,r5,lsl 4h       //r2 = (nbpal * 16) - nbpal
        022ECA24 E0842102 add     r2,r4,r2,lsl 2h       //r2 = (PtrBPLBuff + 4) + (((nbpal * 16) - nbpal) * 4) = PtrSecPartBPL
        022ECA28 E58A21F4 str     r2,[r10,1F4h]         //[r10,1F4h] = PtrSecPartBPL
        022ECA2C E5CA31F8 strb    r3,[r10,1F8h]         //[r10,1F8h] = 1
        022ECA30 E5CA31F9 strb    r3,[r10,1F9h]         //[r10,1F9h] = 1
        022ECA34 E5CA31FA strb    r3,[r10,1FAh]         //[r10,1FAh] = 1
        022ECA38 E5CA31FB strb    r3,[r10,1FBh]         //[r10,1FBh] = 1
        022ECA3C E0823105 add     r3,r2,r5,lsl 2h       //r3 = PtrSecPartBPL + (nbpal * 4) = PtrThirdPartBPL
        022ECA40 E5CA11BC strb    r1,[r10,1BCh]         //[r10,1BCh] = r1
        022ECA44 E28A5C02 add     r5,r10,200h           //r5 = r10 + 0x200
        022ECA48 E1A06001 mov     r6,r1                 //r6 = r1
        022ECA4C EA00000C b       22ECA84h              ///022ECA84 v
        while( r1 >= nbpal && r1 < [r5 + 0xC4] )
        {
            ///022ECA84
            022ECA84 E59D4040 ldr     r4,[r13,40h]          //r4 = PtrBPLHeaderCopy
            022ECA88 E1D440F0 ldrsh   r4,[r4]               //r4 = [PtrBPLHeaderCopy] = nbpal
            //*** CONDITION ***
            022ECA8C E1510004 cmp     r1,r4
            if( r1 >= nbpal )
                022ECA90 AA000009 bge     22ECABCh          ///022ECABC v
            022ECA94 E1D54CF4 ldrsh   r4,[r5,0C4h]          //r4 = [r5 + 0xC4]
            //*** CONDITION ***
            022ECA98 E1510004 cmp     r1,r4                 
            if( r1 < r4 )
                022ECA9C BAFFFFEB blt     22ECA50h          ///022ECA50 ^
            
            ///022ECA50  - Seems to parse the second part of the bpl
            022ECA50 E1D240F2 ldrsh   r4,[r2,2h]            //r4 = [PtrSecPartBPL + 2]
            022ECA54 E2811001 add     r1,r1,1h              //r1 = r1 + 1
            022ECA58 E3540000 cmp     r4,0h             
            if( r4 > 0 )
                022ECA5C C5803004 strgt   r3,[r0,4h]        //[r0,4h] = PtrThirdPartBPL (Basically puts a pointer to the third part of the palette right after the copy of the bpl header!)
                022ECA60 C1D240F2 ldrgtsh r4,[r2,2h]        //r4 = [PtrSecPartBPL + 2]
            022ECA64 E2822004 add     r2,r2,4h              //r2 = PtrSecPartBPL + 4
            if( r4 <= 0 )
                022ECA68 D5807004 strle   r7,[r0,4h]        //[r0,4h] = r7 (0 ?) (Basically puts a pointer to 0 right after the copy of the bpl header!)
            022ECA6C E1C060B0 strh    r6,[r0]               //[r0] = r6
            if( r4 > 0 )
                022ECA70 C0644204 rsbgt   r4,r4,r4,lsl 4h   //r4 = (r4 * 16) - r4
            022ECA74 E1C060B2 strh    r6,[r0,2h]            //[r0,2h] = r6 (0?)
            022ECA78 E5806008 str     r6,[r0,8h]            //[r0,8h] = r6 (0?)
            if( r4 > 0 )
                022ECA7C C0833104 addgt   r3,r3,r4,lsl 2h   //r3 = PtrThirdPartBPL + (r4 * 4) 
            022ECA80 E280000C add     r0,r0,0Ch             //r0 = r0 + 12

        }
        022ECAA0 EA000005 b       22ECABCh              ///022ECABC v
    }
    ///022ECABC - Seems to fill missing missing palettes with zeros
    022ECABC E3A02000 mov     r2,0h
    022ECAC0 EA000005 b       22ECADCh              ///022ECADC v
    ///
    ///022ECAC4 LOOP_022ECAC4_BEG
    ///
    while(r1 < 16)
    {
        ///022ECADC
        022ECADC E3510010 cmp     r1,10h
        022ECAE0 BAFFFFF7 blt     22ECAC4h              ///022ECAC4 ^
        ///022ECAC4
        022ECAC4 E1C020B0 strh    r2,[r0]               //[r0] = 0
        022ECAC8 E1C020B2 strh    r2,[r0,2h]            //[r0,2h] = 0
        022ECACC E5802008 str     r2,[r0,8h]            //[r0,8h] = 0
        022ECAD0 E5802004 str     r2,[r0,4h]            //[r0,4h] = 0
        022ECAD4 E2811001 add     r1,r1,1h              //r1 = r1 + 1
        022ECAD8 E280000C add     r0,r0,0Ch             //r0 = r0 + 12
    }
    022ECAE4 E3A07000 mov     r7,0h                 //r7  = 0
    022ECAE8 E28A40C4 add     r4,r10,0C4h           //r4  = r10 + 0xC4
    022ECAEC E1A09007 mov     r9,r7                 //r9  = 0    
    022ECAF0 E1A05007 mov     r5,r7                 //r5  = 0
    022ECAF4 E3A0802C mov     r8,2Ch                //r8  = 0x2C
    ///
    ///022ECAF8 LOOP_022ECAF8_BEG
    ///
    while( cntr7 < 4 )
    {
        022ECAF8 E0000897 mul     r0,r7,r8              //r0  = cntr7 * r8
        022ECAFC E0846000 add     r6,r4,r0              //r6  = r4 + (cntr7 * r8)
        022ECB00 E7C49000 strb    r9,[r4,r0]            //[r4,r0] = 0
        022ECB04 E5C69001 strb    r9,[r6,1h]            //[r6,1h] = 0
        022ECB08 E1C690B4 strh    r9,[r6,4h]            //[r6,4h] = 0
        022ECB0C E1C690B2 strh    r9,[r6,2h]            //[r6,2h] = 0
        022ECB10 E2860008 add     r0,r6,8h              //r0 = (r4 + (cntr7 * r8)) + 8h
        022ECB14 EBF459C3 bl      2003228h              ///fun_02003228 
        {
            02003228 E3A01000 mov     r1,0h
            0200322C E5801000 str     r1,[r0]
            02003230 E5801004 str     r1,[r0,4h]
            02003234 E12FFF1E bx      r14
        }
        022ECB18 E5865010 str     r5,[r6,10h]
        022ECB1C E5865018 str     r5,[r6,18h]
        022ECB20 E5865014 str     r5,[r6,14h]
        022ECB24 E5865020 str     r5,[r6,20h]
        022ECB28 E586501C str     r5,[r6,1Ch]
        022ECB2C E2877001 add     r7,r7,1h              //r7 = cntr7 + 1
        022ECB30 E5865024 str     r5,[r6,24h]
        022ECB34 E5865028 str     r5,[r6,28h]           //PtrBPLBuff = R5
        022ECB38 E3570004 cmp     r7,4h
        022ECB3C BAFFFFED blt     22ECAF8h          ///022ECAF8 ^
    }
    022ECB40 E28A0C02 add     r0,r10,200h           //r0 = r10 + 0x200
    022ECB44 E1D07BFE ldrsh   r7,[r0,0BEh]          //r7 = [r0,0BEh] // AKA [r10,2BEh]
    022ECB48 EA00004C b       22ECC80h              ///022ECC80 v


    ///
    ///022ECB4C LOOP_BPC_BPA_BEG
    ///
    while( CNT_SzEntry < r0 )
    {
        ///022ECC80
        022ECC80 E28A0C02 add     r0,r10,200h
        022ECC84 E1D00CF0 ldrsh   r0,[r0,0C0h]          //some constant
        022ECC88 E1550000 cmp     r5,r0
        //***LOOP CONDITION EVALUATED HERE***
        022ECC8C BAFFFFAE blt     22ECB4Ch              ///022ECB4C LOOP_BPC_BPA_BEG ^ 
        
        022ECB4C E3A0000C mov     r0,0Ch                //r0 = LenBPCSzTblEntry = 12
        022ECB50 E0020095 mul     r2,r5,r0              //r2 = CNT_SzEntry * 12
        022ECB54 E28A0C02 add     r0,r10,200h           //r0 = r10 + 0x200 (Address of the tileset data struct?)    
        022ECB58 E28A3F72 add     r3,r10,1C8h           //r3 = BPCSizeTableBeg
        022ECB5C E1D01CF6 ldrsh   r1,[r0,0C6h]          //was 0    
        022ECB60 E19300F2 ldrsh   r0,[r3,r2]            //r0 = [BPCSizeTableBeg + (CNT_SzEntry * 12)]
        022ECB64 E3550000 cmp     r5,0h
        if( CNT_SzEntry == 0 )
        {
            022ECB68 03A08000 moveq   r8,0h             //r8 = 0
        }
        022ECB6C E0811000 add     r1,r1,r0              //r1 = r1 + (r10 + 0x200)
        022ECB70 E0830002 add     r0,r3,r2              //r0 = BPCSizeTableBeg + (CNT_SzEntry * 12)
        if( CNT_SzEntry != 0 )
        {
            022ECB74 13A08002 movne   r8,2h             //r8 = 2
        }
        022ECB78 E3550000 cmp     r5,0h
        if( CNT_SzEntry == 0 )
        {
            022ECB7C 03A09000 moveq   r9,0h             //r9 = 0
        }
        022ECB80 E58D0004 str     r0,[r13,4h]           //[r13 + 4h] = BPCSizeTableBeg + (CNT_SzEntry * 12)
        022ECB84 E28A00C4 add     r0,r10,0C4h           //r0 = r10 + 0xC4    //Constant loaded in fun_022EBC18
        022ECB88 E5DA62BC ldrb    r6,[r10,2BCh]         //r6 = [r10 + 0x2BC] //Constant loaded in fun_022EBC18
        022ECB8C E59F4184 ldr     r4,=2320C18h          //r4 = Table_02320C18 VRAM addresses table!! (Table with 5, 8 bytes entries made of 2 int 32) 
        if( CNT_SzEntry != 0 )
        {
            022ECB90 13A09004 movne   r9,4h             //r9 = 4
        }
        022ECB94 E0844186 add     r4,r4,r6,lsl 3h       //r4 = 0x2320C18 + ([r10 + 0x2BC] * 8)
        022ECB98 E7944107 ldr     r4,[r4,r7,lsl 2h]     //r4 = (0x2320C18 + ([r10 + 0x2BC] * 8)) + (r7 * 4)
        022ECB9C E3A06000 mov     r6,0h                 //r6  = CNT_BPASlot = 0    
        022ECBA0 E084B281 add     r11,r4,r1,lsl 5h      //r11 = ((0x2320C18 + ([r10 + 0x2BC] * 8)) + (r7 * 4)) + (r1 << 5)
        022ECBA4 E58D0030 str     r0,[r13,30h]          //[r13,30h] = (r10 + 0xC4)
        do while(CNT_BPASlot < 2) //r6 < 2
        {
            ///022ECBA8 LOOP_BPC_BPA2_BEG
            022ECBA8 E59D0030 ldr     r0,[r13,30h]          //r0 = (r10 + 0xC4)
            022ECBAC E3A0102C mov     r1,2Ch                //r1 = 0x2C
            022ECBB0 E0240198 mla     r4,r8,r1,r0           //r4 = (r8 * 0x2C) + (r10 + 0xC4)
            022ECBB4 E3A0000A mov     r0,0Ah                //r0 = 0xA    
            022ECBB8 E0000099 mul     r0,r9,r0              //r0 = r9 * 0xA
            022ECBBC E28D10CC add     r1,r13,0CCh           //r1 = r13 + 0xCC
            022ECBC0 E0811000 add     r1,r1,r0              //r1 = (r13 + 0xCC) + (r9 * 0xA)
            //022ECBC4 E1D111DE ldrsb   r1,[r1,1Eh]           //r1 = [((r13 + 0xCC) + (r9 * 0xA))  0x1E]
            //022ECBC8 E3510000 cmp     r1,0h
            //if( r1 == 0 )
            //    022ECBCC 0A000024 beq     22ECC64h          ///022ECC64 v
            //022ECBD0 E5DA12BC ldrb    r1,[r10,2BCh]         //r1 = [r10 + 0x2BC]
            //022ECBD4 E3510000 cmp     r1,0h                 
            //if( r1 != 0 )
            //    022ECBD8 1A000021 bne     22ECC64h          ///022ECC64 v
            if( [((r13 + 0xCC) + (r9 * 0xA))  0x1E] != 0 && [r10 + 0x2BC] == 0 )
            {
                //Load BPA
                022ECBDC E28D10EA add     r1,r13,0EAh           //r1 = r13 + 0xEA
                022ECBE0 E0812000 add     r2,r1,r0              //r2 = (r13 + 0xEA) + r0
                022ECBE4 E59F1134 ldr     r1,=2320C94h          //"rom0:/MAP_BG/%s.bpa"
                022ECBE8 E28D004C add     r0,r13,4Ch            //r0 = r13 + 0x4C
                022ECBEC EBF67264 bl      2089584h              ///fun_02089584 ReplaceTokens
                022ECBF0 E59D2000 ldr     r2,[r13]              //r2 = [r13]
                022ECBF4 E2840008 add     r0,r4,8h              //r0 = r4 + 8
                022ECBF8 E28D104C add     r1,r13,4Ch            //r1 = r13 + 0x4C
                022ECBFC EBF4700E bl      2008C3Ch              //LoadFileFromRom
                022ECC00 E3A00001 mov     r0,1h                 //r0 = 1
                022ECC04 E5C40000 strb    r0,[r4]               //[r4] = 1
                022ECC08 E5C40001 strb    r0,[r4,1h]            //[r4,1h] = 1
                022ECC0C E5943008 ldr     r3,[r4,8h]            //r3 = PtrBPABuff
                022ECC10 E59D0004 ldr     r0,[r13,4h]           //r0 = (BPCSizeTableBeg + (CNT_SzEntry * 12)) //PtrBPCIndexEntry
                022ECC14 E5843010 str     r3,[r4,10h]           //[r4,10h] = PtrBPABuff
                022ECC18 E0801086 add     r1,r0,r6,lsl 1h       //r1 = PtrBPCIndexEntry + (CNT_BPASlot * 2)
                022ECC1C E1D300F2 ldrsh   r0,[r3,2h]            //r0 = BPA_unk2
                022ECC20 E2832004 add     r2,r3,4h              //r2 = PtrBPABuff + 4 //Possibly beginning of DatablockA
                022ECC24 E5842018 str     r2,[r4,18h]           //[r4,18h] = (PtrBPABuff + 4)
                022ECC28 E0820100 add     r0,r2,r0,lsl 2h       //r0 = (PtrBPABuff + 4) + (BPA_unk2 * 4)
                022ECC2C E5842014 str     r2,[r4,14h]           //[r4,14h] = (PtrBPABuff + 4)
                022ECC30 E5840020 str     r0,[r4,20h]           //[r4,20h] = ((PtrBPABuff + 4) + (BPA_unk2 * 4))
                022ECC34 E584001C str     r0,[r4,1Ch]           //[r4,1Ch] = ((PtrBPABuff + 4) + (BPA_unk2 * 4))
                022ECC38 E3A00000 mov     r0,0h                 //r0 = 0
                022ECC3C E1C400B2 strh    r0,[r4,2h]            //[r4,2h] = 0
                022ECC40 E5940018 ldr     r0,[r4,18h]           //r0 = (PtrBPABuff + 4)
                022ECC44 E5900000 ldr     r0,[r0]               //r0 = BPA_DBA_Entry
                022ECC48 E1C400B4 strh    r0,[r4,4h]            //[r4,4h] = BPA_DBA_Entry
                022ECC4C E584B024 str     r11,[r4,24h]          //[r4,24h] = r11
                022ECC50 E1D100F2 ldrsh   r0,[r1,2h]            //r0 = [r1,2h]
                022ECC54 E1A00280 mov     r0,r0,lsl 5h
                022ECC58 E5840028 str     r0,[r4,28h]           //PtrBPLBuff = R0 
                022ECC5C E1D100F2 ldrsh   r0,[r1,2h]
                022ECC60 E08BB280 add     r11,r11,r0,lsl 5h     //r11 = r11 + (r0 * 32)
            }
            ///022ECC64
            022ECC64 E2866001 add     r6,r6,1h              //r6 = CNT_BPASlot + 1
            022ECC68 E3560002 cmp     r6,2h
            022ECC6C E2899001 add     r9,r9,1h              //r9 = r9 + 1
            022ECC70 E2888001 add     r8,r8,1h              //r8 = r8 + 1  
            //***LOOP CONDITION EVALUATED HERE****
            022ECC74 BAFFFFCB blt     22ECBA8h          ///022ECBA8 LOOP_BPC_BPA2_BEG ^        
        }
        022ECC78 E2855001 add     r5,r5,1h              //r5 = CNT_SzEntry + 1
        022ECC7C E2877001 add     r7,r7,1h              //r7 = r7 + 1
    } //End While

    ///After handling the BPL end up with the BMA again!!
    ///############ INVESTIGATE THIS PART FOR BMA PARSING ###################
    022ECC90 E3500000 cmp     r0,0h
    022ECC94 E1A0000A mov     r0,r10                //r0 = r10
    if( r0 <= 0 )
    {
        022ECC98 DA000002 ble     22ECCA8h          ///022ECCA8 v
        ///022ECCA8
        022ECCA8 E3A01000 mov     r1,0h                 //r1 = 0
        022ECCAC EB00065B bl      22EE620h              ///fun_022EE620 (see below!)
    }
    022ECC9C E3A01001 mov     r1,1h                 //r1 = 1
    022ECCA0 EB00065E bl      22EE620h              ///fun_022EE620
    {
        ///fun_022EE620
        022EE620 E92D4FF8 push    r3-r11,r14
        022EE624 E24DD018 sub     r13,r13,18h
        022EE628 E1A04000 mov     r4,r0                 //r4 = (PtrLevelStruct + 0x100)
        022EE62C E1A05001 mov     r5,r1                 //r5 = r1
        022EE630 E3550006 cmp     r5,6h
        022EE634 E2849E21 add     r9,r4,210h            //r9 = (PtrLevelStruct + 0x100) + 0x210
        //if( r5 <= 6 )
        //022EE638 908FF105 addls   r15,r15,r5,lsl 2h
        switch(r5)
        {
            case 0:
            {
                022EE640 EA000005 b       22EE65Ch              ///022EE65C 0
                ///022EE65C 0
                022EE65C E3A02000 mov     r2,0h
                022EE660 E58421FC str     r2,[r4,1FCh]          //r2 = [PtrLevelStruct,1FCh] //AKA [r10,1FCh]
                022EE664 E58D2000 str     r2,[r13]              //[r13] = r2
                022EE668 E1A00009 mov     r0,r9                 //r0 = (PtrLevelStruct + 0x100) + 0x210
                022EE66C E1A01004 mov     r1,r4                 //r1 = (PtrLevelStruct + 0x100)
                022EE670 E1A03002 mov     r3,r2                 //r3 = 0
                022EE674 E58D2004 str     r2,[r13,4h]           //[r13,4h] = r2
                022EE678 EB000093 bl      22EE8CCh              ///022EE8CC
                {
                    ///Fun 022EE8CC
                    022EE8CC E92D4070 push    r4-r6,r14                 //r13 -= 16 bytes (4 registers were pushed)
                    022EE8D0 E2812C02 add     r2,r1,200h
                    022EE8D4 E1D22BF8 ldrsh   r2,[r2,0B8h]              // [r1 + 2B8h] (Should be 1 after the padding was applied after the image) //AKA [r10,2B8h]
                    022EE8D8 E5DD4014 ldrb    r4,[r13,14h]              //r4  = [PtrLevelStruct,1FCh] //(PtrBPABuff + 4)
                    022EE8DC E59DC010 ldr     r12,[r13,10h]             //r12 = [PtrLevelStruct,1FCh]
                    022EE8E0 E1C020B0 strh    r2,[r0]                   //[r0] = r2
                    022EE8E4 E1C030B2 strh    r3,[r0,2h]                //[r0,2h] = r3
                    022EE8E8 E1C0C0B4 strh    r12,[r0,4h]               //[r0,4h] = [r13,10h] 
                    022EE8EC E5C04006 strb    r4,[r0,6h]
                    022EE8F0 E5D121E2 ldrb    r2,[r1,1E2h]              //r2 = BMA_unk3
                    022EE8F4 E3540000 cmp     r4,0h
                    022EE8F8 E5802008 str     r2,[r0,8h]
                    022EE8FC E5D121E3 ldrb    r2,[r1,1E3h]              //r2 = BMA_unk4
                    022EE900 E580200C str     r2,[r0,0Ch]
                    022EE904 E5D121E0 ldrb    r2,[r1,1E0h]              //r2 = BMA_Width
                    if( r4 != 0 )
                    {
                        022EE908 11A02182 movne   r2,r2,lsl 3h              //r2 = BMA_Width * 8
                        022EE90C 15802010 strne   r2,[r0,10h]               
                        022EE910 15D121E1 ldrneb  r2,[r1,1E1h]              //r2 = BMA_Height
                        022EE914 11A02182 movne   r2,r2,lsl 3h              //r2 = BMA_Height * 8
                        022EE918 1A000005 bne     22EE934h              ///022EE934
                    }
                    else
                    {
                        022EE91C E1A02182 mov     r2,r2,lsl 3h              //r2 = BMA_Width * 8
                        022EE920 E2422C01 sub     r2,r2,100h                //r2 = (BMA_Width * 8) - 256
                        022EE924 E5802010 str     r2,[r0,10h]
                        022EE928 E5D121E1 ldrb    r2,[r1,1E1h]              //r2 = BMA_Height
                        022EE92C E1A02182 mov     r2,r2,lsl 3h              //r2 = BMA_Height * 8
                        022EE930 E24220C0 sub     r2,r2,0C0h                //r2 = (BMA_Height * 8) - 192
                    }
                    ///022EE934
                    022EE934 E5802014 str     r2,[r0,14h]
                    022EE938 E3A02000 mov     r2,0h                     //r2 = CntR2 = 0
                    022EE93C E281EC02 add     r14,r1,200h               //r14 = (PtrLevelStruct + 0x100) + 0x200
                    022EE940 EA000009 b       22EE96Ch                  ///022EE96C
                    while( CntR2 < [PtrLevelStruct,1FCh] && CntLayers < NbBPCLayers)
                    {
                        ///022EE96C
                        022EE96C E152000C cmp     r2,r12
                        022EE970 AA000002 bge     22EE980h                  ///022EE980
                        022EE974 E1DE4CF0 ldrsh   r4,[r14,0C0h]             //NbBPCLayers(was 2, at 0x210CD04) //AKA [r10,2C0h]
                        022EE978 E1530004 cmp     r3,r4
                        022EE97C BAFFFFF0 blt     22EE944h              ///022EE944
                        ///022EE944 - Copy the pointers to each buffers for each BPC layers related data 
                        022EE944 E0816103 add     r6,r1,r3,lsl 2h           //r6 = (PtrLevelStruct + 0x100) + (r3 * 4)
                        022EE948 E59642DC ldr     r4,[r6,2DCh]              //r4 = PtrScreenMappingData
                        022EE94C E0805102 add     r5,r0,r2,lsl 2h           //r5 = r0 + (CntR2 * 4)
                        022EE950 E585401C str     r4,[r5,1Ch]               //[r5,1Ch] = r4
                        022EE954 E59642E4 ldr     r4,[r6,2E4h]              //
                        022EE958 E2822001 add     r2,r2,1h                  //r2 = CntR2 + 1
                        022EE95C E5854024 str     r4,[r5,24h]
                        022EE960 E59642EC ldr     r4,[r6,2ECh]
                        022EE964 E2833001 add     r3,r3,1h                  //r3 = CntLayers + 1
                        022EE968 E585402C str     r4,[r5,2Ch]
                    }
                    022EE980 E3A0C000 mov     r12,0h
                    022EE984 EA000004 b       22EE99Ch                  ///022EE99C
                    while( r2 < 2 )
                    {
                        ///022EE988
                        022EE988 E0803102 add     r3,r0,r2,lsl 2h
                        022EE98C E583C01C str     r12,[r3,1Ch]
                        022EE990 E583C024 str     r12,[r3,24h]
                        022EE994 E583C02C str     r12,[r3,2Ch]
                        022EE998 E2822001 add     r2,r2,1h
                        ///022EE99C
                        022EE99C E3520002 cmp     r2,2h
                        022EE9A0 BAFFFFF8 blt     22EE988h                  ///022EE988
                    }
                    022EE9A4 E2811C02 add     r1,r1,200h
                    022EE9A8 EB000000 bl      22EE9B0h
                    022EE9AC E8BD8070 pop     r4-r6,r15
                }
                022EE67C E59F0228 ldr     r0,=22EEA60h
                022EE680 E5890018 str     r0,[r9,18h]                   //[(PtrLevelStruct + 0x100) + 0x210, 18h] = 0x22EEA60
                022EE684 EA000035 b       22EE760h                      ///022EE760
                break;
            }
            case 3:
            {
                022EE64C EA00001C b       22EE6C4h              ///022EE6C4 3
                ///022EE6C4 3
                022EE6C4 E3A00001 mov     r0,1h                 //r0 = 1
                022EE6C8 E58401FC str     r0,[r4,1FCh]          //[r4,1FCh] = 1
                022EE6CC E2840C02 add     r0,r4,200h            //r0 = r4 + 0x200
                022EE6D0 E1D01CF0 ldrsh   r1,[r0,0C0h]          //r1 = [(PtrLevelStruct + 0x100)+200,0C0h] (r10,2C0h)
                022EE6D4 E3A02000 mov     r2,0h                 //r2 = 0
                022EE6D8 E1A00009 mov     r0,r9                 //r0 = (PtrLevelStruct + 0x100) + 0x210
                022EE6DC E58D1000 str     r1,[r13]              
                022EE6E0 E1A01004 mov     r1,r4
                022EE6E4 E1A03002 mov     r3,r2
                022EE6E8 E58D2004 str     r2,[r13,4h]
                022EE6EC EB000076 bl      22EE8CCh                  ///022EE8CC
                022EE6F0 EA00001A b       22EE760h                      ///022EE760
                break;
            }
            case 4:
            case 5:
            case 6:
            {
                022EE650 EA000027 b       22EE6F4h              ///022EE6F4 4
                022EE654 EA000026 b       22EE6F4h              ///022EE6F4 5
                022EE658 EA000025 b       22EE6F4h              ///022EE6F4 6  
                ///022EE6F4 4, 5, 6
                022EE6F4 E3A00002 mov     r0,2h
                022EE6F8 E3550006 cmp     r5,6h
                if( r5 == 6 )
                    022EE6FC 03A03001 moveq   r3,1h
                022EE700 E58401FC str     r0,[r4,1FCh]
                if( r5 != 6 )
                    022EE704 13A03000 movne   r3,0h
                022EE708 E3A02001 mov     r2,1h
                022EE70C E20360FF and     r6,r3,0FFh
                022EE710 E58D2000 str     r2,[r13]
                022EE714 E1A00009 mov     r0,r9
                022EE718 E1A01004 mov     r1,r4
                022EE71C E3A03000 mov     r3,0h
                022EE720 E58D6004 str     r6,[r13,4h]
                022EE724 EB000068 bl      22EE8CCh
                022EE728 E2840C02 add     r0,r4,200h
                022EE72C E1D00CF0 ldrsh   r0,[r0,0C0h]
                022EE730 E3550004 cmp     r5,4h
                if( r5 != 4 )
                    022EE734 13A02001 movne   r2,1h
                else
                    022EE738 03A02000 moveq   r2,0h
                022EE73C E2400001 sub     r0,r0,1h
                022EE740 E20260FF and     r6,r2,0FFh
                022EE744 E58D0000 str     r0,[r13]
                022EE748 E1A01004 mov     r1,r4
                022EE74C E2890054 add     r0,r9,54h
                022EE750 E3A02000 mov     r2,0h
                022EE754 E3A03001 mov     r3,1h
                022EE758 E58D6004 str     r6,[r13,4h]
                022EE75C EB00005A bl      22EE8CCh
                ///continue at 022EE760...
                break;
            }
            case 1:
            case 2:
            default:
            {
                022EE63C EA000011 b       22EE688h              ///022EE688 r5 > 6
                022EE644 EA00000F b       22EE688h              ///022EE688 1
                022EE648 EA00000E b       22EE688h              ///022EE688 2
                ///022EE688 - if (r5 > 6), 1 or 2
                022EE688 E3A07001 mov     r7,1h
                022EE68C E58471FC str     r7,[r4,1FCh]
                022EE690 E2840C02 add     r0,r4,200h
                022EE694 E1D06CF0 ldrsh   r6,[r0,0C0h]
                022EE698 E3550002 cmp     r5,2h
                022EE69C E3A02000 mov     r2,0h
                if( r5 != 2 )
                    022EE6A0 13A07000 movne   r7,0h
                022EE6A4 E58D6000 str     r6,[r13]
                022EE6A8 E20760FF and     r6,r7,0FFh
                022EE6AC E1A00009 mov     r0,r9
                022EE6B0 E1A01004 mov     r1,r4
                022EE6B4 E1A03002 mov     r3,r2
                022EE6B8 E58D6004 str     r6,[r13,4h]
                022EE6BC EB000082 bl      22EE8CCh
                022EE6C0 EA000026 b       22EE760h                      ///022EE760
            }
        };

        ///022EE760
        022EE760 E3550003 cmp     r5,3h
        if( r5 != 3 )  
        {
            022EE764 1A000005 bne     22EE780h                  ///022EE780 v
            ///022EE780
            022EE780 E59F012C ldr     r0,=2320BE4h
            022EE784 E3A0A000 mov     r10,0h                    //r10 = Cntr10 = 0
            022EE788 E5908024 ldr     r8,[r0,24h]
            022EE78C E5907028 ldr     r7,[r0,28h]
            022EE790 E58D8008 str     r8,[r13,8h]
            022EE794 E58D700C str     r7,[r13,0Ch]
            022EE798 E2845C02 add     r5,r4,200h
            022EE79C E59FB114 ldr     r11,=22EEA84h
            022EE7A0 E59F6108 ldr     r6,=22EEA64h
            022EE7A4 EA000025 b       22EE840h                  ///022EE840
            while( Cntr10 < [r4,1FCh] )
            {
                ///022EE840
                //022EE840 E59401FC ldr     r0,[r4,1FCh]
                //022EE844 E15A0000 cmp     r10,r0
                //022EE848 BAFFFFD6 blt     22EE7A8h              ///022EE7A8 ^
                ///022EE7A8 - BegLoop
                022EE7A8 E3A00054 mov     r0,54h                    //r0 = 84
                022EE7AC E020909A mla     r0,r10,r0,r9              //r0 = (Cntr10 * 84) + r9
                022EE7B0 E084118A add     r1,r4,r10,lsl 3h          //r1 = r4 + (Cntr10 * 8)
                022EE7B4 E5818200 str     r8,[r1,200h]
                022EE7B8 E5817204 str     r7,[r1,204h]
                022EE7BC E1D010F4 ldrsh   r1,[r0,4h]
                //022EE7C0 E3510001 cmp     r1,1h
                //022EE7C4 0A000001 beq     22EE7D0h              ///022EE7D0 v
                if( r1 == 2 )
                {
                    //022EE7C8 E3510002 cmp     r1,2h
                    //022EE7CC 0A00000C beq     22EE804h              ///022EE804
                    ///022EE804
                    022EE804 E1D010F0 ldrsh   r1,[r0]
                    022EE808 E3510000 cmp     r1,0h
                    if( r1 == 0 )
                        022EE80C 0A000001 beq     22EE818h              ///022EE818
                    022EE810 E3510001 cmp     r1,1h
                    if( r1 == 1 )
                        022EE814 0A000001 beq     22EE820h              ///022EE820
                    ///022EE818
                    022EE818 E580B018 str     r11,[r0,18h]
                    022EE81C EA000004 b       22EE834h                  ///022EE834
                    ///022EE820
                    022EE820 E5D01006 ldrb    r1,[r0,6h]
                    022EE824 E3510000 cmp     r1,0h
                    if( r1 != 0 )
                        022EE828 159F1094 ldrne   r1,=22EEFC0h
                    else
                        022EE82C 059F1094 ldreq   r1,=22EED98h
                    022EE830 E5801018 str     r1,[r0,18h]
                }
                else
                {
                    ///022EE7D0
                    022EE7D0 E1D010F0 ldrsh   r1,[r0]
                    //022EE7D4 E3510000 cmp     r1,0h
                    //022EE7D8 0A000001 beq     22EE7E4h              ///022EE7E4
                    if( r1 == 1 )
                    {
                        //022EE7DC E3510001 cmp     r1,1h
                        //022EE7E0 0A000001 beq     22EE7ECh              ///022EE7EC
                        ///022EE7EC
                        022EE7EC E5D01006 ldrb    r1,[r0,6h]
                        022EE7F0 E3510000 cmp     r1,0h
                        if( r1 != 0 )
                            022EE7F4 159F10C0 ldrne   r1,=22EEBF8h
                        else
                            022EE7F8 059F10C0 ldreq   r1,=22EEAACh
                        022EE7FC E5801018 str     r1,[r0,18h]
                        022EE800 EA00000B b       22EE834h                  ///022EE834
                    }
                    else
                    {
                        ///022EE7E4
                        022EE7E4 E5806018 str     r6,[r0,18h]
                        022EE7E8 EA000011 b       22EE834h                  ///022EE834
                    }//END IF
                } //END IF
                ///022EE834
                022EE834 E085118A add     r1,r5,r10,lsl 3h                  //r1 = r5 + (Cntr10 * 8)
                022EE838 EB00005C bl      22EE9B0h                  ///022EE9B0
                {}
                022EE83C E28AA001 add     r10,r10,1h                    //r10 = Cntr10 + 1
            } //END WHILE
        }
        else
        {
            022EE768 E59F2140 ldr     r2,=22EEA64h                  //r2 = 022EEA64
            022EE76C E1A00009 mov     r0,r9                         //r0 = r9 ((Ptrlevelstruct + 0x100) + 0x210) (r4 + 0x210)
            022EE770 E2841C02 add     r1,r4,200h                    //r1 = r4 + 0x200
            022EE774 E5892018 str     r2,[r9,18h]                   //[r9,18h] = 022EEA64
            022EE778 EB00008C bl      22EE9B0h                      ///022EE9B0
            {///FUN 022EE9B0
                022EE9B0 E92D4010 push    r4,r14
                022EE9B4 E5912000 ldr     r2,[r1]                   //r2 = [r1] (was 3) 
                022EE9B8 E1A04000 mov     r4,r0                     //r4 = ((Ptrlevelstruct + 0x100) + 0x210)
                022EE9BC E1A00142 mov     r0,r2,asr 2h              //r0 = r2 >> 2 (signed)
                022EE9C0 E0820EA0 add     r0,r2,r0,lsr 1Dh          //r0 = r2 + ((r2 * 4) >> 29)
                022EE9C4 E5842034 str     r2,[r4,34h]               //[r4,34h] = r2
                022EE9C8 E1A001C0 mov     r0,r0,asr 3h              //r0 = r0 >> 3 (signed)
                022EE9CC E584003C str     r0,[r4,3Ch]               //[r4,3Ch] = (r0 >> 3 (signed))
                022EE9D0 E5911004 ldr     r1,[r1,4h]                //r1 = [r1,4h] (was 0xC0)
                022EE9D4 E1A00141 mov     r0,r1,asr 2h
                022EE9D8 E0810EA0 add     r0,r1,r0,lsr 1Dh
                022EE9DC E5841038 str     r1,[r4,38h]
                022EE9E0 E1A001C0 mov     r0,r0,asr 3h
                022EE9E4 E5840040 str     r0,[r4,40h]
                022EE9E8 E1D400F0 ldrsh   r0,[r4]
                022EE9EC E3500000 cmp     r0,0h
                if( r0 == 0 )
                {
                    022EE9F0 0A000002 beq     22EEA00h                  ///022EEA00 v
                    ///022EEA00
                    022EEA00 E3A00000 mov     r0,0h
                    022EEA04 E5840044 str     r0,[r4,44h]
                    022EEA08 E5840048 str     r0,[r4,48h]
                    022EEA0C E584004C str     r0,[r4,4Ch]
                    022EEA10 E5840050 str     r0,[r4,50h]
                    022EEA14 E8BD8010 pop     r4,r15
                }
                else if( r0 == 1 )
                022EE9F4 E3500001 cmp     r0,1h
                {
                    022EE9F8 0A000006 beq     22EEA18h                  ///022EEA18 v
                    ///022EEA18
                    022EEA18 E594003C ldr     r0,[r4,3Ch]
                    022EEA1C E3A01003 mov     r1,3h
                    022EEA20 EBF6851F bl      208FEA4h
                    022EEA24 E5840044 str     r0,[r4,44h]
                    022EEA28 E5940040 ldr     r0,[r4,40h]
                    022EEA2C E3A01003 mov     r1,3h
                    022EEA30 EBF6851B bl      208FEA4h
                    022EEA34 E5840048 str     r0,[r4,48h]
                    022EEA38 E5940034 ldr     r0,[r4,34h]
                    022EEA3C E3A01018 mov     r1,18h
                    022EEA40 EBF68517 bl      208FEA4h
                    022EEA44 E2810010 add     r0,r1,10h
                    022EEA48 E584004C str     r0,[r4,4Ch]
                    022EEA4C E5940038 ldr     r0,[r4,38h]
                    022EEA50 E3A01018 mov     r1,18h
                    022EEA54 EBF68512 bl      208FEA4h
                    022EEA58 E5841050 str     r1,[r4,50h]
                    022EEA5C E8BD8010 pop     r4,r15
                }
                else
                {
                    022EE9FC E8BD8010 pop     r4,r15
                } //END IF
            }//END FUN
            022EE77C EA000032 b       22EE84Ch                  ///022EE84C v
        } //END IF

        ///022EE84C
        022EE84C E59F0060 ldr     r0,=2320BE4h
        022EE850 E59471FC ldr     r7,[r4,1FCh]
        022EE854 E5906024 ldr     r6,[r0,24h]
        022EE858 E5905028 ldr     r5,[r0,28h]
        022EE85C E58D6010 str     r6,[r13,10h]
        022EE860 E58D5014 str     r5,[r13,14h]
        022EE864 E3A0A000 mov     r10,0h
        022EE868 E3A08054 mov     r8,54h
        022EE86C EA00000A b       22EE89Ch                  ///022EE89C
        while(r7 < 2)
        {
            ///022EE89C
            //022EE89C E3570002 cmp     r7,2h
            //022EE8A0 BAFFFFF2 blt     22EE870h              ///022EE870    
            ///022EE870 - BegLoop
            022EE870 E0209897 mla     r0,r7,r8,r9
            022EE874 E0841187 add     r1,r4,r7,lsl 3h
            022EE878 E5816200 str     r6,[r1,200h]
            022EE87C E5815204 str     r5,[r1,204h]
            022EE880 E58DA000 str     r10,[r13]
            022EE884 E1A01004 mov     r1,r4
            022EE888 E1A0200A mov     r2,r10
            022EE88C E1A0300A mov     r3,r10
            022EE890 E58DA004 str     r10,[r13,4h]
            022EE894 EB00000C bl      22EE8CCh                  ///022EE8CC
            022EE898 E2877001 add     r7,r7,1h
        }//END WHILE
        022EE8A4 E28DD018 add     r13,r13,18h
        022EE8A8 E8BD8FF8 pop     r3-r11,r15
        022EE8AC 022EEA60
        022EE8B0 022EEA64
        022EE8B4 02320BE4
        022EE8B8 022EEA84
        022EE8BC 022EEBF8
        022EE8C0 022EEAAC
        022EE8C4 022EEFC0
        022EE8C8 022EED98
    }
    022ECCA4 EA000001 b       22ECCB0h              ///022ECCB0 v
    
    ///022ECCB0 - De-alloc buffers
    022ECCB0 E59A1228 ldr     r1,[r10,228h]         //(was 022EEAAC)
    022ECCB4 E28A0E21 add     r0,r10,210h
    022ECCB8 E12FFF31 blx     r1
    {
    }
    022ECCBC E3A01001 mov     r1,1h                 //r1 = 1
    022ECCC0 E28A0F61 add     r0,r10,184h           //R0 = PTR_PtrBPCBuff
    022ECCC4 E5CA12BA strb    r1,[r10,2BAh]
    022ECCC8 EBF4595A bl      2003238h              //IsPointerNull(R0 = Ptr)
    022ECCCC E3500000 cmp     r0,0h
    if( r0 != 0 )
    {
        022ECCD0 1A000001 bne     22ECCDCh          ///022ECCDC v
    }
    else
    {
        022ECCD4 E28A0F61 add     r0,r10,184h           //R0 = PTR_PtrBPCBuff
        022ECCD8 EBF46FBD bl      2008BD4h              ///fun_02008BD4 Memfree wrapper
    }
    ///022ECCDC
    022ECCDC E28A0F63 add     r0,r10,18Ch           //R0 = PTR_PtrBMABuff    
    022ECCE0 EBF45954 bl      2003238h              //IsPointerNull(R0 = Ptr)
    022ECCE4 E3500000 cmp     r0,0h
    if( r0 != 0 )
    {
        022ECCE8 1A000001 bne     22ECCF4h          ///022ECCF4 v  
    }
    else
    {
        022ECCEC E28A0F63 add     r0,r10,18Ch           //R0 = PTR_PtrBMABuff
        022ECCF0 EBF46FB7 bl      2008BD4h              ///fun_02008BD4 Memfree wrapper
    }
}
else // Param1 == 0xFFFFFFFF
{
    022EC2B4 EBFFFF74 bl      22EC08Ch                      ///fun_022EC08C 022EC08C
    { ///fun_022EC08C 022EC08C
        022EC08C E92D4FF8 push    r3-r11,r14
        022EC090 E1A0A000 mov     r10,r0
        022EC094 EBFFFFCB bl      22EBFC8h                  ///022EBFC8 Init bma, bpc, bpl buffers
            022EBFC8 E92D41F0 push    r4-r8,r14
            022EBFCC E1A08000 mov     r8,r0
            022EBFD0 E1D810F0 ldrsh   r1,[r8]
            022EBFD4 E3510003 cmp     r1,3h
            if( R1 != 3 )
                022EBFD8 1A000001 bne     22EBFE4h                  ///022EBFE4 LBL1
            022EBFDC EBFFFFDF bl      22EBF60h
            022EBFE0 E8BD81F0 pop     r4-r8,r15
            ///022EBFE4 LBL1
            022EBFE4 E3A06000 mov     r6,0h                 //R6 = CNT = 0
            022EBFE8 E28850C4 add     r5,r8,0C4h            //R5 = PtrLvlStruct + 0xC4
            022EBFEC E3A0402C mov     r4,2Ch

            ///022EBFF0 LOOP1_BEG
            022EBFF0 E0275496 mla     r7,r6,r4,r5           //R7 = (CNT * 0x2C) + (PtrLvlStruct + 0xC4)
            022EBFF4 E2870008 add     r0,r7,8h              //R0 = ((CNT * 0x2C) + (PtrLvlStruct + 0xC4)) + 8
            022EBFF8 EBF45C8E bl      2003238h              //IsPointerNull(R0 = Ptr)
            022EBFFC E3500000 cmp     r0,0h
            if( R0 != 0 )
                022EC000 1A000001 bne     22EC00Ch          ///022EC00C LBL2
            022EC004 E2870008 add     r0,r7,8h
            022EC008 EBF472F1 bl      2008BD4h              //02008BD4
            {
                02008BD4 E92D4010 push    r4,r14
                02008BD8 E1A04000 mov     r4,r0
                02008BDC E5940000 ldr     r0,[r4]
                02008BE0 EBFFE168 bl      2001188h          ///MemFree(r0 = ptrbufftofree)
                02008BE4 E3A00000 mov     r0,0h
                02008BE8 E5840000 str     r0,[r4]
                02008BEC E5840004 str     r0,[r4,4h]
                02008BF0 E8BD8010 pop     r4,r15
            }
            ///022EC00C LBL2
            022EC00C E2866001 add     r6,r6,1h              //R6 = CNT + 1
            022EC010 E3560004 cmp     r6,4h
            if(CNT < 4)
                022EC014 BAFFFFF5 blt     22EBFF0h              ///022EBFF0 LOOP1_BEG

            022EC018 E2880E1A add     r0,r8,1A0h
            022EC01C EBF5DD96 bl      206367Ch
            022EC020 E2880F65 add     r0,r8,194h
            022EC024 EBF5DD6B bl      20635D8h
            022EC028 E2880F5D add     r0,r8,174h            //R0 = PTR_PtrBPLBuff
            022EC02C EBF45C81 bl      2003238h              //IsPointerNull(R0 = Ptr)
            022EC030 E3500000 cmp     r0,0h
            022EC034 1A000001 bne     22EC040h
            022EC038 E2880F5D add     r0,r8,174h            //R0 = PTR_PtrBPLBuff
            022EC03C EBF472E4 bl      2008BD4h
            {
                02008BD4 E92D4010 push    r4,r14
                02008BD8 E1A04000 mov     r4,r0
                02008BDC E5940000 ldr     r0,[r4]
                02008BE0 EBFFE168 bl      2001188h
                02008BE4 E3A00000 mov     r0,0h
                02008BE8 E5840000 str     r0,[r4]
                02008BEC E5840004 str     r0,[r4,4h]
                02008BF0 E8BD8010 pop     r4,r15
            }
            022EC040 E2880F5F add     r0,r8,17Ch
            022EC044 EBF45C7B bl      2003238h              //IsPointerNull(R0 = Ptr)
            022EC048 E3500000 cmp     r0,0h
            022EC04C 1A000001 bne     22EC058h
            022EC050 E2880F5F add     r0,r8,17Ch
            022EC054 EBF472DE bl      2008BD4h
            {
                02008BD4 E92D4010 push    r4,r14
                02008BD8 E1A04000 mov     r4,r0
                02008BDC E5940000 ldr     r0,[r4]
                02008BE0 EBFFE168 bl      2001188h
                02008BE4 E3A00000 mov     r0,0h
                02008BE8 E5840000 str     r0,[r4]
                02008BEC E5840004 str     r0,[r4,4h]
                02008BF0 E8BD8010 pop     r4,r15
            }
            022EC058 E2880F61 add     r0,r8,184h            //R0 = PTR_PtrBPCBuff
            022EC05C EBF45C75 bl      2003238h              //IsPointerNull(R0 = Ptr)
            022EC060 E3500000 cmp     r0,0h
            022EC064 1A000001 bne     22EC070h
            022EC068 E2880F61 add     r0,r8,184h            //R0 = PTR_PtrBPCBuff
            022EC06C EBF472D8 bl      2008BD4h
            {
                02008BD4 E92D4010 push    r4,r14
                02008BD8 E1A04000 mov     r4,r0
                02008BDC E5940000 ldr     r0,[r4]
                02008BE0 EBFFE168 bl      2001188h
                02008BE4 E3A00000 mov     r0,0h
                02008BE8 E5840000 str     r0,[r4]
                02008BEC E5840004 str     r0,[r4,4h]
                02008BF0 E8BD8010 pop     r4,r15
            }
            022EC070 E2880F63 add     r0,r8,18Ch            //R0 = PTR_PtrBMABuff
            022EC074 EBF45C6F bl      2003238h              //IsPointerNull(R0 = Ptr)
            022EC078 E3500000 cmp     r0,0h
            022EC07C 18BD81F0 popne   r4-r8,r15
            022EC080 E2880F63 add     r0,r8,18Ch            //R0 = PTR_PtrBMABuff
            022EC084 EBF472D2 bl      2008BD4h
            {
                02008BD4 E92D4010 push    r4,r14
                02008BD8 E1A04000 mov     r4,r0
                02008BDC E5940000 ldr     r0,[r4]
                02008BE0 EBFFE168 bl      2001188h
                02008BE4 E3A00000 mov     r0,0h
                02008BE8 E5840000 str     r0,[r4]
                02008BEC E5840004 str     r0,[r4,4h]
                02008BF0 E8BD8010 pop     r4,r15
            }
            022EC088 E8BD81F0 pop     r4-r8,r15
        }
        022EC098 E3A02000 mov     r2,0h
        022EC09C E1CA20B0 strh    r2,[r10]
        022EC0A0 E5CA21C0 strb    r2,[r10,1C0h]
        022EC0A4 E2421001 sub     r1,r2,1h
        022EC0A8 E28A0C01 add     r0,r10,100h
        022EC0AC E1C01BBE strh    r1,[r0,0BEh]
        022EC0B0 E28A0C02 add     r0,r10,200h
        022EC0B4 E1C02BB8 strh    r2,[r0,0B8h]
        022EC0B8 E5CA21E0 strb    r2,[r10,1E0h]
        022EC0BC E5CA21E1 strb    r2,[r10,1E1h]
        022EC0C0 E5CA21E2 strb    r2,[r10,1E2h]
        022EC0C4 E5CA21E3 strb    r2,[r10,1E3h]
        022EC0C8 E5CA21EE strb    r2,[r10,1EEh]
        022EC0CC E58A21F4 str     r2,[r10,1F4h]
        022EC0D0 E5CA21F8 strb    r2,[r10,1F8h]
        022EC0D4 E5CA21F9 strb    r2,[r10,1F9h]
        022EC0D8 E5CA21FA strb    r2,[r10,1FAh]
        022EC0DC E5CA21FB strb    r2,[r10,1FBh]
        022EC0E0 E5CA21BC strb    r2,[r10,1BCh]
        022EC0E4 E28A0004 add     r0,r10,4h
        022EC0E8 E1A05002 mov     r5,r2
        022EC0EC E1C050B0 strh    r5,[r0]
        022EC0F0 E1C050B2 strh    r5,[r0,2h]
        022EC0F4 E2822001 add     r2,r2,1h
        022EC0F8 E5805008 str     r5,[r0,8h]
        022EC0FC E5805004 str     r5,[r0,4h]
        022EC100 E3520010 cmp     r2,10h
        022EC104 E280000C add     r0,r0,0Ch
        022EC108 BAFFFFF7 blt     22EC0ECh
        022EC10C E3A09000 mov     r9,0h             //r9 = 0
        022EC110 E28A40C4 add     r4,r10,0C4h       //r4 = r10 + 0xC4
        022EC114 E1A08009 mov     r8,r9             //r8 = 0
        022EC118 E3A0702C mov     r7,2Ch            //r7 = 0x2C
        022EC11C E0000795 mul     r0,r5,r7          //r0 = r5 * 0x2C
        022EC120 E0846000 add     r6,r4,r0          //r6 = (r10 + 0xC4) + (r5 * 0x2C)
        022EC124 E7C49000 strb    r9,[r4,r0]        
        022EC128 E5C69001 strb    r9,[r6,1h]
        022EC12C E1C690B2 strh    r9,[r6,2h]
        022EC130 E1C690B4 strh    r9,[r6,4h]
        022EC134 E2860008 add     r0,r6,8h
        022EC138 EBF45C3A bl      2003228h
        022EC13C E5868010 str     r8,[r6,10h]
        022EC140 E5868018 str     r8,[r6,18h]
        022EC144 E5868014 str     r8,[r6,14h]
        022EC148 E5868020 str     r8,[r6,20h]
        022EC14C E586801C str     r8,[r6,1Ch]
        022EC150 E2855001 add     r5,r5,1h
        022EC154 E5868024 str     r8,[r6,24h]
        022EC158 E5868028 str     r8,[r6,28h]
        022EC15C E3550004 cmp     r5,4h
        if( r5 < 4 )
        {
            022EC160 BAFFFFED blt     22EC11Ch
        }
        022EC164 E28A4C02 add     r4,r10,200h
        022EC168 E1D41CF2 ldrsh   r1,[r4,0C2h]
        022EC16C E59F00C0 ldr     r0,=2324CBCh
        022EC170 E5DA22BC ldrb    r2,[r10,2BCh]
        022EC174 E5903000 ldr     r3,[r0]
        022EC178 E1A01A01 mov     r1,r1,lsl 14h
        022EC17C E3A00028 mov     r0,28h
        022EC180 E2836098 add     r6,r3,98h
        022EC184 E1650082 smulbb  r5,r2,r0
        022EC188 E1A07821 mov     r7,r1,lsr 10h
        022EC18C E59FB0A4 ldr     r11,=2320BF4h
        022EC190 EA000012 b       22EC1E0h
        022EC194 E2870001 add     r0,r7,1h
        022EC198 E1A03800 mov     r3,r0,lsl 10h
        022EC19C E1A01007 mov     r1,r7
        022EC1A0 E1A0200B mov     r2,r11
        022EC1A4 E0860005 add     r0,r6,r5
        022EC1A8 E1A07823 mov     r7,r3,lsr 10h
        022EC1AC EBF478F7 bl      200A590h
        022EC1B0 E3A09001 mov     r9,1h
        022EC1B4 E2870001 add     r0,r7,1h
        022EC1B8 E1A03800 mov     r3,r0,lsl 10h
        022EC1BC E1A01007 mov     r1,r7
        022EC1C0 E59F2074 ldr     r2,=2320BE8h
        022EC1C4 E0860005 add     r0,r6,r5
        022EC1C8 E1A07823 mov     r7,r3,lsr 10h
        022EC1CC EBF478EF bl      200A590h
        022EC1D0 E2899001 add     r9,r9,1h
        022EC1D4 E3590010 cmp     r9,10h
        022EC1D8 BAFFFFF5 blt     22EC1B4h
        022EC1DC E2888001 add     r8,r8,1h
        022EC1E0 E1D40CF4 ldrsh   r0,[r4,0C4h]
        022EC1E4 E1580000 cmp     r8,r0
        022EC1E8 BAFFFFE9 blt     22EC194h
        022EC1EC E0860005 add     r0,r6,r5
        022EC1F0 EBF478C3 bl      200A504h
        022EC1F4 E28A0C02 add     r0,r10,200h
        022EC1F8 E1D00CF0 ldrsh   r0,[r0,0C0h]
        022EC1FC E3500000 cmp     r0,0h
        022EC200 E1A0000A mov     r0,r10
        022EC204 DA000002 ble     22EC214h
        022EC208 E3A01001 mov     r1,1h
        022EC20C EB000903 bl      22EE620h
        022EC210 EA000001 b       22EC21Ch
        022EC214 E3A01000 mov     r1,0h
        022EC218 EB000900 bl      22EE620h
        022EC21C E59A1228 ldr     r1,[r10,228h]
        022EC220 E28A0E21 add     r0,r10,210h
        022EC224 E12FFF31 blx     r1
        022EC228 E3A00001 mov     r0,1h
        022EC22C E5CA02BA strb    r0,[r10,2BAh]
        022EC230 E8BD8FF8 pop     r3-r11,r15
        022EC234 02324CBC 
        022EC238 02320BF4 
        022EC23C 02320BE8 
    } //END FUNCTION
    022EC2B8 EA00028D b       22ECCF4h                      ///022ECCF4 LBL2 Return v 
} //END IF
///022ECCF4 LBL2
022ECCF4 E28DDF4F add     r13,r13,13Ch
022ECCF8 E8BD8FF0 pop     r4-r11,r15
022ECCFC 02320C58 
022ECD00 02320C6C 
022ECD04 0000030F 
022ECD08 02320C80 
022ECD0C 02324CBC 
022ECD10 02320BF4 
022ECD14 02320BE8 
022ECD18 02320C18
022ECD1C 0000FFFF 
022ECD20 02320C94 





//
//  022F1800 22F1800h (Called from line 022EC9E8)
//      r0 = PtrTargetBuffer? [r10,2D8h]
//      r1 = PtrReadBMADataPos
//      r2 = PtrBMAHeaderCopyBeg
//      r3 = Was 0xBA //[r10,0x2CE] (PtrLevelStructBeg)
//
022F1800 E92D4FF8 push    r3-r11,r14
022F1804 E1D240F6 ldrsh   r4,[r2,6h]        //r4 = BMA_unk6 (was 0)
022F1808 E3A05000 mov     r5,0h             //r5 = 0 //Cnt1
022F180C E3540000 cmp     r4,0h
022F1810 E3A040FF mov     r4,0FFh           //r4 = 0xFF
if( BMA_unk6 <= 0 )
{
    022F1814 DA000088 ble     22F1A3Ch      ///022F1A3C BMA_unk6 <= 0 v
    ///022F1A3C BMA_unk6 <= 0
    022F1A3C E1A06000 mov     r6,r0             //r6 = PtrTargetBuffer?
    022F1A40 E1A07005 mov     r7,r5             //r7 = Cnt1
    while(Cnt1 < 4)
    {
        ///022F1A44
        022F1A44 E1A08007 mov     r8,r7             //r8 = 0 //Cnt
        while(Cnt < 256)
        {
            ///022F1A48
            022F1A48 E2888001 add     r8,r8,1h          //r8 = Cnt + 1
            022F1A4C E3580C01 cmp     r8,100h
            022F1A50 E4C64001 strb    r4,[r6],1h        //[r6] = 0xFF, r6 += 1
            022F1A54 BAFFFFFB blt     22F1A48h          ///022F1A48 ^
        }
        022F1A58 E2855001 add     r5,r5,1h          //r5 = r5 + 1
        022F1A5C E3550004 cmp     r5,4h
        022F1A60 BAFFFFF7 blt     22F1A44h          ///022F1A44 ^
    }
    022F1A64 E5D27001 ldrb    r7,[r2,1h]        //r7 = BMA_MapHeight (was 0x48)
    022F1A68 E3A04000 mov     r4,0h             //r4 = 0 //Cnt6
    022F1A6C E3570000 cmp     r7,0h
    if( BMA_MapHeight <= 0 )
    {
        022F1A70 DA00001B ble     22F1AE4h      ///022F1AE4 v
        ///Continue to 022F1AE4...
    }
    else
    {
        022F1A74 E3A080FF mov     r8,0FFh           //r8  = 0xFF
        022F1A78 E1A09004 mov     r9,r4             //r9  = 0
        022F1A7C E1A0A008 mov     r10,r8            //r10 = 0xFF
        022F1A80 E1A0C004 mov     r12,r4            //r12 = 0
        022F1A84 EA000013 b       22F1AD8h          ///022F1AD8 v
        while(Cnt6 < BMA_MapHeight)
        {
            ///022F1AD8
            022F1AD8 E5D27001 ldrb    r7,[r2,1h]        ///r7 = BMA_MapHeight //(was 0x48)
            022F1ADC E1540007 cmp     r4,r7
            //**** LOOP CONDITION EVALUATED HERE! ****
            022F1AE0 BAFFFFE8 blt     22F1A88h          ///022F1A88
            ///022F1A88
            022F1A88 E1A0E00C mov     r14,r12           //r14 = 0 //Cnt7
            while( Cnt7 < 5 )
            {
                ///022F1A8C
                022F1A8C E28EE001 add     r14,r14,1h        //r14 = Cnt7 + 1
                022F1A90 E4C6A001 strb    r10,[r6],1h       //[r6] = 0xFF, r6 += 1
                022F1A94 E35E0005 cmp     r14,5h
                022F1A98 BAFFFFFB blt     22F1A8Ch          ///022F1A8C ^
            }
            022F1A9C E3A0B000 mov     r11,0h            //r11 = 0
            022F1AA0 EA000001 b       22F1AACh          ///022F1AAC v
            while(r11 < BMA_MapWidth)
            {
                ///022F1AAC
                022F1AAC E5D27000 ldrb    r7,[r2]           //r7 = BMA_MapWidth (was 0x2D)
                022F1AB0 E15B0007 cmp     r11,r7
                022F1AB4 BAFFFFFA blt     22F1AA4h          ///022F1AA4 ^
                ///022F1AA4
                022F1AA4 E4C69001 strb    r9,[r6],1h        //[r6] = 0, r6 += 1
                022F1AA8 E28BB001 add     r11,r11,1h        //r11 = r11 + 1
            }
            022F1AB8 E08EE007 add     r14,r14,r7        //r14 = Cnt7 + r7    
            022F1ABC EA000001 b       22F1AC8h          ///022F1AC8 v
            while(Cnt7 < 256)
            {
                ///022F1AC8
                022F1AC8 E35E0C01 cmp     r14,100h
                022F1ACC BAFFFFFB blt     22F1AC0h      ///022F1AC0 ^
                ///022F1AC0
                022F1AC0 E4C68001 strb    r8,[r6],1h        //[r6] = 0xFF, r6 += 1
                022F1AC4 E28EE001 add     r14,r14,1h        //r14 = Cnt7 + 1
            }
            022F1AD0 E2844001 add     r4,r4,1h          //r4 = Cnt6 + 1
            022F1AD4 E2855001 add     r5,r5,1h          //r5 = Cnt1 + 1

        }
        ///Continue to 022F1AE4...
    }
    
    ///022F1AE4
    022F1AE4 E3A040FF mov     r4,0FFh           //r4 = 0xFF
    022F1AE8 E3A07000 mov     r7,0h             //r7 = 0
    022F1AEC EA000005 b       22F1B08h          ///022F1B08
    while( Cnt1 < r3 ) //r3 was 0xBA
    {
        ///022F1B08
        022F1B08 E1550003 cmp     r5,r3
        022F1B0C BAFFFFF7 blt     22F1AF0h      ///022F1AF0    
        ///022F1AF0
        022F1AF0 E1A08007 mov     r8,r7             //r8 = r7
        while( r8 < 256 )
        {
            ///022F1AF4
            022F1AF4 E2888001 add     r8,r8,1h          //r8 = r8 + 1
            022F1AF8 E3580C01 cmp     r8,100h           
            022F1AFC E4C64001 strb    r4,[r6],1h        //[r6] = r4, r6 += 1
            022F1B00 BAFFFFFB blt     22F1AF4h      ///022F1AF4 ^ 
        }
        022F1B04 E2855001 add     r5,r5,1h          //r5 = Cnt1 + 1
    }
    //Continue to 022F1B10...
}
else //BMA_unk6 > 0
{
    022F1818 E1A07000 mov     r7,r0             //r7 = r0 
    022F181C E1A06005 mov     r6,r5             //r6 = 0
    while( Cnt1 < 4 )
    {
        ///022F1820
        022F1820 E1A08006 mov     r8,r6             //r8 = 0 //Cnt2
        while( Cnt < 256 )
        {
            ///022F1824
            022F1824 E2888001 add     r8,r8,1h          //r8 = Cnt2 + 1
            022F1828 E3580C01 cmp     r8,100h
            022F182C E4C74001 strb    r4,[r7],1h        //[r7] = BMA_unk6, r7 += 1
            022F1830 BAFFFFFB blt     22F1824h          ///022F1824 ^
        }
        022F1834 E2855001 add     r5,r5,1h          //r5 = Cnt1 + 1
        022F1838 E3550004 cmp     r5,4h
        022F183C BAFFFFF7 blt     22F1820h      ///022F1820
    }
    022F1840 E5D26001 ldrb    r6,[r2,1h]        //r6 = BMA_MapHeight
    022F1844 E3A04000 mov     r4,0h             //r4 = 0 //Cnt3
    022F1848 E3560000 cmp     r6,0h
    if( BMA_MapHeight <= 0 )
    {
        022F184C DA00006E ble     22F1A0Ch      ///022F1A0C v
        ///022F1A0C 
        022F1A0C E3A040FF mov     r4,0FFh               //r4 = 0xFF
        022F1A10 E3A06000 mov     r6,0h                 //r6 = 0
        022F1A14 EA000005 b       22F1A30h              ///022F1A30 v
        while(Cnt1 < r3)
        {
            ///022F1A30
            022F1A30 E1550003 cmp     r5,r3
            022F1A34 BAFFFFF7 blt     22F1A18h          ///022F1A18 ^
            ///022F1A18
            022F1A18 E1A08006 mov     r8,r6                 //r8 = 0
            while(r8 < 256)
            {
                ///022F1A1C
                022F1A1C E2888001 add     r8,r8,1h          //r8 = r8 + 1
                022F1A20 E3580C01 cmp     r8,100h               
                022F1A24 E4C74001 strb    r4,[r7],1h        //[r7] = 0xFF, r7 += 1
                022F1A28 BAFFFFFB blt     22F1A1Ch          ///022F1A1C
            }
            022F1A2C E2855001 add     r5,r5,1h              //r5 = Cnt1 + 1
        }
        
    }
    else //BMA_MapHeight >= 0
    {
        022F1850 EA00006A b       22F1A00h          ///022F1A00 v
        
        while(Cnt3 < BMA_MapHeight)
        {
            ///022F1A00
            022F1A00 E5D26001 ldrb    r6,[r2,1h]            //r6 = BMA_MapHeight
            022F1A04 E1540006 cmp     r4,r6
            022F1A08 BAFFFF91 blt     22F1854h          ///022F1854 ^
            
            ///022F1854
            022F1854 E3A09000 mov     r9,0h             //r9 = 0 //Cnt5
            022F1858 E3A060FF mov     r6,0FFh           //r6 = 0xFF
            while(Cnt5 < 5)
            {
                ///022F185C
                022F185C E2899001 add     r9,r9,1h          //r9 = Cnt5 + 1
                022F1860 E3590005 cmp     r9,5h
                022F1864 E4C76001 strb    r6,[r7],1h        //[r7] = 0xFF, r7 += 1
                022F1868 BAFFFFFB blt     22F185Ch      ///022F185C ^
            }
            022F186C E3540000 cmp     r4,0h
            022F1870 E3A0E000 mov     r14,0h            //r14 = 0
            if( Cnt3 !=  0 )
            {
                022F1874 1A00002A bne     22F1924h      ///022F1924 v
                ///022F1924
                022F1924 E247CC01 sub     r12,r7,100h           //r12 = r7 - 256
                022F1928 EA000028 b       22F19D0h              ///022F19D0 v
                
                while( r14 < r6 )
                {
                    ///022F19D0
                    022F19D0 E5D26000 ldrb    r6,[r2]               //r6 = BMA_MapWidth
                    022F19D4 E15E0006 cmp     r14,r6
                    022F19D8 BAFFFFD3 blt     22F192Ch          ///022F192C ^
                    ///022F192C
                    022F192C E5D16000 ldrb    r6,[r1]               //r6 = 
                    022F1930 E2818001 add     r8,r1,1h              //r8 = r1 + 1
                    022F1934 E1A01008 mov     r1,r8                 //r1 = r8
                    022F1938 E35600C0 cmp     r6,0C0h   
                    if( r6 < 0xC0 )
                    {
                        022F193C BA00000B blt     22F1970h          ///022F1970 v
                        ///022F1970
                        022F1970 E3560080 cmp     r6,80h
                        if( r6 < 0x80 )
                        {
                            022F1974 BA00000C blt     22F19ACh          ///022F19AC v
                            ///022F19AC
                            022F19AC E3A0A000 mov     r10,0h                //r10 = 0
                            022F19B0 EA000002 b       22F19C0h              ///022F19C0 v
                            while(r10 <= r6)
                            {
                                ///022F19C0
                                022F19C0 E15A0006 cmp     r10,r6
                                022F19C4 DAFFFFFA ble     22F19B4h          ///022F19B4 ^
                                ///022F19B4
                                022F19B4 E4DC8001 ldrb    r8,[r12],1h           //r8 = [r12], r12 += 1
                                022F19B8 E28AA001 add     r10,r10,1h            //r10 = r10 + 1
                                022F19BC E4C78001 strb    r8,[r7],1h            //[r7] = r8, r7 += 1
                            }
                            022F19C8 E2866001 add     r6,r6,1h              //r6 = r6 + 1
                            022F19CC E08EE006 add     r14,r14,r6            //r14 = r14 + r6
                            ///Continue loop at 022F19D0..
                        }
                        else //r6 >= 0x80
                        {
                            022F1978 E2881001 add     r1,r8,1h              //r1 = r8 + 1
                            022F197C E5D88000 ldrb    r8,[r8]               //r8 = [r8]
                            022F1980 E3A0A080 mov     r10,80h               //r10 = 0x80
                            022F1984 EA000003 b       22F1998h              ///022F1998 v
                            while(r10 <= r6)
                            {
                                ///022F1998
                                022F1998 E15A0006 cmp     r10,r6
                                022F199C DAFFFFF9 ble     22F1988h          ///022F1988
                                ///022F1988
                                022F1988 E4DCB001 ldrb    r11,[r12],1h          //r11 = [r12], r12 += 1
                                022F198C E28AA001 add     r10,r10,1h            //r10 = r10 + 1
                                022F1990 E028B00B eor     r11,r8,r11            //r11 = r8 | r11
                                022F1994 E4C7B001 strb    r11,[r7],1h           //[r7] = r11, r7 += 1
                            }
                            022F19A0 E246607F sub     r6,r6,7Fh             //r6 = r6 - 0x7F
                            022F19A4 E08EE006 add     r14,r14,r6            //r14 = r14 + r6
                            022F19A8 EA000008 b       22F19D0h              ///022F19D0 v
                            ///Continue loop at 022F19D0..
                        }
                    }
                    else //r6 >= 0xC0
                    {
                        022F1940 E3A080C0 mov     r8,0C0h               //r8 = 0xC0
                        022F1944 EA000004 b       22F195Ch              ///022F195C v
                        ///022F195C
                        022F195C E1580006 cmp     r8,r6
                        if( r8 <= r6 )
                        {
                            022F1960 DAFFFFF8 ble     22F1948h          ///022F1948 ^
                            ///022F1948
                            022F1948 E4D1B001 ldrb    r11,[r1],1h           //r11 = [r1], r1 += 1
                            022F194C E4DCA001 ldrb    r10,[r12],1h          //r10 = [r12], r12 += 1
                            022F1950 E2888001 add     r8,r8,1h              //r8  = r8 + 1
                            022F1954 E02BA00A eor     r10,r11,r10           //r10 = r11 ^ r10
                            022F1958 E4C7A001 strb    r10,[r7],1h           //[r7] = r10, r7 += 1
                        }
                        022F1964 E24660BF sub     r6,r6,0BFh            //r6 = r6 - 0xBF
                        022F1968 E08EE006 add     r14,r14,r6            //r14 = r14 + r6
                        022F196C EA000017 b       22F19D0h              ///022F19D0 v
                        ///Continue loop at 022F19D0..
                    }
                }
                //Exit loop to 022F19DC...
            }
            else //Cnt3 ==  0 
            {
                022F1878 E1A0800E mov     r8,r14            //r8 = 0
                022F187C E3A0C0C0 mov     r12,0C0h          //r12 = 0xC0
                022F1880 EA000023 b       22F1914h          ///022F1914 v
                while(r14 < BMA_MapWidth)
                {
                    ///022F1914
                    022F1914 E5D26000 ldrb    r6,[r2]           //r6 = BMA_MapWidth
                    022F1918 E15E0006 cmp     r14,r6
                    022F191C BAFFFFD8 blt     22F1884h          ///022F1884 ^
                    ///022F1884
                    022F1884 E5D16000 ldrb    r6,[r1]           //r6 = 
                    022F1888 E281A001 add     r10,r1,1h         //r10 = r1 + 1
                    022F188C E1A0100A mov     r1,r10            //r1  = r1 + 1 
                    022F1890 E35600C0 cmp     r6,0C0h
                    if( r6 < 0xC0 )
                    {
                        022F1894 BA000009 blt     22F18C0h      ///022F18C0 v
                        ///022F18C0
                        022F18C0 E3560080 cmp     r6,80h
                        if( r6 < 0x80 )
                        {
                            022F18C4 BA00000A blt     22F18F4h      ///022F18F4 v
                            ///022F18F4
                            022F18F4 E3A0A000 mov     r10,0h            //r10 = 0
                            022F18F8 EA000001 b       22F1904h          ///022F1904 v
                            while( r10 <= r6 )
                            {
                                ///022F1904
                                022F1904 E15A0006 cmp     r10,r6
                                022F1908 DAFFFFFB ble     22F18FCh      ///022F18FC
                                ///022F18FC
                                022F18FC E4C78001 strb    r8,[r7],1h        //[r7] = r8, r7 += 1
                                022F1900 E28AA001 add     r10,r10,1h        //r10  = r10 + 1 
                            }
                            022F190C E2866001 add     r6,r6,1h          //r6 = r6 + 1
                            022F1910 E08EE006 add     r14,r14,r6        //r14 = r14 + r6
                            ///Continue loop at 022F1914..
                        }
                        else //r6 >= 0x80
                        {
                            022F18C8 E28A1001 add     r1,r10,1h         //r1  = r10 + 1
                            022F18CC E5DAA000 ldrb    r10,[r10]         //r10 = [r10]
                            022F18D0 E3A0B080 mov     r11,80h           //r11 = 0x80
                            022F18D4 EA000001 b       22F18E0h          ///022F18E0 v
                            while( r11 <= r6 )
                            {
                                ///022F18E0
                                022F18E0 E15B0006 cmp     r11,r6
                                022F18E4 DAFFFFFB ble     22F18D8h      ///022F18D8 ^
                                ///022F18D8
                                022F18D8 E4C7A001 strb    r10,[r7],1h       //[r7] = r10, r7 += 1
                                022F18DC E28BB001 add     r11,r11,1h        //r11  = r11 + 1
                            }
                            022F18E8 E246607F sub     r6,r6,7Fh         //r6 = r6 - 0x7F
                            022F18EC E08EE006 add     r14,r14,r6        //r14 = r14 + r6
                            022F18F0 EA000007 b       22F1914h          ///022F1914 v
                            ///Continue loop at 022F1914..
                        }
                    }
                    else //r6 >= 0xC0
                    {
                        022F1898 E1A0B00C mov     r11,r12           //r11 = r12
                        022F189C EA000002 b       22F18ACh          ///022F18AC v
                        while(r11 <= r6)
                        {
                            ///022F18AC
                            022F18AC E15B0006 cmp     r11,r6
                            022F18B0 DAFFFFFA ble     22F18A0h      ///022F18A0 ^
                            ///022F18A0
                            022F18A0 E4D1A001 ldrb    r10,[r1],1h       //r10 = [r1], r1 += 1
                            022F18A4 E28BB001 add     r11,r11,1h        //r11 = r11 + 1
                            022F18A8 E4C7A001 strb    r10,[r7],1h       //[r7] = r10, r7 += 1
                        }
                        022F18B4 E24660BF sub     r6,r6,0BFh        //r6 = r6 - 0xBF
                        022F18B8 E08EE006 add     r14,r14,r6        //r14 = r14 + r6
                        022F18BC EA000014 b       22F1914h          ///022F1914 v
                        ///Continue loop at 022F1914..
                    }
                }
                022F1920 EA00002D b       22F19DCh              ///022F19DC v
                //Exit else to 022F19DC...
            }
            ///022F19DC
            022F19DC E0899006 add     r9,r9,r6              //r9 = r9 + BMA_MapWidth
            022F19E0 E3A060FF mov     r6,0FFh               //r6 = 0xFF
            022F19E4 EA000001 b       22F19F0h              ///022F19F0 v
            while(r9 < 256)
            {
                ///022F19F0
                022F19F0 E3590C01 cmp     r9,100h
                022F19F4 BAFFFFFB blt     22F19E8h          ///022F19E8 ^
                ///022F19E8
                022F19E8 E4C76001 strb    r6,[r7],1h            //[r7] = 0xFF
                022F19EC E2899001 add     r9,r9,1h              //r9 = r9 + 1
            }
            022F19F8 E2844001 add     r4,r4,1h              //r4 = Cnt3 + 1
            022F19FC E2855001 add     r5,r5,1h              //r5 = Cnt1 + 1
            ///Continue loop  to 022F1A00...
        }
        //Exit else to 022F1A38
    }
    022F1A38 EA000034 b       22F1B10h              ///022F1B10 v
    //Continue to 022F1B10...
}
///022F1B10
022F1B10 E1D230F8 ldrsh   r3,[r2,8h]        //r3 = BMA_unk7
022F1B14 E3530000 cmp     r3,0h
if( BMA_unk7 > 0 )
{
    022F1B18 C5D23001 ldrgtb  r3,[r2,1h]    //r3 = BMA_MapHeight
    022F1B1C C2804B01 addgt   r4,r0,400h    //r4 = PtrTargetBuffer + 1024
    022F1B20 C3A06000 movgt   r6,0h         //r6 = 0
    022F1B24 C3530000 cmpgt   r3,0h
}
if((BMA_unk7 <= 0) || ((BMA_unk7 > 0) && (BMA_MapHeight <= 0)) )   //This condition can be triggered either by (BMA_unk7 <= 0) or (BMA_MapHeight <= 0)
{
    022F1B28 DA00004A ble     22F1C58h      ///022F1C58 v
    ///022F1C58
    022F1C58 E1D230F8 ldrsh   r3,[r2,8h]        //r3 = BMA_unk7
    022F1C5C E3530001 cmp     r3,1h 
    if( BMA_unk7 > 1 )
    {
        022F1C60 C5D24001 ldrgtb  r4,[r2,1h]    //r4 = BMA_MapHeight
        022F1C64 C2803B01 addgt   r3,r0,400h    //r3 = r0 + 1024
        022F1C68 C3A05000 movgt   r5,0h         //r5 = 0
        022F1C6C C3540000 cmpgt   r4,0h
    }
    if( BMA_unk7 <= 1 || ((BMA_unk7 > 1) && r(4 > BMA_MapHeight)) )
    {
        022F1C70 DA00004A ble     22F1DA0h      ///022F1DA0 (Return) v
    }
    else
    {
        022F1C74 EA000046 b       22F1D94h          ///022F1D94 v
        
        while(r5 < BMA_MapHeight)
        {
            ///022F1D94
            022F1D94 E5D20001 ldrb    r0,[r2,1h]        //r0 = BMA_MapHeight
            022F1D98 E1550000 cmp     r5,r0
            022F1D9C BAFFFFB5 blt     22F1C78h      ///022F1C78 ^
            
            ///022F1C78
            022F1C78 E3550000 cmp     r5,0h
            022F1C7C E2833005 add     r3,r3,5h          //r3 = r3 + 5
            022F1C80 E3A04000 mov     r4,0h             //r4 = 0
            if( r5 != 0 )
            {
                022F1C84 1A000017 bne     22F1CE8h      ///022F1CE8 v
                ///022F1CE8
                022F1CE8 E2437C01 sub     r7,r3,100h        //r7 = r3 + 256
                022F1CEC E3A00080 mov     r0,80h            //r0 = 0x80 
                022F1CF0 E1A09004 mov     r9,r4             //r9 = r4
                022F1CF4 EA00001F b       22F1D78h          ///022F1D78
                
                while(r4 < BMA_MapWidth)
                {
                    ///022F1D78
                    022F1D78 E5D28000 ldrb    r8,[r2]           //r8 = BMA_MapWidth
                    022F1D7C E1540008 cmp     r4,r8     
                    022F1D80 BAFFFFDC blt     22F1CF8h      ///022F1CF8 ^
                    
                    ///022F1CF8
                    022F1CF8 E4D16001 ldrb    r6,[r1],1h        //r6 = [r1], r1 += 1    
                    022F1CFC E3560080 cmp     r6,80h
                    if( r6 < 0x80 )
                    {
                        022F1D00 BA00000E blt     22F1D40h      ///022F1D40 v
                        ///022F1D40
                        022F1D40 E1A0B009 mov     r11,r9            //r11 = r9 
                        022F1D44 EA000007 b       22F1D68h          ///022F1D68 v
                        
                        while( r11 <= r6 )
                        {
                            ///022F1D68
                            022F1D68 E15B0006 cmp     r11,r6
                            022F1D6C DAFFFFF5 ble     22F1D48h      ///022F1D48 ^
                            
                            ///022F1D48
                            022F1D48 E4D78001 ldrb    r8,[r7],1h        //r8 = [r7], r7 += 1
                            022F1D4C E28BB001 add     r11,r11,1h        //r11 = r11 + 1
                            022F1D50 E3180040 tst     r8,40h
                            if( r8 & 0x40 != 0 )
                            {
                                022F1D54 11A0A003 movne   r10,r3        //r10 = r3
                                022F1D58 14D38001 ldrneb  r8,[r3],1h    //r8 = [r3], r3 += 1
                                022F1D60 13888040 orrne   r8,r8,40h     //r8 = r8 + 0x40
                                022F1D64 15CA8000 strneb  r8,[r10]      //[r10] = r8
                            }
                            else
                            {
                                022F1D5C 02833001 addeq   r3,r3,1h      //r3 = r3 + 1
                            }
                            //022F1D60 and 022F1D64 would be here, but it would take up extra space
                        }
                        022F1D70 E2866001 add     r6,r6,1h          //r6 = r6 + 1
                        022F1D74 E0844006 add     r4,r4,r6          //r4 = r4 + r6
                        //Continue loop at 022F1D78...
                    }
                    else
                    {
                        022F1D04 E1A0B000 mov     r11,r0            //r11 = r0
                        022F1D08 EA000007 b       22F1D2Ch          ///022F1D2C v
                        
                        while(r11 <= r6)
                        {
                            ///022F1D2C
                            022F1D2C E15B0006 cmp     r11,r6
                            022F1D30 DAFFFFF5 ble     22F1D0Ch      ///022F1D0C ^ 
                            ///022F1D0C
                            022F1D0C E4D78001 ldrb    r8,[r7],1h        //r8 = [r7], r7 += 1
                            022F1D10 E28BB001 add     r11,r11,1h        //r11 = r11 + 1
                            022F1D14 E3180040 tst     r8,40h
                            if( r8 & 0x40 == 0 )
                            {
                                022F1D18 01A0A003 moveq   r10,r3        //r10 = r3
                                022F1D1C 04D38001 ldreqb  r8,[r3],1h    //r8 = [r3], r3 += 1 
                                022F1D24 03888040 orreq   r8,r8,40h     //r8 = r8 | 0x40
                                022F1D28 05CA8000 streqb  r8,[r10]      //[r10] = r8 
                            }
                            else
                            {
                                022F1D20 12833001 addne   r3,r3,1h      //r3 = r3 + 1
                            }
                        }
                        022F1D34 E246607F sub     r6,r6,7Fh         //r6 = r6 - 0x7F
                        022F1D38 E0844006 add     r4,r4,r6          //r4 = r4 + r6
                        022F1D3C EA00000D b       22F1D78h          ///022F1D78 v
                        //Continue loop at 022F1D78...
                    }
                    //Continue loop at 022F1D78...
                }
                
            }
            else
            {
            022F1C88 E3A06080 mov     r6,80h            //r6 = 0x80 
            022F1C8C EA000011 b       22F1CD8h          //022F1CD8 v
            while(r4 < BMA_MapWidth)
            {
                ///022F1CD8
                022F1CD8 E5D28000 ldrb    r8,[r2]           //r8 = BMA_MapWidth
                022F1CDC E1540008 cmp     r4,r8
                022F1CE0 BAFFFFEA blt     22F1C90h      ///022F1C90 ^
                ///022F1C90
                022F1C90 E4D18001 ldrb    r8,[r1],1h        //r8 = [r1], r1 += 1
                022F1C94 E3580080 cmp     r8,80h    
                if( r8 < 0x80 )
                {
                    022F1C98 BA00000B blt     22F1CCCh      ///022F1CCC v
                    ///022F1CCC
                    022F1CCC E2880001 add     r0,r8,1h          //r0 = r8 + 1
                    022F1CD0 E0833000 add     r3,r3,r0          //r3 = r3 + r0
                    022F1CD4 E0844000 add     r4,r4,r0          //r4 = r4 + r0
                    //Continue loop at 022F1CD8..
                }
                else
                {
                    022F1C9C E1A07006 mov     r7,r6             //r7 = r6
                    022F1CA0 EA000004 b       22F1CB8h          ///022F1CB8 v

                    while( r7 <= r8 )
                    {
                        ///022F1CB8
                        022F1CB8 E1570008 cmp     r7,r8
                        022F1CBC DAFFFFF8 ble     22F1CA4h      ///022F1CA4 ^
                        ///022F1CA4
                        022F1CA4 E1A09003 mov     r9,r3             //r9 = r3
                        022F1CA8 E4D30001 ldrb    r0,[r3],1h        //r0 = [r3], r3 += 1
                        022F1CAC E2877001 add     r7,r7,1h          //r7 = r7 + 1
                        022F1CB0 E3800040 orr     r0,r0,40h         //r0 = r0 | 0x40    
                        022F1CB4 E5C90000 strb    r0,[r9]           //[r9] = r0
                    }
                    022F1CC0 E248007F sub     r0,r8,7Fh         //r0 = r8 - 0x7F
                    022F1CC4 E0844000 add     r4,r4,r0          //r4 = r4 + r0
                    022F1CC8 EA000002 b       22F1CD8h          ///022F1CD8 v
                    //Continue loop at 022F1CD8..
                }
            }
            022F1CE4 EA000026 b       22F1D84h          ///022F1D84 v
            ///022F1D84
            022F1D84 E2680C01 rsb     r0,r8,100h        //r0 = 256 - r8
            022F1D88 E2400005 sub     r0,r0,5h          //r0 = r0 - 5
            022F1D8C E0833000 add     r3,r3,r0          //r3 = r3 + r0
            022F1D90 E2855001 add     r5,r5,1h          //r5 = r5 + 1
            //Continue loop at 022F1D94...
        }
    }

}
else
{
    022F1B2C EA000046 b       22F1C4Ch          ///022F1C4C v
    
    while( r6 < BMA_MapHeight )
    {
        ///022F1C4C
        022F1C4C E5D23001 ldrb    r3,[r2,1h]        //r3 = BMA_MapHeight
        022F1C50 E1560003 cmp     r6,r3
        022F1C54 BAFFFFB5 blt     22F1B30h      ///022F1B30 ^
        ///022F1B30
        022F1B30 E3560000 cmp     r6,0h
        022F1B34 E2844005 add     r4,r4,5h          //r4 = r4 + 5
        022F1B38 E3A05000 mov     r5,0h             //r5 = 0
        if( r6 != 0 )
        {
            022F1B3C 1A000017 bne     22F1BA0h          ///022F1BA0 v
            ///022F1BA0
            022F1BA0 E2447C01 sub     r7,r4,100h        //r7 = r4 - 256    
            022F1BA4 E3A0C080 mov     r12,80h           //r12 = 0x80
            022F1BA8 E1A03005 mov     r3,r5             //r3 = r5
            022F1BAC EA00001F b       22F1C30h          ///022F1C30 v
            
            while( r5 < BMA_MapWidth )
            {
                ///022F1C30
                022F1C30 E5D28000 ldrb    r8,[r2]           //r8 = BMA_MapWidth
                022F1C34 E1550008 cmp     r5,r8             
                022F1C38 BAFFFFDC blt     22F1BB0h      ///022F1BB0 ^
                ///022F1BB0
                022F1BB0 E4D1E001 ldrb    r14,[r1],1h       //r14 = [r1], r1 += 1
                022F1BB4 E35E0080 cmp     r14,80h   
                if( r14 < 0x80 )
                {
                    022F1BB8 BA00000E blt     22F1BF8h      ///022F1BF8 v
                    ///022F1BF8
                    022F1BF8 E1A08003 mov     r8,r3             //r8 = r3
                    022F1BFC EA000007 b       22F1C20h          ///022F1C20
                    while( r8 <= r14 )
                    {
                        ///022F1C20
                        022F1C20 E158000E cmp     r8,r14
                        022F1C24 DAFFFFF5 ble     22F1C00h      ///022F1C00 ^
                        ///022F1C00
                        022F1C00 E4D79001 ldrb    r9,[r7],1h        //r9 = [r7], r7 += 1
                        022F1C04 E2888001 add     r8,r8,1h          //r8 = r8 + 1
                        022F1C08 E3190080 tst     r9,80h    
                        if( r9 & 0x80 != 0 )
                        {
                            022F1C0C 11A0A004 movne   r10,r4        //r10 = r4
                            022F1C10 14D49001 ldrneb  r9,[r4],1h    //r9 = [r4], r4 += 1
                            022F1C18 13899080 orrne   r9,r9,80h     //r9 = r9 | 0x80
                            022F1C1C 15CA9000 strneb  r9,[r10]      //[r10] = r9
                        }
                        else
                        {
                            022F1C14 02844001 addeq   r4,r4,1h      //r4 = r4 + 1
                        }
                        ///Continue loop at 022F1C20...
                    }
                }
                else
                {
                    022F1BBC E1A0800C mov     r8,r12            //r8 = r12
                    022F1BC0 EA000007 b       22F1BE4h          ///022F1BE4 v
                    while(r8 <= r14)
                    {
                        ///022F1BE4
                        022F1BE4 E158000E cmp     r8,r14
                        022F1BE8 DAFFFFF5 ble     22F1BC4h      ///022F1BC4 ^
                        ///022F1BC4
                        022F1BC4 E4D79001 ldrb    r9,[r7],1h        //r9 = [r7], r7+= 1
                        022F1BC8 E2888001 add     r8,r8,1h          //r8 = r8 + 1
                        022F1BCC E3190080 tst     r9,80h
                        if( r9 & 0x80 == 0 )
                        {
                            022F1BD0 01A0A004 moveq   r10,r4        //r10 = r4
                            022F1BD4 04D49001 ldreqb  r9,[r4],1h    //r9 = [r4], r4 += 1  
                            022F1BDC 03899080 orreq   r9,r9,80h     //r9 = r9 | 0x80
                            022F1BE0 05CA9000 streqb  r9,[r10]      //[r10] = r9
                        }
                        else
                        {
                            022F1BD8 12844001 addne   r4,r4,1h      //r4 = r4 + 1
                        }
                        ///Continue loop at 022F1BE4...
                    }
                    022F1BEC E24E807F sub     r8,r14,7Fh        //r8 = r14 + 0x7F    
                    022F1BF0 E0855008 add     r5,r5,r8          //r5 = r5 + r8
                    022F1BF4 EA00000D b       22F1C30h          ///022F1C30 v
                }
                022F1C28 E28E8001 add     r8,r14,1h         //r8 = r14 + 1
                022F1C2C E0855008 add     r5,r5,r8          //r5 = r5 + r8
            }
            ///Continue loop to 022F1B3C!
        }
        022F1B40 E3A07080 mov     r7,80h            //r7 = 0x80
        022F1B44 EA000011 b       22F1B90h          ///022F1B90 v
        while( r5 < BMA_MapWidth )
        {
            ///022F1B90
            022F1B90 E5D28000 ldrb    r8,[r2]           //r8 = BMA_MapWidth
            022F1B94 E1550008 cmp     r5,r8
            022F1B98 BAFFFFEA blt     22F1B48h      ///022F1B48 ^
            ///022F1B48
            022F1B48 E4D19001 ldrb    r9,[r1],1h        //r9 = [r1], r1 += 1
            022F1B4C E3590080 cmp     r9,80h
            if(r9 < 0x80)
            {
                022F1B50 BA00000B blt     22F1B84h      ///022F1B84 v
                ///022F1B84
                022F1B84 E2893001 add     r3,r9,1h          //r3 = r9 + 1
                022F1B88 E0844003 add     r4,r4,r3          //r4 = r4 + r3
                022F1B8C E0855003 add     r5,r5,r3          //r5 = r5 + r3
            }
            else
            {
                022F1B54 E1A08007 mov     r8,r7             //r8 = r7
                022F1B58 EA000004 b       22F1B70h          ///022F1B70 v
                
                ///022F1B70
                022F1B70 E1580009 cmp     r8,r9
                if( r8 <= r9 )
                {
                    022F1B74 DAFFFFF8 ble     22F1B5Ch      ///022F1B5C ^
                    ///022F1B5C
                    022F1B5C E1A0A004 mov     r10,r4            //r10 = r4
                    022F1B60 E4D43001 ldrb    r3,[r4],1h        //r3 = [r4], r4 += 1
                    022F1B64 E2888001 add     r8,r8,1h          //r8 = r8 + 1
                    022F1B68 E3833080 orr     r3,r3,80h         //r3 = r3 | 0x80
                    022F1B6C E5CA3000 strb    r3,[r10]          //[r10] = r3
                }
                022F1B78 E249307F sub     r3,r9,7Fh         //r3 = r9 - 0x7F
                022F1B7C E0855003 add     r5,r5,r3          //r5 = r5 + r3
                022F1B80 EA000002 b       22F1B90h          ///022F1B90 v
            }
            ///Continue loop to 022F1B90...
        }
        022F1B9C EA000026 b       22F1C3Ch          ///022F1C3C v
        
        ///022F1C3C
        022F1C3C E2683C01 rsb     r3,r8,100h        //r3 = 256 - r8
        022F1C40 E2433005 sub     r3,r3,5h          //r3 = r3 - 5
        022F1C44 E0844003 add     r4,r4,r3          //r4 = r4 + r3
        022F1C48 E2866001 add     r6,r6,1h          //r6 = r6 + 1   
        ///Continue loop to 022F1C4C...
    }
}
///022F1DA0
022F1DA0 E1A00001 mov     r0,r1             //r0 = r1
022F1DA4 E8BD8FF8 pop     r3-r11,r15        //return


