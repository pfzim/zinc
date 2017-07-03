#ifndef _ZL_OPCODES_H_
#define _ZL_OPCODES_H_

#define ZL_STACK_SIZE				65536
#define ZL_INSTRUCTION_LENGTH		1

// instructions
#define OP_EOF				0x00

#define OP_PUSH_IMM			0x01
#define OP_PUSH_REG			0x02
#define OP_PUSH_MEM			0x03
#define OP_PUSH_OFFSET		0x91
#define OP_PUSH_PMEM		0x93

#define OP_POP_REG			0x05
#define OP_POP_MEM			0x06

#define OP_ADD_REG_IMM		0x07
#define OP_ADD_REG_REG		0x08
#define OP_ADD_REG_MEM		0x09
#define OP_ADD_MEM_IMM		0x0A
#define OP_ADD_MEM_REG		0x0B
#define OP_ADD_MEM_MEM		0x0C

#define OP_SUB_REG_IMM		0x0D
#define OP_SUB_REG_REG		0x0E
#define OP_SUB_REG_MEM		0x0F
#define OP_SUB_MEM_IMM		0x10
#define OP_SUB_MEM_REG		0x11
#define OP_SUB_MEM_MEM		0x12

#define OP_MOV_REG_IMM		0x13
#define OP_MOV_REG_REG		0x14
#define OP_MOV_REG_MEM		0x15
#define OP_MOV_MEM_IMM		0x16
#define OP_MOV_MEM_REG		0x17
#define OP_MOV_MEM_MEM		0x18
#define OP_MOV_PREG_REG		0x92
#define OP_MOV_REG_PREG		0x94

#define OP_CMP_REG_IMM		0x19
#define OP_CMP_REG_REG		0x1A
#define OP_CMP_REG_MEM		0x1B
#define OP_CMP_MEM_IMM		0x1C
#define OP_CMP_MEM_REG		0x1D
#define OP_CMP_MEM_MEM		0x1E

#define OP_JMP				0x1F
#define OP_JNE				0x20
#define OP_JNZ				0x21
#define OP_JE				0x22
#define OP_JZ				0x23
#define OP_JA				0x24
#define OP_JNBE				0x25
#define OP_JAE				0x26
#define OP_JNB				0x27
#define OP_JB				0x28
#define OP_JNAE				0x29
#define OP_JNA				0x2A
#define OP_JBE				0x2B
#define OP_JC				0x2C
#define OP_JNP				0x2D
#define OP_JPO				0x2E
#define OP_JP				0x2F
#define OP_JPE				0x30
#define OP_JECXZ			0x31

#define OP_JG				0x32
#define OP_JNLE				0x33
#define OP_JGE				0x34
#define OP_JNL				0x35
#define OP_JL				0x36
#define OP_JNGE				0x37
#define OP_JLE				0x38
#define OP_JNG				0x39
#define OP_JNO				0x3A
#define OP_JO				0x3B
#define OP_JNS				0x3C
#define OP_JS				0x3D

#define OP_RET				0x3E
#define OP_CALL				0x3F

#define OP_TEST_REG_REG		0x40

#define OP_G_REG_REG		0x41
#define OP_GE_REG_REG		0x42
#define OP_L_REG_REG		0x43
#define OP_LE_REG_REG		0x44
#define OP_E_REG_REG		0x45
#define OP_NE_REG_REG		0x46

#define OP_OR_REG_REG		0x49
#define OP_AND_REG_REG		0x4A
#define OP_XOR_REG_REG		0x4B

#define OP_LNOT_REG			0x4C
#define OP_BNOT_REG			0x4D

#define OP_MUL_REG_REG		0x4E
#define OP_MOD_REG_REG		0x4F
#define OP_DIV_REG_REG		0x50

#define OP_SHL_REG_REG		0x51
#define OP_SHR_REG_REG		0x52
#define OP_MINUS_REG		0x53

#define OP_INC_REG			0x54
#define OP_DEC_REG			0x55

#define OP_RCALL			0x56

#define OP_SIZE_OVERRIDE_1	0x66
#define OP_SIZE_OVERRIDE_2	0x67

#define OP_ADD_STK_STK		0xB0
#define OP_SUB_STK_STK		0xB1
#define OP_MUL_STK_STK		0xB2
#define OP_MOD_STK_STK		0xB3
#define OP_DIV_STK_STK		0xB4
#define OP_OR_STK_STK		0xB5
#define OP_XOR_STK_STK		0xB6
#define OP_AND_STK_STK		0xB7
#define OP_SHL_STK_STK		0xB8
#define OP_SHR_STK_STK		0xB9
#define OP_MINUS_STK		0xBA
#define OP_G_STK_STK		0xBB
#define OP_GE_STK_STK		0xBC
#define OP_L_STK_STK		0xBD
#define OP_LE_STK_STK		0xBE
#define OP_E_STK_STK		0xBF
#define OP_NE_STK_STK		0xC0
#define OP_LNOT_STK			0xC1
#define OP_BNOT_STK			0xC2
#define OP_JZ_STK			0xC3
#define OP_JNZ_STK			0xC4
#define OP_JZ_POP_STK		0xC5
#define OP_JNZ_POP_STK		0xC6

#define OP_NOP				0x90
#define OP_DBG_PRINT		0xFA


// registers
#define REG_COUNT			12

#define REG_EIP				0
#define REG_ESP				1
#define REG_EAX				2
#define REG_ECX				3
#define REG_EBX				4
#define REG_EDX				5
#define REG_EBP				6
#define REG_ESD				7
#define REG_EBD				8
#define REG_ESI				9
#define REG_EDI				10
#define ZL_EFLAGS			11

// flags
#define EF_CF				0x00000001		// 0
#define EF_PF				0x00000004		// 2
#define EF_ZF				0x00000040		// 6
#define EF_SF				0x00000080		// 7
#define EF_OF				0x00000800		// 11

// options
#define ZLF_CLEAR_STACK		0x00000001
#define ZLF_NEAR_CALL		0x00000002
#define ZLF_VOID			0x00000004
#define ZLF_CHAR			0x00000008
#define ZLF_DOUBLE			0x00000010
#define ZLF_FLOAT			0x00000020
#define ZLF_INT				0x00000040		// default
#define ZLF_SHORT			0x00000080		// default
#define ZLF_LONG			0x00000100
#define ZLF_SIGNED			0x00000200		// default
#define ZLF_UNSIGNED		0x00000400
#define ZLF_STRUCT			0x00000800
#define ZLF_INTERNAL		0x00001000		// default
#define ZLF_EXTERNAL		0x00002000
#define ZLF_ARRAY			0x00004000
#define ZLF_AUTOARRAY		0x00008000

#define ZLF_TYPE			0x0000007C		// mask
#define ZLF_POINTER			0xFF000000		// mask

// argument types
#define ARG_NONE			0x00000000
#define ARG_IMM				0x00000001
#define ARG_REG				0x00000002
#define ARG_PREG			0x00000004
#define ARG_MEM				0x00000008
#define ARG_PMEM			0x00000010
#define ARG_DATA			0x00000020

// operators groups
#define INS_PUSH			1
#define INS_POP				2
#define INS_ADD				3
#define INS_MOV				4
#define INS_JZ				5
#define INS_TEST			6
#define INS_CMP				7
#define INS_CALL			8
#define INS_JNZ				9
#define INS_JMP				10
#define INS_DBG_PRINT		11
#define INS_NOP				12
#define INS_SUB				13
#define INS_DIV				14
#define INS_MUL				15
#define INS_MOD				16
#define INS_RCALL			17
#define INS_OR				18
#define INS_AND				19
#define INS_XOR				20
#define INS_SHL				21
#define INS_SHR				22
#define INS_INC				22
#define INS_DEC				22

#define dw(x)				((unsigned long)(x))
#define pdw(x)				(*((unsigned long *)(x)))
#define ppdw(x)				(**((unsigned long **)(x)))

#define db(x)				((unsigned char)(x))
#define pdb(x)				(*((unsigned char *)(x)))
#define ppdb(x)				(**((unsigned char **)(x)))

#define w(x)				((unsigned short)(x))
#define pw(x)				(*((unsigned short *)(x)))
#define ppw(x)				(**((unsigned short **)(x)))

#define mem(x)				ppdw(dw(maptable) + dw(pdw(x)))
#define pmem(x)				pdw(dw(maptable) + dw(pdw(x)))

#define instr(x)			pdb(x)

#define nextinstr(x)		x += ZL_INSTRUCTION_LENGTH

#define push(x)				x += 4
#define pop(x)				x -= 4

#define sdw(x)				(unsigned char) (dw(x) & 0xFF), (unsigned char) (dw(x) >> 8 & 0xFF), (unsigned char) (dw(x) >> 16 & 0xFF), (unsigned char) (dw(x) >> 24)

/*
#define stepdb(x)			x += 1
#define backdb(x)			x -= 1
#define stepdw(x)			x += 4
#define backdw(x)			x -= 4
#define step2dw(x)			x += 8
#define back2dw(x)			x -= 8
*/

#endif // _ZL_OPCODES_H_
