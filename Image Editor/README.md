This is an image editor in C

Features: -load
          -save(bin/ascii)
          -select
          -crop
          -rotate
          -apply(sharpen/blur/gaussian_blur/edge)
          -equalize
          -histogram
          
Compatibility: -.pbm
               -.pgm
               -.ppm

How to use: -download/extract ZIP
            -run Makefile in terminal
            -run image_editor
            -use commands: LOAD 'filename'
                           SAVE 'filename' |ascii|
                           SELECT ALL/X1 Y1 X2 Y2
                           CROP
                           ROTATE 90/180/270/360 OR -90/-180/-270/-360
                           APPLY SHARPEN/BLUR/GAUSSIAN_BLUR/EDGE
                           EQUALIZE
                           HISTOGRAM X Y
                           EXIT
                           (if between: | | then that is an optional parameter)
                           
                        
