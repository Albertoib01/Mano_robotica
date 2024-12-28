const express = require('express');
const axios = require('axios'); // Para realizar solicitudes HTTP al ESP32
const path = require('path');
const app = express();
const port = 8000;

// Dirección IP del ESP32 (ajusta esta dirección según la IP de tu ESP32)
const ESP32_IP = 'http://192.168.1.21';

// Servir el HTML desde el servidor Express
app.use(express.static(path.join(__dirname)));


// Rutas para controlar los servos
app.get('/mano_cerrada', async (req, res) => {
  try {
    await axios.get(`${ESP32_IP}/mano_cerrada`);
    console.log('Comando enviado: Mano cerrada');
  } catch (error) {
    console.error('Error enviando comando:', error);
    res.status(500).send('Error enviando comando al ESP32');
  }
});

app.get('/mano_abierta', async (req, res) => {
  try {
    await axios.get(`${ESP32_IP}/mano_abierta`);
    console.log('Comando enviado: Mano abierta');
  } catch (error) {
    console.error('Error enviando comando:', error);
    res.status(500).send('Error enviando comando al ESP32');
  }
});

app.get('/dedo_1a1', async (req, res) => {
  try {
    await axios.get(`${ESP32_IP}/dedo_1a1`);
    console.log('Comando enviado: Mover dedos uno a uno');
  } catch (error) {
    console.error('Error enviando comando:', error);
    res.status(500).send('Error enviando comando al ESP32');
  }
});

// Servidor escuchando
app.listen(port, () => {
  console.log(`Servidor Express escuchando en http://localhost:${port}`);
});
