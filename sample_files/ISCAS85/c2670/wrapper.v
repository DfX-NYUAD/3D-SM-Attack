/*
###############################################################
#  Generated by:      Cadence Innovus 17.11-s080_1
#  OS:                Linux x86_64(Host ID aduae260-lap)
#  Generated on:      Sun Sep 30 18:12:29 2018
#  Design:            c2670
#  Command:           saveNetlist -module c2670 outputs/wrapper_j.v
###############################################################
*/
module c2670 (
	G1, 
	G2, 
	G3, 
	G4, 
	G5, 
	G6, 
	G7, 
	G8, 
	G11, 
	G14, 
	G15, 
	G16, 
	G19, 
	G20, 
	G21, 
	G22, 
	G23, 
	G24, 
	G25, 
	G26, 
	G27, 
	G28, 
	G29, 
	G32, 
	G33, 
	G34, 
	G35, 
	G36, 
	G37, 
	G40, 
	G43, 
	G44, 
	G47, 
	G48, 
	G49, 
	G50, 
	G51, 
	G52, 
	G53, 
	G54, 
	G55, 
	G56, 
	G57, 
	G60, 
	G61, 
	G62, 
	G63, 
	G64, 
	G65, 
	G66, 
	G67, 
	G68, 
	G69, 
	G72, 
	G73, 
	G74, 
	G75, 
	G76, 
	G77, 
	G78, 
	G79, 
	G80, 
	G81, 
	G82, 
	G85, 
	G86, 
	G87, 
	G88, 
	G89, 
	G90, 
	G91, 
	G92, 
	G93, 
	G94, 
	G95, 
	G96, 
	G99, 
	G100, 
	G101, 
	G102, 
	G103, 
	G104, 
	G105, 
	G106, 
	G107, 
	G108, 
	G111, 
	G112, 
	G113, 
	G114, 
	G115, 
	G116, 
	G117, 
	G118, 
	G119, 
	G120, 
	G123, 
	G124, 
	G125, 
	G126, 
	G127, 
	G128, 
	G129, 
	G130, 
	G131, 
	G132, 
	G135, 
	G136, 
	G137, 
	G138, 
	G139, 
	G140, 
	G141, 
	G142, 
	G169, 
	G174, 
	G177, 
	G178, 
	G179, 
	G180, 
	G181, 
	G182, 
	G183, 
	G184, 
	G185, 
	G186, 
	G189, 
	G190, 
	G191, 
	G192, 
	G193, 
	G194, 
	G195, 
	G196, 
	G197, 
	G198, 
	G199, 
	G200, 
	G201, 
	G202, 
	G203, 
	G204, 
	G205, 
	G206, 
	G207, 
	G208, 
	G209, 
	G210, 
	G211, 
	G212, 
	G213, 
	G214, 
	G215, 
	G239, 
	G240, 
	G241, 
	G242, 
	G243, 
	G244, 
	G245, 
	G246, 
	G247, 
	G248, 
	G249, 
	G250, 
	G251, 
	G252, 
	G253, 
	G254, 
	G255, 
	G256, 
	G257, 
	G262, 
	G263, 
	G264, 
	G265, 
	G266, 
	G267, 
	G268, 
	G269, 
	G270, 
	G271, 
	G272, 
	G273, 
	G274, 
	G275, 
	G276, 
	G277, 
	G278, 
	G452, 
	G483, 
	G543, 
	G559, 
	G567, 
	G651, 
	G661, 
	G860, 
	G868, 
	G1083, 
	G1341, 
	G1348, 
	G1384, 
	G1956, 
	G1961, 
	G1966, 
	G1971, 
	G1976, 
	G1981, 
	G1986, 
	G1991, 
	G1996, 
	G2066, 
	G2067, 
	G2072, 
	G2078, 
	G2084, 
	G2090, 
	G2096, 
	G2100, 
	G2104, 
	G2105, 
	G2106, 
	G2427, 
	G2430, 
	G2435, 
	G2438, 
	G2443, 
	G2446, 
	G2451, 
	G2454, 
	G2474, 
	G2678, 
	G350, 
	G335, 
	G409, 
	G369, 
	G367, 
	G411, 
	G337, 
	G384, 
	G218, 
	G219, 
	G220, 
	G221, 
	G235, 
	G236, 
	G237, 
	G238, 
	G158, 
	G259, 
	G391, 
	G173, 
	G223, 
	G234, 
	G217, 
	G325, 
	G261, 
	G319, 
	G160, 
	G162, 
	G164, 
	G166, 
	G168, 
	G171, 
	G153, 
	G176, 
	G188, 
	G299, 
	G301, 
	G286, 
	G303, 
	G288, 
	G305, 
	G290, 
	G284, 
	G321, 
	G297, 
	G280, 
	G148, 
	G282, 
	G323, 
	G156, 
	G401, 
	G227, 
	G229, 
	G311, 
	G150, 
	G145, 
	G395, 
	G295, 
	G331, 
	G397, 
	G329, 
	G231, 
	G308, 
	G225);
   input G1;
   input G2;
   input G3;
   input G4;
   input G5;
   input G6;
   input G7;
   input G8;
   input G11;
   input G14;
   input G15;
   input G16;
   input G19;
   input G20;
   input G21;
   input G22;
   input G23;
   input G24;
   input G25;
   input G26;
   input G27;
   input G28;
   input G29;
   input G32;
   input G33;
   input G34;
   input G35;
   input G36;
   input G37;
   input G40;
   input G43;
   input G44;
   input G47;
   input G48;
   input G49;
   input G50;
   input G51;
   input G52;
   input G53;
   input G54;
   input G55;
   input G56;
   input G57;
   input G60;
   input G61;
   input G62;
   input G63;
   input G64;
   input G65;
   input G66;
   input G67;
   input G68;
   input G69;
   input G72;
   input G73;
   input G74;
   input G75;
   input G76;
   input G77;
   input G78;
   input G79;
   input G80;
   input G81;
   input G82;
   input G85;
   input G86;
   input G87;
   input G88;
   input G89;
   input G90;
   input G91;
   input G92;
   input G93;
   input G94;
   input G95;
   input G96;
   input G99;
   input G100;
   input G101;
   input G102;
   input G103;
   input G104;
   input G105;
   input G106;
   input G107;
   input G108;
   input G111;
   input G112;
   input G113;
   input G114;
   input G115;
   input G116;
   input G117;
   input G118;
   input G119;
   input G120;
   input G123;
   input G124;
   input G125;
   input G126;
   input G127;
   input G128;
   input G129;
   input G130;
   input G131;
   input G132;
   input G135;
   input G136;
   input G137;
   input G138;
   input G139;
   input G140;
   input G141;
   input G142;
   input G169;
   input G174;
   input G177;
   input G178;
   input G179;
   input G180;
   input G181;
   input G182;
   input G183;
   input G184;
   input G185;
   input G186;
   input G189;
   input G190;
   input G191;
   input G192;
   input G193;
   input G194;
   input G195;
   input G196;
   input G197;
   input G198;
   input G199;
   input G200;
   input G201;
   input G202;
   input G203;
   input G204;
   input G205;
   input G206;
   input G207;
   input G208;
   input G209;
   input G210;
   input G211;
   input G212;
   input G213;
   input G214;
   input G215;
   input G239;
   input G240;
   input G241;
   input G242;
   input G243;
   input G244;
   input G245;
   input G246;
   input G247;
   input G248;
   input G249;
   input G250;
   input G251;
   input G252;
   input G253;
   input G254;
   input G255;
   input G256;
   input G257;
   input G262;
   input G263;
   input G264;
   input G265;
   input G266;
   input G267;
   input G268;
   input G269;
   input G270;
   input G271;
   input G272;
   input G273;
   input G274;
   input G275;
   input G276;
   input G277;
   input G278;
   input G452;
   input G483;
   input G543;
   input G559;
   input G567;
   input G651;
   input G661;
   input G860;
   input G868;
   input G1083;
   input G1341;
   input G1348;
   input G1384;
   input G1956;
   input G1961;
   input G1966;
   input G1971;
   input G1976;
   input G1981;
   input G1986;
   input G1991;
   input G1996;
   input G2066;
   input G2067;
   input G2072;
   input G2078;
   input G2084;
   input G2090;
   input G2096;
   input G2100;
   input G2104;
   input G2105;
   input G2106;
   input G2427;
   input G2430;
   input G2435;
   input G2438;
   input G2443;
   input G2446;
   input G2451;
   input G2454;
   input G2474;
   input G2678;
   output G350;
   output G335;
   output G409;
   output G369;
   output G367;
   output G411;
   output G337;
   output G384;
   output G218;
   output G219;
   output G220;
   output G221;
   output G235;
   output G236;
   output G237;
   output G238;
   output G158;
   output G259;
   output G391;
   output G173;
   output G223;
   output G234;
   output G217;
   output G325;
   output G261;
   output G319;
   output G160;
   output G162;
   output G164;
   output G166;
   output G168;
   output G171;
   output G153;
   output G176;
   output G188;
   output G299;
   output G301;
   output G286;
   output G303;
   output G288;
   output G305;
   output G290;
   output G284;
   output G321;
   output G297;
   output G280;
   output G148;
   output G282;
   output G323;
   output G156;
   output G401;
   output G227;
   output G229;
   output G311;
   output G150;
   output G145;
   output G395;
   output G295;
   output G331;
   output G397;
   output G329;
   output G231;
   output G308;
   output G225;

   // Internal wires
   wire n_3;
   wire n_7;
   wire n_18;
   wire n_22;
   wire n_23;
   wire n_27;
   wire n_29;
   wire n_30;
   wire n_32;
   wire n_33;
   wire n_35;
   wire n_36;
   wire n_37;
   wire n_39;
   wire n_40;
   wire n_41;
   wire n_43;
   wire n_44;
   wire n_46;
   wire n_51;
   wire n_53;
   wire n_54;
   wire n_55;
   wire n_56;
   wire n_58;
   wire n_59;
   wire n_60;
   wire n_61;
   wire n_65;
   wire n_67;
   wire n_68;
   wire n_69;
   wire n_72;
   wire n_74;
   wire n_75;
   wire n_77;
   wire n_80;
   wire n_81;
   wire n_82;
   wire n_83;
   wire n_84;
   wire n_85;
   wire n_89;
   wire n_90;
   wire n_92;
   wire n_96;
   wire n_98;
   wire n_100;
   wire n_105;
   wire n_106;
   wire n_109;
   wire n_110;
   wire n_112;
   wire n_115;
   wire n_123;
   wire n_125;
   wire n_126;
   wire n_127;
   wire n_128;
   wire n_130;
   wire n_131;
   wire n_132;
   wire n_134;
   wire n_135;
   wire n_136;
   wire n_141;
   wire n_143;
   wire n_144;
   wire n_146;
   wire n_147;
   wire n_148;
   wire n_149;
   wire n_150;
   wire n_151;
   wire n_152;
   wire n_158;
   wire n_161;
   wire n_163;
   wire n_166;
   wire n_167;
   wire n_169;
   wire n_172;
   wire n_175;
   wire n_176;
   wire n_177;
   wire n_178;
   wire n_179;
   wire n_180;
   wire n_181;
   wire n_182;
   wire n_183;
   wire n_185;
   wire n_186;
   wire n_187;
   wire n_188;
   wire n_189;
   wire n_190;
   wire n_191;
   wire n_192;
   wire n_193;
   wire n_194;
   wire n_195;
   wire n_197;
   wire n_200;
   wire n_203;
   wire n_204;
   wire n_208;
   wire n_210;
   wire n_211;
   wire n_215;
   wire n_216;
   wire n_217;
   wire n_219;
   wire n_220;
   wire n_221;
   wire n_222;
   wire n_226;
   wire n_227;
   wire n_228;
   wire n_229;
   wire n_231;
   wire n_232;
   wire n_234;
   wire n_237;
   wire n_239;
   wire n_242;
   wire n_244;
   wire n_245;
   wire n_246;
   wire n_250;
   wire n_251;
   wire n_252;
   wire n_259;
   wire n_261;
   wire n_263;
   wire n_265;

   assign G350 = G452 ;
   assign G335 = G350 ;
   assign G409 = G452 ;
   assign G369 = G1083 ;
   assign G367 = G1083 ;
   assign G411 = G2066 ;
   assign G337 = G2066 ;
   assign G384 = G2066 ;
   assign G391 = G452 ;
   assign G321 = G284 ;
   assign G280 = G297 ;
   assign G323 = G282 ;
   assign G331 = G295 ;

   bottom bottom (.G1(G1),
	.G2(G2),
	.G3(G3),
	.G7(G7),
	.G8(G8),
	.G11(G11),
	.G14(G14),
	.G15(G15),
	.G16(G16),
	.G20(G20),
	.G21(G21),
	.G24(G24),
	.G25(G25),
	.G28(G28),
	.G29(G29),
	.G33(G33),
	.G35(G35),
	.G40(G40),
	.G44(G44),
	.G47(G47),
	.G48(G48),
	.G49(G49),
	.G50(G50),
	.G51(G51),
	.G52(G52),
	.G54(G54),
	.G55(G55),
	.G57(G57),
	.G60(G60),
	.G61(G61),
	.G62(G62),
	.G63(G63),
	.G64(G64),
	.G66(G66),
	.G67(G67),
	.G68(G68),
	.G69(G69),
	.G74(G74),
	.G75(G75),
	.G79(G79),
	.G81(G81),
	.G87(G87),
	.G88(G88),
	.G92(G92),
	.G95(G95),
	.G96(G96),
	.G99(G99),
	.G100(G100),
	.G101(G101),
	.G103(G103),
	.G104(G104),
	.G106(G106),
	.G108(G108),
	.G114(G114),
	.G115(G115),
	.G117(G117),
	.G118(G118),
	.G120(G120),
	.G126(G126),
	.G127(G127),
	.G129(G129),
	.G130(G130),
	.G131(G131),
	.G132(G132),
	.G135(G135),
	.G136(G136),
	.G137(G137),
	.G139(G139),
	.G140(G140),
	.G142(G142),
	.G483(G483),
	.G543(G543),
	.G559(G559),
	.G567(G567),
	.G651(G651),
	.G661(G661),
	.G860(G860),
	.G868(G868),
	.G1341(G1341),
	.G1348(G1348),
	.G1384(G1384),
	.G1956(G1956),
	.G1961(G1961),
	.G1966(G1966),
	.G1971(G1971),
	.G1981(G1981),
	.G1986(G1986),
	.G1991(G1991),
	.G1996(G1996),
	.G2067(G2067),
	.G2072(G2072),
	.G2078(G2078),
	.G2084(G2084),
	.G2090(G2090),
	.G2096(G2096),
	.G2100(G2100),
	.G2104(G2104),
	.G2105(G2105),
	.G2106(G2106),
	.G2435(G2435),
	.G2438(G2438),
	.G2678(G2678),
	.g27767_ZN(G308),
	.g27791_ZN(n_227),
	.g27778_ZN(n_259),
	.g28015_ZN(n_172),
	.g27781_ZN(n_250),
	.g27780_ZN(n_263),
	.g27794_ZN(n_239),
	.g27805_ZN(n_245),
	.g27837_Z(n_193),
	.g27790_ZN(n_242),
	.g27801_ZN(n_220),
	.g27786_ZN(n_246),
	.g27842_ZN(n_187),
	.g27803_ZN(n_232),
	.g28032_ZN(n_251),
	.g28040_ZN(n_222),
	.g27798_ZN(n_211),
	.g27854_ZN(n_134),
	.g27806_ZN(n_190),
	.g27874_ZN(n_217),
	.g27866_ZN(n_216),
	.g27840_ZN(n_167),
	.g27876_ZN(n_166),
	.g27883_ZN(n_144),
	.g27862_ZN(n_128),
	.g27906_ZN(n_188),
	.g27859_ZN(n_131),
	.g27861_ZN(n_130),
	.g27872_ZN(n_191),
	.g28020_ZN(n_183),
	.g27867_ZN(n_186),
	.g28021_ZN(n_185),
	.g27869_ZN(n_126),
	.g27864_ZN(n_112),
	.g27886_ZN(n_152),
	.g27936_ZN(n_151),
	.g27873_ZN(n_109),
	.g28034_ZN(n_35),
	.g27996_ZN(n_37),
	.g27914_ZN(n_127),
	.g27905_ZN(G286),
	.g27911_ZN(n_123),
	.g27930_ZN(G288),
	.g27922_ZN(n_150),
	.g27937_ZN(n_98),
	.g28004_ZN(n_106),
	.g27898_ZN(n_96),
	.g27992_ZN(n_32),
	.g28006_ZN(n_81),
	.g28014_ZN(n_80),
	.g27953_ZN(n_77),
	.g27948_ZN(n_54),
	.g27949_ZN(n_53),
	.g27952_ZN(n_51),
	.g27968_ZN(n_67),
	.g27973_ZN(n_92),
	.g27942_ZN(n_58),
	.g27982_ZN(n_41),
	.g28033_ZN(n_89),
	.g27984_ZN(n_59),
	.g27990_ZN(n_27),
	.g27997_Z(n_22),
	.g27980_ZN(n_44),
	.g28005_ZN(n_85),
	.g28007_ZN(n_61),
	.g28028_ZN(n_23),
	.g28030_ZN(n_43),
	.g28037_ZN(n_39),
	.G218(G218),
	.G219(G219),
	.G221(G221),
	.G158(G158),
	.G259(G259),
	.G223(G223),
	.G234(G234),
	.G217(G217),
	.G319(G319),
	.G160(G160),
	.G164(G164),
	.G166(G166),
	.G153(G153),
	.G188(G188),
	.G299(G299),
	.G290(G290),
	.G156(G156),
	.G401(G401),
	.G145(G145),
	.G331(G295),
	.G329(G329),
	.G231(G231),
	.G225(G225),
	.g27769_A1(n_265),
	.g27780_A2(n_244),
	.g27780_A1(n_261),
	.g27805_A(n_221),
	.g27786_A(n_208),
	.g27801_A1(n_219),
	.g27794_B1(n_229),
	.g2_A(n_234),
	.g27789_A2(n_200),
	.g27769_A4(n_195),
	.g27810_A(n_194),
	.g27789_A3(n_192),
	.g27789_A4(n_189),
	.g27798_A1(n_210),
	.g27812_A(n_182),
	.g27778_B(n_178),
	.g27807_A(n_175),
	.g27791_A4(n_226),
	.g27795_A(n_161),
	.g27811_A1(n_197),
	.g27824_A(n_147),
	.g2_B(n_163),
	.g27806_C1(n_204),
	.g27807_B1(n_136),
	.g27792_C1(n_252),
	.g27810_C1(n_135),
	.g27828_A(n_176),
	.g27840_A1(n_177),
	.g27826_A2(n_169),
	.g27806_B1(n_203),
	.g27807_C1(n_179),
	.g27805_B1(n_181),
	.g27887_B(n_158),
	.g28043_A1(n_115),
	.g28044_A1(n_110),
	.g27888_A1(n_237),
	.g27883_A1(n_143),
	.g27876_A2(n_149),
	.g27786_B1(n_146),
	.g27901_A(n_141),
	.g27889_A1(n_228),
	.g27874_A1(n_125),
	.g27922_A(n_148),
	.g27873_A1(n_100),
	.g27886_A(n_90),
	.g27909_A2(n_83),
	.g27909_A1(n_82),
	.g27911_A1(n_75),
	.g27917_A1(n_74),
	.g27914_A2(n_72),
	.g27955_A(n_69),
	.g27926_A1(n_68),
	.g27956_A(n_65),
	.g27930_B(n_46),
	.g27937_B(n_40),
	.g27898_A(n_36),
	.g27930_C1(n_30),
	.g27941_A1(n_60),
	.g27898_B(n_29),
	.g28008_A(n_84),
	.g27968_A1(n_105),
	.g27971_B1(n_55),
	.g27971_A1(n_56),
	.g28000_A(n_18),
	.g27992_A(n_180),
	.g27864_B1(n_132),
	.g27930_C2(n_7),
	.g27803_B2(n_215),
	.g27803_C2(n_231),
	.g27872_B1(n_3),
	.g27991_B1(n_33));
   top top (.G4(G4),
	.G5(G5),
	.G6(G6),
	.G8(G8),
	.G16(G16),
	.G19(G19),
	.G22(G22),
	.G23(G23),
	.G26(G26),
	.G27(G27),
	.G29(G29),
	.G32(G32),
	.G34(G34),
	.G36(G36),
	.G37(G37),
	.G40(G40),
	.G43(G43),
	.G44(G44),
	.G53(G53),
	.G56(G56),
	.G57(G57),
	.G65(G65),
	.G69(G69),
	.G72(G72),
	.G73(G73),
	.G76(G76),
	.G77(G77),
	.G78(G78),
	.G80(G80),
	.G82(G82),
	.G85(G85),
	.G86(G86),
	.G89(G89),
	.G90(G90),
	.G91(G91),
	.G93(G93),
	.G94(G94),
	.G96(G96),
	.G102(G102),
	.G105(G105),
	.G107(G107),
	.G108(G108),
	.G111(G111),
	.G112(G112),
	.G113(G113),
	.G116(G116),
	.G119(G119),
	.G120(G120),
	.G123(G123),
	.G124(G124),
	.G125(G125),
	.G128(G128),
	.G132(G132),
	.G138(G138),
	.G141(G141),
	.G452(G452),
	.G543(G543),
	.G651(G651),
	.G860(G860),
	.G868(G868),
	.G1341(G1341),
	.G1348(G1348),
	.G1384(G1384),
	.G1956(G1956),
	.G1961(G1961),
	.G1966(G1966),
	.G1971(G1971),
	.G1976(G1976),
	.G1981(G1981),
	.G1986(G1986),
	.G1991(G1991),
	.G1996(G1996),
	.G2067(G2067),
	.G2072(G2072),
	.G2078(G2078),
	.G2090(G2090),
	.G2096(G2096),
	.G2100(G2100),
	.G2104(G2104),
	.G2105(G2105),
	.G2427(G2427),
	.G2430(G2430),
	.G2443(G2443),
	.G2446(G2446),
	.G2451(G2451),
	.G2454(G2454),
	.G2474(G2474),
	.bottom_G401(G401),
	.bottom_g27769_A1(n_265),
	.bottom_g27769_A4(n_195),
	.bottom_g27780_A1(n_261),
	.bottom_g27780_A2(n_244),
	.bottom_g27778_B(n_178),
	.bottom_g2_A(n_234),
	.bottom_g27789_A2(n_200),
	.bottom_g27789_A3(n_192),
	.bottom_g27789_A4(n_189),
	.bottom_g27792_C1(n_252),
	.bottom_g27888_A1(n_237),
	.bottom_g27786_A(n_208),
	.bottom_g27786_B1(n_146),
	.bottom_g27803_C2(n_231),
	.bottom_g27805_A(n_221),
	.bottom_g27805_B1(n_181),
	.bottom_g27992_A(n_180),
	.bottom_g27794_B1(n_229),
	.bottom_g27889_A1(n_228),
	.bottom_g27795_A(n_161),
	.bottom_g2_B(n_163),
	.bottom_g27803_B2(n_215),
	.bottom_g27798_A1(n_210),
	.bottom_g27791_A4(n_226),
	.bottom_G319(G319),
	.bottom_g27826_A2(n_169),
	.bottom_g27811_A1(n_197),
	.bottom_g27801_A1(n_219),
	.bottom_g27810_A(n_194),
	.bottom_g27810_C1(n_135),
	.bottom_g27807_A(n_175),
	.bottom_g27807_B1(n_136),
	.bottom_g27807_C1(n_179),
	.bottom_g27812_A(n_182),
	.bottom_g27806_C1(n_204),
	.bottom_g27806_B1(n_203),
	.bottom_g27824_A(n_147),
	.bottom_g27828_A(n_176),
	.bottom_g27922_A(n_148),
	.bottom_g27883_A1(n_143),
	.bottom_g27840_A1(n_177),
	.bottom_g27901_A(n_141),
	.bottom_g27991_B1(n_33),
	.bottom_g27887_B(n_158),
	.bottom_g27872_B1(n_3),
	.bottom_g27876_A2(n_149),
	.bottom_g27864_B1(n_132),
	.bottom_G290(G290),
	.bottom_G160(G160),
	.bottom_G164(G164),
	.bottom_g27874_A1(n_125),
	.bottom_g27873_A1(n_100),
	.bottom_g27968_A1(n_105),
	.bottom_g27886_A(n_90),
	.bottom_G166(G166),
	.bottom_g27917_A1(n_74),
	.bottom_g27909_A1(n_82),
	.bottom_g27909_A2(n_83),
	.bottom_g27911_A1(n_75),
	.bottom_g27914_A2(n_72),
	.bottom_g27926_A1(n_68),
	.bottom_g27930_B(n_46),
	.bottom_g27930_C1(n_30),
	.bottom_g27930_C2(n_7),
	.bottom_g27898_A(n_36),
	.bottom_g27898_B(n_29),
	.bottom_g27937_B(n_40),
	.bottom_g27955_A(n_69),
	.bottom_g27956_A(n_65),
	.bottom_g28008_A(n_84),
	.bottom_g27941_A1(n_60),
	.bottom_g27971_A1(n_56),
	.bottom_g27971_B1(n_55),
	.bottom_g28000_A(n_18),
	.bottom_G223(G223),
	.bottom_g28043_A1(n_115),
	.bottom_g28044_A1(n_110),
	.G220(G220),
	.G235(G235),
	.G236(G236),
	.G237(G237),
	.G238(G238),
	.G173(G173),
	.G325(G325),
	.G261(G261),
	.G162(G162),
	.G168(G168),
	.G171(G171),
	.G176(G176),
	.G301(G301),
	.G286(G286),
	.G303(G303),
	.G288(G288),
	.G305(G305),
	.G321(G284),
	.G280(G297),
	.G148(G148),
	.G323(G282),
	.G227(G227),
	.G229(G229),
	.G311(G311),
	.G150(G150),
	.G395(G395),
	.G397(G397),
	.G308(G308),
	.bottom_g27780_ZN(n_263),
	.bottom_g27778_ZN(n_259),
	.bottom_g27781_ZN(n_250),
	.bottom_g27786_ZN(n_246),
	.bottom_g27805_ZN(n_245),
	.bottom_g27790_ZN(n_242),
	.bottom_g27794_ZN(n_239),
	.bottom_g27803_ZN(n_232),
	.bottom_g27791_ZN(n_227),
	.bottom_g27801_ZN(n_220),
	.bottom_g27798_ZN(n_211),
	.bottom_g27837_Z(n_193),
	.bottom_g27806_ZN(n_190),
	.bottom_g27840_ZN(n_167),
	.bottom_g27866_ZN(n_216),
	.bottom_g27872_ZN(n_191),
	.bottom_g27876_ZN(n_166),
	.bottom_g27883_ZN(n_144),
	.bottom_g27842_ZN(n_187),
	.bottom_g27854_ZN(n_134),
	.bottom_g27859_ZN(n_131),
	.bottom_g27861_ZN(n_130),
	.bottom_g27862_ZN(n_128),
	.bottom_g27867_ZN(n_186),
	.bottom_g27869_ZN(n_126),
	.bottom_g27874_ZN(n_217),
	.bottom_g27906_ZN(n_188),
	.bottom_g27864_ZN(n_112),
	.bottom_g27873_ZN(n_109),
	.bottom_g27886_ZN(n_152),
	.bottom_g27922_ZN(n_150),
	.bottom_g27911_ZN(n_123),
	.bottom_g27914_ZN(n_127),
	.bottom_g27898_ZN(n_96),
	.bottom_g27937_ZN(n_98),
	.bottom_g27973_ZN(n_92),
	.bottom_g27936_ZN(n_151),
	.bottom_g27953_ZN(n_77),
	.bottom_g27968_ZN(n_67),
	.bottom_g27942_ZN(n_58),
	.bottom_g27948_ZN(n_54),
	.bottom_g27949_ZN(n_53),
	.bottom_g27952_ZN(n_51),
	.bottom_g27980_ZN(n_44),
	.bottom_g27982_ZN(n_41),
	.bottom_g27992_ZN(n_32),
	.bottom_g27990_ZN(n_27),
	.bottom_g27996_ZN(n_37),
	.bottom_g27997_Z(n_22),
	.bottom_g28005_ZN(n_85),
	.bottom_g28007_ZN(n_61),
	.bottom_g27984_ZN(n_59),
	.bottom_g28014_ZN(n_80),
	.bottom_g28006_ZN(n_81),
	.bottom_g28004_ZN(n_106),
	.bottom_g28037_ZN(n_39),
	.bottom_g28033_ZN(n_89),
	.bottom_g28020_ZN(n_183),
	.bottom_g28034_ZN(n_35),
	.bottom_g28040_ZN(n_222),
	.bottom_g28021_ZN(n_185),
	.bottom_g28028_ZN(n_23),
	.bottom_g28015_ZN(n_172),
	.bottom_g28032_ZN(n_251),
	.bottom_g28030_ZN(n_43));
endmodule

