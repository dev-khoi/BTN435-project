const express = require("express");
const cors = require("cors");
const net = require("net");

const app = express();

const HTTP_PORT = process.env.BRIDGE_PORT || 3000;
const TCP_HOST = process.env.TCP_HOST || "127.0.0.1";
const TCP_PORT = process.env.TCP_PORT || 8080;

const READ_IDLE_MS = 150;
// if no new data is found, throw error after this timeout
const SOCKET_TIMEOUT_MS = 3000;

app.use(cors());
app.use(express.json());


function sendTcpCommand(command) {
  return new Promise((resolve, reject) => {
    const socket = new net.Socket();
    let responseBuffer = "";
    let finished = false;
    let idleTimer = null;

    const finishSuccess = () => {
      if (finished) {
        return;
      }
      finished = true;
      if (idleTimer) {
        clearTimeout(idleTimer);
      }
      socket.destroy();
      resolve(responseBuffer);
    };

    const finishError = (message) => {
      if (finished) {
        return;
      }
      finished = true;
      if (idleTimer) {
        clearTimeout(idleTimer);
      }
      socket.destroy();
      reject(new Error(message));
    };

    const scheduleIdleFinish = () => {
      if (idleTimer) {
        clearTimeout(idleTimer);
      }
      idleTimer = setTimeout(finishSuccess, READ_IDLE_MS);
    };

    socket.setTimeout(SOCKET_TIMEOUT_MS);

    socket.connect(TCP_PORT, TCP_HOST, () => {
      socket.write(`${command.trim()}\n`);
    });

    socket.on("data", (chunk) => {
      responseBuffer += chunk.toString("utf8");
      scheduleIdleFinish();
    });

    socket.on("timeout", () => {
      if (responseBuffer.length > 0) {
        finishSuccess();
        return;
      }
      finishError("TCP server timeout");
    });

    socket.on("end", () => {
      finishSuccess();
    });

    socket.on("error", (err) => {
      finishError(`TCP connection error: ${err.message}`);
    });
  });
}

app.post("/api/command", async (req, res) => {
  const command = req.body && req.body.command;

  try {
    const response = await sendTcpCommand(command);
    res.json({
      ok: true,
      command: command.trim(),
      response
    });
  } catch (err) {
    res.status(502).json({
      ok: false,
      error: err.message
    });
  }
});

app.listen(HTTP_PORT, () => {
  console.log(`Bridge listening on http://localhost:${HTTP_PORT}`);
  console.log(`Forwarding TCP commands to ${TCP_HOST}:${TCP_PORT}`);
});
