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
    
    char html[2000];
    strcat(html, "<html>");
    strcat(html, "<head>");
    strcat(html, "<meta charset='UTF-8'>");
    strcat(html, "<meta http-equiv='X-UA-Compatible' content='IE=edge'>");
    strcat(html, "<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    strcat(html, "<style>");
        strcat(html, "body{");
            strcat(html, "background-color: beige;");
        strcat(html, "}");

        strcat(html, "#container{");
           strcat(html, "width: 60%;");
            strcat(html, "height: 80%;");
            strcat(html, "margin: auto;");
        strcat(html, "}");

        strcat(html, "#mode{");
            strcat(html, "font-size: 20px;");
        strcat(html, "}");

        strcat(html, "#status{");
            strcat(html, "width: 100%;");
            strcat(html, "height: 100px;");
            strcat(html, "display: flex;");
            strcat(html, "align-items: center;");
            strcat(html, "justify-content: space-around;");
        strcat(html, "}");

        strcat(html, "#status button{");
            strcat(html, "width: 180px;");
            strcat(html, "height: 80px;");
            strcat(html, "border-radius: 50%;");
            strcat(html, "font-size: 20px;");
            strcat(html, "background-color: coral;");
            strcat(html, "color: white;");
            strcat(html, "margin-bottom: 30px;");
        strcat(html, "}");

        strcat(html, "#status button:hover{");
            strcat(html, "background-color: rgba(245, 186, 247, 0.322);");
            strcat(html, "color: black;");
        strcat(html, "}");

    // ---------------------------------------------------------------------------------------
        strcat(html, "form input{");
            strcat(html, "width: 60px;");
            strcat(html, "text-align: center;");
        strcat(html, "}");

        strcat(html, "form button{");
            strcat(html, "width: 70px;");
            strcat(html, "height: 25px;");
            strcat(html, "border-radius: 8px;");
            strcat(html, "background-color: coral;");
            strcat(html, "color: white;");
         strcat(html, "}");
    // ------------------------------------------------------------------------------------  
        strcat(html, "</style>");
        strcat(html, "<title>SmartFan</title>");
        strcat(html, "</head>");
        strcat(html, "<body>");
        strcat(html, "<div id='container'>");
        strcat(html, "<h1>Management of modes</h1>");
        strcat(html, "<p id='mode'><b>Current fan status:</b></p>");
        strcat(html, "<div id='status'>");
            strcat(html, "<button>activate</button>");
            strcat(html, "<button>switch off</button>");
            strcat(html, "<button>Activate by temperature</button>");
        strcat(html, "</div>");
        strcat(html, "<form>");
            strcat(html, "<label>Temperature: <input type='number' id='temp' value='20' /></label>");
            strcat(html, "<button><b>Submit</b></button>");
        strcat(html, "</form>");
       strcat(html, "</div>");
      strcat(html, "</body>");
      strcat(html, "</html>");

    server.send(200, "text/html", html);
  }