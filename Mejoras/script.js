const express = require('express');
const axios = require('axios');
const path = require('path');
const app = express();
const port = 8000;

const ESP32_IP = 'http://192.168.1.27';

let estadoActual = 'Mano abierta';

app.use(express.static(path.join(__dirname)));

app.get('/estado', (req, res) => {
  res.send(estadoActual);
});

app.get('/mano_cerrada', async (req, res) => {
  try {
    await axios.get(`${ESP32_IP}/mano_cerrada`);
    estadoActual = 'Mano cerrada';
    console.log('Comando enviado: Mano cerrada');
    res.send('Mano cerrada');
  } catch (error) {
    console.error('Error enviando comando:', error);
    res.status(500).send('Error enviando comando al ESP32');
  }
});

app.get('/mano_abierta', async (req, res) => {
  try {
    await axios.get(`${ESP32_IP}/mano_abierta`);
    estadoActual = 'Mano abierta';
    console.log('Comando enviado: Mano abierta');
    res.send('Mano abierta');
  } catch (error) {
    console.error('Error enviando comando:', error);
    res.status(500).send('Error enviando comando al ESP32');
  }
});

app.get('/dedo_1a1', async (req, res) => {
  try {
    await axios.get(`${ESP32_IP}/dedo_1a1`);
    estadoActual = 'Mover dedos uno a uno';
    console.log('Comando enviado: Mover dedos uno a uno');
    res.send('Mover dedos uno a uno');
  } catch (error) {
    console.error('Error enviando comando:', error);
    res.status(500).send('Error enviando comando al ESP32');
  }
});

app.get('/servo:servo', async (req, res) => {
  const { servo } = req.params;
  const { pos } = req.query;
  try {
    await axios.get(`${ESP32_IP}/servo${servo}?pos=${pos}`);
    console.log(`Comando enviado: Mover servo ${servo} a posición ${pos}`);
    res.send(`Servo ${servo} movido a posición ${pos}`);
  } catch (error) {
    console.error('Error enviando comando:', error);
    res.status(500).send('Error enviando comando al ESP32');
  }
});

// Nueva ruta para manejar la solicitud de la palabra
app.get('/palabra', async (req, res) => {
  const { texto } = req.query;
  try {
    await axios.get(`${ESP32_IP}/palabra?texto=${texto}`);
    console.log(`Comando enviado: Deletrear palabra ${texto}`);
    res.send(`Palabra ${texto} enviada`);
  } catch (error) {
    console.error('Error enviando comando:', error);
    res.status(500).send('Error enviando comando al ESP32');
  }
});

app.listen(port, () => {
  console.log(`Servidor Express escuchando en http://localhost:${port}`);
});