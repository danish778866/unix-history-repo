!	RESTART COMMMAND FILE
SET DEF HEX
SET DEF LONG
SET REL:0
HALT			! HALT PROCESSOR
UNJAM			! UNJAM SBI
INIT			! INITIALIZE PROCESSOR
D PC C00
D PSL 0
C
