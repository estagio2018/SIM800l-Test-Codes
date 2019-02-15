/*Code Test - Enviando e recebendo SMS com módulo SIM800L.
  Ficha técnica:
    * O módulo possui picos de transmissão, o que pode utilizar muita corrente do próprio arduino, recomenda-se
      utilizar uma fonte externa de 3,4V ~ 4,3V com pelo menos 2A.
    * Aplicar resistores nos sinais de dados de transmissão entre o Arduino Mega e o Módulo.
  */

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);//Inicia a Serial do GSM
  pinMode(8, OUTPUT);//Pino de reset do GSM
 
  //Reinicia o GSM
  digitalWrite(8, 0); delay(2000);
  digitalWrite(8, 1); delay(7000);
  String rcv = gsmWrite("AT", "*PSUTTZ:");//Espera o GSM obter o horario da rede
  if (rcv.indexOf("FAIL") > -1){
    gsmWrite("AT+CLTS=1;&W", "OK");//Se nao responder, pode ser configuracao do seu GSM, entao vamos ativar a obtencao automatica de horario pela rede
    setup();//retorna ao inicio
  }
  delay(5000);
}


void loop(){
  Serial.println(gsmTIME());//Mostra a data e horario no Serial Monitor
  delay(5000);
}
  
String gsmWrite(String snd, String rcv){//Funcao que envia dados ao GSM e espera a resposta
  Serial2.println(snd);
  if (rcv.indexOf("+CMGS") > -1){
    delay(150); Serial2.write(0x1A);
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
String gsmTIME(){//Funcao que retorna a data e horario
  String c = gsmWrite("AT+CCLK?", "+CCLK:");
  int16_t a = c.indexOf("\"") + 1; int16_t b = c.indexOf("\"", a);
  return c.substring(a, b);
}

/*https://portal.vidadesilicio.com.br/modulo-gsm-sim800l/
*/
