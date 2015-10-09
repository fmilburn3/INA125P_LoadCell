# INA125P_LoadCell

Since it is getting closer to Halloween...
  
A sketch to report the candy in a candy bowl using a load cell, INA125P instrumentation amplifier, MSP430F5529 LaunchPad, and 430BOOST-SHARP96 BoosterPack.  
  
The load cell was taken from a defunct 1 Kg "postal" scale.
  
To use it, start with an empty bowl on the load cell.  The raw analog value will be shown along with some directions. Push button "PUSH1" when the raw value has settled. Then add 10 pieces of candy to the bowl (or modify below with the variable candyCount2 to your liking) and push button "PUSH2" to finish calibration when things have settled again.  The calibration data is then used with the map function to estimate how many pieces of candy there are. A rounding variable is used to address the "jitter" in the   analog readings and the rounding that results from integer math.
  
I made a simple BoosterPack that plugs in under the LaunchPad so the SHARP96 sits unobstructed on top.  See photographs and a circuit schematic at 43oh.
  
Frank Milburn October 2015
Released into the public domain
