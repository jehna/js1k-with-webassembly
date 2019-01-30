(module
  (func $r (param $x i32) (param $y i32) (result i32)
    (if (result i32)
      (i32.lt_u
        (get_local $x)
        (get_local $y))
      (then (i32.const 0))
      (else (i32.const 255))))
  (export "r" (func $r))
)