/*
    fun_022F12C0 GroundMap_Select 22F12C0h

*/
022F12C0 E92D4070 push    r4-r6,r14
022F12C4 E24DD008 sub     r13,r13,8h
022F12C8 E1A05000 mov     r5,r0
022F12CC E59F1260 ldr     r1,=2320FB4h          //"GroundMap Select %3d"
022F12D0 E1A02005 mov     r2,r5
022F12D4 E3A00001 mov     r0,1h
022F12D8 EBF46BD8 bl      200C240h              //DebugPrint
022F12DC EBFFFFD8 bl      22F1244h              //022F1244
{
    022F1244 E92D4008 push    r3,r14
    022F1248 EB008083 bl      231145Ch
    022F124C E3500000 cmp     r0,0h
    022F1250 0A000000 beq     22F1258h
    022F1254 EB008063 bl      23113E8h
    022F1258 EB0081BF bl      231195Ch
    022F125C E3500000 cmp     r0,0h
    022F1260 0A000000 beq     22F1268h
    022F1264 EB00819F bl      23118E8h
    022F1268 E59F004C ldr     r0,=2324CC0h
    022F126C E3A02000 mov     r2,0h
    022F1270 E5901004 ldr     r1,[r0,4h]
    022F1274 E5C12003 strb    r2,[r1,3h]
    022F1278 E5900004 ldr     r0,[r0,4h]
    022F127C E5C02002 strb    r2,[r0,2h]
    022F1280 EB000762 bl      22F3010h
    022F1284 E3A00000 mov     r0,0h
    022F1288 EB000419 bl      22F22F4h
    022F128C E59F0028 ldr     r0,=2324CC0h
    022F1290 E5900000 ldr     r0,[r0]
    022F1294 E3500000 cmp     r0,0h
    022F1298 08BD8008 popeq   r3,r15
    022F129C EBFFEB02 bl      22EBEACh
    022F12A0 E59F0014 ldr     r0,=2324CC0h
    022F12A4 E5900000 ldr     r0,[r0]
    022F12A8 EBF43FB6 bl      2001188h
    022F12AC E59F0008 ldr     r0,=2324CC0h
    022F12B0 E3A01000 mov     r1,0h
    022F12B4 E5801000 str     r1,[r0]
    022F12B8 E8BD8008 pop     r3,r15
    022F12BC 02324CC0
}
022F12E0 E3A00FBD mov     r0,2F4h
022F12E4 E3A01006 mov     r1,6h
022F12E8 EBF43FA0 bl      2001170h                  
022F12EC E59F2244 ldr     r2,=2324CC0h
022F12F0 E3E01000 mvn     r1,0h
022F12F4 E5820000 str     r0,[r2]
022F12F8 E5920004 ldr     r0,[r2,4h]
022F12FC E1550001 cmp     r5,r1
022F1300 E1C050B0 strh    r5,[r0]
022F1304 1A000010 bne     22F134Ch
022F1308 E5920000 ldr     r0,[r2]
022F130C E59F1228 ldr     r1,=2320CD8h
022F1310 EBFFEA40 bl      22EBC18h
022F1314 E59F021C ldr     r0,=2324CC0h
022F1318 E3E01000 mvn     r1,0h
022F131C E5900000 ldr     r0,[r0]
022F1320 EBFFEBD5 bl      22EC27Ch
022F1324 E59F020C ldr     r0,=2324CC0h
022F1328 E8900006 ldmia   [r0],r1,r2
022F132C E2820004 add     r0,r2,4h
022F1330 EBFFFA89 bl      22EFD5Ch
022F1334 E59F01FC ldr     r0,=2324CC0h
022F1338 E5901004 ldr     r1,[r0,4h]
022F133C E2810020 add     r0,r1,20h
022F1340 E2811004 add     r1,r1,4h
022F1344 EBFFFC1E bl      22F03C4h
022F1348 EA000066 b       22F14E8h
022F134C E3A0000C mov     r0,0Ch
022F1350 E1630085 smulbb  r3,r5,r0
022F1354 E59F41E4 ldr     r4,=20A5488h          //Level list beg
022F1358 E19400F3 ldrsh   r0,[r4,r3]
022F135C E0844003 add     r4,r4,r3
022F1360 E2800001 add     r0,r0,1h
022F1364 E350000C cmp     r0,0Ch
022F1368 908FF100 addls   r15,r15,r0,lsl 2h
022F136C EA00003E b       22F146Ch
022F1370 EA000033 b       22F1444h
022F1374 EA00003C b       22F146Ch
022F1378 EA00003B b       22F146Ch
022F137C EA00003A b       22F146Ch
022F1380 EA000039 b       22F146Ch
022F1384 EA000038 b       22F146Ch
022F1388 EA000005 b       22F13A4h
022F138C EA000004 b       22F13A4h
022F1390 EA000003 b       22F13A4h
022F1394 EA000002 b       22F13A4h
022F1398 EA000001 b       22F13A4h
022F139C EA000008 b       22F13C4h
022F13A0 EA000007 b       22F13C4h
022F13A4 E5920000 ldr     r0,[r2]
022F13A8 E59F1194 ldr     r1,=2320CF4h
022F13AC EBFFEA19 bl      22EBC18h
022F13B0 E3A00000 mov     r0,0h
022F13B4 EBFFF860 bl      22EF53Ch
022F13B8 E3A00002 mov     r0,2h
022F13BC EB0003CC bl      22F22F4h
022F13C0 EA000031 b       22F148Ch
022F13C4 E59F617C ldr     r6,=2320D2Ch
022F13C8 EA000011 b       22F1414h
022F13CC E1500005 cmp     r0,r5
022F13D0 1A00000E bne     22F1410h
022F13D4 E59F015C ldr     r0,=2324CC0h
022F13D8 E5900000 ldr     r0,[r0]
022F13DC EBF43F69 bl      2001188h
022F13E0 E1D410F0 ldrsh   r1,[r4]
022F13E4 E59F014C ldr     r0,=2324CC0h
022F13E8 E3A03000 mov     r3,0h
022F13EC E5803000 str     r3,[r0]
022F13F0 E351000B cmp     r1,0Bh
022F13F4 03A03001 moveq   r3,1h
022F13F8 E5962004 ldr     r2,[r6,4h]
022F13FC E1A00005 mov     r0,r5
022F1400 E2861002 add     r1,r6,2h
022F1404 E20330FF and     r3,r3,0FFh
022F1408 EB000053 bl      22F155Ch
022F140C EA000046 b       22F152Ch
022F1410 E2866008 add     r6,r6,8h
022F1414 E1D600F0 ldrsh   r0,[r6]
022F1418 E1500001 cmp     r0,r1
022F141C 1AFFFFEA bne     22F13CCh
022F1420 E59F0110 ldr     r0,=2324CC0h
022F1424 E59F1120 ldr     r1,=2320D10h
022F1428 E5900000 ldr     r0,[r0]
022F142C EBFFE9F9 bl      22EBC18h
022F1430 E3A00000 mov     r0,0h
022F1434 EBFFF840 bl      22EF53Ch
022F1438 E3A00001 mov     r0,1h
022F143C EB0003AC bl      22F22F4h
022F1440 EA000011 b       22F148Ch
022F1444 E59F0104 ldr     r0,=2320CA8h
022F1448 E1D430F0 ldrsh   r3,[r4]
022F144C E590C01C ldr     r12,[r0,1Ch]
022F1450 E590E018 ldr     r14,[r0,18h]
022F1454 E59F10F8 ldr     r1,=2320FCCh
022F1458 E28D0000 add     r0,r13,0h
022F145C E1A02005 mov     r2,r5
022F1460 E58DE000 str     r14,[r13]
022F1464 E58DC004 str     r12,[r13,4h]
022F1468 EBF46B7B bl      200C25Ch
022F146C E59F00C4 ldr     r0,=2324CC0h
022F1470 E59F10C4 ldr     r1,=2320CD8h
022F1474 E5900000 ldr     r0,[r0]
022F1478 EBFFE9E6 bl      22EBC18h
022F147C E3A00000 mov     r0,0h
022F1480 EBFFF82D bl      22EF53Ch
022F1484 E3A00001 mov     r0,1h
022F1488 EB000399 bl      22F22F4h
022F148C E59F00A4 ldr     r0,=2324CC0h
022F1490 E1D410F4 ldrsh   r1,[r4,4h]
022F1494 E5900000 ldr     r0,[r0]
022F1498 EBFFEB77 bl      22EC27Ch
022F149C E59F0094 ldr     r0,=2324CC0h
022F14A0 E8900006 ldmia   [r0],r1,r2
022F14A4 E2820004 add     r0,r2,4h
022F14A8 EBFFFA2B bl      22EFD5Ch
022F14AC E59F0084 ldr     r0,=2324CC0h
022F14B0 E5901004 ldr     r1,[r0,4h]
022F14B4 E2810020 add     r0,r1,20h
022F14B8 E2811004 add     r1,r1,4h
022F14BC EBFFFBC0 bl      22F03C4h
022F14C0 E1D400F6 ldrsh   r0,[r4,6h]
022F14C4 E3E01000 mvn     r1,0h
022F14C8 E1500001 cmp     r0,r1
022F14CC 0A000000 beq     22F14D4h
022F14D0 EB000433 bl      22F25A4h
022F14D4 E59F005C ldr     r0,=2324CC0h
022F14D8 E59F2078 ldr     r2,=2320CA8h
022F14DC E5900000 ldr     r0,[r0]
022F14E0 E3A01000 mov     r1,0h
022F14E4 EBFFF77A bl      22EF2D4h
022F14E8 E59F0048 ldr     r0,=2324CC0h
022F14EC E3A01001 mov     r1,1h
022F14F0 E5900004 ldr     r0,[r0,4h]
022F14F4 E5C01002 strb    r1,[r0,2h]
022F14F8 EB007FD7 bl      231145Ch
022F14FC E3500000 cmp     r0,0h
022F1500 0A000003 beq     22F1514h
022F1504 E3A00000 mov     r0,0h
022F1508 E1A01000 mov     r1,r0
022F150C EB006F81 bl      230D318h
022F1510 EA000005 b       22F152Ch
022F1514 EB008110 bl      231195Ch
022F1518 E3500000 cmp     r0,0h
022F151C 0A000002 beq     22F152Ch
022F1520 E3A00000 mov     r0,0h
022F1524 E1A01000 mov     r1,r0
022F1528 EB006F7A bl      230D318h
022F152C E28DD008 add     r13,r13,8h
022F1530 E8BD8070 pop     r4-r6,r15
022F1534 02320FB4 
022F1538 02324CC0 
022F153C 02320CD8 
022F1540 020A5488 
022F1544 02320CF4 
022F1548 02320D2C 
022F154C 02320D10 
022F1550 02320CA8 
022F1554 02320FCC 
022F1558 02320CA8 