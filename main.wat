(module
  (func $pow (param $value i32) (result i32)
    (i32.mul
      (get_local $value)
      (get_local $value)))
  (func $r (param $x i32) (param $y i32) (result i32)
    (if (result i32)
      (i32.lt_u
        (i32.add
          (call $pow (i32.sub (get_local $x) (i32.const 100)))
          (call $pow (i32.sub (get_local $y) (i32.const 100))))
        (i32.const 1000))
      (then (i32.const 0))
      (else (i32.const 255))))
  (export "r" (func $r))
)