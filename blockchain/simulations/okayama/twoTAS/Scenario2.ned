//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package blockchain.simulations.okayama.twoTAS;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.node.ethernet.Eth10G;

import simu5g.world.radio.LteChannelControl;
import simu5g.common.carrierAggregation.CarrierAggregation;
import simu5g.nodes.Upf;
import simu5g.common.binder.Binder;
import simu5g.nodes.NR.gNodeB;
import simu5g.nodes.cars.NRCar;
import simu5g.nodes.NR.NRUe;

import org.car2x.veins.subprojects.veins_inet.VeinsInetManager;

network Scenario2
{
    parameters:
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=732,483");

    submodules:

        routingRecorder: RoutingTableRecorder {
            @display("p=50,75;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,125");
            config = xmldoc("demo.xml");
        }

        //# Veins manager module
        veinsManager: VeinsInetManager {
            @display("p=50,227;is=s");
        }

        //# LTE modules
        channelControl: LteChannelControl {
            @display("p=50,25;is=s");
        }
        binder: Binder {
            @display("p=50,175;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=50.993748,258.7;is=s");
        }
        TAS: StandardHost {
            @display("p=666,45;is=n;i=device/server");
        }
        router: Router {
            @display("p=495,86;i=device/smallrouter");
        }
        upf: Upf {
            @display("p=356,75");
        }
        rsu1: NRUe {
            @display("i=device/antennatower;p=176,219");
        }
        rsu2: NRUe {
            @display("i=device/antennatower;p=589,34");
        }
        //# submodule vector to store all cars - will be filled dynamically during simulations
        car[0]: NRCar {
            @display("p=419,34;is=vs");
        }

        blockchain: StandardHost {
            @display("p=684,175;i=block/routing;is=m");
        }

        //gNodeB3: gNodeB {
        //    @display("p=542,429;is=vl");
        // }
        rsu3: NRUe {
            @display("i=device/antennatower;p=286,125");
        }
        //gNodeB3: gNodeB {
        //    @display("p=542,429;is=vl");
        // }
        rsu4: NRUe {
            @display("i=device/antennatower;p=562,136");
        }
        upf1: Upf {
            @display("p=319,367");
        }
        router1: Router {
            @display("p=481,444;i=device/smallrouter");
        }
        TAS1: StandardHost {
            @display("p=650,444;is=n;i=device/server");
        }
        //gNodeB3: gNodeB {
        //    @display("p=542,429;is=vl");
        // }
        rsu5: NRUe {
            @display("i=device/antennatower;p=411,219");
        }
        //gNodeB3: gNodeB {
        //    @display("p=542,429;is=vl");
        // }
        rsu6: NRUe {
            @display("i=device/antennatower;p=608,357");
        }
        //gNodeB3: gNodeB {
        //    @display("p=542,429;is=vl");
        // }
        rsu7: NRUe {
            @display("i=device/antennatower;p=193,25");
        }
        //gNodeB3: gNodeB {
        //    @display("p=542,429;is=vl");
        // }
        rsu8: NRUe {
            @display("i=device/antennatower;p=176,343");
        }
        gNodeB1: gNodeB {
            @display("i=device/antennatower;p=156,55;is=vl");
        }
        gNodeB2: gNodeB {
            @display("i=device/antennatower;p=472,155;is=vl");
        }
        gNodeB3: gNodeB {
            @display("i=device/antennatower;p=156,265;is=vl");
        }
        gNodeB4: gNodeB {
            @display("i=device/antennatower;p=481,319;is=vl");
        }
    connections allowunconnected:
        TAS.pppg++ <--> Eth10G <--> router.pppg++;
        blockchain.pppg++ <--> Eth10G <--> router.pppg++;
        router.pppg++ <--> Eth10G <--> upf.filterGate;

        TAS1.pppg++ <--> Eth10G <--> router1.pppg++;
        blockchain.pppg++ <--> Eth10G <--> router1.pppg++;
        router1.pppg++ <--> Eth10G <--> upf1.filterGate;

        upf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
        upf.pppg++ <--> Eth10G <--> gNodeB2.ppp;

        upf1.pppg++ <--> Eth10G <--> gNodeB3.ppp;
        upf1.pppg++ <--> Eth10G <--> gNodeB4.ppp;

        //# X2 connections
        gNodeB1.x2++ <--> Eth10G <--> gNodeB2.x2++;
        gNodeB3.x2++ <--> Eth10G <--> gNodeB4.x2++;
        gNodeB1.x2++ <--> Eth10G <--> gNodeB3.x2++;
        gNodeB2.x2++ <--> Eth10G <--> gNodeB4.x2++;

}
