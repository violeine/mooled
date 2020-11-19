const { exec } = require("child_process");
const { pid } = require("process");
exec("ps aux | grep 'htop'| awk '{print $2}'", (err, stdout, stderr) => {
  if (err) {
    console.error("exec error :", err);
    return;
  }
  stdout
    .split("\n")
    .filter((el) => el != pid)
    .forEach((el) => exec(`kill -9 ${el}`));
});
