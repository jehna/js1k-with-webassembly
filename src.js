var b = "WASM_HERE";
var SIZE = 500;
var Wasm = WebAssembly;

Wasm.compile(Uint8Array.from(atob(b), c => c.charCodeAt(0)))
  .then(Wasm.instantiate)
  .then(m => {
    let imageData = new ImageData(SIZE, SIZE);
    for (let index = 0; index < SIZE * SIZE * 4; index++) {
      imageData.data[index] = m.exports._r(index);
    }
    c.putImageData(imageData, 0, 0);
  });
