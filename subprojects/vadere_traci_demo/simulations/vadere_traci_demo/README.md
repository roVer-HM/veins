# Vadere TraCI Demo

This project contains a small demonstration to connect OMNeT++ to Vadere.

## prerequisite 

* Java JDK 11
* Vadere Server: use tag ``vadere_traci_demo_001``

```
git clone --branch vadere_traci_demo_001 https://gitlab.lrz.de/vadere/vadere
cd vadere
mvn clean package -Dmaven.test.skip
java -jar VadereManager/target/vadere-server.jar
10:32:26,128   INFO Manager:30 - Start Server(VadereTraCI-20.0.2 This is a TraCI Server implementing only a small subset of TraCI Version 20) with Loglevel: INFO
10:32:26,133   INFO VadereServer:40 - listening on port 9999... (gui-mode: false)
...
```

* use ``java -jar VadereManager/target/vadere-server.jar -h`` for configuration help.
* INET: tested with Branch ``master`` ([8dd34240][1])
* setup veins and inet in OMNeT++ IDE

## run demo 

* start vadere server 
* start subproject  vadere_traci_demo
* choose config: *VadereTracIDemo_001*
* choose host and port from vadere server

[1]: https://github.com/inet-framework/inet/commit/8dd34240fb61b6ff931d417b9234e339f022e6c8

