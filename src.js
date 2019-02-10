var SIZE = 500;
var Wasm = WebAssembly;

Wasm.instantiateStreaming(fetch`data:application/wasm;base64,WASM_HERE`).then(
  a => {
    let imageData = new ImageData(SIZE, SIZE);
    for (let index = 0; index < SIZE * SIZE * 4; index++) {
      imageData.data[index] = a.instance.exports._r(index);
    }
    c.putImageData(imageData, 0, 0);
  }
);
