;init
        .ORIG x3000
        AND R0, R0, #0   ;0
        AND R7, R7, #0   ;idx

;main
READ    LD R4, BEGIN     ;pointer
        AND R1, R1, #0   ;curmax
        AND R3, R3, #0   ;i
        ADD R3, R3, #10  
        ADD R3, R3, #6   ;16numbers
LOOP1   LDR R5, R4, #0   ;get the number
        NOT R5, R5
        ADD R5, R5, #1
        ADD R5, R5, R1
        BRp OK           ;R1>R5
        LDR R1, R4, #0   ;update curmax
OK      ADD R4, R4, #1
        ADD R3, R3, #-1
        BRp LOOP1

        LD R4, BEGIN
        AND R3, R3, #0   ;i
        ADD R3, R3, #10  
        ADD R3, R3, #6   ;16numbers
LOOP2   LDR R5, R4, #0   ;get the number
        NOT R5, R5
        ADD R5, R5, #1
        ADD R5, R5, R1
        BRz YES
        ADD R4, R4, #1
        ADD R3, R3, #-1
        BRp LOOP2

YES     LDR R5, R4, #0
        LD R6, SORTED
        ADD R6, R6, R7   ;destination
        STR R5, R6, #0   ;place curmax

        STR R0, R4, #0
        ADD R7, R7, #1

        ADD R6, R7, #-16
        BRz GET
        BRnzp READ

GET     AND R1, R1, #0   ;resA
        AND R2, R2, #0   ;resB
        LD R4, SORTED
        AND R3, R3, #0   ;i
        ADD R3, R3, #4   ;the front 25%
LOOP3   LD R6, EIGHT     ;85
        LDR R5, R4, #0   ;get the number
        NOT R5, R5
        ADD R5, R5, #1
        ADD R6, R5, R6
        BRp CHECKB       ;R5<85
        ADD R1, R1, #1   ;resA++
NOB     ADD R4, R4, #1
        ADD R3, R3, #-1  ;i--
        BRp LOOP3

        ;write resA
        LD R0, ASCII
        ADD R0, R0, R1   ;resA
        OUT
        LD R0, SPACE
        OUT

        ;getB
        LD R4, SORTED
        ADD R4, R4, #4   ;start at NO.5
        AND R3, R3, #0   ;i
        ADD R3, R3, #4   ;the front 25% ~ 50%
LOOP4   LD R6, SEVEN     ;75
        LDR R5, R4, #0   ;get the number
        NOT R5, R5
        ADD R5, R5, #1
        ADD R6, R5, R6
        BRp NOBB
        ADD R2, R2, #1   ;resB++
NOBB    ADD R4, R4, #1
        ADD R3, R3, #-1  ;i--
        BRp LOOP4

        ;write resB
        LD R0, ASCII
        ADD R0, R0, R2   ;resB
        OUT
        BRnzp DONE

CHECKB  LD R6, SEVEN    ;75
        ADD R6, R5, R6
        BRp NOB
        ADD R2, R2, #1  ;resB++
        BRnzp NOB

DONE    HALT

BEGIN   .FILL x3200
SORTED  .FILL x4000
EIGHT   .FILL #85
SEVEN   .FILL #75
ASCII   .FILL x0030
SPACE   .FILL #32

        .END