interrupt_stub:
push rax
push rbx
push rcx
push rdx
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15

call interruptDispatch

pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rdx
pop rcx
pop rbx
pop rax

; remove the vector number + error code
add qword rsp, 16

iret

ALIGN 16
vector_0_handler:
push qword 0
push qword 0
jmp interrupt_stub

ALIGN 16
vector_1_handler:
push qword 0
push qword 1
jmp interrupt_stub

ALIGN 16
vector_2_handler:
push qword 0
push qword 2
jmp interrupt_stub

ALIGN 16
vector_3_handler:
push qword 0
push qword 3
jmp interrupt_stub

ALIGN 16
vector_4_handler:
push qword 0
push qword 4
jmp interrupt_stub

ALIGN 16
vector_5_handler:
push qword 0
push qword 5
jmp interrupt_stub

ALIGN 16
vector_6_handler:
push qword 0
push qword 6
jmp interrupt_stub

ALIGN 16
vector_7_handler:
push qword 0
push qword 7
jmp interrupt_stub

ALIGN 16
vector_8_handler:
push qword 8
jmp interrupt_stub

ALIGN 16
vector_9_handler:
push qword 0
push qword 9
jmp interrupt_stub

ALIGN 16
vector_10_handler:
push qword 10
jmp interrupt_stub

ALIGN 16
vector_11_handler:
push qword 11
jmp interrupt_stub

ALIGN 16
vector_12_handler:
push qword 12
jmp interrupt_stub

ALIGN 16
vector_13_handler:
push qword 13
jmp interrupt_stub

ALIGN 16
vector_14_handler:
push qword 14
jmp interrupt_stub

ALIGN 16
vector_15_handler:
push qword 0
push qword 15
jmp interrupt_stub

ALIGN 16
vector_16_handler:
push qword 0
push qword 16
jmp interrupt_stub

ALIGN 16
vector_17_handler:
push qword 17
jmp interrupt_stub

ALIGN 16
vector_18_handler:
push qword 0
push qword 18
jmp interrupt_stub

ALIGN 16
vector_19_handler:
push qword 0
push qword 19
jmp interrupt_stub

ALIGN 16
vector_20_handler:
push qword 0
push qword 20
jmp interrupt_stub

ALIGN 16
vector_21_handler:
push qword 0
push qword 21
jmp interrupt_stub

ALIGN 16
vector_22_handler:
push qword 0
push qword 22
jmp interrupt_stub

ALIGN 16
vector_23_handler:
push qword 0
push qword 23
jmp interrupt_stub

ALIGN 16
vector_24_handler:
push qword 0
push qword 24
jmp interrupt_stub

ALIGN 16
vector_25_handler:
push qword 0
push qword 25
jmp interrupt_stub

ALIGN 16
vector_26_handler:
push qword 0
push qword 26
jmp interrupt_stub

ALIGN 16
vector_27_handler:
push qword 0
push qword 27
jmp interrupt_stub

ALIGN 16
vector_28_handler:
push qword 0
push qword 28
jmp interrupt_stub

ALIGN 16
vector_29_handler:
push qword 0
push qword 29
jmp interrupt_stub

ALIGN 16
vector_30_handler:
push qword 0
push qword 30
jmp interrupt_stub

ALIGN 16
vector_31_handler:
push qword 0
push qword 31
jmp interrupt_stub

ALIGN 16
vector_32_handler:
push qword 0
push qword 32
jmp interrupt_stub

ALIGN 16
vector_33_handler:
push qword 0
push qword 33
jmp interrupt_stub

ALIGN 16
vector_34_handler:
push qword 0
push qword 34
jmp interrupt_stub

ALIGN 16
vector_35_handler:
push qword 0
push qword 35
jmp interrupt_stub

ALIGN 16
vector_36_handler:
push qword 0
push qword 36
jmp interrupt_stub

ALIGN 16
vector_37_handler:
push qword 0
push qword 37
jmp interrupt_stub

ALIGN 16
vector_38_handler:
push qword 0
push qword 38
jmp interrupt_stub

ALIGN 16
vector_39_handler:
push qword 0
push qword 39
jmp interrupt_stub

ALIGN 16
vector_40_handler:
push qword 0
push qword 40
jmp interrupt_stub

ALIGN 16
vector_41_handler:
push qword 0
push qword 41
jmp interrupt_stub

ALIGN 16
vector_42_handler:
push qword 0
push qword 42
jmp interrupt_stub

ALIGN 16
vector_43_handler:
push qword 0
push qword 43
jmp interrupt_stub

ALIGN 16
vector_44_handler:
push qword 0
push qword 44
jmp interrupt_stub

ALIGN 16
vector_45_handler:
push qword 0
push qword 45
jmp interrupt_stub

ALIGN 16
vector_46_handler:
push qword 0
push qword 46
jmp interrupt_stub

ALIGN 16
vector_47_handler:
push qword 0
push qword 47
jmp interrupt_stub

ALIGN 16
vector_48_handler:
push qword 0
push qword 48
jmp interrupt_stub

ALIGN 16
vector_49_handler:
push qword 0
push qword 49
jmp interrupt_stub

ALIGN 16
vector_50_handler:
push qword 0
push qword 50
jmp interrupt_stub

ALIGN 16
vector_51_handler:
push qword 0
push qword 51
jmp interrupt_stub

ALIGN 16
vector_52_handler:
push qword 0
push qword 52
jmp interrupt_stub

ALIGN 16
vector_53_handler:
push qword 0
push qword 53
jmp interrupt_stub

ALIGN 16
vector_54_handler:
push qword 0
push qword 54
jmp interrupt_stub

ALIGN 16
vector_55_handler:
push qword 0
push qword 55
jmp interrupt_stub

ALIGN 16
vector_56_handler:
push qword 0
push qword 56
jmp interrupt_stub

ALIGN 16
vector_57_handler:
push qword 0
push qword 57
jmp interrupt_stub

ALIGN 16
vector_58_handler:
push qword 0
push qword 58
jmp interrupt_stub

ALIGN 16
vector_59_handler:
push qword 0
push qword 59
jmp interrupt_stub

ALIGN 16
vector_60_handler:
push qword 0
push qword 60
jmp interrupt_stub

ALIGN 16
vector_61_handler:
push qword 0
push qword 61
jmp interrupt_stub

ALIGN 16
vector_62_handler:
push qword 0
push qword 62
jmp interrupt_stub

ALIGN 16
vector_63_handler:
push qword 0
push qword 63
jmp interrupt_stub

ALIGN 16
vector_64_handler:
push qword 0
push qword 64
jmp interrupt_stub

ALIGN 16
vector_65_handler:
push qword 0
push qword 65
jmp interrupt_stub

ALIGN 16
vector_66_handler:
push qword 0
push qword 66
jmp interrupt_stub

ALIGN 16
vector_67_handler:
push qword 0
push qword 67
jmp interrupt_stub

ALIGN 16
vector_68_handler:
push qword 0
push qword 68
jmp interrupt_stub

ALIGN 16
vector_69_handler:
push qword 0
push qword 69
jmp interrupt_stub

ALIGN 16
vector_70_handler:
push qword 0
push qword 70
jmp interrupt_stub

ALIGN 16
vector_71_handler:
push qword 0
push qword 71
jmp interrupt_stub

ALIGN 16
vector_72_handler:
push qword 0
push qword 72
jmp interrupt_stub

ALIGN 16
vector_73_handler:
push qword 0
push qword 73
jmp interrupt_stub

ALIGN 16
vector_74_handler:
push qword 0
push qword 74
jmp interrupt_stub

ALIGN 16
vector_75_handler:
push qword 0
push qword 75
jmp interrupt_stub

ALIGN 16
vector_76_handler:
push qword 0
push qword 76
jmp interrupt_stub

ALIGN 16
vector_77_handler:
push qword 0
push qword 77
jmp interrupt_stub

ALIGN 16
vector_78_handler:
push qword 0
push qword 78
jmp interrupt_stub

ALIGN 16
vector_79_handler:
push qword 0
push qword 79
jmp interrupt_stub

ALIGN 16
vector_80_handler:
push qword 0
push qword 80
jmp interrupt_stub

ALIGN 16
vector_81_handler:
push qword 0
push qword 81
jmp interrupt_stub

ALIGN 16
vector_82_handler:
push qword 0
push qword 82
jmp interrupt_stub

ALIGN 16
vector_83_handler:
push qword 0
push qword 83
jmp interrupt_stub

ALIGN 16
vector_84_handler:
push qword 0
push qword 84
jmp interrupt_stub

ALIGN 16
vector_85_handler:
push qword 0
push qword 85
jmp interrupt_stub

ALIGN 16
vector_86_handler:
push qword 0
push qword 86
jmp interrupt_stub

ALIGN 16
vector_87_handler:
push qword 0
push qword 87
jmp interrupt_stub

ALIGN 16
vector_88_handler:
push qword 0
push qword 88
jmp interrupt_stub

ALIGN 16
vector_89_handler:
push qword 0
push qword 89
jmp interrupt_stub

ALIGN 16
vector_90_handler:
push qword 0
push qword 90
jmp interrupt_stub

ALIGN 16
vector_91_handler:
push qword 0
push qword 91
jmp interrupt_stub

ALIGN 16
vector_92_handler:
push qword 0
push qword 92
jmp interrupt_stub

ALIGN 16
vector_93_handler:
push qword 0
push qword 93
jmp interrupt_stub

ALIGN 16
vector_94_handler:
push qword 0
push qword 94
jmp interrupt_stub

ALIGN 16
vector_95_handler:
push qword 0
push qword 95
jmp interrupt_stub

ALIGN 16
vector_96_handler:
push qword 0
push qword 96
jmp interrupt_stub

ALIGN 16
vector_97_handler:
push qword 0
push qword 97
jmp interrupt_stub

ALIGN 16
vector_98_handler:
push qword 0
push qword 98
jmp interrupt_stub

ALIGN 16
vector_99_handler:
push qword 0
push qword 99
jmp interrupt_stub

ALIGN 16
vector_100_handler:
push qword 0
push qword 100
jmp interrupt_stub

ALIGN 16
vector_101_handler:
push qword 0
push qword 101
jmp interrupt_stub

ALIGN 16
vector_102_handler:
push qword 0
push qword 102
jmp interrupt_stub

ALIGN 16
vector_103_handler:
push qword 0
push qword 103
jmp interrupt_stub

ALIGN 16
vector_104_handler:
push qword 0
push qword 104
jmp interrupt_stub

ALIGN 16
vector_105_handler:
push qword 0
push qword 105
jmp interrupt_stub

ALIGN 16
vector_106_handler:
push qword 0
push qword 106
jmp interrupt_stub

ALIGN 16
vector_107_handler:
push qword 0
push qword 107
jmp interrupt_stub

ALIGN 16
vector_108_handler:
push qword 0
push qword 108
jmp interrupt_stub

ALIGN 16
vector_109_handler:
push qword 0
push qword 109
jmp interrupt_stub

ALIGN 16
vector_110_handler:
push qword 0
push qword 110
jmp interrupt_stub

ALIGN 16
vector_111_handler:
push qword 0
push qword 111
jmp interrupt_stub

ALIGN 16
vector_112_handler:
push qword 0
push qword 112
jmp interrupt_stub

ALIGN 16
vector_113_handler:
push qword 0
push qword 113
jmp interrupt_stub

ALIGN 16
vector_114_handler:
push qword 0
push qword 114
jmp interrupt_stub

ALIGN 16
vector_115_handler:
push qword 0
push qword 115
jmp interrupt_stub

ALIGN 16
vector_116_handler:
push qword 0
push qword 116
jmp interrupt_stub

ALIGN 16
vector_117_handler:
push qword 0
push qword 117
jmp interrupt_stub

ALIGN 16
vector_118_handler:
push qword 0
push qword 118
jmp interrupt_stub

ALIGN 16
vector_119_handler:
push qword 0
push qword 119
jmp interrupt_stub

ALIGN 16
vector_120_handler:
push qword 0
push qword 120
jmp interrupt_stub

ALIGN 16
vector_121_handler:
push qword 0
push qword 121
jmp interrupt_stub

ALIGN 16
vector_122_handler:
push qword 0
push qword 122
jmp interrupt_stub

ALIGN 16
vector_123_handler:
push qword 0
push qword 123
jmp interrupt_stub

ALIGN 16
vector_124_handler:
push qword 0
push qword 124
jmp interrupt_stub

ALIGN 16
vector_125_handler:
push qword 0
push qword 125
jmp interrupt_stub

ALIGN 16
vector_126_handler:
push qword 0
push qword 126
jmp interrupt_stub

ALIGN 16
vector_127_handler:
push qword 0
push qword 127
jmp interrupt_stub

ALIGN 16
vector_128_handler:
push qword 0
push qword 128
jmp interrupt_stub

ALIGN 16
vector_129_handler:
push qword 0
push qword 129
jmp interrupt_stub

ALIGN 16
vector_130_handler:
push qword 0
push qword 130
jmp interrupt_stub

ALIGN 16
vector_131_handler:
push qword 0
push qword 131
jmp interrupt_stub

ALIGN 16
vector_132_handler:
push qword 0
push qword 132
jmp interrupt_stub

ALIGN 16
vector_133_handler:
push qword 0
push qword 133
jmp interrupt_stub

ALIGN 16
vector_134_handler:
push qword 0
push qword 134
jmp interrupt_stub

ALIGN 16
vector_135_handler:
push qword 0
push qword 135
jmp interrupt_stub

ALIGN 16
vector_136_handler:
push qword 0
push qword 136
jmp interrupt_stub

ALIGN 16
vector_137_handler:
push qword 0
push qword 137
jmp interrupt_stub

ALIGN 16
vector_138_handler:
push qword 0
push qword 138
jmp interrupt_stub

ALIGN 16
vector_139_handler:
push qword 0
push qword 139
jmp interrupt_stub

ALIGN 16
vector_140_handler:
push qword 0
push qword 140
jmp interrupt_stub

ALIGN 16
vector_141_handler:
push qword 0
push qword 141
jmp interrupt_stub

ALIGN 16
vector_142_handler:
push qword 0
push qword 142
jmp interrupt_stub

ALIGN 16
vector_143_handler:
push qword 0
push qword 143
jmp interrupt_stub

ALIGN 16
vector_144_handler:
push qword 0
push qword 144
jmp interrupt_stub

ALIGN 16
vector_145_handler:
push qword 0
push qword 145
jmp interrupt_stub

ALIGN 16
vector_146_handler:
push qword 0
push qword 146
jmp interrupt_stub

ALIGN 16
vector_147_handler:
push qword 0
push qword 147
jmp interrupt_stub

ALIGN 16
vector_148_handler:
push qword 0
push qword 148
jmp interrupt_stub

ALIGN 16
vector_149_handler:
push qword 0
push qword 149
jmp interrupt_stub

ALIGN 16
vector_150_handler:
push qword 0
push qword 150
jmp interrupt_stub

ALIGN 16
vector_151_handler:
push qword 0
push qword 151
jmp interrupt_stub

ALIGN 16
vector_152_handler:
push qword 0
push qword 152
jmp interrupt_stub

ALIGN 16
vector_153_handler:
push qword 0
push qword 153
jmp interrupt_stub

ALIGN 16
vector_154_handler:
push qword 0
push qword 154
jmp interrupt_stub

ALIGN 16
vector_155_handler:
push qword 0
push qword 155
jmp interrupt_stub

ALIGN 16
vector_156_handler:
push qword 0
push qword 156
jmp interrupt_stub

ALIGN 16
vector_157_handler:
push qword 0
push qword 157
jmp interrupt_stub

ALIGN 16
vector_158_handler:
push qword 0
push qword 158
jmp interrupt_stub

ALIGN 16
vector_159_handler:
push qword 0
push qword 159
jmp interrupt_stub

ALIGN 16
vector_160_handler:
push qword 0
push qword 160
jmp interrupt_stub

ALIGN 16
vector_161_handler:
push qword 0
push qword 161
jmp interrupt_stub

ALIGN 16
vector_162_handler:
push qword 0
push qword 162
jmp interrupt_stub

ALIGN 16
vector_163_handler:
push qword 0
push qword 163
jmp interrupt_stub

ALIGN 16
vector_164_handler:
push qword 0
push qword 164
jmp interrupt_stub

ALIGN 16
vector_165_handler:
push qword 0
push qword 165
jmp interrupt_stub

ALIGN 16
vector_166_handler:
push qword 0
push qword 166
jmp interrupt_stub

ALIGN 16
vector_167_handler:
push qword 0
push qword 167
jmp interrupt_stub

ALIGN 16
vector_168_handler:
push qword 0
push qword 168
jmp interrupt_stub

ALIGN 16
vector_169_handler:
push qword 0
push qword 169
jmp interrupt_stub

ALIGN 16
vector_170_handler:
push qword 0
push qword 170
jmp interrupt_stub

ALIGN 16
vector_171_handler:
push qword 0
push qword 171
jmp interrupt_stub

ALIGN 16
vector_172_handler:
push qword 0
push qword 172
jmp interrupt_stub

ALIGN 16
vector_173_handler:
push qword 0
push qword 173
jmp interrupt_stub

ALIGN 16
vector_174_handler:
push qword 0
push qword 174
jmp interrupt_stub

ALIGN 16
vector_175_handler:
push qword 0
push qword 175
jmp interrupt_stub

ALIGN 16
vector_176_handler:
push qword 0
push qword 176
jmp interrupt_stub

ALIGN 16
vector_177_handler:
push qword 0
push qword 177
jmp interrupt_stub

ALIGN 16
vector_178_handler:
push qword 0
push qword 178
jmp interrupt_stub

ALIGN 16
vector_179_handler:
push qword 0
push qword 179
jmp interrupt_stub

ALIGN 16
vector_180_handler:
push qword 0
push qword 180
jmp interrupt_stub

ALIGN 16
vector_181_handler:
push qword 0
push qword 181
jmp interrupt_stub

ALIGN 16
vector_182_handler:
push qword 0
push qword 182
jmp interrupt_stub

ALIGN 16
vector_183_handler:
push qword 0
push qword 183
jmp interrupt_stub

ALIGN 16
vector_184_handler:
push qword 0
push qword 184
jmp interrupt_stub

ALIGN 16
vector_185_handler:
push qword 0
push qword 185
jmp interrupt_stub

ALIGN 16
vector_186_handler:
push qword 0
push qword 186
jmp interrupt_stub

ALIGN 16
vector_187_handler:
push qword 0
push qword 187
jmp interrupt_stub

ALIGN 16
vector_188_handler:
push qword 0
push qword 188
jmp interrupt_stub

ALIGN 16
vector_189_handler:
push qword 0
push qword 189
jmp interrupt_stub

ALIGN 16
vector_190_handler:
push qword 0
push qword 190
jmp interrupt_stub

ALIGN 16
vector_191_handler:
push qword 0
push qword 191
jmp interrupt_stub

ALIGN 16
vector_192_handler:
push qword 0
push qword 192
jmp interrupt_stub

ALIGN 16
vector_193_handler:
push qword 0
push qword 193
jmp interrupt_stub

ALIGN 16
vector_194_handler:
push qword 0
push qword 194
jmp interrupt_stub

ALIGN 16
vector_195_handler:
push qword 0
push qword 195
jmp interrupt_stub

ALIGN 16
vector_196_handler:
push qword 0
push qword 196
jmp interrupt_stub

ALIGN 16
vector_197_handler:
push qword 0
push qword 197
jmp interrupt_stub

ALIGN 16
vector_198_handler:
push qword 0
push qword 198
jmp interrupt_stub

ALIGN 16
vector_199_handler:
push qword 0
push qword 199
jmp interrupt_stub

ALIGN 16
vector_200_handler:
push qword 0
push qword 200
jmp interrupt_stub

ALIGN 16
vector_201_handler:
push qword 0
push qword 201
jmp interrupt_stub

ALIGN 16
vector_202_handler:
push qword 0
push qword 202
jmp interrupt_stub

ALIGN 16
vector_203_handler:
push qword 0
push qword 203
jmp interrupt_stub

ALIGN 16
vector_204_handler:
push qword 0
push qword 204
jmp interrupt_stub

ALIGN 16
vector_205_handler:
push qword 0
push qword 205
jmp interrupt_stub

ALIGN 16
vector_206_handler:
push qword 0
push qword 206
jmp interrupt_stub

ALIGN 16
vector_207_handler:
push qword 0
push qword 207
jmp interrupt_stub

ALIGN 16
vector_208_handler:
push qword 0
push qword 208
jmp interrupt_stub

ALIGN 16
vector_209_handler:
push qword 0
push qword 209
jmp interrupt_stub

ALIGN 16
vector_210_handler:
push qword 0
push qword 210
jmp interrupt_stub

ALIGN 16
vector_211_handler:
push qword 0
push qword 211
jmp interrupt_stub

ALIGN 16
vector_212_handler:
push qword 0
push qword 212
jmp interrupt_stub

ALIGN 16
vector_213_handler:
push qword 0
push qword 213
jmp interrupt_stub

ALIGN 16
vector_214_handler:
push qword 0
push qword 214
jmp interrupt_stub

ALIGN 16
vector_215_handler:
push qword 0
push qword 215
jmp interrupt_stub

ALIGN 16
vector_216_handler:
push qword 0
push qword 216
jmp interrupt_stub

ALIGN 16
vector_217_handler:
push qword 0
push qword 217
jmp interrupt_stub

ALIGN 16
vector_218_handler:
push qword 0
push qword 218
jmp interrupt_stub

ALIGN 16
vector_219_handler:
push qword 0
push qword 219
jmp interrupt_stub

ALIGN 16
vector_220_handler:
push qword 0
push qword 220
jmp interrupt_stub

ALIGN 16
vector_221_handler:
push qword 0
push qword 221
jmp interrupt_stub

ALIGN 16
vector_222_handler:
push qword 0
push qword 222
jmp interrupt_stub

ALIGN 16
vector_223_handler:
push qword 0
push qword 223
jmp interrupt_stub

ALIGN 16
vector_224_handler:
push qword 0
push qword 224
jmp interrupt_stub

ALIGN 16
vector_225_handler:
push qword 0
push qword 225
jmp interrupt_stub

ALIGN 16
vector_226_handler:
push qword 0
push qword 226
jmp interrupt_stub

ALIGN 16
vector_227_handler:
push qword 0
push qword 227
jmp interrupt_stub

ALIGN 16
vector_228_handler:
push qword 0
push qword 228
jmp interrupt_stub

ALIGN 16
vector_229_handler:
push qword 0
push qword 229
jmp interrupt_stub

ALIGN 16
vector_230_handler:
push qword 0
push qword 230
jmp interrupt_stub

ALIGN 16
vector_231_handler:
push qword 0
push qword 231
jmp interrupt_stub

ALIGN 16
vector_232_handler:
push qword 0
push qword 232
jmp interrupt_stub

ALIGN 16
vector_233_handler:
push qword 0
push qword 233
jmp interrupt_stub

ALIGN 16
vector_234_handler:
push qword 0
push qword 234
jmp interrupt_stub

ALIGN 16
vector_235_handler:
push qword 0
push qword 235
jmp interrupt_stub

ALIGN 16
vector_236_handler:
push qword 0
push qword 236
jmp interrupt_stub

ALIGN 16
vector_237_handler:
push qword 0
push qword 237
jmp interrupt_stub

ALIGN 16
vector_238_handler:
push qword 0
push qword 238
jmp interrupt_stub

ALIGN 16
vector_239_handler:
push qword 0
push qword 239
jmp interrupt_stub

ALIGN 16
vector_240_handler:
push qword 0
push qword 240
jmp interrupt_stub

ALIGN 16
vector_241_handler:
push qword 0
push qword 241
jmp interrupt_stub

ALIGN 16
vector_242_handler:
push qword 0
push qword 242
jmp interrupt_stub

ALIGN 16
vector_243_handler:
push qword 0
push qword 243
jmp interrupt_stub

ALIGN 16
vector_244_handler:
push qword 0
push qword 244
jmp interrupt_stub

ALIGN 16
vector_245_handler:
push qword 0
push qword 245
jmp interrupt_stub

ALIGN 16
vector_246_handler:
push qword 0
push qword 246
jmp interrupt_stub

ALIGN 16
vector_247_handler:
push qword 0
push qword 247
jmp interrupt_stub

ALIGN 16
vector_248_handler:
push qword 0
push qword 248
jmp interrupt_stub

ALIGN 16
vector_249_handler:
push qword 0
push qword 249
jmp interrupt_stub

ALIGN 16
vector_250_handler:
push qword 0
push qword 250
jmp interrupt_stub

ALIGN 16
vector_251_handler:
push qword 0
push qword 251
jmp interrupt_stub

ALIGN 16
vector_252_handler:
push qword 0
push qword 252
jmp interrupt_stub

ALIGN 16
vector_253_handler:
push qword 0
push qword 253
jmp interrupt_stub

ALIGN 16
vector_254_handler:
push qword 0
push qword 254
jmp interrupt_stub

ALIGN 16
vector_255_handler:
push qword 0
push qword 255
jmp interrupt_stub

extern interruptDispatch
global vector_0_handler