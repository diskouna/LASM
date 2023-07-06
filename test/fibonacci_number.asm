;; Ask user to enter a number and
;; Compute and display Fn, the  n-th Fibonacci number
;;
        .ORIG x3000
        LEA R0,PROMPT
        TRAP x22
        TRAP x23

        ADD R0, R0, #-16
        ADD R0, R0, #-16
        ADD R4, R0, #-16 ; counter
        AND R1, R1, #0
        ADD R1, R1, #1 ; F0 = 1
        ADD R2, R1, #0 ; F1 = 1

        ADD R4, R4, #0        
LOOP    BRz DISPLAY
        ADD R3, R1, R2 ; Fn = Fn-1 + Fn-2 
        ADD R1, R2, #0 ; Fn-1 --> Fn
        ADD R2, R3, #0 ; Fn-2 --> Fn   

        ADD R4, R4, #-1
        BRp LOOP   

DISPLAY LEA R0,RESULT
        TRAP x22
        ADD R0, R1, #16
        ADD R0, R0, #16
        ADD R0, R0, #16
        TRAP x21

        TRAP x25
PROMPT  .STRINGZ "Enter a number : "
RESULT  .STRINGZ "Fn : "
        .END

;; END OF SOURCE CODE