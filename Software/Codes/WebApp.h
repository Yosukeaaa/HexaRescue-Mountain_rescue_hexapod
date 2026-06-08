#pragma once

static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8"/>
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"/>
<title>HexaRescue</title>
<style>
:root{
  --bg:#0d1117;--panel:#161b22;--border:#30363d;
  --accent:#f97316;--cam:#38bdf8;
  --text:#e6edf3;--muted:#8b949e;
  --ok:#22c55e;--warn:#eab308;--danger:#ef4444;--r:12px;
}
*{box-sizing:border-box;margin:0;padding:0;-webkit-tap-highlight-color:transparent}
html,body{background:var(--bg);color:var(--text);font-family:'Courier New',monospace;min-height:100vh}
#app{display:flex;flex-direction:column;padding:10px;gap:8px;max-width:480px;margin:0 auto;padding-bottom:75px}

/* HEADER */
#header{display:flex;align-items:center;justify-content:space-between;
        background:var(--panel);border:1px solid var(--border);
        border-radius:var(--r);padding:11px 14px;}
.logo{font-size:18px;font-weight:700;letter-spacing:3px;color:var(--accent)}
.logo span{color:var(--text)}
#gps-mini{font-size:8px;color:var(--muted);margin-top:4px;letter-spacing:1px}
#ws-pill{display:flex;align-items:center;gap:7px;font-size:10px;letter-spacing:1px;color:var(--muted)}
#ws-led{width:9px;height:9px;border-radius:50%;background:var(--danger);transition:background .3s}
#ws-led.ok{background:var(--ok);animation:pulse 2s infinite}
@keyframes pulse{0%{box-shadow:0 0 0 0 #22c55e88}70%{box-shadow:0 0 0 7px #22c55e00}100%{box-shadow:0 0 0 0 #22c55e00}}

/* ETATS */
#states{display:flex;justify-content:space-around;align-items:center;
        background:var(--panel);border:1px solid var(--border);
        border-radius:var(--r);padding:12px 8px;}
.state-item{display:flex;flex-direction:column;align-items:center;gap:5px;min-width:60px}
.led{width:13px;height:13px;border-radius:50%;background:#1c2128;border:1px solid var(--border);transition:all .3s}
.led.green{background:var(--ok);border-color:var(--ok);box-shadow:0 0 8px var(--ok)}
.led.yellow{background:var(--warn);border-color:var(--warn);box-shadow:0 0 8px var(--warn)}
.led.red{background:var(--danger);border-color:var(--danger);box-shadow:0 0 8px var(--danger)}
.state-label{font-size:9px;letter-spacing:1px;color:var(--muted);text-transform:uppercase}
.state-val{font-size:10px;font-weight:700;color:var(--text)}
.sep{width:1px;height:48px;background:var(--border)}

/* BOUTONS */
#controls{display:grid;grid-template-columns:1fr 1fr 1fr;gap:8px}
.btn{padding:13px 4px;border-radius:var(--r);border:1px solid var(--border);
     background:#1c2128;color:var(--text);font-family:inherit;font-size:12px;
     font-weight:700;letter-spacing:1px;cursor:pointer;transition:all .12s;}
.btn:active{transform:scale(0.95)}
.btn.start{border-color:var(--danger);color:var(--danger)}
.btn.start:active{background:#450a0a}
.btn.neutral{border-color:var(--warn);color:var(--warn)}
.btn.neutral:active{background:#422006}
.btn.stop{border-color:var(--ok);color:var(--ok)}
.btn.stop:active{background:#14532d}

/* PANELS */
.panel{background:var(--panel);border:1px solid var(--border);border-radius:var(--r);padding:12px}
.pt{font-size:9px;letter-spacing:2px;color:var(--muted);text-transform:uppercase;margin-bottom:10px}

/* PADS */
#pads-row{display:grid;grid-template-columns:1fr 1fr;gap:8px}
.dpad{display:grid;grid-template-columns:repeat(3,56px);grid-template-rows:repeat(3,56px);
      gap:5px;margin:0 auto;width:fit-content;}
.arrow{background:#1c2128;border:1px solid var(--border);border-radius:var(--r);
       display:flex;align-items:center;justify-content:center;
       font-size:22px;cursor:pointer;transition:all .1s;
       user-select:none;-webkit-user-select:none;touch-action:none;}
.arrow.dir.pressed,.arrow.dir:active{background:#f9731622;border-color:var(--accent);color:var(--accent);transform:scale(0.91)}
.arrow.cam.pressed,.arrow.cam:active{background:#38bdf822;border-color:var(--cam);color:var(--cam);transform:scale(0.91)}
.arrow.empty{background:transparent;border:none;pointer-events:none}
.cam-pos{display:flex;justify-content:space-between;margin-top:8px;font-size:9px;color:var(--muted)}
.cam-pos span{color:var(--cam);font-weight:700;font-size:10px}
.btn-center{width:100%;margin-top:6px;padding:6px;border-radius:8px;
            border:1px solid var(--cam);background:transparent;
            color:var(--cam);font-family:inherit;font-size:10px;
            letter-spacing:1px;cursor:pointer;transition:all .1s;}
.btn-center:active{background:#0c2a3a;transform:scale(0.97)}

/* SLIDERS */
.slider-row{display:flex;align-items:center;gap:8px;margin-bottom:10px}
.slider-row:last-child{margin-bottom:0}
.sl{font-size:10px;color:var(--muted);width:88px;flex-shrink:0;line-height:1.3}
.slider-row input[type=range]{flex:1;accent-color:var(--accent)}
.sv{font-size:10px;color:var(--accent);width:54px;text-align:right;flex-shrink:0}

/* POPUP */
#popup-overlay{display:none;position:fixed;inset:0;background:#000000cc;
               z-index:100;align-items:center;justify-content:center;padding:20px;}
#popup-overlay.visible{display:flex}
#popup{background:var(--panel);border:2px solid var(--danger);border-radius:var(--r);
       padding:24px 20px;width:100%;max-width:380px;
       box-shadow:0 0 30px #ef444444;animation:popIn .2s ease;}
@keyframes popIn{from{transform:scale(0.85);opacity:0}to{transform:scale(1);opacity:1}}
.popup-header{display:flex;align-items:center;gap:10px;margin-bottom:16px}
.popup-icon{font-size:28px;animation:blink 1s infinite}
@keyframes blink{0%,100%{opacity:1}50%{opacity:0.3}}
.popup-title{font-size:15px;font-weight:700;color:var(--danger);letter-spacing:1px}
.popup-sub{font-size:10px;color:var(--muted);margin-top:2px;letter-spacing:1px}
.popup-body{background:#1c2128;border:1px solid var(--border);border-radius:8px;
            padding:12px;margin-bottom:16px;}
.popup-line{display:flex;justify-content:space-between;align-items:center;margin-bottom:6px}
.popup-line:last-child{margin-bottom:0}
.popup-key{font-size:10px;color:var(--muted);text-transform:uppercase;letter-spacing:1px}
.popup-val{font-size:11px;color:var(--text);font-weight:700}
.popup-val.gps{color:var(--accent)}
#popup-ok{width:100%;padding:12px;border-radius:var(--r);
          border:1px solid var(--danger);background:#2d1a1a;
          color:var(--danger);font-family:inherit;font-size:12px;
          font-weight:700;letter-spacing:2px;cursor:pointer;transition:all .12s;}
#popup-ok:active{background:#450a0a;transform:scale(0.97)}

/* VICTIMS */
#victims-panel{display:none}
#victims-panel.visible{display:block}
.victim-row{display:flex;align-items:center;gap:8px;padding:8px 10px;border-radius:8px;
            background:#1c2128;border:1px solid var(--border);margin-bottom:6px;}
.victim-row:last-child{margin-bottom:0}
.victim-num{font-size:11px;font-weight:700;color:var(--danger);
            background:#2d1a1a;border:1px solid var(--danger);
            border-radius:6px;padding:2px 7px;flex-shrink:0;}
.victim-info{display:flex;flex-direction:column;gap:2px;flex:1}
.victim-time{font-size:10px;color:var(--muted)}
.victim-gps{font-size:10px;color:var(--accent)}

/* FOOTER EMOTES */
#footer{
  position:fixed;bottom:0;left:0;right:0;
  background:var(--panel);border-top:1px solid var(--border);
  padding:10px 14px;display:flex;gap:8px;justify-content:center;
  z-index:50;
}
.emote-btn{
  padding:10px 18px;border-radius:var(--r);
  border:1px solid var(--border);background:#1c2128;
  color:var(--text);font-family:inherit;font-size:13px;
  cursor:pointer;transition:all .12s;letter-spacing:1px;
}
.emote-btn:active{transform:scale(0.93)}
.emote-btn.hello{border-color:#a78bfa;color:#a78bfa}
.emote-btn.hello:active{background:#1e1030}
.emote-btn.twist{border-color:#a78bfa;color:#a78bfa}
.emote-btn.twist:active{background:#1e1030}
.emote-btn.jump{border-color:#a78bfa;color:#a78bfa}
.emote-btn.jump:active{background:#1e1030}


/* Barre de modes visualisation */
#view-bar{display:flex;gap:6px;margin-bottom:10px}
.vbtn{flex:1;padding:8px 4px;border-radius:8px;border:1px solid var(--border);
      background:#1c2128;color:var(--muted);font-family:inherit;font-size:10px;
      letter-spacing:1px;cursor:pointer;transition:all .12s}
.vbtn.active{border-color:var(--accent);color:var(--accent);background:#1c1208}
.vbtn:active{transform:scale(0.95)}



/* FULLSCREEN */
#fs-btn{position:absolute;top:8px;right:8px;background:#1c2128cc;border:1px solid var(--border);
        border-radius:6px;padding:4px 7px;font-size:14px;cursor:pointer;z-index:10;
        color:var(--text);transition:all .12s}
#fs-btn:active{transform:scale(0.9)}
#fs-overlay{
  display:none;
  position:fixed;
  top:0; left:0;
  width:100vw;
  height:100vh;
  height:100dvh;
  margin:0; padding:0;
  background:#000;
  z-index:9999;
  overflow:hidden;
}
#fs-overlay.visible{display:block}

/* Contenu interne tourné si portrait */
@media (orientation: portrait) {
  #fs-overlay.visible #fs-main,
  #fs-overlay.visible #fs-topbar,
  #fs-overlay.visible #fs-bottom,
  #fs-overlay.visible #fs-hud {
    transform: rotate(90deg);
    transform-origin: center center;
    position: fixed;
    width: 100vh;
    height: 100vw;
    top: calc(50% - 50vw);
    left: calc(50% - 50vh);
  }
}

/* Cam plein écran derrière tout */
#fs-main{position:absolute;inset:0;}
#fs-main img, #fs-main canvas{position:absolute;top:0;left:0;width:100%;height:100%;object-fit:cover}
#fs-main canvas{image-rendering:pixelated}
#fs-fusion-overlay{position:absolute;top:0;left:0;width:100%;height:100%;opacity:0.45;image-rendering:pixelated}

/* Barre du haut par dessus */
#fs-topbar{position:absolute;top:0;left:0;right:0;z-index:10;
           display:flex;gap:6px;padding:8px;
           background:linear-gradient(to bottom,#000000bb,transparent)}
.fsbtn{flex:1;padding:8px 4px;border-radius:8px;
       border:1px solid rgba(255,255,255,0.25);
       background:transparent;color:#ffffffcc;
       font-family:inherit;font-size:10px;letter-spacing:1px;cursor:pointer;transition:all .12s}
.fsbtn.active{border-color:var(--accent);color:var(--accent);background:#f9731622}
#fs-close{padding:8px 12px;border-radius:8px;
          border:1px solid rgba(255,255,255,0.25);
          background:transparent;color:#ffffffcc;
          font-family:inherit;font-size:14px;cursor:pointer}

/* HUD infos haut gauche */
#fs-hud{position:absolute;top:56px;left:8px;z-index:10;
        display:flex;flex-direction:column;gap:4px;pointer-events:none}
.fs-hud-item{background:#00000066;border:1px solid rgba(255,255,255,0.15);
             border-radius:6px;padding:3px 8px;font-size:10px;
             letter-spacing:1px;color:#ffffffcc}
.fs-hud-item span{color:var(--accent);font-weight:700}

/* Contrôles bas - horizontal, transparent */
#fs-bottom{position:absolute;bottom:0;left:0;right:0;z-index:10;
           display:flex;flex-direction:row;justify-content:space-between;
           padding:8px 16px;
           background:linear-gradient(to top,#000000bb,transparent)}
.fs-dpad{display:grid;grid-template-columns:repeat(3,52px);
         grid-template-rows:repeat(3,52px);gap:4px}
.fs-dpad .arrow{background:transparent;border:1px solid rgba(255,255,255,0.25);
                border-radius:var(--r);display:flex;align-items:center;
                justify-content:center;font-size:22px;cursor:pointer;
                color:#ffffffcc;transition:all .1s;
                user-select:none;-webkit-user-select:none;touch-action:none}
.fs-dpad .arrow.dir.pressed,.fs-dpad .arrow.dir:active{
  background:#f9731644;border-color:var(--accent);color:var(--accent);transform:scale(0.91)}
.fs-dpad .arrow.cam.pressed,.fs-dpad .arrow.cam:active{
  background:#38bdf844;border-color:var(--cam);color:var(--cam);transform:scale(0.91)}
.fs-dpad .arrow.empty{background:transparent;border:none;pointer-events:none}



</style>
</head>
<body>
<div id="app">

  <!-- HEADER -->
  <div id="header">
    <div>
      <div class="logo">HEXA<span>RESCUE</span></div>
      <div id="gps-mini">LAT: -- | LNG: -- | ALT: --m</div>
    </div>
    <div id="ws-pill">
      <div id="ws-led"></div>
      <span id="ws-txt">DECONNECTE</span>
    </div>
  </div>

  <!-- ETATS -->
  <div id="states">
    <div class="state-item">
      <div class="led" id="led-robot"></div>
      <div class="state-label">Robot</div>
      <div class="state-val" id="val-robot">--</div>
    </div>
    <div class="sep"></div>
    <div class="state-item">
      <div class="led" id="led-dist"></div>
      <div class="state-label">Distance</div>
      <div class="state-val" id="val-dist">-- mm</div>
    </div>
    <div class="sep"></div>
    <div class="state-item">
      <div class="led" id="led-thermal"></div>
      <div class="state-label">Thermal</div>
      <div class="state-val" id="val-thermal">-- °C</div>
    </div>
    <div class="sep"></div>
    <div class="state-item">
      <div class="led" id="led-ping"></div>
      <div class="state-label">Ping</div>
      <div class="state-val" id="val-ping">-- ms</div>
    </div>
  </div>

  <!-- CONTROLES -->
  <div id="controls">
    <button class="btn start"   onclick="send({type:'cmd',cmd:'O'})">START</button>
    <button class="btn neutral" onclick="send({type:'cmd',cmd:'N'})">NEUTRE</button>
    <button class="btn stop"    onclick="send({type:'cmd',cmd:'S'})">STOP</button>
  </div>

  <!-- VIDEO + THERMIQUE -->
<div class="panel">
  <div class="pt">Visualisation</div>

          <!-- FULLSCREEN OVERLAY -->
  <div style="position:relative">
    <button id="fs-btn" onclick="openFullscreen()">⛶</button>
  </div>

  <!-- Barre de modes -->
  <div id="view-bar">
    <button class="vbtn active" onclick="setMode('logitech')">📷 LOGITECH</button>
    <button class="vbtn"        onclick="setMode('thermal')"> 🌡 THERMAL</button>
    <button class="vbtn"        onclick="setMode('fusion')">  🔀 FUSION</button>
  </div>

  <!-- Conteneur vidéo Logitech -->
  <div id="view-logitech">
    <img id="jetson-video"
         src="http://192.168.4.3:8080/stream"
         style="width:100%;border-radius:8px;border:1px solid var(--border);">
  </div>

  <!-- Canvas thermique 8x8 interpolé -->
  <div id="view-thermal" style="display:none">
    <canvas id="thermal-canvas" width="256" height="256"
            style="width:100%;border-radius:8px;image-rendering:pixelated;
                   border:1px solid var(--border)"></canvas>
    <div style="display:flex;align-items:center;gap:8px;margin-top:6px">
  <span id="t-min" style="font-size:9px;color:var(--muted);width:36px;text-align:right">--°C</span>
  <canvas id="thermal-legend-bar" width="180" height="14"
          style="flex:1;border-radius:4px;height:14px"></canvas>
  <span id="t-max" style="font-size:9px;color:var(--muted);width:36px">--°C</span>
</div>
<div style="text-align:center;font-size:9px;color:var(--muted);margin-top:2px">
  moy: <span id="t-avg">--°C</span>
</div>
  </div>

  <!-- Fusion : vidéo + overlay canvas -->
  <div id="view-fusion" style="display:none;position:relative">
    <img src="http://192.168.4.3:8080/stream"
         style="width:100%;border-radius:8px;border:1px solid var(--border);">
    <canvas id="fusion-canvas" width="256" height="256"
            style="position:absolute;top:0;left:0;width:100%;height:100%;
                   border-radius:8px;opacity:0.45;image-rendering:pixelated"></canvas>
  </div>
</div>

  <!-- PADS -->
  <div id="pads-row">

    <!-- Direction -->
    <div class="panel">
      <div class="pt">Direction</div>
      <div class="dpad">
        <div class="arrow empty"></div>
        <div class="arrow dir" data-type="dir" data-cmd="z">▲</div>
        <div class="arrow empty"></div>
        <div class="arrow dir" data-type="dir" data-cmd="q">◀</div>
        <div class="arrow empty"></div>
        <div class="arrow dir" data-type="dir" data-cmd="d">▶</div>
        <div class="arrow empty"></div>
        <div class="arrow dir" data-type="dir" data-cmd="s">▼</div>
        <div class="arrow empty"></div>
      </div>
    </div>

    <!-- Camera -->
    <div class="panel">
      <div class="pt">Camera</div>
      <div class="dpad">
        <div class="arrow empty"></div>
        <div class="arrow cam" data-type="cam" data-axis="pitch" data-dir="-1">▲</div>
        <div class="arrow empty"></div>
        <div class="arrow cam" data-type="cam" data-axis="yaw" data-dir="1">◀</div>
        <div class="arrow empty"></div>
        <div class="arrow cam" data-type="cam" data-axis="yaw" data-dir="-1">▶</div>
        <div class="arrow empty"></div>
        <div class="arrow cam" data-type="cam" data-axis="pitch" data-dir="1">▼</div>
        <div class="arrow empty"></div>
      </div>
      <div class="cam-pos">
        <div>Yaw <span id="cam-yaw-val">0°</span></div>
        <div>Pitch <span id="cam-pitch-val">0°</span></div>
      </div>
      <button class="btn-center" onclick="centerCamera()">⊙ CENTRER</button>
    </div>

  </div>

  <!-- SLIDERS -->
  <div class="panel">
    <div class="pt">Parametres</div>
    <div class="slider-row">
      <span class="sl">Vitesse cycle</span>
      <input type="range" min="1000" max="2000" value="1500" step="50" oninput="sc('V',this,'ms')"/>
      <span class="sv" id="v-V">1500 ms</span>
    </div>
    <div class="slider-row">
      <span class="sl">Hauteur pas</span>
      <input type="range" min="70" max="150" value="80" step="1" oninput="sc('U',this,'mm')"/>
      <span class="sv" id="v-U">80 mm</span>
    </div>
    <div class="slider-row">
      <span class="sl">Hauteur corps</span>
      <input type="range" min="80" max="200" value="120" step="1" oninput="sc('H',this,'mm')"/>
      <span class="sv" id="v-H">120 mm</span>
    </div>
  </div>

  <!-- VICTIMS TABLE -->
  <div class="panel" id="victims-panel">
    <div class="pt">Victimes detectees</div>
    <div id="victims-list"></div>
  </div>

</div>

<!-- POPUP -->
<div id="popup-overlay">
  <div id="popup">
    <div class="popup-header">
      <div class="popup-icon">🚨</div>
      <div>
        <div class="popup-title">VICTIME DETECTEE</div>
        <div class="popup-sub" id="popup-num">VICTIME #1</div>
      </div>
    </div>
    <div class="popup-body">
      <div class="popup-line">
        <span class="popup-key">Heure</span>
        <span class="popup-val" id="popup-time">--:--:--</span>
      </div>
      <div class="popup-line">
        <span class="popup-key">Temp. max</span>
        <span class="popup-val" id="popup-temp">-- °C</span>
      </div>
      <div class="popup-line">
        <span class="popup-key">GPS</span>
        <span class="popup-val gps" id="popup-gps">En attente GPS...</span>
      </div>
    </div>
    <button id="popup-ok" onclick="closePopup()">CONFIRMER</button>
  </div>
</div>

<script>
// ---- WebSocket ----
let ws, pingStart;
let currentGPS = null;
const wsLed = document.getElementById('ws-led');
const wsTxt = document.getElementById('ws-txt');

function connect(){
  ws = new WebSocket('ws://192.168.4.1:81');
  ws.onopen  = ()=>{ wsLed.className='ok'; wsTxt.textContent='CONNECTE'; startPing(); };
  ws.onclose = ()=>{
    wsLed.className=''; wsTxt.textContent='DECONNECTE';
    setLed('led-ping',''); setVal('val-ping','-- ms');
    setTimeout(connect, 2000);
  };
  ws.onmessage = (e)=>{
    try{
      const d = JSON.parse(e.data);
      if(d.type==='pong'){
        const ms = Date.now()-pingStart;
        setLed('led-ping', ms<100?'green':ms<300?'yellow':'red');
        setVal('val-ping', ms+' ms');
        return;
      }
      if(d.type==='telem') updateTelem(d);
    }catch(_){}
  };
}
function send(obj){ if(ws&&ws.readyState===1) ws.send(JSON.stringify(obj)); }
function startPing(){ setInterval(()=>{ pingStart=Date.now(); send({type:'ping'}); }, 1000); }
function setLed(id,cls){ document.getElementById(id).className='led '+cls; }
function setVal(id,txt){ document.getElementById(id).textContent=txt; }

// ---- Telemetrie ----
let lastBodyState = false;

function updateTelem(d){

  // GPS temps réel dans le header
  if(d.gpsValid){
    currentGPS =
      d.gpsLat.toFixed(6)+', '+d.gpsLng.toFixed(6)+
      ' | ALT '+d.gpsAlt.toFixed(1)+'m';
    document.getElementById('gps-mini').textContent =
      'LAT: '+d.gpsLat.toFixed(6)+
      ' | LNG: '+d.gpsLng.toFixed(6)+
      ' | ALT: '+d.gpsAlt.toFixed(1)+'m';
    // Mise à jour live dans le popup si ouvert
    if(popupOpen)
      document.getElementById('popup-gps').textContent = currentGPS;
  }

  // Robot
  if(d.robotEnabled!==undefined){
    if(!d.robotEnabled){ setLed('led-robot',''); setVal('val-robot','OFF'); }
    else if(d.robotNeutral){ setLed('led-robot','yellow'); setVal('val-robot','NEUTRE'); }
    else { setLed('led-robot','green'); setVal('val-robot','ON'); }
  }

  // Distance
  if(d.distance!==undefined){
    const v=d.distance;
    if(v<=0||v>1000){ setLed('led-dist','');       setVal('val-dist','-- mm'); }
    else if(v>500)  { setLed('led-dist','yellow'); setVal('val-dist',v+' mm'); }
    else            { setLed('led-dist','red');    setVal('val-dist',v+' mm'); }
  }

  // Thermal
  if(d.tempValid!==undefined){
    if(!d.tempValid){ setLed('led-thermal',''); setVal('val-thermal','ERR'); }
    else{
      setVal('val-thermal', d.tempMax.toFixed(1)+' °C');
      setLed('led-thermal', d.bodyDetected?'red':'green');
    }
  }

 // Détection victime — YOLO + confirmation thermique
if(d.yoloDetected !== undefined || d.bodyDetected !== undefined){
  lastYoloState = d.yoloDetected || false;
  lastBodyState = d.bodyDetected || false;
  
  if(lastYoloState && lastBodyState && !popupOpen)
    triggerVictim(d.tempMax, 'YOLO + Thermique');
}

  if(d.grid && (currentMode==='thermal' || currentMode==='fusion')){
  drawThermal(d.grid, currentMode==='thermal' ? 'thermal-canvas' : 'fusion-canvas');
}
}

// ---- Popup victime ----
let victimCount=0, popupOpen=false, pendingVictim=null;

function triggerVictim(tempMax){
  victimCount++;
  const time = new Date().toTimeString().slice(0,8);
  const gpsStr = currentGPS ? currentGPS : 'En attente GPS...';
  document.getElementById('popup-num').textContent  = 'VICTIME #'+victimCount;
  document.getElementById('popup-time').textContent = time;
  document.getElementById('popup-temp').textContent = tempMax ? tempMax.toFixed(1)+' °C' : '-- °C';
  document.getElementById('popup-gps').textContent  = gpsStr;
  pendingVictim = { num:victimCount, time:time, gps:gpsStr };
  popupOpen = true;
  document.getElementById('popup-overlay').className = 'visible';
}

function closePopup(){
  document.getElementById('popup-overlay').className = '';
  popupOpen = false;
  if(pendingVictim) addVictimRow(pendingVictim);
  pendingVictim = null;
}

function addVictimRow(v){
  const panel = document.getElementById('victims-panel');
  const list  = document.getElementById('victims-list');
  const row   = document.createElement('div');
  row.className = 'victim-row';
  row.innerHTML =
    '<div class="victim-num">#'+v.num+'</div>'+
    '<div class="victim-info">'+
      '<div class="victim-time">'+v.time+'</div>'+
      '<div class="victim-gps">'+v.gps+'</div>'+
    '</div>';
  list.appendChild(row);
  panel.classList.add('visible');
  panel.scrollIntoView({behavior:'smooth', block:'end'});
}

// ---- Sliders ----
function sc(key,el,unit){
  document.getElementById('v-'+key).textContent = el.value+' '+unit;
  send({type:'param', key:key, value:parseInt(el.value)});
}

// ---- D-PAD Direction ----
let activeDirBtn = null;
function pressDirBtn(cmd,el){
  if(activeDirBtn&&activeDirBtn!==el) activeDirBtn.classList.remove('pressed');
  activeDirBtn=el; el.classList.add('pressed');
  send({type:'cmd', cmd:cmd});
}
function releaseDirBtn(){
  if(activeDirBtn){ activeDirBtn.classList.remove('pressed'); activeDirBtn=null; }
  send({type:'cmd', cmd:'N'});
}

// ---- Camera ----
let camYaw=0, camPitch=0;
const YAW_MAX=45, YAW_MIN=-45, PITCH_MAX=45, PITCH_MIN=-45;
const CAM_SPEED=40, CAM_INTERVAL=50;
let camInterval=null, activeCamEl=null, activeCamAxis=null, activeCamDir=0;

function degToUs(deg, minDeg, maxDeg){
  return Math.round(1000 + ((deg-minDeg)/(maxDeg-minDeg))*1000);
}

function updateCamDisplay(){
  document.getElementById('cam-yaw-val').textContent   = camYaw.toFixed(0)+'°';
  document.getElementById('cam-pitch-val').textContent = camPitch.toFixed(0)+'°';
}

function sendCamPos(isCenter){
  const usYaw   = degToUs(camYaw,   YAW_MIN,   YAW_MAX);
  const usPitch = degToUs(camPitch, PITCH_MIN, PITCH_MAX);
  send({type:'cam', yaw:usYaw, pitch:usPitch, center: isCenter||false});
}

function startCamMove(axis,dir,el){
  if(activeCamEl&&activeCamEl!==el) activeCamEl.classList.remove('pressed');
  activeCamEl=el; el.classList.add('pressed');
  activeCamAxis=axis; activeCamDir=dir;
  if(camInterval) clearInterval(camInterval);
  camInterval = setInterval(()=>{
    const step = activeCamDir * CAM_SPEED * (CAM_INTERVAL/1000);
    if(activeCamAxis==='yaw')   camYaw   = Math.max(YAW_MIN,   Math.min(YAW_MAX,   camYaw+step));
    else                        camPitch = Math.max(PITCH_MIN, Math.min(PITCH_MAX, camPitch+step));
    updateCamDisplay();
    sendCamPos(false);
  }, CAM_INTERVAL);
}

function stopCamMove(){
  if(camInterval){ clearInterval(camInterval); camInterval=null; }
  if(activeCamEl){ activeCamEl.classList.remove('pressed'); activeCamEl=null; }
  activeCamAxis=null; activeCamDir=0;
}

function centerCamera(){
  camYaw=0; camPitch=0;
  updateCamDisplay();
  sendCamPos(true);
}

// ---- Bindings ----
document.querySelectorAll('.arrow[data-type="dir"]').forEach(el=>{
  const cmd=el.dataset.cmd;
  el.addEventListener('touchstart',e=>{ e.preventDefault(); pressDirBtn(cmd,el); },{passive:false});
  el.addEventListener('touchend',  e=>{ e.preventDefault(); releaseDirBtn(); },   {passive:false});
  el.addEventListener('mousedown', ()=>pressDirBtn(cmd,el));
});

document.querySelectorAll('.arrow[data-type="cam"]').forEach(el=>{
  const axis=el.dataset.axis, dir=parseInt(el.dataset.dir);
  el.addEventListener('touchstart',e=>{ e.preventDefault(); startCamMove(axis,dir,el); },{passive:false});
  el.addEventListener('touchend',  e=>{ e.preventDefault(); stopCamMove(); },{passive:false});
  el.addEventListener('mousedown', ()=>startCamMove(axis,dir,el));
});

window.addEventListener('mouseup', ()=>{ releaseDirBtn(); stopCamMove(); });

// ---- Emotes ----
function sendEmote(name){
  send({type:'emote', name:name});
}

// ---- POLLING IA JETSON ----
setInterval(async () => {
  try {
    const res = await fetch("http://192.168.4.2:8080/status");
    const data = await res.json();
    const alertBox = document.getElementById("jetson-alert");
    if (data.detected === true) {
      alertBox.style.display = "block";
    } else {
      alertBox.style.display = "none";
    }
  } catch (e) {
    // Jetson non joignable : on ne spam pas la console
  }
}, 300);


  // ---- Modes visualisation ----
  let currentMode = 'logitech';
  function setMode(mode){
    currentMode = mode;
    document.getElementById('view-logitech').style.display = mode==='logitech' ? 'block' : 'none';
    document.getElementById('view-thermal').style.display  = mode==='thermal'  ? 'block' : 'none';
    document.getElementById('view-fusion').style.display   = mode==='fusion'   ? 'block' : 'none';
    document.querySelectorAll('.vbtn').forEach((b,i)=>{
      b.classList.toggle('active', ['logitech','thermal','fusion'][i]===mode);
    });
  }

  // ---- Rendu thermique ----
function tempToColor(t, tMin, tMax) {
  const r = Math.max(0, Math.min(1, (t - tMin) / (tMax - tMin)));
  // Palette fixe : bleu froid → cyan → vert → jaune → rouge chaud
  let R, G, B;
  if      (r < 0.25) { R = 0;                                    G = Math.round(r / 0.25 * 255);         B = 255; }
  else if (r < 0.5)  { R = 0;                                    G = 255;                                B = Math.round((0.5 - r) / 0.25 * 255); }
  else if (r < 0.75) { R = Math.round((r - 0.5) / 0.25 * 255);  G = 255;                                B = 0; }
  else               { R = 255;                                   G = Math.round((1 - r) / 0.25 * 255);  B = 0; }
  return [R, G, B];
}

  function tempToColor(t, tMin, tMax) {
  const r = Math.max(0, Math.min(1, (t - tMin) / (tMax - tMin)));
  let R, G, B;
  if      (r < 0.25) { R = 0;                                   G = Math.round(r / 0.25 * 255);        B = 255; }
  else if (r < 0.5)  { R = 0;                                   G = 255;                               B = Math.round((0.5 - r) / 0.25 * 255); }
  else if (r < 0.75) { R = Math.round((r - 0.5) / 0.25 * 255); G = 255;                               B = 0; }
  else               { R = 255;                                  G = Math.round((1 - r) / 0.25 * 255); B = 0; }
  return [R, G, B];
}

  function drawThermal(grid, canvasId) {
  const canvas = document.getElementById(canvasId);
  if (!canvas) return;
  const ctx = canvas.getContext('2d');
  const W = 256, H = 256;

  const tMin = Math.min(...grid);  // dynamique
  const tMax = Math.max(...grid);  // dynamique

  const imgData = ctx.createImageData(W, H);
  const px = imgData.data;

  for (let py = 0; py < H; py++) {
    for (let px2 = 0; px2 < W; px2++) {
      const gx = (px2 / (W - 1)) * 7;
      const gy = (py / (H - 1)) * 7;

      const x0 = Math.floor(gx), x1 = Math.min(x0 + 1, 7);
      const y0 = Math.floor(gy), y1 = Math.min(y0 + 1, 7);
      const fx = gx - x0, fy = gy - y0;

      const t00 = grid[y0 * 8 + x0];
      const t10 = grid[y0 * 8 + x1];
      const t01 = grid[y1 * 8 + x0];
      const t11 = grid[y1 * 8 + x1];
      const t = t00 * (1-fx)*(1-fy) + t10 * fx*(1-fy)
              + t01 * (1-fx)*fy     + t11 * fx*fy;

      const [R, G, B] = tempToColor(t, tMin, tMax);
      const idx = (py * W + px2) * 4;
      px[idx]     = R;
      px[idx + 1] = G;
      px[idx + 2] = B;
      px[idx + 3] = 255;
    }
  }
  ctx.putImageData(imgData, 0, 0);

  if (canvasId === 'thermal-canvas') {
    const avg = grid.reduce((a, b) => a + b, 0) / 64;
    document.getElementById('t-min').textContent = tMin.toFixed(1) + '°C';
    document.getElementById('t-max').textContent = tMax.toFixed(1) + '°C';
    document.getElementById('t-avg').textContent = 'moy: ' + avg.toFixed(1) + '°C';
  }
}



// ---- FULLSCREEN ----
let fsMode = 'logitech';

function openFullscreen(){
  const ov = document.getElementById('fs-overlay');
  ov.classList.add('visible');

  if(screen.orientation && screen.orientation.lock){
    screen.orientation.lock('landscape').catch(()=>{});
  }

  // Rebind flèches
  document.querySelectorAll('#fs-overlay .arrow[data-type="dir"]').forEach(el=>{
    el.replaceWith(el.cloneNode(true)); // reset listeners
  });
  document.querySelectorAll('#fs-overlay .arrow[data-type="dir"]').forEach(el=>{
    const cmd=el.dataset.cmd;
    el.addEventListener('touchstart',e=>{ e.preventDefault(); pressDirBtn(cmd,el); },{passive:false});
    el.addEventListener('touchend',  e=>{ e.preventDefault(); releaseDirBtn(); },{passive:false});
    el.addEventListener('mousedown', ()=>pressDirBtn(cmd,el));
  });
  document.querySelectorAll('#fs-overlay .arrow[data-type="cam"]').forEach(el=>{
    el.replaceWith(el.cloneNode(true));
  });
  document.querySelectorAll('#fs-overlay .arrow[data-type="cam"]').forEach(el=>{
    const axis=el.dataset.axis, dir=parseInt(el.dataset.dir);
    el.addEventListener('touchstart',e=>{ e.preventDefault(); startCamMove(axis,dir,el); },{passive:false});
    el.addEventListener('touchend',  e=>{ e.preventDefault(); stopCamMove(); },{passive:false});
    el.addEventListener('mousedown', ()=>startCamMove(axis,dir,el));
  });

  fsSetMode(currentMode);
}

function closeFullscreen(){
  const ov = document.getElementById('fs-overlay');
  ov.classList.remove('visible');
  ov.style.transform = '';
  ov.style.width = ov.style.height = '';
  ov.style.top = ov.style.left = '';
  ov.style.marginLeft = ov.style.marginTop = '';
  screen.orientation && screen.orientation.unlock && screen.orientation.unlock();
}

function fsSetMode(mode){
  fsMode = mode;
  document.getElementById('fs-logitech-cam').style.display   = mode==='logitech' ? 'block' : 'none';
  document.getElementById('fs-thermal-canvas').style.display = mode==='thermal'  ? 'block' : 'none';
  document.getElementById('fs-fusion-cam').style.display     = mode==='fusion'   ? 'block' : 'none';
  document.getElementById('fs-fusion-overlay').style.display = mode==='fusion'   ? 'block' : 'none';
  document.querySelectorAll('.fsbtn').forEach((b,i)=>{
    b.classList.toggle('active', ['logitech','thermal','fusion'][i]===mode);
  });
}

// Mise à jour HUD fullscreen depuis updateTelem
const _origUpdateTelem2 = updateTelem;
updateTelem = function(d){
  _origUpdateTelem2(d);
  if(d.gpsValid) document.getElementById('fs-gps').textContent =
    d.gpsLat.toFixed(5)+', '+d.gpsLng.toFixed(5);
  if(d.distance!==undefined && d.distance>0)
    document.getElementById('fs-dist').textContent = d.distance+' mm';
  if(d.tempValid) document.getElementById('fs-temp').textContent = d.tempMax.toFixed(1)+' °C';
  if(d.grid && (fsMode==='thermal'||fsMode==='fusion')){
    drawThermal(d.grid, fsMode==='thermal' ? 'fs-thermal-canvas' : 'fs-fusion-overlay');
  }
};

connect();
updateCamDisplay();
</script>

<!-- FOOTER EMOTES -->
<div id="footer">
  <button class="emote-btn hello" onclick="sendEmote('hello')"> Hello </button>
  <button class="emote-btn twist" onclick="sendEmote('twist')"> Twist </button>
  <button class="emote-btn jump" onclick="sendEmote('jump')"> Jump </button>
</div>




<!-- FULLSCREEN OVERLAY -->
<div id="fs-overlay">
  <!-- Barre du haut -->
  <div id="fs-topbar">
    <button class="fsbtn active" onclick="fsSetMode('logitech')">📷 LOGITECH</button>
    <button class="fsbtn"        onclick="fsSetMode('thermal')">🌡 THERMAL</button>
    <button class="fsbtn"        onclick="fsSetMode('fusion')">🔀 FUSION</button>
    <button id="fs-close"        onclick="closeFullscreen()">✕</button>
  </div>

  <!-- Vue principale -->
  <div id="fs-main">
    <!-- Logitech -->
    <img id="fs-logitech-cam" src="http://192.168.4.3:8080/stream" style="display:block">
    <!-- Thermal -->
    <canvas id="fs-thermal-canvas" width="256" height="256" style="display:none"></canvas>
    <!-- Fusion -->
    <img id="fs-fusion-cam" src="http://192.168.4.3:8080/stream" style="display:none">
    <canvas id="fs-fusion-overlay" width="256" height="256" style="display:none"></canvas>

    <!-- HUD infos -->
    <div id="fs-hud">
      <div class="fs-hud-item">📍 <span id="fs-gps">--</span></div>
      <div class="fs-hud-item">📏 <span id="fs-dist">-- mm</span></div>
      <div class="fs-hud-item">🌡 <span id="fs-temp">-- °C</span></div>
    </div>
  </div>

  <!-- Contrôles bas -->
  <div id="fs-bottom">
    <!-- Direction -->
    <div class="fs-dpad">
      <div class="arrow empty"></div>
      <div class="arrow dir" data-type="dir" data-cmd="z">▲</div>
      <div class="arrow empty"></div>
      <div class="arrow dir" data-type="dir" data-cmd="q">◀</div>
      <div class="arrow empty"></div>
      <div class="arrow dir" data-type="dir" data-cmd="d">▶</div>
      <div class="arrow empty"></div>
      <div class="arrow dir" data-type="dir" data-cmd="s">▼</div>
      <div class="arrow empty"></div>
    </div>
    <!-- Camera -->
    <div class="fs-dpad">
      <div class="arrow empty"></div>
      <div class="arrow cam" data-type="cam" data-axis="pitch" data-dir="-1">▲</div>
      <div class="arrow empty"></div>
      <div class="arrow cam" data-type="cam" data-axis="yaw" data-dir="1">◀</div>
      <div class="arrow empty"></div>
      <div class="arrow cam" data-type="cam" data-axis="yaw" data-dir="-1">▶</div>
      <div class="arrow empty"></div>
      <div class="arrow cam" data-type="cam" data-axis="pitch" data-dir="1">▼</div>
      <div class="arrow empty"></div>
    </div>
  </div>
</div>

</body>
</html>
)rawliteral";