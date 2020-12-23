var http = require("http");
var WebSocket = require("ws");
var express = require("express");
var app = express();
var { exec, execSync } = require("child_process");
var cors = require("cors");
var chroma = require("chroma-js");
// function gửi yêu cầu(response) từ phía server hoặc nhận yêu cầu (request) của client gửi lên
// create http server
var server = http.createServer(app);
var ws = new WebSocket.Server({
  server,
  path: "/ws",
});

var pid;

app.use(cors());
app.use(express.json());
app.get("/", (req, res) => {
  res.send(`<h1> hello world </h1>`);
});

app.post("/array", (req, res) => {
  const gradient = chroma.scale(req.body).colors(400);
  pid = exec(`node client.js -A ${JSON.stringify(gradient)}`).pid;
  res.send("ok");
});

app.post("/stop", (req, res) => {
  console.log(req.body, pid);
  res.send("ok");
  if (pid) {
    execSync(`kill -9 ${pid + 1}`);
    pid = undefined;
  }
  exec(
    `node client.js -S "${req.body.color}" -B ${req.body.brightness}`,
    (err, out, stderr) => {
      console.log(err);
    }
  );
});

app.post("/weather", (req, res) => {
  res.send("ok");
  if (pid) {
    execSync(`kill -9 ${pid + 1}`);
    pid = undefined;
  }
  console.log(req.body.location);
  exec(`bash weather.sh ${req.body.location}`);
});
ws.on("connection", function (socket) {
  socket.on("message", function (message) {
    ws.clients.forEach(function (client) {
      if (client !== socket && client.readyState === WebSocket.OPEN) {
        client.send(message);
      }
    });
  });
});
server.listen(3001);
console.log("Server listening on port 3001");
