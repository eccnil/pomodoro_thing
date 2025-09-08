# Contador

Este proyecto permite fabricar un dispositivo creando una placa pcb, su circuito y su caja

![render](https://files.mastodon.social/media_attachments/files/115/028/003/383/688/009/small/37454dcc8c4b1f6a.png)

El resultado es un aparato que puede servir como contador, como reloj de cocina o como metrónomo.
Aunque su funcionalidad no es su proprosito, se han añadido estas para dar vistosidad.

Conultese el [manual de operación](ManualUsuario.md) para saber como
operar el producto final

## Organización

Existen 3 carpetas, que recogen las principales partes de este proyecto:

| Carpeta          | Descripción              | Uso        |
| ---------------- | ------------------------ | ---------- |
| [shema](shema)   | Creación de la placa     | KiCad 9    |
| [case](case)     | Carcasa exterior         | Freecad 1  |
| [codigo](codigo) | Software (c++ / Arduino) | PlatformIO |
