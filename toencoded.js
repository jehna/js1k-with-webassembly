const { readFileSync } = require("fs");

console.log(readFileSync("./main.wasm").toString("base64"));
