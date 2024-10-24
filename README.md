# Blockchain Authentication System for VANETs
This project implements an innovative authentication system designed for Vehicular Ad-Hoc Networks (VANETs). It combines the Kerberos authentication protocol with blockchain technology to ensure secure, scalable, and efficient authentication between vehicles and network components. This system is especially tailored for mobility scenarios, reducing authentication delays during handovers by using a decentralized blockchain ledger for storing authentication information.

## Key Features
-  **Secure Authentication**: Provides a robust authentication mechanism for       vehicle-to-infrastructure (V2I) and vehicle-to-vehicle (V2V) communication within VANETs.
-  **Efficient Handover**: Streamlines the handover process during vehicle movement, reducing delays and signaling overhead.
-  **Blockchain Integration**: Authenticator messages are stored in a decentralized, tamper-resistant blockchain ledger, minimizing reliance on centralized servers.
-  **Scalability**: Leverages blockchain’s distributed nature to enhance scalability and system resilience.

## System Architecture
The architecture includes the following components:
1.  **Vehicular Ad-Hoc Network (VANET)**: Enables communication between vehicles and infrastructure.
    Kerberos Authentication Server: Composed of a Trusted Authority Server (TAS) and an Authentication Server (AS) with Ticket Granting Server (TGS) functionality.
2.  **Blockchain Network**: Decentralized storage for authenticator messages, operated by Trusted  Authorities (TAs) and Road-Side Units (RSUs).
3.  **Road-Side Units (RSUs)**: Infrastructure that facilitates vehicle communications and        authentication.
4.  **Vehicles (On-Board Units, OBUs)**: Mobile nodes in the network that authenticate via Kerberos and the blockchain.

## Workflow
1.  Vehicles and RSUs register with the Trusted Authority Server (TAS).
2.  Vehicles initiate authentication via Kerberos, obtaining a ticket from the AS and TGS.
3.  Authenticator messages are stored in the blockchain, making them accessible to all nodes (TAs, RSUs).
4.  RSUs retrieve and verify authenticator messages from the blockchain to authenticate vehicles without requiring direct communication with the central server.
5.  Upon successful authentication, vehicles can securely communicate with RSUs and other vehicles.

## Authentication Phases
### 1. Registration Phase 
-   **Vehicles**: Registered by providing vehicle identification, user details, and role information.
-   **RSUs**: Registered by the Trusted Authorities with details like location, MAC address, and IP.

### 2. Initial Authentication Phase
1.  **Vehicle → AS**: Sends registration info and an RA message.
2.  **AS → Vehicle**: AS verifies registration and issues a Ticket Granting Ticket (TGT).
3.  **Vehicle → TGS**: Vehicle sends the TGT and requests service from TGS.
4.  **TGS → Blockchain**: Authenticator messages are validated and stored in the blockchain.
5.  **Vehicle → RSU**: The vehicle communicates with the RSU using the stored authenticator.

## Blockchain Integration
The blockchain manages the following key components:
-   **Transaction**: Represents data exchange, including the authenticator messages.
-   **Block**: Stores multiple transactions and ensures immutability through proof-of-work.
-   **Blockchain Network**: Distributed ledger that stores authenticator messages, eliminating the need for repeated central server authentication.

## Key Cryptographic Functions
-   **Key Pair Generation**: RSA key pairs for digital signatures.
-   **SHA-256 Hashing**: For secure data hashing.
-   **Message Signing/Verification**: Ensures message authenticity using RSA signatures.

## Performance Metrics
The system's performance is evaluated using the following metrics:
1. **Authentication Delay**: Time taken to authenticate a vehicle.
2. **End-to-End Delay**: Latency in message transmission.
3. **Throughput**: Measured by total bytes received over time.
4. **Computational Cost**: Time taken to execute tasks at different nodes.
5. **Packet Loss**: Difference between sent and received packets.

## Limitations
-  This system focuses on authentication and does not address other aspects like data privacy or message confidentiality.
-  The system assumes the presence of a trusted infrastructure with functioning Trusted Authorities (TAs) and Road-Side Units (RSUs).
-  Vulnerabilities in the underlying technologies (Kerberos, blockchain) could affect the system’s security.

## How to Run
-  Install OMNeT++ following the official documentation.
-  Install SUMO
-  Install INET FRAMEWORK & VEINS
-  Clone the repository:
   ```bash
   git clone https://github.com/prototypedave/blockchain.git
   ```
-  Configure the network topology in the OMNeT++ simulation environment.
-  if using the omnetpp IDE run the simulation as omnetpp project

## Contributions
Open to contributions and ideas to perfect this