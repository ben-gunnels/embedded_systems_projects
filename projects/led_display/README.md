
<h1 align="center">
  <br>
  <img src="assets/LedLogo.png" alt="Markdownify" width="200">
  <br>
  <br>
  LED Matrix
  <br>
</h1>

<h4 align="center">Interacting with a WS2812 Neopixel LED board using <a href="https://www.lf-lang.org/" target="_blank">Lingua Franca</a>.</h4>

<br>

<div style="text-align: center;">
  <iframe width="600" height="338" 
          src="https://www.youtube.com/embed/ed-fL5peNKo" 
          frameborder="0" 
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" 
          allowfullscreen>
  </iframe>
</div>


## About the Project

LedDisplay.lf uses a modal model to toggle LED animations which are projected onto a WS2812 Neopixel LED array board. The program depends on Programmable I/O (PIO) to interface with the hardware on the LED array using sub-microsecond timing precision. 

1. **Hardware Required**
    - Raspberry Pi Pico Microcontroller
        - https://www.adafruit.com/pico?src=raspberrypi
    - Breadboard
        - 7 Jumper Wires, explained below
        - 220 ohm - 330 ohm resistor for LED din connection
        - Button Switch to change modes
    - WS2812 Neopixel LED Array Board
        - https://www.amazon.com/dp/B07PB2P81N/ref=twister_B07P5TNCHP?_encoding=UTF8&th=1
2. **Setting up the Hardware**
<table>
    <tr>
        <td> <img src="assets/Breadboard.jpg" alt="Breadboard" width="800">
    </tr>
    <tr>
        <td> An example circuit used to connect the WS2812 LED array using a breadboard and the Pico microcontroller. To see valid GPIO pins for the LED Array refer to the Raspberry Pi Pico datasheet. Ensure that your pins are mapped correctly in <a href="lib/src/pins.h">pins.h</a> for BUTTON and ws2812.</td>
    </tr>
</table>

The goal is to use a modal model to control a set of LED animations and to use a timer to poll for mode changes via a button on a circuit. 

## How To Use

To clone and run this application, you'll need [Git](https://git-scm.com) and [Lingua Franca](https://www.lf-lang.org/docs/installation) installed on your computer. From your command line:

```bash
# Clone this repository
$ git clone https://github.com/lf-lang/playground-lingua-franca.git

# Go into the example folder of the repository
$ cd playground-lingua-franca/examples/C

# Compile the main reactor
$ lfc src/led-matrix/LedMatrix.lf

# Run the program
$ bin/LedMatrix.elf
```

> **Note:**
> Or you can compile and run the program simply using VS Code, [see section "Running Locally"](https://github.com/lf-lang/playground-lingua-franca).


## Component

<table>
<tr>
<td> <img src="assets/LedMatrix.png" alt="LedMatrix" width="100">
<td> <a href="LedMatrix.lf">LedMatrix.lf</a>: LED contains the modes for the LED matrix. It generates the image patterns and sends the data to the light board.</td>
</tr>
</table>

## Further Discussion
Initially, the LED matrix driver was based on an existing solution that I adapted to suit the needs of this project. The first implementation, in C, performed reliably and communicated effectively with the WS2812 LED panel.

However, issues arose after translating the C implementation into a lingua franca program using reactors. Despite seeming to maintain logical equivalence, the driver began to fail. After extensive debugging, it became apparent that the problem stemmed from disruptions in the timing protocol required by the WS2812 LEDs. These LEDs rely on extremely precise timing to differentiate between logical high and low signals, with tolerances on the order of a few hundred nanoseconds.

Achieving this level of timing precision in software alone proved unreliable and impractical. To address this, I transitioned to using the Raspberry Pi Pico's Programmable I/O (PIO) subsystem for signal generation. The PIO allows for deterministic, low-level control over signal timing and is well-suited for protocols with strict temporal requirements like that of the WS2812.

Fortunately, the official pico-examples repository includes a PIO implementation for driving WS2812 LEDs. By integrating this into the project, I was able to restore reliable operation and ensure consistent communication with the LED matrix.

If you have any insight on why the lingua franca might mess with the timing protocol of my original driver I would love to hear about it. 

For any inquiries on this project please email me at: bengunnels8@gmail.com


