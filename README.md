# js1k with WebAssembly

> Trying out WebAssembly

[js1k](http://js1k.com/) is a coding competition that limits the size of your
code to maximum of 1kb. This repository has an attempt of cracking that size
limit using WebAssembly

## See it live!

https://jehna.github.io/js1k-with-webassembly/

## Run on your local machine

You can run the project by running

```shell
yarn
yarn dev
```

This fetches the dependencies, builds the C++ code to webassembly and starts a
simple http server that serves the js1k "chrome".

Note that you should have Emscripten installed and available at your path.

## Features

This js1k demo currently

- Draws a mountain view on html canvas
- Compiles c++ code to Webassembly
- Has a development pipeline to code in c, c++ or .wat and compile it down to
  binary webassembly, which is base64 encoded to run in a js1k entry.

## Licensing

The code in this project is licensed under MIT license.
