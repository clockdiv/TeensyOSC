/*
  UDPSendReceiveString:
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender

  A Processing sketch is included at the end of file that can be used to send
  and received messages for testing with a computer.

  created 21 Aug 2010
  by Michael Margolis

  This code is in the public domain.
*/

#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include <OSCBundle.h>

// MAC Address:
byte mac[] = { 0x04, 0xE9, 0xE5, 0x0E, 0x03, 0x82};

// IP:
// I use a DHCP and map a static IP with the router.
// If still an IP should be given with the Teensy, here you go:
// IPAddress ip(192, 168, 235, 177);

// PORT:
unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet, size: 24
char ReplyBuffer[] = "acknowledged";        // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;




void foo(OSCMessage &msg) {
  if (msg.isFloat(0)) {
    Serial.print("Received a float, ");
    float f = msg.getFloat(0);
    Serial.println(f);
  }
  else if (msg.isInt(0)) {
    Serial.print("Received an int, ");
    int i = msg.getInt(0);
    Serial.println(i);
  }
  else if (msg.isString(0)) {
    Serial.print("Received a string, ");
    char str[20];
    msg.getString(0, str);
    String s = (char*)str;
    Serial.println(s);
  }


}

void setup() {
  Serial.begin(115200);

  // Start Ethernet with DHCP or your own IP
  Ethernet.begin(mac);
  //Ethernet.begin(mac, ip);


  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  } else {
    Serial.println("CHECK - ethernet hardware found");
  }


  // Check if cable is connected
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  } else {
    Serial.println("CHECK - ethernet cable is connected");
  }


  // Print the local IP
  Serial.print("Local IP-Address: ");
  Serial.println(Ethernet.localIP());


  // start UDP
  Udp.begin(localPort);
}





void loop() {
  OSCBundle bundleIN;
  int osc_bundle_size;

  if ( (osc_bundle_size = Udp.parsePacket()) > 0)
  {
    while (osc_bundle_size--) {
      bundleIN.fill(Udp.read());
    }

    if (!bundleIN.hasError()) {
      bundleIN.dispatch("/foo", foo);
      bundleIN.empty();
    }

  }

}
