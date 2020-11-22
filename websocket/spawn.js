const { exec } = require("child_process");
const chroma = require("chroma-js");

const colors = chroma.scale("RdYlBu").colors(400);

exec(`node client.js -A ${JSON.stringify(colors)}`, (err, stdout, stderr) => {
  console.log(stdout);
});
