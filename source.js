WebAssembly.instantiateStreaming(fetch("data:application/wasm;base64,AGFzbQEAAAABBgFgAX8BfwMCAQAHBgECX3IAAAq+BAG7BAIEfwh9IABB0A9tskM3mtk6lEPNE9S+kiIFQwAAAACSIABBBG0iA0H0A2+yQzea2TqUQ80T1L6SIgZDAAAAAJSSIgdDAACAPyAFQwAAAACUQy0tfr+SIAZDawL0PZSSIgggCJQgByAHlCAFQwAAAICUQ2kC9D2SIAZDLy1+P5SSIgUgBZSSkpGVIgaUIQcgCCAGlCIIQwAAUMGUIAdDAAAAAJQgBSAGlCIJQ5qZ2T+UkpIiBSAFlEPX4yLDkiEGQyhrbk4gBSAGkZMgBkMAAAAAXRsiBUMoa25OIAVDKGtuTl0bIgVDKGtuTlsEf0H/ASEBQf8BIQJB/wEFQwAAgD8gCCAFlEMAACBBkiIKQwAAQECSIgggCJQgByAFlEMAAAAAkiIGIAaUIAkgBZRDZmbmP5IiB0MAAGDAkiIJIAmUkpKRlSEFQwAAgD9DAACgQSAKkyIKIAqUQwAA8MEgBpMiCyALlEMAACDBIAeTIgwgDJSSkpGVIQdDAACAPyAIIAWUIAogB5SUIAYgBZQgCyAHlJQgCSAFlCAMIAeUlJKSIgVDAAAAAJJDAAAAACAFQwAAAABeGyIFIAVDAACAP14bQzMzMz+UIgVDAAB/Q5RDAABMQpJDAADMQZKoIQEgBUMAABtDlEMAAExCkkMAAHhBkqghAiAFQwAASEOUQwAATEKSQwAAoEGSqAshBAJAAkACQAJAAkAgACADQQJ0aw4EAAECAwQLIAEPCyAEDwsgAg8LQf8BDwtBAAs=")).then(a=>{let t=new ImageData(500,500);for(let e=0;e<1e6;e++)t.data[e]=a.instance.exports._r(e);c.putImageData(t,0,0)});