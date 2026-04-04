const API_URL = "http://localhost:3000/api/command";

const outputEl = document.getElementById("output");
const lastCommandEl = document.getElementById("lastCommand");
const tempInputEl = document.getElementById("tempInput");
const setTempBtnEl = document.getElementById("setTempBtn");
const commandButtons = Array.from(document.querySelectorAll(".cmd-btn"));

async function sendCommand(command) {
  setLoadingState(true);
  outputEl.textContent = `> Sending request...`;

  try {
    const response = await fetch(API_URL, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ command }),
    });

    const payload = await response.json();

    if (!response.ok || !payload.ok) {
      const errMessage =
        payload.error || `${response.status}`;
      outputEl.textContent = `Error:\n${errMessage}`;
      return;
    }

    lastCommandEl.textContent = `Command: ${payload.command}`;
    outputEl.textContent = payload.response;
  } catch (err) {
    outputEl.textContent = `Network error:\n${err.message}`;
  } finally {
    setLoadingState(false);
  }
}

function setLoadingState(isLoading) {
  for (const button of commandButtons) {
    button.disabled = isLoading;
  }
}

for (const button of commandButtons) {
  const command = button.dataset.command;
  if (!command) {
    continue;
  }

  button.addEventListener("click", () => {
    sendCommand(command);
  });
}

setTempBtnEl.addEventListener("click", () => {
  const value = Number.parseInt(tempInputEl.value, 10);

  if (Number.isNaN(value)) {
    outputEl.textContent =
      "Please enter a numeric temperature value before pressing SET.";
    return;
  }

  sendCommand(`GET /thermostat/set/${value}`);
});
