const WebSocket = require("ws");
const chroma = require("chroma-js");
const ws = new WebSocket("ws://violpi:3001/ws");

const { exec } = require("child_process");
const { pid } = require("process");
//kill all process except itself
exec("ps aux | grep 'node client.js'| awk '{print $2}'", (err, stdout, stderr) => {
  if (err) {
    console.error("exec error :", err);
    return;
  }
  stdout
    .split("\n")
    .filter((el) => el != pid)
    .forEach((el) => exec(`kill -9 ${el}`));
});

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

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

ws.onopen = () => {
  console.log("connect to ws success");
  sendColors();
};

const test = chroma.scale("RdYlBu").colors(400);

// test.forEach((el) => ws.send(JSON.stringify(hexToRGB(el))));

async function sendColors() {
  while (1) {
    for (let i = 0; i < test.length; i++) {
      ws.send(JSON.stringify(hexToRGB(test[i])));
      await sleep(50);
    }
    for (let i = test.length - 1; i >= 0; i--) {
      ws.send(JSON.stringify(hexToRGB(test[i])));
      await sleep(50);
    }
  }
}

// for (i = 0; i <= 100; i++) {
//   console.log(test(i));
// }
