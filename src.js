var b = `WASM_HERE`;
var SIZE = 500;
var Wasm = WebAssembly;

Wasm.instantiateStreaming(fetch("data:application/wasm," + b)).then(a => {
  let imageData = new ImageData(SIZE, SIZE);
  for (let index = 0; index < SIZE * SIZE * 4; index++) {
    imageData.data[index] = a.instance.exports._r(index);
  }
  c.putImageData(imageData, 0, 0);
});
