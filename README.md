# Jeti App Manager

Der Jeti App Manager listet alle online verfügbaren LUA Apps übersichtilich auf, und dies können dann 
einfach und schnell auf dem Jeti DC/DS Sender installiert werden. Einfach App auswählen, oben links die SD-Karte des Senders anwählen, 
und auf installieren drücken - Fertig !

![preview](https://github.com/nightflyer88/JetiAppManager/blob/master/images/JetiAppManager.png?raw=true)

Der Jeti App Manager wird auf dem localen Computer installiert, danach werden in den Einstellungen die Quellen zu den LUA-Apps angegeben. 
Der Jeti App Manager speichert keine Daten local, somit wird garantiert, dass die LUA-Apps immer auf dem neusten Stand sind.

### Quelldatei

Jeder App Entwickler der seine Apps anbieten möchte, muss ein Link zu einer Quelldatei seiner Apps bereitstellen. Die Quelldatei kann zB. 
im App Repository auf GIT liegen oder sonst wo im Web. Der Link dazu muss einfach direkt sein und darf nicht umgeleitet sein.

Die Datei muss im *.json Format sein. Mit diesem online [Tool](http://jsoneditoronline.org) ist die Quelldatei einfach und schnell erstellt.

Die Quelldatei muss wie folgt struckturiert sein, und enthält dann alle informationen und links zur App:

```
{
  "GPS to QR-Code": {
    "author": "M. Lehmann",
    "version": "V1.1",
    "previewImg": "https://raw.githubusercontent.com/nightflyer88/Lua_gpsQRcode/master/img/qpsQRcode.jpg",
    "description": "https://raw.githubusercontent.com/nightflyer88/Lua_gpsQRcode/master/README.md",
    "sourceFile": [
      "https://raw.githubusercontent.com/nightflyer88/Lua_gpsQRcode/master/gpsQRcode.lc",
      "https://raw.githubusercontent.com/nightflyer88/Lua_gpsQRcode/master/gpsQRcode/gpsQRcode.jsn"
    ],
    "destinationPath": [
      "/Apps",
      "/Apps/gpsQRcode"
    ]
  },
  "Artificial Horizon": {
    "author": "JETI model",
    "version": "V1.0",
    "previewImg": "https://raw.githubusercontent.com/JETImodel/Lua-Apps/master/Img/Artificial%20Horizon/Horizon6.png",
    "description": "https://raw.githubusercontent.com/JETImodel/Lua-Apps/master/Artificial%20Horizon/README.md",
    "sourceFile": [
      "https://raw.githubusercontent.com/JETImodel/Lua-Apps/master/Artificial%20Horizon/Horizon.lua"
    ],
    "destinationPath": [
      "/Apps"
    ]
  }
}
```

Kurze Erklärung der einzelnen Einträge:
```
{
  "GPS to QR-Code": {                     <-- Name der App
    "author": "M. Lehmann",               <-- Name des Authors
    "version": "V1.1",                    <-- aktuelle Version
    "previewImg": "https://raw.git...",   <-- Link zum Vorschau Bild der App
    "description": "https://raw.git...",  <-- Link zur Beschreibung (muss eine MarkDown *.md Datei sein)
    "sourceFile": [                       <-- Liste der Dateien, die auf den Sender kopiert werden
      "https://raw.git...",               <-- erste Datei
      "https://raw.git..."                <-- zweite Datei
    ],
    "destinationPath": [                  <-- Liste der Ziel Pfade (absolut) auf dem Sender
      "/Apps",                            <-- erste Datei wird in diesen Ordner kopiert
      "/Apps/gpsQRcode"                   <-- zweite Datei wird in diesen Ordner kopiert
    ]
  }
}
```
Die Quelldatei kann mehrere Apps enthalten.

Ist der Zielpfad auf dem Sender nicht verhanden, wird er erstellt.

Alle Links zu den Dateien müssen direkt sein. Auch die Bilder die in der Beschreibung (*.md Datei) verlinkt sind müssen 
direkt zur Datei führen und dürfen nicht weiter- oder umgeleitet sein.

Beispiel Links:

Dieser Link ist in Ordnung: https://raw.githubusercontent.com/JETImodel/Lua-Apps/master/Img/Artificial%20Horizon/Horizon6.png

Dieser Link funktioniert nicht: https://github.com/nightflyer88/JetiAppManager/blob/master/images/JetiAppManager.png?raw=true

Man erkennt den umgeleiteten Link an **?raw=true**
