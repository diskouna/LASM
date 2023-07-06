        .ORIG x3000
START   LEA R0, PROMPT
        TRAP x22        
        TRAP x23        
        ADD R3, R0, #0
        ADD R3, R3, #-16   
        ADD R3, R3, #-16
        ADD R3, R3, #-16
        LEA R0, DAYS
        ADD R3, R3, #0
LOOP    BRz DISPLAY
        ADD R0, R0, #10
        ADD R3, R3, #-1
        BRnzp LOOP
DISPLAY TRAP x22 
        BR START
        TRAP x25
PROMPT  .STRINGZ " Please enter number :" 
DAYS    .STRINGZ "Sunday   " 
        .STRINGZ "MONDAY   "
        .STRINGZ "TUESDAY  "
        .STRINGZ "WEDNESDAY"
        .STRINGZ "THURSDAY "
        .STRINGZ "FRIDAY   "
        .STRINGZ "SATURDAY "
        .END 
