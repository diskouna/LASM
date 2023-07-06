              .orig  x3000
              lea    R0, String
              puts   ; Output a string to the screen 
              ld     R0, linefeed
              out    ; Output the linefeed 
                     ;       '\n' character
              halt   ; Halt the execution

String        .stringz  "This is a string !"
linefeed      .fill     #10 ; '\n'
              
              .end