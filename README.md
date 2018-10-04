# Word-Clock

Project Description:
  Laser cutting plans and arduino code for a clock displayed by plain english.
  Check pictures folder for pictures of finished clock in order to better visualize.
  
Execution:
  The code (in WordClockArduino folder) was programmed in C on an Sparkfun Redboard. I used a library for the RTC.
  Pieces of the clock were laser cut, designed solely in Inkscape.
  Electronics of the clock include the RTC circuit, the Redboard, a series of shift
  registers and their respective LED drivers. The Redboard shifts in the appropriate signal into the
  shift registers. The parallel output of the regs is wired to LED drivers that sink the current.
  Each word is wired with a common source of 5-13V, the same input as the Redboard.
