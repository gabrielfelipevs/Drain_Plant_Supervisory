
#include "DHT.h"
#include <Modbusino.h>

/************************************************
  ARDUINO - COMUNICAÇÃO SERIAL (MODBUS)
  https://github.com/stephane/modbusino
**************************************************/
/* CONSTANTES */
# define  DHTPIN  A1      // Pino digital conectado ao sensor DHT
#define DHTTYPE DHT11
// Conecte pino 1 do sensor (esquerda) ao +5V
// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino
// Conecte pino 4 do sensor ao GND
// Conecte o resistor de 10K entre pin 2 (dados)
// e ao pino 1 (VCC) do sensor
DHT dht(DHTPIN, DHTTYPE);

const int SENSOR_PIN = A0; /* SENSOR ANALÓGICO - POTENCIÔMETRO */
const int LED_PIN = 8; /* ATUADOR ON/OFF - LED */

/* INICIALIZA O ID DO DISPOSITIVO. NESTE CASO, ADDR = 20 */
ModbusinoSlave modbusino_slave(10);
/* ALOCA O NÚMERO DE REGISTRADORES */
uint16_t tab_reg[10];
/* VARIÁVEIS */
int leitura = 0;
int command = 0;
int umidade = 0;
int temperatura = 0;
void setup()
{
  dht.begin();
  /* TAXA DE TRANSMISSÃO - BAUD RATE */
  modbusino_slave.setup(9600);
  /* CONFIGURACAO DO GPIO */
  pinMode(LED_PIN, OUTPUT);
}
void loop() {
  while (1)
  {

    umidade = dht.readHumidity();
    temperatura = dht.readTemperature();
    /* LENDO O VALOR ANALÓGICO - POTENCIÔMETRO */
    leitura = analogRead(SENSOR_PIN);
    //PASSANDO A LEITURA DO SENSOR PARA O MODBUS
    tab_reg[0] = leitura;
    tab_reg[2] = umidade;
    tab_reg[3] = temperatura;
    //VERIFICACAO DA VARIÁVEL COMMAND - RECUPERANDO O VALOR DO MODBUS
    command = tab_reg[1];
    if (command <= 0) digitalWrite(LED_PIN, LOW);
    if (command >= 1) digitalWrite(LED_PIN, HIGH);
    // ATUALIZANDO O MODBUS
    modbusino_slave.loop(tab_reg, 10);
  }
}
