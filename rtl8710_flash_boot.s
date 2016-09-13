#define Seg1LoadAddr 0x10000BC8

	.syntax unified
	.global	cortex_vectors

start:
cortex_vectors:
StartFlashRecord:
	.org  0x00000000
SpicCalibrationPattern:	
	.word 0x96969999
	.word 0xFC66CC3F
	.word 0x03CC33C0
	.word 0x6231DCE5
SegLoadSize:	
	.word end - load_address
SegLoadAddr:	
	.word Seg1LoadAddr                            
NextImageSeg:	
	.word 0xFFFF0000
	.word 0xFFFFFFFF
load_address:                                          
	.word Seg1LoadAddr + (Init - load_address) + 1
	.word Seg1StartAddr
	.word Seg1StartAddr
	.word Seg1StartAddr
	.word Seg1StartAddr
ImageValidPattern:	
	.word 0x88167923 // if != -> Image1 Validation Incorrect !!! Please Re-boot and try again, or re-burn the flash image
	.word 0xFFFFFFFF
	.word 0xFFFFFFFF
EndFlashRecord:
Seg1Start:

Init:
	cpsid f
	mov r2, #Seg1StartAddr
	ldr r3, [r2, #0]
	mov sp, r3
	ldr r3, [r2, #4]
	bx r3

	.org (load_address - StartFlashRecord) + (Seg1StartAddr - Seg1LoadAddr)
address:
	.incbin BinFileName
end:
