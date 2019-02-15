/*Code Test - Obtendo Localização Através da Rede.
  Ficha técnica:
    * O módulo possui picos de transmissão, o que pode utilizar muita corrente do próprio arduino, recomenda-se
      utilizar uma fonte externa de 3,4V ~ 4,3V com pelo menos 2A.
    * Aplicar resistores nos sinais de dados de transmissão entre o Arduino Mega e o Módulo.
  */
void setup(){
  Serial.begin(115200); Serial2.begin(115200);
  pinMode(8, OUTPUT);//Pino de reset do GSM
 
  //Reseta o GSM
  digitalWrite(8, 0);
  delay(2000);
  digitalWrite(8, 1);
  delay(7000);
  if (gsmWrite("AT", "OK") == "FAIL"){
    return;
  } 
  delay(5000);
  if (gsmWrite("AT+SAPBR=1,1", "OK") == "FAIL"){//Ativa a rede pra localizacao
    return;
  }
  else{
    String rcv = gsmWrite("AT+CIPGSMLOC=1,1", "+CIPGSMLOC:");//Pergunta a localizacao ao GSM
    int a = rcv.indexOf(":") + 2;
    int b = rcv.indexOf(",", a);
    if (rcv.substring(a, b) == "0"){
      a = rcv.indexOf(",", b) + 1;
      b = rcv.indexOf(",", a) + 1;
      b = rcv.indexOf(",", b);
      Serial.println(rcv.substring(a, b));//Mostra a localizacao no Serial monitor
    }
  }
}

void loop(){

} 
 
String gsmWrite(String snd, String rcv){//Funcao que envia dados ao GSM e espera a resposta
  Serial2.println(snd);
  if (rcv.indexOf("+CMGS") > -1)  {
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

/*https://portal.vidadesilicio.com.br/modulo-gsm-sim800l/
*/
