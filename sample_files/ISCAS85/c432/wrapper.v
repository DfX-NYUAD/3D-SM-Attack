/*
###############################################################
#  Generated by:      Cadence Innovus 17.11-s080_1
#  OS:                Linux x86_64(Host ID aduae260-lap)
#  Generated on:      Tue Sep 25 17:17:58 2018
#  Design:            c432
#  Command:           saveNetlist -module c432 outputs/wrapper_j.v
###############################################################
*/
module c432 (
	G1GAT, 
	G4GAT, 
	G8GAT, 
	G11GAT, 
	G14GAT, 
	G17GAT, 
	G21GAT, 
	G24GAT, 
	G27GAT, 
	G30GAT, 
	G34GAT, 
	G37GAT, 
	G40GAT, 
	G43GAT, 
	G47GAT, 
	G50GAT, 
	G53GAT, 
	G56GAT, 
	G60GAT, 
	G63GAT, 
	G66GAT, 
	G69GAT, 
	G73GAT, 
	G76GAT, 
	G79GAT, 
	G82GAT, 
	G86GAT, 
	G89GAT, 
	G92GAT, 
	G95GAT, 
	G99GAT, 
	G102GAT, 
	G105GAT, 
	G108GAT, 
	G112GAT, 
	G115GAT, 
	G223GAT, 
	G329GAT, 
	G370GAT, 
	G421GAT, 
	G430GAT, 
	G431GAT, 
	G432GAT);
   input G1GAT;
   input G4GAT;
   input G8GAT;
   input G11GAT;
   input G14GAT;
   input G17GAT;
   input G21GAT;
   input G24GAT;
   input G27GAT;
   input G30GAT;
   input G34GAT;
   input G37GAT;
   input G40GAT;
   input G43GAT;
   input G47GAT;
   input G50GAT;
   input G53GAT;
   input G56GAT;
   input G60GAT;
   input G63GAT;
   input G66GAT;
   input G69GAT;
   input G73GAT;
   input G76GAT;
   input G79GAT;
   input G82GAT;
   input G86GAT;
   input G89GAT;
   input G92GAT;
   input G95GAT;
   input G99GAT;
   input G102GAT;
   input G105GAT;
   input G108GAT;
   input G112GAT;
   input G115GAT;
   output G223GAT;
   output G329GAT;
   output G370GAT;
   output G421GAT;
   output G430GAT;
   output G431GAT;
   output G432GAT;

   // Internal wires
   wire n_0;
   wire n_1;
   wire n_4;
   wire n_6;
   wire n_7;
   wire n_14;
   wire n_16;
   wire n_18;
   wire n_19;
   wire n_21;
   wire n_22;
   wire n_24;
   wire n_26;
   wire n_28;
   wire n_29;
   wire n_30;
   wire n_33;
   wire n_34;
   wire n_35;
   wire n_36;
   wire n_37;
   wire n_38;
   wire n_40;
   wire n_42;
   wire n_43;
   wire n_44;
   wire n_45;
   wire n_46;
   wire n_47;
   wire n_48;
   wire n_50;
   wire n_51;
   wire n_52;
   wire n_54;
   wire n_55;
   wire n_56;
   wire n_57;
   wire n_63;
   wire n_64;
   wire n_66;
   wire n_67;
   wire n_68;
   wire n_69;
   wire n_70;
   wire n_72;
   wire n_73;
   wire n_75;
   wire n_77;
   wire n_78;
   wire n_80;
   wire n_81;
   wire n_82;
   wire n_83;
   wire n_84;
   wire n_85;
   wire n_89;
   wire n_91;
   wire n_96;
   wire n_97;
   wire n_98;
   wire n_99;
   wire n_100;
   wire n_103;
   wire n_105;
   wire n_108;
   wire n_110;
   wire n_111;
   wire n_113;
   wire n_114;
   wire n_116;
   wire n_117;
   wire n_157;

   bottom bottom (.G4GAT(G4GAT),
	.G8GAT(G8GAT),
	.G11GAT(G11GAT),
	.G14GAT(G14GAT),
	.G17GAT(G17GAT),
	.G21GAT(G21GAT),
	.G24GAT(G24GAT),
	.G27GAT(G27GAT),
	.G30GAT(G30GAT),
	.G37GAT(G37GAT),
	.G40GAT(G40GAT),
	.G43GAT(G43GAT),
	.G47GAT(G47GAT),
	.G50GAT(G50GAT),
	.G53GAT(G53GAT),
	.G56GAT(G56GAT),
	.G69GAT(G69GAT),
	.G73GAT(G73GAT),
	.G76GAT(G76GAT),
	.G79GAT(G79GAT),
	.G82GAT(G82GAT),
	.G86GAT(G86GAT),
	.G92GAT(G92GAT),
	.G95GAT(G95GAT),
	.G99GAT(G99GAT),
	.G102GAT(G102GAT),
	.G105GAT(G105GAT),
	.G108GAT(G108GAT),
	.G115GAT(G115GAT),
	.g37004_ZN(n_105),
	.g37017_ZN(n_111),
	.g37011_ZN(n_100),
	.g37018_ZN(n_99),
	.g37028_ZN(n_98),
	.g37060_ZN(n_51),
	.g37031_ZN(n_84),
	.g37027_ZN(n_89),
	.g37026_ZN(n_83),
	.g37040_ZN(n_63),
	.g37033_ZN(n_81),
	.g37041_ZN(n_70),
	.g37071_ZN(n_56),
	.g37051_ZN(n_50),
	.g37068_ZN(n_72),
	.g37044_ZN(n_77),
	.g37050_ZN(n_64),
	.g37080_ZN(n_48),
	.g37095_ZN(n_47),
	.g37053_ZN(n_54),
	.g37093_ZN(n_24),
	.g37078_ZN(n_35),
	.g37074_ZN(n_40),
	.g37075_ZN(n_37),
	.g37102_ZN(n_6),
	.g37099_ZN(n_1),
	.g37083_ZN(n_30),
	.g37085_ZN(n_29),
	.g37084_ZN(G223GAT),
	.g37090_ZN(n_22),
	.g37097_ZN(n_19),
	.g37113_ZN(n_16),
	.g37010_ZN(n_114),
	.g37013_ZN(n_113),
	.G329GAT(G329GAT),
	.G370GAT(G370GAT),
	.g36997_A(n_117),
	.g36995_B2(n_110),
	.g37001_A1(n_103),
	.g37000_A1(n_116),
	.g37002_A2(n_96),
	.g37013_A(n_97),
	.g37005_A2(n_108),
	.g37019_A(n_91),
	.g37029_A(n_85),
	.g37027_A1(n_82),
	.g37031_A2(n_78),
	.g37037_A(n_75),
	.g37033_B2(n_68),
	.g37041_A1(n_69),
	.g37033_A2(n_73),
	.g37033_C2(n_80),
	.g37031_B2(n_46),
	.g37041_A2(n_67),
	.g37012_A(n_52),
	.g37011_A(n_44),
	.g37028_A(n_43),
	.g37044_A1(n_57),
	.g37051_A1(n_45),
	.g37041_A3(n_66),
	.g37044_A3(n_55),
	.g37044_A4(n_34),
	.g37071_A1(n_42),
	.g37080_A(n_33),
	.g37065_A1(n_38),
	.g37068_A1(n_36),
	.g37086_A1(n_26),
	.g37084_B(n_21),
	.g37085_B1(n_28),
	.g37092_A1(n_14),
	.g37071_A2(n_7),
	.g37084_C1(n_18),
	.g37027_A3(n_4),
	.g37090_A1(n_0),
	.g36996_A1(n_157));
   top top (.G1GAT(G1GAT),
	.G4GAT(G4GAT),
	.G8GAT(G8GAT),
	.G14GAT(G14GAT),
	.G17GAT(G17GAT),
	.G24GAT(G24GAT),
	.G30GAT(G30GAT),
	.G34GAT(G34GAT),
	.G37GAT(G37GAT),
	.G40GAT(G40GAT),
	.G43GAT(G43GAT),
	.G47GAT(G47GAT),
	.G50GAT(G50GAT),
	.G53GAT(G53GAT),
	.G56GAT(G56GAT),
	.G60GAT(G60GAT),
	.G63GAT(G63GAT),
	.G66GAT(G66GAT),
	.G69GAT(G69GAT),
	.G73GAT(G73GAT),
	.G79GAT(G79GAT),
	.G82GAT(G82GAT),
	.G86GAT(G86GAT),
	.G89GAT(G89GAT),
	.G92GAT(G92GAT),
	.G95GAT(G95GAT),
	.G102GAT(G102GAT),
	.G105GAT(G105GAT),
	.G108GAT(G108GAT),
	.G112GAT(G112GAT),
	.bottom_g36995_B2(n_110),
	.bottom_g36996_A1(n_157),
	.bottom_g37005_A2(n_108),
	.bottom_g36997_A(n_117),
	.bottom_g37001_A1(n_103),
	.bottom_g37002_A2(n_96),
	.bottom_g37000_A1(n_116),
	.bottom_g37013_A(n_97),
	.bottom_G370GAT(G370GAT),
	.bottom_g37011_A(n_44),
	.bottom_G329GAT(G329GAT),
	.bottom_g37012_A(n_52),
	.bottom_g37019_A(n_91),
	.bottom_g37027_A1(n_82),
	.bottom_g37071_A1(n_42),
	.bottom_g37027_A3(n_4),
	.bottom_g37029_A(n_85),
	.bottom_g37031_A2(n_78),
	.bottom_g37031_B2(n_46),
	.bottom_g37044_A3(n_55),
	.bottom_g37028_A(n_43),
	.bottom_g37033_A2(n_73),
	.bottom_g37033_C2(n_80),
	.bottom_g37033_B2(n_68),
	.bottom_g37041_A1(n_69),
	.bottom_g37037_A(n_75),
	.bottom_g37041_A2(n_67),
	.bottom_g37041_A3(n_66),
	.bottom_g37044_A1(n_57),
	.bottom_g37044_A4(n_34),
	.bottom_g37051_A1(n_45),
	.bottom_g37065_A1(n_38),
	.bottom_g37068_A1(n_36),
	.bottom_g37071_A2(n_7),
	.bottom_g37080_A(n_33),
	.bottom_g37084_B(n_21),
	.bottom_g37084_C1(n_18),
	.bottom_g37085_B1(n_28),
	.bottom_g37086_A1(n_26),
	.bottom_g37090_A1(n_0),
	.bottom_g37092_A1(n_14),
	.G223GAT(G223GAT),
	.G421GAT(G421GAT),
	.G430GAT(G430GAT),
	.G431GAT(G431GAT),
	.G432GAT(G432GAT),
	.bottom_g37004_ZN(n_105),
	.bottom_g37013_ZN(n_113),
	.bottom_g37010_ZN(n_114),
	.bottom_g37011_ZN(n_100),
	.bottom_g37017_ZN(n_111),
	.bottom_g37018_ZN(n_99),
	.bottom_g37027_ZN(n_89),
	.bottom_g37031_ZN(n_84),
	.bottom_g37026_ZN(n_83),
	.bottom_g37028_ZN(n_98),
	.bottom_g37033_ZN(n_81),
	.bottom_g37041_ZN(n_70),
	.bottom_g37040_ZN(n_63),
	.bottom_g37044_ZN(n_77),
	.bottom_g37050_ZN(n_64),
	.bottom_g37051_ZN(n_50),
	.bottom_g37053_ZN(n_54),
	.bottom_g37060_ZN(n_51),
	.bottom_g37068_ZN(n_72),
	.bottom_g37071_ZN(n_56),
	.bottom_g37080_ZN(n_48),
	.bottom_g37074_ZN(n_40),
	.bottom_g37075_ZN(n_37),
	.bottom_g37078_ZN(n_35),
	.bottom_g37083_ZN(n_30),
	.bottom_g37085_ZN(n_29),
	.bottom_g37090_ZN(n_22),
	.bottom_g37095_ZN(n_47),
	.bottom_g37093_ZN(n_24),
	.bottom_g37113_ZN(n_16),
	.bottom_g37097_ZN(n_19),
	.bottom_g37102_ZN(n_6),
	.bottom_g37099_ZN(n_1));
endmodule

