Getting started

To start the simulation, make an instance of Cardatasimulator.
the hz parameter of the class needs to correspond with Cardatasimulator.update(). The example code uses a timer interrupt to achieve this.
The data of the simulator gets returned as a CarData struct, and can be retrieved by calling Cardatasimulator.GetData(), 
after which you can use the data.



TODO:
Warnings aren't implemented yet.
Warnings aren't defined yet in the design, and should be defined, and then added to the current program.
Some ideas could be to use the simulated data to trigger those warnings, such as going over the speed limit, running out of fuel, 
or the RPM getting too low to keep running.
