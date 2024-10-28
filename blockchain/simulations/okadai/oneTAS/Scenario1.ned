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

package blockchain.simulations.okadai.oneTAS;

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

network Scenario1
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
            @display("p=547,44;is=n;i=device/server");
        }
        router: Router {
            @display("p=414,98;i=device/smallrouter");
        }
        upf: Upf {
            @display("p=414,196");
        }
        //# submodule vector to store all cars - will be filled dynamically during simulations
        car[0]: NRCar {
            @display("p=241,162;is=vs");
        }

        blockchain: StandardHost {
            @display("p=547,144;i=block/routing;is=m");
        }
        //gNodeB3: gNodeB {
        //    @display("p=542,429;is=vl");
        // }
        rsu3: NRUe {
            @display("i=device/antennatower;p=496,313;is=m");
        }
        rsu4: NRUe {
            @display("i=device/antennatower;p=156,321;is=m");
        }
        rsu2: NRUe {
            @display("i=device/antennatower;p=193,44;is=m");
        }
        rsu1: NRUe {
            @display("i=device/antennatower;p=485,35;is=m");
        }
        gNodeB1: gNodeB {
            @display("i=device/antennatower;p=156,136;is=vl");
        }
        gNodeB2: gNodeB {
            @display("i=device/antennatower;p=391,313;is=vl");
        }
    connections allowunconnected:
        TAS.pppg++ <--> Eth10G <--> router.pppg++;
        blockchain.pppg++ <--> Eth10G <--> router.pppg++;
        router.pppg++ <--> Eth10G <--> upf.filterGate;

        upf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
        upf.pppg++ <--> Eth10G <--> gNodeB2.ppp;

        //# X2 connections
        gNodeB1.x2++ <--> Eth10G <--> gNodeB2.x2++;
}
