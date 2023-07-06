;; Counts the number of times a character occurs in a string
;; Character -- stored at x4000
;; String    -- stored at x5000
;; Result    -- stored at x6000
        .ORIG    x3000
        ADD      R0, R0, #-16
        TRAP     x21
nmChr   AND      R0,R0,#0
        LEA      R1, String           ;; R1 has address of the string
        LD       R2, Char             ;; R2 has the value of the string
        NOT      R2,R2
        ADD      R2,R2,#1
ALOOP   LDR      R3,R1,#0
        BRz      STOPIT             ;; Leave loop on zero word
        ADD      R3,R3,R2
        BRnp     NOCOUNT
        ADD      R0,R0,#1
NOCOUNT ADD      R1,R1,#1
        BR       ALOOP

STOPIT  TRAP x21
        HALT 

Char    .FILL    #65
String  .STRINGZ "AAAA"

        .END