# Jeti App Manager

Der Jeti App Manager listet alle online verfügbaren LUA Apps übersichtilich auf, und können dann 
einfach und schnell auf den Jeti DC/DS Sender installiert werden. Die App einfach und bequem auswählen, oben links die SD-Karte des Senders wählen, 
und auf installieren drücken - Fertig !

![preview](https://github.com/nightflyer88/JetiAppManager/blob/master/images/JetiAppManager.png?raw=true)

### Funktionsweise

Der Jeti App Manager wird auf dem lokalen Computer (Mac oder Windows) installiert, die LUA-Apps werden durch eine Quelldatei eingebunden, so kann der Anwender selber bestimmen welche Apps aus welchen Quellen er haben möchte. 
Die Apps sind jedoch nicht lokal gespeichert, sondern werden erst bei der installation auf den Sender aus dem Internet geladen. Dadurch wird garantiert, dass der Anwender immer auf dem neusten Software Stand ist. 

Unter *Einstellungen* können weiter App-Quellen definiert werden. Es muss lediglich ein Link zu dieser *.json Quelldatei eingefügt werden (pro Zeile ein Link). Dieser Link zu der *.json Datei muss der App-Entwickler bereitstellen. Da die Quelldatei ebenfalls nicht lokal gespeichert ist, werden so immer die neusten App Versionen geladen, auch neue Apps von dem App-Entwickler werden automatisch angezeigt, ohne das man etwas aktuallisieren muss. 

### Installation

Neuste Version des Jeti App Manager für Mac oder Windows hier herunterladen: https://github.com/nightflyer88/JetiAppManager/releases

#### Mac

JetiAppManager.dmg herunterladen und öffnen, nun die JetiAppManager.app per Drag&Drop in den Programm Ordner ziehen.

#### Windows

JetiAppManager_setup.exe herunterladen, ausführen und den Anweisungen folgen. 

### Quelldatei

Jeder App Entwickler der seine Apps anbieten möchte, muss eine *.json Quelldatei erstellen. In der *.json Quelldatei sind dann alle nötigen Informationen zur App enthalten, unter anderem: benötigte Dateien, Ziel Pfade auf dem Sender, minimale Hard- und Softwareanforderungen des Senders, App-Beschreibung, Vorschaubild, usw.

Die Quelldatei kann zB. im App Repository auf GIT liegen oder sonst wo im Web. Der Anwender braucht dann nur einen Link zu der *.json Datei. Dieser Link kann nun einfach unter *Einstellungen* auf einer neuen Zeile eingefügt werden. Der Link muss direkt zur Datei führen und darf nicht umgeleitet sein. 

Beispiel Link:
```
https://raw.githubusercontent.com/nightflyer88/JetiAppManager/master/default.json
```

Die Quell-Datei muss im *.json Format sein. Mit diesem online [Tool](http://jsoneditoronline.org) ist die Quelldatei einfach und schnell erstellt, und wird auch auf Syntaxfehler geprüft.

Die Quelldatei kann mehrere Apps enthalten und muss wie in den folgenden Beispielen struckturiert sein:

Beispiel für mehrere Apps pro Quelldatei:
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
Die einzelnen Apps werden in der JSON Datei als Object{} definiert. Der Object-Name entspricht dem App Name. In einer JSON Datei gibt es keine Begrenzung der maximal anzahl Apps.


Beispiel für eine App die auf jedem Sender läuft:
```
{
  "GPS to QR-Code": {                                   <-- Name der App
    "author": "M. Lehmann",                             <-- Name des Authors
    "version": "V1.1",                                  <-- aktuelle Version
    "previewImg": "https://raw.git.../qpsQRcode.jpg",   <-- Link zum Vorschau Bild der App, kann im *.jpeg, *.png oder *.bmp Format sein
    "description": "https://raw.git.../README.md",      <-- Link zur Beschreibung, muss im MarkDown Format sein (*.md Datei, zB auf git die README.md)
    "sourceFile": [                                     <-- Liste der Dateien, die auf den Sender kopiert werden
      "https://raw.git.../gpsQRcode.lc",                <-- erste Datei
      "https://raw.git.../gpsQRcode/gpsQRcode.jsn"      <-- zweite Datei
    ],
    "destinationPath": [                                <-- Liste der Ziel Pfade (absolut) auf dem Sender
      "/Apps",                                          <-- erste Datei wird in diesen Ordner kopiert
      "/Apps/gpsQRcode"                                 <-- zweite Datei wird in diesen Ordner kopiert
    ]
  }
}
```
Sollte die App auf jeden Sendertyp installiert werden können, so kann die Liste der App-Dateien mit dem Key-Wort _**sourceFile**_ definiert werden. Sind die Zielpfade auf dem Sender nicht verhanden, werden diese erstellt. Bei der Deinstallation werden leere Ordner im /Apps Verzeichnis automatisch gelöscht.

Beispiel für eine App, die die Senderfirmware 4.23 oder höher voraussetzt, und verschiedene App-Versionen für DC/DS14-16 oder DC/DS24 enthält.
```
{
  "Battery Percentage": {                               <-- Name der App
    "author": "RC-Thoughts",                            <-- Name des Authors
    "version": "V2.4",                                  <-- aktuelle App-Version
    "previewImg": "https://.../Jeti-Tools_small.png",   <-- Link zum Vorschau Bild der App, kann im *.jpeg, *.png oder *.bmp Format sein
    "description": "https://.../README.md",             <-- Link zur Beschreibung, muss im MarkDown Format sein (*.md Datei, zB auf git die README.md)
    "requiredFirmware": 4.23,                           <-- erforderliche minimale Senderfirmware
    "sourceFile14_16": [                                <-- Liste der Dateien, die auf den Sender kopiert werden, wenn der Sendertyp DC/DS14-16 ist
      "https://raw.git.../DCDS-1416/RCT-Batt.lc",
      "https://raw.git.../DCDS-1416/Lang/RCT-Batt.jsn"
    ],
    "sourceFile24": [                                   <-- Liste der Dateien, die auf den Sender kopiert werden, wenn der Sendertyp DC/DS24 ist
      "https://raw.git.../DCDS-24/RCT-Batt.lc",
      "https://raw.git.../DCDS-24/Lang/RCT-Batt.jsn"
    ],
    "destinationPath": [                                <-- Liste der Ziel Pfade (absolut) auf dem Sender
      "/Apps",
      "/Apps/Lang"
    ]
  }
}
```
Sollte die App nur auf einer DC/DS24 installiert werden können, so wird die Dateiliste mit dem Key-Wort _**sourceFile24**_ definiert. Für die Sendertypn DC/DS14-16 wird _**sourceFile14_16**_ definiert. Die minimale Senderfirmware wird mit dem Key-Wort _**requiredFirmware**_ und als Double-Zahl definiert.

Beispiel für eine mehrsprachige App:
```
{
  "LUA App deutsch": {                                  <-- standart Name der App (hier in Deutsch)
    "author": "M. Lehmann",                             # Standart informationen der App
    "version": "V1.1",                                  # ist keine andere Sprache vorhanden werden dies informationen verwendet
    "previewImg": "https://raw.git.../qpsQRcode.jpg",
    "description": "https://raw.git.../README.md",
    "requiredFirmware": 4.23,
    "sourceFile24": [
      "https://raw.git.../gpsQRcode.lc"
    ],
    "destinationPath": [
      "/Apps"
    ],
    "en": {                                             # ist diese Sprache (Englisch) eingestellt werden die App Informationen überschrieben
      "appName": "LUA App english",                     <-- neuer Name der App in Englisch
      "description": "https://raw.git.../README_en.md"  <-- andere Datei für die Beschreibung 
    },
    "cz": {
      "appName": "LUA App český",
      "description": "https://raw.git.../README_cz.md",
      "previewImg": "https://raw.git.../cz.jpg"         <-- anderes Vorschaubild für Tschechisch
    },
    "it": {
      "description": "https://raw.git.../README_it.md"  <-- hier wird nur eine andere Beschreibung definiert, App Name bleibt gleich
    }
  }
}
```

Durch die Sprachobjekte können die Standart App Informationen durch andere überschrieben werden. Es kann zB. ein anderer App Name, eine andere (übersetzte) Beschreibung, usw.. definiert werden. Es ist möglich, alle Standart Informationen zu überschreiben, auch die _**sourceFile**_ und _**destinationPath**_ Objekte.

Momentan werden die folgenden Sprachen unterstützt:

| Sprache      | Kürzel |
|--------------|--------|
| Deutsch      | de     |
| Englisch     | en     |
| Tschechisch  | cz     |
| Französisch  | fr     |
| Italienisch  | it     |
| Spanisch     | es     |
| Portugisisch | pt     |


Alle Links zu den Dateien müssen direkt sein. Auch die Bilder die in der Beschreibung (*.md Datei) verlinkt sind müssen 
direkt zur Datei führen und dürfen nicht weiter- oder umgeleitet sein.

Beispiel Links:

Dieser Link wird akzeptiert: https://raw.githubusercontent.com/JETImodel/Lua-Apps/master/Img/Artificial%20Horizon/Horizon6.png

Dieser Link funktioniert nicht: https://github.com/nightflyer88/JetiAppManager/blob/master/images/JetiAppManager.png?raw=true

Man erkennt den umgeleiteten Link an **?raw=true**
