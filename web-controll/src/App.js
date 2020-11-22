import "./App.css";
import { useState, useEffect, useRef } from "react";

const WS_URI = `ws://${window.location.hostname}:3001/ws`;
function hexToRGB(h) {
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

  return { red: +r, green: +g, blue: +b };
}

function App() {
  const [color, setColor] = useState("#00ffff");
  const [brightness, setBrightness] = useState(64);
  const [gradient, setGradient] = useState(["#00ffff", "#ff00ff"]);
  const [location, setLocaton] = useState("HoChiMinh");
  const [weather, setWeather] = useState("");
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
  const fetchFn = (payload, route) => {
    fetch(`http://${window.location.hostname}:3001/${route}`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json;charset=UTF-8",
      },
      body: payload,
    });
  };
  const sendFn = () => fetchFn(JSON.stringify(gradient), "array");
  const stopFn = () =>
    fetchFn(JSON.stringify({ color, brightness: +brightness }), "stop");
  const weatherFn = async () => {
    const result = await fetch(
      `https://wttr.in/${location}?format=%C`
    ).then((res) => res.text());
    fetchFn(JSON.stringify({ location }), "weather");
    setWeather(result);
  };
  useEffect(connectWebsocket, []);
  return (
    <div className="App">
      <h2>{color}</h2>
      <input
        type="color"
        value={color}
        onChange={(e) => {
          setColor(e.target.value);
          ws.current.send(JSON.stringify(hexToRGB(e.target.value)));
        }}
      />
      <hr />
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
          ws.current.send(
            JSON.stringify({
              brightness: +e.target.value,
            })
          );
        }}
      />
      <hr />
      <h2>gradient</h2>
      {gradient.map((el, idx) => {
        return (
          <input
            type="color"
            value={el}
            onChange={(e) => {
              const newArray = [...gradient];
              newArray[idx] = e.target.value;
              setGradient(newArray);
            }}
          />
        );
      })}
      <br />
      <button onClick={() => setGradient([...gradient, "#ffffff"])}>
        add new gradient
      </button>
      <button
        onClick={() => setGradient(gradient.slice(0, gradient.length - 1))}
      >
        remove gradient
      </button>
      <br />
      <button onClick={sendFn}>send</button>
      <button onClick={stopFn}> stop </button>
      <hr />
      <h2>weather</h2>
      <label>location</label> <br />
      <input
        type="text"
        value={location}
        onChange={(e) => setLocaton(e.target.value)}
      />
      <button onClick={weatherFn}> send </button>
      {weather ? <div>{weather}</div> : null}
    </div>
  );
}

export default App;
