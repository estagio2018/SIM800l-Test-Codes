/*Code Test - Enviando e recebendo SMS com módulo SIM800L.
  Ficha técnica:
    * O módulo possui picos de transmissão, o que pode utilizar muita corrente do próprio arduino, recomenda-se
      utilizar uma fonte externa de 3,4V ~ 4,3V com pelo menos 2A.
    * Aplicar resistores nos sinais de dados de transmissão entre o Arduino Mega e o Módulo.
  */
      

void setup(){
  Serial2.begin(115200);
  pinMode(13, OUTPUT);//LED
  pinMode(8, OUTPUT);//Pino de reset do GSM

  //reinicia o GSM
  digitalWrite(8, 0); delay(2000);
  digitalWrite(8, 1); delay(7000);
  if (gsmWrite("AT", "OK") == "FAIL"){
    return;//Se o GSM nao responder, retorna a funcao (trava o mcu)
  }

  delay(5000);//Espera o termino da inicializacao do GSM
  SMSsnd("062991973345", "Arduino ON, analizando...");//Envia a mensagem pro numero. Atencao: precisa do DDD!
}

void loop(){
  SMSrd();//Verifica se há novas mensagens para serem lidas a cada 10seg
  delay(10000);
}

String gsmWrite(String snd, String rcv){//Funcao que envia dados pro GSM e espera a resposta de ate 30seg
  Serial2.println(snd);
  if (rcv.indexOf("+CMGS") > -1){
    delay(150);
    Serial2.write(0x1A);
  }
  for (uint16_t i = 0; i < 1200; i++){
    delay(25);
    if (Serial2.available()){
      delay(50);
      String a = Serial2.readString();
      if (a.indexOf(rcv) > -1 || rcv.length() == 0){
        return a;
      }
    }
  }
  return "FAIL";
}

void SMSsnd(String nm, String msg){//Funcao que envia o SMS
  String snd = "AT+CMGS=\"";  snd += nm;  snd += "\"";
  gsmWrite("AT+CMGF=1", "OK");
  gsmWrite("AT+CSMP=17,255,0,0", "OK");
  gsmWrite(snd, ">");
  gsmWrite(msg, "+CMGS:");
}

void SMSrd(){//Funcao que le se ha SMS nao lido
  String c = "";
  gsmWrite("AT+CMGF=1", "OK");
  c = gsmWrite("AT+CMGL=\"REC UNREAD\"", "");
  if(c.indexOf("+CMGL:") > -1){
    if(c.indexOf("ON") > -1){//ON
      digitalWrite(13, 1);//se o SMS conter o texto ON, liga o LED
    }
    else if (c.indexOf("OFF") > -1){//OFF
      digitalWrite(13, 0);//se o SMS conter o texto OFF, desliga o LED
    }
    gsmWrite("AT+CMGD=1,1", "OK");//apaga todas mensagens (SIM card ha pouca memoria)
  }
}

/*https://portal.vidadesilicio.com.br/modulo-gsm-sim800l/
*/
