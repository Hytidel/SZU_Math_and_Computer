	.ORIG x3000
LOOP1 LD R0 TIMES  ;R0=1
	JSR DISPLAY  ;print the board
REINPUT	JSR PROMPT  ;output prompt info
	JSR INPUT
	ADD R0,  R0,  #0
	BRZ REINPUT  ;invalid input
	JSR PLAY  ;begin the game
	ADD R0,  R0,  #0
	BRZ REINPUT
	JSR JUDGE_W  ;check whether someone wins
	ADD R0,  R0,  #0
	BRp OVER  ;game over
	BRnzp LOOP1  ;game continues
OVER JSR DISPLAY  ;print the board
	JSR ANSWER  ;show the result
	HALT  ;OHHHHHHH

;print the board
DISPLAY ST R1,  SAVE1  ;backup
	ST R2,  SAVE2
	ST R3,  SAVE3
	ST R6,  SAVE6
	
	LEA R6,  BOARD  ;calculate the offset
	LD R2,  NUM_ROW
COL_L	LD R1,  NUM_ROW
ROW_L	LDR R3,  R6,  #0  ;traverse the board
	BRnp TYPE_L1  ;get the type of the chess
	LD R0, CHESS_N  ;no chess
	BRnzp TYPR_LD
	
TYPE_L1	BRp TYPE_L2
	LD R0, CHESS_X  ;X chess
	BRnzp TYPR_LD
TYPE_L2	LD R0, CHESS_O  ;O chess
TYPR_LD	ST R7, SAVE7  ;backup
	TRAP X21  ;print the chess
	LD R7, SAVE7
	ADD R6, R6, #1  ;board address++
	ADD R1, R1, #-1
	BRNZ SPACE_L
	LD R0, SPACE	
	ST R7, SAVE7
	TRAP X21  ;print the space
	LD R7, SAVE7
	ADD R1, R1, #0
SPACE_L	BRp ROW_L
	LD R0, ENDL
	ST R7, SAVE7
	TRAP X21  ;print endl
	LD R7, SAVE7
	ADD R2, R2, #-1
	BRp COL_L
	LD R1, SAVE1	
	LD R2, SAVE2
	LD R3, SAVE3
	LD R6, SAVE6                                                                                                                                                                                                                         
	RET
	
;print answer
ANSWER ST R1 SAVE1  ;backup
	ST R2 SAVE2
	LD R1, TIMES
	LD R2, NUM_END  ;R2=-36
	ADD R2, R1, R2
	BRNZ ANS_L1
	LEA R0 TIE  ;tie game
	BRnzp ANS_LD
ANS_L1	LD R0, TIMES  ;backup
	ST R7 SAVE77
	JSR JUDGE_N
	ADD R0, R0, #0
	BRp ANS_L2
	LEA R0, WIN1
	BRnzp ANS_LD	
ANS_L2	LEA R0, WIN2
ANS_LD	LD R7, SAVE77  ;backup
	ST R7, SAVE7
	TRAP X22
	LD R7, SAVE7
	LD R1 SAVE1
	LD R2 SAVE2
	RET

;print prompt info
PROMPT ST R1, SAVE1  ;backup
	ST R2, SAVE2
	ST R0, SAVE0
	
	ADD R0, R0, #0
	BRp PRO_L3
	LEA R0 INVALID  ;invalid input
	ST R7 SAVE7
	TRAP X22
	LD R0 ENDL
	TRAP X21
	LD R7 SAVE7

PRO_L3	ST R0, SAVE0
	LD R0, TIMES
	ST R7, SAVE7
	JSR JUDGE_N
	ADD R0, R0, #0
	BRZ PRO_L1
	LEA R0, PROMPT1
	BRnzp PRO_L2	
PRO_L1	LEA R0, PROMPT2
PRO_L2	LD R7, SAVE7
	ST R7, SAVE7
	TRAP X22
	LD R7, SAVE7
	LD R1, SAVE1
	LD R2, SAVE2
	RET

;Labels
TIE	.STRINGZ "Tie Game."
INVALID	.STRINGZ "Invalid move. Try again. "
PROMPT1	.STRINGZ "Player 1,  choose a column: "
PROMPT2	.STRINGZ "Player 2,  choose a column: "
WIN1	.STRINGZ "Player 1 Wins."
WIN2	.STRINGZ "Player 2 Wins."

CHESS_O	.FILL	X004F
CHESS_X	.FILL	X0058
CHESS_N	.FILL	X002D

TIMES	.FILL	#1
BOARD	.BLKW 36 #0

SAVE0	.FILL 	0
SAVE1	.FILL 	0
SAVE2	.FILL 	0
SAVE3	.FILL 	0
SAVE4	.FILL 	0
SAVE5	.FILL 	0
SAVE6	.FILL 	0
SAVE7	.FILL 	0
SAVE77	.FILL	0

NUM_ROW	.FILL	#6
NUM_END	.FILL	#-36
NUM_CH	.FILL 	#-48
	
SPACE	.FILL	X0020
ENDL	.FILL	X000A

INPUT 	ST R1 SAVE1  ;backup
	ST R7 SAVE7
	TRAP X23
	LD R7 SAVE7	
	LD R1 NUM_CH  ;R1=-48
	ADD R0, R0, R1  ;char->int
	BRNZ INP_L1  ;check whether R0 > 0
	AND R1, R1, #0
	ADD R1, R1, #-6
	ADD R1, R0, R1
	BRp INP_L1  ;check whether R0 < 7
	BRnzp INP_L2
INP_L1	AND R0, R0, #0
INP_L2	LD R1 SAVE1
	RET

;start the game
PLAY ST R1, SAVE1  ;backup
	ST R2, SAVE2
	ST R3, SAVE3
	AND R3, R3, #0
	ADD R3, R3, #6  ;R3=6
	LEA R1, BOARD  ;calculate the offset
	ADD R1, R1, #+15  ;R1 points at the begin of 6th line
	ADD R1, R1, #+14
	ADD R1, R1, R0	
	LD R0 TIMES
PLA_L4	LDR R2, R1, #0
	BRZ PLA_L3
	ADD R3, R3, #-1
	BRNZ PLA_LD
	ADD R1, R1, #-6
	BRnzp PLA_L4
PLA_L3	ST R7 SAVE7
	ST R0 SAVE0
	JSR JUDGE_N
	LD R7, SAVE7
	ADD R0, R0, #0
	BRZ PLA_L2
	AND R2, R2, #0
	ADD R2, R2, #1
	STR R2, R1, #0
	BRnzp PLA_L1
PLA_L2	AND R2, R2, #0
	ADD R2, R2, #-1
	STR R2, R1, #0
PLA_L1	LD R1, TIMES
	ADD R1, R1, #1
	LEA R2, TIMES
	STR R1, R2, #0
	LD R0 SAVE0
	BRnzp PLA_L5
PLA_LD	LEA R0 INVALID
	AND R0, R0, #0	
PLA_L5	LD R1, SAVE1
	LD R2, SAVE2
	LD R3, SAVE3
	RET 	

;judge the number is whether odd or even
JUDGE_N	ST R1, SAVE1
	LD R1, TIMES
	AND R0, R0, #0
	ADD R0, R0, #1
	AND R0, R0, R1
	LD R1, SAVE1
	RET

;judge win
JUDGE_W	ST R1 SAVE1  ;backup
	ST R2 SAVE2
	ST R3 SAVE3
	ST R4 SAVE4
	ST R5 SAVE5
	ST R6 SAVE6
	
	;judge line
	LD R1 NUM_ROW  ;first layer loop
	AND R2, R2, #0	
	ADD R2, R2, #3  ;second layer loop
	LEA R6 BOARD  ;board address		
WIN_L1	AND R0, R0, #0
	ADD R0, R0, #1
	ST R7 SAVE7
	JSR SUM_N	
	LD R7 SAVE7
	ADD R0, R0, #0
	BRp WIN_LW
	ADD R6, R6, #1
	ADD R2, R2, #-1
	BRp WIN_L1
	ADD R6, R6, #-3
	ADD R6, R6, #6
	ADD R2, R2, #3
	ADD R1, R1, #-1
	BRp WIN_L1
	
	;judge column
	LD R1 NUM_ROW  ;first layer loop
	AND R2, R2, #0	
	ADD R2, R2, #3  ;second layer loop
	LEA R6 BOARD  ;board address		
WIN_L2	AND R0, R0, #0
	ADD R0, R0, #6
	ST R7 SAVE7
	JSR SUM_N
	LD R7 SAVE7
	ADD R0, R0, #0
	BRp WIN_LW
	ADD R6, R6, #6
	ADD R2, R2, #-1
	BRp WIN_L2
	ADD R6, R6, #-9
	ADD R6, R6, #-9
	ADD R6, R6, #1
	ADD R2, R2, #3
	ADD R1, R1, #-1
	BRp WIN_L2
	
	;judge down
	AND R1, R1, #0  ;first layer loop
	ADD R1, R1, #3
	LEA R6 BOARD  ;board address	
WIN_L4	AND R2, R2, #0	
	ADD R2, R2, R1  ;second layer loop
WIN_L3	AND R0, R0, #0
	ADD R0, R0, #7
	ST R7 SAVE7
	JSR SUM_N
	LD R7 SAVE7
	ADD R0, R0, #0
	BRp WIN_LW
	ADD R6, R6, #1
	ADD R2, R2, #-1
	BRp WIN_L3
	AND R3, R3, #0
	ADD R3, R3, R1
	NOT R3, R3
	ADD R3, R3, #1
	ADD R6, R6, R3
	ADD R6, R6, #6
	ADD R1, R1, #-1
	BRp WIN_L4


	;judge up
	AND R1, R1, #0	;first layer loop
	ADD R1, R1, #3
	LEA R6 BOARD
	ADD R6, R6, #5	;board address	
WIN_L6	AND R2, R2, #0	
	ADD R2, R2, R1	;second layer loop
WIN_L5	AND R0, R0, #0
	ADD R0, R0, #5
	ST R7 SAVE7
	JSR SUM_N
	LD R7 SAVE7
	ADD R0, R0, #0
	BRp WIN_LW
	ADD R6, R6, #-1
	ADD R2, R2, #-1
	BRp WIN_L5
	ADD R6, R6, R1
	ADD R6, R6, #6
	ADD R1, R1, #-1
	BRp WIN_L6
	AND R0, R0, #0
WIN_LW	LD R1, TIMES
	LD R2, NUM_END	;R2=-36
	ADD R2, R1, R2
	BRNZ WIN_LD
	ADD R0, R0, #1
WIN_LD	LD R1 SAVE1
	LD R2 SAVE2
	LD R3 SAVE3
	LD R4 SAVE4
	LD R5 SAVE5
	LD R6 SAVE6
	RET

;get sum
SUM_N ST R1 SAVE1  ;backup
	ST R2 SAVE2
	ST R3 SAVE3
	ST R6 SAVE6
	AND R3, R3, #0	;sum
	AND R2, R2, #0
	ADD R2, R2, #4
SUM_L1	LDR R1, R6, #0 	;R1=M[R6]
	ADD R3, R1, R3
	ADD R6, R6, R0  ;next row(R0=6) or next column(R0=1)
	ADD R2, R2, #-1
	BRp SUM_L1	
	AND R1, R1, #0
	ADD R1, R1, #4	
	AND R0, R0, #0	
	ADD R1, R1, R3
	BRnp SUM_L2
	ADD R0, R0, #1
	BRnzp SUM_LD	
SUM_L2	AND R1, R1, #0
	ADD R1, R1, #-4
	ADD R1, R1, R3
	BRnp SUM_LD
	ADD R0, R0, #1
	BRnzp SUM_LD	;if the total is 4 or -4 , return positive number(R0)
SUM_LD	LD R1 SAVE1	;else return zero(R0)
	LD R2 SAVE2
	LD R3 SAVE3
	LD R6 SAVE6
	RET

	.END