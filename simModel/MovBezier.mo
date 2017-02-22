package MovBezier
  connector PosConn
    Real p[3];
    Real v[3];
    Real a[3];
  end PosConn;
  
  package Comm
    function sendMessage "send a message through the socket"
      input String message;  
      external "C" sendMessage(message);
      annotation(Include = "#include \"Socket.h\"", Library = {"SocketModelicaUDP", "ws2_32"});
    end sendMessage;
  
    function receiveMessage "receive a message through the socket"
      external "C" receiveMessage();
      annotation(Include = "#include \"Socket.h\"", Library = {"SocketModelicaUDP", "ws2_32"});
    end receiveMessage;
  
    function createSocket "Create the TCP socket"
      input String IPAddress;
      input String port;  
      external "C" createSocket(IPAddress, port);
      annotation(Include = "#include \"Socket.h\"", Library = {"SocketModelicaUDP", "ws2_32"});
    end createSocket;
  
    function startUP "Initialize the Windows Socket Library"
      external "C" startUP();
      annotation(Include = "#include \"Socket.h\"", Library = {"SocketModelicaUDP", "ws2_32"});
    end startUP;
    
    function cleanUP
      external "C" cleanUp();
      annotation(Include = "#include \"Socket.h\"", Library = {"SocketModelicaUDP", "ws2_32"});
    end cleanUP;
  
    model Socket "Model to create a TCP socket to specified IPAddress and port"
      parameter String IPAddress = "127.0.0.1" "IP-Address of Server";
      parameter String port = "8887" "Port to connect to";
    initial algorithm
      startUP();
      createSocket(IPAddress, port);
    end Socket;
  end Comm;
  
  package Control
    model ControlSystem
      Comm.Socket socket(IPAddress = "127.0.0.1", port =  "8887");
      Real p[3];
      Real v[3];
      Real a[3];
      PosConn inConn;
    algorithm
      when sample(0, 0.1) then
        sendState(p, v, a, time, false);
      end when;
      when terminal() then
        sendState(p, {0,0,0}, {0,0,0}, time, true);
        Comm.cleanUP();
      end when;
    equation
      inConn.p = p;
      inConn.v = v;
      inConn.a = a;
    end ControlSystem;
    
    function sendState
      input Real p[3];
      input Real v[3];
      input Real a[3];
      input Real t;
      input Boolean last;
    protected
      String message;
      String lastBit;
    algorithm
      if (last) then
        lastBit := "1";
      else
        lastBit := "0";
      end if;
      message := "1;"
      + String(p[1]) + ";"
      + String(p[2]) + ";"
      + String(p[3]) + ";"
      + String(v[1]) + ";"
      + String(v[2]) + ";"
      + String(v[3]) + ";"
      + String(a[1]) + ";"
      + String(a[2]) + ";"
      + String(a[3]) + ";"
      + String(t) + ";" 
      + lastBit + "\f";
      Comm.sendMessage(message);
    end sendState;
  end Control;
  
  model Train
    Real p[3];
    Real v[3];
    Real u;
    Real du;
    Real speed;
    parameter Real p0[3];
    parameter Real p1[3];
    parameter Real p2[3];
    parameter Real p3[3];
    PosConn inConn;
  equation
    inConn.p = {u,0,0};
    inConn.v = {du,0,0};
    inConn.a = {0,0,0};
    p = Bezier(u,p0,p1,p2,p3);
    v = der(p); //Derivada respecto del tiempo
    speed = sqrt(v*v); //Módulo del vector velocidad
    speed = 10;
    du = der(u);
  end Train;
  
  function Bezier
    input Real u;
    input Real p0[3];
    input Real p1[3];
    input Real p2[3];
    input Real p3[3];
    output Real x[3];
  algorithm
    x := (1-u)^3*p0 + 3*(1-u)^2*u*p1 + 3*(1-u)*u^2*p2 + u^3*p3;
  end Bezier;
  
  model CigiEntity
    Train ent(p0={0,0,0}, p1={0,0,100}, p2={50,0,100}, p3={100,0,100});
    Control.ControlSystem ctrl;
  equation
    connect(ent.inConn, ctrl.inConn);
  end CigiEntity;
end MovBezier;
