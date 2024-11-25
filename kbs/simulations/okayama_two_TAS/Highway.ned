// 
//                  Simu5G
//
// Authors: Giovanni Nardini, Giovanni Stea, Antonio Virdis (University of Pisa)
// 
// This file is part of a software released under the license included in file
// "license.pdf". Please read LICENSE and README files before using it.
// The above files and the present reference are part of the software itself, 
// and cannot be removed from it.
//
package kbs.simulations.okayama_two_TAS;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.inet.AdhocHost;
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

network Highway
{
    parameters:
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=2500,1000");

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
        TAS1: StandardHost {
            @display("p=663,125;is=n;i=device/server");
        }
        TAS2: StandardHost {
            @display("p=1901,125;is=n;i=device/server");
        }
        router: Router {
            @display("p=1275,350;i=device/smallrouter");
        }
        upf: Upf {
            @display("p=1275,463");
        }
        //# submodule vector to store all cars - will be filled dynamically during simulations
        car[0]: NRCar {
            @display("is=vs");
        }

        blockchain: StandardHost {
            @display("p=1275,655;i=block/routing;is=s");
        }
        //gNodeB3: gNodeB {
        //    @display("p=542,429;is=vl");
        // }
        RSU3: NRUe {
            @display("i=device/antennatower;p=2140,135;is=l");
        }
        RSU4: NRUe {
            @display("i=device/antennatower;p=2119,828;is=l");
        }
        RSU2: NRUe {
            @display("i=device/antennatower;p=241,793;is=l");
        }
        RSU1: NRUe {
            @display("i=device/antennatower;p=241,161;is=l");
        }
        gNodeB1: gNodeB {
            @display("i=device/antennatower;p=519,493;is=vl");
        }
        gNodeB2: gNodeB {
            @display("i=device/antennatower;p=2052,582;is=vl");
        }
        RSU5: NRUe {
            @display("i=device/antennatower;p=930,239;is=l");
        }
        RSU6: NRUe {
            @display("i=device/antennatower;p=429,423;is=l");
        }
        RSU7: NRUe {
            @display("i=device/antennatower;p=1196,793;is=l");
        }
        RSU8: NRUe {
            @display("i=device/antennatower;p=1901,403;is=l");
        }

    connections allowunconnected:
        TAS1.pppg++ <--> Eth10G <--> router.pppg++;
        TAS2.pppg++ <--> Eth10G <--> router.pppg++;
        blockchain.pppg++ <--> Eth10G <--> router.pppg++;
        router.pppg++ <--> Eth10G <--> upf.filterGate;
        upf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
        upf.pppg++ <--> Eth10G <--> gNodeB2.ppp;

        //# X2 connections
        gNodeB1.x2++ <--> Eth10G <--> gNodeB2.x2++;
}

