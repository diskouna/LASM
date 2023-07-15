              .orig  x3000
              lea    R0, String
              puts   
              ld     R0, linefeed
              out    
              halt   
String        .stringz  "Hello world !"
linefeed      .fill     #10 
              .end
