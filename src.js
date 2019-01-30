var b = "WASM_HERE";

var r = (x, y, color) => {
  c.fillStyle =
    "#" + color.toString(16) + color.toString(16) + color.toString(16);
  c.fillRect(x, y, 1, 1);
};
var Wasm = WebAssembly;

Wasm.compile(Uint8Array.from(atob(b), c => c.charCodeAt(0)))
  .then(compiled => Wasm.instantiate(compiled))
  .then(m => {
    for (let x = 0; x < 256; x++) {
      for (let y = 0; y < 256; y++) {
        r(x, y, m.exports.r(x, y));
      }
    }
  });
