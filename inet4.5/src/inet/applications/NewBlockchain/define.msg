import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;

namespace inet;

class VehicleRA extends FieldsChunk
{
    int VehId;
    int rsuId;
    string userIp;
    int requestedLifetime;
}

class ASAT extends FieldsChunk
{
    string AT;
    string secret;
    int vehId;
}

class TGT extends FieldsChunk
{
    string tGT;
    string secret;
}

class VehicleAT extends FieldsChunk
{
    int serviceName;
    int LTReqTGT;
}

class VehicleAU extends FieldsChunk
{
    string AU;
    string secret;
}

class CHAIN extends FieldsChunk
{
    string AU;
    string vehId;
    string destId;
}

class TGSAT extends FieldsChunk
{
    string AT;
    string secret;
}

class STPacket extends FieldsChunk
{
    string sT;
    string secret;
}

class VehicleToServerAU extends FieldsChunk
{
    string AU;
    string secret;
}

class ServerToVehicleAU extends FieldsChunk
{
    string AU;
    string secret;
    int rsuId;
}

