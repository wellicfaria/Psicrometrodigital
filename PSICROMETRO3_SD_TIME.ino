#include <Time.h>
#include <SD.h>
#include <dht.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

//DECLAÇÃO DAS VARIAVEIS
float TempMAX, TempMIN,TempAtual; //Temperatura Maxima, Minima e Atual
float UrMAX,UrMIN,UrAtual; //UR Maxima, Minina e Atual
int hora,minuto,segundo,dia,mes,ano;
int fhora,fminuto,fsegundo,fdia,fmes,fano;
int op;
float POrvalho; //Ponto de Orvalho
const int Botao1=0,Botao2=3; //Se led estiver acesso indica que microcrolador foi reninciado e medidas podem estar erradas(Ex: Teve uma queda de energia)
LiquidCrystal lcd(10, 9, 8, 7, 6, 5); //Configuração do LCD
dht DHT;
//O SHIELD SD CART UTILIZA O D4,D11,D12 E D13
const int chipSelect = 4;
String dado;
unsigned long tfinal;
//DEFINIÇÃO DE POSIÇÃO DA EEPROM PARA SALVAR OS DADOS
#define DHT03_PIN 2
#define TMax 0
#define TMin 2
#define UMax 4
#define UMin 6
#define phora 8
#define pminuto 9
#define psegundo 10
#define pdia 11
#define pmes 12
#define pano 13

void GravarNoSd()
{
  dado="";
  dado+=dia;
  dado+= "/";
  dado+=mes;
  dado+= "/";
  dado+=ano;
  dado+="	";
  dado+=hora;
  dado+= ":";
  dado+=minuto;
  dado+= ":";
  dado+=segundo;
  dado+="	";
  dado+=int(TempAtual);
  dado+=",";
  dado+=int((TempAtual-int(TempAtual))*100);
  dado+="	";
  dado+=int(TempMIN);
  dado+=",";
  dado+=int((TempMIN-int(TempMIN))*100);
  dado+="	";
  dado+=int(TempMAX);
  dado+=",";
  dado+=int((TempMAX-int(TempMAX))*100);
  dado+="	";
  dado+=int(UrAtual);
  dado+="	";
  dado+=int(UrMIN);
  dado+="	";
  dado+=int(UrMAX);
  dado+="	";
  dado+=int(POrvalho);
  dado+=",";
  dado+=int((POrvalho-int(POrvalho))*100);
  File dataFile = SD.open("Medidas.txt", FILE_WRITE);
  if (dataFile) 
  {
    dataFile.println(dado);
    dataFile.close();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Gravou SD");
  }  
  else 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SD ERRO");  
  } 
}
//Roda o Relogio
void relogio()
{
  hora=hour(); 
  minuto=minute();    
  segundo= second();
  dia=day(); 
  mes=month();     
  ano=year(); 
  if(hora!=fhora)
  {
    fhora=hora;
    EEPROM.write(phora,hora);
  }
  if(minuto!=fminuto)
  {
    fminuto=minuto;
    EEPROM.write(pminuto,minuto);
  }
  if(segundo!=fsegundo)
  {
    fsegundo=segundo;
    EEPROM.write(psegundo,segundo);
  }
  if(dia!=fdia)
  {
    fdia=dia;
    EEPROM.write(pdia,dia);
    Reincinciar();
  }
  if(mes!=fmes)
  {
    fmes=mes;
    EEPROM.write(pmes,mes);
  }
  if(ano!=fano)
  {
    fano=ano;
    EEPROM.write(pano,(ano-2000));
  }
}
//Ajusta o Relogio
void AjustarRelogio()
{
  hora=0;
  minuto=0;    
  segundo=0;
  dia=1; 
  mes=1;     
  ano=2010;
  int time1=200,time2=200;
  lcd.clear();

  while(digitalRead(Botao2)!=LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("HORA:");
    lcd.setCursor(0,1);
    if(digitalRead(Botao1)==LOW)
    {
      hora++;
      if(hora>23)
      {
        hora=0;
        lcd.clear();
      }
       delay(time1);
    }
    lcd.print(hora);
  }
  lcd.clear();
  delay(time2);
  while(digitalRead(Botao2)!=LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("MINUTO:");
    lcd.setCursor(0,1);
    if(digitalRead(Botao1)==LOW)
    {
      minuto++;
      if(minuto>59)
      {
        minuto=0;
        lcd.clear();
      }
     delay(time1);
    }
    lcd.print(minuto);
  }
  lcd.clear();
  delay(time2);
  while(digitalRead(Botao2)!=LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("DIA:");
    lcd.setCursor(0,1);
    if(digitalRead(Botao1)==LOW)
    {
      dia++;
      if(dia>31)
      {
        dia=1;
        lcd.clear();
      }
        delay(time1);
    }
    lcd.print(dia);
  }

  lcd.clear();
  delay(time2);
  while(digitalRead(Botao2)!=LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("MES:");
    lcd.setCursor(0,1);
    if(digitalRead(Botao1)==LOW)
    {
      mes++;
      if(mes>12)
      {
        mes=1;
        lcd.clear();
      }
       delay(time1);
    }
    lcd.print(mes);
  }

  lcd.clear();
  delay(time2);
  while(digitalRead(Botao2)!=LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("ANO:");
    lcd.setCursor(0,1);
    if(digitalRead(Botao1)==LOW)
    {
      ano++;
      delay(time1);
    }
    lcd.print(ano);   
  }
  setTime(hora,minuto,segundo,dia,mes,ano); 
  fhora=hora;
  fminuto=minuto,
  fsegundo=segundo;
  fdia=dia;
  fmes=mes;
  fano=ano;
  EEPROM.write(phora,hora);
  EEPROM.write(pminuto,minuto);
  EEPROM.write(psegundo,segundo);
  EEPROM.write(pdia,dia);
  EEPROM.write(pmes,mes);
  EEPROM.write(pano,(ano-2000));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Relogio OK");
  lcd.setCursor(0,1);
  lcd.print(hora);
  lcd.print(":");
  lcd.print(minuto);
  lcd.setCursor(0,2);
  lcd.print(dia);
  lcd.print("/");
  lcd.print(mes);
  lcd.print("/");
  lcd.print(ano);
  delay(3000);

}
//FUNÇÃO QUE GRAVA O DADO X NA EEPROM P
//NA GRAVAÇÃO DO DADO É CONSUMIDA 2 POSIÇÕES DA EEPROM
void gravar(float x,int p)
{

  EEPROM.write (p,int(x));
  EEPROM.write (p+1,int((x-int(x))*100));
}

//FUNÇÃO QUE LÊ O VALOR NO POSISÃO p DA EEPROM
float ler(int p)
{
  return float(EEPROM.read(p))+ float(EEPROM.read(p+1))/100;
}

//FUNÇÃO PARA CALCULO DO PONTO DE ORVALHO
float PontoDeOrvalho (float celsius , float humidity ) 
{
  float RATIO = 373.15 / (273.15 + celsius );         
  float SUM = -7.90298 * (RATIO - 1);        
  SUM += 5.02808 * log10 (RATIO );        
  SUM += -1.3816e-7 * (pow (10 , (11.344 * (1 - 1/RATIO ))) - 1) ;        
  SUM += 8.1328e-3 * (pow (10 , (-3.49149 * (RATIO - 1))) - 1) ;        
  SUM += log10 (1013.246 );        
  float VP = pow (10 , SUM - 3) * humidity ;       
  float T = log (VP /0.61078 );         
  return (241.88 * T ) / (17.558 - T ); 
}

//Função de Configuração Inicial
//Quando o Arduino é ligado/reniciado
//busca os valores máximos salvos na memoria e liga o led
//indicando que houve uma queda de energia.
void ConfiguracoesIniciais()
{
  TempMAX=ler(TMax);
  TempMIN=ler(TMin);
  UrMAX=ler(UMax);
  UrMIN=ler(UMin);
  hora=EEPROM.read(phora);
  minuto=EEPROM.read(pminuto);
  segundo=EEPROM.read(psegundo);
  dia=EEPROM.read(pdia);
  mes=EEPROM.read(pmes);
  ano=2000+int(EEPROM.read(pano));
  fhora=hora;
  fminuto=minuto;
  fsegundo=segundo;
  fdia=dia;
  fmes=mes;
  fano=ano;
  setTime(hora,minuto,segundo,dia,mes,ano); 
}

//Função que zera o dados
void Reincinciar()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("..Reiniciando.."); 
  delay(3000);
  lcd.clear();
  TempMAX=TempAtual;
  TempMIN=TempAtual; 
  UrMAX=UrAtual;
  UrMIN=UrAtual;

  gravar(TempMAX,TMax);
  gravar(TempMIN,TMin);
  gravar(UrMAX,UMax);
  gravar(UrMIN,UMin);
  lcd.setCursor(0,0);
  lcd.print("..Reiniciando..");
  lcd.setCursor(0,1);
  delay(1000);
  lcd.print("!!!OK!!!");
  lcd.setCursor(0,2);
  lcd.print("Reiniciado!!!");
  delay(3000);
  lcd.clear();

}
//Mostra dados Atuais no Display
void LCDAtual()
{

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dados Atuais:");

  lcd.setCursor(4,1);
  lcd.print("Temp:");
  lcd.print(TempAtual);

  lcd.setCursor(0,2);
  lcd.print("Umi:");
  lcd.print(UrAtual); 

  lcd.setCursor(0,3);
  lcd.print("PO:");
  lcd.print(POrvalho); 


}
//Processa e regista o Maximos e Mininos e calcula Ponto de Orvalho
void Processamento()
{
  if(TempAtual>TempMAX)
  {
    TempMAX=TempAtual;
    gravar(TempMAX,TMax);
  }
  if(TempAtual<TempMIN)
  {
    TempMIN=TempAtual;
    gravar(TempMIN,TMin);
  }
  if(UrAtual>UrMAX)
  {
    UrMAX=UrAtual;
    gravar(UrMAX,UMax);
  }
  if(UrAtual<UrMIN)
  {
    UrMIN=UrAtual; 
    gravar(UrMIN,UMin);
  }
  POrvalho=PontoDeOrvalho(TempAtual,UrAtual);
}
//Mostra os valores Maximos e Minimos no Display
void LCDMaxeMin()
{
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("TMAX:");
  lcd.print(TempMAX);
  lcd.setCursor(4,1);
  lcd.print("TMIN:");
  lcd.print(TempMIN);
  lcd.setCursor(0,2);
  lcd.print("UMAX:");
  lcd.print(UrMAX);
  lcd.setCursor(0,3);
  lcd.print("UMIN:");
  lcd.print(UrMIN); 
}
void menu()
{
  if(op==1)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(dia);
    lcd.print("/");
    lcd.print(mes);
    lcd.print("/");
    lcd.print(ano-2000);  
    lcd.setCursor(0,1);
    lcd.print(hora);
    lcd.print(":");
    lcd.print(minuto);
    lcd.print(":");
    lcd.print(segundo);
  }else if(op==2)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ajus. Relogio:");
    lcd.setCursor(0,1);
    lcd.print("Press. B2");
    if(digitalRead(Botao2)==LOW)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Aguarde...");
      lcd.setCursor(0,1);
      lcd.print("Solte B2!");
      delay(2000);
      AjustarRelogio();
      op=3;
    }
  }else if(op==3)
  {
    LCDAtual();
  }else if(op==4)
  {
    LCDMaxeMin();
  }else if(op==5)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reininciar:");
    lcd.setCursor(0,1);
    lcd.print("Press. B2");
    if(digitalRead(Botao2)==LOW)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Aguarde...");
      lcd.setCursor(0,1);
      lcd.print("Solte B2!");
      delay(2000);
      Reincinciar();
      op=3;
    }

  }else
  {
    op=1;
  }
  
}
//CONFIGURAÇÕES INICIAIS DO PROGRAMA
void setup()
{
  lcd.begin(16,4); //CONFIGURA O TAMANHO DO LCD 16 COLUNAS 4 LINHAS
  pinMode(Botao1,INPUT_PULLUP);//CONFIGURA LED
  pinMode(Botao2,INPUT_PULLUP);//CONFIGURA BOTÃO
  ConfiguracoesIniciais(); //CHAMA A FUNÇÃO INICIAL
  tfinal=millis();
  op=3;
  if (!SD.begin(chipSelect)) 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ERRO no SD");
    delay(1000);
    return;
  }
  File dataFile = SD.open("Medidas.txt", FILE_WRITE);
  dado="MicroReinicidado:";
  dado+=dia;
  dado+= "/";
  dado+=mes;
  dado+= "/";
  dado+=ano;
  dado+="	";
  dado+=hora;
  dado+= ":";
  dado+=minuto;
  dado+= ":";
  dado+=segundo;
  if (dataFile) 
  {
    dataFile.println(dado);
    dataFile.close();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Gravou SD X");
  }  
  else 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SD ERRO X");  
  } 
  //AjustarRelogio();
}
void loop()        
{
  int chk = DHT.read22(DHT03_PIN);
  delay(500);//Tempo minimo para cada Leitura
  
  switch (chk)
  {
  case DHTLIB_OK: 

    TempAtual=float(DHT.temperature);
    UrAtual=float(DHT.humidity);
    Processamento();
    relogio();
    if(digitalRead(Botao1)==LOW)
    {
      op++;
      if(op>5)
      {
        op=1;
      }
    }
    menu();
    if(millis()-tfinal>1800000)
    {
      tfinal=millis();
      GravarNoSd();
    }
    if(millis()-tfinal<0)
    {
      tfinal=millis();
    }
    
    break;
  case DHTLIB_ERROR_CHECKSUM: 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ChecksumErro!");
    delay(500);
    break;
  case DHTLIB_ERROR_TIMEOUT: 
    lcd.clear();
    lcd.setCursor(0,0) ;
    lcd.print("DHT TimeOUT!");
    delay(500);
    break;
  default: 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DHT ERRO!");
    delay(500);
    break;
  }

}



