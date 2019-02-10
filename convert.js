const { readFileSync, writeFileSync } = require("fs");
/*var wabt = require("wabt")();

const inputWat = "main.wat";
const outputWasm = "main.wasm";

const wasmModule = wabt.parseWat(inputWat, readFileSync(inputWat, "utf8"));
const { buffer } = wasmModule.toBinary({});

const base = Buffer.from(buffer).toString("base64");*/
const base = Buffer.from(readFileSync("./main.wasm")).toString("utf-8");
writeFileSync(
  "./source.js",
  readFileSync("./source.js")
    .toString()
    .replace("WASM_HERE", base)
);
