;; Set R0 to the number of bits "on" in R1
        .ORIG   x3000
        LEA     R0, intro
        PUTS
        LD      R0, newline
        OUT
LOOP    LEA     R0, prompt
        PUTS
        IN
        LD      R2, mspace
        ADD     R2, R0, R2
        brz     end
        LD      R1, mascii
        ADD     R1, R0, R1
        LEA     R0, result
        PUTS
pop     AND     R0, R0, #0
        ADD     R1, R1, #0       ;; test the msb
        BRzp    skipf
        ADD     R0, R0, #1
skipf   AND     R2, R2, #0
        ADD     R2, R2, #15
loop    ADD     R1, R1, R1       ;; now test the other 15 : left shift 
        BRzp    skip
        ADD     R0, R0, #1
skip    ADD     R2, R2, #-1
        BRp     loop
        ; convert ascii value to its number equivalent 
        LD      R1, ascii
        ADD     R0, R0, R1
        OUT
        LD      R0, newline
        OUT
        BR      LOOP
end     LD      R0, newline
        OUT
        HALT
ascii   .FILL   x30    ; ascii value for character '0'  
mascii  .FILL   x-30   ; -'0'
newline .FILL   #10    ; '\n'
mspace  .FILL   #-32   ; -' '
intro   .STRINGZ " Program to count the number of set bits in a given digit. Enter ' '(space) to halt the program " 
prompt  .STRINGZ " --> Enter a digit : "
result  .stringz "  pop_count : "     
        .END
;; TODO :         
;; use divide and conquer method
;; use brian kerninghan