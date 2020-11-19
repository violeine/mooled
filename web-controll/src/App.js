import "./App.css";
import { useState, useEffect, useRef } from "react";

const WS_URI = `ws://${window.location.hostname}:3001/ws`;
function hexToRGB(h, brightness) {
  let r = 0,
    g = 0,
    b = 0;

  // 3 digits
  if (h.length == 4) {
    r = "0x" + h[1] + h[1];
    g = "0x" + h[2] + h[2];
    b = "0x" + h[3] + h[3];

    // 6 digits
  } else if (h.length == 7) {
    r = "0x" + h[1] + h[2];
    g = "0x" + h[3] + h[4];
    b = "0x" + h[5] + h[6];
  }

  return { red: +r, green: +g, blue: +b, brightness };
}

function App() {
  const [color, setColor] = useState(null);
  const [brightness, setBrightness] = useState(0);
  const ws = useRef(null);
  function connectWebsocket() {
    ws.current = new WebSocket(WS_URI);
    console.log("connecting...");
    ws.current.onopen = () => {
      console.log("connected");
    };
    ws.current.onclose = () => {
      console.log("closed");
    };
    return () => ws.current.close();
  }

  useEffect(connectWebsocket, []);
  return (
    <div className="App">
      <h2>{color}</h2>
      <input
        type="color"
        value={color}
        onChange={(e) => {
          setColor(e.target.value);
          ws.current.send(JSON.stringify(hexToRGB(e.target.value, brightness)));
        }}
      />
      <br />
      <h2>Brightness {brightness}</h2>
      <input
        type="range"
        id="vol"
        name="vol"
        min="0"
        max="255"
        value={brightness}
        onChange={(e) => {
          setBrightness(e.target.value);
          ws.current.send(JSON.stringify(hexToRGB(color, e.target.value)));
        }}
      />
    </div>
  );
}

export default App;
