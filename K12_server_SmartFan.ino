	#include <ESP8266WiFi.h>
	#include <ESP8266WebServer.h> 

  IPAddress    apIP(77, 77, 77, 77);

  const char* ssid = "MySmartFan";

  ESP8266WebServer server(80);

  void wifi_Setup() {

	    WiFi.mode(WIFI_AP_STA);

	    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));  

	    WiFi.softAP(ssid);

	    IPAddress myIP = WiFi.softAPIP();

	    Serial.print("AP IP address: ");

	    Serial.println(myIP);

	    server.on("/", handleRoot);

	    server.onNotFound(handleNotFound);

	    server.begin();

	    Serial.println("HTTP server started");

	}

  void wifi_loop() {

	    server.handleClient();

	    delay(10);

	} 

  void handleNotFound() {

	    String message = "File Not Foundnn";

	    message += "URI: ";

	    message += server.uri();

	    message += "nMethod: ";

	    message += (server.method() == HTTP_GET) ? "GET" : "POST";

	    message += "nArguments: ";

	    message += server.args();

	    message += "n";

	

	    for (uint8_t i = 0; i < server.args(); i++) {

	        message += " " + server.argName(i) + ": " + server.arg(i) + "n";

	    }

	

	    server.send(404, "text/plain", message);

	}  

  void handleRoot(){
    if(server.hasArg("temp")){
      inputTeperature = server.arg("temp").toInt();
    }

    String btnStete ="";
    if(server.hasArg("stts")){
      btnStete = server.arg("stts");
    }
    
    if(btnStete == "N"){
       CurrentStatus = ALWAYS_ON_MOSE;
    }else if(btnStete == "F"){
      CurrentStatus = ALWAYS_OFF_MOSE;
    }else if(btnStete == "T"){
      CurrentStatus = WORKING_MODE_BY_TEMPERATURE;
    }

    char html[2000];
    char TempNow[4] = "";
    char valInput[4]= "";
    strcat(html, "<html>");
    strcat(html, "<head>");
    strcat(html, "<style>");
    strcat(html, "body{");
    strcat(html, "background-color: beige;");
    strcat(html, "text-align: center;");            
    strcat(html, "}");

    strcat(html, "h1{");
    strcat(html, " font-size: 60px;");
    strcat(html, "}");

    strcat(html, "p{");
    strcat(html, "font-size: 30px;");
    strcat(html, "}");

    strcat(html, "#status{");
    strcat(html, "width: 80%;");
    strcat(html, "height: 100px;");
    strcat(html, "display: flex;");
    strcat(html, "align-items: center;");
    strcat(html, "justify-content: space-around;");
    strcat(html, " margin: auto;");
    strcat(html, "}");

    strcat(html, "#status button{");
    strcat(html, "width: 250px;");
    strcat(html, "height: 80px;");
    strcat(html, "border-radius: 50%;");
    strcat(html, "font-size: 30px;");
    strcat(html, "background-color: coral;");
    strcat(html, "color: white;");
    strcat(html, "margin-bottom: 30px;");
    strcat(html, "}");

    strcat(html, "#status button:hover{");
    strcat(html, "background-color: rgba(245, 186, 247, 0.322);");
    strcat(html, "color: black;");
    strcat(html, "}");

    strcat(html, "form label{");
    strcat(html, "font-size: 30px;");
    strcat(html, "}");

    strcat(html, "form input{");
    strcat(html, "width: 100px;");
    strcat(html, "font-size: 30px;");
    strcat(html, "text-align: center;");
    strcat(html, "border-radius: 8px;");
    strcat(html, "height: 40px;");
    strcat(html, "margin-right: 5px;");
    strcat(html, "}");

    strcat(html, "form button{");
    strcat(html, "width: 130px;");
    strcat(html, "height: 45px;");
    strcat(html, "font-size: 30px;");
    strcat(html, "border-radius: 8px;");
    strcat(html, "background-color: coral;");
    strcat(html, "color: white;");
    strcat(html, "}");
     
    strcat(html, "</style>");
    strcat(html, "<META HTTP-EQUIV='refresh' CONTENT='20'>");
    strcat(html, "<title>SmartFan</title>");
    strcat(html, "</head>");
    strcat(html, "<body>");
    strcat(html, "<h1>Management of modes</h1>");
        
    strcat(html, "<p><b>Current fan status: ");
    strcat(html, status);
    strcat(html, "</b></p>");
    itoa(nowTemperature, TempNow, 10 );
    strcat(html, "<p><b>Temperature Now: ");
    strcat(html, TempNow);
    strcat(html, "</b></p>");

    strcat(html, "<form action='' id='status'>");
    strcat(html, "<button name='stts' value='N'>activate</button>");
    strcat(html, "<button name='stts' value='F'>switch off</button>");
    strcat(html, "<button name='stts' value='T'>Activate by temperature</button>");
    strcat(html, "</form>");
    strcat(html, "<form action=''>");
    itoa(inputTeperature, valInput, 10 );        
    strcat(html, "<label>Temperature: <input type='number' name='temp' value='");
    strcat(html, valInput);
    strcat(html, "'/></label>");
    strcat(html, "<button><b>Submit</b></button>");
    strcat(html, "</form>");
    strcat(html, "</body>");
    strcat(html, "</html>");

    server.send(200, "text/html", html);
  }