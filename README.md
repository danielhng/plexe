### Cooperis Simulation Work

This repository proposes three simulation based on the plexe cooperis framemwork, integrating a RIS into different security relevant scenarios. Each Simulation adresses a different point of RIS security, with stress points on signal control (Side Lobe eavesdropping), Machine in the Middle attack, Redirection of signal and Lawful Interception.

Note

Each Simulation is not entirely finished and needs improvement for concrete usage and data gathering.

The simulation work depends on the plexe software ecosystem, by Michele Segat, as well as on the CoopeRIS framework to be able to integrate a RIS, you will need SUMO, OMNeT++, Veins, CoopeRIS and this fork of plexe. 
For OMNeT++, please install version 6.0.1, whereas for SUMO, verison 1.18.0.

You will also need to install the GNU scientific library. On a Ubuntu/Debian system, please install it with

sudo apt install libgsl-dev

while on macOS you can either use MacPorts

sudo port install gsl

or homebrew

brew install gsl

## Veins

CooperRIS works on a customized version of Veins. Clone it using this link and be sure to choose the cooperis branch:
```
git clone https://github.com/michele-segata/veins/tree/cooperis
cd veins
git checkout cooperis
```
Then simply compile Veins as usual:
```
./configure
make
```
## CoopeRIS (Multithread support, default)

CoopeRIS can be built with multithread or GPU support to accelerate the computation of the RIS. By default, CoopeRIS is built with multithread support. First, clone the repository:
```
git clone https://github.com/michele-segata/cooperis
cd cooperis
```
To build it, please configure it indicating the path to the GSL include and lib folders. You can do so in the following way:
```
./configure --with-gsl-include=/opt/local/include --with-gsl-lib=/opt/local/lib
```
Please make sure to change the GSL paths to match your owns.

Finally, simply type
```
make
```
Plexe

Evey simulation example is included within this branch of Plexe (starting from version 3.1.3). Clone it using this link and be sure to choose the plexe-cooperis-work branch:
```
git clone https://github.com/danielhng/plexe/
cd plexe
git checkout -b plexe-3.1.4-work plexe-3.1.4
```
Then simply compile Plexe as usual:
```
./configure
make
```
to build plexe.

To start each scenario, proceed as following: 
```
cd plexe/subprojects/plexe_cooperis
source setenv
./configure
make
```
Then run each example with:
```
cd examples/plexe_cooperis
plexe_cooperis_run -u Qtenv -c MaliciousRIS -r 0 
```
You can replace "MaliciousRIS" with every scenario defined in the omnetpp.ini file, i.e. 

# MaliciousRIS

For a redirection scenario from the RIS, the RIS will direct the beam towards the first node until SimTime = 3. It will afterwards be considered as compromised by an external actor and redirect the signal towards the other node.

# SideLobe

This scenario tries to implement an attacker situated on the SideLobe of the signal. In this setting the attacker located before the reception node is on the path of the RIS and should be able to intercept a certain amount of the signal

# Lawful Interception of traffic

The last scenario represents a RIS being used for lawful interception of traffic, redirecting everything to a passive listener which would be a dataserver or a proxy for logging.



## Maleability

Each scenario can be modified and adapted for further usage.

# Adding a protocol:

If you wish to emulate a different protocol than the EDHOC protocol in the scenarios, you can add a .cc and a .h file extending BaseProtocol in the src/protocol folder. You then need to create a new .ned file for that protocol and you can finally set it in the omnetpp.ini file as an argument, i.e. ```*.node[0].protocol_type = "YourProtocolName"```

# Adding a node:

Each node needs to be situated on a route in the sumo/veins framework. If you want to add a new route, you will need to do that through ```t-intersection.rou.xml```. You can then implement that route in the IntersectionTrafficManager.h and .cc to then add it as a new route in the .ini file. If you want to add a node on an existing or on a new route, you can add a car in the insertPlatoons function of IntersectionTrafficManager.

# Adding a RIS:

You can specify the amount of RIS you want in the TIntersection.ned file, if you want a dynamic amount of RIS depending on the simulation, you can use a variable. You then have to define the position, nodes to track and nodes to reflect from and to in the omnetpp.ini file. 

# Handling connections

To modify connections between nodes, you have to modify the MitMRisConnectionManager.cc file and the MitMConnectionManager.cc file, in these you can follow the templates of other connection handling in the isInRange function to define wich nodes and RIS should be connected together. 


## Credits

Plexe - The platooning extension for Veins

See the Plexe website <http://plexe.car2x.org/> for a tutorial, documentation,
and publications.

Plexe is composed of many parts. See the version control log for a full list of
contributors and modifications. Each part is protected by its own, individual
copyright(s), but can be redistributed and/or modified under an open source
license. License terms are available at the top of each file. Parts that do not
explicitly include license text shall be assumed to be governed by the "GNU
General Public License" as published by the Free Software Foundation -- either
version 2 of the License, or (at your option) any later version
(SPDX-License-Identifier: GPL-2.0-or-later). Parts that are not source code and
do not include license text shall be assumed to allow the Creative Commons
"Attribution-ShareAlike 4.0 International License" as an additional option
(SPDX-License-Identifier: GPL-2.0-or-later OR CC-BY-SA-4.0). Full license texts
are available with the source distribution.

Besides myself (Michele Segata), Plexe has been improved over the years thanks
to some incredibly valuable contributors:

Bastian Bloessl
Tobias Hardes
Julian Heinovski
Stefan Joerer
Max Schettler
Christoph Sommer
