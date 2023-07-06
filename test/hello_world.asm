              .orig  x3000
              lea    R0, String
              puts   ; Output the String 
                     ; to the screen
              ld     R0, linefeed
              out    ; Output the linefeed 
                     ;       '\n' character
              halt   ; Halt the execution

String        .stringz  "Hello world !"
linefeed      .fill     #10 ; '\n'
              
              .end