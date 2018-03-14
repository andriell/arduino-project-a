#define GRAPH_SIZE 128
short graph[GRAPH_SIZE];
byte graphI = 0;
byte graphZoom = 4;
short graphMin = 2000;
unsigned long graphTime = 0;

void graphMenu() {
  // Начало график
  byte x, y;
  short graphMax = graphMin + 64 / graphZoom * 100;
  for (byte i = 0; i < GRAPH_SIZE; i++) {
    if (i > graphI) {
      x = GRAPH_SIZE - i + graphI;
    } else {
      x = graphI - i;
    }
    x += 16;
    y = map(graph[i], graphMin, graphMax, 63, 0);
    oledPixel(x, y);
  }
  // Конец график
  // Начало сетка
  short dt = 500;
  if (graphZoom == 1) {
    dt = 1000;
  } else if (graphZoom == 2) {
    dt = 500;
  } else if (graphZoom == 2) {
    dt = 500;
  } else if (graphZoom == 4) {
    dt = 500;
  } else if (graphZoom == 8) {
    dt = 200;
  } else {
    dt = 100;
  }
  short t;
  if (graphMin % dt == 0) {
    t = graphMin;
  } else {
    t = graphMin - graphMin % dt + dt;
  }
  for (; t < graphMax; t += dt) {
    for (byte i = 16; i < 128; i += 5) {
      y = map(t, graphMin, graphMax, 63, 0);
      oledPixel(i, y);
      oledPrintInt((t / 100) % 100, 0, y + 4, 0);
    }
  }
  // Конец сетка
  // Начало текст
  //oled.drawRect(0, 0, 127, 9, true, false);
  oledPrint("t", 0, 7, 0);
  oledPrintFloat(((float) graph[graphI]) / 100, 6, 7, 0);
  oledPrint("min", 50, 7, 0);
  oledPrintFloat(((float) graphMin) / 100, 69, 7, 0);
  oledPrint("x", 105, 7, 0);
  oledPrintInt(graphZoom, 111, 7, 0);
  // Конец текст

  // -- Начало контроля
  short a = 0;
  if (graphZoom == 1) {
    a = 400;
  } else if (graphZoom == 2) {
    a = 200;
  } else if (graphZoom == 4) {
    a = 100;
  } else {
    a = 50;
  }
  if (bitRead(jButtons, 10)) {
    graphMin -= a;
  }
  if (bitRead(jButtons, 12)) {
    graphMin += a;
  }
  if (bitRead(jButtons, 11) && graphZoom < 16) {
    graphMin += 64 / graphZoom * 25;
    graphMin -= graphMin % 100;
    graphZoom = graphZoom * 2;
  }
  if (bitRead(jButtons, 13) && graphZoom > 1) {
    graphZoom = graphZoom / 2;
    graphMin -= 64 / graphZoom * 25;
    graphMin -= graphMin % 100;
  }
  if (bitRead(jButtons, 14)) {
    menuOpen(255);
  }
  // -- Конец контроля
}

void graphPut(float t) {
  unsigned long m = millis();
  if (m - graphTime < 60000) {
    return;
  }
  graphTime = m;
  graphI++;
  if (graphI >= GRAPH_SIZE) {
    graphI = 0;
  }
  graph[graphI] = (int) (t * 100);
}

