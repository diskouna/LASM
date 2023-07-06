            .orig X3000

rev         lea r0, FILE 
            add r1, r0, #-1
LOOP1       ldr r3, r1, #1
            brz done1
            add r1, r1, #1
            br LOOP1

done1       not r2, r0 
            add r2, r2, r1 

;; r0 == address of first character of string 
;; r1 == address of last character of string 
;; r2 == size of string - 2

LOOP2       add r2, r2, #0
            brn DONE2
            ldr r3, r0, #0
            ldr r4, r1, #0
            str r4, r0, #0
            str r3, r1, #0
            add r0, r0, #1
            add r1, r1, #-1
            add r2, r2, #-2
            br LOOP2

DONE2       HALT  

FILE        .stringz "This is so much fun!"
            .end