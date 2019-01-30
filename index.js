const b = "AGFzbQEAAAABBQFgAAF/AwIBAAcFAQFyAAAKBgEEAEEqCw==";
const r = console.log;

const run = `W=WebAssembly,W.compile(Uint8Array.from(atob(b),c=>c.charCodeAt(0))).then(W.instantiate).then(m=>r(m.exports.r()))`;

console.log(`Current chrome: ${run.length} bytes`);
console.log(`Current code: ${b.length} bytes`);
console.log(`Total: ${run.length + b.length} bytes`);

eval(run);
