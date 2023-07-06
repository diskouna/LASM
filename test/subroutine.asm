            .ORIG x3000
mainloop    LD      R1, iascii  ; -'0'
            ADD     R2, R1, #0  ; -'0'
            LEA     R0, askx
            PUTS
            IN 
            LD      R3, linefeed
            NOT     R3, R3
            ADD     R3, R3, #1
            ADD     R3, R0, R3
            BRz     mainend
            ADD     R2, R2, R0
            LEA     R0, asky
            PUTS
            IN
            jsr     newline
            ADD     R1, R1, R0   ; y
            LEA     R0, multres
            PUTS
            ADD     R0, R2, #0   ; x   
            jsr     MULT
            ADD     R0, R2, #0   ; x*y
            jsr     printnumber
            jsr     newline   
            BR      mainloop 
mainend     HALT

;;      subroutine   
;;  Multiplicaion  
;;      x is stored in R0
;;      y is stored in R1
;; The result x*y is stored in R2        
MULT        STI   R3, saveR3
            AND   R3, R3, #0
            AND   R2, R2, #0
            ADD   R3, R3, #1
            ADD   R0, R0, #0 
            BRz   multend
            BRp   multnext
            NOT   R3, R3
            ADD   R3, R3, #1
            NOT   R0, R0
            ADD   R0, R0, #1
multnext    ADD   R1, R1, #0 
            BRz   multend
            BRp   multloop
            NOT   R3, R3
            ADD   R3, R3, #1   
            NOT   R1, R1
            ADD   R1, R1, #1         
multloop    ADD   R1, R1, #-1
            BRn   multend
            ADD   R2, R2, R0
            BR    multloop
multend     LDI R3, saveR3
            RET   

;; Integer division and modulo operations
;;      x is stored in R0
;;      y is stored in R1 
;;  Subroutine set R2 to 1 if the division is valid (x >=0 && y > 0)
;;                   and 0 otherwise    
;;  The result of integer division x/y is stored in R0 and 
;;  modulo' result               x % y is stored in R1               

DIV         STI     R3, saveR3
            STI     R4, saveR4 

            AND     R2, R2, #0  ; R2 = 0

            ADD     R0, R0, #0
            BRn     divend
            ADD     R1, R1, #0
            BRnz    divend  

            ADD     R2, R2, #1   ; R2 = 1 

            AND     R3, R3, #0   ; R3 = 0 
            ADD     R4, R0, #0   ; R4 = x 
            NOT     R1, R1
            ADD     R1, R1, #1   ; y = - y
divloop     ADD     R0, R0, R1   ; x = x - y
            BRn     divend       ; while (x >= y)
            ADD     R3, R3, #1   ;      R3 += 1
            ADD     R4, R0, #0   ;      R4 = x - y
            BR      divloop 

divend      ADD     R0, R3, #0
            ADD     R1, R4, #0
            LDI     R3, saveR3
            LDI     R4, saveR4          
            RET


newline     STI R0, saveR0
            LD  R0, linefeed
            OUT
            LDI R0, saveR0
            RET
            
askx        .STRINGZ " x = "
asky        .STRINGZ " y = "
multres     .STRINGZ " x * y = " 
valdivres   .STRINGZ " Is Valid = " 
divres      .STRINGZ " x / y = "
modres      .STRINGZ " x % y = "

printnumber STI     R0, saveR0
            STI     R1, saveR1
            STI     R2, saveR2
            STI     R3, saveR3
            STI     R7, saveR7  ; save caller RET 
            LD      R6, stackbase
            getloop     AND     R1, R1, #0
                        ADD     R1, R1, #10  ; 10    
                        jsr     DIV          
                        ADD     R2, R0, #0  ; x*y / 10
                        ADD     R0, R1, #0  ; x*y % 10
                        jsr     PUSH
                        ADD     R0, R2, #0  
                        BRp     getloop
            
                        LD     R3, ascii
            printloop   jsr    POP
                        ADD    R0, R0, R3
                        OUT
                        jsr    ISEMPTY       
                        BRz    printloop    ;  test R0
            LDI R0, saveR0
            LDI R1, saveR1
            LDI R2, saveR2
            LDI R3, saveR3
            LDI R7, saveR7    ; restore caller RET 
RET

;; STACK 

stackbase   .FILL   x4000
mstackbase  .FILL   x-4000      ; minus Base

PUSH        ADD     R6, R6, #-1
            STR     R0, R6, #0
            RET

POP         LDR     R0, R6, #0
            ADD     R6, R6, #1
            RET

;; set R0 to 1 when the stack is empty and to 0 otherwise

ISEMPTY     LD      R0, mstackbase
            ADD     R0, R0, R6
            BRz     IS 
            AND     R0, R0, #0
            RET
IS          AND     R0, R0, #0
            ADD     R0, R0, #1
            RET

saveR0      .FILL x4120
saveR1      .FILL x4121
saveR2      .FILL x4122
saveR3      .FILL x4123
saveR4      .FILL x4124
saveR5      .FILL x4125
saveR6      .FILL x4126
saveR7      .FILL x4127

ascii       .FILL x0030  ;  '0'
iascii      .FILL x-0030 ; -'0'
space       .FILL x0020  ;  ' '
linefeed    .FILL #10    ;  '\n' 

testprint   .FILL x7FFF
            .END
