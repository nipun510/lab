package mutility
/*

import (
  "net"
  "fmt"
  "time"
)

// failure detector runs as separate process
type FailureDetector struct {
  ip string
  port string
  selfNode string
  endpoints []string 
  lastHeartBeatTime map[string]
  failedNodes []string
  sendInterval int
  timeoutInterval int
}

func NewFailureDetector(ip,  port, selfNode string,
                        sendInterval int, 
                        timeOutFactor double) (* FailureDetector) 
{
  detector := &FailureDetector{ip: ip, port: port, 
                            selfNode: selfNode, 
                            sendInterval: sendInterval, 
                            timeoutInterval: sendInterval * timeOutFactor}                          
                            
  return detector                          
}

func (detector *FailureDetector) addEndPoint(ip, port string) {
  endpoints = append(endpoints, ip + ":" + port)
}

func (detector *FailureDetector) startUDPServer() {
  address := dectector.ip + ":" + detector.port
  listener, err := net.ListenUDP("udp4",  address)
  if err != nil {
    fmt.Println(err)
    return
  }

  defer listener.Close()
  buffer := make([]byte, 1024)

  for {
    n, addr, err := listener.ReadFromUDP(buffer)
    if err != nil {
      fmt.Println(err)
      return
    }
    go receiveHeartBeat(addr, string(buffer[0: n-1]))
  }
}

// sends heartbeat packet after a fixed interval
func (detector *FailureDetector) sendHeartBeatMessage(conn *net.Conn) {
}


// receives hearbeat packet and updates the node as being active
func (detector *FailureDetector) receiveHeatBeatMessage(addr, message string) {
  fmt.Println("Received %s from %s \n", message, addr)
}


// updates nodes status and mark a node failing
// if no heartbeat packet received from it in TIMEOUT interval
func (FailureDetector *) markFail() {
}

func (detector *FailureDetector) Start() {
  startUDPServer()
}
*/
