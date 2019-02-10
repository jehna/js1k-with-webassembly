var b = "WASM_HERE";
var SIZE = 500;
var r = (x, y, color) => {
  const rr = ((color >> 16) & 255).toString(16);
  const gg = ((color >> 8) & 255).toString(16);
  const bb = ((color >> 0) & 255).toString(16);
  c.fillStyle = "#" + rr + gg + bb;
  c.fillRect(x, y, 1, 1);
};
var Wasm = WebAssembly;

Wasm.compile(Uint8Array.from(atob(b), c => c.charCodeAt(0)))
  .then(Wasm.instantiate)
  .then(m => {
    for (let x = 0; x < SIZE; x++) {
      for (let y = 0; y < SIZE; y++) {
        r(x, y, m.exports._r(x, y));
      }
    }
  });
