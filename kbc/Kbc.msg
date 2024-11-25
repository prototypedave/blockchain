import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;

namespace inet;

class RAPacket extends FieldsChunk
{
 	string vID;
 	string service;
 	string vIP;
 	double lifetime;   
}

class TGTPacket extends FieldsChunk
{
 	string AT;
 	string key;
 	string TGT;  
}

class AttributePacket extends FieldsChunk
{
 	string service;
 	string lifetime;
 	string AU;  
 	string TGT; 
}

class BlockchainPacket extends FieldsChunk
{
 	string AU;
 	string vehicleId;
 	string destination;   
}

class TicketPacket extends FieldsChunk 
{
 	string ST;
 	string AT;
 	string key;   
}

class SecretPacket extends FieldsChunk
{
 	string key;   
}

class AuthPacket extends FieldsChunk
{
    string AU;
    string ST;
}

class AuthenticatorPacket extends FieldsChunk
{
    string AU;
}

class HandoverPacket extends FieldsChunk
{
    string service;
    string ip;
}